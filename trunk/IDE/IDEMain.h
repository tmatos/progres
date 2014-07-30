/***************************************************************
 * Name:      IDEMain.h
 * Purpose:   Defines Application Frame
 * Author:    Tiago Matos ()
 * Created:   2014-06-12
 * Copyright: Tiago Matos ()
 * License:
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

class IDEFrame: public wxFrame
{
    public:

        IDEFrame(wxWindow* parent,wxWindowID id = -1);
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
        static const long ID_MENUITEM2;
        static const long ID_MENUITEM1;
        static const long idMenuOpen;
        static const long idMenuSave;
        static const long ID_MENUITEM8;
        static const long idMenuClose;
        static const long idMenuQuit;
        static const long ID_MENUITEM6;
        static const long ID_MENUITEM_TESTE;
        static const long ID_MENUITEM_ENTRADA_NOVO;
        static const long ID_MENUITEM_ENTRADA_ABRIR;
        static const long ID_MENUITEM4;
        static const long ID_MENUITEM7;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(IDEFrame)
        wxMenu* MenuItem2;
        wxMenuItem* MenuItemSave;
        wxMenuItem* MenuItemClose;
        wxMenuItem* MenuItem5;
        wxMenuItem* MenuItemNovoCircuito;
        wxTextCtrl* EditBox;
        wxMenu* Menu3;
        wxMenuItem* MenuItem1;
        wxMenuItem* MenuItem4;
        wxMenu* MenuOpcoes;
        wxNotebook* bookFontes;
        wxMenu* Menu1;
        wxMenuItem* MenuItem3;
        wxMenuItem* MenuItemTeste;
        wxMenuItem* MenuItemConfig;
        wxMenuItem* MenuItemAnalisar;
        wxSplitterWindow* SplitterWindow1;
        wxMenu* Menu2;
        wxStatusBar* StatusBarPrincipal;
        wxMenuItem* MenuItemSelecionarTudo;
        wxListBox* ListBoxErros;
        wxMenu* Menu4;
        //*)

        wxString verilogFilePath;
        wxString waveinFilePath;
        wxString waveoutFilePath;

        wxString simuladorExePath;
        bool AbrirUltimoAoIniciar;
        wxString UltimoArquivoVerilog;

        wxString defaultWindowTitle;
        long textLenght;
        bool arquivoNaoSalvo;

        Sinais* ondas;

        DECLARE_EVENT_TABLE()
};

#endif // IDEMAIN_H
