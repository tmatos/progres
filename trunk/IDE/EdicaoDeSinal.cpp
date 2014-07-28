
#include <wx/wx.h>
#include <wx/textfile.h>

#include "EdicaoDeSinal.h"
#include "SinaisDrawPane.h"

//(*InternalHeaders(EdicaoDeSinal)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//(*IdInit(EdicaoDeSinal)
const long EdicaoDeSinal::ID_TEXTCTRL1 = wxNewId();
const long EdicaoDeSinal::idBtn_Salvar = wxNewId();
//*)

BEGIN_EVENT_TABLE(EdicaoDeSinal,wxDialog)
	//(*EventTable(EdicaoDeSinal)
	//*)

EVT_CLOSE(EdicaoDeSinal::OnClose)

END_EVENT_TABLE()

EdicaoDeSinal::EdicaoDeSinal(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(EdicaoDeSinal)
	Create(parent, id, _("Arquivo de entrada"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(590,366));
	Move(wxDefaultPosition);
	txtWaveIn = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(8,8), wxSize(576,320), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	btnSalvar = new wxButton(this, idBtn_Salvar, _("Salvar"), wxPoint(480,336), wxSize(104,23), 0, wxDefaultValidator, _T("idBtn_Salvar"));

	Connect(idBtn_Salvar,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EdicaoDeSinal::OnbtnSalvarClick);
	//*)
}

EdicaoDeSinal::~EdicaoDeSinal()
{
	//(*Destroy(EdicaoDeSinal)
	//*)
}

void EdicaoDeSinal::setFile(wxString filePath)
{
    file = filePath;

    txtWaveIn->LoadFile(file);
}

void EdicaoDeSinal::OnbtnSalvarClick(wxCommandEvent& event)
{
    wxTextFile arquivoSalvando;

    if(arquivoSalvando.Open(file))
    {
        arquivoSalvando.Clear();
        arquivoSalvando.AddLine(txtWaveIn->GetValue());
        arquivoSalvando.Write();
        arquivoSalvando.Close();

        SinaisDrawPane* pai = (SinaisDrawPane*) this->GetParent();

        pai->setSinais(file, true);
    }
    else
    {
        wxMessageBox(_("Impossibilitado de salvar o arquivo: ") + file, _("Erro"));
    }
}

void EdicaoDeSinal::OnClose(wxCloseEvent& event)
{
    SinaisDrawPane* pai = (SinaisDrawPane*) this->GetParent();
    pai->estaEmEdicao = false;

    this->Show(false);
}
