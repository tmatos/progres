/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#ifndef EDICAODESINAL_H
#define EDICAODESINAL_H

//(*Headers(EdicaoDeSinal)
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class EdicaoDeSinal : public wxDialog
{
    public:
        EdicaoDeSinal(wxWindow* parent,
                      wxWindowID id = wxID_ANY,
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize);
        
        virtual ~EdicaoDeSinal();

        void setFile(wxString filePath);
        void closeFile();

        //(*Declarations(EdicaoDeSinal)
        wxButton* btnSalvar;
        wxTextCtrl* txtWaveIn;
        //*)

    protected:
        //(*Identifiers(EdicaoDeSinal)
        static const long ID_TEXTCTRL1;
        static const long idBtn_Salvar;
        //*)

    private:
        //(*Handlers(EdicaoDeSinal)
        void OnbtnDescartarClick(wxCommandEvent& event);
        void OnbtnSalvarClick(wxCommandEvent& event);
        //*)

        void keyPressed(wxKeyEvent& event);

        void OnClose(wxCloseEvent& event);

        wxString file;

        DECLARE_EVENT_TABLE()
};

#endif // EDICAODESINAL_H
