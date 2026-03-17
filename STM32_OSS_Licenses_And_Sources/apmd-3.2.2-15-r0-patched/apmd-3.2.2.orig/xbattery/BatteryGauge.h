/*  Copyright (C) 1999 Nathan Sidwell <nathan@acm.org>
 *  $Id: BatteryGauge.h,v 1.1 1999/08/02 20:08:10 apenwarr Exp $
 *  This file is available under the GNU General Public License.
 */

#ifndef BatteryGauge_h
#define BatteryGauge_h
#include <X11/Xaw/Simple.h>
#include <X11/Xmu/Converters.h>
/*{{{  BatteryGauge widget resources*/
/* 
 * A subclass of the Simple widget
 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 length		     Length		Dimension	1
 orientation	     Orientation	XtOrientation	XtorientVertical
 thickness	     Thickness		Dimension	14
 highColor           Foreground         Color           green
 lowColor            Foreground         Color           orange
 criticalColor       Foreground         Color           red
 chargingColor       Background         Color           blue
 flash               Delay              int             250
 update              Delay              int             15
 criticalLevel       Value              int             0
 level               Value              int             0
 lifetime            Value              int             0
 status              Value              int             0
 */
/*}}}*/
/*{{{  new resource names*/
#ifndef XtNlowColor
#define XtNlowColor "lowColor"
#endif
#ifndef XtNcriticalColor
#define XtNcriticalColor "criticalColor"
#endif
#ifndef XtNhighColor
#define XtNhighColor "highColor"
#endif
#ifndef XtNchargingColor
#define XtNchargingColor "chargingColor"
#endif
#ifndef XtNflash
#define XtNflash "flash"
#endif
#ifndef XtNupdate
#define XtNupdate "update"
#endif
#ifndef XtCDelay
#define XtCDelay "Delay"
#endif
#ifndef XtNcriticalLevel
#define XtNcriticalLevel "criticalLevel"
#endif
#ifndef XtNlevel
#define XtNlevel "level"
#endif
#ifndef XtNstatus
#define XtNstatus "status"
#endif
#ifndef XtNlifetime
#define XtNlifetime "lifetime"
#endif
/*}}}*/
extern WidgetClass batteryGaugeWidgetClass;
typedef struct _BatteryGaugeClassRec*	BatteryGaugeWidgetClass;
typedef struct _BatteryGaugeRec*	BatteryGaugeWidget;
#endif /* BatteryGauge_h */
