#include "moPreviewWindow.h"

//(*InternalHeaders(moPreviewWindow)
#include <wx/bitmap.h>
#include <wx/intl.h>
#include <wx/image.h>
#include <wx/string.h>
//*)

//(*IdInit(moPreviewWindow)
const long moPreviewWindow::ID_BITMAPBUTTONPLAY = wxNewId();
const long moPreviewWindow::ID_BITMAPBUTTONPAUSE = wxNewId();
const long moPreviewWindow::ID_BITMAPBUTTONFULLSCREEN = wxNewId();
const long moPreviewWindow::ID_BITMAPBUTTONSCREENSHOT = wxNewId();
const long moPreviewWindow::ID_BITMAPBUTTONSAVEVIDEO = wxNewId();
const long moPreviewWindow::ID_BITMAPBUTTONSAVESESSION = wxNewId();
const long moPreviewWindow::ID_BITMAPBUTTONFOCUSOUTPUT = wxNewId();
//*)

BEGIN_EVENT_TABLE(moPreviewWindow,wxPanel)
	//(*EventTable(moPreviewWindow)
	//*)
END_EVENT_TABLE()

moPreviewWindow::moPreviewWindow(wxWindow* parent,wxWindowID id,const wxPoint& pos,const wxSize& size)
{
    wxInitAllImageHandlers();
	//(*Initialize(moPreviewWindow)
	Create(parent, id, wxDefaultPosition, wxSize(401,336), wxTAB_TRAVERSAL, _T("id"));
	SetForegroundColour(wxColour(255,255,255));
	SetBackgroundColour(wxColour(0,0,0));
	BitmapButtonPlay = new wxBitmapButton(this, ID_BITMAPBUTTONPLAY, wxBitmap(wxImage(_T("D:\\_data\\proyectos\\SINESTESIA\\OPENSVN\\trunk\\0.7\\doc\\icons\\play32.png"))), wxPoint(0,0), wxSize(32,32), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTONPLAY"));
	BitmapButtonPlay->SetDefault();
	BitmapButtonPause = new wxBitmapButton(this, ID_BITMAPBUTTONPAUSE, wxBitmap(wxImage(_T("D:\\_data\\proyectos\\SINESTESIA\\OPENSVN\\trunk\\0.7\\doc\\icons\\pause32.png"))), wxPoint(32,0), wxSize(32,32), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTONPAUSE"));
	BitmapButtonPause->SetDefault();
	ButtonFullSCreen = new wxBitmapButton(this, ID_BITMAPBUTTONFULLSCREEN, wxBitmap(wxImage(_T("D:\\_data\\proyectos\\SINESTESIA\\OPENSVN\\trunk\\0.7\\doc\\icons\\fullscreen32.png"))), wxPoint(96,0), wxSize(32,32), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTONFULLSCREEN"));
	ButtonFullSCreen->SetDefault();
	BitmapButtonScreenshot = new wxBitmapButton(this, ID_BITMAPBUTTONSCREENSHOT, wxBitmap(wxImage(_T("D:\\_data\\proyectos\\SINESTESIA\\OPENSVN\\trunk\\0.7\\doc\\icons\\screenshot32.png"))), wxPoint(312,0), wxSize(32,32), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTONSCREENSHOT"));
	BitmapButtonScreenshot->SetDefault();
	BitmapButtonSaveVideo = new wxBitmapButton(this, ID_BITMAPBUTTONSAVEVIDEO, wxBitmap(wxImage(_T("D:\\_data\\proyectos\\SINESTESIA\\OPENSVN\\trunk\\0.7\\doc\\icons\\savevideo32.png"))), wxPoint(344,0), wxSize(32,32), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTONSAVEVIDEO"));
	BitmapButtonSaveVideo->SetDefault();
	BitmapButtonSaveSession = new wxBitmapButton(this, ID_BITMAPBUTTONSAVESESSION, wxBitmap(wxImage(_T("D:\\_data\\proyectos\\SINESTESIA\\OPENSVN\\trunk\\0.7\\doc\\icons\\savesession32.png"))), wxPoint(64,0), wxSize(32,32), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTONSAVESESSION"));
	BitmapButtonFocusOutput = new wxBitmapButton(this, ID_BITMAPBUTTONFOCUSOUTPUT, wxNullBitmap, wxPoint(136,5), wxSize(24,23), wxBU_AUTODRAW, wxDefaultValidator, _T("ID_BITMAPBUTTONFOCUSOUTPUT"));
	BitmapButtonFocusOutput->SetDefault();

	Connect(ID_BITMAPBUTTONFULLSCREEN,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&moPreviewWindow::OnButtonFullSCreenClick);
	Connect(ID_BITMAPBUTTONFOCUSOUTPUT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&moPreviewWindow::OnBitmapButtonFocusAndActivate);
	//*)
	m_pGLCanvas = NULL;
}

moPreviewWindow::~moPreviewWindow()
{
	//(*Destroy(moPreviewWindow)
	//*)
}

void moPreviewWindow::Init( moIDirectorActions* pActionsHandler, wxGLContext* p_pGLContext   ) {

    SetNextActionHandler( pActionsHandler);

    if (p_pGLContext) {
        if (m_pGLCanvas==NULL)
            m_pGLCanvas = new moGLCanvas( this, p_pGLContext, -1, wxPoint(0,40), wxSize(400,300), 0, wxT("moGLCanvas"));
    } else {
        if (m_pGLCanvas==NULL)
            m_pGLCanvas = new moGLCanvas( this, -1, wxPoint(0,40), wxSize(400,300), 0, wxT("moGLCanvas"));
    }
    if (m_pGLCanvas)
        m_pGLCanvas->Init( this );
}

void moPreviewWindow::OnButtonFullSCreenClick(wxCommandEvent& event)
{
    FullScreen();
}

void moPreviewWindow::OnBitmapButtonFocusAndActivate(wxCommandEvent& event)
{
    FocusOutput();
}
