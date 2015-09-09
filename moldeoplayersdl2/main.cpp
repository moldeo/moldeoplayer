/*
 *
 */

//sdl 1.2 support
//#define SDL_1

//sdl 2.0 support (multiwindow)
#define SDL_2


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
  PLAYER_PRESENTATION=2 /** SECOND DISPLAY IF AVAILABLE OR FULLSCREEN ON FIRST DISPLAY */
};

#define PLAYER_WINDOWED_WIDTH 640
#define PLAYER_WINDOWED_HEIGHT 360
#define PLAYER_PREVIEW_WIDTH 400
#define PLAYER_PREVIEW_HEIGHT 220

int screen_left = 0, screen_top = 0;
int screen_display = 0;
int render_width, render_height ,screen_width, screen_height;
int preview_width  = PLAYER_PREVIEW_WIDTH, preview_height = PLAYER_PREVIEW_HEIGHT;

moText config("");
moText molproject("");
moText molproject_preview = molproject+" Preview";
moText molfolder("");
moText gamemode("");
moText presentationmode("");
moText mwindow("");
moText windowtitle("");
SDL_Window* displayWindow = NULL;
SDL_Window* previewWindow = NULL;
int displayWindowId = -1;
int previewWindowId = -1;

SDL_GLContext context;
SDL_GLContext context2;
moPlayerStateWindow PlayerState = PLAYER_WINDOWED;

int processarguments( int argc, char** argv ) {

    if (argc<2) {
			cout << "Missing arguments!!! usage: moldeoplayerglut [-mol ../../sample.mol ] [-window 640x480] [-gamemode 1024x768:32]" << endl;
			cout.flush();
			exit(1);
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
        } else if (  arglast == moText("--help") ) {

            cout << "Usage: " << moText(arg0) << " [-mol ../../sample.mol ] [-window 640x480] [-gamemode 1024x768:32]" << endl;

        } else {
          cout << "Usage: " << moText(arg0) << " [-mol ../../sample.mol ] [-window 640x480] [-gamemode 1024x768:32]" << endl;
          exit(0);
        }
    }
	return 0;
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
                           SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN_DESKTOP|SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
        */
        break;
      case PLAYER_GAMEMODE:
        PlayerState = PLAYER_PRESENTATION;
        if (previewWindow) {
          SDL_ShowWindow( previewWindow );
        } else {
          SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
          previewWindow = SDL_CreateWindow( molproject+" Preview",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        PLAYER_PREVIEW_WIDTH,
                                        PLAYER_PREVIEW_HEIGHT,
                                        SDL_WINDOW_OPENGL
                                        |SDL_WINDOW_RESIZABLE
                                        |SDL_WINDOW_SHOWN );
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
                           SDL_WINDOW_OPENGL|SDL_WINDOW_BORDERLESS|SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
        */
        break;
      case PLAYER_PRESENTATION:
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
                           SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
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
}

