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

#define SDL_NLK_CAPS 0x01
#define SDL_NLK_NUM  0x02

class moX11_IODeviceManager : public moIODeviceManager {

  public:

    moX11_IODeviceManager() : moIODeviceManager() {

      m_Dpy = NULL;
      m_Win = NULL;
      m_Win2 = NULL;
    }

    virtual ~moX11_IODeviceManager() {
    }

    virtual bool Init( Display *dpy=NULL, Window win=0, Window win2=0, int p_DISABLE_LOCK_KEYS=0 ) {

      m_Dpy = dpy;
      m_Win = win;
      m_Win2 = win2;
      SDL_ModState = KMOD_NONE;

      if (m_Dpy && m_Win) XSelectInput(m_Dpy, m_Win,  KeyPressMask |
                                                      KeyReleaseMask |
                                                      ButtonPressMask |
                                                      ButtonReleaseMask |
                                                      PointerMotionMask |
                                                      ResizeRedirectMask |
                                                      StructureNotifyMask |
                                                      ExposureMask
                                                      );

      if (m_Dpy && m_Win2) XSelectInput(m_Dpy, m_Win2, KeyPressMask |
                                                      KeyReleaseMask |
                                                      ButtonPressMask |
                                                      ButtonReleaseMask |
                                                      PointerMotionMask |
                                                      ResizeRedirectMask|
                                                      StructureNotifyMask |
                                                      ExposureMask);

      X11_InitKeymap();

      SDL_NoLockKeys = 0;
      m_SDL_DISABLE_LOCK_KEYS = p_DISABLE_LOCK_KEYS;


      switch (m_SDL_DISABLE_LOCK_KEYS) {
        case 1:
          SDL_NoLockKeys = SDL_NLK_CAPS | SDL_NLK_NUM;
          break;
        case 2:
          SDL_NoLockKeys = SDL_NLK_CAPS;
          break;
        case 3:
          SDL_NoLockKeys = SDL_NLK_NUM;
          break;
        default:
          break;
      }

      m_MouseX = 0;
      m_MouseY = 0;


      return (m_bInitialized=true);
    }

