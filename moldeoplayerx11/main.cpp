/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */


#include "gst/gst.h"
#include <moldeo.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/XKBlib.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>

#ifdef MO_WIN32
  #include "SDL.h"
#else
  #include "SDL/SDL.h"
#endif

#include <X11/extensions/Xinerama.h>
#ifdef HAVE_X11_EXTENSIONS_XF86VMODE_H
  #include <X11/extensions/xf86vmode.h>
#endif

moConsole* gpConsole = NULL;
#define RENDERMODE RENDERMANAGER_MODE_NORMAL

#include "moX11_IODeviceManager.h"


static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display   *dpy;

Window     win;
Window     win2;

int twoscreens = false;
int swapped = false;
int screennumber = 0;

GLfloat    xAngle = 42.0, yAngle = 82.0, zAngle = 112.0;
GLboolean  doubleBuffer = GL_TRUE;

enum moPlayerStateWindow {
  PLAYER_WINDOWED=0,    /** WINDOWED ON FIRST DISPLAY */
  PLAYER_GAMEMODE=1,    /** FULLSCREEN ON FIRST DISPLAY */
  PLAYER_PRESENTATION=2, /** SECOND DISPLAY IF AVAILABLE OR FULLSCREEN ON FIRST DISPLAY */
  PLAYER_FULLSCREEN=3
};

#ifdef OPENGLESV2
//#define MO_SDL_WINDOW_SHOWN 0
#else
//#define MO_SDL_WINDOW_SHOWN SDL_WINDOW_SHOWN
#endif // OPENGLESV2
#define PLAYER_WINDOWED_WIDTH 640
#define PLAYER_WINDOWED_HEIGHT 360
#define PLAYER_PREVIEW_WIDTH 400
#define PLAYER_PREVIEW_HEIGHT 220

int screen_left = 0, screen_top = 0;
int screen_display = 0;
int render_width, render_height ,screen_width, screen_height;
int preview_width  = PLAYER_PREVIEW_WIDTH, preview_height = PLAYER_PREVIEW_HEIGHT;

//TODO: migrate to X11
//Uint32 window_resizable = SDL_WINDOW_RESIZABLE;
int window_resizable = 1;

moText config("");
moText molproject("");
moText molproject_preview = molproject+" Preview";
moText molfolder("");
moText gamemode("");
moText fullscreenmode("");
moText presentationmode("");
moText mwindow("");
moText windowtitle("");
Display* displayWindow = NULL;
Window previewWindow = 0;
int displayWindowId = -1;
int previewWindowId = -1;
bool cmdresolution = false;

moPlayerStateWindow PlayerState = PLAYER_WINDOWED;
bool arguments_ok = false;

