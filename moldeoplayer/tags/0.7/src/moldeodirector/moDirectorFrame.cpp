#include "moDirectorFrame.h"

#include "moNewProject.h"
#include "moNewEffectDialog.h"

moDataNotebook::moDataNotebook( wxWindow* parent, wxWindowID id ) :
		wxAuiNotebook(parent, id, wxPoint(0,0), wxSize(100,100),  wxBORDER_NONE | wxAUI_NB_TAB_SPLIT | wxAUI_NB_CLOSE_ON_ALL_TABS | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TAB_MOVE ),
		m_pVirtualDirTreeCtrl(NULL),
		m_pProjectTreeCtrl(NULL),
		m_pIODevicesTreeCtrl(NULL),
		m_pResourcesTreeCtrl(NULL) {


}

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions(event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(moDirectorFrame, wxFrame)
	EVT_MENU( MODIRECTOR_NEWEFFECT, moDirectorFrame::OnNewEffect )
	EVT_MENU( MODIRECTOR_NEWPREEFFECT, moDirectorFrame::OnNewEffect )
	EVT_MENU( MODIRECTOR_NEWPOSTEFFECT, moDirectorFrame::OnNewEffect )
	EVT_MENU( MODIRECTOR_NEWIODEVICE, moDirectorFrame::OnNewEffect )
	EVT_MENU( MODIRECTOR_NEWRESOURCE, moDirectorFrame::OnNewEffect )

    EVT_MENU( MODIRECTOR_NEWPROJECT, moDirectorFrame::OnNewProject )
    EVT_MENU( MODIRECTOR_OPENPROJECT, moDirectorFrame::OnOpenProject )
	EVT_MENU( MODIRECTOR_CLOSEPROJECT, moDirectorFrame::OnCloseProject )
	EVT_MENU( MODIRECTOR_SAVEPROJECT, moDirectorFrame::OnSaveProject )

	EVT_MENU( MODIRECTOR_SAVEMOB, moDirectorFrame::OnSaveMob )
	EVT_MENU( MODIRECTOR_CLOSEMOB, moDirectorFrame::OnCloseMob )
	EVT_MENU( MODIRECTOR_OPENMOB, moDirectorFrame::OnOpenMob )

	EVT_MENU( MODIRECTOR_SAVEALL, moDirectorFrame::OnSaveAll )
	EVT_MENU( MODIRECTOR_CLOSEALL, moDirectorFrame::OnCloseAll )

	EVT_MENU( MODIRECTOR_QUIT,  moDirectorFrame::OnQuit )
    EVT_MENU( MODIRECTOR_ABOUT, moDirectorFrame::OnAbout )

	EVT_MENU( MODIRECTOR_PROJECT_PREVIEW, moDirectorFrame::OnProjectPreview )
	EVT_MENU( MODIRECTOR_PROJECT_PREVIEW_FULLSCREEN, moDirectorFrame::OnProjectPreviewFullscreen )
	EVT_MENU( MODIRECTOR_FULLSCREEN, moDirectorFrame::OnFullscreen )

	EVT_KEY_DOWN( moDirectorFrame::OnKeyDown )
	EVT_COMMAND( -1, MODIRECTOR_COMMAND, moDirectorFrame::OnCommand)
	EVT_CLOSE( moDirectorFrame::OnClose )
END_EVENT_TABLE()

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
moDirectorFrame::moDirectorFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxPoint(0,0), wxSize(1024,768))
{

    m_cForeground = wxColour(255,255,255);
    m_cBackground = wxColour(80,80,80);

    // set the frame icon
    //SetIcon(wxICON(sample));

	FrameManager.SetManagedWindow(this);

#if wxUSE_MENUS
    // create a menu bar

	//FILE
    wxMenu *fileMenu = new wxMenu;

	wxMenuItem *preeffectsMenuItem = NULL;
	wxMenu *preeffectsMenu = new wxMenu;

	wxMenu *effectsMenu = new wxMenu;
	wxMenu *posteffectsMenu = new wxMenu;
	wxMenu *iodevicesMenu = new wxMenu;
	wxMenu *parameterMenu = new wxMenu;

	fileMenu->Append( MODIRECTOR_NEWPROJECT, _T("&New Project\tAlt-N"), _T("Create a new project"));
	fileMenu->Append( MODIRECTOR_OPENPROJECT, _T("&Open Project\tAlt-O"), _T("Open an existing project"));
	fileMenu->Append( MODIRECTOR_CLOSEPROJECT, _T("&Close Project"), _T("Close the current project"));
	fileMenu->Append( MODIRECTOR_CLOSEMOB, _T("&Close Mob"), _T("Close the current mob file"));

    fileMenu->AppendSeparator();
    fileMenu->Append( MODIRECTOR_SAVEPROJECT, _T("Save Project\tCtrl-S"), _T("Save the current project"));
    fileMenu->Append( MODIRECTOR_SAVEMOB, _T("Save Object\tAlt-S"), _T("Save the current object"));
    fileMenu->Append( MODIRECTOR_SAVEALL, _T("Save All\tCtrl-A"), _T("Save All"));

	fileMenu->AppendSeparator();
    fileMenu->Append(MODIRECTOR_NEWPREEFFECT, _T("Add P&reEffect\tAlt-W"), _T("Add a new pre-effect"));
	fileMenu->Append(MODIRECTOR_NEWEFFECT, _T("Add &Effect\tAlt-E"), _T("Add a new effect"));
	fileMenu->Append(MODIRECTOR_NEWPOSTEFFECT, _T("Add P&ostEffect\tAlt-R"), _T("Add a new post-effect"));

    fileMenu->AppendSeparator();
    fileMenu->Append(MODIRECTOR_NEWIODEVICE, _T("Add IODevice"), _T("Add IODevice"));
	fileMenu->Append(MODIRECTOR_NEWRESOURCE, _T("Add Resource"), _T("Add Resource"));


	fileMenu->Append(MODIRECTOR_QUIT, _T("E&xit\tAlt-X"), _T("Quit this program"));
	//EDIT
	wxMenu *editMenu = new wxMenu;

	//VIEW
	wxMenu *viewMenu = new wxMenu;
	viewMenu->Append( MODIRECTOR_FULLSCREEN, _T("&Interface Fullscreen\tAlt-F11"), _T("Interface Fullscreen"));
	viewMenu->Append( MODIRECTOR_PROJECT_PREVIEW, _T("&Preview\tAlt-P"), _T("View frame preview"));
	viewMenu->Append( MODIRECTOR_PROJECT_PREVIEW_FULLSCREEN, _T("&Toggle Preview Fullscreen\tAlt-Return"), _T("Frame preview fullscreen"));

	//OPTIONS
	wxMenu *optionsMenu = new wxMenu;

    //HELP
    wxMenu *helpMenu = new wxMenu;

    helpMenu->Append(MODIRECTOR_ABOUT, _T("&About...\tF1"), _T("Show about dialog"));


    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(fileMenu, _T("&File"));
	menuBar->Append(editMenu, _T("&Edit"));
	menuBar->Append(viewMenu, _T("&View"));
	menuBar->Append(optionsMenu, _T("&Options"));

    menuBar->Append(helpMenu, _T("&Help"));

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
    // create a status bar just for fun(by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText(_T("Welcome to wxWidgets!"));
#endif // wxUSE_STATUSBAR

    CreateDataBook();
    CreateGLWindows();
    CreateInspector();
    CreateLayerControls();
    CreateProjectPanel();
    CreateFilesBook();


	m_pLogTextCtrl = CreateTextCtrl();
	m_pLogTextCtrl->SetForegroundColour(m_cForeground);
    m_pLogTextCtrl->SetBackgroundColour(m_cBackground);


	FrameManager.AddPane( m_pDataNotebook,      wxAuiPaneInfo().Name(wxT("explorer")).Caption(wxT("Explorer")).CenterPane().Hide().CaptionVisible().Floatable().Movable().Dockable());
	FrameManager.AddPane( m_pFilesBook,         wxAuiPaneInfo().Name(wxT("filesbook")).CenterPane().Hide().Caption(wxT("Parameters")).CaptionVisible().Floatable().Movable().Dockable());
	FrameManager.AddPane( m_pInspectorNotebook, wxAuiPaneInfo().Name(wxT("inspector")).CenterPane().Hide().Caption(wxT("Inspector")).CaptionVisible().Floatable().Movable().Dockable());
	FrameManager.AddPane( m_pLayersPanelCtrl,   wxAuiPaneInfo().Name(wxT("layers")).CenterPane().Hide().Caption(wxT("Layers")).CaptionVisible().Floatable().Movable().Dockable());
	FrameManager.AddPane( m_pLogTextCtrl,       wxAuiPaneInfo().Name(wxT("log")).CenterPane().Hide().Caption(wxT("Log")).CaptionVisible().Floatable().Movable().Dockable());



	FrameManager.AddPane( m_pPreviewWindow, wxAuiPaneInfo().Name(wxT("preview")).Caption(wxT("Console")).CaptionVisible().Floatable().Movable().Dockable().MaximizeButton().MinimizeButton().Resizable(true));

	//LEFT PANE
	FrameManager.GetPane(wxT("explorer")).Show().Left().Layer(1).Row(0).Position(0);
	FrameManager.GetPane(wxT("log")).Show().Left().Layer(1).Row(0).Position(1);

	//CENTER PANE
	FrameManager.GetPane(wxT("preview")).Show().Center().Top().Layer(0).Row(0).Position(0);
	FrameManager.GetPane(wxT("layers")).Show().Center().Layer(0).Row(0).Position(0);

	//RIGHT PANE
	FrameManager.GetPane(wxT("inspector")).Show().Right().Layer(1).Row(0).Position(0);
	FrameManager.GetPane(wxT("filesbook")).Show().Right().Layer(1).Row(0).Position(1);

    wxAuiDockArt* pFrameArt;

    pFrameArt = FrameManager.GetArtProvider();
    pFrameArt->SetColor( wxAUI_DOCKART_SASH_COLOUR, wxColour(80,80,80) );
    pFrameArt->SetColor( wxAUI_DOCKART_BORDER_COLOUR, wxColour(255,255,255) );
    pFrameArt->SetColor( wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR, wxColour(10,40,120) );
    pFrameArt->SetColor( wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR, wxColour(0,0,0) );
    pFrameArt->SetColor( wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR, wxColour(255,255,255) );
    //pFrameArt->SetColor( wxAUI_DOCKART_SASH_COLOUR, wxColour(255*30/100,255*30/100,255*30/100 );
    //pFrameArt->SetColor( wxAUI_DOCKART_SASH_COLOUR, wxColour(255*30/100,255*30/100,255*30/100 );
    //pFrameArt->SetColor( wxAUI_DOCKART_SASH_COLOUR, wxColour(255*30/100,255*30/100,255*30/100 );
    //moTabArt*   pTabArt = new moTabArt();
    wxAuiTabArt*    pTabArt = m_pInspectorNotebook->GetArtProvider();
    wxFont* pFont = new wxFont( 7, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
    pTabArt->SetNormalFont(*pFont);

    //m_pInspectorNotebook->SetArtProvider(pTabArt);

	FrameManager.Update();

}

moDirectorFrame::~moDirectorFrame() {

    m_pGLCanvas = NULL;
	FrameManager.UnInit();

}

void
moDirectorFrame::CreateDataBook() {

	m_pDataNotebook = new moDataNotebook( this, wxID_ANY);
	if (m_pDataNotebook) {
		m_pDataNotebook->SetForegroundColour(m_cForeground);
        m_pDataNotebook->SetBackgroundColour(m_cBackground);

		m_pDataNotebook->m_pVirtualDirTreeCtrl = new wxVirtualDirTreeCtrl(m_pDataNotebook, wxID_ANY, wxPoint(0,0), wxSize(300,377),wxTR_DEFAULT_STYLE | wxNO_BORDER | wxBORDER_NONE );
		m_pDataNotebook->m_pVirtualDirTreeCtrl->SetForegroundColour(m_cForeground);
        m_pDataNotebook->m_pVirtualDirTreeCtrl->SetBackgroundColour(m_cBackground);

		m_pDataNotebook->m_pProjectTreeCtrl = new moProjectTreeCtrl(m_pDataNotebook, wxID_ANY, wxPoint(0,0), wxSize(300,377), wxTR_DEFAULT_STYLE | wxNO_BORDER | wxBORDER_NONE);
		m_pDataNotebook->m_pProjectTreeCtrl->SetForegroundColour(m_cForeground);
        m_pDataNotebook->m_pProjectTreeCtrl->SetBackgroundColour(m_cBackground);
		m_pDataNotebook->m_pProjectTreeCtrl->SetDirectorFrame( this );

        m_pDataNotebook->m_pIODevicesTreeCtrl = new moProjectTreeCtrl(m_pDataNotebook, wxID_ANY, wxPoint(0,0), wxSize(300,377), wxTR_DEFAULT_STYLE | wxNO_BORDER | wxBORDER_NONE);
		m_pDataNotebook->m_pIODevicesTreeCtrl->SetForegroundColour(m_cForeground);
        m_pDataNotebook->m_pIODevicesTreeCtrl->SetBackgroundColour(m_cBackground);
		m_pDataNotebook->m_pIODevicesTreeCtrl->SetDirectorFrame( this );

        m_pDataNotebook->m_pResourcesTreeCtrl = new moProjectTreeCtrl(m_pDataNotebook, wxID_ANY, wxPoint(0,0), wxSize(300,377), wxTR_DEFAULT_STYLE | wxNO_BORDER | wxBORDER_NONE);
		m_pDataNotebook->m_pResourcesTreeCtrl->SetForegroundColour(m_cForeground);
        m_pDataNotebook->m_pResourcesTreeCtrl->SetBackgroundColour(m_cBackground);
		m_pDataNotebook->m_pResourcesTreeCtrl->SetDirectorFrame( this );

		m_pDataNotebook->AddPage( m_pDataNotebook->m_pProjectTreeCtrl, wxT("Project") );
		m_pDataNotebook->GetPage(0)->SetBackgroundColour(m_cBackground);
		m_pDataNotebook->AddPage( m_pDataNotebook->m_pIODevicesTreeCtrl, wxT("IODevices") );
		m_pDataNotebook->GetPage(1)->SetBackgroundColour(m_cBackground);
        m_pDataNotebook->AddPage( m_pDataNotebook->m_pResourcesTreeCtrl, wxT("Resources") );
		m_pDataNotebook->GetPage(2)->SetBackgroundColour(m_cBackground);
		m_pDataNotebook->AddPage( m_pDataNotebook->m_pVirtualDirTreeCtrl, wxT("Data") );
		m_pDataNotebook->GetPage(3)->SetBackgroundColour(m_cBackground);
	}


}

void
moDirectorFrame::CreateFilesBook() {

	m_pFilesBook = NULL;
	m_pFilesBook = new moFilesbook( this, wxID_ANY );

}

void
moDirectorFrame::CreateGLWindows() {

   int attribList[2];
    attribList[0] = WX_GL_RGBA; //true color
    attribList[1] = 0;

    m_pGLContext = NULL;
    //create base wxGLCanvas to create context...
    //retreive context...
    //m_pBaseGLCanvas = (wxGLCanvas*)new moGLCanvas( this, wxID_ANY, &attribList[0], wxPoint(0,0), wxSize(800,600) );
    //m_pGLContext = m_pBaseGLCanvas->GetContext();
    //m_pBaseGLCanvas->Destroy();

    //create preview window, dockable...

	m_pPreviewWindow = NULL;
	m_pPreviewWindow = new moPreviewWindow( this, wxID_ANY );

	if (m_pPreviewWindow) {

        m_pPreviewWindow->Init( this, m_pGLContext );
        m_pGLCanvas = m_pPreviewWindow->m_pGLCanvas;
        if (m_pGLCanvas) {
            m_pGLContext = m_pGLCanvas->GetContext();
            m_pGLCanvas->SetCurrent();
        }

	}

	m_pPreviewFrame = NULL;

}

void
moDirectorFrame::CreateProjectPanel() {


}

void
moDirectorFrame::CreateLayerControls() {

	m_pLayers2 = m_pLayers = NULL;
	m_pLayersPanelCtrl = new moLayersPanelCtrl(this, wxID_ANY, wxPoint(0,0), wxSize(440,220), wxSP_NOBORDER | wxNO_BORDER );
	if (m_pLayersPanelCtrl) {
        m_pLayers = new moTreeObjectsLayerCtrl( m_pLayersPanelCtrl, wxID_ANY );
        m_pLayers->SetForegroundColour(m_cForeground);
        m_pLayers->SetBackgroundColour( wxColour(0,0,0) );
        m_pLayers->ShowScrollBars( true, false);

        m_pLayers2 = new moTreeObjectsLayerCtrl( m_pLayersPanelCtrl, wxID_ANY );
        m_pLayers2->SetForegroundColour(m_cForeground);
        m_pLayers2->SetBackgroundColour( wxColour(52,52,52) );
        m_pLayers2->ShowScrollBars( true, true);
        m_pLayers2->SetSecondTargetWindow( m_pLayers );
        m_pLayers->SetSecondTargetWindow( m_pLayers2 );

        m_pLayersPanelCtrl->SplitVertically( m_pLayers, m_pLayers2 );
	}

}


void
moDirectorFrame::CreateInspector() {

	m_pInspectorNotebook = new wxAuiNotebook( this, wxID_ANY, wxPoint(0,0), wxSize(300,200), wxBORDER_NONE | wxAUI_NB_TAB_SPLIT | wxAUI_NB_CLOSE_ON_ALL_TABS | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TAB_MOVE  );
	m_pInspectorNotebook->SetForegroundColour(m_cForeground);
    m_pInspectorNotebook->SetBackgroundColour(m_cBackground);

    m_pPanelTexture = new moPanelTexture( m_pInspectorNotebook, wxID_ANY );
    m_pPanelTexture->SetNextActionHandler( this );

    m_pObjectMotion = new moObjectMotion( m_pInspectorNotebook, wxID_ANY );
    m_pObjectMotion->SetNextActionHandler( this );

    m_pColorMotion =  new moColorMotion( m_pInspectorNotebook, wxID_ANY );
    m_pColorMotion->SetNextActionHandler( this );

    m_pFunctionPanel =  new moFunctionPanel( m_pInspectorNotebook, wxID_ANY );
    m_pFunctionPanel->SetNextActionHandler( this );

    m_pShadersPanel = new moShadersPanel( m_pInspectorNotebook, wxID_ANY );
    m_pShadersPanel->SetNextActionHandler( this );

    m_p3dModelPanel = new mo3dModelPanel( m_pInspectorNotebook, wxID_ANY );
    m_p3dModelPanel->SetNextActionHandler( this );

	m_pInspectorNotebook->AddPage( m_pPanelTexture, wxT("Texture") );
	m_pInspectorNotebook->AddPage( m_pColorMotion , wxT("Color") );
	m_pInspectorNotebook->AddPage( m_pObjectMotion, wxT("Motion") );
	m_pInspectorNotebook->AddPage( m_pFunctionPanel, wxT("Function") );
	m_pInspectorNotebook->AddPage( m_p3dModelPanel , wxT("3d Model") );
	m_pInspectorNotebook->AddPage( m_pShadersPanel , wxT("Shader") );

	m_pInspectorNotebook->AddPage( new wxPanel(m_pInspectorNotebook,wxID_ANY) , wxT("Sound") );
	m_pInspectorNotebook->AddPage( new wxPanel(m_pInspectorNotebook,wxID_ANY) , wxT("Font") );
	m_pInspectorNotebook->AddPage( new wxTextCtrl(m_pInspectorNotebook,wxID_ANY) , wxT("Script") );
	m_pInspectorNotebook->AddPage( new wxTextCtrl(m_pInspectorNotebook,wxID_ANY) , wxT("Text") );


}


//Invocar al DirectorFrame para inspeccionar un valor, se propaga al resto de los parametros del MOB correspondiente
// esto sirve tambien para cuando se hace foco en algun objeto del MOB, si se llama a este, se asegura
// que todos los inspectores seran notificados
void
moDirectorFrame::Inspect( moValueDescriptor  p_ValueDescriptor, bool setselection  ) {

        switch( p_ValueDescriptor.GetParamDescriptor().GetParamDefinition().GetType()) {
            case MO_PARAM_ALPHA:
            case MO_PARAM_SYNC:
            case MO_PARAM_PHASE:
            case MO_PARAM_FUNCTION:
            case MO_PARAM_NUMERIC:
                // to the function inspector....
                m_pFunctionPanel->Inspect( p_ValueDescriptor );
                if (setselection) m_pInspectorNotebook->SetSelection(0);
                break;
            case MO_PARAM_ROTATEX:
            case MO_PARAM_ROTATEY:
            case MO_PARAM_ROTATEZ:
            case MO_PARAM_TRANSLATEX:
            case MO_PARAM_TRANSLATEY:
            case MO_PARAM_TRANSLATEZ:
            case MO_PARAM_SCALEX:
            case MO_PARAM_SCALEY:
            case MO_PARAM_SCALEZ:
                m_pObjectMotion->Inspect( p_ValueDescriptor );
                if (setselection) m_pInspectorNotebook->SetSelection(0);
                break;

            case MO_PARAM_COLOR:
                m_pColorMotion->Inspect( p_ValueDescriptor );
                if (setselection) m_pInspectorNotebook->SetSelection(0);
                break;

            case MO_PARAM_TEXTURE:
            case MO_PARAM_VIDEO:
                m_pPanelTexture->Inspect( p_ValueDescriptor );
                if (setselection) m_pInspectorNotebook->SetSelection(0);
                break;
            case MO_PARAM_OBJECT:
            case MO_PARAM_3DMODEL:
                m_p3dModelPanel->Inspect( p_ValueDescriptor );
                if (setselection) m_pInspectorNotebook->SetSelection(0);
                break;
            case MO_PARAM_FILTER:
                m_pShadersPanel->Inspect( p_ValueDescriptor );
                if (setselection) m_pInspectorNotebook->SetSelection(0);
                break;

        }

}


void
moDirectorFrame::Inspect( moMobDescriptor  p_MobDescriptor ) {

        moValueDescriptor p_ValueDescriptor;
        moParameterDescriptors mParams;
        moParameterDescriptor mParam;

        mParams = GetParameterDescriptors( p_MobDescriptor );

        for(int i=0; i<mParams.Count(); i++) {

            mParam = mParams[i];

            moValueIndex valindex;
            valindex.m_ParamIndex = mParam.GetIndex();
            valindex.m_ValueIndex = mParam.GetIndexValue();

            p_ValueDescriptor = GetValue( moValueDescriptor( mParams[i], valindex ) );

            Inspect( p_ValueDescriptor );

        }

}


void moDirectorFrame::OnKeyDown( wxKeyEvent &event ) {

	event.Skip();

}

void moDirectorFrame::OnCommand( wxCommandEvent& event ) {

	// do something
	wxString text = event.GetString();

}

void
moDirectorFrame::SetDirectorCore( moDirectorCore* p_pDirectorCore ) {

	m_pDirectorCore = p_pDirectorCore;
	this->SetNextActionHandler( (moIDirectorActions*)m_pDirectorCore );

}

void
moDirectorFrame::SetIODeviceManager( wxEvtHandler*  m_pEventHandler ) {

    if (m_pPreviewFrame)
        if (m_pPreviewFrame->m_pGLCanvas)
         m_pPreviewFrame->m_pGLCanvas->Init( this);
    if (m_pPreviewWindow)
        if (m_pPreviewWindow->m_pGLCanvas)
            m_pPreviewWindow->m_pGLCanvas->Init( this);
}

moGLCanvas*  moDirectorFrame::GetGLCanvas() {
	return m_pGLCanvas;
}

MOboolean
moDirectorFrame::Init() {

    m_pStartFrame = new moDirectorStartFrame( m_pFilesBook );

    if (m_pStartFrame) {
        //m_pStartFrame->Activate();
        m_pFilesBook->AddPage( (wxWindow*) m_pStartFrame, _T("Welcome to Moldeo Director"));

        //m_pFilesBook->AddPage( (wxWindow*) new wxScrolledWindow( this, wxID_ANY), "Welcome to Moldeo Director Dialog");
        //m_pFilesBook->AddPage((wxWindow*)new wxDialog(this,wxID_ANY,"Dialog"), "Welcome to Moldeo Director Dialog");
        //m_pFilesBook->AddPage((wxWindow*)new wxPanel(this,wxID_ANY), "Welcome to Moldeo Director Panel");
        //m_pFilesBook->AddPage((wxWindow*)new wxTextCtrl(this,wxID_ANY), "Welcome to Moldeo Director Control");
        //m_pFilesBook->AddPage(new wxFrame(this,wxID_ANY,"Dialog",wxPoint(0,0), wxSize(100,100),wxFRAME_NO_TASKBAR | wxBORDER_NONE | wxNO_BORDER,"Start frame" ), "Welcome to Moldeo Director Dialog");
        //m_pFilesBook->AddPage(new wxDialog(this,wxID_ANY,"Dialog"), "Welcome to Moldeo Director Dialog");
        //m_pStartFrame->Maximize();
        m_pStartFrame->Init(this);
    }
	return true;
}

MOboolean moDirectorFrame::Finish() {

    m_pGLCanvas = NULL;

	return true;
}


void moDirectorFrame::OnEraseBackground(wxEraseEvent& event)
{
    event.Skip();
}

void moDirectorFrame::OnSize(wxSizeEvent& event)
{
    event.Skip();
}

wxPanel* moDirectorFrame::CreatePanel() {

	return new wxPanel(this, -1,
		wxPoint(0,0), wxSize(160,250),wxTAB_TRAVERSAL, _T("panel"));

}


wxTextCtrl* moDirectorFrame::CreateTextCtrl()
{

    return new wxTextCtrl(this,-1, wxT(""),
                          wxPoint(0,0), wxSize(150,90),
                          wxNO_BORDER | wxTE_MULTILINE | wxTE_RICH);
}


void
moDirectorFrame::CreateGLContext() {

	if (m_pGLCanvas)
        m_pGLContext = m_pGLCanvas->GetContext();

}

void moDirectorFrame::OnClose(wxCloseEvent& event) {
	if( wxMessageBox(_T("Really close Moldeo Director?"), _T("Please confirm"),
                          wxICON_QUESTION | wxYES_NO) != wxYES )
    {
        event.Veto();

        return;
    } else {
		if(this->CloseProject()==MO_DIRECTOR_STATUS_OK) {
		    if (this->CloseApp()==MO_DIRECTOR_STATUS_OK) {
                event.Skip();
                return;
		    }
		}
		event.Veto();
	}
}

void moDirectorFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}

void moDirectorFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("This is the About dialog of the minimal sample.\n")
                _T("Welcome to %s"), wxVERSION_STRING);

    wxMessageBox(msg, _T("About Minimal"), wxOK | wxICON_INFORMATION, this);
}


void
moDirectorFrame::OnNewProject( wxCommandEvent& event ) {

	moNewProject*   pNewProject = new moNewProject( this, -1 );

    if (pNewProject)
	if (pNewProject->ShowModal() == wxID_OK) {

        this->NewProject( pNewProject->GetProjectDescriptor() );

	}

}

//INTERFACE METHODS
void
moDirectorFrame::OnOpenProject(wxCommandEvent& event) {

	wxFileDialog* pFileDialog = NULL;
	moProjectDescriptor ProjectDescriptor;
	moDirectorStatus	mStatus;

	//open browser window
	if ( event.GetString()==_T("") ) {

        pFileDialog = new wxFileDialog( this );

        if(pFileDialog) {

            pFileDialog->SetWildcard(wxT("MOL files (*.mol)|*.mol|All files (*.*)|*.*"));

            if( pFileDialog->ShowModal() == wxID_OK ) {

                wxFileName	FileName( pFileDialog->GetPath() );

                wxString path = FileName.GetPath();
                #ifdef MO_WIN32
                    path+= "\\";
                #else
                    path+= _T("/");
                #endif
                wxString name = FileName.GetFullName();
                const char *cfilepath = (char*)path.c_str();
                const char *cfilename = (char*)name.c_str();

                ProjectDescriptor.Set( moText((char*)cfilepath), moText((char*)cfilename) );

                mStatus = OpenProject( ProjectDescriptor );

            }

        }

	} else {

	    Log( moText("Direct opening..") + moWx2Text(event.GetString()));

        wxFileName	FileName( event.GetString() );

        wxString path = FileName.GetPath();
        #ifdef MO_WIN32
            path+= "\\";
        #else
            path+= _T("/");
        #endif
        wxString name = FileName.GetFullName();
        const char *cfilepath = (char*)path.c_str();
        const char *cfilename = (char*)name.c_str();

        ProjectDescriptor.Set( moText((char*)cfilepath), moText((char*)cfilename) );

        mStatus = OpenProject( ProjectDescriptor );

    }

}

