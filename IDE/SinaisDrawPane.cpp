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
#include "estruturas.h"
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

    // drawing start coordinates just for signals area
    int x0 = 70;
    int y0 = 30;

    int yTexto = y0;

    // draw signal names
    canvas.SetTextForeground(corDoTexto);
    for ( int i=0 ; i < ondas->quantidade ; i++ )
    {
        canvas.DrawText( wxString::FromUTF8(ondas->lista[i].nome),
                         wxPoint(5, yTexto) );
        yTexto = yTexto + signalSpacement;
    }

    drawGrid(canvas, x0, y0, yTexto);

    drawSignals(canvas, x0, y0);

    if (guide > x0) // draw vertical guide line defined by mouse click
    {
        canvas.SetPen(*wxGREEN_PEN);
        canvas.DrawLine( guide, 1,
                         guide, 700 );
    }
}

void SinaisDrawPane::drawGrid(wxDC& canvas, int x0, int y0, int height)
{
    int xCoord;
    int yCoord;
    int yOffset;
    int maxHorizonLineSize = maxVerticalLines * horizontSize;

    canvas.SetPen(*wxLIGHT_GREY_PEN); // grid line colors

    // draw horizontal lines
    for ( int i=0 ; i < (ondas->quantidade + 1) ; i++ )
    {
        yCoord = y0 + (i * signalSpacement) - 7;
        canvas.DrawLine(                  0, yCoord,
                         maxHorizonLineSize, yCoord );
    }

    // font for the time axis numbering
    wxFont font(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    canvas.SetFont(font);
    canvas.SetTextForeground(*wxBLUE);

    // draw vertical lines and time axis numbering
    for ( int j=0 ; j < maxVerticalLines  ; j++ )
    {
        xCoord = x0 + (j * horizontSize);
        yOffset = 10;

        if ( j%5 == 0 )
        {
            canvas.DrawText( wxString::Format(_("%i"), j),
                             wxPoint(xCoord + 2, yOffset ) );
        }

        canvas.DrawLine( xCoord, yOffset,
                         xCoord, height );
    }
}

void SinaisDrawPane::drawSignals(wxDC& canvas, int x0, int y0)
{
    int x;
    int y;
    int horizontalIncrement;
    Pulso* it;

    canvas.SetPen(wxPen(corDaLinha));
    y = y0;

    for ( int i=0 ; i < ondas->quantidade ; i++ )
    {
        x = x0;
        it = ondas->lista[i].pulsos;

        while ( it->valor != VAL_BLANK )
        {
            horizontalIncrement = horizontSize * it->tempo;

            switch (it->valor)
            {
            case VAL_1:
                canvas.DrawLine( x                      , y,
                                 x + horizontalIncrement, y );
                break;
            case VAL_0:
                canvas.DrawLine( x                      , y + verticalSize,
                                 x + horizontalIncrement, y + verticalSize );
                break;
            case VAL_H:
                canvas.DrawLine( x                      , y,
                                 x + horizontalIncrement, y );
                break;
            case VAL_L:
                canvas.DrawLine( x                      , y + verticalSize,
                                 x + horizontalIncrement, y + verticalSize );
                break;
            case VAL_X:
                for ( unsigned int k=0 ; k < it->tempo ; k++ )
                {
                    canvas.DrawLine( x + horizontSize*(k)    , y,
                                     x + horizontSize*(1 + k), y + verticalSize );

                    canvas.DrawLine( x + horizontSize*(k)    , y + verticalSize,
                                     x + horizontSize*(1 + k), y );
                }
                break;
            case VAL_Z:
                for ( unsigned int k=0 ; k < it-> tempo; k++ )
                {
                    for ( int l=0 ; l < horizontSize ; l += 2 )
                    {
                        canvas.DrawLine( x + k*(horizontSize) + l, y,
                                         x + k*(horizontSize) + l, y + verticalSize - 1 );
                    }

                    for ( int l=1 ; l < horizontSize ; l+=2 )
                    {
                        canvas.DrawLine( x + k*(horizontSize) + l, y,
                                         x + k*(horizontSize) + l, y + verticalSize - 3 );
                    }
                }
               break;
            case VAL_BLANK:
               break;
            }

            x = x + horizontalIncrement;
            it++;
        }

        y = y + signalSpacement;
    }
}