int processarguments( int argc, char** argv ) {

    string player_manual = "moldeoplayersdl2 -mol moldeoprojectfilename.mol [-verbose] [-window 640x480] [-gamemode 1024x768:32] [-fullscreen] [-presentationmode]";
    cout << "argc:" << argc << endl;

    for(int c=0;c<argc;c++) {
      cout << " argv[" << c << "] : " << argv[c] << endl;
    }

    if (argc<2) {
			cout << "Missing arguments!" << argc << endl;
			cout << "MoldeoPlayer Manual:" << endl;
			cout << player_manual << endl;
			cout.flush();
			return 0;
		}

    moText argexe(argv[0]);

    if (argc==2) {
      moText argMol(argv[1]);

      cout <<  "Argument [0] exe: " << argexe << endl;
      cout <<  "Argument [1] mol: " << argMol << endl;


      molproject = moText( argMol );
      moFile file( molproject );
      molfolder = file.GetPath();
      cout << "Argument -mol is: " << molproject << " in folder: " << molfolder << endl;
      argc = 0;
      return 0;
    }

    //more than exe+-mol
    while( argc > 2 ) {
        argc--;
        moText arglast(argv[argc]);
        moText argprev;

        if (argc>0) argprev = argv[argc-1];

        moText  arg0(argv[0]);

        cout <<  "Argument [" << (argc) << "] : " << moText(arglast) << endl;

        moFile molFile( arglast );

        if( argprev == moText("-mol") || molFile.GetExtension() == moText(".mol") ) {
          molproject = moText( arglast );
          moFile file( molproject );
          molfolder = file.GetPath();
          cout << "Argument -mol is: " << molproject << " in folder: " << molfolder << endl;
          if ( argprev == moText("-mol") ) argc--;
        } else if (argprev == moText("-window")) {
          mwindow = moText( arglast );
          cout << "Windowed mode -window found! : " << mwindow << endl;
          moTextArray resolution = mwindow.Explode( moText("x") );
          if (resolution.Count()>1) {
            screen_width = atoi((char*)resolution[0]);
            screen_height = atoi((char*)resolution[1]);
            render_width = screen_width;
            render_height = screen_height;
            cmdresolution = true;
          }
          argc--;
        } else if (  argprev == moText("-gamemode") ) {
          gamemode = moText( arglast );
          cout << "Game mode -gamemode found! : " << gamemode << endl;
          moTextArray gmode = gamemode.Explode( moText(":") );
          if (gmode.Count()>1) {
              moText resolutions = gmode[0];
              moTextArray resolution = resolutions.Explode( moText("x") );
              if (resolution.Count()>1) {
                  screen_width = atoi((char*)resolution[0]);
                  screen_height = atoi((char*)resolution[1]);
                  render_width = screen_width;
                  render_height = screen_height;
                  cmdresolution = true;
              }
          }
          argc--;
        } else if (  arglast == moText("-fullscreen") ) {
          fullscreenmode = "fullscreen";
        } else if (  argprev == moText("-fullscreen") ) {
          fullscreenmode = moText( arglast );
          screennumber = atoi(fullscreenmode);
          cout << "Fullscreen mode -fullscreen found! : " << fullscreenmode << " screennumber:" << screennumber << endl;
          moTextArray fmode = fullscreenmode.Explode( moText(":") );
          if (fmode.Count()>1) {
              moText resolutions = fmode[0];
              moTextArray resolution = resolutions.Explode( moText("x") );
              if (resolution.Count()>1) {
                  screen_width = atoi((char*)resolution[0]);
                  screen_height = atoi((char*)resolution[1]);
                  render_width = screen_width;
                  render_height = screen_height;
                  cmdresolution = true;
              }
          } else {
              moText resolutions = arglast;
              moTextArray resolution = resolutions.Explode( moText("x") );
              if (resolution.Count()>1) {
                  screen_width = atoi((char*)resolution[0]);
                  screen_height = atoi((char*)resolution[1]);
                  render_width = screen_width;
                  render_height = screen_height;
                  cmdresolution = true;
              }
          }
          argc--;
        } else if (  arglast == moText("-presentationmode") ) {
          presentationmode = "presentationmode";
        } else if (  argprev == moText("-presentationmode") ) {
          presentationmode = moText( arglast );
          screennumber = atoi(presentationmode);
          cout << "Presentation mode -presentationmode found! : " << presentationmode << " screennumber:" << screennumber << endl;
          moTextArray pmode = presentationmode.Explode( moText(":") );
          if (pmode.Count()>1) {

              moText resolutions = pmode[0];
              moTextArray resolution = resolutions.Explode( moText("x") );
              if (resolution.Count()>1) {
                  screen_width = atoi((char*)resolution[0]);
                  screen_height = atoi((char*)resolution[1]);
                  render_width = screen_width;
                  render_height = screen_height;
                  cmdresolution = true;
              }
              /**
              moText positions = pmode[1];
              moTextArray position = positions.Explode( moText(",") );
              if (position.Count()>1) {
                  screen_width = atoi((char*)resolution[0]);
                  screen_height = atoi((char*)resolution[1]);
                  render_width = screen_width;
                  render_height = screen_height;
              }
              moText display = pmode[2];
              */
          }
          argc--;
        } else if (  arglast == moText("-noresize") ) {
          window_resizable = 0;
        } else if (  arglast == moText("--help") ) {

            cout << "MoldeoPlayer Manual:" << endl;
            cout << player_manual << endl;
            cout.flush();
            return 0;
        } else {
          cout << "Missing arguments!" << endl;
          cout << "MoldeoPlayer Manual:" << endl;
          cout << player_manual << endl;
          cout.flush();
          //exit(0);
          return 0;
        }
    }
	return 1;
}















