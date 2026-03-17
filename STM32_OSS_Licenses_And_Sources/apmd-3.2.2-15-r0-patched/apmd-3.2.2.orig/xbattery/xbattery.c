/*  Copyright (C) Nathan Sidwell 1999 <nathan@acm.org>
 *  $Id: xbattery.c,v 1.1 1999/08/02 20:08:10 apenwarr Exp $
 * 
 *  This file is available under the GNU General Public License.
 *  This is inspired by the xapm application by Rickard E Faith
 * (r.faith@ieee.org). This is just a launcher for a BatteryGauge widget.
 * Read that widget's documentation for use.
 */

/*{{{  includes*/
#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Command.h>
#include "BatteryGauge.h"
#include <apm.h>
/*}}}*/

/*{{{  static XrmOptionDescRec options[] =*/
static XrmOptionDescRec options[] =
{
  {"-delay", "*update", XrmoptionSepArg, NULL},
  {"-update", "*update", XrmoptionSepArg, NULL},
  {"-criticalColor", "*criticalColor", XrmoptionSepArg, NULL},
  {"-lowColor", "*warningColor", XrmoptionSepArg, NULL},
  {"-highColor", "*normalColor", XrmoptionSepArg, NULL},
  {"-chargingColor", "*chargingColor", XrmoptionSepArg, NULL},
  {"-criticalLevel", "*criticalLevel", XrmoptionSepArg, NULL},
  {"-flash", "*flash", XrmoptionSepArg, NULL},
  {"-orientation", "*orientation", XrmoptionSepArg, NULL},
};
/*}}}*/
/*{{{  int main(int argc, char **argv)*/
int main(int argc, char **argv)
{
  XtAppContext context;
  Widget topLevel;
  Widget  gauge;
  
  /*{{{  check APM support*/
  switch(apm_exists())
    {
      case 1:
        fprintf(stderr, "xbattery:No APM support available\n");
        exit(1);
      case 2:
        fprintf(stderr, "xbattery:Obsolete APM support in kernel\n");
        exit(1);
    }
  /*}}}*/
  topLevel = XtAppInitialize(&context, "XBattery",
      options, XtNumber(options),
      &argc, argv, NULL, NULL, 0);

  gauge = XtCreateManagedWidget("gauge",
      batteryGaugeWidgetClass, topLevel, NULL, 0);
  XtRealizeWidget(topLevel);
  XtAppMainLoop(context);
  return 0;
}
/*}}}*/
