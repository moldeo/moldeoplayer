
#ifndef _MO_GLUT_IODEVICEMANAGER_H__
#define _MO_GLUT_IODEVICEMANAGER_H__


#include <SDL/SDL.h>
#include "moIODeviceManager.h"

#define SDL_NLK_CAPS 0x01
#define SDL_NLK_NUM  0x02

class moGLUT_IODeviceManager : public moIODeviceManager {

  public:

    moGLUT_IODeviceManager() : moIODeviceManager() {
/*
      m_Dpy = NULL;
      m_Win = NULL;
      m_Win2 = NULL;*/
      m_CachedEvents.Init();
    }

    virtual ~moGLUT_IODeviceManager() {
    }

    virtual bool Init( void* dpy /*Display *dpy=NULL, Window win=0, Window win2=0, int p_DISABLE_LOCK_KEYS=0*/ ) {
/*
      m_Dpy = dpy;
      m_Win = win;
*/
      m_MouseX = 0;
      m_MouseY = 0;

      moIODeviceManager::Init();

      return (m_bInitialized=true);
    }

    static SDLKey  GLUT_TranslateSpecialKey( unsigned int key ) {

    }

    static bool GLUT_CheckModState( int state, SDL_keysym &keysym ) {

      if (state=SDL_KEYDOWN) {

        keysym.mod = KMOD_NONE;

        int mod = glutGetModifiers();

        if (mod & GLUT_ACTIVE_ALT) {
          cout << "ALT ACTIVE" << endl;
          keysym.mod =  (SDLMod) ( (int)keysym.mod & (int)KMOD_ALT );
        }
        if (mod & GLUT_ACTIVE_CTRL) {
          cout << "CTRL ACTIVE" << endl;
          keysym.mod|= (SDLMod)KMOD_CTRL;
        }
        if (mod & GLUT_ACTIVE_SHIFT) {
          cout << "SHIFT ACTIVE" << endl;
          keysym.mod|= (SDLMod)KMOD_SHIFT;
        }
      }
/*
      int repeatable;
      Uint16 modstate;

        // Set up the keysym
        modstate = (Uint16)SDL_ModState;

        repeatable = 0;

        if ( state == SDL_KEYDOWN ) {
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
              // Only send keydown events
              return(0);
            case SDLK_CAPSLOCK:
              if ( SDL_NoLockKeys & SDL_NLK_CAPS )
                break;
              // Only send keydown events
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
*/
        return true;

    }


    static void Key( unsigned char key, int xmouse, int ymouse ) {

      m_DataLock.Lock();

      //convert glut key to SDL... key
/*

/// KEYBOARD
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
                (xev.type == KeyPress ) ? GetEvents()->Add(MO_IODEVICE_KEYBOARD,SDL_KEYDOWN, sdl_keysym.sym,
                                                          ///mod
                                                          sdl_keysym.mod,0,0) :
                                          GetEvents()->Add(MO_IODEVICE_KEYBOARD,SDL_KEYUP, sdl_keysym.sym,
                                                          ///mod
                                                          sdl_keysym.mod,0,0);
                break;
*/
      SDL_keysym sdl_keysym;
      sdl_keysym.mod = KMOD_NONE;
      sdl_keysym.unicode = 0;
      sdl_keysym.scancode = 0;
      sdl_keysym.sym = SDLK_UNKNOWN;


      sdl_keysym.sym = (SDLKey)key;

      GLUT_CheckModState( SDL_KEYDOWN, sdl_keysym );

      //cout << "moGLUT_IODeviceManager::Key( unsigned char key, int xmouse, int ymouse )" << endl;

      m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYDOWN, key, sdl_keysym.mod );

      //cout << "cached event: key: " << key << " int: " << (int)key << " sdlmod:" << sdl_keysym.mod << endl;

      switch (key)
      {
          case  32:
              glutFullScreen();
              break;
          case 27 :
              glutLeaveGameMode();
              exit(0);
              break;
          default:
              break;
      }

      //glutPostRedisplay();

