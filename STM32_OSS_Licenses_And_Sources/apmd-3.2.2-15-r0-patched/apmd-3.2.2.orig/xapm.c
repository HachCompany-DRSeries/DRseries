/* xapm.c -- 
 * Created: Tue Jan  9 21:23:09 1996 by r.faith@ieee.org
 * Revised: Sun Apr 21 16:37:44 1996 by r.faith@ieee.org
 * Copyright 1996 Rickard E. Faith (r.faith@ieee.org)
 *
 * Modified: Mon Apr 15 11:18:30 2002 by jork.loeser@inf.tu-dresden.de
 *	     Added support to  to switch between percent and text automatically
 *	     Fri Mar 26 11:26:00 1999 by bill.mair@bm-soft.com
 *           Added support for IBM ThinkPad charging options
 *           Added option display of AC/Battery useage
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/cursorfont.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Scrollbar.h>
#include "apm.h"

typedef struct _instance_variables
{
    Pixel highColor;
    Pixel lowColor;
    Pixel criticalColor;
    Pixel chargingColor;
    Pixel foreground;
    int highValue;
    int lowValue;
    String geometry;
    int delay;
    Boolean displayPercent;
    Boolean displayPercentAuto;
} instance_variable_rec;

static XtAppContext app_context;
static Widget scrollbar;
static Widget topLevel;
static Widget command;
static XtIntervalId timerId;
static instance_variable_rec iv;
static int debug;

/* Bill Mair, 26.03.1999 ----> */
#define IBM_THINKPAD       0	/* ThinkPads use the battery_flags to indicate charging */
#define SHOW_AC_BAT_STATUS 1
#define DEBUG_UPDATE       0
#define DISPLAYPERCENT_AUTO 1	/* We add support to switch between percent
				   and text automatically. */

#if IBM_THINKPAD == 1
#define TP_CHARGE_FLAG 0x08	/* This works on the TP770X but may be different on other TPs */
#endif

#if SHOW_AC_BAT_STATUS == 1
#define CHAR_COUNT 5
#else
#define CHAR_COUNT 4
#endif
/* <---- Bill Mair, 26.03.1999 */

static void update(XtPointer client_data, XtIntervalId * id)
{
    apm_info i;
    char buf[128];
    static int pixel = -1;
    static int lpixel = -1;
    static int bpixel = -1;
    static int lastPercentage = -1;
    static int lastMinutes = -1;
    static int lastDisplay = -1;
    static int count = 0;
#if SHOW_AC_BAT_STATUS == 1
    static int lastACStatus = -1;
#endif
    apm_read(&i);

#if DEBUG_UPDATE == 1
    /* BM: Added this to display all of the APM flags */
    if (debug)
    {
	printf("\nxapm update\n");
	printf("  apm_flags:          %d\n", i.apm_flags);
	printf("  ac_line_status:     %d\n", i.ac_line_status);
	printf("  battery_status:     %d\n", i.battery_status);
	printf("  battery_flags:      %d\n", i.battery_flags);
	printf("  battery_percentage: %d\n", i.battery_percentage);
	printf("  battery_time:       %d\n", i.battery_time);
	printf("  using_minutes:      %d\n", i.using_minutes);
    }
#endif

#if DISPLAYPERCENT_AUTO == 1
    if(iv.displayPercentAuto){
	/* We are in auto-change mode, where we print percentages if on
	   ac-power, but time if we are on battery power.
	 */
	if(i.ac_line_status != lastACStatus){
	    /* we changed the powerness */
	    lastDisplay = -1;
	    iv.displayPercent = i.ac_line_status;
	}
    }
#endif

    if (iv.displayPercent)
    {
	if (lastDisplay != iv.displayPercent
	    || i.battery_percentage != lastPercentage

#if SHOW_AC_BAT_STATUS == 1
	    || i.ac_line_status != lastACStatus
#endif
	    )
	{
	    /* lastPercentage updated at end */
#if SHOW_AC_BAT_STATUS == 1
	    sprintf(buf, "%s%d%%", i.ac_line_status ? "L" : "B", i.battery_percentage);
#else
	    sprintf(buf, "%d%%", i.battery_percentage);
#endif
	    XtVaSetValues(command, XtNlabel, buf, NULL);
	}
    }
    else
    {
	int minutes = i.using_minutes ? i.battery_time : i.battery_time / 60;

	if (lastDisplay != iv.displayPercent || lastMinutes != minutes
#if SHOW_AC_BAT_STATUS == 1
	    || i.ac_line_status != lastACStatus
#endif
	    )
	{
	    lastMinutes = minutes;
#if SHOW_AC_BAT_STATUS == 1
	    sprintf(buf, "%s%s", i.ac_line_status ? "L" : "B", apm_time_nosec(minutes * 60));
#else
	    sprintf(buf, "%s", apm_time_nosec(minutes * 60));
#endif
	    XtVaSetValues(command, XtNlabel, buf, NULL);
	}
    }
    lastDisplay = iv.displayPercent;
#if SHOW_AC_BAT_STATUS == 1
    lastACStatus = i.ac_line_status;
#endif

    if (i.battery_percentage <= iv.lowValue)
    {
	if (pixel != iv.criticalColor)
	    XtVaSetValues(scrollbar, XtNforeground,
			  pixel = iv.criticalColor, NULL);
	if (bpixel != iv.criticalColor)
	    XtVaSetValues(scrollbar, XtNborderColor,
			  bpixel = iv.criticalColor, NULL);
    }
    else if (i.battery_percentage <= iv.highValue)
    {
	if (pixel != iv.lowColor)
	    XtVaSetValues(scrollbar, XtNforeground, pixel = iv.lowColor, NULL);
	if (bpixel != iv.foreground)
	    XtVaSetValues(scrollbar, XtNborderColor,
			  bpixel = iv.foreground, NULL);
    }
    else
    {
	if (pixel != iv.highColor)
	    XtVaSetValues(scrollbar, XtNforeground, pixel = iv.highColor, NULL);
	if (i.battery_percentage == 100)
	{
	    if (bpixel != iv.highColor)
		XtVaSetValues(scrollbar, XtNborderColor,
			      bpixel = iv.highColor, NULL);
	}
	else
	{
	    if (bpixel != iv.foreground)
		XtVaSetValues(scrollbar, XtNborderColor,
			      bpixel = iv.foreground, NULL);
	}
    }

    if (debug)
	printf("scrollbar color = %d\n", pixel);

    if (i.battery_status == 3
#if IBM_THINKPAD == 1
	|| (i.battery_flags & TP_CHARGE_FLAG)
#endif
	)
    {
	if (lpixel != iv.chargingColor)
	    XtVaSetValues(command, XtNforeground,
			  lpixel = iv.chargingColor, NULL);
    }
    else
    {
	if (i.battery_percentage < iv.lowValue && count++ % 2)
	{
	    if (lpixel != iv.criticalColor)
		XtVaSetValues(command,
			 XtNforeground, lpixel = iv.criticalColor, NULL);
	}
	else
	{
	    if (lpixel != iv.foreground)
		XtVaSetValues(command,
			    XtNforeground, lpixel = iv.foreground, NULL);
	}
    }

    if (debug)
	printf("command color = %d, status = %d\n",
	       lpixel, i.battery_status);

    if (i.battery_percentage != lastPercentage)
	XawScrollbarSetThumb(scrollbar, 0.0,
			     i.battery_percentage < 0
			     ? 0.0
			     : i.battery_percentage / 100.0);

    lastPercentage = i.battery_percentage;

    timerId = XtAppAddTimeOut(app_context, 1000 * iv.delay + 500,
			      update, app_context);
}

