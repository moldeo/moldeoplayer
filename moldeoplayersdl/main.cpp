/*
 *
 */

//sdl 1.2 support
#define SDL_1

//sdl 2.0 support (multiwindow)
//#define SDL_2


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


#ifdef MO_MACOSX
#endif

#ifdef MO_LINUX
#endif

#ifdef WIN32
    #include "SDL.h"
    //#include "SDL_opengl.h"
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

int render_width, render_height ,screen_width, screen_height;


moText config("");
moText molproject("");
moText molfolder("");
moText gamemode("");
moText mwindow("");
moText windowtitle("");

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

        } else if (  arglast == moText("--help") ) {

            cout << "Usage: " << moText(arg0) << " [-mol ../../sample.mol ] [-window 640x480] [-gamemode 1024x768:32]" << endl;

        } else {
          cout << "Usage: " << moText(arg0) << " [-mol ../../sample.mol ] [-window 640x480] [-gamemode 1024x768:32]" << endl;
          exit(0);
        }
    }
	return 0;
}


/* Main function:  */
int main(int argc, char** argv) {

  char app_path[1000];


  cout << "Console output: " << fileno(stdout) << endl;
  cout << "MoldeoPlayer SDL version 1.0, libmoldeo version: " << (char*)moGetVersionStr() << endl;

  moConsole   Moldeo;
  /*Initialization default*/
  screen_width = 1024;
  screen_height = 256;
  render_width = screen_width;
  render_height = screen_height;

  if (SDL_Init( SDL_INIT_EVERYTHING )<0) {
    cout << "Couldn't init SDL" << endl;
    exit(1);
  }

  freopen( "CON", "wt", stdout );
  freopen( "CON", "wt", stderr );


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
        gamemode = "AUTOPLAY";
    }
  } else exit(1);

  unsigned int major, minor, micro, nano;

  gst_init(NULL,NULL);
  gst_version (&major, &minor, &micro, &nano);
  cout << "Gstreamer version " << major << "." << minor << "." << micro << "." << nano << endl;

  getcwd(app_path,1000);



  if (mwindow!=moText("")) {
      SDL_ShowCursor(SDL_ENABLE);
      if ( SDL_SetVideoMode( screen_width, screen_height, 32, SDL_OPENGL|SDL_DOUBLEBUF|SDL_RESIZABLE ) == NULL) {
        cout << "Couldn't set SDL_SetVideoMode to window mode (resizable) " << screen_width << "x" << screen_height << " in 32 bits mode " << endl;
        exit(1);
      }
      //default mode
  } else if (gamemode!=moText("")) {
      SDL_ShowCursor(SDL_DISABLE);
      if ( SDL_SetVideoMode( screen_width, screen_height, 32, SDL_OPENGL|SDL_DOUBLEBUF|SDL_FULLSCREEN ) == NULL) {
        cout << "Couldn't set SDL_SetVideoMode to gamemode (fullscreen) " << screen_width << "x" << screen_height << " in 32 bits mode " << endl;
        exit(1);
      }
  } else {
      if ( SDL_SetVideoMode( screen_width, screen_height, 32, SDL_OPENGL|SDL_DOUBLEBUF|SDL_RESIZABLE ) == NULL) {
        cout << "Couldn't set SDL_SetVideoMode to window mode (resizable) " << screen_width << "x" << screen_height << " in 32 bits mode " << endl;
        exit(1);
      }
  }

  windowtitle = molproject + " - " + IntToStr(screen_width)+" x "+IntToStr(screen_height);
  SDL_WM_SetCaption( windowtitle, NULL );

  pIODeviceManager = new moSDL_IODeviceManager();
  pIODeviceManager->Init(NULL);

  bool res = Moldeo.Init( app_path,
                          molfolder, molproject,
                          pIODeviceManager /*IODeviceManager*/, NULL/*ResourceManager*/,
                          RENDERMANAGER_MODE_NORMAL /*render mode*/,
                          screen_width, screen_height, render_width, render_height
                         );

  if (!res) {
    cout << "error couldnt init console" << endl;
    exit(1);
  }

  Moldeo.ConsolePlay();
  gpConsole = &Moldeo;

  while( Moldeo.Interaction()
        && pIODeviceManager
        && !pIODeviceManager->m_CloseNeeded ) {

      if ( pIODeviceManager->m_ResizeNeeded ) {
          pIODeviceManager->m_ResizeNeeded = false;
          if (Moldeo.GetResourceManager()) {
            //SDL_GetWindowSize( displayWindow, &screen_width, &screen_height);
            if (pIODeviceManager->reEvent.w
                &&
                pIODeviceManager->reEvent.h) {
              screen_width = pIODeviceManager->reEvent.w;
              screen_height = pIODeviceManager->reEvent.h;
            }
            render_width = screen_width;
            render_height = screen_height;
            Moldeo.GetResourceManager()->GetRenderMan()->Finish();
            Moldeo.GetResourceManager()->GetRenderMan()->Init( RENDERMANAGER_MODE_NORMAL,
                                                              screen_width, screen_height,
                                                              render_width, render_height );
          }
      }


      moEvent* event = pIODeviceManager->GetEvents()->First;
      moMessage* newMessage = NULL;
      while(event!=NULL) {
        if ( event->deviceid == MO_IODEVICE_KEYBOARD || event->deviceid==999 ) {
          if (event->devicecode == SDL_KEYDOWN || event->devicecode==0) {
            if ( event->reservedvalue0 == SDLK_F12 || event->reservedvalue3 == MO_MESSAGE ) {
              //SwitchPresentation( Moldeo );
              //SDL_SetVideoMode( screen_width, screen_height, SDL_FULLSCREEN|SDL_OPENGL);
              //SDL_SetVideoMode( screen_width, screen_height, 32, SDL_OPENGL|SDL_DOUBLEBUF|SDL_NOFRAME );
              //pIODeviceManager->m_ResizeNeeded = true;
              //pIODeviceManager->reEvent.w = screen_width;
              //pIODeviceManager->reEvent.h = screen_height;
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

      Moldeo.Update();
      Moldeo.Draw();
      //glFlush();
      SDL_GL_SwapBuffers();
  }

  Moldeo.Finish();

  return 0;
}
