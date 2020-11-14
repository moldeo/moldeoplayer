/*
 *
 */

//sdl 1.2 support
//#define SDL_1

//sdl 2.0 support (multiwindow)
#define SDL_2
#undef MODEBUG

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>

using namespace std;

void delay(int secs) {
    for(int i = (time(NULL) + secs); time(NULL) != i; time(NULL));
}

#include "gst/gst.h"
#include "moldeo.h"


moConsole* gpConsole = NULL;
//#define RENDERMODE RENDERMANAGER_MODE_FRAMEBUFFER
#define RENDERMODE RENDERMANAGER_MODE_NORMAL

#ifdef MO_MACOSX
#endif

#ifdef MO_LINUX
#endif

#ifdef WIN32
    #include "SDL.h"
    //#include "SDL_opengl.h"
    #define GL_TEXTURE_NON_POWER_OF_TWO GLEW_ARB_texture_non_power_of_two
#else
    #define GL_TEXTURE_NON_POWER_OF_TWO !Moldeo.GetResourceManager()->GetRenderMan()->IsTextureNonPowerOf2Disabled()
#endif

#include "SDL_syswm.h"

#include "moSDL_IODeviceManager.h"
moSDL_IODeviceManager* pIODeviceManager = NULL;

/*
void ResizeWindow( int width, int height) {

  glutReshapeWindow(width, height);
  //glViewport(0,0,width,height);
  #ifndef MOLDEOTEST
    moResourceManager* RESOURCES = gpConsole->GetResourceManager();
    if (RESOURCES) {
        moRenderManager* RENDER = RESOURCES->GetRenderMan();
        if (RENDER) RENDER->SetView( width, height );
    }
  #endif
}
*/

/*
void MoldeoDisplay() {
  #ifndef MOLDEOTEST
  if (gpConsole) {
    gpConsole->Interaction();
    gpConsole->Update();
    gpConsole->Draw();
    glFlush();
    glutSwapBuffers();
  }
  #else
    display();
  #endif
}
*/
enum moPlayerStateWindow {
  PLAYER_WINDOWED=0,    /** WINDOWED ON FIRST DISPLAY */
  PLAYER_GAMEMODE=1,    /** FULLSCREEN ON FIRST DISPLAY */
  PLAYER_PRESENTATION=2, /** SECOND DISPLAY IF AVAILABLE OR FULLSCREEN ON FIRST DISPLAY */
  PLAYER_FULLSCREEN=3
};

#ifdef OPENGLESV2
#define MO_SDL_WINDOW_SHOWN 0
#else
#define MO_SDL_WINDOW_SHOWN SDL_WINDOW_SHOWN
#endif // OPENGLESV2
#define PLAYER_WINDOWED_WIDTH 640
#define PLAYER_WINDOWED_HEIGHT 360
#define PLAYER_PREVIEW_WIDTH 400
#define PLAYER_PREVIEW_HEIGHT 220

int screen_left = 0, screen_top = 0;
int screen_display = 0;
int render_width, render_height ,screen_width, screen_height;
int preview_width  = PLAYER_PREVIEW_WIDTH, preview_height = PLAYER_PREVIEW_HEIGHT;

Uint32 window_resizable = SDL_WINDOW_RESIZABLE;

moText config("");
moText molproject("");
moText molproject_preview = molproject+" Preview";
moText molfolder("");
moText gamemode("");
moText fullscreenmode("");
moText presentationmode("");
moText mwindow("");
moText windowtitle("");
SDL_Window* displayWindow = NULL;
SDL_Window* previewWindow = NULL;
int displayWindowId = -1;
int previewWindowId = -1;
bool cmdresolution = false;

SDL_GLContext context;
SDL_GLContext context2;
moPlayerStateWindow PlayerState = PLAYER_WINDOWED;
bool arguments_ok = false;

