/*
 * GL01Hello.cpp: Test OpenGL C/C++ Setup
 */
#include "moldeo.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

moConsole* gpConsole = NULL;


//#include <GL/glut.h>  // GLUT, includes glu.h and gl.h


/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

   // Draw a Red 1x1 Square centered at origin
   glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
      glColor3f(1.0f, 0.0f, 0.0f); // Red
      glVertex2f(-0.5f, -0.5f);    // x, y
      glVertex2f( 0.5f, -0.5f);
      glVertex2f( 0.5f,  0.5f);
      glVertex2f(-0.5f,  0.5f);
   glEnd();

   glFlush();  // Render now
}



void MoldeoDisplay() {
  if (gpConsole) {
    gpConsole->Interaction();
    gpConsole->Update();
    gpConsole->Draw();
  }
}



/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {

  char app_path[1000];
  moConsole   Moldeo;
  int render_width, render_height ,screen_width, screen_height;

  /*Initialization*/

  render_width = 640;
  render_height = 480;
  screen_width = 640;
  screen_height = 480;
  getcwd(app_path,1000);
  glutInit(&argc, argv);                 // Initialize GLUT
  glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
  glutInitWindowSize(screen_width, screen_height);   // Set the window's initial width & height
  glutInitWindowPosition(50, 50); // Position the window's initial top-left corner

  Moldeo.Init( app_path, moText("ChannelControl2"), moText("ChannelControl2/channelcontrol2.mol"),
                              NULL /*IODeviceManager*/, NULL/*ResourceManager*/,
                              RENDERMANAGER_MODE_NORMAL /*render mode*/,
                              screen_width, screen_height, render_width, render_height
                           );

  /*PLAY*/
  Moldeo.ConsolePlay();
  gpConsole = &Moldeo;

  /*DISPLAY*/
  glutDisplayFunc(MoldeoDisplay); // Register display callback handler for window re-paint
  glutMainLoop();           // Enter the infinitely event-processing loop

  return 0;
}