static void press(Widget w, XtPointer client_data, XtPointer call_data)
{
    iv.displayPercent = !iv.displayPercent;
    XtRemoveTimeOut(timerId);
    timerId = XtAppAddTimeOut(app_context, 0, update, app_context);
    if (debug)
	printf("displayPercent = %d\n", iv.displayPercent);
}

static XrmOptionDescRec options[] =
{
    {"-highcolor", "*highColor", XrmoptionSepArg, NULL},
    {"-lowcolor", "*lowColor", XrmoptionSepArg, NULL},
    {"-criticalcolor", "*criticalColor", XrmoptionSepArg, NULL},
    {"-chargingcolor", "*chargingColor", XrmoptionSepArg, NULL},
    {"-highvalue", "*highValue", XrmoptionSepArg, NULL},
    {"-lowvalue", "*lowValue", XrmoptionSepArg, NULL},
    {"-delay", "*delay", XrmoptionSepArg, NULL},
    {"-percent", "*percent", XrmoptionNoArg, (XtPointer) "true"},
#if DISPLAYPERCENT_AUTO == 1
    {"-percentauto", "*percentauto", XrmoptionNoArg, (XtPointer) "true"},
#endif
};

#define offset(field) XtOffsetOf( instance_variable_rec, field )
static XtResource resources[] =
{
    {"highColor", XtCForeground, XtRPixel, sizeof(Pixel),
     offset(highColor), XtRString, "green"},
    {"lowColor", XtCForeground, XtRPixel, sizeof(Pixel),
     offset(lowColor), XtRString, "yellow"},
    {"criticalColor", XtCForeground, XtRPixel, sizeof(Pixel),
     offset(criticalColor), XtRString, "red"},
    {"chargingColor", XtCForeground, XtRPixel, sizeof(Pixel),
     offset(chargingColor), XtRString, "blue"},
    {XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
     offset(foreground), XtRString, XtDefaultForeground},
    {"highValue", XtCValue, XtRInt, sizeof(int),
     offset(highValue), XtRImmediate, (XtPointer) 50},
    {"lowValue", XtCValue, XtRInt, sizeof(int),
     offset(lowValue), XtRImmediate, (XtPointer) 10},
    {"geometry", XtCString, XtRString, sizeof(String),
     offset(geometry), XtRString, (XtPointer) "10x100"},
    {"delay", XtCValue, XtRInt, sizeof(int),
     offset(delay), XtRImmediate, (XtPointer) 1},
    {"percent", XtCValue, XtRBoolean, sizeof(Boolean),
     offset(displayPercent), XtRImmediate, (XtPointer) FALSE},
#if DISPLAYPERCENT_AUTO == 1
    {"percentauto", XtCValue, XtRBoolean, sizeof(Boolean),
     offset(displayPercentAuto), XtRImmediate, (XtPointer) FALSE},
#endif
};

