/***************************************************************
 * Name:      AgendaApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Ciro Meneses (ciroccuva@hotmail.com)
 * Created:   2015-10-18
 * Copyright: Ciro Meneses (www.programacaopradiversao.blogspot.com.br)
 * License:
 **************************************************************/

#include "AgendaApp.h"

//(*AppHeaders
#include "AgendaMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(AgendaApp);

bool AgendaApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	AgendaFrame* Frame = new AgendaFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
