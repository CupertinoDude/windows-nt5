
#ifndef _TAPI_OBJECT_SAFETY_
#define _TAPI_OBJECT_SAFETY_

#include "PromptedObjectSafety.h"
#include "ScrpScrtDlg.h"
#include "ObjectWithSite.h"

/*++

Copyright (c) 1999  Microsoft Corporation

Module Name:

    TAPIObjectSafety.h

Abstract:

  Implements IObjectSafety and IObjectWithSite

  Manages decision logic of whether tapi functionality should be allowed on 
  the current page. Checks persistent settings for this page and prompts with
  a dialog if necessary.

  Objects that need this protection (CTTAPI, CRequest) derive from this
  class.

--*/


static const TCHAR gszCookieName[] = _T("TAPI");


class CTAPIObjectSafety : public CPromptedObjectSafety, public CObjectWithSite
{


public:
DECLARE_TRACELOG_CLASS(CTAPIObjectSafety)
    
    //
    // call CObjectWithSite's constructor and pass in the cookie name
    //

    CTAPIObjectSafety()
        :CObjectWithSite(gszCookieName)
    {
    }


    //
    // implementing CPromptedObjectSafety's pure virtual method
    // if the page is not in the safe list, and this is the first 
    // time we are asking, prompt the user. act accordingly.
    // if the user chooses, mark the page safe for scripting (persistently)
    //
    
    virtual BOOL Ask()
    {

        //
        // if the object does not have a site pointer. we should not consider 
        // it to be safe. Do not prompt the user
        //

        if ( !HaveSite() )
        {

            return FALSE;
        }


        EnValidation enCurrentValidation = GetValidation();
        
        //
        // if the page has not been validated, try to validate it.
        //

        if (UNVALIDATED == enCurrentValidation)
        {

           CScriptSecurityDialog *pDialog = new CScriptSecurityDialog;
       
           //
           // if succeeded displaying the dialog
           // validate the page based on user's input
           //
           
           if ( NULL != pDialog )
           {

               switch (pDialog->DoModalWithText(IDS_TAPI_SEC_PROMPT))
               {

                case ID_YES:

                    Validate(VALIDATED_SAFE);
                    break;

                case ID_NO:

                    Validate(VALIDATED_UNSAFE);
                    break;

                case ID_YES_DONT_ASK_AGAIN:

                    Validate(VALIDATED_SAFE_PERMANENT);
                    break;

                default:

                    break;

               }

               delete pDialog;

                // 
                // get the new validation.
                //

                enCurrentValidation = GetValidation();

           } // if (NULL != pDialog) 

        }

        //
        // by now we either got the validation data or validation did not change
        //
        // return true if the page is validated as safe
        //

        return (VALIDATED_SAFE == enCurrentValidation);
    }

};

#endif // _TAPI_OBJECT_SAFETY_