static void quit(Widget w, XtPointer client_data, XtPointer call_data)
{
    exit (0);
}

static XtActionsRec main_actions[] =
{
    {"Quit", (XtActionProc) quit},
};

static String main_translations = "<Message>WM_PROTOCOLS:Quit()\n";

int main(int argc, char **argv)
{
    Cursor cursor;
    int c;
    Widget form;
    XFontStruct *fs;
    int fontWidth, fontHeight;
    int x = 0, y = 0, height = 10, width = 100;
    Atom wm_protocols[1];

    switch (apm_exists())
    {
    case 1:
	fprintf(stderr, "No APM support in kernel\n");
	exit(1);
    case 2:
	fprintf(stderr, "Old APM support in kernel\n");
	exit(2);
    }

    topLevel = XtVaAppInitialize(&app_context, "XApm",
				 options, XtNumber(options),
				 &argc, argv, NULL, NULL);

    XtGetApplicationResources(topLevel,
			      &iv,
			      resources,
			      XtNumber(resources),
			      NULL, 0);

    if (iv.delay < 1)
	iv.delay = 1;

    XParseGeometry(iv.geometry, &x, &y, &width, &height);

    while ((c = getopt(argc, argv, "DV")) != -1)
	switch (c)
	{
	case 'D':
	    ++debug;
	    break;
	case 'V':
	    fprintf(stderr, "apmd version %s\n", VERSION);
	    exit(0);
	    break;
	}

    if (debug)
    {
	printf("highColor = %ld\n", iv.highColor);
	printf("lowColor = %ld\n", iv.lowColor);
	printf("criticalColor = %ld\n", iv.criticalColor);
	printf("chargingColor = %ld\n", iv.chargingColor);
	printf("foreground = %ld\n", iv.foreground);
	printf("highValue = %d\n", iv.highValue);
	printf("lowValue = %d\n", iv.lowValue);
	printf("geometry = %s\n", iv.geometry);
    }

    cursor = XCreateFontCursor(XtDisplay(topLevel), XC_top_left_arrow);

    form = XtVaCreateManagedWidget("form",
				   formWidgetClass, topLevel,
				   XtNorientation, XtorientHorizontal,
				   XtNborderWidth, 0,
				   XtNdefaultDistance, 2,
				   NULL);

    command = XtVaCreateManagedWidget("command",
				      commandWidgetClass, form,
				      XtNleft, XtChainLeft,
				      XtNhighlightThickness, 0,
				      XtNinternalHeight, 0,
				      XtNinternalWidth, 0,
				      XtNborderWidth, 0,
				      XtNlabel, "",
				      XtNresize, FALSE,
				      NULL);

    XtVaGetValues(command, XtNfont, &fs, NULL);
    fontWidth = fs->max_bounds.width;
    fontHeight = fs->max_bounds.ascent + fs->max_bounds.descent;
    XtVaSetValues(command, XtNwidth, fontWidth * CHAR_COUNT, NULL);

    if (debug)
    {
	int tmp = (width > fontWidth * CHAR_COUNT - 6)
	? width - fontWidth * CHAR_COUNT - 6
	: fontWidth * CHAR_COUNT;

	printf("width = %d, using %d + %d = %d\n",
	width, fontWidth * CHAR_COUNT, tmp, fontWidth * CHAR_COUNT + tmp);
    }

    scrollbar = XtVaCreateManagedWidget("scrollbar",
					scrollbarWidgetClass, form,
					XtNhorizDistance, 3,
					XtNfromHoriz, command,
				      XtNorientation, XtorientHorizontal,
					XtNscrollHCursor, cursor,
					XtNthickness, fontHeight,
			  XtNlength, (width > fontWidth * CHAR_COUNT - 6)
				     ? width - fontWidth * CHAR_COUNT - 6
					: fontWidth * CHAR_COUNT,
					NULL);

    XawScrollbarSetThumb(scrollbar, 0.0, 0.0);
    XtVaSetValues(scrollbar,
		  XtNtranslations, XtParseTranslationTable(""), NULL);

    XtAddCallback(command, XtNcallback, press, NULL);

    XtRealizeWidget(topLevel);

    /* Add code to handle WM_DELETE_WINDOW cleanly.  */
    XtAppAddActions(app_context, main_actions, XtNumber(main_actions));
    XtOverrideTranslations(topLevel, XtParseTranslationTable(main_translations));
    wm_protocols[0]
      = XInternAtom(XtDisplay(topLevel), "WM_DELETE_WINDOW", False);
    XSetWMProtocols(XtDisplay(topLevel), XtWindow(topLevel), wm_protocols, 1);

    timerId = XtAppAddTimeOut(app_context, 0, update, app_context);
    XtAppMainLoop(app_context);

    return 0;
}
