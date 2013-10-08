/*
 *
 */
 //#define MOLDEOTEST
#include "gst/gst.h"

#include <time.h>
void delay(int secs) {
    for(int i = (time(NULL) + secs); time(NULL) != i; time(NULL));
}

#ifdef MOLDEOTEST
//#include "moldeotest.h"
#else
#include "moldeo.h"
moConsole* gpConsole = NULL;
#endif


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef MO_MACOSX
    #include <GLUT/glut.h>  // GLUT, includes glu.h and gl.h
#endif

#ifdef MO_LINUX
    #define FREEGLUT_STATIC
    #include <GL/freeglut.h>  // GLUT, includes glu.h and gl.h
#endif

#include "moGLUT_IODeviceManager.h"
moGLUT_IODeviceManager* pIODeviceManager = NULL;

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



moText config("");
moText molproject("");
moText molfolder("");
moText gamemode("");
moText mwindow("");
int render_width, render_height ,screen_width, screen_height;


int processarguments( int argc, char** argv ) {

    if (argc<2) {
			cout << "Missing arguments!!! usage: moldeoplayerglut [-mol ../../sample.mol ] [-window 640x480] [-gamemode 1024x768:32]" << endl;
			exit(0);
		}

    while( argc >= 2 ) {
		--argc;

        moText arglast(argv[argc]);
        moText argprev;

        if (argc>0) argprev = argv[argc-1];

        moText  arg0(argv[0]);

		cout <<  "Argument id" << (argc) << " : " << moText(arglast) << endl;

		if( argprev == moText("-mol") )  {

		    molproject = moText( arglast );
            moFile file( molproject );
            molfolder = file.GetPath();
		    cout << "Argument -mol found! : " << molproject << " folder:" << molfolder << endl;
			--argc;

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

}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

  guint major, minor, micro, nano;
  char app_path[1000];

  #ifndef MOLDEOTEST
  moConsole   Moldeo;
  #endif

  /*Initialization default*/
  screen_width = 640;
  screen_height = 480;
  render_width = screen_width;
  render_height = screen_height;

  processarguments( argc, argv );

  gst_init(NULL,NULL);

  getcwd(app_path,1000);

  gst_init(NULL,NULL);
  gst_version (&major, &minor, &micro, &nano);
  cout << "Gstreamer version " << major << "." << minor << "." << micro << "." << nano << endl;

  glutInit(&argc, argv);                 // Initialize GLUT

  glutInitWindowPosition(0,0); // Position the window's initial top-left corner
  glutInitWindowSize(screen_width, screen_height);   // Set the window's initial width & height

  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);

  if (mwindow!=moText("")) {
      glutCreateWindow( molproject ); // Create a window with the given title
  } else if (gamemode!=moText("")) {
      glutGameModeString(gamemode);
      glutEnterGameMode();
  } else {
      glutCreateWindow( molproject );
  }

  glutDisplayFunc(MoldeoDisplay);

#ifndef MOLDEOTEST

  pIODeviceManager = new moGLUT_IODeviceManager();

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

#endif

  glutReshapeFunc( ResizeWindow );

  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

  glutKeyboardUpFunc( moGLUT_IODeviceManager::KeyUp );
  glutKeyboardFunc( moGLUT_IODeviceManager::Key );
  glutSpecialFunc(  moGLUT_IODeviceManager::SpecialFunc);
  glutMouseFunc( moGLUT_IODeviceManager::Mouse );
  glutMotionFunc( moGLUT_IODeviceManager::Motion );
  glutPassiveMotionFunc( moGLUT_IODeviceManager::PassiveMotion );
  glutMouseWheelFunc(  moGLUT_IODeviceManager::Wheel  );

#ifndef MOLDEOTEST
  /*PLAY*/
  Moldeo.ConsolePlay();
  gpConsole = &Moldeo;
#endif

    //delay(2000);

  /*DISPLAY*/
   // Register display callback handler for window re-paint

  glutIdleFunc(MoldeoDisplay);
  glutMainLoop();           // Enter the infinitely event-processing loop

  Moldeo.Finish();
  return 0;
}