    void X11_InitKeymap(void) {
      int i;

      /* Odd keys used in international keyboards */
      for ( i=0; i<SDL_arraysize(ODD_keymap); ++i )
        ODD_keymap[i] = SDLK_UNKNOWN;

      /* Some of these might be mappable to an existing SDLK_ code */
      ODD_keymap[XK_dead_grave&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_acute&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_tilde&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_macron&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_breve&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_abovedot&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_diaeresis&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_abovering&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_doubleacute&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_caron&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_cedilla&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_ogonek&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_iota&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_voiced_sound&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_semivoiced_sound&0xFF] = SDLK_COMPOSE;
      ODD_keymap[XK_dead_belowdot&0xFF] = SDLK_COMPOSE;
    #ifdef XK_dead_hook
      ODD_keymap[XK_dead_hook&0xFF] = SDLK_COMPOSE;
    #endif
    #ifdef XK_dead_horn
      ODD_keymap[XK_dead_horn&0xFF] = SDLK_COMPOSE;
    #endif

    #ifdef XK_dead_circumflex
      /* These X keysyms have 0xFE as the high byte */
      ODD_keymap[XK_dead_circumflex&0xFF] = SDLK_CARET;
    #endif
    #ifdef XK_ISO_Level3_Shift
      ODD_keymap[XK_ISO_Level3_Shift&0xFF] = SDLK_MODE; /* "Alt Gr" key */
    #endif

      /* Map the miscellaneous keys */
      for ( i=0; i<SDL_arraysize(MISC_keymap); ++i )
        MISC_keymap[i] = SDLK_UNKNOWN;

      /* These X keysyms have 0xFF as the high byte */
      MISC_keymap[XK_BackSpace&0xFF] = SDLK_BACKSPACE;
      MISC_keymap[XK_Tab&0xFF] = SDLK_TAB;
      MISC_keymap[XK_Clear&0xFF] = SDLK_CLEAR;
      MISC_keymap[XK_Return&0xFF] = SDLK_RETURN;
      MISC_keymap[XK_Pause&0xFF] = SDLK_PAUSE;
      MISC_keymap[XK_Escape&0xFF] = SDLK_ESCAPE;
      MISC_keymap[XK_Delete&0xFF] = SDLK_DELETE;

      MISC_keymap[XK_KP_0&0xFF] = SDLK_KP0;		/* Keypad 0-9 */
      MISC_keymap[XK_KP_1&0xFF] = SDLK_KP1;
      MISC_keymap[XK_KP_2&0xFF] = SDLK_KP2;
      MISC_keymap[XK_KP_3&0xFF] = SDLK_KP3;
      MISC_keymap[XK_KP_4&0xFF] = SDLK_KP4;
      MISC_keymap[XK_KP_5&0xFF] = SDLK_KP5;
      MISC_keymap[XK_KP_6&0xFF] = SDLK_KP6;
      MISC_keymap[XK_KP_7&0xFF] = SDLK_KP7;
      MISC_keymap[XK_KP_8&0xFF] = SDLK_KP8;
      MISC_keymap[XK_KP_9&0xFF] = SDLK_KP9;
      MISC_keymap[XK_KP_Insert&0xFF] = SDLK_KP0;
      MISC_keymap[XK_KP_End&0xFF] = SDLK_KP1;
      MISC_keymap[XK_KP_Down&0xFF] = SDLK_KP2;
      MISC_keymap[XK_KP_Page_Down&0xFF] = SDLK_KP3;
      MISC_keymap[XK_KP_Left&0xFF] = SDLK_KP4;
      MISC_keymap[XK_KP_Begin&0xFF] = SDLK_KP5;
      MISC_keymap[XK_KP_Right&0xFF] = SDLK_KP6;
      MISC_keymap[XK_KP_Home&0xFF] = SDLK_KP7;
      MISC_keymap[XK_KP_Up&0xFF] = SDLK_KP8;
      MISC_keymap[XK_KP_Page_Up&0xFF] = SDLK_KP9;
      MISC_keymap[XK_KP_Delete&0xFF] = SDLK_KP_PERIOD;
      MISC_keymap[XK_KP_Decimal&0xFF] = SDLK_KP_PERIOD;
      MISC_keymap[XK_KP_Divide&0xFF] = SDLK_KP_DIVIDE;
      MISC_keymap[XK_KP_Multiply&0xFF] = SDLK_KP_MULTIPLY;
      MISC_keymap[XK_KP_Subtract&0xFF] = SDLK_KP_MINUS;
      MISC_keymap[XK_KP_Add&0xFF] = SDLK_KP_PLUS;
      MISC_keymap[XK_KP_Enter&0xFF] = SDLK_KP_ENTER;
      MISC_keymap[XK_KP_Equal&0xFF] = SDLK_KP_EQUALS;

      MISC_keymap[XK_Up&0xFF] = SDLK_UP;
      MISC_keymap[XK_Down&0xFF] = SDLK_DOWN;
      MISC_keymap[XK_Right&0xFF] = SDLK_RIGHT;
      MISC_keymap[XK_Left&0xFF] = SDLK_LEFT;
      MISC_keymap[XK_Insert&0xFF] = SDLK_INSERT;
      MISC_keymap[XK_Home&0xFF] = SDLK_HOME;
      MISC_keymap[XK_End&0xFF] = SDLK_END;
      MISC_keymap[XK_Page_Up&0xFF] = SDLK_PAGEUP;
      MISC_keymap[XK_Page_Down&0xFF] = SDLK_PAGEDOWN;

      MISC_keymap[XK_F1&0xFF] = SDLK_F1;
      MISC_keymap[XK_F2&0xFF] = SDLK_F2;
      MISC_keymap[XK_F3&0xFF] = SDLK_F3;
      MISC_keymap[XK_F4&0xFF] = SDLK_F4;
      MISC_keymap[XK_F5&0xFF] = SDLK_F5;
      MISC_keymap[XK_F6&0xFF] = SDLK_F6;
      MISC_keymap[XK_F7&0xFF] = SDLK_F7;
      MISC_keymap[XK_F8&0xFF] = SDLK_F8;
      MISC_keymap[XK_F9&0xFF] = SDLK_F9;
      MISC_keymap[XK_F10&0xFF] = SDLK_F10;
      MISC_keymap[XK_F11&0xFF] = SDLK_F11;
      MISC_keymap[XK_F12&0xFF] = SDLK_F12;
      MISC_keymap[XK_F13&0xFF] = SDLK_F13;
      MISC_keymap[XK_F14&0xFF] = SDLK_F14;
      MISC_keymap[XK_F15&0xFF] = SDLK_F15;

      MISC_keymap[XK_Num_Lock&0xFF] = SDLK_NUMLOCK;
      MISC_keymap[XK_Caps_Lock&0xFF] = SDLK_CAPSLOCK;
      MISC_keymap[XK_Scroll_Lock&0xFF] = SDLK_SCROLLOCK;
      MISC_keymap[XK_Shift_R&0xFF] = SDLK_RSHIFT;
      MISC_keymap[XK_Shift_L&0xFF] = SDLK_LSHIFT;
      MISC_keymap[XK_Control_R&0xFF] = SDLK_RCTRL;
      MISC_keymap[XK_Control_L&0xFF] = SDLK_LCTRL;
      MISC_keymap[XK_Alt_R&0xFF] = SDLK_RALT;
      MISC_keymap[XK_Alt_L&0xFF] = SDLK_LALT;
      MISC_keymap[XK_Meta_R&0xFF] = SDLK_RMETA;
      MISC_keymap[XK_Meta_L&0xFF] = SDLK_LMETA;
      MISC_keymap[XK_Super_L&0xFF] = SDLK_LSUPER; /* Left "Windows" */
      MISC_keymap[XK_Super_R&0xFF] = SDLK_RSUPER; /* Right "Windows */
      MISC_keymap[XK_Mode_switch&0xFF] = SDLK_MODE; /* "Alt Gr" key */
      MISC_keymap[XK_Multi_key&0xFF] = SDLK_COMPOSE; /* Multi-key compose */

      MISC_keymap[XK_Help&0xFF] = SDLK_HELP;
      MISC_keymap[XK_Print&0xFF] = SDLK_PRINT;
      MISC_keymap[XK_Sys_Req&0xFF] = SDLK_SYSREQ;
      MISC_keymap[XK_Break&0xFF] = SDLK_BREAK;
      MISC_keymap[XK_Menu&0xFF] = SDLK_MENU;
      MISC_keymap[XK_Hyper_R&0xFF] = SDLK_MENU;   /* Windows "Menu" key */
    }