void
moDirectorFrame::OnSaveProject(wxCommandEvent& event) {
    SaveProject();
}

void
moDirectorFrame::OnCloseProject(wxCommandEvent& event) {

    this->CloseProject();
}

void
moDirectorFrame::OnNewEffect( wxCommandEvent& event ) {

    moNewEffectDialog*  pNewEffectDialog = new moNewEffectDialog( this, -1 );

    pNewEffectDialog->Init( this );

    if( pNewEffectDialog->ShowModal() == wxID_OK ) {

        moMobDefinition pMobDefinition = pNewEffectDialog->GetMobDefinition();

        NewMob( moMobDescriptor( pMobDefinition ) );

    }

}

moDirectorStatus
moDirectorFrame::OpenMob( moMobDescriptor p_MobDescriptor ) {

    moDirectorChildFrame* pChild = NULL;

    for( size_t i=0; i<m_pFilesBook->GetPageCount(); i++ ) {
        if (m_pFilesBook->GetPage(i)->GetName()==_T("child frame") ) {
            pChild =(moDirectorChildFrame*)m_pFilesBook->GetPage(i);
            if (pChild) {
                if( pChild->IsMob( p_MobDescriptor ) ) {

                    //to avoid innecesary inspections:
                    //check the actual selection
                    //focus and return

                    if (m_pFilesBook->GetSelection()!=i) {
                        m_pFilesBook->SetSelection(i);
                    }

                    //now that we founded it run the iterative Inspector...
                    this->Inspect( p_MobDescriptor );

                    return MO_DIRECTOR_STATUS_CONFIG_ALREADY_OPENED;
                }
            }
        }
    }

    // Never get here if filesbook has the page opened...
    m_pDirectorCore->OpenMob( p_MobDescriptor );
}


