
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/font.h>

#include "SinaisDrawPane.h"
#include "EdicaoDeSinal.h"
#include "sinais.h"
#include "inout.h"

BEGIN_EVENT_TABLE(SinaisDrawPane, wxPanel)

// some useful events
/*
 EVT_MOTION(SinaisDrawPane::mouseMoved)
 EVT_LEFT_DOWN(SinaisDrawPane::mouseDown)
 EVT_LEFT_UP(SinaisDrawPane::mouseReleased)
 EVT_RIGHT_DOWN(SinaisDrawPane::rightClick)
 EVT_LEAVE_WINDOW(SinaisDrawPane::mouseLeftWindow)
 EVT_KEY_DOWN(SinaisDrawPane::keyPressed)
 EVT_KEY_UP(SinaisDrawPane::keyReleased)
 EVT_MOUSEWHEEL(SinaisDrawPane::mouseWheelMoved)
 */

// captura os eventos do sistema para redesenho
EVT_PAINT(SinaisDrawPane::paintEvent)

// clique duplo no gráfico
EVT_LEFT_DCLICK(SinaisDrawPane::mouseDoubleClick)

END_EVENT_TABLE()

// some useful events
/*
 void SinaisDrawPane::mouseMoved(wxMouseEvent& event) {}
 void SinaisDrawPane::mouseDown(wxMouseEvent& event) {}
 void SinaisDrawPane::mouseWheelMoved(wxMouseEvent& event) {}
 void SinaisDrawPane::mouseReleased(wxMouseEvent& event) {}
 void SinaisDrawPane::rightClick(wxMouseEvent& event) {}
 void SinaisDrawPane::mouseLeftWindow(wxMouseEvent& event) {}
 void SinaisDrawPane::keyPressed(wxKeyEvent& event) {}
 void SinaisDrawPane::keyReleased(wxKeyEvent& event) {}
 */

void SinaisDrawPane::mouseDoubleClick(wxMouseEvent& event)
{
    if(isInputFile && !estaEmEdicao)
    {
        EdicaoDeSinal *editor = new EdicaoDeSinal(this);

        editor->setFile(waveFilePath);

        estaEmEdicao = true;

        editor->Show();
    }
}

SinaisDrawPane::SinaisDrawPane(wxWindow* parent) : wxPanel(parent)
{
    ondas = NULL;
    bool isInputFile = true;
    bool estaEmEdicao = false;
}

void SinaisDrawPane::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}

void SinaisDrawPane::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}

void SinaisDrawPane::setSinais(wxString filePath, bool isInput)
{
    ondas = carregaArquivoSinais( (const char*) filePath.mb_str() );

    isInputFile = isInput;
    waveFilePath = filePath;

    Refresh();
}

void SinaisDrawPane::render(wxDC&  canvas)
{
    if(!ondas)
    {
        canvas.SetPen(*wxRED);
        canvas.DrawText( _("Arquivo vazio ou com formato errado."), wxPoint(30, 30) );

        return;
    }

    int i, j, k;

    const int hzTam = 15; // comprimeto horizontal de uma unidade de tempo
    const int vrTam = 15; // altura de um pulso entre 0 e 1
    const int spacmtSinal = 30; // espaçamento vertical entre os sinais

    int x0 = 70, y0 = 30; // início do desenho
    int x = x0, y = y0;

    int yTexto = y0;

    // nomes das entradas
    canvas.SetTextForeground(*wxBLACK);

    for(i=0 ; i < ondas->quantidade ; i++)
    {
        canvas.DrawText( wxString::FromUTF8(ondas->lista[i].nome), wxPoint(5, yTexto) );
        yTexto = yTexto + spacmtSinal;
    }

    canvas.SetPen(*wxLIGHT_GREY_PEN);

    // linhas verticais do grid
    for(i=0 ; i < ondas->quantidade+1 ; i++)
    {
        canvas.DrawLine(    5, y0 + (i * spacmtSinal) - 7,
                          2000, y0 + (i * spacmtSinal) - 7 );
    }

    // linhas horizontais do grid e numeração
    canvas.SetTextForeground(*wxBLUE);

    wxFont fonte(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    canvas.SetFont(fonte);

    for(j=0 ; j < 120  ; j++)
    {
        if(j%5 == 0)
            canvas.DrawText( wxString::Format(_("%i"), j), wxPoint(x0 + (j*hzTam) + 2, 10 ) );

        canvas.DrawLine( x0 + (j*hzTam), 10,
                         x0 + (j*hzTam), yTexto+10 );
    }

    y = y0;

    canvas.SetPen(*wxBLACK_PEN);

    for(i=0 ; i < ondas->quantidade ; i++)
    {
        x = x0;

        Pulso* it = ondas->lista[i].pulsos;

        while(it->valor != nulo)
        {
            switch(it->valor)
            {
            case um:
                canvas.DrawLine(x, y,
                                 x + (hzTam * it->tempo), y);
                break;
            case zero:
                canvas.DrawLine(x, (y + vrTam),
                                 x + (hzTam * it->tempo), (y + vrTam));
                break;
            case xis:
                for(k=0 ; k < it->tempo ; k++)
                {
                    canvas.DrawLine((x + (k*hzTam)), y,
                                     (x + hzTam + (k*hzTam)), (y + vrTam));

                    canvas.DrawLine((x + (k*hzTam)), (y + vrTam),
                                     (x + hzTam + (k*hzTam)), y);
                }
                break;
            }

            x = x + hzTam * it->tempo;

            it++;
        }

        y = y + spacmtSinal;
    }

}