    /* Get the translated SDL virtual keysym */
    SDLKey X11_TranslateKeycode(Display *display, KeyCode kc) {
      KeySym xsym;
      SDLKey key;

      xsym = XKeycodeToKeysym(display, kc, 0);
    #ifdef DEBUG_KEYS
      fprintf(stderr, "Translating key code %d -> 0x%.4x\n", kc, xsym);
    #endif
      key = SDLK_UNKNOWN;
      if ( xsym ) {
        switch (xsym>>8) {
            case 0x1005FF:
    #ifdef SunXK_F36
          if ( xsym == SunXK_F36 )
            key = SDLK_F11;
    #endif
    #ifdef SunXK_F37
          if ( xsym == SunXK_F37 )
            key = SDLK_F12;
    #endif
          break;
            case 0x00:	/* Latin 1 */
          key = (SDLKey)(xsym & 0xFF);
          break;
            case 0x01:	/* Latin 2 */
            case 0x02:	/* Latin 3 */
            case 0x03:	/* Latin 4 */
            case 0x04:	/* Katakana */
            case 0x05:	/* Arabic */
            case 0x06:	/* Cyrillic */
            case 0x07:	/* Greek */
            case 0x08:	/* Technical */
            case 0x0A:	/* Publishing */
            case 0x0C:	/* Hebrew */
            case 0x0D:	/* Thai */
          /* These are wrong, but it's better than nothing */
          key = (SDLKey)(xsym & 0xFF);
          break;
            case 0xFE:
          key = ODD_keymap[xsym&0xFF];
          break;
            case 0xFF:
          key = MISC_keymap[xsym&0xFF];
          break;
            default:
          /*
          fprintf(stderr, "X11: Unhandled xsym, sym = 0x%04x\n",
              (unsigned int)xsym);
          */
          break;
        }
      } else {
        /* X11 doesn't know how to translate the key! */
        switch (kc) {
            /* Caution:
               These keycodes are from the Microsoft Keyboard
             */
            case 115:
          key = SDLK_LSUPER;
          break;
            case 116:
          key = SDLK_RSUPER;
          break;
            case 117:
          key = SDLK_MENU;
          break;
            default:
          /*
           * no point in an error message; happens for
           * several keys when we get a keymap notify
           */
          break;
        }
      }
      return key;
    }

