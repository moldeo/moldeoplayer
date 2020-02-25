
#ifndef _MO_SDL_IODEVICEMANAGER_H__
#define _MO_SDL_IODEVICEMANAGER_H__


#ifdef MO_WIN32
    #include "SDL.h"
#else
    #include "SDL.h"
    #include "SDL2/SDL_syswm.h"
    #include "X11/X.h"
/**
XEvent.type
=================================
#define KeyPress                2
#define KeyRelease              3
#define ButtonPress             4
#define ButtonRelease           5
#define MotionNotify            6
#define EnterNotify             7
#define LeaveNotify             8
#define FocusIn                 9
#define FocusOut                10
#define KeymapNotify            11
#define Expose                  12
#define GraphicsExpose          13
#define NoExpose                14
#define VisibilityNotify        15
#define CreateNotify            16
#define DestroyNotify           17
#define UnmapNotify             18
#define MapNotify               19
#define MapRequest              20
#define ReparentNotify          21
#define ConfigureNotify         22
#define ConfigureRequest        23
#define GravityNotify           24
#define ResizeRequest           25
#define CirculateNotify         26
#define CirculateRequest        27
#define PropertyNotify          28
#define SelectionClear          29
#define SelectionRequest        30
#define SelectionNotify         31
#define ColormapNotify          32
#define ClientMessage           33
#define MappingNotify           34
#define GenericEvent            35
*/
    #ifdef MO_LINUX
      #include <X11/extensions/XInput.h>
    #endif
#endif

class moSDL_IODeviceManager : public moIODeviceManager {

