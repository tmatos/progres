
#ifndef IDECONFIG_H
#define IDECONFIG_H

//(*Headers(IDEConfig)
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/filedlg.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

#include <wx/config.h>

class IDEConfig: public wxDialog
{
    public:

        IDEConfig(wxWindow* parent,wxWindowID id=wxID_ANY);
        virtual ~IDEConfig();

        //(*Declarations(IDEConfig)
        wxTextCtrl* txtSimuladorPath;
        wxButton* btnSimuladorPath;
        wxStaticText* lblSimuladorPath;
        wxButton* btnSalvar;
        wxCheckBox* ChkAbrirUltimoAoIniciar;
        wxButton* btnCancelar;
        wxFileDialog* fileDiagSimuladorPath;
        //*)

    protected:

        //(*Identifiers(IDEConfig)
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_STATICTEXT1;
        static const long ID_TEXTCTRL1;
        static const long ID_BUTTON3;
        static const long ID_CHECKBOX1;
        //*)

    private:

        //(*Handlers(IDEConfig)
        void OnbtnCancelarClick(wxCommandEvent& event);
        void OnbtnSimuladorPathClick(wxCommandEvent& event);
        void OnbtnSalvarClick(wxCommandEvent& event);
        void OnChkAbrirUltimoAoIniciarClick(wxCommandEvent& event);
        //*)

        wxString simuladorExePath;
        bool AbrirUltimoAoIniciar;
        wxConfig *config;

        DECLARE_EVENT_TABLE()
};

#endif // IDECONFIG_H
