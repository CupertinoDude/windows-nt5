PrintMOF();

function PrintMOF()
{
    var args = WScript.Arguments;

    for (Index = 0; Index < args(0); Index++)
    {
        var Class,
            CurrentClockSpeed,
            ProcessID,
            SubIndex,
            Type = Index % 3,
            Obj;

        if (Type == 0)
        {
            Class = "Win32_Processor";
            CurrentClockSpeed = Index % 100;

                Obj = 
                    "instance of Win32_Processor " +
                    "{" +
                    "   CurrentClockSpeed = " + CurrentClockSpeed + ";" +
                    "}";
        }
        else if (Type == 1)
        {
            Class = "Win32_Process";
            ProcessID = Index % 2000;

            Obj = 
                "instance of Win32_Process " +
                "{" +
                "   ProcessID = " + ProcessID + ";" +
                "}";
        }
        else
        {
            Class = "MSFT_EssTestIndex2Event";
            SubIndex = Index + 1000;

            Obj = 
                "instance of MSFT_EssTestIndex2Event " +
                "{" +
                "   Index = " + SubIndex + ";" +
                "}";
        }

        //if (%ScriptRule%)
        if (1)
        {
            var strOut =
                "instance of MSFT_EssTestIndex2Event " +
                "{" +
                "   Index = " + Index + ";" +
                "   Obj = " + Obj + ";" +
                "};"
            
            WScript.Echo(strOut);
        }
    }
};
