/***************************************************************
 * Name:      IDEMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Tiago Matos ()
 * Created:   2014-06-12
 * Copyright: Tiago Matos ()
 * License:
 **************************************************************/

#include "IDEMain.h"
#include "IDEConfig.h"
#include <wx/msgdlg.h>
#include <wx/dcclient.h>
#include <wx/textfile.h>
#include <wx/filename.h>
#include <wx/tokenzr.h>

//(*InternalHeaders(IDEFrame)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(IDEFrame)
const long IDEFrame::ID_TEXTCTRL_FONTE = wxNewId();
const long IDEFrame::ID_NOTEBOOK1 = wxNewId();
const long IDEFrame::ID_LISTBOXERROS = wxNewId();
const long IDEFrame::ID_SPLITTERWINDOW1 = wxNewId();
const long IDEFrame::ID_MENUITEM2 = wxNewId();
const long IDEFrame::ID_MENUITEM3 = wxNewId();
const long IDEFrame::ID_MENUITEM1 = wxNewId();
const long IDEFrame::idMenuOpen = wxNewId();
const long IDEFrame::ID_MENUITEM8 = wxNewId();
const long IDEFrame::idMenuQuit = wxNewId();
const long IDEFrame::ID_MENUITEM6 = wxNewId();
const long IDEFrame::ID_MENUITEM4 = wxNewId();
const long IDEFrame::ID_MENUITEM7 = wxNewId();
const long IDEFrame::idMenuAbout = wxNewId();
const long IDEFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(IDEFrame,wxFrame)
    //(*EventTable(IDEFrame)
    //*)
END_EVENT_TABLE()

