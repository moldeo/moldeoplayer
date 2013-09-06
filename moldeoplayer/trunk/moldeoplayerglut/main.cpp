/*
 *
 */
 //#define MOLDEOTEST
#include "gst/gst.h"

#ifdef MOLDEOTEST
//#include "moldeotest.h"
#else
#include "moldeo.h"
moConsole* gpConsole = NULL;
#endif


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include <GL/glut.h>  // GLUT, includes glu.h and gl.h

#define FREEGLUT_STATIC
#include <GL/freeglut.h>  // GLUT, includes glu.h and gl.h


/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   //glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   //glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
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

void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case  32:
            glutFullScreenToggle();
            break;
        case 27 :
            exit(0);
            break;
        default:
            break;
    }

    glutPostRedisplay();
}



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

  guint major, minor, micro, nano;
  char app_path[1000];

  #ifndef MOLDEOTEST
  moConsole   Moldeo;
  #endif
  int render_width, render_height ,screen_width, screen_height;

  /*Initialization*/

  render_width = 1280;
  render_height = 768;
  screen_width = 1280;
  screen_height = 768;
  getcwd(app_path,1000);

  gst_init(NULL,NULL);
  gst_version (&major, &minor, &micro, &nano);

  glutInit(&argc, argv);                 // Initialize GLUT

  glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
  glutInitWindowSize(screen_width, screen_height);   // Set the window's initial width & height

  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);


  glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
  glutDisplayFunc(MoldeoDisplay);

#ifndef MOLDEOTEST
  bool res = Moldeo.Init( app_path,
                              //moText("../../data/samples/ChannelControl2"), moText("../../data/samples/ChannelControl2/channelcontrol2.mol"),
                              //moText("../../data/samples/Rain_Particles"), moText("../../data/samples/Rain_Particles/Rain_Particles.mol"),
                              //moText("../../data/samples/MoviePlayer"), moText("../../data/samples/MoviePlayer/MoviePlayer.mol"),
                              //moText("../../data/samples/ParticlesBounce"), moText("../../data/samples/ParticlesBounce/particles_bounce.mol"),
                              //moText("ChannelControl2"), moText("ChannelControl2/channelcontrol2.mol"),
                              //moText("../../data/samples/Cameras"), moText("../../data/samples/Cameras/cameras.mol"),
                              moText("../../data/samples/Tuio"), moText("../../data/samples/Tuio/Tuio.mol"),
                              NULL /*IODeviceManager*/, NULL/*ResourceManager*/,
                              RENDERMANAGER_MODE_NORMAL /*render mode*/,
                              screen_width, screen_height, render_width, render_height
                           );
  if (!res) {
    cout << "error couldnt init console" << endl;
    exit(1);
  }

#endif

  glutReshapeFunc( ResizeWindow );
  glutKeyboardFunc(key);

#ifndef MOLDEOTEST
  /*PLAY*/
  Moldeo.ConsolePlay();
  gpConsole = &Moldeo;
#endif
  /*DISPLAY*/
   // Register display callback handler for window re-paint

  glutIdleFunc(MoldeoDisplay);
  glutMainLoop();           // Enter the infinitely event-processing loop

  Moldeo.Finish();
  return 0;
}