void
moDirectorFrame::OnOpenMob( wxCommandEvent& event ) {


}


void
moDirectorFrame::OnSaveMob( wxCommandEvent& event ) {

    //guardar el MOB correspondiente al MOB elegido
    moDirectorChildFrame* pChild = NULL;

    int i = m_pFilesBook->GetSelection();
    if (m_pFilesBook->GetPage(i)->GetName()==_T("child frame") ) {
        pChild =(moDirectorChildFrame*)m_pFilesBook->GetPage(i);
        if (pChild) {
            pChild->Save();
        }
    }

}

void
moDirectorFrame::OnCloseMob( wxCommandEvent& event ) {

    moDirectorChildFrame* pChild = NULL;

    int i = m_pFilesBook->GetSelection();
    if (m_pFilesBook->GetPage(i)->GetName() == _T("child frame") ) {
        pChild =(moDirectorChildFrame*)m_pFilesBook->GetPage(i);
        if (pChild) {
            if ( pChild->Close() ) {
                pChild->Destroy();
                m_pFilesBook->RemovePage(i);
            }
        }
    }

}

void
moDirectorFrame::OnSaveAll( wxCommandEvent& event ) {

    this->SaveAll();

}


void
moDirectorFrame::OnCloseAll( wxCommandEvent& event ) {

    this->CloseAll();

}

