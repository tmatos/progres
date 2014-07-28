
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
const long EdicaoDeSinal::ID_BUTTON1 = wxNewId();
const long EdicaoDeSinal::ID_BUTTON2 = wxNewId();
//*)

BEGIN_EVENT_TABLE(EdicaoDeSinal,wxDialog)
	//(*EventTable(EdicaoDeSinal)
	//*)
END_EVENT_TABLE()

EdicaoDeSinal::EdicaoDeSinal(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
	//(*Initialize(EdicaoDeSinal)
	Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
	SetClientSize(wxSize(590,366));
	Move(wxDefaultPosition);
	txtWaveIn = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxPoint(8,8), wxSize(576,320), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	btnDescartar = new wxButton(this, ID_BUTTON1, _("Descartar"), wxPoint(496,336), wxSize(83,23), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	btnSalvar = new wxButton(this, ID_BUTTON2, _("Salvar"), wxPoint(400,336), wxSize(83,23), 0, wxDefaultValidator, _T("ID_BUTTON2"));

	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EdicaoDeSinal::OnbtnDescartarClick);
	Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&EdicaoDeSinal::OnbtnSalvarClick);
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

void EdicaoDeSinal::OnbtnDescartarClick(wxCommandEvent& event)
{
    this->Close();
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
