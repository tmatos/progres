/********************************
 Progres - Verilog Simulator
 (C) 2014-2025 Tiago Matos

 Under terms of the MIT license.
*********************************/

#ifndef SINAISDRAWPANE_H
#define SINAISDRAWPANE_H

#include "wx/panel.h"

#include "sinais.h"
#include "EdicaoDeSinal.h"

class SinaisDrawPane : public wxPanel
{
public:
    SinaisDrawPane(wxWindow* parent);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();

    void render(wxDC& dc);

    void setSinais(wxString filePath, bool isInput);
    bool isInputFile;
    bool estaEmEdicao;
    wxString waveFilePath;

    void mouseDoubleClick(wxMouseEvent& event);
    void mouseRightClick(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    
    // eventos que podem ser uteis
    /*
    void mouseMoved(wxMouseEvent& event);
    void mouseDown(wxMouseEvent& event);
    void mouseWheelMoved(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    void rightClick(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);
    */

private:
    void drawGrid(wxDC& canvas, int x0, int y0, int height);
    void drawSignals(wxDC& canvas, int x0, int y0);

    Sinais* ondas;
    EdicaoDeSinal* editor;

    wxColour corDoTexto;
    wxColour corDaLinha;
    
    int horizontSize = 15; // comprimeto horizontal de uma unidade de tempo
    int verticalSize = 15; // altura de um pulso entre 0 e 1
    int signalSpacement = 30; // espacamento vertical entre os sinais

    int maxVerticalLines = 120;
        
    int guide = 0;
    
    DECLARE_EVENT_TABLE()
};

#endif // SINAISDRAWPANE_H
