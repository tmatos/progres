/***************************************************************
 * Name:      IDEMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Tiago Matos (2014)
 * Created:   2014-06-12
 * Copyright: Tiago Matos (2014)
 * License:   MIT License
 **************************************************************/

#include "IDEMain.h"
#include "IDEConfig.h"
#include "SinaisDrawPane.h"
#include "inout.h"
#include "sinais.h"

#include <wx/msgdlg.h>
#include <wx/aboutdlg.h>
#include <wx/dcclient.h>
#include <wx/textfile.h>
#include <wx/filename.h>
#include <wx/tokenzr.h>
#include <wx/sizer.h>

//(*InternalHeaders(IDEFrame)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f,
    long_f
};

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
const long IDEFrame::ID_MENU_FILE_NEW_CIRCUIT = wxNewId();
const long IDEFrame::ID_MENU_FILE_NEW = wxNewId();
const long IDEFrame::ID_MENU_FILE_OPEN = wxNewId();
const long IDEFrame::ID_MENU_FILE_SAVE = wxNewId();
const long IDEFrame::ID_MENU_FILE_RECENTS_CLEAR = wxNewId();
const long IDEFrame::ID_MENU_FILE_RECENTS = wxNewId();
const long IDEFrame::ID_MENU_FILE_CLOSE = wxNewId();
const long IDEFrame::ID_MENU_FILE_QUIT = wxNewId();
const long IDEFrame::ID_MENU_EDIT_SELECTALL = wxNewId();
const long IDEFrame::ID_MENUITEM_TESTE = wxNewId();
const long IDEFrame::ID_MENUITEM_ENTRADA_NOVO = wxNewId();
const long IDEFrame::ID_MENUITEM_ENTRADA_ABRIR = wxNewId();
const long IDEFrame::ID_MENU_SIMULATION_ANALYSE = wxNewId();
const long IDEFrame::ID_MENU_OPTIONS_CONFIG = wxNewId();
const long IDEFrame::ID_MENU_HELP_ABOUT = wxNewId();
const long IDEFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(IDEFrame,wxFrame)
    //(*EventTable(IDEFrame)
    //*)
END_EVENT_TABLE()