    int X11_Pending(Display *display) {
      /* Flush the display connection and look to see if events are queued */
      XFlush(display);
      if ( XEventsQueued(display, QueuedAlready) ) {
        return(1);
      }

      /* More drastic measures are required -- see if X is ready to talk */
      {
        static struct timeval zero_time;	/* static == 0 */
        int x11_fd;
        fd_set fdset;

        x11_fd = ConnectionNumber(display);
        FD_ZERO(&fdset);
        FD_SET(x11_fd, &fdset);
        if ( select(x11_fd+1, &fdset, NULL, NULL, &zero_time) == 1 ) {
          return(XPending(display));
        }
      }

      /* Oh well, nothing is ready .. */
      return(0);
    }

    static int X11_KeyRepeat(Display *display, XEvent *event) {
      XEvent peekevent;
      int repeated;

      repeated = 0;
      if ( XPending(display) ) {
        XPeekEvent(display, &peekevent);
        if ( (peekevent.type == KeyPress) &&
             (peekevent.xkey.keycode == event->xkey.keycode) &&
             ((peekevent.xkey.time - event->xkey.time) < 2) ) {
          repeated = 1;
          XNextEvent(display, &peekevent);
        }
      }
      return(repeated);
    }

    bool X11_CheckModState( int state /*XEvent.type*/, SDL_keysym *keysym ) {

      int repeatable;
      Uint16 modstate;

        /* Set up the keysym */
        modstate = (Uint16)SDL_ModState;

        repeatable = 0;

        if ( state == KeyPress ) {
          keysym->mod = (SDLMod)modstate;
          switch (keysym->sym) {
            case SDLK_UNKNOWN:
              break;
            case SDLK_NUMLOCK:
              modstate ^= KMOD_NUM;
              if ( SDL_NoLockKeys & SDL_NLK_NUM )
                break;
              if ( ! (modstate&KMOD_NUM) )
                state = SDL_RELEASED;
              keysym->mod = (SDLMod)modstate;
              break;
            case SDLK_CAPSLOCK:
              modstate ^= KMOD_CAPS;
              if ( SDL_NoLockKeys & SDL_NLK_CAPS )
                break;
              if ( ! (modstate&KMOD_CAPS) )
                state = SDL_RELEASED;
              keysym->mod = (SDLMod)modstate;
              break;
            case SDLK_LCTRL:
              modstate |= KMOD_LCTRL;
              break;
            case SDLK_RCTRL:
              modstate |= KMOD_RCTRL;
              break;
            case SDLK_LSHIFT:
              modstate |= KMOD_LSHIFT;
              break;
            case SDLK_RSHIFT:
              modstate |= KMOD_RSHIFT;
              break;
            case SDLK_LALT:
              modstate |= KMOD_LALT;
              break;
            case SDLK_RALT:
              modstate |= KMOD_RALT;
              break;
            case SDLK_LMETA:
              modstate |= KMOD_LMETA;
              break;
            case SDLK_RMETA:
              modstate |= KMOD_RMETA;
              break;
            case SDLK_MODE:
              modstate |= KMOD_MODE;
              break;
            default:
              repeatable = 1;
              break;
          }
        } else {
          switch (keysym->sym) {
            case SDLK_UNKNOWN:
              break;
            case SDLK_NUMLOCK:
              if ( SDL_NoLockKeys & SDL_NLK_NUM )
                break;
              /* Only send keydown events */
              return(0);
            case SDLK_CAPSLOCK:
              if ( SDL_NoLockKeys & SDL_NLK_CAPS )
                break;
              /* Only send keydown events */
              return(0);
            case SDLK_LCTRL:
              modstate &= ~KMOD_LCTRL;
              break;
            case SDLK_RCTRL:
              modstate &= ~KMOD_RCTRL;
              break;
            case SDLK_LSHIFT:
              modstate &= ~KMOD_LSHIFT;
              break;
            case SDLK_RSHIFT:
              modstate &= ~KMOD_RSHIFT;
              break;
            case SDLK_LALT:
              modstate &= ~KMOD_LALT;
              break;
            case SDLK_RALT:
              modstate &= ~KMOD_RALT;
              break;
            case SDLK_LMETA:
              modstate &= ~KMOD_LMETA;
              break;
            case SDLK_RMETA:
              modstate &= ~KMOD_RMETA;
              break;
            case SDLK_MODE:
              modstate &= ~KMOD_MODE;
              break;
            default:
              break;
          }
          keysym->mod = (SDLMod)modstate;
        }

        SDL_ModState = (SDLMod)modstate;

        return true;

    }