int processarguments( int argc, char** argv ) {

    string player_manual = "moldeoplayersdl2 -mol moldeoprojectfilename.mol [-verbose] [-window 640x480] [-gamemode 1024x768:32] [-fullscreen] [-presentationmode]";

    if (argc<2) {
			cout << "Missing arguments!" << endl;
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


    while( argc >= 2 ) {
        --argc;

        moText arglast(argv[argc]);
        moText argprev;

        if (argc>0) argprev = argv[argc-1];

        moText  arg0(argv[0]);

        cout <<  "Argument [" << (argc) << "] : " << moText(arglast) << endl;

        moFile molFile( arglast );

        if( argprev == moText("-mol")
           || molFile.GetExtension() == moText(".mol") ) {

            molproject = moText( arglast );
                moFile file( molproject );
                molfolder = file.GetPath();
            cout << "Argument -mol is: " << molproject << " in folder: " << molfolder << endl;

          if ( argprev == moText("-mol") ) --argc;

        } else if (  argprev == moText("-window") ) {

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
                --argc;

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
                --argc;
        } else if (  argprev == moText("-fullscreen") ) {

            fullscreenmode = moText( arglast );
                cout << "Fullscreen mode -fullscreen found! : " << fullscreenmode << endl;
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
                --argc;
        } else if (  argprev == moText("-presentationmode") ) {
          presentationmode = moText( arglast );
          cout << "Presentation mode -presentationmode found! : " << presentationmode << endl;
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
          --argc;
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



int SetScreenDisplay( int displayid=-1 ) {
  /*

                    for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i) {
                      int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);
                    }

  */

  SDL_DisplayMode current;
  SDL_Rect displayBoundaries;
  int resizeRenderMan = 0;
  if (displayid==-1) {
    int last_display = SDL_GetNumVideoDisplays()-1;
    if (last_display>0) {
      SDL_GetCurrentDisplayMode( last_display, &current);
      SDL_GetDisplayBounds( last_display, &displayBoundaries );
      if ( screen_width!=displayBoundaries.w
          && screen_height!=displayBoundaries.h ) {
         resizeRenderMan = 1;
      }
      screen_left = displayBoundaries.x;
      screen_top = displayBoundaries.y;
      screen_width = displayBoundaries.w;
      screen_height = displayBoundaries.h;
      render_width = screen_width;
      render_height = screen_height;
      cout << " screen_left:" << screen_left << " screen_top:" << screen_top;
      cout << " screen_width:" << screen_width << " screen_height:" << screen_height;
      cout << " render_width:" << render_width << " render_height:" << render_height << endl;

    }
  }

  return resizeRenderMan;
}

void SwitchPresentation( moConsole& Moldeo ) {
    //SDL_DestroyWindow(displayWindow);
    switch(PlayerState) {
      case PLAYER_WINDOWED:
        PlayerState = PLAYER_GAMEMODE;
        SDL_SetWindowBordered( displayWindow, SDL_FALSE );
        SDL_SetWindowPosition( displayWindow, 0, 0 );
        SDL_SetWindowSize( displayWindow, screen_width, screen_height );
        SDL_SetWindowFullscreen( displayWindow, SDL_WINDOW_FULLSCREEN_DESKTOP );
        SDL_ShowCursor(SDL_DISABLE);
        /*
        displayWindow = SDL_CreateWindow(molproject, 0, 0, screen_width, screen_height,
                           SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN_DESKTOP|MO_SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
        */
        break;
      case PLAYER_GAMEMODE:
        PlayerState = PLAYER_PRESENTATION;
        if (previewWindow) {
          SDL_ShowWindow( previewWindow );
        } else {
          SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
          previewWindow = SDL_CreateWindow( "Preview "+molproject,
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        preview_width,
                                        preview_height,
                                        SDL_WINDOW_OPENGL
                                        |window_resizable
                                        |MO_SDL_WINDOW_SHOWN );
          context2 = SDL_GL_CreateContext( displayWindow );
        }
        SDL_ShowCursor(SDL_DISABLE);
        if ( 1 == SetScreenDisplay(-1) ) {
          if (Moldeo.GetResourceManager()) {
            Moldeo.GetResourceManager()->GetRenderMan()->Finish();
            Moldeo.GetResourceManager()->GetRenderMan()->Init( RENDERMODE,
                                                              screen_width, screen_height,
                                                              render_width, render_height );
          }
        }
        SDL_SetWindowFullscreen( displayWindow, 0 );
        SDL_SetWindowBordered( displayWindow, SDL_FALSE );
        SDL_SetWindowPosition( displayWindow, screen_left, screen_top );
        SDL_SetWindowSize( displayWindow, screen_width, screen_height );
        /*
        displayWindow = SDL_CreateWindow(molproject, screen_left, screen_top, screen_width, screen_height,
                           SDL_WINDOW_OPENGL|SDL_WINDOW_BORDERLESS|MO_SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
        */
        break;
      case PLAYER_PRESENTATION:
        PlayerState = PLAYER_FULLSCREEN;
        //SDL_SetWindowBordered( displayWindow, SDL_FALSE );
        //SDL_SetWindowPosition( displayWindow, 0, 0 );
        //SDL_SetWindowSize( displayWindow, screen_width, screen_height );
        SDL_SetWindowFullscreen( displayWindow, SDL_WINDOW_FULLSCREEN );
        SDL_ShowCursor(SDL_DISABLE);
        break;

     case PLAYER_FULLSCREEN:
        PlayerState = PLAYER_WINDOWED;

        SDL_SetWindowFullscreen( displayWindow, 0 );
        SDL_SetWindowBordered( displayWindow, SDL_TRUE );
        SDL_SetWindowPosition( displayWindow, 40, 40 );
        screen_width = PLAYER_WINDOWED_WIDTH;
        screen_height = PLAYER_WINDOWED_HEIGHT;
        render_width = screen_width;
        render_height = screen_height;

        if (previewWindow)
          SDL_MinimizeWindow( previewWindow );

        SDL_SetWindowSize( displayWindow, screen_width, screen_height );
        SDL_ShowCursor(SDL_ENABLE);
        if (Moldeo.GetResourceManager()) {
          Moldeo.GetResourceManager()->GetRenderMan()->Finish();
          Moldeo.GetResourceManager()->GetRenderMan()->Init( RENDERMODE,
                                                            screen_width, screen_height,
                                                            render_width, render_height );
        }
        /*
        displayWindow = SDL_CreateWindow(molproject, 0, 0, screen_width, screen_height,
                           SDL_WINDOW_OPENGL|window_resizable|MO_SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
        */
        break;
      default:
        break;
    }
    /*
    if (displayWindow)
      SDL_GL_MakeCurrent( displayWindow, context );
    else
      cout << "Couldn't create display Window!" << endl;
      */

    if (displayWindow)
      displayWindowId = SDL_GetWindowID(displayWindow);
    if (previewWindow)
      previewWindowId = SDL_GetWindowID(previewWindow);


}

/* Main function:  */
int main(int argc, char** argv) {

  char app_path[1000];
  unsigned int gsmajor, gsminor, gsmicro, gsnano;
  SDL_SysWMinfo info;

  gst_init(NULL,NULL);
  gst_version (&gsmajor, &gsminor, &gsmicro, &gsnano);

  moConsole   Moldeo;
  /*Initialization default*/
  screen_width = 640;
  screen_height = 480;
  render_width = screen_width;
  render_height = screen_height;

  if (SDL_Init( SDL_INIT_EVERYTHING ) != 0) {
    cout << "Couldn't init SDL: " << SDL_GetError() << endl;
    exit(1);
  }

  SDL_VERSION(&info.version);

  cout << "MoldeoPlayer, compiled with SDL version " << (int)(info.version.major) << "; libmoldeo version: " << (char*)moGetVersionStr()
  << "; Gstreamer version " << gsmajor << "." << gsminor << "." << gsmicro << "." << gsnano << endl;
  //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2.0 );
  //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1.0 );
#ifdef OPENGLESV2

  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

#endif
  //SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  //SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );
  //SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

#ifdef MO_WIN32
  freopen( "CON", "wt", stdout );
  freopen( "CON", "wt", stderr );
#endif

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
      if (screen_width>0)
        preview_width = max( screen_width/2, PLAYER_PREVIEW_WIDTH);
      if (screen_height>0)
       preview_height = max( screen_height/2, (preview_width* screen_height) / screen_width );
    }

    if (outputmode.Length() && outputmode=="AUTOPLAY" ) {
        presentationmode = "AUTOPLAY";
    }
  } else {
    //test mode
    //exit(1);
  }
/*
*/
  if (getcwd(app_path,1000)==NULL) cerr << "Cannot retreive working path!" << endl;



  if (mwindow!=moText("")) {
      //if ( SDL_SetVideoMode( screen_width, screen_height, 32, SDL_OPENGL|SDL_DOUBLEBUF|SDL_RESIZABLE ) == NULL) {
      displayWindow = SDL_CreateWindow(molproject, SDL_WINDOWPOS_CENTERED, 40, screen_width, screen_height,
                                       SDL_WINDOW_OPENGL|window_resizable|MO_SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
      PlayerState = PLAYER_WINDOWED;
      if (displayWindow==NULL) {
        cout << "Couldn't set SDL_SetVideoMode to window mode (resizable) " << screen_width << "x" << screen_height << " in 32 bits mode " << endl;
        exit(1);
      }
      context = SDL_GL_CreateContext(displayWindow);
      //default mode
  } else if (fullscreenmode!=moText("")) {
      //if ( SDL_SetVideoMode( screen_width, screen_height, 32, SDL_OPENGL|SDL_DOUBLEBUF|SDL_RESIZABLE ) == NULL) {
      displayWindow = SDL_CreateWindow(molproject, SDL_WINDOWPOS_CENTERED, 40, screen_width, screen_height,
                                       SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN|window_resizable|MO_SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
      PlayerState = PLAYER_FULLSCREEN;
      if (displayWindow==NULL) {
        cout << "Couldn't set SDL_SetVideoMode to window mode (resizable) " << screen_width << "x" << screen_height << " in 32 bits mode " << endl;
        exit(1);
      }
      context = SDL_GL_CreateContext(displayWindow);

      //SDL_SetWindowFullscreen( displayWindow, SDL_WINDOW_FULLSCREEN );
      //default mode
  } else if (gamemode!=moText("")) {

      //if ( SDL_SetVideoMode( screen_width, screen_height, 32, SDL_OPENGL|SDL_DOUBLEBUF|SDL_FULLSCREEN ) == NULL) {
      displayWindow = SDL_CreateWindow(molproject, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
                                       SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN_DESKTOP|MO_SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
      PlayerState = PLAYER_GAMEMODE;
      if (displayWindow==NULL) {
        cout << "Couldn't set SDL_SetVideoMode to gamemode (fullscreen) " << screen_width << "x" << screen_height << " in 32 bits mode " << endl;
        exit(1);
      }
      context = SDL_GL_CreateContext(displayWindow);
  } else if (presentationmode!=moText("")) {
      SetScreenDisplay(-1);

      SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 0);
      displayWindow = SDL_CreateWindow( molproject,
                                       screen_left,
                                       screen_top,
                                       screen_width,
                                       screen_height,
                                       SDL_WINDOW_OPENGL
                                       |SDL_WINDOW_BORDERLESS
                                       /*|SDL_WINDOW_FULLSCREEN_DESKTOP*/
                                       /*|window_resizable*/
                                       |MO_SDL_WINDOW_SHOWN
                                       |SDL_WINDOW_MOUSE_FOCUS );
      context = SDL_GL_CreateContext(displayWindow);

      SDL_GL_MakeCurrent( displayWindow, context );
      //SDL_GL_SetSwapInterval(1);
      SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
      previewWindow = SDL_CreateWindow( molproject_preview,
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        preview_width,
                                        preview_height,
                                        SDL_WINDOW_OPENGL
                                        |window_resizable
                                        |MO_SDL_WINDOW_SHOWN );
      context2 = SDL_GL_CreateContext( displayWindow );

      PlayerState = PLAYER_PRESENTATION;
      if (displayWindow==NULL) {
        cout << "Couldn't set SDL_SetVideoMode to gamemode (fullscreen) " << screen_width << "x" << screen_height << " in 32 bits mode " << endl;
        exit(1);
      }
  } else {

      displayWindow = SDL_CreateWindow(molproject, 40, 40, screen_width, screen_height,
                                             SDL_WINDOW_OPENGL|window_resizable|SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
//                                       SDL_WINDOW_OPENGL|window_resizable|SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
      PlayerState = PLAYER_WINDOWED;
      //if ( SDL_SetVideoMode( screen_width, screen_height, 32, SDL_OPENGL|SDL_DOUBLEBUF|SDL_RESIZABLE ) == NULL) {
      if (displayWindow==NULL) {
        cout << "Couldn't set SDL_SetVideoMode to window mode (resizable) " << screen_width << "x" << screen_height << " in 32 bits mode " << endl;
        exit(1);
      }
      context = SDL_GL_CreateContext(displayWindow);
  }



  //SDL_WM_SetCaption( molproject, NULL );
  if (displayWindow)
    SDL_GL_MakeCurrent( displayWindow, context );

  pIODeviceManager = new moSDL_IODeviceManager();
  pIODeviceManager->Init(NULL);
  pIODeviceManager->SetWindows( displayWindow, previewWindow );

  int r, g, b, a, bf, db, st, dp;
  SDL_GL_GetAttribute(SDL_GL_RED_SIZE, &r);
  SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE, &g);
  SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &b);
  SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE, &a);
  SDL_GL_GetAttribute(SDL_GL_BUFFER_SIZE, &bf);
  SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &db);
  SDL_GL_GetAttribute(SDL_GL_DEPTH_SIZE, &dp);
  SDL_GL_GetAttribute(SDL_GL_STENCIL_SIZE, &st);
  SDL_GetWindowSize( displayWindow, &screen_width, &screen_height );

  cout << "r:" << r << " g:" << g << " b:" << b << " a:" << a << " buffer size:" << bf << " double buf:" << db << " depth:" << dp << " stencil:" << st << endl;
  cout << "sdl_error?:" << SDL_GetError() << endl;
  cout << "screen_width:" << screen_width << " screen_height:" << screen_height << endl;


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
#ifndef MODEBUG
        SDL_GL_SwapWindow(displayWindow);
#endif
    }

    loops = maxloops;
  while(loops>0) {
    loops--;
    //delay(1);

    Moldeo.TestScreen(DisplayInfo);

#ifndef MODEBUG
    SDL_GL_SwapWindow(displayWindow);
      //cout << loops << endl;
#endif
  }
  moResetTicksAbsoluteStep();


  glClearColor( 0.0, 0.0, 0.0, 1.0 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
#ifndef MODEBUG
  SDL_GL_SwapWindow(displayWindow);
#endif
  glClearColor( 0.0, 0.0, 0.0, 1.0 );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
#ifndef MODEBUG
  SDL_GL_SwapWindow(displayWindow);
#endif

  MO_HANDLE _WINDOW_HANDLE;
  MO_DISPLAY _DISPLAY = NULL;
  /** https://wiki.libsdl.org/SDL_SysWMinfo */
  if(SDL_GetWindowWMInfo(displayWindow,&info)) { /* the call returns true on success */
    /* success */
    const char *subsystem = "an unknown system!";
    switch(info.subsystem) {
      case SDL_SYSWM_UNKNOWN:   break;
#ifdef MO_WIN32
      case SDL_SYSWM_WINDOWS:   subsystem = "Microsoft Windows(TM)"; _WINDOW_HANDLE = info.info.win.window; _DISPLAY = info.info.win.hdc;  break;
#if SDL_VERSION_ATLEAST(2, 0, 3)
      case SDL_SYSWM_WINRT:     subsystem = "WinRT";                  break;
#endif
#endif // MO_WIN32
#ifdef MO_LINUX
      case SDL_SYSWM_X11:       subsystem = "X Window System"; _WINDOW_HANDLE = info.info.x11.window; _DISPLAY = info.info.x11.display;        break;
#endif
#ifndef MO_WIN32
      case SDL_SYSWM_DIRECTFB:  subsystem = "DirectFB";               break;
      case SDL_SYSWM_COCOA:     subsystem = "Apple OS X";             break;
      case SDL_SYSWM_UIKIT:     subsystem = "UIKit";                  break;
#endif // MO_WIN32
#if SDL_VERSION_ATLEAST(2, 0, 2)
      case SDL_SYSWM_WAYLAND:   subsystem = "Wayland";                break;
      case SDL_SYSWM_MIR:       subsystem = "Mir";                    break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 4)
      case SDL_SYSWM_ANDROID:   subsystem = "Android";                break;
#endif
#if SDL_VERSION_ATLEAST(2, 0, 5)
      case SDL_SYSWM_VIVANTE:   subsystem = "Vivante";                break;
#endif
    }

    cout << "This program is running SDL version "
    << (int)info.version.major
    << "." << (int)info.version.minor
    << "." <<  (int)(info.version.patch)
    << " " << subsystem << endl;
  } else {
    /* call failed */
    cout << "SDL ERROR: Couldn't get window information: " << SDL_GetError() << endl;
  }


  if (!config_ok || !arguments_ok) {
    Moldeo.Finish();
    SDL_DestroyWindow(displayWindow);
    SDL_Quit();
    return 0;
  }

  //pIODeviceManager = NULL;

  bool res = Moldeo.Init( app_path,
                          molfolder, molproject,
                          pIODeviceManager , NULL,
                          RENDERMODE ,
                          screen_width,
                          screen_height,
                          render_width,
                          render_height,
                          _WINDOW_HANDLE,
                          _DISPLAY
                         );


  if (presentationmode!="") pIODeviceManager->m_ResizeNeeded = true;

  int ttid = Moldeo.GetResourceManager()->GetTextureMan()->m_preview_texture_idx;
  moTexture* ptex = Moldeo.GetResourceManager()->GetTextureMan()->GetTexture(ttid);
  int glidprev = 0;
  if (ptex) {
      ///devuelve el gl if
      glidprev = ptex->GetGLId();

  }

  if (!res) {
    cout << "error couldnt init console" << endl;
    SDL_DestroyWindow(displayWindow);
    SDL_Quit();
    exit(1);
  }

  Moldeo.ConsolePlay();
  gpConsole = &Moldeo;

  loops = 10;

  if (displayWindow) {

    while( Moldeo.Interaction()
          && pIODeviceManager
          && !pIODeviceManager->m_CloseNeeded
          && !pIODeviceManager->m_CloseNeededForPreview ) {
        //glFlush();
        //SDL_GL_SwapBuffers();
        //if (Moldeo.GetResourceManager().GetRenderMan().FullScreen()) {
        //}
        //cout << "frame" << endl;

        if (loops==0) {
          //Moldeo.GetResourceManager()->GetTextureMan()->RefreshAll();
          loops = -1;//set to -1
        } else if (loops>0) {
          loops--;
        }


        if ( pIODeviceManager->m_ResizeNeeded ) {
            pIODeviceManager->m_ResizeNeeded = false;
            if (Moldeo.GetResourceManager()) {
              SDL_GetWindowSize( displayWindow, &screen_width, &screen_height);
              render_width = screen_width;
              render_height = screen_height;
              Moldeo.GetResourceManager()->GetRenderMan()->Finish();
              Moldeo.GetResourceManager()->GetRenderMan()->Init( RENDERMODE,
                                                                screen_width, screen_height,
                                                                render_width, render_height );
            }
        }

        moEvent* event = pIODeviceManager->GetEvents()->First;
        moMessage* newMessage = NULL;
        while(event!=NULL) {
          if ( event->deviceid == MO_IODEVICE_KEYBOARD || event->deviceid==999 ) {
            if (event->devicecode == SDL_KEYDOWN || event->devicecode==0) {
              if ( event->reservedvalue0 == SDLK_ESCAPE || event->reservedvalue3 == MO_MESSAGE ) {
                SwitchPresentation( Moldeo );
                pIODeviceManager->m_displayWindowId = displayWindowId;
                pIODeviceManager->m_previewWindowId = previewWindowId;
              }
            }
          }

          if (event->deviceid==999) {
              newMessage = (moMessage*) event;
          }

          event = event->next;
        }

        if ( newMessage ) {
          pIODeviceManager->GetEvents()->Delete(newMessage);
        }

        if (displayWindow) {

          Moldeo.Update();
          Moldeo.Draw();

          if (previewWindow

              ) {
            glBindTexture(GL_TEXTURE_2D, glidprev );
            if (!GL_TEXTURE_NON_POWER_OF_TWO) {
              glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,0,0,512,512,0);
            } else {
              glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,0,0,screen_width,screen_height,0);
            }
          }
          SDL_GL_SwapWindow(displayWindow);
        }

        if (previewWindow

            ) {


          SDL_GL_MakeCurrent( previewWindow, context2 );

          if ( pIODeviceManager->m_ResizeNeededForPreview ) {
            pIODeviceManager->m_ResizeNeededForPreview = false;
            if (Moldeo.GetResourceManager()) {
              SDL_GetWindowSize( previewWindow, &preview_width, &preview_height);
            }
          }


          glClearColor(0.5,0.0,0.0,0.0);
          glClear( GL_COLOR_BUFFER_BIT );

          Moldeo.GetResourceManager()->GetGLMan()->SetOrthographicView(preview_width, preview_height, 0, preview_width, 0, preview_height, -1, 1 );

          ///Moldeo.GetResourceManager()->GetRenderMan()->DrawTexture( 400, 300, MO_SCREEN_TEX );
          glEnable( GL_TEXTURE_2D );
#ifndef OPENGLESV2
          glDisable(GL_DEPTH);
#endif
          glDisable(GL_DEPTH_TEST);
//          Moldeo.GetResourceManager()->GetRenderMan()->DrawTexture( 400, 300, MO_SCREEN_TEX );
          ///glBindTexture(GL_TEXTURE_2D, Moldeo.GetResourceManager()->GetRenderMan()->RenderTexGLId(MO_RENDER_TEX));
          glBindTexture( GL_TEXTURE_2D, glidprev );

          glColor4f( 1.0, 1.0, 1.0, 1.0);
#ifndef OPENGLESV2
          glBegin(GL_QUADS);
            glTexCoord2f(0, 0.0);
            glVertex2f(0, 0);

            glTexCoord2f(1.0, 0.0);
            glVertex2f(preview_width, 0.0f);

            glTexCoord2f(1.0, 1.0);
            glVertex2f(preview_width, preview_height);

            glTexCoord2f(0, 1.0);
            glVertex2f(0.0f, preview_height);
          glEnd();
#endif

#ifndef MODEBUG
          SDL_GL_SwapWindow( previewWindow);
#endif
          SDL_GL_MakeCurrent( displayWindow, context );

        }

    }
  }

  //exit(0);

  Moldeo.Finish();

  SDL_DestroyWindow(displayWindow);

  SDL_Quit();

  return 0;
}
