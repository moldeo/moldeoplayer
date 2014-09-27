
#ifndef _MO_X11_IODEVICEMANAGER_H__
#define _MO_X11_IODEVICEMANAGER_H__

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


#endif