void fatalError(const char *message)
{
  fprintf(stderr, "main: %s\n", message);
  exit(1);
}

void redraw(void)
{
  static GLboolean   displayListInited = GL_FALSE;

  if (displayListInited)
  {
    /* if display list already exists, just execute it */
    glCallList(1);
  }
  else
  {
    /* otherwise compile and execute to create the display list */
    glNewList(1, GL_COMPILE_AND_EXECUTE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* front face */
    glBegin(GL_QUADS);
      glColor3f(0.0, 0.7, 0.1);  /* green */
      glVertex3f(-1.0, 1.0, 1.0);
      glVertex3f(1.0, 1.0, 1.0);
      glVertex3f(1.0, -1.0, 1.0);
      glVertex3f(-1.0, -1.0, 1.0);

      /* back face */
      glColor3f(0.9, 1.0, 0.0);  /* yellow */
      glVertex3f(-1.0, 1.0, -1.0);
      glVertex3f(1.0, 1.0, -1.0);
      glVertex3f(1.0, -1.0, -1.0);
      glVertex3f(-1.0, -1.0, -1.0);

      /* top side face */
      glColor3f(0.2, 0.2, 1.0);  /* blue */
      glVertex3f(-1.0, 1.0, 1.0);
      glVertex3f(1.0, 1.0, 1.0);
      glVertex3f(1.0, 1.0, -1.0);
      glVertex3f(-1.0, 1.0, -1.0);

      /* bottom side face */
      glColor3f(0.7, 0.0, 0.1);  /* red */
      glVertex3f(-1.0, -1.0, 1.0);
      glVertex3f(1.0, -1.0, 1.0);
      glVertex3f(1.0, -1.0, -1.0);
      glVertex3f(-1.0, -1.0, -1.0);
    glEnd();
    glEndList();
    displayListInited = GL_TRUE;
  }
  if (doubleBuffer)
    glXSwapBuffers(dpy, win);/* buffer swap does implicit glFlush */
  else
    glFlush();  /* explicit flush for single buffered case */
}





int main(int argc, char **argv)
{
  char app_path[1000];
  unsigned int gsmajor, gsminor, gsmicro, gsnano;
  gst_init(NULL,NULL);
  gst_version (&gsmajor, &gsminor, &gsmicro, &gsnano);

  XEvent			xev;
  GC          gc;
  moConsole   Moldeo;
  gpConsole = &Moldeo;

  XineramaScreenInfo *m_screens;

  moResourceManager* pResourceMan = NULL;
  moRenderManager* pRenderMan = NULL;
  moGLManager* pGLMan = NULL;

  int render_width, render_height;
  int screen_width, screen_height;

  int screen_output = 0;


  XVisualInfo         *vi;
  XVisualInfo         *vi2;
  Colormap             cmap;
  Colormap             cmap2;
  XSetWindowAttributes swa;
  XSetWindowAttributes swa2;
  GLXContext           cx;
  GLXContext           cx2;

  GLXContext           moldeocx;

  XEvent               event;
  GLboolean            needRedraw = GL_FALSE, recalcModelView = GL_TRUE;
  int                  dummy;
  int                  screen_numbers;
  int screen_number = -1;

  twoscreens = false;

  //16 : 10
  screen_width = 800;
  screen_height = 600;
  // 640x400 : 800x500
  render_width = screen_width;
  render_height = screen_height;

  /*** (1) open a connection to the X server ***/
  cout << "MoldeoPlayer, compiled with X11 support " << "; libmoldeo version: " << (char*)moGetVersionStr()
    << "; Gstreamer version " << gsmajor << "." << gsminor << "." << gsmicro << "." << gsnano << endl;


  cout << "Processing arguments. n: " << argc << endl;

  arguments_ok = processarguments( argc, argv );

  moConfig checkConfig;
  bool config_ok = checkConfig.LoadConfig( molproject )==MO_CONFIG_OK;

  if (config_ok) {
    moText rw = checkConfig.GetParam( moText("outputresolution") ).GetValue( 0 ).GetSubValue( 0 ).ToText();
    moText rh = checkConfig.GetParam( moText("outputresolution") ).GetValue( 0 ).GetSubValue( 1 ).ToText();
    moText outputmode = checkConfig.GetParam( moText("outputmode") ).GetValue( 0 ).GetSubValue( 0 ).ToText();
    if (rw.Length() && !cmdresolution ) {
      screen_width = atoi( rw );
      screen_height = atoi( rh );
      render_width = screen_width;
      render_height = screen_height;
    }

    if (outputmode.Length() && outputmode=="AUTOPLAY" ) {
        presentationmode = "AUTOPLAY";
    }
  } else {
    //test mode
    //exit(1);
  }

  if (presentationmode!="") {
    twoscreens = true;
  }

  if (screen_width>0)
    preview_width = max( screen_width/2, PLAYER_PREVIEW_WIDTH);
  if (screen_height>0)
   preview_height = max( screen_height/2, (preview_width* screen_height) / screen_width );

   cout << " pw:" << preview_width << " ph:" << preview_height << endl;

  dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    cerr << "could not open display" << endl;
    exit(1);
  }

  screen_number = DefaultScreen(dpy);
  m_screens = XineramaQueryScreens( dpy, &screen_numbers );
  cout << "screen numbers" << screen_numbers << " screen_number:" << screen_number << endl;

  if (screen_numbers>1) {
    screen_output = screen_numbers-1;
  }

  unsigned long white_pixel = WhitePixel( dpy, screen_output);
  unsigned long black_pixel = BlackPixel( dpy, screen_output);

  for( int s=0; s<screen_numbers; s++ ) {
    XineramaScreenInfo &sinfo = m_screens[ s ];
    cout << "screen :" << s << " x_org:" << sinfo.x_org << " y_org:" << sinfo.y_org << " width:" << sinfo.width << " height:" << sinfo.height << endl;
  }

  XineramaScreenInfo &screen_info = m_screens[ screen_output ];
  cout << "Using screen :" << screen_info.screen_number << " x_org:" << screen_info.x_org << " y_org:" << screen_info.y_org << " width:" << screen_info.width << " height:" << screen_info.height << endl;



  /*** (2) make sure OpenGL's GLX extension supported ***/

//  if(!glXQueryExtension(dpy, &dummy, &dummy))
//    fatalError("X server has no OpenGL GLX extension");

  /*** (3) find an appropriate visual ***/

  /* find an OpenGL-capable RGB visual with depth buffer */
if (1==2) {

  GLXFBConfig *fbc = NULL;
  int nelements = 0;

  int attrib_list[] =
                        {
                        GLX_RENDER_TYPE, GLX_RGBA_BIT,
                        GLX_DOUBLEBUFFER, 1,
                        GLX_RED_SIZE, 8,
                        GLX_GREEN_SIZE, 8,
                        GLX_BLUE_SIZE, 8,
                        GLX_ALPHA_SIZE, 8,
                        GLX_DEPTH_SIZE, 16,
                        GLX_STENCIL_SIZE, 8,
                        0
                        };


        fbc = glXChooseFBConfig( dpy,
                            screen_info.screen_number,
                            attrib_list,
                            &nelements );
  if (fbc==NULL) {
    cerr << "no FBConfig visual with depth buffer" << endl;
    exit(1);
  }
  vi = glXGetVisualFromFBConfig( dpy, fbc[0] );
} else {
  vi = glXChooseVisual(dpy, /*screen_output,*/ screen_info.screen_number, dblBuf);
  if (vi == NULL) {
    cout <<  "no RGB visual with double buffer" << endl;
    vi = glXChooseVisual(dpy, screen_number, dblBuf);
    if (vi == NULL) cout << "desktop extended need to reposition the window" << endl;
  }

}

  if (vi == NULL)
  {
    vi = glXChooseVisual(dpy, screen_info.screen_number, snglBuf);
    if (vi == NULL) {
      cout << "no RGB visual with singel buffer" << endl;
      doubleBuffer = GL_FALSE;
      exit(1);
    }
  }

  /*
  if(vi->class != TrueColor)
    fatalError("TrueColor visual required for this program");
  */
  /*** (4) create an OpenGL rendering context  ***/

  /* create an OpenGL rendering context */
  if (1==1) {
    cx = glXCreateContext(dpy, vi, /* moldeo shared dlists */ None,
                          /* direct rendering if possible */ GL_TRUE);
    if (cx == NULL) {
      cerr << "could not create rendering context cx" << endl;
      exit(1);
    }
    cx2 = glXCreateContext(dpy, vi, /* moldeo shared dlists */ cx,
                          /* direct rendering if possible */ GL_TRUE);

    if (cx2 == NULL) {
      cerr << "could not create rendering context cx2 for preview" << endl;
      exit(1);
    }

  }


  /*** (5) create an X window with the selected visual ***/

  /* create an X colormap since probably not using default visual */
/*
SDL2 X11
https://github.com/davidsiaw/SDL2/blob/b90a1cdaa54f998e759debae15f2479de610918a/src/video/x11/SDL_x11window.c
  */

  cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
  Pixmap pixmap = XCreatePixmap(dpy, RootWindow(dpy, vi->screen), screen_width, screen_height, 32);

  swa.colormap = cmap;
  swa.override_redirect = false;//((window->flags & SDL_WINDOW_TOOLTIP) || (window->flags & SDL_WINDOW_POPUP_MENU)) ? True : False;
  /*swa.background_pixmap = pixmap;*/
  swa.border_pixel = 0;
  swa.background_pixel = 0;
  //swa.cursor = None;
  //swa.event_mask = ExposureMask | StructureNotifyMask;
  //swa.event_mask = KeyPressMask;
  //swa.override_redirect = true;
  swa.event_mask = ResizeRedirectMask | StructureNotifyMask;
  /*swa.event_mask = ExposureMask
                 | ButtonPressMask
                 | ResizeRedirectMask
                 | StructureNotifyMask;*/


  //screen_width = screen_info.width;
  //screen_height = screen_info.height;
  //XSetWindowAttributes attr = { 0 };

  win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), screen_info.x_org, screen_info.y_org,
                      screen_width, screen_height, 0, vi->depth, InputOutput, vi->visual,
                      CWOverrideRedirect |CWBackPixel| CWBorderPixel | CWColormap | CWEventMask,
                      &swa);


  //XSetStandardProperties(dpy, win, molproject, molproject, None,
  //                       NULL, NULL, NULL);

