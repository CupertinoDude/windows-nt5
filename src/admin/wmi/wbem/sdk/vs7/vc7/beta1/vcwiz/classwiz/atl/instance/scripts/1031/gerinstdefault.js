// Script for WMI Instance provider wizard

function OnFinish(selProj, selObj)
{

	var oCM	= selProj.CodeModel;
	try
	{	
	
		oCM.StartTransaction("Add WMI Instance provider");
	
				
		var bDLL;
		if (typeDynamicLibrary == selProj.Object.Configurations(1).ConfigurationType) //REVIEW: hardcoded configuration
			bDLL = true;
		else
			bDLL = false;
				
		wizard.AddSymbol("DLL_APP", bDLL);
		
		var strProjectName 		= wizard.FindSymbol("PROJECT_NAME");
		var strProjectPath		= wizard.FindSymbol("PROJECT_PATH");
		var strTemplatePath		= wizard.FindSymbol("TEMPLATES_PATH");
		var strShortName		= wizard.FindSymbol("SHORT_NAME");
		var strUpperShortName	= strShortName.toUpperCase();
		wizard.AddSymbol("UPPER_SHORT_NAME", strUpperShortName);
				
		var strVIProgID			= wizard.FindSymbol("VERSION_INDEPENDENT_PROGID");
		wizard.AddSymbol("PROGID", strVIProgID + ".1");
 		var strClassName		= wizard.FindSymbol("CLASS_NAME");
		var strHeaderFile		= wizard.FindSymbol("HEADER_FILE");
		var strImplFile			= wizard.FindSymbol("IMPL_FILE");
		var strCoClass			= wizard.FindSymbol("COCLASS");
		var bAttributed	= wizard.FindSymbol("ATTRIBUTED");
		
		var strProjectCPP		= GetProjectFile(selProj, "CPP", false, false);
		var strProjectRC		= GetProjectFile(selProj, "RC", true, false);
		
		var bClassSpecified = (wizard.FindSymbol("WMICLASSNAME").toString() != "");
		wizard.AddSymbol ("CLASS_SPECIFIED", bClassSpecified);
		if (bClassSpecified)
		{
			//add "dynamic" and "provider" qualifiers, if necessary
			
			var locator = new ActiveXObject("WbemScripting.SWbemLocator");
			var services = locator.ConnectServer("", //local server
							wizard.FindSymbol("NAMESPACE"));												
			var classObj = services.Get(wizard.FindSymbol("WMICLASSNAME"));
			
			AddQualifier (classObj, 
						"provider", 
						wizard.FindSymbol("SHORT_NAME"),
						false,
						true,
						true);
							
			AddQualifier (classObj, 
						"dynamic", 
						true,
						false,
						true,
						true);
																							
		}
		
		// Create necessary GUIDS
		CreateGUIDs();
				
		if (!bAttributed)
		{
			// Get LibName
			wizard.AddSymbol("LIB_NAME", oCM.IDLLibraries(1).Name);
	
			// Get LibID
			wizard.AddSymbol("LIBID_REGISTRY_FORMAT", oCM.IDLLibraries(1).Attributes("uuid").Value);
		
			// Get AppID
			var strAppID = wizard.GetAppID();
			if (strAppID.length > 0)
			{
				wizard.AddSymbol("APPID_EXIST", true);
				wizard.AddSymbol("APPID_REGISTRY_FORMAT", strAppID);
			}
			
			// add RGS file resource
			var strRGSFile = GetUniqueFileName(strProjectPath, strShortName + ".rgs");
			var strRGSID = "IDR_" + strUpperShortName;
			RenderAddTemplate("wmiprov.rgs", strRGSFile, false);
				
			var oResHelper = wizard.ResourceHelper; //selProj.ResourceHelper;
			oResHelper.OpenResourceFile(strProjectRC);
			//oResHelper.AddResource(strRGSID, strProjectPath + "\\" + strRGSFile, "REGISTRY");
			oResHelper.AddResource(strRGSID, strProjectPath + strRGSFile, "REGISTRY");
			oResHelper.CloseResourceFile();	
												
			// Render wmiprovco.idl and insert into strProject.idl
			AddCoclassFromFile(oCM, "wmiprovco.idl");		
						
			SetMergeProxySymbol(selProj);
		
		}

		// Add #include "strHeaderFile" to strProjectName.cpp
		if (!DoesIncludeExist(selProj, '"' + strHeaderFile + '"', strProjectCPP))
			oCM.AddInclude('"' + strHeaderFile + '"', strProjectCPP, vsCMAddPositionEnd);


		// Add header
		RenderAddTemplate("wmiprov.h", strHeaderFile, selProj);
		
		// Add CPP
		RenderAddTemplate("wmiprov.cpp", strImplFile, selProj);
	
		//Add MOF
		AddMOFFromFile("wmiprov.mof", selProj);
		
		//add wmi libraries to linker input
		var nTotal = selProj.Object.Configurations.Count;
		var nCntr;		
		for (nCntr = 1; nCntr <= nTotal; nCntr++)
		{
					
			var VCLinkTool = selProj.Object.Configurations(nCntr).Tools("VCLinkerTool");
			if (-1 == VCLinkTool.AdditionalInputs.toUpperCase().search("WBEMUUID.LIB"))
			{
				VCLinkTool.AdditionalInputs += " wbemuuid.lib";
			}
			if (-1 == VCLinkTool.AdditionalInputs.toUpperCase().search("WMIUTILS.LIB"))
			{
				VCLinkTool.AdditionalInputs += " wmiutils.lib";
			}
		}
		
		oCM.CommitTransaction();
					
	}
	catch(e)
	{
		oCM.AbortTransaction();
		var L_ErrMsg1 = "Fehler in OnFinish: ";
		wizard.ReportError( L_ErrMsg1 + e.description);
	}
}

