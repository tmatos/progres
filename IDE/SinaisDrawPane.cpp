/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>

#include "SinaisDrawPane.h"
#include "EdicaoDeSinal.h"
#include "sinais.h"
#include "inout.h"
#include "util.h"

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

// clique duplo no grafico
EVT_LEFT_DCLICK(SinaisDrawPane::mouseDoubleClick)

// clique simples no grafico com btn direito
EVT_RIGHT_UP(SinaisDrawPane::mouseRightClick)

// clique simples no grafico com btn esquerdo
EVT_LEFT_UP(SinaisDrawPane::mouseReleased)

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

void SinaisDrawPane::mouseReleased(wxMouseEvent& event)
{
    wxClientDC dc(this);
    wxPoint p = event.GetLogicalPosition(dc);

    guide = p.x;

    Refresh();
}

void SinaisDrawPane::mouseRightClick(wxMouseEvent& event)
{
    wxClientDC dc(this);
    wxPoint p = event.GetLogicalPosition(dc);

    wxString msg;
    msg << "x = " << p.x << " ; y = " << p.y;

    wxMessageBox(msg, _("DEBUG"));
}

void SinaisDrawPane::mouseDoubleClick(wxMouseEvent& event)
{
    if ( isInputFile && !estaEmEdicao )
    {
        editor->setFile(waveFilePath);
        estaEmEdicao = true;
        editor->Show();
    }
}

SinaisDrawPane::SinaisDrawPane(wxWindow* parent) : wxPanel(parent)
{
    ondas = NULL;
    estaEmEdicao = false;

    editor = new EdicaoDeSinal(this);
    
    wxSystemSettings sisConfig;
    
    wxColour corDeFundoSis = sisConfig.GetColour(wxSYS_COLOUR_BACKGROUND);
    wxColour preto("black");
    wxColour branco("white");
    
    if ( corDeFundoSis.Red() > 100 &&
         corDeFundoSis.Green() > 100 &&
         corDeFundoSis.Blue() > 100 )
    {
        corDoTexto = preto;
        corDaLinha = preto;
    }
    else
    {
        corDoTexto = branco;
        corDaLinha = branco;
    }
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
    ondas = load_signals_from_path( (const char*) filePath.mb_str() );
    isInputFile = isInput;
    waveFilePath = filePath;
    Refresh();
}

void SinaisDrawPane::render(wxDC&  canvas)
{
    canvas.Clear();

    if (!ondas)
    {
        canvas.SetPen(*wxRED);
        canvas.DrawText( _("Arquivo vazio ou com formato errado."),
                         wxPoint(30, 30) );
        return;
    }

    int i;
    int j;
    unsigned int k;
    int l;

    hzTam = 15; // comprimeto horizontal de uma unidade de tempo
    const int vrTam = 15; // altura de um pulso entre 0 e 1
    const int spacmtSinal = 30; // espaçamento vertical entre os sinais

    // drawing start coords
    int x0 = 70;
    int y0 = 30; 

    int x = x0;
    int y = y0;

    int yTexto = y0;

    // nomes das entradas
    canvas.SetTextForeground(corDoTexto);

    for ( i=0 ; i < ondas->quantidade ; i++ )
    {
        canvas.DrawText( wxString::FromUTF8(ondas->lista[i].nome),
                         wxPoint(5, yTexto) );
        yTexto = yTexto + spacmtSinal;
    }

    canvas.SetPen(*wxLIGHT_GREY_PEN);

    // linhas horizontais do grid
    for ( i=0 ; i < ondas->quantidade+1 ; i++ )
    {
        canvas.DrawLine(    5, y0 + (i * spacmtSinal) - 7,
                         2000, y0 + (i * spacmtSinal) - 7 );
    }

    // linhas verticais do grid e numeração
    canvas.SetTextForeground(*wxBLUE);

    wxFont font(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    canvas.SetFont(font);

    for ( j=0 ; j < 120  ; j++ )
    {
        if ( j%5 == 0 )
        {
            canvas.DrawText( wxString::Format(_("%i"), j),
                             wxPoint(x0 + (j*hzTam) + 2, 10 ) );
        }

        canvas.DrawLine( x0 + (j*hzTam), 10,
                         x0 + (j*hzTam), yTexto+10 );
    }

    y = y0;

    canvas.SetPen(wxPen(corDaLinha));

    for ( i=0 ; i < ondas->quantidade ; i++ )
    {
        x = x0;

        Pulso* it = ondas->lista[i].pulsos;

        while ( it->valor != VAL_BLANK )
        {
            switch (it->valor)
            {
            case VAL_1:
                canvas.DrawLine( x                      , y,
                                 x + (hzTam * it->tempo), y );
                break;
            case VAL_0:
                canvas.DrawLine( x                      , y + vrTam,
                                 x + (hzTam * it->tempo), y + vrTam );
                break;
            case VAL_X:
                for ( k=0; k < it->tempo; k++ )
                {
                    canvas.DrawLine( x + hzTam*(k)      , y,
                                     x + hzTam*(1 + k), y + vrTam );

                    canvas.DrawLine( x + hzTam*(k)    , y + vrTam,
                                     x + hzTam*(1 + k), y );
                }
                break;
            case VAL_Z:
                for ( k=0; k < it->tempo; k++ )
                {
                    for ( l=0; l < hzTam; l+=2 )
                    {
                        canvas.DrawLine( x + k*(hzTam) + l, y,
                                         x + k*(hzTam) + l, y + vrTam - 1 );
                    }

                    for ( l=1; l < hzTam; l+=2 )
                    {
                        canvas.DrawLine( x + k*(hzTam) + l, y,
                                         x + k*(hzTam) + l, y + vrTam - 3 );
                    }
                }
               break;
            case VAL_BLANK:
               break;
            }

            x = x + (hzTam * it->tempo);

            it++;
        }

        y = y + spacmtSinal;
    }

    if (guide > x0)
    {
        canvas.SetPen(*wxGREEN_PEN);
        canvas.DrawLine( guide, 1,
                         guide, 700 );
    }

}
