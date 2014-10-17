
#ifndef _MO_GLUT_IODEVICEMANAGER_H__
#define _MO_GLUT_IODEVICEMANAGER_H__


#ifdef MO_WIN32
    #include "SDL_keysym.h"
#else
    #include "SDL/SDL_keysym.h"
#endif


enum { SDL_NOEVENT = 0,			/* Unused (do not remove) */
       SDL_ACTIVEEVENT,			/* Application loses/gains visibility */
       SDL_KEYDOWN,			/* Keys pressed */
       SDL_KEYUP,			/* Keys released */
       SDL_MOUSEMOTION,			/* Mouse moved */
       SDL_MOUSEBUTTONDOWN,		/* Mouse button pressed */
       SDL_MOUSEBUTTONUP,		/* Mouse button released */
       SDL_JOYAXISMOTION,		/* Joystick axis motion */
       SDL_JOYBALLMOTION,		/* Joystick trackball motion */
       SDL_JOYHATMOTION,		/* Joystick hat position change */
       SDL_JOYBUTTONDOWN,		/* Joystick button pressed */
       SDL_JOYBUTTONUP,			/* Joystick button released */
       SDL_QUIT,			/* User-requested quit */
       SDL_SYSWMEVENT,			/* System specific event */
       SDL_EVENT_RESERVEDA,		/* Reserved for future use.. */
       SDL_EVENT_RESERVEDB,		/* Reserved for future use.. */
       SDL_VIDEORESIZE,			/* User resized video mode */
       SDL_VIDEOEXPOSE,			/* Screen needs to be redrawn */
       SDL_EVENT_RESERVED2,		/* Reserved for future use.. */
       SDL_EVENT_RESERVED3,		/* Reserved for future use.. */
       SDL_EVENT_RESERVED4,		/* Reserved for future use.. */
       SDL_EVENT_RESERVED5,		/* Reserved for future use.. */
       SDL_EVENT_RESERVED6,		/* Reserved for future use.. */
       SDL_EVENT_RESERVED7,		/* Reserved for future use.. */
       /* Events SDL_USEREVENT through SDL_MAXEVENTS-1 are for your use */
       SDL_USEREVENT = 24,
       /* This last event is only for bounding internal arrays
	  It is the number of bits in the event mask datatype -- Uint32
        */
       SDL_NUMEVENTS = 32
};
#define SDL_BUTTON(X)		(SDL_PRESSED << ((X)-1))
#define SDL_BUTTON_LEFT		1
#define SDL_BUTTON_MIDDLE	2
#define SDL_BUTTON_RIGHT	3
#define SDL_BUTTON_WHEELUP	4
#define SDL_BUTTON_WHEELDOWN	5
#define SDL_BUTTON_LMASK	SDL_BUTTON(SDL_BUTTON_LEFT)
#define SDL_BUTTON_MMASK	SDL_BUTTON(SDL_BUTTON_MIDDLE)
#define SDL_BUTTON_RMASK	SDL_BUTTON(SDL_BUTTON_RIGHT)

typedef struct SDL_keysym {
	int scancode;			/* hardware specific scancode */
	SDLKey sym;			/* SDL virtual keysym */
	SDLMod mod;			/* current key modifiers */
	int unicode;			/* translated character */
} SDL_keysym;

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


/*
    static SDLKey  GLUT_TranslateSpecialKey( unsigned int key ) {

    }
*/
    static bool GLUT_CheckModState( int state, SDL_keysym &keysym ) {

       keysym.mod = KMOD_NONE;
       int mod = glutGetModifiers();

      if (state==SDL_KEYDOWN) {

        if (mod & GLUT_ACTIVE_ALT) {
          cout << "ALT ACTIVE ON" << endl;
          keysym.mod =  (SDLMod) ( (int)keysym.mod & (int)KMOD_ALT );
          int key = SDLK_LSHIFT;
           m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYDOWN, key, KMOD_SHIFT, 0, 0 );
        } else {
          cout << "ALT ACTIVE STOP" << endl;
          int key = SDLK_LSHIFT;
           m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYUP, key, KMOD_SHIFT, 0, 0 );
        }