IDEFrame::IDEFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(IDEFrame)
    wxMenuItem* MenuItemSair;
    wxMenuItem* MenuItemSobre;
    wxMenu* MenuAjuda;
    wxMenuBar* MenuBarPrincipal;
    wxMenu* MenuArquivo;

    Create(parent, wxID_ANY, _("Progres IDE"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(640,480));
    SplitterWindow1 = new wxSplitterWindow(this, ID_SPLITTERWINDOW1, wxPoint(128,272), wxSize(656,325), wxSP_3D, _T("ID_SPLITTERWINDOW1"));
    SplitterWindow1->SetMinSize(wxSize(10,10));
    SplitterWindow1->SetMinimumPaneSize(10);
    SplitterWindow1->SetSashGravity(0.5);
    bookFontes = new wxNotebook(SplitterWindow1, ID_NOTEBOOK1, wxPoint(156,100), wxSize(176,64), 0, _T("ID_NOTEBOOK1"));
    EditBox = new wxTextCtrl(bookFontes, ID_TEXTCTRL_FONTE, wxEmptyString, wxPoint(191,70), wxSize(543,208), wxTE_MULTILINE|wxTE_RICH2|wxTE_DONTWRAP, wxDefaultValidator, _T("ID_TEXTCTRL_FONTE"));
    EditBox->SetMinSize(wxSize(-1,-1));
    EditBox->SetMaxSize(wxSize(-1,-1));
    wxFont EditBoxFont(11,wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
    EditBox->SetFont(EditBoxFont);
    bookFontes->AddPage(EditBox, wxEmptyString, false);
    ListBoxErros = new wxListBox(SplitterWindow1, ID_LISTBOXERROS, wxPoint(0,276), wxSize(551,49), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOXERROS"));
    ListBoxErros->SetMinSize(wxSize(-1,-1));
    ListBoxErros->SetMaxSize(wxSize(-1,-1));
    SplitterWindow1->SplitHorizontally(bookFontes, ListBoxErros);
    SplitterWindow1->SetSashPosition(400);
    MenuBarPrincipal = new wxMenuBar();
    MenuArquivo = new wxMenu();
    MenuItem2 = new wxMenu();
    MenuItemNovoCircuito = new wxMenuItem(MenuItem2, ID_MENUITEM2, _("Circuto"), wxEmptyString, wxITEM_NORMAL);
    MenuItem2->Append(MenuItemNovoCircuito);
    MenuItemNovaOnda = new wxMenuItem(MenuItem2, ID_MENUITEM3, _("Onda"), wxEmptyString, wxITEM_NORMAL);
    MenuItem2->Append(MenuItemNovaOnda);
    MenuArquivo->Append(ID_MENUITEM1, _("Novo"), MenuItem2, wxEmptyString);
    MenuItem1 = new wxMenuItem(MenuArquivo, idMenuOpen, _("Abrir\tCtrl-O"), _("Abrir um fonte Verilog"), wxITEM_NORMAL);
    MenuArquivo->Append(MenuItem1);
    MenuItem4 = new wxMenuItem(MenuArquivo, ID_MENUITEM8, _("Recentes"), wxEmptyString, wxITEM_NORMAL);
    MenuArquivo->Append(MenuItem4);
    MenuItemSair = new wxMenuItem(MenuArquivo, idMenuQuit, _("Sair\tAlt-F4"), _("Encerrar o aplicativo."), wxITEM_NORMAL);
    MenuArquivo->Append(MenuItemSair);
    MenuBarPrincipal->Append(MenuArquivo, _("&Arquivo"));
    Menu2 = new wxMenu();
    MenuItemSelecionarTudo = new wxMenuItem(Menu2, ID_MENUITEM6, _("Selecionar tudo\tCtrl-A"), wxEmptyString, wxITEM_NORMAL);
    Menu2->Append(MenuItemSelecionarTudo);
    MenuBarPrincipal->Append(Menu2, _("Editar"));
    Menu3 = new wxMenu();
    MenuBarPrincipal->Append(Menu3, _("Exibir"));
    Menu1 = new wxMenu();
    MenuItemAnalisar = new wxMenuItem(Menu1, ID_MENUITEM4, _("Analisar circuito\tF5"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItemAnalisar);
    MenuBarPrincipal->Append(Menu1, _("Simulação"));
    MenuOpcoes = new wxMenu();
    MenuItemConfig = new wxMenuItem(MenuOpcoes, ID_MENUITEM7, _("Configura"), wxEmptyString, wxITEM_NORMAL);
    MenuOpcoes->Append(MenuItemConfig);
    MenuBarPrincipal->Append(MenuOpcoes, _("Opções"));
    MenuAjuda = new wxMenu();
    MenuItemSobre = new wxMenuItem(MenuAjuda, idMenuAbout, _("Sobre\tF1"), _("Exibir info sobre o aplicativo."), wxITEM_NORMAL);
    MenuAjuda->Append(MenuItemSobre);
    MenuBarPrincipal->Append(MenuAjuda, _("Ajuda"));
    SetMenuBar(MenuBarPrincipal);
    StatusBarPrincipal = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[3] = { -80, -10, -10 };
    int __wxStatusBarStyles_1[3] = { wxSB_NORMAL, wxSB_NORMAL, wxSB_NORMAL };
    StatusBarPrincipal->SetFieldsCount(3,__wxStatusBarWidths_1);
    StatusBarPrincipal->SetStatusStyles(3,__wxStatusBarStyles_1);
    SetStatusBar(StatusBarPrincipal);
    FileDialogFonte = new wxFileDialog(this, _("Selecionar arquivo"), wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr, wxFD_DEFAULT_STYLE|wxFD_OPEN|wxFD_FILE_MUST_EXIST, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));

    Connect(ID_TEXTCTRL_FONTE,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&IDEFrame::OnEditBoxText);
    Connect(ID_LISTBOXERROS,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&IDEFrame::OnListBoxErrosDClick);
    Connect(ID_MENUITEM2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemNovoCircuitoSelected);
    Connect(ID_MENUITEM3,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemNovaOndaSelected);
    Connect(idMenuOpen,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemOpen);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnQuit);
    Connect(ID_MENUITEM6,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemSelecionarTudoSelected);
    Connect(ID_MENUITEM4,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemAnalisarSelected);
    Connect(ID_MENUITEM7,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemConfigSelected);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnAbout);
    //*)

    defaultWindowTitle = _("Progres IDE");
    SetTitle(defaultWindowTitle);

    carregaConfigs();

    if(AbrirUltimoAoIniciar) {
        if(!UltimoArquivoVerilog.IsEmpty())
            CarregarArquivoVerilog(UltimoArquivoVerilog);
    }
}

IDEFrame::~IDEFrame()
{
    if(AbrirUltimoAoIniciar)
    {
        wxConfig *config = new wxConfig(_("ProgresIDE"));
        config->Write(_("UltimoArquivoVerilog"), verilogFilePath);
        delete config;
    }

    //(*Destroy(IDEFrame)
    //*)
}

void IDEFrame::carregaConfigs()
{
    wxConfig *config = new wxConfig(_("ProgresIDE"));

    config->Read(_("SimuladorExePath"), &simuladorExePath);

    if(config->Read(_("AbrirUltimoAoIniciar"), &AbrirUltimoAoIniciar))
        config->Read(_("UltimoArquivoVerilog"), &UltimoArquivoVerilog);
    else
        AbrirUltimoAoIniciar = false;

    delete config;

    if(simuladorExePath.IsEmpty())
        wxMessageBox(_("O executável do simulador não foi definido. Por-favor selecione-o nas configurações."), _("Aviso"));
}

void IDEFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void IDEFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = _("Progres Verilog Simulator");
    wxMessageBox(msg, _("Progres"));
}

void IDEFrame::OnMenuItemOpen(wxCommandEvent& event)
{
    if(FileDialogFonte->ShowModal() == wxID_OK)
    {
        CarregarArquivoVerilog(FileDialogFonte->GetPath());
    }
}

