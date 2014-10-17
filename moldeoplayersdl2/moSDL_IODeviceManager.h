
#ifndef _MO_SDL_IODEVICEMANAGER_H__
#define _MO_SDL_IODEVICEMANAGER_H__


#ifdef MO_WIN32
    #include "SDL.h"
#else
    #include "SDL.h"
#endif

class moSDL_IODeviceManager : public moIODeviceManager {

  public:

    moSDL_IODeviceManager() : moIODeviceManager() {
      m_ResizeNeeded = false;
      m_CloseNeeded = false;
      m_displayWindowId = -1;
      m_previewWindowId = -1;
      m_ResizeNeededForPreview = false;
      m_CloseNeededForPreview = false;
    }

    virtual ~moSDL_IODeviceManager() {
    }



    virtual bool Init( void* dpy /*Display *dpy=NULL, Window win=0, Window win2=0, int p_DISABLE_LOCK_KEYS=0*/ ) {
/*
      m_Dpy = dpy;
      m_Win = win;
*/
      moIODeviceManager::Init();
      return (m_bInitialized=true);
    }

    virtual void PollEvents() {

      SDL_Event event;
   		moEventList				*Events = GetEvents();

       //SDL_KEYDOWN,			// Keys pressed
         //SDL_KEYUP,				// Keys released
         //SDL_MOUSEMOTION,		// Mouse moved
         //SDL_MOUSEBUTTONDOWN,	// Mouse button pressed
         //SDL_MOUSEBUTTONUP,		// Mouse button released

      while(SDL_PollEvent(&event)) {
        switch(event.type) {

          case SDL_WINDOWEVENT:
                switch (event.window.event) {

                  case SDL_WINDOWEVENT_RESIZED:
                    if (event.window.windowID==m_displayWindowId)
                      m_ResizeNeeded = true;
                    if (event.window.windowID==m_previewWindowId)
                      m_ResizeNeededForPreview = true;
                    break;
                  case SDL_WINDOWEVENT_CLOSE:
                    if (event.window.windowID==m_displayWindowId)
                      m_CloseNeeded = true;
                    if (event.window.windowID==m_previewWindowId)
                      m_CloseNeededForPreview = true;
                    break;
                  default:
                    break;
                }
            break;




            case SDL_MOUSEMOTION:
              Events->Add(MO_IODEVICE_MOUSE,SDL_MOUSEMOTION, event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y);
              break;
            case SDL_MOUSEBUTTONDOWN:
              Events->Add(MO_IODEVICE_MOUSE,SDL_MOUSEBUTTONDOWN, event.button.button, event.button.x, event.button.y);
              break;
            case SDL_MOUSEBUTTONUP:
              Events->Add(MO_IODEVICE_MOUSE,SDL_MOUSEBUTTONUP, event.button.button, event.button.x, event.button.y);
              break;
            case SDL_KEYDOWN:
              // Ignore ALT-TAB for windows
              if((event.key.keysym.sym == SDLK_LALT) ||
                (event.key.keysym.sym == SDLK_RALT) ||
                (event.key.keysym.sym == SDLK_TAB)) {
                break;
              }
              MODebug2->Message("SDL_KEYDOWN event.key.keysym.sym: "+IntToStr(event.key.keysym.sym)
                                + " event.key.keysym.mod:" + IntToStr(event.key.keysym.mod) );
              Events->Add(MO_IODEVICE_KEYBOARD,SDL_KEYDOWN, event.key.keysym.sym, event.key.keysym.mod,0,0);
              break;

            case SDL_KEYUP:
              if((event.key.keysym.sym == SDLK_LALT) ||
                (event.key.keysym.sym == SDLK_RALT) ||
                (event.key.keysym.sym == SDLK_TAB)) {
                break;
              }
              MODebug2->Message("SDL_KEYUP event.key.keysym.sym: "+IntToStr(event.key.keysym.sym)
                                + " event.key.keysym.mod:" + IntToStr(event.key.keysym.mod) );
              Events->Add(MO_IODEVICE_KEYBOARD,SDL_KEYUP, event.key.keysym.sym, event.key.keysym.mod,0,0);
              break;
            default:
              break;
        }
      }

    }

    void SetWindows( SDL_Window* displayWin, SDL_Window* previewWin ) {

        if (displayWin)
          m_displayWindowId = SDL_GetWindowID(displayWin);

        if (previewWin)
          m_previewWindowId = SDL_GetWindowID(previewWin);

    }

    static int   m_MouseX;
    static int   m_MouseY;

    MOuint m_displayWindowId;
    MOuint m_previewWindowId;

    bool m_ResizeNeeded;
    bool m_CloseNeeded;

    bool m_ResizeNeededForPreview;
    bool m_CloseNeededForPreview;
};

#endif
