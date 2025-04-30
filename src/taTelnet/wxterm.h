/*
    taTelnet - A cross-platform telnet program.
    Copyright (c) 2000 Derry Bryson
                  2004 Mark Erikson
                  2012-2013 Jeremy Salwen

License: wxWindows License Version 3.1 (See the file license3.txt)

*/


#ifndef INCLUDE_WXTERM
#define INCLUDE_WXTERM

#ifdef __GNUG__
#pragma interface
#endif

#include <wx/colour.h>
#include <wx/dcmemory.h>
#include <wx/event.h>
#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/scrolwin.h>
#include <wx/string.h>
#include <wx/timer.h>
#include <wx/window.h>
#include "../GTerm/gterm.hpp"

#define wxEVT_COMMAND_TERM_RESIZE wxEVT_USER_FIRST + 1000
#define wxEVT_COMMAND_TERM_NEXT wxEVT_USER_FIRST + 1001

#define EVT_TERM_RESIZE(id, fn)                                                                    \
    {wxEVT_COMMAND_TERM_RESIZE, id, -1,                                                            \
     (wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction) & fn, (wxObject *)NULL},

class wxTerm : public wxScrolledWindow, public GTerm //wxScrolled<wxWindow>
{
    int m_charWidth, m_charHeight, m_init, m_width, m_height, m_selx1, m_sely1, m_selx2, m_sely2,
        m_curX, m_curY, m_curFG, m_curBG, m_curFlags, m_curState, m_curBlinkRate;

    int m_charsInLine;
    int m_linesDisplayed;

    unsigned char m_curChar;

    bool m_selecting, m_marking, m_autoscroll;

    bool m_inUpdateSize;

    wxColour m_vt_colors[16], m_pc_colors[16], *m_colors;

    wxPen m_vt_colorPens[16], m_pc_colorPens[16], *m_colorPens;

    wxFont m_normalFont, m_underlinedFont, m_boldFont, m_boldUnderlinedFont;

    wxDC *m_curDC;

    wxMemoryDC m_memDC;

    wxBitmap *m_bitmap;

    FILE *m_printerFN;

    char *m_printerName;

    wxTimer m_timer;

    wxLongLong m_blinkTimer;

public:
    enum BOLDSTYLE
    {
        BS_DEFAULT = -1,
        BS_COLOR = 0,
        BS_OVERSTRIKE = 1,
        BS_FONT = 2
    };

private:
    BOLDSTYLE
    m_boldStyle;

    typedef struct
    {
        wxKeyCode keyCode;

        int VTKeyCode;
    } TermKeyMap;

    static TermKeyMap keyMapTable[];
    static unsigned char xCharMap[];

public:
    wxTerm(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, int width = 80,
           int height = 24, const wxString &name = "wxTerm");

    virtual ~wxTerm();

    bool SetFont(const wxFont &font);

    void GetDefVTColors(wxColor colors[16], wxTerm::BOLDSTYLE boldStyle = wxTerm::BS_DEFAULT);
    void GetVTColors(wxColour colors[16]);
    void SetVTColors(wxColour colors[16]);
    void GetDefPCColors(wxColour colors[16]);
    void GetPCColors(wxColour colors[16]);
    void SetPCColors(wxColour colors[16]);
    int GetCursorBlinkRate() { return m_curBlinkRate; }
    void SetCursorBlinkRate(int rate);

    void SetBoldStyle(wxTerm::BOLDSTYLE boldStyle);
    wxTerm::BOLDSTYLE GetBoldStyle(void) { return m_boldStyle; }

    void ScrollTerminal(int numLines, bool scrollUp = true);

    void ClearSelection();
    bool HasSelection();
    wxString GetSelection();
    void SelectAll();

    void UpdateSize();
    void ScrollToBottom();
    // void UpdateSize(int &termheight, int &linesReceived);
    // void UpdateSize(wxSizeEvent &event);

    /*
    **  GTerm stuff
    */
    virtual void DrawText(int fg_color, int bg_color, int flags, int x, int y, int len,
                          unsigned char *string);
    virtual void DrawCursor(int fg_color, int bg_color, int flags, int x, int y, unsigned char c);

    virtual void MoveChars(int sx, int sy, int dx, int dy, int w, int h);
    virtual void ClearChars(int clear_bg_color, int x, int y, int w, int h);
    //  virtual void SendBack(int len, char *data);
    virtual void ModeChange(int state);
    virtual void Bell();
    virtual void ResizeTerminal(int w, int h);
    virtual void RequestSizeChange(int w, int h);

    virtual void ProcessInput(int len, unsigned char *data);
    //  virtual void ProcessOutput(int len, unsigned char *data);

    virtual void SelectPrinter(char *PrinterName);
    virtual void PrintChars(int len, unsigned char *data);

    virtual void Dirty();
    virtual void UpdateRemoteSize(int width, int height);
    int GetTermWidth() { return m_charsInLine; }
    int GetTermHeight() { return m_linesDisplayed; }
    int GetTermCharWidth() { return m_charWidth; }
    int GetTermCharHeight() { return m_charHeight; }
    bool CharPositionFromPoint(int x, int y, int &column, int &lineNumber);

    int MapKeyCode(int keyCode);
    void MarkSelection();
    void DoDrawCursor(int fg_color, int bg_color, int flags, int x, int y, unsigned char c);

    virtual void OnChar(wxKeyEvent &event);
    virtual void OnKeyDown(wxKeyEvent &event);
    virtual void OnPaint(wxPaintEvent &event);
    virtual void OnClearBg(wxEraseEvent &event);
    virtual void OnScroll(wxScrollWinEvent &event);
    virtual void OnLeftDown(wxMouseEvent &event);
    virtual void OnLeftUp(wxMouseEvent &event);
    virtual void OnRightDown(wxMouseEvent &event);
    virtual void OnMouseMove(wxMouseEvent &event);
    virtual void OnSize(wxSizeEvent &event);
    virtual void OnTimer(wxTimerEvent &event);

    virtual void OnGainFocus(wxFocusEvent &event);
    virtual void OnLoseFocus(wxFocusEvent &event);

    virtual void OnMenuCopy(wxCommandEvent &event);
    virtual void OnMenuPaste(wxCommandEvent &event);

private:

    DECLARE_EVENT_TABLE()
};

#endif /* INCLUDE_WXTERM */