IDEFrame::IDEFrame(wxWindow* parent, wxWindowID id)
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
    SplitterWindow1->SetSashGravity(0.5);
    bookFontes = new wxNotebook(SplitterWindow1, ID_NOTEBOOK1, wxPoint(156,100), wxSize(176,64), 0, _T("ID_NOTEBOOK1"));
    EditBox = new wxTextCtrl(bookFontes, ID_TEXTCTRL_FONTE, wxEmptyString, wxPoint(191,70), wxSize(543,208), wxTE_MULTILINE|wxTE_RICH2|wxTE_DONTWRAP, wxDefaultValidator, _T("ID_TEXTCTRL_FONTE"));
    EditBox->SetMinSize(wxSize(-1,-1));
    EditBox->SetMaxSize(wxSize(-1,-1));
    wxFont EditBoxFont(11,wxFONTFAMILY_SCRIPT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Courier New"),wxFONTENCODING_DEFAULT);
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
    MenuItemNovoCircuito = new wxMenuItem(MenuItem2, ID_MENU_FILE_NEW_CIRCUIT, _("Circuito"), wxEmptyString, wxITEM_NORMAL);
    MenuItem2->Append(MenuItemNovoCircuito);
    MenuArquivo->Append(ID_MENU_FILE_NEW, _("Novo"), MenuItem2, wxEmptyString);
    MenuItem1 = new wxMenuItem(MenuArquivo, ID_MENU_FILE_OPEN, _("Abrir\tCtrl-O"), _("Abrir um fonte Verilog"), wxITEM_NORMAL);
    MenuArquivo->Append(MenuItem1);
    MenuItemSave = new wxMenuItem(MenuArquivo, ID_MENU_FILE_SAVE, _("Salvar\tCtrl-S"), _("Salvar o arquivo em edição."), wxITEM_NORMAL);
    MenuArquivo->Append(MenuItemSave);
    MenuItem4 = new wxMenu();
    MenuItem6 = new wxMenuItem(MenuItem4, ID_MENU_FILE_RECENTS_CLEAR, _("Limpar"), _("Limpar a lista de arquivos abertos mais recentemente."), wxITEM_NORMAL);
    MenuItem4->Append(MenuItem6);
    MenuItem6->Enable(false);
    MenuArquivo->Append(ID_MENU_FILE_RECENTS, _("Recentes"), MenuItem4, wxEmptyString);
    MenuItemClose = new wxMenuItem(MenuArquivo, ID_MENU_FILE_CLOSE, _("Fechar\tCtrl-W"), _("Fechar o arquivo em edição."), wxITEM_NORMAL);
    MenuArquivo->Append(MenuItemClose);
    MenuArquivo->AppendSeparator();
    MenuItemSair = new wxMenuItem(MenuArquivo, ID_MENU_FILE_QUIT, _("Sair\tAlt-F4"), _("Encerrar o aplicativo."), wxITEM_NORMAL);
    MenuArquivo->Append(MenuItemSair);
    MenuBarPrincipal->Append(MenuArquivo, _("&Arquivo"));
    Menu2 = new wxMenu();
    MenuItemSelecionarTudo = new wxMenuItem(Menu2, ID_MENU_EDIT_SELECTALL, _("Selecionar tudo\tCtrl-A"), wxEmptyString, wxITEM_NORMAL);
    Menu2->Append(MenuItemSelecionarTudo);
    MenuBarPrincipal->Append(Menu2, _("Editar"));
    Menu3 = new wxMenu();
    MenuItemTeste = new wxMenuItem(Menu3, ID_MENUITEM_TESTE, _("Teste"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItemTeste);
    MenuBarPrincipal->Append(Menu3, _("Exibir"));
    Menu4 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu4, ID_MENUITEM_ENTRADA_NOVO, _("Novo arquivo de entrada"), wxEmptyString, wxITEM_NORMAL);
    Menu4->Append(MenuItem3);
    MenuItem5 = new wxMenuItem(Menu4, ID_MENUITEM_ENTRADA_ABRIR, _("Abrir arquivo de entrada"), wxEmptyString, wxITEM_NORMAL);
    Menu4->Append(MenuItem5);
    MenuBarPrincipal->Append(Menu4, _("Entradas"));
    Menu1 = new wxMenu();
    MenuItemAnalisar = new wxMenuItem(Menu1, ID_MENU_SIMULATION_ANALYSE, _("Analisar circuito\tF5"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuItemAnalisar);
    MenuBarPrincipal->Append(Menu1, _("Simulation"));
    MenuOpcoes = new wxMenu();
    MenuItemConfig = new wxMenuItem(MenuOpcoes, ID_MENU_OPTIONS_CONFIG, _("Configs"), wxEmptyString, wxITEM_NORMAL);
    MenuOpcoes->Append(MenuItemConfig);
    MenuBarPrincipal->Append(MenuOpcoes, _("Options"));
    MenuAjuda = new wxMenu();
    MenuItemSobre = new wxMenuItem(MenuAjuda, ID_MENU_HELP_ABOUT, _("Sobre\tF1"), _("Exibir info sobre o aplicativo."), wxITEM_NORMAL);
    MenuAjuda->Append(MenuItemSobre);
    MenuBarPrincipal->Append(MenuAjuda, _("Ajuda"));
    SetMenuBar(MenuBarPrincipal);
    StatusBarPrincipal = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[3] = { -80, -10, -10 };
    int __wxStatusBarStyles_1[3] = { wxSB_NORMAL, wxSB_NORMAL, wxSB_NORMAL };
    StatusBarPrincipal->SetFieldsCount(3,__wxStatusBarWidths_1);
    StatusBarPrincipal->SetStatusStyles(3,__wxStatusBarStyles_1);
    SetStatusBar(StatusBarPrincipal);

    Connect(ID_TEXTCTRL_FONTE,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&IDEFrame::OnEditBoxText);
    Connect(ID_LISTBOXERROS,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&IDEFrame::OnListBoxErrosDClick);
    Connect(ID_MENU_FILE_NEW_CIRCUIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemNovoCircuitoSelected);
    Connect(ID_MENU_FILE_OPEN,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemOpen);
    Connect(ID_MENU_FILE_SAVE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemSave);
    Connect(ID_MENU_FILE_CLOSE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemCloseSelected);
    Connect(ID_MENU_FILE_QUIT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnQuit);
    Connect(ID_MENU_EDIT_SELECTALL,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemSelecionarTudoSelected);
    Connect(ID_MENUITEM_TESTE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemTesteSelected);
    Connect(ID_MENUITEM_ENTRADA_NOVO,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemEntradaNovoSelected);
    Connect(ID_MENUITEM_ENTRADA_ABRIR,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemEntradaAbrirSelected);
    Connect(ID_MENU_SIMULATION_ANALYSE,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemAnalisarSelected);
    Connect(ID_MENU_OPTIONS_CONFIG,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnMenuItemConfigSelected);
    Connect(ID_MENU_HELP_ABOUT,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&IDEFrame::OnAbout);
    //*)

    defaultWindowTitle = _("Progres IDE");
    SetTitle(defaultWindowTitle);

    // Configuração default para o exececutável do simulador
    wxConfig *config = new wxConfig(_("ProgresIDE"));
    config->Read(_("SimuladorExePath"), &simuladorExePath);
    if (simuladorExePath.IsEmpty())
    {
        config->Write(_("SimuladorExePath"), _("progres"));
        simuladorExePath = _("progres");
    }
    delete config;

    carregaConfigs();

    if (AbrirUltimoAoIniciar) {
        if (!UltimoArquivoVerilog.IsEmpty()) {
            if (wxFile::Exists(UltimoArquivoVerilog))
                CarregarArquivoVerilog(UltimoArquivoVerilog);
        }
    }

    arquivoNaoSalvo = false;
    ondas = NULL;
}

IDEFrame::~IDEFrame()
{
    if (AbrirUltimoAoIniciar)
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
    wxConfig* config = new wxConfig(_("ProgresIDE"));

    config->Read(_("SimuladorExePath"), &simuladorExePath);

    if (config->Read(_("AbrirUltimoAoIniciar"), &AbrirUltimoAoIniciar))
        config->Read(_("UltimoArquivoVerilog"), &UltimoArquivoVerilog);
    else
        AbrirUltimoAoIniciar = false;

    delete config;

    if (simuladorExePath.IsEmpty())
        wxMessageBox(_("O executável do simulador não foi definido. Por favor selecione-o nas configurações."),
                     _("Aviso"));
}

void IDEFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void IDEFrame::OnAbout(wxCommandEvent& event)
{
    //wxString msg = _("Progres IDE\n(C) 2014 Tiago Matos\n\ntiagoms88@gmail.com");
    //wxMessageBox(msg, _("Progres"));

    wxAboutDialogInfo aboutInfo;

    aboutInfo.SetName(_("Progres IDE"));
    aboutInfo.SetVersion(_("0.1 alpha"));
    aboutInfo.SetDescription(_("Verilog Circuit Simulation"));
    aboutInfo.SetCopyright("(C) 2014-2015");
    aboutInfo.SetWebSite("https://github.com/tmatos/progres");
    aboutInfo.AddDeveloper("Tiago Matos");

    wxAboutBox(aboutInfo);
}

void IDEFrame::OnMenuItemOpen(wxCommandEvent& event)
{
    wxFileDialog FileDialogFonte(this,
                                 _("Selecionar arquivo"),
                                 _(""),
                                 _(""),
                                 _("Arquivos do Verilog (*.v)|*.v|Qualquer arquivo (*.*)|*.*"),
                                 wxFD_FILE_MUST_EXIST);

    if ( FileDialogFonte.ShowModal() == wxID_OK )
    {
        CarregarArquivoVerilog(FileDialogFonte.GetPath());
    }
}

void IDEFrame::CarregarArquivoVerilog(wxString arquivo)
{
    verilogFilePath = arquivo;

    EditBox->LoadFile(verilogFilePath);

    SetTituloJanelaComArquivo(verilogFilePath);
    bookFontes->SetPageText(0, verilogFilePath);
    bookFontes->ChangeSelection(0);

    textLenght = EditBox->GetNumberOfLines();
    wxString statusText;
    statusText << textLenght << " linhas";
    StatusBarPrincipal->SetStatusText(statusText, 1);

    ListBoxErros->Clear();

    arquivoNaoSalvo = false;
}

void IDEFrame::SetTituloJanelaComArquivo(wxString nome)
{
    SetTitle(defaultWindowTitle + _(" - ") + nome);
}

void IDEFrame::OnMenuItemAnalisarSelected(wxCommandEvent& event)
{
    if (arquivoNaoSalvo)
    {
        wxMessageBox(_("Salve o arquivo primeiro."),
                     _("Aviso"));
        return;
    }

    if ( verilogFilePath.IsEmpty() )
    {
        wxMessageBox(_("Inexiste arquivo de circuito aberto."),
                     _("Erro"),
                     wxICON_ERROR);
        return;
    }

    wxArrayString saida;

    wxString comando;

    comando << simuladorExePath << " " << verilogFilePath;

    if ( !waveinFilePath.IsEmpty() ) {
        comando << " " << waveinFilePath;
    }

    ListBoxErros->Append(_("DEBUG> EXECUTE: ") + comando);

    // synchronous
    int r = wxExecute(comando, saida, wxEXEC_SYNC, NULL);

    if ( r == -1 ) {
        wxMessageBox(_("Problema ao executar o comando do simulador"),
                     _("Erro"),
                     wxICON_ERROR);
        ListBoxErros->Append(_("DEBUG> FALHA: -1"));
        return;
    }

    ListBoxErros->Clear();

    for ( unsigned int i = 0 ; i < saida.Count() ; i++ ) {
        ListBoxErros->Append(saida[i]);
    }

    if (!waveinFilePath.IsEmpty())
    {
        Sinais* ondas_out = NULL;
        waveoutFilePath = waveinFilePath + _(".out");
        ondas_out = carregaArquivoSinais( waveoutFilePath.ToStdString().c_str() );

        if (ondas_out)
        {
            SinaisDrawPane* panel = new SinaisDrawPane(bookFontes);

            if ( bookFontes->GetPageCount() == 3 )
            {
              delete bookFontes->GetPage(2);
              bookFontes->RemovePage(2);
            }

            bookFontes->AddPage(panel, waveoutFilePath);
            bookFontes->ChangeSelection(bookFontes->GetPageCount() - 1);

            panel->setSinais(waveoutFilePath, false);
        }
        else {
            wxMessageBox(_("Sem valores de saída para a simulação."),
                         _("Erro"));
        }
    }
}

int IDEFrame::PerguntaSalvarArquivo()
{
    return wxMessageBox(_("O arquivo em edição ainda não foi salvo. Deseja salvá-lo?"),
                        _("Salvar arquivo"),
                        wxYES_NO | wxCANCEL,
                        this);
}

void IDEFrame::OnMenuItemNovoCircuitoSelected(wxCommandEvent& event)
{
    if (arquivoNaoSalvo)
    {
        int resp = PerguntaSalvarArquivo();

        if (resp == wxYES) {
          SalvarArquivoAtual();
        }
        else if (resp == wxCANCEL) {
          return;
        }
    }

    FecharArquivoAtual();
    bookFontes->ChangeSelection(0);
}

void IDEFrame::OnMenuItemEntradaNovoSelected(wxCommandEvent& event)
{
    wxTextFile novoArquivo;

    wxFileDialog NewEntradaDialog(this,
                                  _("Novo arquivo de entradas"),
                                  _(""),
                                  _(""),
                                  _("Arquivos de entrada (*.in)|*.in"),
                                  wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    if ( NewEntradaDialog.ShowModal() == wxID_OK )
    {
        if ( novoArquivo.Create( NewEntradaDialog.GetPath() ) )
        {
            novoArquivo.AddLine( _("entrada1 {}") );
            novoArquivo.Write();
            novoArquivo.Close();

            waveinFilePath = NewEntradaDialog.GetPath();
            waveoutFilePath.Clear();

            AtualizaTudoParaNovaEntrada( NewEntradaDialog.GetPath() );
        }
        else {
            wxMessageBox(_("Impossibilitado de criar o arquivo."),
                         _("Erro"));
        }
    }
}

void IDEFrame::OnMenuItemEntradaAbrirSelected(wxCommandEvent& event)
{
    wxFileDialog EntradaDialog(this,
                               _("Abrir"),
                               _(""),
                               _(""),
                               _("Arquivos de entrada (*.in)|*.in|Qualquer arquivo (*.*)|*.*"),
                               wxFD_FILE_MUST_EXIST);

    if ( EntradaDialog.ShowModal() == wxID_OK )
    {
        AtualizaTudoParaNovaEntrada(EntradaDialog.GetPath());
    }
}

void IDEFrame::AtualizaTudoParaNovaEntrada(wxString novoPathArquivoWaveIn)
{
    waveinFilePath = novoPathArquivoWaveIn;
    waveoutFilePath.Clear();

    SinaisDrawPane *panel = new SinaisDrawPane(bookFontes);

    //wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    //sizer->Add(panel, 1, wxEXPAND);
    //bookFontes->SetSizer(sizer);
    //bookFontes->SetAutoLayout(true);

    if(bookFontes->GetPageCount() == 2)
    {
        delete bookFontes->GetPage(1);
        bookFontes->RemovePage(1);
    }
    else if(bookFontes->GetPageCount() == 3)
    {
        delete bookFontes->GetPage(2);
        bookFontes->RemovePage(2);

        delete bookFontes->GetPage(1);
        bookFontes->RemovePage(1);
    }

    bookFontes->AddPage(panel, waveinFilePath);
    bookFontes->ChangeSelection(bookFontes->GetPageCount() - 1);

    panel->setSinais(waveinFilePath, true);
}

void IDEFrame::OnEditBoxText(wxCommandEvent& event)
{
    wxString statusText;
    textLenght = EditBox->GetNumberOfLines();
    statusText << textLenght << " linhas";
    StatusBarPrincipal->SetStatusText(statusText, 1);

    arquivoNaoSalvo = true;

    if ( verilogFilePath.IsEmpty() )
    {
        SetTituloJanelaComArquivo(_("Arquivo não salvo *"));
        bookFontes->SetPageText(0, _("Arquivo não salvo *"));
    }
    else {
        SetTituloJanelaComArquivo(verilogFilePath + _(" *"));
        bookFontes->SetPageText(0, verilogFilePath + _(" *"));
    }

//  if( event.GetId() != ID_LISTBOXERROS ) {
//      EditBox->SetStyle(0, EditBox->GetLastPosition(), wxTextAttr(*wxBLACK, *wxWHITE));
//  }
}

void IDEFrame::OnListBoxErrosDClick(wxCommandEvent& event)
{
    long line = 0; // primeira linha (origem em zero)

    wxString msg = ListBoxErros->GetString(ListBoxErros->GetSelection());

    wxStringTokenizer partes(msg, _(":"));

    if ( partes.HasMoreTokens() )
    {
        wxString token = partes.GetNextToken();

        if ( token.ToLong(&line) ) {
            line--; // reajuste da origem das linhas
        }
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
    if ( bookFontes->GetSelection() == 0 ) {
        EditBox->SelectAll();
        EditBox->SetFocus();
    }
}

void IDEFrame::OnMenuItemConfigSelected(wxCommandEvent& event)
{
    IDEConfig* janelaCfg = new IDEConfig(this);
    janelaCfg->Show();
}

void IDEFrame::OnMenuItemSave(wxCommandEvent& event)
{
    SalvarArquivoAtual();
}

void IDEFrame::SalvarArquivoAtual()
{
    wxFileDialog SaveDialog(this,
                            _("Salvar arquivo Verilog"),
                            _(""),
                            _(""),
                            _("Arquivos do Verilog (*.v)|*.v"),
                            wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    wxTextFile arquivo;

    if (verilogFilePath.IsEmpty())
    {
        if ( SaveDialog.ShowModal() == wxID_OK ) {
            wxString pathArquivo = SaveDialog.GetPath();

            if ( arquivo.Create(pathArquivo) ) {
                arquivo.AddLine(EditBox->GetValue());
                arquivo.Write();
                arquivo.Close();

                verilogFilePath = pathArquivo;
                SetTituloJanelaComArquivo(verilogFilePath);
                bookFontes->SetPageText(0, verilogFilePath);
                arquivoNaoSalvo = false;
            }
            else {
                wxMessageBox(_("Impossibilitado de salvar o arquivo."),
                             _("Erro"));
            }
        }
    }
    else
    {
        if (arquivo.Open(verilogFilePath))
        {
            arquivo.Clear();
            arquivo.AddLine(EditBox->GetValue());
            arquivo.Write();
            arquivo.Close();

            SetTituloJanelaComArquivo(verilogFilePath);
            bookFontes->SetPageText(0, verilogFilePath);
            arquivoNaoSalvo = false;
        }
        else {
            wxMessageBox(_("Impossibilitado de salvar o arquivo."),
                         _("Erro"));
        }
    }
}

void IDEFrame::OnMenuItemCloseSelected(wxCommandEvent& event)
{
    if (arquivoNaoSalvo)
    {
        int resp = PerguntaSalvarArquivo();

        if (resp == wxYES) {
            SalvarArquivoAtual();
        }
        else if (resp == wxCANCEL) {
            return;
        }
    }

    FecharArquivoAtual();
}

void IDEFrame::FecharArquivoAtual()
{
    ListBoxErros->Clear();
    EditBox->Clear();
    verilogFilePath.Clear();
    arquivoNaoSalvo = false;
    SetTitle(defaultWindowTitle);
    bookFontes->SetPageText(0, _(""));
}

void IDEFrame::OnMenuItemTesteSelected(wxCommandEvent& event)
{
}