/*
   Atom wm_state   = XInternAtom (dpy, "_NET_WM_STATE", true );
   Atom wm_fullscreen = XInternAtom (dpy, "_NET_WM_STATE_FULLSCREEN", true );

   XChangeProperty(dpy, win, wm_state, XA_ATOM, 32,
                   PropModeReplace, (unsigned char *)&wm_fullscreen, 1);
*/
  /*** (6) bind the rendering context to the window ***/

  int cursor_shape = XC_arrow;
  Cursor cursor;
  cursor = XCreateFontCursor(dpy, cursor_shape);
  XDefineCursor(dpy, win, cursor);


  glXMakeCurrent(dpy, win, cx);

  /*** (7) request the X window to be displayed on the screen ***/

  XMapWindow(dpy, win);

  XWindowAttributes xwa;
  XGetWindowAttributes( dpy, win, &xwa );

  XColor red_col;
  char red[] = "#FF0000";

  XParseColor(dpy, cmap, red, &red_col);
  XAllocColor(dpy, cmap, &red_col);

  XWindowAttributes xwa2;
  if (twoscreens) {
    win2 = XCreateWindow(dpy, RootWindow(dpy, vi->screen),
                         0, 0,
                        preview_width,
                        preview_height, 0,
                        vi->depth,
                        InputOutput,
                        vi->visual,
                        CWBorderPixel | CWColormap | CWEventMask, &swa);

    XSetStandardProperties(dpy, win2, "interface", "interface", None,
                           argv, argc, NULL);

    XMapWindow(dpy, win2);
    //gc = XCreateGC( dpy, win2, 0, 0 );
    //if (!gc)  fatalError("could not create graphic context on win2");
    XGetWindowAttributes( dpy, win2, &xwa2 );
  }

  /***********************************************/
  /**   INITIALIZING MOLDEO CONSOLE */
  /***********************************************/
  int maxloops = 120;
    int loops = maxloops;
    moDisplay DisplayInfo( screen_width, screen_height);

    moGLManager GL;
    GL.Init();

    /*if (GL.GetGLMajorVersion()<2) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_COLOR_TEXT,
                             "OpenGL HARDWARE",
                             moText("OpenGL Version: ") + GL.GetGLVersion()
                             + moText("\nHardware: ") + GL.GetGPUVendorString()
                             + moText("\nModel: ")+ GL.GetGPURendererString()
                             + moText("\nGstreamer Version: ")+IntToStr(gsmajor)+moText(".")
                                                                +IntToStr(gsminor),
                             NULL);
    }*/
      loops = 20;
      while(loops>0) {
          loops--;
          glClearColor( 0.0, 0.0, ((float) loops )/20.0f, 1.0 );
          glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
          glXSwapBuffers(dpy, win);
      }

      loops = maxloops;
    while(loops>0) {
      loops--;
      //delay(1);

      Moldeo.TestScreen(DisplayInfo);
      glXSwapBuffers(dpy, win);
    }
    moResetTicksAbsoluteStep();

  MO_HANDLE _WINDOW_HANDLE;
  MO_DISPLAY _DISPLAY = NULL;

 if (getcwd(app_path,1000)==NULL) cerr << "Cannot retreive working path!" << endl;

 if (!config_ok || !arguments_ok) {
    Moldeo.Finish();
    exit(-1);
  }

  _DISPLAY = dpy;
  _WINDOW_HANDLE = win;

  moX11_IODeviceManager* pIODeviceManager = new moX11_IODeviceManager();
  pIODeviceManager->Init( dpy, win, win2 );

  if (screennumber>=0 && screen_number<screen_numbers) {
    screen_info = m_screens[screennumber];
    if (fullscreenmode!="" || presentationmode!="") {
      if (fullscreenmode!="") moDebugManager::Message("Fullscreen mode on");
      if (presentationmode!="") moDebugManager::Message("Presentation mode on");
      XMoveResizeWindow(dpy,win,screen_info.x_org, screen_info.y_org, screen_info.width, screen_info.height);
      screen_width = screen_info.width;
      screen_height = screen_info.height;
      render_width = screen_width;
      render_height = screen_height;
      pIODeviceManager->m_ResizeNeeded = true;
    } else {
      XMoveResizeWindow(dpy,win,screen_info.x_org, screen_info.y_org, screen_width, screen_height);
      pIODeviceManager->m_ResizeNeeded = true;
    }
  }


 if ( 1==1 &&

      Moldeo.Init( app_path,
                          molfolder, molproject,
                          pIODeviceManager /*IODeviceManager*/,
                            NULL/*ResourceManager*/,
                            RENDERMANAGER_MODE_NORMAL /*render mode*/,
                            screen_width, screen_height,
                            render_width,render_height,
                            _WINDOW_HANDLE,
                            _DISPLAY
                         )
     ) {
    moDebugManager::Message("moldeoplayer::main > Moldeo Initialized successfully ! ");

    /** getting rendering gl contexts and displays */

    pResourceMan = Moldeo.GetResourceManager();
    if (pResourceMan) {
      pRenderMan = pResourceMan->GetRenderMan();
      pGLMan = pResourceMan->GetGLMan();

      /** **************************************************/
      /** CHANGE WINDOW SIZE IF CONFIG DID IT!!!! */
      /** **************************************************/
      /*
      render_width = pRenderMan->RenderWidth();
      render_height = pRenderMan->RenderHeight();
      screen_width = pRenderMan->ScreenWidth();
      screen_height = pRenderMan->ScreenHeight();
      */
      //dpy = (Display*) pGLMan->GetDisplayServer();
      //moldeocx = (GLXContext) pGLMan->GetContext();
      pRenderMan->SetInterfaceView( preview_width, preview_height);
    }

    Moldeo.ConsolePlay();
    moDebugManager::Message("init w:"+IntToStr(preview_width)+" h:"+IntToStr(preview_height));
    moDebugManager::Message("init iw:"+IntToStr(pRenderMan->InterfaceWidth())+" ih:"+IntToStr(pRenderMan->InterfaceHeight()));
  } else {
    moDebugManager::Error("moldeoplayer::main > Moldeo Initialization with errors. ");
    //exit(1);
  }
  /***********************************************/
  /***********************************************/
  /*
      // Inicializamos en primer lugar
      if ( Moldeo.Init( moText("test"),
                              moText("test.mol") ) ) {

          // Ciclo principal
          // Mientras interactuamos
          while( Moldeo.Interaction() ) {

              //Actualizamos
              Moldeo.Update();

              //Dibujamos
              Moldeo.Draw();
          }
       }
  */


  //glXMakeContextCurrent( dpy, win, win, moldeocx );


  /*** (8) configure the OpenGL context for rendering ***/