/*
        if (mod & GLUT_ACTIVE_CTRL) {
          cout << "CTRL ACTIVE ON" << endl;
          keysym.mod = (SDLMod)((int)keysym.mod | (int)KMOD_CTRL);
          int key = SDLK_LCTRL;
          m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYDOWN, key, KMOD_CTRL, 0, 0 );
        } else {
          cout << "CTRL ACTIVE STOP" << endl;
          int key = SDLK_LCTRL;
          m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYUP, key, KMOD_CTRL, 0, 0 );
        }
        if (mod & GLUT_ACTIVE_SHIFT) {
          cout << "SHIFT ACTIVE DOWN" << endl;
          keysym.mod = (SDLMod)((int)keysym.mod | (int)KMOD_SHIFT);
          int key = SDLK_LSHIFT;
          m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYDOWN, key, KMOD_SHIFT, 0, 0 );
        } else {
            cout << "SHIFT ACTIVE UP!?" << endl;
            int key = SDLK_LSHIFT;
            m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYUP, key, KMOD_SHIFT, 0, 0 );
        }
*/
      } else if (state==SDL_KEYUP) {

        if (mod & GLUT_ACTIVE_ALT) {
          cout << "ALT ACTIVE ON" << endl;
          keysym.mod =  (SDLMod) ( (int)keysym.mod & (int)KMOD_ALT );
          int key = SDLK_LSHIFT;
           m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYDOWN, key, KMOD_SHIFT, 0, 0 );
        } else {
          cout << "ALT ACTIVE STOP" << endl;
          int key = SDLK_LSHIFT;
           m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYUP, key, KMOD_SHIFT, 0, 0 );
        }

/*
        if (mod & GLUT_ACTIVE_CTRL) {
          cout << "CTRL ACTIVE ON" << endl;
          keysym.mod = (SDLMod)((int)keysym.mod | (int)KMOD_CTRL);
          int key = SDLK_LCTRL;
          m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYDOWN, key, KMOD_CTRL, 0, 0 );
        } else {
          cout << "CTRL ACTIVE STOP" << endl;
          int key = SDLK_LCTRL;
          m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYUP, key, KMOD_CTRL, 0, 0 );
        }
        if (mod & GLUT_ACTIVE_SHIFT) {
          cout << "SHIFT ACTIVE DOWN 2" << endl;
          keysym.mod = (SDLMod)((int)keysym.mod | (int)KMOD_SHIFT);
          int key = SDLK_LSHIFT;
          m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYDOWN, key, KMOD_SHIFT, 0, 0 );
        } else {
            cout << "SHIFT ACTIVE UP 2!?" << endl;
            int key = SDLK_LSHIFT;
            m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYUP, key, KMOD_SHIFT, 0, 0 );
        }
*/
      }

        return true;

    }


    static void Key( unsigned char key, int xmouse, int ymouse ) {

      m_DataLock.Lock();

      //convert glut key to SDL... key

      SDL_keysym sdl_keysym;
      sdl_keysym.mod = KMOD_NONE;
      sdl_keysym.unicode = 0;
      sdl_keysym.scancode = 0;
      sdl_keysym.sym = SDLK_UNKNOWN;


      sdl_keysym.sym = (SDLKey)key;

      GLUT_CheckModState( SDL_KEYDOWN, sdl_keysym );

      //cout << "moGLUT_IODeviceManager::Key( unsigned char key, int xmouse, int ymouse )" << endl;

      m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYDOWN, key, sdl_keysym.mod );

      cout << "moGLUT_IODeviceManager::Key(...) cached event: key: " << key << " int: " << (int)key << " sdlmod:" << sdl_keysym.mod << endl;

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

        //SDLKey sym = SDLK_UNKNOWN;
        SDLMod mod = KMOD_NONE;
        //SDL_keysym sdl_keysym;


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

      if (mod & GLUT_ACTIVE_ALT)    cout << "ALT ACTIVE: missing up event: " << key << endl;
      if (mod & GLUT_ACTIVE_CTRL)   cout << "CTRL ACTIVE: missing up event: " << key << endl;
      if (mod & GLUT_ACTIVE_SHIFT)  cout << "SHIFT ACTIVE: missing up event: " << key <<  endl;

      //m_CachedEvents.Add( MO_IODEVICE_KEYBOARD, SDL_KEYDOWN, key, mod, 0, 0 );

      //cout << "cached event: key: " << key << " int: " << (int)key << endl;

      m_DataLock.Unlock();

    }

    static void Mouse( int button, int state, int x, int y ) {
        m_DataLock.Lock();

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

    }

    static moLock     m_DataLock;
    static int   m_MouseX;
    static int   m_MouseY;
    static moEventList m_CachedEvents;
};

    moEventList moGLUT_IODeviceManager::m_CachedEvents;
    moLock moGLUT_IODeviceManager::m_DataLock;
    int   moGLUT_IODeviceManager::m_MouseX = 0;
    int   moGLUT_IODeviceManager::m_MouseY = 0;

#endif