      m_DataLock.Unlock();
    }

    static void KeyUp( unsigned char key, int xmouse, int ymouse ) {

          SDLKey sym = SDLK_UNKNOWN;
          SDLMod mod = KMOD_NONE;
          SDL_keysym sdl_keysym;


          m_DataLock.Lock();

          //cout << "moGLUT_IODeviceManager::KeyUp( unsigned char key, int xmouse, int ymouse )" << endl;
          //cout << "cached event: key: " << key << " int: " << (int)key << endl;
          m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYUP, key, mod );

          m_DataLock.Unlock();
    }

    static void SpecialFunc( int key, int xmouse, int ymouse ) {

      m_DataLock.Lock();


      int mod = glutGetModifiers();

      //cout << "moGLUT_IODeviceManager::SpecialFunc( unsigned char key, int xmouse, int ymouse )" << endl;

      if (mod & GLUT_ACTIVE_ALT) cout << "ALT ACTIVE" << endl;
      if (mod & GLUT_ACTIVE_CTRL) cout << "CTRL ACTIVE" << endl;
      if (mod & GLUT_ACTIVE_SHIFT) cout << "SHIFT ACTIVE" << endl;

      m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, key, mod, xmouse, ymouse );

      //cout << "cached event: key: " << key << " int: " << (int)key << endl;

      m_DataLock.Unlock();

    }

    static void Mouse( int button, int state, int x, int y ) {
        m_DataLock.Lock();
      /*
/// MOUSE
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
      */
      switch(button) {
        case GLUT_LEFT_BUTTON:
          button = SDL_BUTTON_LEFT;
          break;
        case GLUT_MIDDLE_BUTTON:
          button = SDL_BUTTON_MIDDLE;
          break;
        case GLUT_RIGHT_BUTTON:
          button = SDL_BUTTON_RIGHT;
          break;
      }

      if (state==GLUT_DOWN) {
        m_CachedEvents.Add( MO_IODEVICE_MOUSE, SDL_MOUSEBUTTONDOWN, button, x, y );
        //cout << "Pressing mouse button " << button << endl;
      }
      if (state==GLUT_UP) {
        m_CachedEvents.Add( MO_IODEVICE_MOUSE, SDL_MOUSEBUTTONUP, button, x, y );
        //cout << "Releasing mouse button " << button << endl;
      }

      m_MouseX = x;
      m_MouseY = y;

      m_DataLock.Unlock();
    }

    static void Motion( int x, int y ) {

/*
/// MOUSE
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
*/

      m_DataLock.Lock();

      //m_CachedEvents.Add( MO_IODEVICE_MOUSE, SDL_MOUSEMOTION, x-m_MouseX, y-m_MouseY );
      m_CachedEvents.Add( MO_IODEVICE_MOUSE, SDL_MOUSEMOTION, x-m_MouseX, y-m_MouseY, x, y );
      //cout << "Motion mouse x:" << x << " y:" << y << endl;

      m_MouseX = x;
      m_MouseY = y;

      m_DataLock.Unlock();
    }

    static void PassiveMotion( int x, int y ) {

      m_DataLock.Lock();

      //m_CachedEvents.Add( MO_IODEVICE_MOUSE, SDL_MOUSEMOTION, x-m_MouseX, y-m_MouseY );
      m_CachedEvents.Add( MO_IODEVICE_MOUSE, SDL_MOUSEMOTION, x-m_MouseX, y-m_MouseY, x, y );
      //cout << "Motion mouse x:" << x << " y:" << y << endl;

      m_MouseX = x;
      m_MouseY = y;

      m_DataLock.Unlock();
    }

    static void Wheel( int wheel, int direction, int x, int y ) {
      //
      //m_CachedEvents.Add( );
      //glutMouseFunc
    }


    virtual void PollEvents() {

        m_DataLock.Lock();

        moEvent* tmp;
        moEvent* actual;

        actual = m_CachedEvents.First;

        //cout << "PollEvents: m_CachedEvents: First: " << m_CachedEvents.First << " Last:" << m_CachedEvents.Last << " actual:" << actual << endl;


        while(actual!=NULL) {
          //cout << "PollEvents: Adding new event " << actual << endl;
          moEventList* pEvents = GetEvents();
          if (pEvents) {
            //cout << "PollEvents: GetEvents(): First: " << GetEvents()->First << " Last:" << GetEvents()->Last << endl;
            //pEvents->Add( (moEvent*)actual );
            pEvents->Add( actual->deviceid,
                          actual->devicecode,
                          actual->reservedvalue0,
                          actual->reservedvalue1,
                          actual->reservedvalue2,
                          actual->reservedvalue3,
                          actual->pointer );
            //cout << "PollEvents: GetEvents(): First: " << GetEvents()->First << " Last:" << GetEvents()->Last << endl;
            //cout << "PollEvents: Added OK!!!" << endl;
          }

          tmp = actual;
          actual = tmp->next;
        }
/*
        cout << "PollEvents: Events added last: " << actual << endl;
        cout << "Resetting m_CachedEvents " << endl;
        m_CachedEvents.Finish();
        cout << "Reinit m_CachedEvents " << endl;
        m_CachedEvents.Init();
*/
        m_CachedEvents.Finish();

        //cout << "PollEvents: DataLock Unlock" << endl;

        m_DataLock.Unlock();



/* X11 implementation:*/
/*

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


              // RESIZE EVENTS
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

              /// MOUSE
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

              /// KEYBOARD
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
                (xev.type == KeyPress ) ? GetEvents()->Add(MO_IODEVICE_KEYBOARD,SDL_KEYDOWN, sdl_keysym.sym,
                                                          ///mod
                                                          sdl_keysym.mod,0,0) :
                                          GetEvents()->Add(MO_IODEVICE_KEYBOARD,SDL_KEYUP, sdl_keysym.sym,
                                                          ///mod
                                                          sdl_keysym.mod,0,0);
                break;

            }



        }


        ++pending;
      }
*/

    }
/*
    Display   *m_Dpy;
    Window     m_Win;
    Window     m_Win2;


    SDLKey ODD_keymap[256];
    SDLKey MISC_keymap[256];

    SDLMod    SDL_ModState;
    Uint8 SDL_NoLockKeys;
*/

    static moLock     m_DataLock;
    static moEventList m_CachedEvents;
    static int   m_MouseX;
    static int   m_MouseY;

    int   m_SDL_DISABLE_LOCK_KEYS;
};

moEventList moGLUT_IODeviceManager::m_CachedEvents;
moLock moGLUT_IODeviceManager::m_DataLock;
int   moGLUT_IODeviceManager::m_MouseX;
int   moGLUT_IODeviceManager::m_MouseY;



#endif