moDirectorStatus
moDirectorFrame::CloseAll() {

    moDirectorChildFrame* pChild = NULL;

    //int i = m_pFilesBook->GetSelection();
    for( size_t i=0; i<m_pFilesBook->GetPageCount(); i++ ) {
        if (m_pFilesBook->GetPage(i)->GetName()==_T("child frame") ) {
            pChild =(moDirectorChildFrame*)m_pFilesBook->GetPage(i);
            if (pChild) {
                if (pChild->Close()) {
                    pChild->Destroy();
                    m_pFilesBook->RemovePage(i);
                    i--;
                } else return MO_DIRECTOR_STATUS_ERROR;
            }
        }
    }
    return MO_DIRECTOR_STATUS_OK;

}

moDirectorStatus
moDirectorFrame::SaveAll() {

    moDirectorChildFrame* pChild = NULL;

    //int i = m_pFilesBook->GetSelection();
    for( size_t i=0; i<m_pFilesBook->GetPageCount(); i++ ) {
        if (m_pFilesBook->GetPage(i)->GetName()==_T("child frame") ) {
            pChild =(moDirectorChildFrame*)m_pFilesBook->GetPage(i);
            if (pChild) {
                pChild->Save();
            }
        }
    }
    return MO_DIRECTOR_STATUS_OK;

}

