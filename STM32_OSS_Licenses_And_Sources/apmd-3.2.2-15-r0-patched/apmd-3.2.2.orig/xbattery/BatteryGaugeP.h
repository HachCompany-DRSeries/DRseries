/*  Copyright (C) 1999 Nathan Sidwell <nathan@acm.org>
 *  $Id: BatteryGaugeP.h,v 1.1 1999/08/02 20:08:10 apenwarr Exp $
 *  This file is available under the GNU General Public License.
 */

#ifndef BatteryGaugeP_h
#define BatteryGaugeP_h
#include <X11/Xaw/SimpleP.h>
#include "BatteryGauge.h"
/*{{{  structs*/
/*{{{  typedef struct _BatteryGaugeClass*/
typedef struct _BatteryGaugeClass
{
  int unused;
} BatteryGaugeClassPart;
/*}}}*/
/*{{{  typedef struct _BatteryGaugeClassRec*/
typedef struct _BatteryGaugeClassRec
{
  CoreClassPart   core_class;
  SimpleClassPart simple_class;
  BatteryGaugeClassPart batteryGauge_class;
} BatteryGaugeClassRec;
/*}}}*/
/*{{{  typedef struct _BatteryGaugePart*/
typedef struct _BatteryGaugePart
{
  /* resources */
  XtOrientation orientation;  /* horizontal or vertical */
  Pixel     criticalColor;    /* critical color */
  Pixel     lowColor;         /* low color */
  Pixel     highColor;        /* high color */
  Pixel     chargingColor;    /* charging color */
  int       flashDelay;       /* flash delay */
  int       updateDelay;      /* update delay */
  Dimension length;           /* length of widget */
  Dimension thickness;        /* height of widget */
  int       criticalLevel;    /* override BIOS's idea */
  int       level;            /* current percent level */
  int       status;           /* current status */
  int       lifetime;         /* current lifetime */
  
  /* private state */
  GC	    gc;	              /* my (unshared) gc */
  int       position;         /* position current level */
  int       inverted;         /* drawn inverted */
  int       flash;            /* flash on and off */
  int       notified;         /* notified of flashing */
  Widget    popup;            /* popup widget */
  Widget    label;            /* label widget */
  int       popped;           /* popped up */
  XtIntervalId flashTimeout;       /* flasher timeout id */
  XtIntervalId updateTimeout;      /* update timeout id */
} BatteryGaugePart;
/*}}}*/
/*{{{  typedef struct _BatteryGaugeRec*/
typedef struct _BatteryGaugeRec
{
  CorePart    core;
  SimplePart  simple;
  BatteryGaugePart  batteryGauge;
} BatteryGaugeRec;
/*}}}*/
/*}}}*/
extern BatteryGaugeClassRec batteryGaugeClassRec;
#endif /* BatteryGaugeP_h */