void IDEFrame::CarregarArquivoVerilog(wxString arquivo)
{
    verilogFilePath = arquivo;

    EditBox->LoadFile(verilogFilePath);

    SetTitle(defaultWindowTitle + _(" - ") + verilogFilePath);

    textLenght = EditBox->GetNumberOfLines();
    StatusBarPrincipal->SetStatusText(wxString::Format(wxT("%i"), textLenght), 1);

    ListBoxErros->Clear();
}

void IDEFrame::OnMenuItemAnalisarSelected(wxCommandEvent& event)
{
    if(verilogFilePath.IsEmpty())
    {
        wxMessageBox(_("Não há arquivo aberto."), _("Erro"));
    }
    else
    {
        wxArrayString saida;
        wxString comando = simuladorExePath + _(" ") + verilogFilePath;

        wxExecute(comando, saida);

        ListBoxErros->Clear();

        for(unsigned int i = 0 ; i < saida.Count() ; i++)
            ListBoxErros->Append(saida[i]);
    }
}

void IDEFrame::OnMenuItemNovoCircuitoSelected(wxCommandEvent& event)
{
    //
}

void IDEFrame::OnMenuItemNovaOndaSelected(wxCommandEvent& event)
{
    wxPanel *panel = new wxPanel(bookFontes);
    bookFontes->AddPage(panel, _("Nova onda"));
    bookFontes->ChangeSelection(bookFontes->GetPageCount() - 1);

    wxClientDC *canvas = new wxClientDC(panel);
    canvas->SetBrush( *wxRED_BRUSH );
    //canvas->DrawRectangle( 15, 15, 50, 70 );

    int n = 5; // num. de tempos
    int un = 20; // qtd de pixels de uma unidadde
    int x0 = 20, y0 = 30; // inicio do desenho
    int x = x0, y = y0;
    int vlr = 1; // valor lógico

    int tx = x + un; // deslc. de pixel relat. ao tempo q passou
    y = vlr ? y : y + 15;
    canvas->DrawLine(x, y, tx, y);
    x = tx;

    vlr = 0;
    canvas->DrawLine(x, y0, x, y0 + 15); // se há mudança de nivel lógico, deve-se ter a linha vert.

    tx = x + un;
    y = vlr ? y0 : y0 + 15;
    canvas->DrawLine(x, y, tx, y);
    x = tx;

    vlr = 1;
    canvas->DrawLine(x, y0, x, y0 + 15); // se há mudança de nivel lógico, deve-se ter a linha vert.

    tx = x + (un * n);
    y = vlr ? y0 : y0 + 15;
    canvas->DrawLine(x, y, tx, y);
    x = tx;

    vlr = 0;
    canvas->DrawLine(x, y0, x, y0 + 15); // se há mudança de nivel lógico, deve-se ter a linha vert.

    tx = x + un* 234;
    y = vlr ? y0 : y0 + 15;
    canvas->DrawLine(x, y, tx, y);
    x = tx;
}

void IDEFrame::OnEditBoxText(wxCommandEvent& event)
{
        textLenght = EditBox->GetNumberOfLines();
        StatusBarPrincipal->SetStatusText(wxString::Format(wxT("%i"), textLenght), 1);

//        if(event.GetId() != ID_LISTBOXERROS)
//            EditBox->SetStyle(0, EditBox->GetLastPosition(), wxTextAttr(*wxBLACK, *wxWHITE));
}

void IDEFrame::OnListBoxErrosDClick(wxCommandEvent& event)
{
    long line = 0; // primeira linha (origem em zero)

    wxString msg = ListBoxErros->GetString(ListBoxErros->GetSelection());

    wxStringTokenizer partes(msg, _(":"));

    if ( partes.HasMoreTokens() )
    {
        wxString token = partes.GetNextToken();

        if ( token.ToLong(&line) );
            line--; // reajuste da origem das linhas
    }

    long posicao = EditBox->XYToPosition(0, line); // posicao na seq. completa do texto
    long lineSize = EditBox->GetLineLength(line); // comprimento do texto nessa linha

    //StatusBarPrincipal->SetStatusText(wxString::Format(wxT("DBG: %i"), lineSize), 2); // DEBG
    //EditBox->SetStyle(posicao, posicao+lineSize, wxTextAttr(*wxBLACK, *wxRED));

    EditBox->SetInsertionPoint(posicao+lineSize);
    EditBox->SetFocus();
}

void IDEFrame::OnMenuItemSelecionarTudoSelected(wxCommandEvent& event)
{
    if(bookFontes->GetSelection() == 0) {
        EditBox->SelectAll();
        EditBox->SetFocus();
    }
}

void IDEFrame::OnMenuItemConfigSelected(wxCommandEvent& event)
{
    IDEConfig* janelaCfg = new IDEConfig(this);
    janelaCfg->Show();
}