moDirectorStatus
moDirectorFrame::CloseProject() {

	moDirectorChildFrame* pChild = NULL;
	moDirectorChildConsole* pDirectorChildConsole = NULL;

    if (this->CloseAll()==MO_DIRECTOR_STATUS_OK) {
        if ( m_pDirectorCore->CloseProject() == MO_DIRECTOR_STATUS_OK ) {

            if(m_pDataNotebook)
                if (m_pDataNotebook->m_pProjectTreeCtrl)
                    m_pDataNotebook->m_pProjectTreeCtrl->DeleteAllItems();
            if (m_pLayers)
                m_pLayers->DeleteAllItems();
            if (m_pLayers2)
                m_pLayers2->DeleteAllItems();
            return MO_DIRECTOR_STATUS_OK;
        }
    }
    return MO_DIRECTOR_STATUS_ERROR;

}

void
moDirectorFrame::OnProjectPreview( wxCommandEvent& event ) {

    ProjectPreview();

}

void
moDirectorFrame::OnProjectPreviewFullscreen( wxCommandEvent& event ) {

    FullScreen(false);

}

void
moDirectorFrame::OnFullscreen( wxCommandEvent& event ) {

    if (!IsFullScreen() ) {
        ShowFullScreen( true, wxFULLSCREEN_NOBORDER | wxFULLSCREEN_NOCAPTION );
    } else {
        ShowFullScreen( false);
    }

}


moDirectorChildFrame* moDirectorFrame::CreateChildFrame( moMobDescriptor p_MobDescriptor ) {

    // Make another frame, containing a canvas

	wxString title =(wxChar*)(char*) p_MobDescriptor.GetMobDefinition().GetConfigName();

    moDirectorChildFrame* pDirectorChildFrame = new moDirectorChildFrame( m_pFilesBook, title);

    //pDirectorChildFrame->SetTitle(title);

    // Give it an icon
#ifdef __WXMSW__
    //pDirectorChildFrame->SetIcon(wxIcon(_T("chrt_icn")));
#else
    //pDirectorChildFrame->SetIcon(wxIcon( mondrian_xpm ));
#endif

    //pDirectorChildFrame->Show(true);

	//extract filename form config full path name
	wxFileName xfname( wxString((wxChar*)(char*) p_MobDescriptor.GetMobDefinition().GetName() ) );

	m_pFilesBook->AddPage( pDirectorChildFrame,
                            (wxChar*)(char*) p_MobDescriptor.GetMobDefinition().GetLabelName(),
                            true);

	return pDirectorChildFrame;
}

/*================================================

  IDirectorActions Implemented by FrameDirector:

================================================*/

moDirectorStatus
moDirectorFrame::FocusOutput() {


    if(m_pGLCanvas) {

        m_pGLCanvas->Activate( m_pDirectorCore->GetDirectorIODeviceManager() );
    }

}


moDirectorStatus
moDirectorFrame::FullScreen( bool force ) {

    if (m_pPreviewFrame==NULL) {
        ProjectPreview();
        if (m_pPreviewFrame)
            m_pPreviewFrame->FullScreen();
    } else
    if ( m_pPreviewFrame!=NULL && !m_pPreviewFrame->IsShown() ) {
        ProjectPreview();
        m_pPreviewFrame->FullScreen(true);
    } else
    if (m_pPreviewFrame!=NULL && m_pPreviewFrame->IsShown()) {
        m_pPreviewFrame->FullScreen();
    }


}

moDirectorStatus
moDirectorFrame::ProjectPreview() {

    if (m_pPreviewWindow)
        if (m_pGLCanvas)
            if (m_pGLCanvas==m_pPreviewWindow->m_pGLCanvas) {

                m_pPreviewWindow->m_pGLCanvas->Finish();

                if (!m_pPreviewFrame) {
                    m_pPreviewFrame = new moPreviewFrame( this, wxID_ANY );
                    //if (!m_pGLContext) {
                        m_pGLContext = m_pPreviewWindow->m_pGLCanvas->GetContext();
                    //}
                    m_pPreviewFrame->Init( this, m_pGLContext );
                }
                m_pPreviewFrame->Init( this, m_pGLContext );
                m_pPreviewFrame->m_pGLCanvas->SetCurrent();
                m_pPreviewFrame->Show();

                m_pGLCanvas = m_pPreviewFrame->m_pGLCanvas;

            } else {
                m_pGLCanvas = NULL;
                m_pPreviewFrame->m_pGLCanvas->Finish();
                m_pPreviewFrame->Hide();

                m_pPreviewWindow->Init( this, m_pGLContext );
                m_pPreviewWindow->m_pGLCanvas->SetCurrent();
                m_pPreviewWindow->Refresh();

                m_pGLCanvas = m_pPreviewWindow->m_pGLCanvas;

            }
        else return MO_DIRECTOR_STATUS_OK;
    else return MO_DIRECTOR_STATUS_OK;
    return MO_DIRECTOR_STATUS_OK;

}

void
moDirectorFrame::ViewSwapBuffers() {
    if (m_pGLCanvas!=NULL) {
        m_pGLCanvas->ViewSwapBuffers();
    }
}

MO_HANDLE
moDirectorFrame::GetHandle() {
    if (m_pGLCanvas)
        return m_pGLCanvas->GetHandle();
    return NULL;

}


moDirectorStatus
moDirectorFrame::EditMob( moMobDescriptor p_MobDescriptor ) {

	moDirectorStatus res;
	moMobDescriptor MobDescriptor;


    moDirectorChildFrame* pChild = NULL;
    moDirectorChildConsole* pDirectorChildConsole = NULL;

    for( size_t i=0; i<m_pFilesBook->GetPageCount(); i++ ) {
        if (m_pFilesBook->GetPage(i)->GetName()==_T("child frame") ) {
            pChild =(moDirectorChildFrame*)m_pFilesBook->GetPage(i);
            if (pChild) {
                if( pChild->IsMob( p_MobDescriptor ) ) {
                    return MO_DIRECTOR_STATUS_CONFIG_ALREADY_OPENED;
                }
            }
        }
    }

    //si el archivo todavia no fue abierto...
    pChild = NULL;
    pChild = CreateChildFrame( p_MobDescriptor );

    if(pChild!=NULL) {

        //m_pChildFrameList.Append( pChild );
        pChild->Init( this, p_MobDescriptor);

        return MO_DIRECTOR_STATUS_OK;

    } else return MO_DIRECTOR_STATUS_NOT_FOUND;

	return res;
}

moDirectorStatus
moDirectorFrame::CloseMob( moMobDescriptor p_MobDesc ) {

	if ( m_pDirectorCore->CloseMob( p_MobDesc ) == MO_DIRECTOR_STATUS_OK ) {

		moDirectorChildFrame* pChild;

		for( size_t i=0; i<m_pFilesBook->GetPageCount(); i++ ) {
            if (m_pFilesBook->GetPage(i)->GetName()==_T("child frame") ) {
                pChild =(moDirectorChildFrame*)m_pFilesBook->GetPage(i);
                if (pChild) {
                    if( pChild->IsMob( p_MobDesc ) ) {
                        m_pFilesBook->RemovePage( i );
                        return MO_DIRECTOR_STATUS_OK;
                    }
                }
            }
		}
	}
	return MO_DIRECTOR_STATUS_MOB_NOT_FOUND;

}

