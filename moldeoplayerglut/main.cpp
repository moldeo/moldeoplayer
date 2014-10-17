/*
 *
 */
//#define MOLDEOTEST
//#define DONTUSEGLUT

#ifndef _MSC_VER
#include <unistd.h>
#else
#include "direct.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <iostream>

using namespace std;

void delay(int secs) {
    for(int i = (time(NULL) + secs); time(NULL) != i; time(NULL));
}

#ifdef MOLDEOTEST

#else
    #include "gst/gst.h"
    #include "moldeo.h"
    #include "moDebugManager.h"
    moConsole* gpConsole = NULL;
#endif


#ifndef DONTUSEGLUT


#ifdef MO_MACOSX
    #include <GLUT/glut.h>  // GLUT, includes glu.h and gl.h
#endif

#ifdef MO_LINUX
    #define FREEGLUT_STATIC
    #include <GL/freeglut.h>  // GLUT, includes glu.h and gl.h
#endif

#ifdef WIN32
    #include "GL/glut.h"  // GLUT, includes glu.h and gl.h
#endif

#ifndef MOLDEOTEST
    #include "moGLUT_IODeviceManager.h"
    moGLUT_IODeviceManager* pIODeviceManager = NULL;
#else

#endif

/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
    //glClear(GL_DEPTH_BUFFER);

   // Draw a Red 1x1 Square centered at origin
   glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(1.0f, 0.0f, 0.0f); // Red
      glVertex2f(-0.5f, -0.5f);    // x, y
      glVertex2f( 0.5f, -0.5f);
      glVertex2f( 0.5f,  0.5f);
      glVertex2f(-0.5f,  0.5f);
   glEnd();

   glFlush();  // Render now
   glutSwapBuffers();
}

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




int render_width, render_height ,screen_width, screen_height;

#ifndef MOLDEOTEST

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
#endif
#endif

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

  char app_path[1000];
/*
  FILE* fstd = freopen("CON", "w", stdout);

    fprintf(fstd, "Con\n");
    fflush(fstd);
*/
  //freopen("CON", "w", stdout);
//freopen("CON", "w", stdout);
  cout << "Console output: " << fileno(stdout) << endl;
  #ifndef MOLDEOTEST
  cout << "MoldeoPLayer GLUT version 1.0, libmoldeo version: " << (char*)moGetVersionStr() << endl;
  #endif

  #ifndef MOLDEOTEST
  moConsole   Moldeo;
  /*Initialization default*/
  screen_width = 1024;
  screen_height = 400;
  render_width = screen_width;
  render_height = screen_height;
  #else

  #endif

  moPreConfig preconf;
  moPreconfigParamIndex pconf;
  pconf.m_ParamName = "test";


  #ifndef MOLDEOTEST
  processarguments( argc, argv );
  #endif

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
#ifndef MOLDEOTEST
  unsigned int major, minor, micro, nano;

  gst_init(NULL,NULL);
  gst_version (&major, &minor, &micro, &nano);
  cout << "Gstreamer version " << major << "." << minor << "." << micro << "." << nano << endl;
#endif

  getcwd(app_path,1000);

#ifndef DONTUSEGLUT
  glutInit(&argc, argv);                 // Initialize GLUT

  glutInitWindowPosition(0,0); // Position the window's initial top-left corner
  glutInitWindowSize(screen_width, screen_height);   // Set the window's initial width & height

  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);

  windowtitle = molproject + " - " + IntToStr(screen_width)+" x "+IntToStr(screen_height);
  #ifndef MOLDEOTEST
  if (mwindow!=moText("")) {
      glutCreateWindow( windowtitle ); // Create a window with the given title
  } else if (gamemode!=moText("")) {
      glutGameModeString(gamemode);
      glutEnterGameMode();
  } else {
    cout << "creating default window" << endl;
      glutCreateWindow( windowtitle );
  }
  #else
  glutCreateWindow( "Moldeo test" );
  #endif

  glutDisplayFunc(MoldeoDisplay);

#ifndef MOLDEOTEST

  pIODeviceManager = new moGLUT_IODeviceManager();
  pIODeviceManager->Init(NULL);

  bool res = Moldeo.Init( app_path,
                          molfolder, molproject,
                          pIODeviceManager /*IODeviceManager*/, NULL/*ResourceManager*/,
                          RENDERMANAGER_MODE_NORMAL
                          /*RENDERMANAGER_MODE_FRAMEBUFFER*/ /*render mode*/,
                          screen_width, screen_height, render_width, render_height
                         );

  //moText tojson = Moldeo.ToJSON();
  //Moldeo.MODebug2->Message( tojson );

  if (!res) {
    cout << "error couldnt init console" << endl;
    exit(1);
  }

#endif

  glutReshapeFunc( ResizeWindow );

  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);


#ifndef MOLDEOTEST
  /*PLAY*/

  glutKeyboardUpFunc( moGLUT_IODeviceManager::KeyUp );
  glutKeyboardFunc( moGLUT_IODeviceManager::Key );
  glutSpecialFunc(  moGLUT_IODeviceManager::SpecialFunc);
  glutSpecialUpFunc( NULL );
  glutMouseFunc( moGLUT_IODeviceManager::Mouse );
  glutMotionFunc( moGLUT_IODeviceManager::Motion );
  glutPassiveMotionFunc( moGLUT_IODeviceManager::PassiveMotion );
  glutSetCursor( GLUT_CURSOR_NONE );
  /*glutMouseWheelFunc(  moGLUT_IODeviceManager::Wheel  );*/


    Moldeo.ConsolePlay();
  gpConsole = &Moldeo;
#endif

    //delay(2000);

  /*DISPLAY*/
   // Register display callback handler for window re-paint

  glutIdleFunc(MoldeoDisplay);
  glutMainLoop();           // Enter the infinitely event-processing loop

#ifndef MOLDEOTEST
  Moldeo.Finish();
#endif

#endif
  return 0;
}