//  glEnable(GL_DEPTH_TEST); /* enable depth buffering */
//  glDepthFunc(GL_LESS);    /* pedantic, GL_LESS is the default */
//  glClearDepth(1.0);       /* pedantic, 1.0 is the default */

  /* frame buffer clears should be to black */
//  glClearColor(0.0, 0.0, 0.0, 0.0);

  /* set up projection transform */
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
  /* establish initial viewport */
  /* pedantic, full window size is default viewport */
//  glViewport(0, 0, 300, 300);
/*
  printf( "Press left mouse button to rotate around X axis\n" );
  printf( "Press middle mouse button to rotate around Y axis\n" );
  printf( "Press right mouse button to rotate around Z axis\n" );
  printf( "Press ESC to quit the application\n" );
*/
  /*** (9) dispatch X events ***/
  usleep( 2000 );
  while ( Moldeo.Interaction()
          && pIODeviceManager
          && !pIODeviceManager->m_CloseNeeded
          && !pIODeviceManager->m_CloseNeededForPreview )
  {

        if ( pIODeviceManager->m_ResizeNeeded ) {
            pIODeviceManager->m_ResizeNeeded = false;
            if (Moldeo.GetResourceManager()) {
            /*
              int x, y;
              XWindowAttributes getxwa;
              XGetWindowAttributes( dpy, win, &getxwa );
              moDebugManager::Message("getxwa: w: "+IntToStr(getxwa.width)+" h: "+IntToStr(getxwa.height));
              screen_width = getxwa.width;
              screen_height = getxwa.height;
              render_width = screen_width;
              render_height = screen_height;*/
              screen_width = Moldeo.GetResourceManager()->GetRenderMan()->ScreenWidth();
              screen_height = Moldeo.GetResourceManager()->GetRenderMan()->ScreenHeight();
              render_width = screen_width;
              render_height = screen_height;
              moDebugManager::Message("Resizing w: "+IntToStr(screen_width)+" h: "+IntToStr(screen_height));
              Moldeo.GetResourceManager()->GetRenderMan()->Finish();
              Moldeo.GetResourceManager()->GetRenderMan()->Init( RENDERMODE,
                                                                screen_width, screen_height,
                                                                render_width, render_height );
            }
        }

      /** RENDER OUTPUT */
      glXMakeCurrent(dpy, win, cx);
      Moldeo.Update();
      Moldeo.Draw();
      glXSwapBuffers(dpy, win);

      moEvent* event = pIODeviceManager->GetEvents()->First;
      moMessage* newMessage = NULL;
      while(event!=NULL) {
        if ( event->deviceid == MO_IODEVICE_KEYBOARD || event->deviceid==999 ) {
          if (event->devicecode == SDL_KEYDOWN || event->devicecode==0) {
            if ( event->reservedvalue0 == SDLK_ESCAPE || event->reservedvalue3 == MO_MESSAGE ) {
              if (event->reservedvalue0 == SDLK_ESCAPE) {

              }
              //SwitchPresentation( Moldeo );
              //pIODeviceManager->m_displayWindowId = displayWindowId;
              //pIODeviceManager->m_previewWindowId = previewWindowId;
            }
          }
        }

        if (event->deviceid==999) {
            newMessage = (moMessage*) event;
        }

        event = event->next;
      }

      if ( newMessage )
        pIODeviceManager->GetEvents()->Delete(newMessage);


      /** RENDER INTERFACE */
      if (twoscreens && pRenderMan) {

          glXMakeCurrent(dpy, win2, cx );
          //glViewport( 0, 0, pRenderMan->InterfaceWidth(), pRenderMan->InterfaceHeight());
          if (preview_width != pRenderMan->InterfaceWidth()) {
            moDebugManager::Message("pw:"+IntToStr(preview_width)+" ph:"+IntToStr(preview_height));
            moDebugManager::Message("iw:"+IntToStr(pRenderMan->InterfaceWidth())+" ih:"+IntToStr(pRenderMan->InterfaceHeight()));
            preview_width = pRenderMan->InterfaceWidth();
            preview_height = pRenderMan->InterfaceHeight();
            moDebugManager::Message("xwa2.x:"+IntToStr(xwa2.x)+" xwa2.y:"+IntToStr(xwa2.y) );
            XMoveResizeWindow( dpy, win2,
              m_screens[0].x_org+m_screens[0].width/2-preview_width/2,
              m_screens[0].y_org+m_screens[0].height/2-preview_height/2, preview_width, preview_height);
          }
          glViewport( 0, 0, preview_width, preview_height);
          glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

          pRenderMan->DrawTexture( preview_width, preview_height, MO_FINAL_TEX );
          //Moldeo.DrawMasterEffects(0,0);
          if (!swapped) {
            glXSwapBuffers(dpy, win2);
            swapped=true;
          } else swapped=false;

          /*
          XSetForeground( dpy, gc, red_col.pixel);
          XDrawRectangle( dpy, win2, gc, 0, 0, interface_width, interface_height );
          XFlush(dpy);
          */
      }

      //copiar la textura final
      //dibujar los Master Effects ?


      //printf( "Press ESC to quit the application\n" );
      //moDebugManager::Message("hello");
/*
    while(XCheckWindowEvent(dpy, win, KeyPressMask, &xev)) {
      char	*key_string1 = XKeysymToString(XkbKeycodeToKeysym(dpy, xev.xkey.keycode, 0, 0));
      moDebugManager::Message( key_string1 );
    }

    while(XCheckWindowEvent(dpy, win2, KeyPressMask, &xev)) {
      char	*key_string2 = XKeysymToString(XkbKeycodeToKeysym(dpy, xev.xkey.keycode, 0, 0));
      moDebugManager::Message( key_string2 );
    }
*/
  }

  Moldeo.Finish();

  XFree(m_screens);

  return 0;
}