moDirectorStatus
moDirectorFrame::SaveMob( moMobDescriptor p_MobDescriptor ) {
	return m_pDirectorCore->SaveMob( p_MobDescriptor );
}

moDirectorStatus
moDirectorFrame::ProjectUpdated( moProjectDescriptor p_ProjectDescriptor ) {

	MOuint i;
	wxTreeItemId exid;
	wxString cPath;
	wxString cMol;
	wxString xitemname;
	moMobDescriptor MobDesc;

    if (m_pPanelTexture)
        m_pPanelTexture->InitTexturesTree();

	if (!m_pDataNotebook) return MO_DIRECTOR_STATUS_ERROR;
	if (!m_pDataNotebook->m_pProjectTreeCtrl) return MO_DIRECTOR_STATUS_ERROR;

	m_pDataNotebook->m_pProjectTreeCtrl->DeleteAllItems();
	m_pDataNotebook->m_pIODevicesTreeCtrl->DeleteAllItems();
	m_pDataNotebook->m_pResourcesTreeCtrl->DeleteAllItems();

	cPath = (wxChar*)(char*)p_ProjectDescriptor.GetConfigPath();
	cMol = (wxChar*)(char*) p_ProjectDescriptor.GetConfigName();

	if(m_pDataNotebook->m_pVirtualDirTreeCtrl)
		m_pDataNotebook->m_pVirtualDirTreeCtrl->SetRootPath( cPath, wxVDTC_DEFAULT);

	FrameManager.GetPane(wxT("manager")).Caption(wxT("Project manager: ")+cMol);

    wxTreeItemId preeffectsid,effectsid,posteffectsid, mastereffectsid,iodevicesid, resourcesid;

	wxTreeItemId root = m_pDataNotebook->m_pProjectTreeCtrl->AddRoot( (wxChar*)(char*)(moText("Layer effects: ") + p_ProjectDescriptor.GetConfigName()) );

	iodevicesid = m_pDataNotebook->m_pIODevicesTreeCtrl->AddRoot( (wxChar*)(char*)(moText("IODevices: ") + p_ProjectDescriptor.GetConfigName()) );

	resourcesid = m_pDataNotebook->m_pResourcesTreeCtrl->AddRoot( (wxChar*)(char*)(moText("Resources: ") + p_ProjectDescriptor.GetConfigName()) );

	preeffectsid = m_pDataNotebook->m_pProjectTreeCtrl->AppendItem(root, wxT("PreEffects"), 0);
	effectsid = m_pDataNotebook->m_pProjectTreeCtrl->AppendItem(root, wxT("Effects"), 0);
	posteffectsid = m_pDataNotebook->m_pProjectTreeCtrl->AppendItem(root, wxT("PostEffects"), 0);
	mastereffectsid = m_pDataNotebook->m_pProjectTreeCtrl->AppendItem(root, wxT("MasterEffects"), 0);

	//moMoldeoObjects	*pMOBS = p_ProjectDescriptor.m_pMOBS;
	moMobDescriptors pMobDescriptors = GetMobDescriptors();

	moEffectLayerCtrl* pobject = NULL;
	moEffectLayerTimelineCtrl* pobject2 = NULL;

	if (m_pLayers && m_pLayers2) {

		m_pLayers->DeleteAllItems();

		m_pLayers2->DeleteAllItems();

		m_preeffectsid = m_pLayers->AddRoot( wxT("Layer 0 [pre]"), wxT("pre-effects"), (wxWindow*)new wxStaticText(m_pLayers, wxID_ANY,wxT("- 0")) );
		m_effectsid = m_pLayers->AddRoot( wxT("Layer 1 [effects]"), wxT("effects"), (wxWindow*)new wxStaticText(m_pLayers, wxID_ANY,wxT("- 1")) );
		m_posteffectsid = m_pLayers->AddRoot( wxT("Layer 2 [post]"), wxT("post-effects"), (wxWindow*)new wxStaticText(m_pLayers, wxID_ANY,wxT("- 3")) );

		m_preeffectsid2 = m_pLayers2->AddRoot( wxT("timeline"), wxT("pre-effects"),  (wxWindow*)new wxStaticText(m_pLayers, wxID_ANY,wxT("- 0")));
		m_effectsid2 = m_pLayers2->AddRoot( wxT("timeline"), wxT("effects"), (wxWindow*)new wxStaticText(m_pLayers, wxID_ANY,wxT("- 1")) );
		m_posteffectsid2 = m_pLayers2->AddRoot( wxT("timeline"), wxT("post-effects"),  (wxWindow*)new wxStaticText(m_pLayers, wxID_ANY,wxT("- 2")) );
	}

	//intento de borrar unicamente los layers correspondientes...al final resolvimos borrar todo...mejor..
	//esto lo usaremos para actualizar lo que querramos de cada layer
	//TreeMultiItemNode *preeffectslayers = (TreeMultiItemNode *)m_preeffectsid.GetItem();
	//if (preeffectslayers)
		//preeffectslayers->

    wxTreeMultiWindowInfo wndinfo( wxTMC_SPAN_WIDTH, 0, 0, false);
    wndinfo.SetTopSpacing(0);
    wndinfo.SetFrontSpacing(0);
    wndinfo.Indent(0);
    //m_pLayers->SetCheckboxView();

    m_pLayers->SetSpacingY(0);
    m_pLayers2->SetSpacingY(0);

	for( i=0;  i < pMobDescriptors.Count(); i++) {
	    moMobDescriptor pMobDescriptor = pMobDescriptors[i];
		xitemname = wxString( (wxChar*)(char*) pMobDescriptor.GetMobDefinition().GetName() ) + wxT("::");
		xitemname+= wxString( (wxChar*)(char*) pMobDescriptor.GetMobDefinition().GetConfigName() );

		switch( pMobDescriptor.GetMobDefinition().GetType() ) {

			case MO_OBJECT_PREEFFECT:
				exid = m_pDataNotebook->m_pProjectTreeCtrl->AppendItem( preeffectsid, xitemname , 1 );
				pobject = new moEffectLayerCtrl(m_pLayers);
				pobject2 = new moEffectLayerTimelineCtrl(m_pLayers2);
				if (pobject) {
				    pobject->SetNextActionHandler(this);
					pobject->Set( pMobDescriptor );
				    pobject2->SetNextActionHandler(this);
					pobject2->Set( pMobDescriptor );
					m_pLayers->AppendWindow( m_preeffectsid, pobject, _(""), wndinfo);
					m_pLayers2->AppendWindow( m_preeffectsid2, pobject2, _(""), wndinfo);
				}
				break;

			case MO_OBJECT_EFFECT:
				exid = m_pDataNotebook->m_pProjectTreeCtrl->AppendItem( effectsid, xitemname , 1);
				pobject = new moEffectLayerCtrl(m_pLayers);
				pobject2 = new moEffectLayerTimelineCtrl(m_pLayers2);
				if (pobject) {
				    pobject->SetNextActionHandler(this);
					pobject->Set( pMobDescriptor );
				    pobject2->SetNextActionHandler(this);
					pobject2->Set( pMobDescriptor );
					m_pLayers->AppendWindow( m_effectsid, pobject, _(""), wndinfo);
					m_pLayers2->AppendWindow( m_effectsid2, pobject2, _(""), wndinfo);
				}
				break;
			case MO_OBJECT_POSTEFFECT:
				exid = m_pDataNotebook->m_pProjectTreeCtrl->AppendItem( posteffectsid, xitemname , 1);
				pobject = new moEffectLayerCtrl(m_pLayers);
				pobject2 = new moEffectLayerTimelineCtrl(m_pLayers2);
				if (pobject) {
				    pobject->SetNextActionHandler(this);
					pobject->Set( pMobDescriptor );
				    pobject2->SetNextActionHandler(this);
					pobject2->Set( pMobDescriptor );
					m_pLayers->AppendWindow( m_posteffectsid, pobject, _(""), wndinfo);
					m_pLayers2->AppendWindow( m_posteffectsid2, pobject2, _(""), wndinfo);
				}
				break;
			case MO_OBJECT_MASTEREFFECT:
				exid = m_pDataNotebook->m_pProjectTreeCtrl->AppendItem( mastereffectsid, xitemname , 1);
				break;
			case MO_OBJECT_IODEVICE:
				exid = m_pDataNotebook->m_pIODevicesTreeCtrl->AppendItem( iodevicesid, xitemname , 1);
				break;
			case MO_OBJECT_RESOURCE:
				exid = m_pDataNotebook->m_pResourcesTreeCtrl->AppendItem( resourcesid, xitemname , 1);
				break;

			default:
				exid.Unset();
				break;
		}

		if (exid.IsOk()) {
			m_pDataNotebook->m_pProjectTreeCtrl->SetItemData( exid, new moMobItemData( pMobDescriptor ) );
		}
	}

	m_pDataNotebook->m_pProjectTreeCtrl->Expand(root);

	FrameManager.Update();

	return MO_DIRECTOR_STATUS_OK;
}


