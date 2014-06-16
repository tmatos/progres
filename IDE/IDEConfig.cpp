#include "IDEConfig.h"
#include "IDEMain.h"

//(*InternalHeaders(IDEConfig)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(IDEConfig)
const long IDEConfig::ID_BUTTON1 = wxNewId();
const long IDEConfig::ID_BUTTON2 = wxNewId();
const long IDEConfig::ID_STATICTEXT1 = wxNewId();
const long IDEConfig::ID_TEXTCTRL1 = wxNewId();
const long IDEConfig::ID_BUTTON3 = wxNewId();
//*)

BEGIN_EVENT_TABLE(IDEConfig,wxDialog)
	//(*EventTable(IDEConfig)
	//*)
END_EVENT_TABLE()

IDEConfig::IDEConfig(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(IDEConfig)
	Create(parent, wxID_ANY, _("Configura"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetClientSize(wxSize(480,340));
	btnSalvar = new wxButton(this, ID_BUTTON1, _("Salvar"), wxPoint(304,304), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	btnCancelar = new wxButton(this, ID_BUTTON2, _("Cancelar"), wxPoint(392,304), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	lblSimuladorPath = new wxStaticText(this, ID_STATICTEXT1, _("Simulador:"), wxPoint(24,23), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	txtSimuladorPath = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(80,20), wxSize(304,21), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	btnSimuladorPath = new wxButton(this, ID_BUTTON3, _("Selecionar"), wxPoint(392,19), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
	fileDiagSimuladorPath = new wxFileDialog(this, _("Selecionar o executavel do simulador"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_DEFAULT_STYLE|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&IDEConfig::OnbtnSalvarClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&IDEConfig::OnbtnCancelarClick);
	Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&IDEConfig::OnbtnSimuladorPathClick);
	//*)

	config = new wxConfig(_("ProgresIDE"));

	if ( config->Read(_("SimuladorExePath"), &simuladorExePath) ) {
        txtSimuladorPath->ChangeValue(simuladorExePath);
    }
    else {

    }

}

IDEConfig::~IDEConfig()
{
	//(*Destroy(IDEConfig)
	//*)
}


void IDEConfig::OnbtnCancelarClick(wxCommandEvent& event)
{
    this->Close();
}

void IDEConfig::OnbtnSimuladorPathClick(wxCommandEvent& event)
{
    if(fileDiagSimuladorPath->ShowModal() == wxID_OK)
    {
        simuladorExePath = fileDiagSimuladorPath->GetPath();
        txtSimuladorPath->ChangeValue(simuladorExePath);
    }
}

void IDEConfig::OnbtnSalvarClick(wxCommandEvent& event)
{
    simuladorExePath = txtSimuladorPath->GetValue();
    config->Write(_("SimuladorExePath"), simuladorExePath);

    delete config; // deve salvar as configs

    IDEFrame* pai = (IDEFrame*) this->GetParent();

    pai->carregaConfigs();

    this->Close();
}
