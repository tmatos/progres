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

//(*Headers(IDEFrame)
#include <wx/notebook.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <wx/splitter.h>
#include <wx/listbox.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

class IDEFrame: public wxFrame
{
    public:

        IDEFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~IDEFrame();

        void carregaConfigs();

    private:
        //(*Handlers(IDEFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnMenuItemOpen(wxCommandEvent& event);
        void OnMenuItemAnalisarSelected(wxCommandEvent& event);
        void OnMenuItemNovoCircuitoSelected(wxCommandEvent& event);
        void OnMenuItemNovaOndaSelected(wxCommandEvent& event);
        void OnEditBoxText(wxCommandEvent& event);
        void OnListBoxErrosDClick(wxCommandEvent& event);
        void OnMenuItemSelecionarTudoSelected(wxCommandEvent& event);
        void OnMenuItemConfigSelected(wxCommandEvent& event);
        //*)

        //(*Identifiers(IDEFrame)
        static const long ID_TEXTCTRL_FONTE;
        static const long ID_NOTEBOOK1;
        static const long ID_LISTBOXERROS;
        static const long ID_SPLITTERWINDOW1;
        static const long ID_MENUITEM2;
        static const long ID_MENUITEM3;
        static const long ID_MENUITEM1;
        static const long idMenuOpen;
        static const long ID_MENUITEM8;
        static const long idMenuQuit;
        static const long ID_MENUITEM6;
        static const long ID_MENUITEM4;
        static const long ID_MENUITEM5;
        static const long ID_MENUITEM7;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(IDEFrame)
        wxMenu* MenuItem2;
        wxMenuItem* MenuItemNovoCircuito;
        wxTextCtrl* EditBox;
        wxMenu* Menu3;
        wxMenuItem* MenuItem1;
        wxMenuItem* MenuItem4;
        wxMenu* MenuOpcoes;
        wxMenuItem* MenuItemNovaOnda;
        wxNotebook* bookFontes;
        wxMenu* Menu1;
        wxMenuItem* MenuItem3;
        wxMenuItem* MenuItemConfig;
        wxFileDialog* FileDialogFonte;
        wxMenuItem* MenuItemAnalisar;
        wxSplitterWindow* SplitterWindow1;
        wxMenu* Menu2;
        wxStatusBar* StatusBarPrincipal;
        wxMenuItem* MenuItemSelecionarTudo;
        wxListBox* ListBoxErros;
        //*)

        wxString verilogFilePath;
        wxString simuladorExePath;
        wxString defaultWindowTitle;
        long textLenght;

        DECLARE_EVENT_TABLE()
};

#endif // IDEMAIN_H
