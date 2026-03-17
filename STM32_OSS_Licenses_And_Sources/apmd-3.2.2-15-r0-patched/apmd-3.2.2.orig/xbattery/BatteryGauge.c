/*  Copyright (C) 1999 Nathan Sidwell <nathan@acm.org>
 *  $Id: BatteryGauge.c,v 1.1 1999/08/02 20:08:10 apenwarr Exp $
 *  This file is available under the GNU General Public License.
 */

/*{{{  includes*/
#include <X11/X.h>
#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Xmu/Drawing.h>
#include <X11/Shell.h>
#include <X11/Xaw/Label.h>
#include "BatteryGaugeP.h"
#include <apm.h>
/*}}}*/
/*{{{  resources*/
static XtResource resources[] =
{
  {XtNorientation, XtCOrientation, XtROrientation, sizeof(XtOrientation),
      XtOffsetOf(BatteryGaugeRec, batteryGauge.orientation),
      XtRImmediate, (XtPointer)XtorientVertical},
  {XtNlength, XtCLength, XtRDimension, sizeof(Dimension),
       XtOffsetOf(BatteryGaugeRec, batteryGauge.length),
       XtRImmediate, (XtPointer)0},
  {XtNthickness, XtCThickness, XtRDimension, sizeof(Dimension),
       XtOffsetOf(BatteryGaugeRec, batteryGauge.thickness),
       XtRImmediate, (XtPointer)0},
  {XtNflash, XtCDelay, XtRInt, sizeof(int),
      XtOffsetOf(BatteryGaugeRec, batteryGauge.flashDelay),
      XtRImmediate, (XtPointer)250},
  {XtNupdate, XtCDelay, XtRInt, sizeof(int),
      XtOffsetOf(BatteryGaugeRec, batteryGauge.updateDelay),
      XtRImmediate, (XtPointer)15},
  {XtNlowColor, XtCForeground, XtRPixel, sizeof(Pixel),
      XtOffsetOf(BatteryGaugeRec, batteryGauge.lowColor),
      XtRString, (XtPointer)"orange"},
  {XtNhighColor, XtCForeground, XtRPixel, sizeof(Pixel),
      XtOffsetOf(BatteryGaugeRec, batteryGauge.highColor),
      XtRString, (XtPointer)"green"},
  {XtNcriticalColor, XtCForeground, XtRPixel, sizeof(Pixel),
      XtOffsetOf(BatteryGaugeRec, batteryGauge.criticalColor),
      XtRString, (XtPointer)"red"},
  {XtNchargingColor, XtCBackground, XtRPixel, sizeof(Pixel),
      XtOffsetOf(BatteryGaugeRec, batteryGauge.chargingColor),
      XtRString, (XtPointer)"blue"},
  {XtNcriticalLevel, XtCValue, XtRInt, sizeof(int),
      XtOffsetOf(BatteryGaugeRec, batteryGauge.criticalLevel),
      XtRImmediate, (XtPointer)0},
  {XtNlevel, XtCValue, XtRInt, sizeof(int),
      XtOffsetOf(BatteryGaugeRec, batteryGauge.level),
      XtRImmediate, (XtPointer)0},
  {XtNstatus, XtCValue, XtRInt, sizeof(int),
      XtOffsetOf(BatteryGaugeRec, batteryGauge.status),
      XtRImmediate, (XtPointer)-1},
  {XtNlifetime, XtCValue, XtRInt, sizeof(int),
      XtOffsetOf(BatteryGaugeRec, batteryGauge.lifetime),
      XtRImmediate, (XtPointer)0},
};
/*}}}*/
/*{{{  translations*/
static char translations[] ="\
<BtnDown>: Notify()\n\
<BtnUp>: Release()\n\
";
/*}}}*/
/*{{{  prototypes*/
/*{{{  methods*/
static void ClassInitialize(void);
static void Destroy(Widget);
static void Initialize(Widget, Widget, ArgList, Cardinal *);
static void Realize(Widget, XtValueMask *, XSetWindowAttributes *);
static void Redisplay(Widget, XEvent *, Region);
static void Resize(Widget);
static Boolean SetValues(Widget, Widget, Widget, ArgList, Cardinal *);
/*}}}*/
/*{{{  actions*/
static void Notify(Widget, XEvent *, String *, Cardinal *);
static void Release(Widget, XEvent *, String *, Cardinal *);
/*}}}*/
/*{{{  helpers*/
static void redraw(BatteryGaugeWidget me);
static int setPosition(BatteryGaugeWidget me, int);
static void setFlashTimeout(BatteryGaugeWidget me);
static void flashTimeout(XtPointer, XtIntervalId *);
static void updateLabel(BatteryGaugeWidget);
static void updateTimeout(XtPointer, XtIntervalId *);
/*}}}*/
/*}}}*/
/*{{{  actions*/
static XtActionsRec actions[] =
{
  {"Notify", Notify},
  {"Release", Release},
};
/*}}}*/
#define SuperClass (&simpleClassRec)
/*{{{  BatteryGaugeClassRec batteryGaugeClassRec =*/
BatteryGaugeClassRec batteryGaugeClassRec =
{
  /*{{{  core class part*/
  {
    (WidgetClass)SuperClass,          /* superclass */
    "BatteryGauge",                   /* class_name */
    sizeof(BatteryGaugeRec),          /* size */
    ClassInitialize,                  /* class_initialize */
    NULL,                             /* class_part_initialize */
    False,                            /* class_inited */
    Initialize,                       /* initialize */
    NULL,                             /* initialize_hook */
    Realize,                          /* realize */
    actions,                          /* actions */
    XtNumber(actions),                /* num_actions */
    resources,                        /* resources */
    XtNumber(resources),              /* num_resources */
    NULLQUARK,                        /* xrm_class */
    True,                             /* compress_motion */
    XtExposeCompressMultiple,         /* compress_exposure */
    True,                             /* compress_enterleave */
    False,                            /* visible_interest */
    Destroy,                          /* destroy */
    Resize,                           /* resize */
    Redisplay,                        /* expose */
    SetValues,                        /* set_values */
    NULL,                             /* set_values_hook */
    XtInheritSetValuesAlmost,         /* set_values_almost */
    NULL,                             /* get_values_hook */
    XtInheritAcceptFocus,             /* accept_focus */
    XtVersion,                        /* version */
    NULL,                             /* callback_private */
    translations,                     /* default_translations */
    XtInheritQueryGeometry,           /* query_geometry */
    XtInheritDisplayAccelerator,      /* display_accelerator */
    NULL,                             /* extension */
  },
  /*}}}*/
  /*{{{  simple class part*/
  {
    XtInheritChangeSensitive      /* change_sensitive */
  },
  /*}}}*/
  /*{{{  batteryGauge class part*/
  {
  },
  /*}}}*/
};
/*}}}*/
WidgetClass batteryGaugeWidgetClass = (WidgetClass)&batteryGaugeClassRec;
/*{{{  methods*/
/*{{{  static void ClassInitialize()*/
static void ClassInitialize()
/* register the orientation resource converter
 */
{
  XtAddConverter(XtRString, XtROrientation, XmuCvtStringToOrientation,
      (XtConvertArgList)NULL, (Cardinal)0);
  return;
}
/*}}}*/
/*{{{  static void Destroy(Widget w)*/
static void Destroy(Widget w)
/* removes pending timeouts and workprocs
 */
{
  BatteryGaugeWidget me;
  
  me = (BatteryGaugeWidget)w;
  if(me->batteryGauge.updateTimeout != (XtIntervalId)0)
    XtRemoveTimeOut(me->batteryGauge.updateTimeout);
  if(me->batteryGauge.flashTimeout != (XtIntervalId)0)
    XtRemoveTimeOut(me->batteryGauge.flashTimeout);
  if(XtIsRealized((Widget)me))
    XFreeGC(XtDisplay((Widget)me), me->batteryGauge.gc);
  if(me->batteryGauge.popup)
    XtDestroyWidget(me->batteryGauge.popup);
  return;
}
/*}}}*/
/*{{{  static void Initialize(Widget request_w, Widget new_w, ArgList args, Cardinal *num_args)*/
static void Initialize(Widget request_w, Widget new_w, ArgList args, Cardinal *num_args)
/* initializes an instance of the batteryGauge widget
 * set the width and height from the thickness and length
 */
{
  BatteryGaugeWidget request_me;
  BatteryGaugeWidget new_me;
  
  new_me = (BatteryGaugeWidget)new_w;
  request_me = (BatteryGaugeWidget)request_w;
  if(!new_me->batteryGauge.thickness)
    new_me->batteryGauge.thickness = 8;
  if(!new_me->batteryGauge.length)
    new_me->batteryGauge.length = new_me->batteryGauge.thickness;
  if(new_me->batteryGauge.orientation != XtorientHorizontal)
    {
      if(!new_me->core.width)
	new_me->core.width = new_me->batteryGauge.thickness;
      if(!new_me->core.height)
	new_me->core.height = new_me->batteryGauge.length;
      new_me->batteryGauge.length = new_me->core.height;
      new_me->batteryGauge.thickness = new_me->core.width;
    }
  else
    {
      if(!new_me->core.width)
	new_me->core.width = new_me->batteryGauge.length;
      if(!new_me->core.height)
	new_me->core.height = new_me->batteryGauge.thickness;
      new_me->batteryGauge.length = new_me->core.width;
      new_me->batteryGauge.thickness = new_me->core.height;
    }
  new_me->batteryGauge.inverted = new_me->batteryGauge.flash =
      new_me->batteryGauge.notified = 0;
  new_me->batteryGauge.popup = new_me->batteryGauge.label = NULL;
  new_me->batteryGauge.updateTimeout =
      new_me->batteryGauge.flashTimeout = (XtIntervalId)0;
  new_me->batteryGauge.popped = 0;
  return;
}
/*}}}*/
/*{{{  static void Realize(Widget w, XtValueMask *valueMask, XSetWindowAttributes *attributes)*/
static void Realize(Widget w, XtValueMask *valueMask, XSetWindowAttributes *attributes)
/* realizes the batteryGauge widget
 * it is at this point that the  GC is allocated
 * set the idle cursor
 * clip the limits and value
 * set the batteryGauge position, but don't paint it
 */
{
  BatteryGaugeWidget me;
  
  me = (BatteryGaugeWidget)w;
  SuperClass->core_class.realize((Widget)me, valueMask, attributes);
  /*{{{  create the gc*/
  {
    XGCValues gcValues;
    XtGCMask  mask;
    unsigned  depth;
    
    depth = 1;
    gcValues.function = GXcopy;
    gcValues.foreground = 0; /* filled prior to drawing */
    gcValues.background = 0;
    gcValues.fill_style = FillSolid;
    mask = GCForeground | GCFunction | GCFillStyle;
    me->batteryGauge.gc = XCreateGC(XtDisplay((Widget)me),
        XtWindow((Widget)me), mask, &gcValues);
  }
  /*}}}*/
  updateTimeout((XtPointer)me, NULL);
  return;
}
/*}}}*/
/*{{{  static void Redisplay(Widget  w, XEvent  *event, Region  region)*/
static void Redisplay(Widget  w, XEvent  *event, Region  region)
{
  BatteryGaugeWidget  me;
  
  me = (BatteryGaugeWidget)w;
  redraw(me);
  return;
}
/*}}}*/
/*{{{  static void Resize(Widget  w)*/
static void Resize(Widget  w)
/* 
 * get the length and thickness from width and height
 */
{
  BatteryGaugeWidget me;
  
  me = (BatteryGaugeWidget)w;
  if(me->batteryGauge.orientation != XtorientHorizontal)
    {
      me->batteryGauge.length = me->core.height;
      me->batteryGauge.thickness = me->core.width;
    }
  else
    {
      me->batteryGauge.length = me->core.width;
      me->batteryGauge.thickness = me->core.height;
    }
  setPosition(me, -1);
  return;
}
/*}}}*/
/*{{{  static Boolean SetValues(Widget current_w, Widget request_w, Widget new_w, ArgList args, Cardinal *num_args)*/
static Boolean SetValues(Widget current_w, Widget request_w, Widget new_w, ArgList args, Cardinal *num_args)
/* if thickness, length or orientation changed, then request
 * new geometry
 */
{
  BatteryGaugeWidget current_me;  /* this is how we look now */
  BatteryGaugeWidget request_me;  /* this is what we were asked for */
  BatteryGaugeWidget new_me;      /* this is what we'll end up with */
  Boolean repaint;
  
  current_me = (BatteryGaugeWidget)current_w;
  request_me = (BatteryGaugeWidget)request_w;
  new_me = (BatteryGaugeWidget)new_w;
  repaint = False;
  /*{{{  orientation changed?*/
  if(new_me->batteryGauge.orientation != current_me->batteryGauge.orientation)
    {
      repaint = True;
      new_me->core.width = new_me->batteryGauge.orientation != XtorientHorizontal ?
  	  new_me->batteryGauge.thickness : new_me->batteryGauge.length;
      new_me->core.height = new_me->batteryGauge.orientation != XtorientHorizontal ?
  	  new_me->batteryGauge.length : new_me->batteryGauge.thickness;
    }
  /*}}}*/
  /*{{{  length changed?*/
  if(new_me->batteryGauge.length != current_me->batteryGauge.length)
    {
      if(new_me->batteryGauge.orientation != XtorientHorizontal)
  	new_me->core.height = new_me->batteryGauge.length;
      else
  	new_me->core.width = new_me->batteryGauge.length;
      repaint = True;
    }
  /*}}}*/
  /*{{{  thickness changed?*/
  if(new_me->batteryGauge.thickness != current_me->batteryGauge.thickness)
    {
      if(new_me->batteryGauge.orientation != XtorientHorizontal)
  	new_me->core.width = new_me->batteryGauge.thickness;
      else
  	new_me->core.height = new_me->batteryGauge.thickness;
      repaint = True;
    }
  /*}}}*/
  /* we have to reset length and thickness to current dimensions, 
   * in case we never get resized */
  if(new_me->batteryGauge.orientation != XtorientHorizontal)
    {
      new_me->batteryGauge.length = current_me->core.height;
      new_me->batteryGauge.thickness = current_me->core.width;
    }
  else
    {
      new_me->batteryGauge.length = current_me->core.width;
      new_me->batteryGauge.thickness = current_me->core.height;
    }
  if(XtIsRealized((Widget)new_me))
    {
      new_me->batteryGauge.level = current_me->batteryGauge.level;
      new_me->batteryGauge.status = current_me->batteryGauge.status;
      new_me->batteryGauge.lifetime = current_me->batteryGauge.lifetime;
      repaint = setPosition(new_me, current_me->batteryGauge.status);
      if(new_me->batteryGauge.highColor != current_me->batteryGauge.highColor ||
          new_me->batteryGauge.lowColor != current_me->batteryGauge.lowColor ||
          new_me->batteryGauge.criticalColor != current_me->batteryGauge.criticalColor ||
          new_me->batteryGauge.chargingColor != current_me->batteryGauge.chargingColor)
        repaint = True;
      setFlashTimeout(new_me);
    }
  return repaint;
}
/*}}}*/
/*}}}*/
/*{{{  actions*/
/*{{{  static void Notify(Widget w, XEvent *event, String *params, Cardinal *num_params)*/
static void Notify(Widget w, XEvent *event, String *params, Cardinal *num_params)
/* notify of flashing
 */
{
  BatteryGaugeWidget me;
  
  me = (BatteryGaugeWidget)w;
  me->batteryGauge.notified = me->batteryGauge.flash;
  if(me->batteryGauge.flashTimeout != (XtIntervalId)0)
    {
      XtRemoveTimeOut(me->batteryGauge.flashTimeout);
      me->batteryGauge.flashTimeout = (XtIntervalId)0;
    }
  if(me->batteryGauge.inverted)
    {
      me->batteryGauge.inverted = 0;
      redraw(me);
    }
  if(!me->batteryGauge.popup)
    /*{{{  create popup*/
    {
      me->batteryGauge.popup = XtCreatePopupShell("popup",
          overrideShellWidgetClass, (Widget)me, NULL, 0);
      me->batteryGauge.label = XtCreateManagedWidget("label",
          labelWidgetClass, me->batteryGauge.popup, NULL, 0);
    }
    /*}}}*/
  updateLabel(me);
  /*{{{  position popup*/
  {
    /*{{{  static Arg setArgs[] =*/
    static Arg setArgs[] =
    {
      {XtNx, 0},
      {XtNy, 0},
    };
    /*}}}*/
    /*{{{  static Arg getArgs[] =*/
    static Arg getArgs[] =
    {
      {XtNwidth, 0},
      {XtNheight, 0},
    };
    /*}}}*/
    int x, y;
    Dimension w, h;
    int got;
    int screenW;
    
    /*{{{  get position from event*/
    switch(event->type)
    {
      case ButtonPress:
      case ButtonRelease:
        x = event->xbutton.x_root;
        y = event->xbutton.y_root;
        got = 1;
        break;
      case MotionNotify:
        x = event->xmotion.x_root;
        y = event->xmotion.y_root;
        got = 1;
        break;
      case EnterNotify:
      case LeaveNotify:
        x = event->xcrossing.x_root;
        y = event->xcrossing.y_root;
        got = 1;
        break;
      default:
        x = y = 0;
        got = 0;
    }
    /*}}}*/
    XtRealizeWidget(me->batteryGauge.popup);
    getArgs[0].value = (XtArgVal)&w;
    getArgs[1].value = (XtArgVal)&h;
    XtGetValues(me->batteryGauge.popup, getArgs, 2);
    y -= h + 4;
    x -= w / 2;
    screenW = WidthOfScreen(XtScreen((Widget)me));
    if(x < 0)
      x = 0;
    if(y < 0)
      y = 0;
    if(x > (int)(screenW - w))
      x = (int)(screenW - w);
    setArgs[0].value = (XtArgVal)x;
    setArgs[1].value = (XtArgVal)y;
    XtSetValues(me->batteryGauge.popup, setArgs, 2);
  }
  /*}}}*/
  XtPopup(me->batteryGauge.popup, XtGrabNone);
  me->batteryGauge.popped = 1;
  return;
}
/*}}}*/
/*{{{  static void Release(Widget w, XEvent *event, String *params, Cardinal *num_params)*/
static void Release(Widget w, XEvent *event, String *params, Cardinal *num_params)
{
  BatteryGaugeWidget me;
  
  me = (BatteryGaugeWidget)w;
  if(me->batteryGauge.popup)
    XtPopdown(me->batteryGauge.popup);
  me->batteryGauge.popped = 0;
  return;
}
/*}}}*/
/*}}}*/
/*{{{  helpers*/
/*{{{  static void redraw(BatteryGaugeWidget me)*/
static void redraw(BatteryGaugeWidget me)
/* Draw the batteryGauge position. If charging then background color is
 * charging color, otherwise it's normal background. If value <= emptyValue,
 * foreground is emptyColor, else if value >= warningValue, foreground is
 * normalColor, else foreground is warning foreground. If inverted then swap
 * foreground and background.
 */
{
  Pixel foreground;
  Pixel background;
  XRectangle foreRect;
  XRectangle backRect;
  
  background = me->core.background_pixel;
  /*{{{  set foreground color*/
  switch(me->batteryGauge.status)
    {
      case 0:
        foreground = me->batteryGauge.inverted ?
            me->batteryGauge.chargingColor : me->batteryGauge.highColor;
        break;
      case 1:
        foreground = me->batteryGauge.inverted ? background :
            me->batteryGauge.lowColor;
        break;
      case 2:
        foreground = me->batteryGauge.criticalColor;
        if(me->batteryGauge.inverted)
          {
            foreground = background;
            background = me->batteryGauge.criticalColor;
          }
        break;
      case 3:
        foreground = me->batteryGauge.inverted ?
            me->batteryGauge.highColor : me->batteryGauge.chargingColor;
        break;
      default:
        foreground = background;
    }
  /*}}}*/
  foreRect.x = backRect.x = 0;
  foreRect.y = backRect.y = 0;
  foreRect.width = backRect.width = me->core.width;
  foreRect.height = backRect.height = me->core.height;
  if(me->batteryGauge.orientation == XtorientHorizontal)
    /*{{{  horizontal*/
    {
      foreRect.width = me->batteryGauge.position;
      backRect.width -= me->batteryGauge.position;
      backRect.x = foreRect.width;
    }
    /*}}}*/
  else
    /*{{{  vertical*/
    {
      foreRect.height = me->batteryGauge.position;
      backRect.height -= me->batteryGauge.position;
      foreRect.y = backRect.height;
    }
    /*}}}*/
    
  XSetForeground(XtDisplay((Widget)me), me->batteryGauge.gc, foreground);
  XFillRectangle(XtDisplay((Widget)me), XtWindow((Widget)me),
      me->batteryGauge.gc, foreRect.x, foreRect.y, foreRect.width, foreRect.height);
  XSetForeground(XtDisplay((Widget)me), me->batteryGauge.gc, background);
  XFillRectangle(XtDisplay((Widget)me), XtWindow((Widget)me),
      me->batteryGauge.gc, backRect.x, backRect.y, backRect.width, backRect.height);
  return;
}
/*}}}*/
/*{{{  static int setPosition(BatteryGaugeWidget me, int oldStatus)*/
static int setPosition(BatteryGaugeWidget me, int oldStatus)
{
  int changed = 0;
  int pos;
  int flash;
  
  if(me->batteryGauge.level < 0)
    me->batteryGauge.level = 0;
  if(me->batteryGauge.level > 100)
    me->batteryGauge.level = 100;
  
  pos = (me->batteryGauge.length * me->batteryGauge.level + 50) / 100;
  changed |= me->batteryGauge.position != pos;
  me->batteryGauge.position = pos;
  if(me->batteryGauge.level <= me->batteryGauge.criticalLevel &&
      me->batteryGauge.status != 3)
    me->batteryGauge.status = 2;
  changed |= me->batteryGauge.status != oldStatus;
  
  flash = me->batteryGauge.flash;
  if(me->batteryGauge.status == 2 && oldStatus != 2)
    /*{{{  gone critical*/
    {
      me->batteryGauge.notified = 0;
      flash = -1;
    }
    /*}}}*/
  else if(me->batteryGauge.status == 1 && oldStatus != 1)
    /*{{{  gone low*/
    {
      me->batteryGauge.notified = 0;
      flash = 1;
    }
    /*}}}*/
  else if(flash < 1 && me->batteryGauge.level == 100 &&
      (me->batteryGauge.status == 3 ||
        (me->batteryGauge.status == 0 && oldStatus == 3)))
    /*{{{  charged*/
    {
      flash = 1;
      me->batteryGauge.notified = 0;
    }
    /*}}}*/
  else if(me->batteryGauge.level != 100 &&
      me->batteryGauge.status != 2 && me->batteryGauge.status != 1)
    /*{{{  stop flashing*/
    {
      flash = 0;
    }
    /*}}}*/
  
  if(!flash)
    {
      changed |= me->batteryGauge.inverted;
      me->batteryGauge.inverted = 0;
    }
  me->batteryGauge.flash = flash;
  
  return changed;
}
/*}}}*/
/*{{{  static void setFlashTimeout(BatteryGaugeWidget me)*/
static void setFlashTimeout(BatteryGaugeWidget me)
{
  if(!me->batteryGauge.flashTimeout &&
        me->batteryGauge.flash != me->batteryGauge.notified)
    me->batteryGauge.flashTimeout =
        XtAppAddTimeOut(XtWidgetToApplicationContext((Widget)me),
          (unsigned long)me->batteryGauge.flashDelay,
          flashTimeout, (XtPointer)me);
  return;
}
/*}}}*/
/*{{{  static void flashTimeout(XtPointer client, XtIntervalId *id)*/
static void flashTimeout(XtPointer client, XtIntervalId *id)
{
  BatteryGaugeWidget me;
  
  me = (BatteryGaugeWidget)client;
  me->batteryGauge.flashTimeout = (XtIntervalId)0;
  if(me->batteryGauge.flash)
    {
      me->batteryGauge.inverted ^= 1;
      redraw(me);
      setFlashTimeout(me);
    }
  return;
}
/*}}}*/
/*{{{  static void updateLabel(BatteryGaugeWidget me)*/
static void updateLabel(BatteryGaugeWidget me)
{
  if(me->batteryGauge.label)
    {
      static char buffer[24];
      static char const *states[] =
          {"high", "low", "critical", "charging"};
      static Arg arg = {XtNlabel, (XtArgVal)buffer};
      
      if(me->batteryGauge.lifetime < 0)
        sprintf(buffer, "%d%% (%s)", me->batteryGauge.level,
            states[me->batteryGauge.status]);
      else
        sprintf(buffer, "%d%% %s (%s)", me->batteryGauge.level,
            apm_time_nosec(me->batteryGauge.lifetime),
            states[me->batteryGauge.status]);
      XtSetValues(me->batteryGauge.label, &arg, 1);
    }
}
/*}}}*/
/*{{{  static void updateTimeout(XtPointer client, XtIntervalId *id)*/
static void updateTimeout(XtPointer client, XtIntervalId *id)
{
  BatteryGaugeWidget me = (BatteryGaugeWidget)client;
  apm_info info;
  int oldStatus = me->batteryGauge.status;
  unsigned long delay;
  
  me->batteryGauge.updateTimeout = 0;
  apm_read(&info);
  me->batteryGauge.level = info.battery_percentage;
  me->batteryGauge.status = info.battery_status;
  me->batteryGauge.lifetime = info.battery_time < 0 ? -1 :
      info.using_minutes ? info.battery_time * 60 : info.battery_time;
  
  if(setPosition(me, oldStatus) && XtIsRealized((Widget)me))
    {
      redraw(me);
      setFlashTimeout(me);
    }
  
  if(me->batteryGauge.popped)
    updateLabel(me);
  
  delay = me->batteryGauge.updateDelay * 100;
  if(me->batteryGauge.status & 1)
    delay *= 10;
  me->batteryGauge.updateTimeout =
      XtAppAddTimeOut(XtWidgetToApplicationContext((Widget)me),
        delay + 500, updateTimeout, (XtPointer)me);
}
/*}}}*/
/*}}}*/