function AddMOFFromFile (strTemplateName, selProj)
{
	try
	{
	   	var ForReading = 1, ForWriting = 2, ForAppending = 8;
    	var TristateUseDefault = -2, TristateUnicode = -1, TristateAnsi = 0;
    
		var fso = new ActiveXObject("Scripting.FileSystemObject");
		var strMOFName = wizard.FindSymbol("PROJECT_NAME") + ".mof";
		
		var strMOFPath = wizard.FindSymbol("PROJECT_PATH") + strMOFName;		
		
		var bFirstRun = !fso.FileExists(strMOFPath);
			
		if (bFirstRun)
		{			
			//add MOF filter to "Source Files"					
			var sourceFolder = selProj.Object.Filters("Quelldateien");			
			sourceFolder.Filter += ";mof";	
						
			RenderAddTemplate("header.mof", strMOFName, selProj);
		}
		
		if (wizard.FindSymbol("CLASS_SPECIFIED") == true)
		{

			//create ESCAPED_NAMESPACE symbol that would have 2 backslashes in
			//complex namespace names
			var strEscapedNS = "";
			var strNS = wizard.FindSymbol("NAMESPACE");
			var arPieces = strNS.split("\\");				
			
			if (arPieces.length == 0)
			{
				strEscapedNS = strNS;
			}
			else 
			{			
				for (var i = 0; i < arPieces.length - 1; i++)
				{
					strEscapedNS +=	arPieces[i] + "\\\\";
				}										
				strEscapedNS += arPieces[arPieces.length - 1];			
			}
			
			wizard.AddSymbol("ESCAPED_NAMESPACE", strEscapedNS);	
		}
		
		var strTemp = wizard.FindSymbol("PROJECT_PATH") + strTemplateName;
		RenderAddTemplate(strTemplateName, strTemp, false);
				
		var fAddon = fso.OpenTextFile(strTemp, ForReading, false, TristateAnsi);	//ANSI for now, should be Unicode
		var strAddon = fAddon.ReadAll();		
		fAddon.Close();
					
		//now, add wmiprov.mof at the bottom
		var strMOFPath = wizard.FindSymbol("PROJECT_PATH") + strMOFName;		
		var fHeader = fso.GetFile(strMOFPath);
		var ts = fHeader.OpenAsTextStream(ForAppending, TristateAnsi);	//ANSI for now, should be Unicode
		ts.WriteBlankLines(1);		
		ts.Write(strAddon);	
		ts.Close();		
				
		fso.DeleteFile(strTemp);
		
		if (bFirstRun) 
		{
			//set custom build step for the MOF file in each configuration
			
			var files = selProj.Object.Files;
			var Moffile = files(strMOFName);

			var nTotal = Moffile.FileConfigurations.Count;			
			var nCntr;		
			for (nCntr = 1; nCntr <= nTotal; nCntr++)
			{				
				var customBuildTool = Moffile.FileConfigurations(nCntr).Tool;
								
				customBuildTool.CommandLine = "mofcomp \"$(InputDir)\\$(InputName)\".mof\n\recho mofcomptrace > \"$(OutDir)\\$(InputName)\".trace";
				
				customBuildTool.Outputs  = 	"\"$(OutDir)\\$(InputName)\".trace";	
				
				customBuildTool.Description = "Kompilierende MOF-Datei";				
								
			}			
		}								
	}
	catch(e)
	{
		wizard.ReportError("Fehler in AddMOFFile: " + e.description);
	}
}

