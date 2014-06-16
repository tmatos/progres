/***************************************************************
 * Name:      IDEApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Tiago Matos ()
 * Created:   2014-06-12
 * Copyright: Tiago Matos ()
 * License:
 **************************************************************/

#include "IDEApp.h"

//(*AppHeaders
#include "IDEMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(IDEApp);

bool IDEApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	IDEFrame* Frame = new IDEFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