/* Main function:  */
int main(int argc, char** argv) {

  char app_path[1000];


  cout << "Console output: " << fileno(stdout) << endl;
  cout << "MoldeoPlayer SDL2 version 1.0, libmoldeo version: " << (char*)moGetVersionStr() << endl;


  moConsole   Moldeo;
  /*Initialization default*/
  screen_width = 640;
  screen_height = 480;
  render_width = screen_width;
  render_height = screen_height;

  if (SDL_Init( SDL_INIT_EVERYTHING )<0) {
    cout << "Couldn't init SDL" << endl;
    exit(1);
  }

  //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2.0 );
  //SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1.0 );
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  //SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32 );
  //SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

#ifdef MO_WIN32
  freopen( "CON", "wt", stdout );
  freopen( "CON", "wt", stderr );
#endif

  cout << "Processing arguments. n: " << argc << endl;

  processarguments( argc, argv );

  moConfig checkConfig;

  if (checkConfig.LoadConfig( molproject )==MO_CONFIG_OK) {
    moText rw = checkConfig.GetParam( moText("outputresolution") ).GetValue( 0 ).GetSubValue( 0 ).ToText();
    moText rh = checkConfig.GetParam( moText("outputresolution") ).GetValue( 0 ).GetSubValue( 1 ).ToText();
    moText outputmode = checkConfig.GetParam( moText("outputmode") ).GetValue( 0 ).GetSubValue( 0 ).ToText();
    if (rw.Length()) {
      screen_width = atoi( rw );
      screen_height = atoi( rh );
      render_width = screen_width;
      render_height = screen_height;
    }

    if (outputmode.Length() && outputmode=="AUTOPLAY" ) {
        presentationmode = "AUTOPLAY";
    }
  } else exit(1);
/*
*/
  getcwd(app_path,1000);



  if (mwindow!=moText("")) {
      //if ( SDL_SetVideoMode( screen_width, screen_height, 32, SDL_OPENGL|SDL_DOUBLEBUF|SDL_RESIZABLE ) == NULL) {
      displayWindow = SDL_CreateWindow(molproject, SDL_WINDOWPOS_CENTERED, 40, screen_width, screen_height,
                                       SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
      PlayerState = PLAYER_WINDOWED;
      if (displayWindow==NULL) {
        cout << "Couldn't set SDL_SetVideoMode to window mode (resizable) " << screen_width << "x" << screen_height << " in 32 bits mode " << endl;
        exit(1);
      }
      //default mode
  } else if (gamemode!=moText("")) {

      //if ( SDL_SetVideoMode( screen_width, screen_height, 32, SDL_OPENGL|SDL_DOUBLEBUF|SDL_FULLSCREEN ) == NULL) {
      displayWindow = SDL_CreateWindow(molproject, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
                                       SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN_DESKTOP|SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
      PlayerState = PLAYER_GAMEMODE;
      if (displayWindow==NULL) {
        cout << "Couldn't set SDL_SetVideoMode to gamemode (fullscreen) " << screen_width << "x" << screen_height << " in 32 bits mode " << endl;
        exit(1);
      }
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
                                       /*|SDL_WINDOW_RESIZABLE*/
                                       |SDL_WINDOW_SHOWN
                                       |SDL_WINDOW_MOUSE_FOCUS );
      context = SDL_GL_CreateContext(displayWindow);

      SDL_GL_MakeCurrent( displayWindow, context );
      //SDL_GL_SetSwapInterval(1);
      SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
      previewWindow = SDL_CreateWindow( molproject_preview,
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        PLAYER_PREVIEW_WIDTH,
                                        PLAYER_PREVIEW_HEIGHT,
                                        SDL_WINDOW_OPENGL
                                        |SDL_WINDOW_RESIZABLE
                                        |SDL_WINDOW_SHOWN );
      context2 = SDL_GL_CreateContext( displayWindow );

      PlayerState = PLAYER_PRESENTATION;
      if (displayWindow==NULL) {
        cout << "Couldn't set SDL_SetVideoMode to gamemode (fullscreen) " << screen_width << "x" << screen_height << " in 32 bits mode " << endl;
        exit(1);
      }
  } else {

      displayWindow = SDL_CreateWindow(molproject, 40, 40, screen_width, screen_height,
                                       SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE|SDL_WINDOW_SHOWN|SDL_WINDOW_MOUSE_FOCUS );
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


  cout << "r:" << r << " g:" << g << " b:" << b << " a:" << a << " buffer size:" << bf << " double buf:" << db << " depth:" << dp << " stencil:" << st << endl;
  cout << "sdl_error:" << SDL_GetError() << endl;

  int loops = 10;

  while(loops>0) {
    loops--;
    //delay(1);
    SDL_GL_SwapWindow(displayWindow);
  }

  unsigned int gsmajor, gsminor, gsmicro, gsnano;

  gst_init(NULL,NULL);
  gst_version (&gsmajor, &gsminor, &gsmicro, &gsnano);
  cout << "Gstreamer version " << gsmajor << "." << gsminor << "." << gsmicro << "." << gsnano << endl;

  //pIODeviceManager = NULL;

  bool res = Moldeo.Init( app_path,
                          molfolder, molproject,
                          pIODeviceManager /*IODeviceManager*/, NULL/*ResourceManager*/,
                          RENDERMODE /*render mode*/,
                          screen_width,
                          screen_height,
                          render_width,
                          render_height
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

        if (loops==0) {
          Moldeo.GetResourceManager()->GetTextureMan()->RefreshAll();
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
             /* &&
              (SDL_GetWindowFlags(previewWindow)&SDL_WINDOW_SHOWN ) */
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
            /*&&
              (SDL_GetWindowFlags(previewWindow)&SDL_WINDOW_SHOWN )*/
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

          Moldeo.GetResourceManager()->GetGLMan()->SetOrthographicView(preview_width, preview_height);

          ///Moldeo.GetResourceManager()->GetRenderMan()->DrawTexture( 400, 300, MO_SCREEN_TEX );
          glEnable( GL_TEXTURE_2D );
          glDisable(GL_DEPTH);
          glDisable(GL_DEPTH_TEST);
//          Moldeo.GetResourceManager()->GetRenderMan()->DrawTexture( 400, 300, MO_SCREEN_TEX );
          ///glBindTexture(GL_TEXTURE_2D, Moldeo.GetResourceManager()->GetRenderMan()->RenderTexGLId(MO_RENDER_TEX));
          glBindTexture( GL_TEXTURE_2D, glidprev );

          glColor4f( 1.0, 1.0, 1.0, 1.0);
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


          SDL_GL_SwapWindow( previewWindow);
          SDL_GL_MakeCurrent( displayWindow, context );

        }

    }
  }

  exit(0);

  Moldeo.Finish();

  SDL_DestroyWindow(displayWindow);

  SDL_Quit();

  return 0;
}
