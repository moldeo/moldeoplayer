/* A simple program to show how to set up an X window for OpenGL rendering.
 * X86 compilation: gcc -o -L/usr/X11/lib   main main.c -lGL -lX11
 * X64 compilation: gcc -o -L/usr/X11/lib64 main main.c -lGL -lX11
 */


#include <moldeo.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/XKBlib.h>
#include <X11/keysym.h>

#ifdef MO_WIN32
  #include "SDL.h"
#else
  #include "SDL/SDL.h"
#endif

#include <X11/extensions/Xinerama.h>
#ifdef HAVE_X11_EXTENSIONS_XF86VMODE_H
  #include <X11/extensions/xf86vmode.h>
#endif

#include "moX11_IODeviceManager.h"

moConsole* gpConsole = NULL;

static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display   *dpy;

Window     win;
Window     win2;

int twoscreens = true;
int swapped = false;

GLfloat    xAngle = 42.0, yAngle = 82.0, zAngle = 112.0;
GLboolean  doubleBuffer = GL_TRUE;

void fatalError(char *message)
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
  XEvent			xev;
  GC          gc;
  moConsole   MoldeoConsole;

  XineramaScreenInfo *m_screens;

  moResourceManager* pResourceMan = NULL;
  moRenderManager* pRenderMan = NULL;
  moGLManager* pGLMan = NULL;

  int render_width, render_height;
  int screen_width, screen_height;
  int interface_width, interface_height;

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

  twoscreens = true;

  // 640x400 : 800x500
  render_width = 512;
  render_height = 512;
  //16 : 10
  screen_width = 640;
  screen_height = 480;

  //interface_width = 144;
  interface_width = 144*5;
  interface_height = 90*5;

  /*** (1) open a connection to the X server ***/


  dpy = XOpenDisplay(NULL);
  if (dpy == NULL)
    fatalError("could not open display");

  m_screens = XineramaQueryScreens( dpy, &screen_numbers );
  cout << "screen numbers" << screen_numbers << endl;

  if (screen_numbers>1) {
    screen_output = screen_numbers-1;
  }
  for( int s=0; s<screen_numbers; s++ ) {
    XineramaScreenInfo &sinfo = m_screens[ s ];
    cout << "screen :" << s << " x_org:" << sinfo.x_org << " y_org:" << sinfo.y_org << " width:" << sinfo.width << " height:" << sinfo.height << endl;
  }

  XineramaScreenInfo &screen_info = m_screens[ screen_output ];
  cout << "Using screen :" << screen_output << " x_org:" << screen_info.x_org << " y_org:" << screen_info.y_org << " width:" << screen_info.width << " height:" << screen_info.height << endl;


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
                            DefaultScreen(dpy)/*screen number*/,
                            attrib_list,
                            &nelements );
  if (fbc==NULL) {
    fatalError("no FBConfig visual with depth buffer");
    exit(1);
  }
  vi = glXGetVisualFromFBConfig( dpy, fbc[0] );
} else vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);

  if (vi == NULL)
  {
    vi = glXChooseVisual(dpy, DefaultScreen(dpy), snglBuf);
    if (vi == NULL) fatalError("no RGB visual with depth buffer");
    doubleBuffer = GL_FALSE;
    exit(1);
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
    if (cx == NULL)
      fatalError("could not create rendering context");

    cx2 = glXCreateContext(dpy, vi, /* moldeo shared dlists */ cx,
                          /* direct rendering if possible */ GL_TRUE);

    if (cx2 == NULL)
      fatalError("could not create rendering context 2");

  }


  /*** (5) create an X window with the selected visual ***/

  /* create an X colormap since probably not using default visual */

  cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
  swa.colormap = cmap;
  swa.border_pixel = 0;
  swa.event_mask = KeyPressMask;

  screen_width = screen_info.width;
  screen_height = screen_info.height;

  win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), screen_info.x_org, screen_info.y_org,
                      screen_width, screen_height, 0, vi->depth, InputOutput, vi->visual,
                      CWColormap | CWEventMask, &swa);

  XSetStandardProperties(dpy, win, "main", "main", None,
                         argv, argc, NULL);


  /*** (6) bind the rendering context to the window ***/

  glXMakeCurrent(dpy, win, cx);


  /*** (7) request the X window to be displayed on the screen ***/

  XMapWindow(dpy, win);

  XColor red_col;
  char red[] = "#FF0000";

  XParseColor(dpy, cmap, red, &red_col);
  XAllocColor(dpy, cmap, &red_col);

  if (twoscreens) {
    win2 = XCreateWindow(dpy, RootWindow(dpy, vi->screen),
                         0, 0,
                        interface_width,
                        interface_height, 0,
                        vi->depth,
                        InputOutput,
                        vi->visual,
                        CWBorderPixel | CWColormap | CWEventMask, &swa);

    XSetStandardProperties(dpy, win2, "interface", "interface", None,
                           argv, argc, NULL);

    XMapWindow(dpy, win2);
    //gc = XCreateGC( dpy, win2, 0, 0 );
    //if (!gc)  fatalError("could not create graphic context on win2");
  }

/***********************************************/
/**   INITIALIZING MOLDEO CONSOLE */
/***********************************************/
   char app_path[1000];

    getcwd(app_path,1000);

   moX11_IODeviceManager* pIODeviceManager = new moX11_IODeviceManager();

   pIODeviceManager->Init( dpy, win, win2 );

   if ( 1==1 &&

        MoldeoConsole.Init( app_path,
/*
                            moText("SimpleProject"),
                            moText("SimpleProject/simple_project.mol"),
*/
                            moText("ChannelControl2"),
                            moText("ChannelControl2/channelcontrol2.mol"),

                            (moIODeviceManager*)pIODeviceManager /*IODeviceManager*/,
                              NULL/*ResourceManager*/,
                              RENDERMANAGER_MODE_NORMAL /*render mode*/,
                              screen_width, screen_height,
                              render_width,render_height
                           )
       ) {
      moDebugManager::Message("moldeoplayer::main > MoldeoConsole Initialized successfully ! ");

      /** getting rendering gl contexts and displays */

      pResourceMan = MoldeoConsole.GetResourceManager();
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
      }

      MoldeoConsole.ConsolePlay();
      gpConsole = &MoldeoConsole;

    } else {
      moDebugManager::Error("moldeoplayer::main > MoldeoConsole Initialization with errors. ");
      //exit(1);
    }
/***********************************************/
/***********************************************/
/*
    // Inicializamos en primer lugar
    if ( MoldeoConsole.Init( moText("test"),
                            moText("test.mol") ) ) {

        // Ciclo principal
        // Mientras interactuamos
        while( MoldeoConsole.Interaction() ) {

            //Actualizamos
            MoldeoConsole.Update();

            //Dibujamos
            MoldeoConsole.Draw();
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
  while (1)
  {
      /** RENDER OUTPUT */

      glXMakeCurrent(dpy, win, cx);
      MoldeoConsole.Interaction();
      MoldeoConsole.Update();
      MoldeoConsole.Draw();

      glXSwapBuffers(dpy, win);

      /** RENDER INTERFACE */
      if (twoscreens) {

          glXMakeCurrent(dpy, win2, cx );
          glViewport( 0, 0, pRenderMan->InterfaceWidth(), pRenderMan->InterfaceHeight());
          glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

          pRenderMan->DrawTexture( pRenderMan->InterfaceWidth(), pRenderMan->InterfaceHeight(), MO_FINAL_TEX );
          MoldeoConsole.DrawMasterEffects(0,0);
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

  XFree(m_screens);

  return 0;
}
