
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
    Sinais* ondas;
    EdicaoDeSinal* editor;

    wxColour corDoTexto;
    wxColour corDaLinha;
    
    int hzTam = 15;

    int guide = 0;
    
    DECLARE_EVENT_TABLE()
};

#endif // SINAISDRAWPANE_H
