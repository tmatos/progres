
#ifndef SINAISDRAWPANE_H

#define SINAISDRAWPANE_H

#include "sinais.h"

class SinaisDrawPane : public wxPanel
{
public:
    SinaisDrawPane(wxWindow* parent);

    void paintEvent(wxPaintEvent & evt);
    void paintNow();

    void render(wxDC& dc);

    void setSinais(wxString filePath, bool isInput);
    bool isInputFile;
    wxString waveFilePath;

    void mouseDoubleClick(wxMouseEvent& event);

    // some useful events
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

    DECLARE_EVENT_TABLE()
};

#endif // SINAISDRAWPANE_H
