/***************************************************************
 * Name:      IDEMain.h
 * Purpose:   Defines Application Frame
 * Author:    Tiago Matos ()
 * Created:   2014-06-12
 * Copyright: Tiago Matos ()
 * License:   MIT
 **************************************************************/

#ifndef IDEMAIN_H
#define IDEMAIN_H

#include "sinais.h"

//(*Headers(IDEFrame)
#include <wx/notebook.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/splitter.h>
#include <wx/listbox.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class IDEFrame : public wxFrame
{
    public:
        IDEFrame(wxWindow* parent, wxWindowID id = -1);
        virtual ~IDEFrame();

        void carregaConfigs();
        void CarregarArquivoVerilog(wxString arquivo);
        void SetTituloJanelaComArquivo(wxString nome);
        void AtualizaTudoParaNovaEntrada(wxString novoPathArquivoWaveIn);

        void SalvarArquivoAtual();
        void FecharArquivoAtual();
        int PerguntaSalvarArquivo();

    private:
        //(*Handlers(IDEFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnMenuItemOpen(wxCommandEvent& event);
        void OnMenuItemAnalisarSelected(wxCommandEvent& event);
        void OnMenuItemNovoCircuitoSelected(wxCommandEvent& event);
        void OnMenuItemEntradaAbrirSelected(wxCommandEvent& event);
        void OnEditBoxText(wxCommandEvent& event);
        void OnListBoxErrosDClick(wxCommandEvent& event);
        void OnMenuItemSelecionarTudoSelected(wxCommandEvent& event);
        void OnMenuItemConfigSelected(wxCommandEvent& event);
        void OnMenuItemSave(wxCommandEvent& event);
        void OnMenuItemCloseSelected(wxCommandEvent& event);
        void OnMenuItemTesteSelected(wxCommandEvent& event);
        void OnMenuItemEntradaNovoSelected(wxCommandEvent& event);
        //*)

        //(*Identifiers(IDEFrame)
        static const long ID_TEXTCTRL_FONTE;
        static const long ID_NOTEBOOK1;
        static const long ID_LISTBOXERROS;
        static const long ID_SPLITTERWINDOW1;
        static const long ID_MENU_FILE_NEW_CIRCUIT;
        static const long ID_MENU_FILE_NEW;
        static const long ID_MENU_FILE_OPEN;
        static const long ID_MENU_FILE_SAVE;
        static const long ID_MENU_FILE_RECENTS_CLEAR;
        static const long ID_MENU_FILE_RECENTS;
        static const long ID_MENU_FILE_CLOSE;
        static const long ID_MENU_FILE_QUIT;
        static const long ID_MENU_EDIT_SELECTALL;
        static const long ID_MENUITEM_TESTE;
        static const long ID_MENUITEM_ENTRADA_NOVO;
        static const long ID_MENUITEM_ENTRADA_ABRIR;
        static const long ID_MENU_SIMULATION_ANALYSE;
        static const long ID_MENU_OPTIONS_CONFIG;
        static const long ID_MENU_HELP_ABOUT;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(IDEFrame)
        wxListBox* ListBoxErros;
        wxMenu* Menu1;
        wxMenu* Menu2;
        wxMenu* Menu3;
        wxMenu* Menu4;
        wxMenu* MenuItem2;
        wxMenu* MenuItem4;
        wxMenu* MenuOpcoes;
        wxMenuItem* MenuItem1;
        wxMenuItem* MenuItem3;
        wxMenuItem* MenuItem5;
        wxMenuItem* MenuItem6;
        wxMenuItem* MenuItemAnalisar;
        wxMenuItem* MenuItemClose;
        wxMenuItem* MenuItemConfig;
        wxMenuItem* MenuItemNovoCircuito;
        wxMenuItem* MenuItemSave;
        wxMenuItem* MenuItemSelecionarTudo;
        wxMenuItem* MenuItemTeste;
        wxNotebook* bookFontes;
        wxSplitterWindow* SplitterWindow1;
        wxStatusBar* StatusBarPrincipal;
        wxTextCtrl* EditBox;
        //*)

        wxString verilogFilePath;
        wxString waveinFilePath;
        wxString waveoutFilePath;

        wxString simuladorExePath;
        bool AbrirUltimoAoIniciar;
        wxString UltimoArquivoVerilog;
        wxString UltimoArquivoEntrada;

        wxString defaultWindowTitle;
        long textLenght;
        bool arquivoNaoSalvo;

        Sinais* ondas;

        DECLARE_EVENT_TABLE()
};

#endif // IDEMAIN_H
