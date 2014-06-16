/***************************************************************
 * Name:      IDEApp.h
 * Purpose:   Defines Application Class
 * Author:    Tiago Matos ()
 * Created:   2014-06-12
 * Copyright: Tiago Matos ()
 * License:
 **************************************************************/

#ifndef IDEAPP_H
#define IDEAPP_H

#include <wx/app.h>

class IDEApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // IDEAPP_H
