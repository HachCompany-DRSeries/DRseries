/*
 *  tslib/src/ts_print.c
 *
 *  Derived from tslib/src/ts_test.c by Douglas Lowder
 *  Just prints touchscreen events -- does not paint them on framebuffer
 *
 * This file is placed under the GPL.  Please see the file
 * COPYING for more details.
 *
 * Basic test program for touchscreen library.
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "tslib.h"

void usage( int argc, char** argv ) {
	printf( "Usage: %s device_name wait_for_sec\n", argv[0] );
	printf( "\tdevice_name  - tdevice to probe, example /dev/input/event0\n" );
	printf( "\twait_for_sec - wait seconds for touch event, if 0 - dont wait!\n" );
	printf( "\tReturn codes:\n" );
	printf( "\t  0          - timeout expired without receiving event.\n" );
	printf( "\t               But this maybe is TouchScreen.\n" );
	printf( "\t -1          - this is NOT TouchScreen device!\n" );
	printf( "\t  1          - this is TouchScreen for shure!\n" );  
	exit(-1);
}

void alarm_handler( int sig ) {
	// time is expired!
	exit(0);
}

int main( int argc, char** argv )
{
	struct tsdev *ts;
	struct ts_sample samp;
	char *tsdevice=NULL;
	int waitsec;
	int ret;
	
	if (argc != 3)
		usage( argc, argv );
		
	tsdevice = argv[1];
	waitsec = atoi( argv[2] );
	if (waitsec < 0)
		usage( argc, argv );
		
	ts = ts_open( tsdevice, 0 );
	if (!ts)
		return -1;
	if (ts_config(ts))
		return -1;
		
	if (!waitsec) {
		return 0;
	}
		
	printf( "Probe device %s, Please Touch Screen Anywhere in %i seconds! ... \n", tsdevice, waitsec );
	signal( SIGALRM, alarm_handler );
	alarm( waitsec );
	ret = ts_read_raw(ts, &samp, 1 );
	if (ret)
		return 1;
		
	return -1;
}