moDirectorStatus
moDirectorFrame::ParameterUpdated( moParameterDescriptor p_ParameterDesc ) {

	moDirectorChildFrame* pChild;

    moMobDescriptor MobDesc = p_ParameterDesc.GetMobDescriptor();

    for( size_t i=0; i<m_pFilesBook->GetPageCount(); i++ ) {
            if (m_pFilesBook->GetPage(i)->GetName()==_T("child frame") ) {
                pChild =(moDirectorChildFrame*)m_pFilesBook->GetPage(i);
                if (pChild) {
                    if( pChild->IsMob( MobDesc ) ) {
                        return pChild->ParameterUpdated( p_ParameterDesc );
                    }
                }
            }
    }

    return MO_DIRECTOR_STATUS_MOB_NOT_FOUND;

}


moDirectorStatus
moDirectorFrame::EditParameter( moParameterDescriptor p_ParameterDesc ) {

	moDirectorChildFrame* pChild;

    moMobDescriptor MobDesc = p_ParameterDesc.GetMobDescriptor();

    for( size_t i=0; i<m_pFilesBook->GetPageCount(); i++ ) {
			if (m_pFilesBook->GetPage(i)->GetName()==_T("child frame") ) {
                pChild =(moDirectorChildFrame*)m_pFilesBook->GetPage(i);
                if (pChild) {
                    if( pChild->IsMob( MobDesc ) ) {
                        return pChild->EditParameter( p_ParameterDesc );
                    }
                }
			}
    }

    return MO_DIRECTOR_STATUS_MOB_NOT_FOUND;

}

moDirectorStatus
moDirectorFrame::EditValue( moValueDescriptor p_ValueDesc ) {

	moDirectorChildFrame* pChild;

    moParameterDescriptor ParamDesc = p_ValueDesc.GetParamDescriptor();
    moMobDescriptor MobDesc = ParamDesc.GetMobDescriptor();

    for( size_t i=0; i<m_pFilesBook->GetPageCount(); i++ ) {
        if (m_pFilesBook->GetPage(i)->GetName()==_T("child frame") ) {
			pChild =(moDirectorChildFrame*)m_pFilesBook->GetPage(i);
			if (pChild) {
                if( pChild->IsMob( MobDesc ) ) {
                    return pChild->EditValue( p_ValueDesc );
                }
			}
        }
    }

    return MO_DIRECTOR_STATUS_MOB_NOT_FOUND;

}

moDirectorStatus
moDirectorFrame::ValueUpdated( moValueDescriptor p_ValueDesc ) {

    moDirectorStatus Dstatus = MO_DIRECTOR_STATUS_OK;

	moDirectorChildFrame* pChild;

    moParameterDescriptor ParamDesc = p_ValueDesc.GetParamDescriptor();
    moMobDescriptor MobDesc = ParamDesc.GetMobDescriptor();

    for( size_t i=0; i<m_pFilesBook->GetPageCount(); i++ ) {
        if (m_pFilesBook->GetPage(i)->GetName()==_T("child frame") ) {
			pChild =(moDirectorChildFrame*)m_pFilesBook->GetPage(i);
			if (pChild) {
                if( pChild->IsMob( MobDesc ) ) {
                    Dstatus = CHECK_DIRECTOR_STATUS_ERROR( Dstatus, pChild->ValueUpdated( p_ValueDesc ));
                }
			}
        }
    }

    //aca tenemos que actualizar tambien a los controles
    //dentro de los effect-layer-control

    //hay que hacerlo bien, a trav�s del LayerControlPanel
    Dstatus = CHECK_DIRECTOR_STATUS_ERROR( Dstatus, m_pLayers->ValueUpdated( p_ValueDesc ) );
    Dstatus = CHECK_DIRECTOR_STATUS_ERROR( Dstatus, m_pLayers2->ValueUpdated( p_ValueDesc ) );

    return Dstatus;
}

void
moDirectorFrame::Log( moText p_message ) {

	wxString  w =(wxChar*)(char*)p_message;

	m_pLogTextCtrl->SetDefaultStyle( wxTextAttr( wxColour( 0, 200, 200 )) );

	m_pLogTextCtrl->AppendText(w + wxT("\n"));

}

void
moDirectorFrame::LogError( moText p_message ) {
	//seria bueno en rojo...

	wxString  w =(wxChar*)(char*)p_message;

	m_pLogTextCtrl->SetDefaultStyle( wxTextAttr( wxColour(255,0,0) ) );

	m_pLogTextCtrl->AppendText(w + wxT("\n"));

	wxMessageBox(w);

}
