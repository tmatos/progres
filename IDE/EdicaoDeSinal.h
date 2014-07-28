#ifndef EDICAODESINAL_H
#define EDICAODESINAL_H

//(*Headers(EdicaoDeSinal)
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

class EdicaoDeSinal: public wxDialog
{
    public:

        EdicaoDeSinal(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
        virtual ~EdicaoDeSinal();

        void setFile(wxString filePath);

		//(*Declarations(EdicaoDeSinal)
		wxTextCtrl* txtWaveIn;
		wxButton* btnDescartar;
		wxButton* btnSalvar;
		//*)

	protected:

		//(*Identifiers(EdicaoDeSinal)
		static const long ID_TEXTCTRL1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:

		//(*Handlers(EdicaoDeSinal)
		void OnbtnDescartarClick(wxCommandEvent& event);
		void OnbtnSalvarClick(wxCommandEvent& event);
		//*)

        wxString file;

		DECLARE_EVENT_TABLE()
};

#endif