    virtual void PollEvents() {

      XEvent			xev;
      char buffer[20];
      int bufsize = 20;
      KeySym xkeysym;
      XComposeStatus compose;
      int charcount;

      SDLKey sym = SDLK_UNKNOWN;
      SDLMod mod = KMOD_NONE;
      SDL_keysym sdl_keysym;

      //XK_Num_Lock

      int pending;
      char	*key_string1;


      pending = 0;
      while ( X11_Pending(m_Dpy) ) {
        //call XNextEvent
        //X11_DispatchEvent(this);
        XNextEvent(m_Dpy, &xev);

        moDebugManager::Message( "X Event :" + IntToStr( xev.type ) );

        if ( xev.type == KeyRelease
             && X11_KeyRepeat( m_Dpy, &xev) ) {

        } else {

            sdl_keysym.mod = KMOD_NONE;
            sdl_keysym.unicode = 0;

            sdl_keysym.scancode = 0;
            sdl_keysym.sym = SDLK_UNKNOWN;

            switch( xev.type ) {


              case ConfigureNotify:
                if (xev.xconfigure.display==m_Dpy) {

                  if (xev.xconfigure.window==m_Win && gpConsole) {
                    gpConsole->GetResourceManager()->GetRenderMan()->SetView( xev.xconfigure.width, xev.xconfigure.height );
                    moDebugManager::Message( "X Event ConfigureNotify m_Win width:" + IntToStr(xev.xconfigure.width) + " height:" + IntToStr(xev.xconfigure.height) );
                  }

                  if (xev.xconfigure.window==m_Win2 && gpConsole) {
                    gpConsole->GetResourceManager()->GetRenderMan()->SetInterfaceView( xev.xconfigure.width, xev.xconfigure.height );
                    moDebugManager::Message( "X Event ConfigureNotify m_Win2 width:" + IntToStr(xev.xconfigure.width) + " height:" + IntToStr(xev.xconfigure.height) );
                  }

                }
                break;


              /** RESIZE EVENTS*/
              case ResizeRequest:
                if (xev.xresizerequest.display==m_Dpy) {

                  if (xev.xresizerequest.window==m_Win && gpConsole) {
                    gpConsole->GetResourceManager()->GetRenderMan()->SetView( xev.xresizerequest.width, xev.xresizerequest.height );
                    moDebugManager::Message( "X Event ResizeRequest m_Win width:" + IntToStr(xev.xresizerequest.width) + " height:" + IntToStr(xev.xresizerequest.height) );
                  }

                  if (xev.xresizerequest.window==m_Win2 && gpConsole) {
                    gpConsole->GetResourceManager()->GetRenderMan()->SetInterfaceView( xev.xresizerequest.width, xev.xresizerequest.height );
                    moDebugManager::Message( "X Event ResizeRequest m_Win2 width:" + IntToStr(xev.xresizerequest.width) + " height:" + IntToStr(xev.xresizerequest.height) );
                  }

                }
                break;

              /** MOUSE */
              case MotionNotify:
                GetEvents()->Add( MO_IODEVICE_MOUSE, SDL_MOUSEMOTION,  xev.xmotion.x - m_MouseX, xev.xmotion.y - m_MouseY );
                m_MouseX = xev.xmotion.x;
                m_MouseY = xev.xmotion.y;
                moDebugManager::Message( "X Event MotionNotify X: " + IntToStr( m_MouseX ) + " Y: " + IntToStr( m_MouseY )  );
                break;
              case ButtonPress:
                GetEvents()->Add( MO_IODEVICE_MOUSE, SDL_MOUSEBUTTONDOWN,  xev.xbutton.button, xev.xbutton.x, xev.xbutton.y );
                m_MouseX = xev.xbutton.x;
                m_MouseY = xev.xbutton.y;
                moDebugManager::Message( "ButtonPress button: " + IntToStr( xev.xbutton.button ) + " X: " + IntToStr( m_MouseX ) + " Y: " + IntToStr( m_MouseY )  );
                break;
              case ButtonRelease:
                GetEvents()->Add( MO_IODEVICE_MOUSE, SDL_MOUSEBUTTONUP,  xev.xbutton.button, xev.xbutton.x, xev.xbutton.y );
                m_MouseX = xev.xbutton.x;
                m_MouseY = xev.xbutton.y;
                moDebugManager::Message( "ButtonRelease button: " + IntToStr( xev.xbutton.button ) + " X: " + IntToStr( m_MouseX ) + " Y: " + IntToStr( m_MouseY )  );
                break;

              /** KEYBOARD */
              case KeyPress:
              case KeyRelease:

                sdl_keysym.scancode = xev.xkey.keycode;
                sdl_keysym.sym = X11_TranslateKeycode( m_Dpy, xev.xkey.keycode );

                //keysim = XkbKeycodeToKeysym( m_Dpy, xev.xkey.keycode, 0, 0)
                charcount = XLookupString(&xev.xkey, buffer, bufsize, &xkeysym, &compose);
                key_string1 = XKeysymToString(XkbKeycodeToKeysym( m_Dpy, xev.xkey.keycode, 0, 0));
                moDebugManager::Message( "Key (2:press/3:release):" + IntToStr(xev.type) + " keysim STR: " + moText(key_string1) + +" Xkeysim code:" + IntToStr(xkeysym) + " keycode: " + IntToStr(xev.xkey.keycode) + " sdl sym:" + IntToStr(sdl_keysym.sym) );

                X11_CheckModState( xev.type, &sdl_keysym );

                moDebugManager::Message( " Mod:" + IntToStr(sdl_keysym.mod) );

                //Events->Add(MO_IODEVICE_KEYBOARD,SDL_KEYDOWN, event.key.keysym.sym, event.key.keysym.mod,0,0);
                (xev.type == KeyPress ) ? GetEvents()->Add(MO_IODEVICE_KEYBOARD,SDL_KEYDOWN, sdl_keysym.sym, /*mod*/sdl_keysym.mod,0,0) : GetEvents()->Add(MO_IODEVICE_KEYBOARD,SDL_KEYUP, sdl_keysym.sym, /*mod*/sdl_keysym.mod,0,0);
                break;

            }



        }


        ++pending;
      }
  /*
      while(XCheckTypedWindowEvent( m_Dpy, m_Win, KeyRelease, &xev)) {
      //while(XCheckWindowEvent( m_Dpy, m_Win, 0xFF, &xev)) {

        char	*key_string1 = XKeysymToString(XkbKeycodeToKeysym(m_Dpy, xev.xkey.keycode, 0, 0));

        moDebugManager::Message( "Win1: keysim: " + moText(key_string1) + " keycode: " + IntToStr(xev.xkey.keycode) );
      }
  */

    }

    Display   *m_Dpy;
    Window     m_Win;
    Window     m_Win2;

    SDLKey ODD_keymap[256];
    SDLKey MISC_keymap[256];

    SDLMod    SDL_ModState;
    Uint8 SDL_NoLockKeys;
    int   m_SDL_DISABLE_LOCK_KEYS;

    int   m_MouseX;
    int   m_MouseY;
};

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
