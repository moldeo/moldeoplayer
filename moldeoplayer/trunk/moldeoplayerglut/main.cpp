/*
 *
 */
 #define MOLDEOTEST


#ifdef MOLDEOTEST
#include "moldeotest.h"
#else
#include "moldeo.h"
moConsole* gpConsole = NULL;
#endif


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include <GL/glut.h>  // GLUT, includes glu.h and gl.h


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
  glViewport(0,0,width,height);
  #ifndef MOLDEOTEST
    moResourceManager* RESOURCES = gpConsole->GetResourceManager();
    moRenderManager* RENDER = RESOURCES->GetRenderMan();
    if (RENDER) RENDER->SetView( width, height );
  #endif
}

void MoldeoDisplay() {
  #ifndef MOLDEOTEST
  if (gpConsole) {
    gpConsole->Interaction();
    gpConsole->Update();
    gpConsole->Draw();
    glutSwapBuffers();
  }
  #else
    display();
  #endif
}



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

  char app_path[1000];

  #ifndef MOLDEOTEST
  moConsole   Moldeo;
  #endif
  int render_width, render_height ,screen_width, screen_height;

  /*Initialization*/

  render_width = 640;
  render_height = 480;
  screen_width = 640;
  screen_height = 480;
  getcwd(app_path,1000);
  glutInit(&argc, argv);                 // Initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);

  glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title

  glutInitWindowPosition(50, 50); // Position the window's initial top-left corner

#ifndef MOLDEOTEST
  Moldeo.Init( app_path, moText("ChannelControl2"), moText("ChannelControl2/channelcontrol2.mol"),
                              NULL /*IODeviceManager*/, NULL/*ResourceManager*/,
                              RENDERMANAGER_MODE_NORMAL /*render mode*/,
                              screen_width, screen_height, render_width, render_height
                           );
#endif
  glutInitWindowSize(screen_width, screen_height);   // Set the window's initial width & height
  glutReshapeFunc( ResizeWindow );

#ifndef MOLDEOTEST
  /*PLAY*/
  Moldeo.ConsolePlay();
  gpConsole = &Moldeo;
#endif
  /*DISPLAY*/
  //glutDisplayFunc(MoldeoDisplay); // Register display callback handler for window re-paint
  glutIdleFunc(MoldeoDisplay);
  glutMainLoop();           // Enter the infinitely event-processing loop

  return 0;
}