function CreateGUIDs()
{
	try
	{
		// create CLSID
		var strRawGUID = wizard.CreateGuid();
		var strFormattedGUID = wizard.FormatGuid(strRawGUID, 0);
		wizard.AddSymbol("CLSID_REGISTRY_FORMAT", strFormattedGUID);

		
	}
	catch(e)
	{
		wizard.ReportError("Fehler in CreateGUIDs(): " + e.description);
	}
}

function AddQualifier (classObj, 
							qualName, 
							qualValue,
							bToSubClasses,
							bToInstances,
							bOverridable)
{
	try 
	{	
		var theQual;
		try
		{
			theQual = classObj.Qualifiers_(qualName);
			theQual.Value = qualValue;
		}
		catch (e)
		{
			//if (e.number == wbemErrNotFound)  //qualifier not found
			//{
			if (e.number == -2147217406)  //qualifier not found
			{
				theQual = classObj.Qualifiers_.Add(qualName, 
													qualValue, 
													bToSubClasses,
													bToInstances,
													bOverridable);
			}
			else
			{
				throw(e);	//re-throw
			}
		}
		
		//commit changes to WMI
		classObj.Put_();
	}
	catch (e)
	{
		wizard.ReportError ("Fehler in AddQualifier(): " + e.description);
	}

}


function IsWMIOk (selProj, selObj)
{
	//uses WMI Registry provider to determine WMI version
		
	try 
	{
		if (!CanAddATLClass(selProj, selObj))
		{
			return false;
		}
		try 
		{
			var locator = new ActiveXObject("WbemScripting.SWbemLocator");
		
			var services = locator.ConnectServer("", //local server
										"root\\default");												
		}
		catch (e)
		{
			throw("Die Verbindung mit WMI konnte nicht hergestellt werden. Vergewissern Sie sich, dass WMI auf dem COmputer installiert ist.");
		}
				
		var classObj = services.Get("StdRegProv");
				
		var methObject = classObj.Methods_.Item("GetExpandedStringValue");
		var inParmsInstance = methObject.InParameters.SpawnInstance_();
				
		inParmsInstance.Properties_.Add("sSubKeyName", 8);
	
		inParmsInstance.Properties_("sSubKeyName").Value = 
			"SOFTWARE\\Microsoft\\WBEM";
	

		inParmsInstance.Properties_.Add("sValueName", 8);
	
		inParmsInstance.Properties_("sValueName").Value = "Build";


		var outParms = classObj.ExecMethod_("GetExpandedStringValue",
											inParmsInstance);
											
		if (outParms.Properties_("ReturnValue").Value != 0)
		{
			throw ("Die WMI-Buildnummer konnte nicht ermittelt werden.");
		}

		if (outParms.Properties_("sValue").Value >= "1085")
		{
			return true;
		}
		else 
		{			
			throw ("Die auf dem Computer installierte WMI-Version ist " + outParms.Properties_("sValue").Value +
					". Version 1085 oder h�her ist zum Ausf�hren dieses Assistenten erforderlich.");
		}
	}

	catch (e)
	{
		wizard.ReportError ("Das WMI-Ereignisanbieterobjekt konnte dem Projekt aufgrund des folgenden Fehlers nicht hinzugef�gt werden:\n\r" 
				+ e);
		return false;
	}

	
}