  public:
    long ei;

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
      ei = 0;
      SDL_EventState( SDL_SYSWMEVENT, SDL_ENABLE);
      MODebug2->Message( "Joystick events:"+moText(SDL_JoystickEventState(SDL_QUERY)) );
      //SDL_EventState( SDL_MOUSEMOTION, SDL_DISABLE);
      moIODeviceManager::Init();
      return (m_bInitialized=true);
    }

    virtual void PollEvents() {
      int nevents = ei;
      SDL_Event event;
   		moEventList				*Events = GetEvents();

       //SDL_KEYDOWN,			// Keys pressed
         //SDL_KEYUP,				// Keys released
         //SDL_MOUSEMOTION,		// Mouse moved
         //SDL_MOUSEBUTTONDOWN,	// Mouse button pressed
         //SDL_MOUSEBUTTONUP,		// Mouse button released
         //cout << "SDL PollEvent" << endl;

      while(SDL_PollEvent(&event)) {
        ei =ei + 1;
        switch(event.type) {
          case SDL_WINDOWEVENT:
              /*cout << "SDL_WINDOWEVENT PollEvent" << endl;*/
                switch (event.window.event) {

                  case SDL_WINDOWEVENT_RESIZED:
                  /*cout << "SDL_WINDOWEVENT_RESIZED PollEvent" << endl;
                  cout << "event.window.windowID: " << event.window.windowID << endl;
                  cout << "m_displayWindowId: " << m_displayWindowId << endl;
                  cout << "m_previewWindowId: " << m_previewWindowId << endl;*/
                    if (event.window.windowID==m_displayWindowId)
                      m_ResizeNeeded = true;
                    if (event.window.windowID==m_previewWindowId)
                      m_ResizeNeededForPreview = true;
                    break;
                  case SDL_WINDOWEVENT_CLOSE:
                  /*cout << "SDL_WINDOWEVENT_CLOSE PollEvent" << endl;*/
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
              //cout << ei << " Mouse motion: x:" << event.motion.x << " y:" << event.motion.y << endl;
              /*cout << ei << " Mouse motion" << endl;*/
              break;
            case SDL_MOUSEBUTTONDOWN:
              Events->Add(MO_IODEVICE_MOUSE,SDL_MOUSEBUTTONDOWN, event.button.button, event.button.x, event.button.y);
              //cout << ei << " Mouse button down" << endl;
              break;
            case SDL_MOUSEBUTTONUP:
              Events->Add(MO_IODEVICE_MOUSE,SDL_MOUSEBUTTONUP, event.button.button, event.button.x, event.button.y);
              //cout << ei << " Mouse button up" << endl;
              break;
            case SDL_KEYDOWN:
              // Ignore ALT-TAB for windows
              if((event.key.keysym.sym == SDLK_LALT) ||
                (event.key.keysym.sym == SDLK_RALT) ||
                (event.key.keysym.sym == SDLK_TAB)) {
                break;
              }
              //MODebug2->Message("SDL_KEYDOWN event.key.keysym.sym: "+IntToStr(event.key.keysym.sym)
              //                + " event.key.keysym.mod:" + IntToStr(event.key.keysym.mod) );
              Events->Add(MO_IODEVICE_KEYBOARD,SDL_KEYDOWN, event.key.keysym.sym, event.key.keysym.mod,0,0);
              break;

            case SDL_KEYUP:
              if((event.key.keysym.sym == SDLK_LALT) ||
                (event.key.keysym.sym == SDLK_RALT) ||
                (event.key.keysym.sym == SDLK_TAB)) {
                break;
              }
              //MODebug2->Message("SDL_KEYUP event.key.keysym.sym: "+IntToStr(event.key.keysym.sym)
              //                  + " event.key.keysym.mod:" + IntToStr(event.key.keysym.mod) );
              Events->Add(MO_IODEVICE_KEYBOARD,SDL_KEYUP, event.key.keysym.sym, event.key.keysym.mod,0,0);
              break;

            /* Game controller events */
            case SDL_CONTROLLERAXISMOTION:
            case SDL_CONTROLLERBUTTONDOWN:          /**< Game controller button pressed */
            case SDL_CONTROLLERBUTTONUP:            /**< Game controller button released */
            case SDL_CONTROLLERDEVICEADDED:         /**< A new Game controller has been inserted into the system */
            case SDL_CONTROLLERDEVICEREMOVED:       /**< An opened Game controller has been removed */
            case SDL_CONTROLLERDEVICEREMAPPED:      /**< The controller mapping was updated */
              /*cout << "Game controller" << endl;*/
              break;
            case SDL_JOYDEVICEADDED:
              {
              int njoys = SDL_NumJoysticks();
              moText jname = SDL_JoystickNameForIndex(0);
              /*cout << "Joystick added (" << njoys << ") :" << jname << endl;*/
                if (njoys) {
                  SDL_Joystick *joy = SDL_JoystickOpen(0);
                  if (joy) {
                    /*cout << "Num axes:" << SDL_JoystickNumAxes(joy)
                    << " Num buttons:" << SDL_JoystickNumButtons(joy)
                    << " Num balls:" << SDL_JoystickNumBalls(joy)
                    << endl;*/
                  }
                }
              }
              break;
            case SDL_JOYDEVICEREMOVED:
              /*cout << "Joystick removed: " << SDL_NumJoysticks() << endl;*/
              break;
            case SDL_JOYAXISMOTION:
            case SDL_JOYHATMOTION:
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
              cout << "Joystick event" << endl;
              break;
            case SDL_SYSWMEVENT:
              #ifndef MO_MACOSX
              XEvent xev;
              SDL_SysWMmsg* sysm;
              if (event.syswm.msg) {
                sysm = (SDL_SysWMmsg*) event.syswm.msg;
                if (sysm) {
                  xev = sysm->msg.x11.event;
                  if (xev.type==MotionNotify) {
                    XDeviceMotionEvent  *MotionEvent = (XDeviceMotionEvent*)(&xev);
                    if (MotionEvent->axis_data[2]>0) {
                      if (1==2) {
                    //cout << ei << " Pressure: " << (float)MotionEvent->axis_data[2] << endl;
                    //if (MotionEvent->y_root>0) {
                    //if (MotionEvent->device_state==256) {
                      cout << ei << " SYSWM XID:" << (int)MotionEvent->deviceid
                      << " Type:" << (int)MotionEvent->type
                      << " Serial:" << (int)MotionEvent->serial
                      << " sned:" << (int)MotionEvent->send_event
                      << " time:" << (int) MotionEvent->time
                      << " is_hint:" << (int)MotionEvent->is_hint
                      << " same_screen:" << (int)MotionEvent->same_screen
                      << " root:" << (int)MotionEvent->root
                      << " window:" << (int)MotionEvent->window
                      << " subwindow:" << (int)MotionEvent->subwindow
                      << " Motion: PosX: " << (float)MotionEvent->x << " PosY: " << (float)MotionEvent->y
                      << " Motion: PosRootX: " << (float)MotionEvent->x_root << " PosRootY: " << (float)MotionEvent->y_root
                      << " State: " << (int)MotionEvent->state
                      << " DevState: " << (int)MotionEvent->device_state
                      << " Pressure: " << (float)MotionEvent->axis_data[2]
                      << " first: " << (int)MotionEvent->first_axis
                      << " Axes: " << (int)MotionEvent->axes_count
                      << " 0: " <<  (float)MotionEvent->axis_data[0]
                      << " 1: " <<  (float)MotionEvent->axis_data[1]
                      << " 2: " <<  (float)MotionEvent->axis_data[2]
                      << endl;
                      }
                      Events->Add( MO_IODEVICE_TABLET, SDL_MOUSEMOTION, MotionEvent->x, MotionEvent->y,MotionEvent->axis_data[2],0);
                    }
                  } else {
                    //cout << ei << " SYSWM XID:" << endl;
                  }
                }
              }
            #endif
            {}
              break;
            default:
              break;
        }
      }
      if (ei>nevents) {
        //cout << endl;
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
