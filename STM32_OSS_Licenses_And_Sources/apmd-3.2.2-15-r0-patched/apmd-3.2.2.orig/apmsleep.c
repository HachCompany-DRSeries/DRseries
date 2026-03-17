/* apmsleep.c:
 * Suspend computer using APM and wake-up at given time using 
 * the Real Time Clock (RTC) device. 
 *
 * see man page apmsleep.1 for further details
 * 
 * Possible applications:
 *     - Run certain internet lookup programs at certain times
 *       e.g. download email, news, etc.
 *     - Send Faxes at night, but do not run laptop all night
 *     - Start up in the morning to catch up old cron jobs
 * 
 * Hint:
 *     use 'date' to calculate suspend time including daylight saving
 *     time correction.
 *
 * Possible future extensions (anyone?):
 *     - correct handling daylight saving time jumps
 *     - Set/restore '/dev/nvram' to set 'suspend to dram' option
 *     - apmcron job scheduler to add flexibility
 *
 *      Based on Paul Gortmacher's RTC Test/Example Program
 *	Copyright (C) 1999, Peter Englmaier <ppe@mpe.mpg.de>
 *
 *	Released under the GNU General Public License, version 2,
 *	included herein by reference.
 *
 * $Log: apmsleep.c,v $
 * Revision 1.1  1999/08/02 20:08:08  apenwarr
 *
 *
 * Integrated apmsleep, and added xbattery (but without Makefile support).
 *
 * Revision 0.5  1999/08/01 01:29:29  ppe
 * Now using the apm library
 *
 * Revision 0.4  1999/07/19 16:45:29  ppe
 * *** empty log message ***
 * 
 * Revision 0.3  1999/07/19 16:10:38  ppe
 * Bug in wakeup time calculation corrected.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <sys/wait.h>
#include <time.h>
#include <apm.h>

static char rcsid[]="$Id: apmsleep.c,v 1.1 1999/08/02 20:08:08 apenwarr Exp $";
static char rcsrev[] = "$Revision: 1.1 $";

void version() {
  printf("Version: %s (apmsleep 1.3)\n",rcsrev);
}
void usage() {
  printf("Usage:\n\n"
	 "apmsleep [+]hh:mm\n\n"
	 "Example:\n"
	 "   apmsleep +1:15    will suspend for one hour and 15 minutes\n"
	 "   apmsleep 8:00     will suspend until 8:00 am\n\n"
	 "Bugs: Daylight saving jumps are not taken into account.\n"
	 "      Modem ring detection may trigger early wake-up.\n"
	 "      Does not work with Suspend to Disk.\n"
	 "Bug reports to author Peter Englmaier <ppe@mpe.mpg.de>.\n");
}

int main(int argc, char **argv)
{
  int i, rtcfd, retval;
  struct rtc_time rtc_tm;
  struct tm kernel_tm;
  time_t kernel_time, new_time, old_time;
  char *p;
  int c;
  int apmfd=-1;       /* APM device file descriptor */
  int isoffset;       /* setting time offset */
  int hour, minute;   /* alarm time requested */
  int hour1, minute1;  /* backup for precise_mode */
  int flag;
  static int suspend_mode=0, wait_mode=0, debug_mode=0, precise_mode=0,
    noapm_mode=0;

  while (1) {
    /* int this_option_optind = optind ? optind : 1; */
    int option_index = 0;
    static struct  option long_options[] = {
      {"help",    0, 0, 'h'},
      {"debug",   0, 0, 'd'},
      {"version", 0, 0, 'V'},
      {"suspend", 0, 0, 's'},
      {"standby", 0, 0, 'S'},
      {"wait",    0, 0, 'w'},
      {"noapm",   0, 0, 'n'},
      {"precise",   0, 0, 'p'},
      {0,0,0,0}
    };

    c = getopt_long(argc, argv, "hdVsSwnp", long_options, &option_index);
    if (c == -1) break;
    switch(c)
      {
      case 'h':
	usage();
        exit(0);
      case 'V':
	version();
        exit(0);
      case 'd':
	debug_mode=1;
	break;
      case 's': /*suspend*/
	suspend_mode=0;
	break;
      case 'S': /*standby*/
	suspend_mode=1;
	break;
      case 'n': /*noapm*/
	noapm_mode=1;
	wait_mode=1;
	break;
      case 'w':
	wait_mode=!precise_mode;
	break;
      case 'p':/*precise*/
	precise_mode=1;
	break;
      case '?': /*unknown option*/
	exit(1);
      default:
	fprintf(stderr,"unknown option return %d\n", c);
	exit(1);
      }
  } /* end while (1) */
  if (optind < argc) {
    if (optind+1 != argc) {
      fprintf(stderr,"To many arguments.\n"); usage();
      exit(1);
    }
    p=argv[optind];
    isoffset=0;
    if (p[0]=='+') { p++; isoffset=1;}
    if (2!=sscanf(p, "%d:%d", &hour, &minute)) {
      fprintf(stderr,"apmsleep: Bad argument(s)\n");
      exit(1);
    }
    hour1=hour; minute1=minute; /* backup */
  } else {
    fprintf(stderr,"apmsleep: missing argument.\n"); usage();
    exit(1);
  }

  if (noapm_mode) suspend_mode=2;

  /* argument processing finished */

  if (geteuid()!=0) {
    fprintf(stderr, "apmsleep: warning: This program must be run as root "
	      "or have the SUID attribute set\n");      
  }

  /* check if APM is supported */
  if (apm_exists()) {
    fprintf(stderr, "apmsleep: Your kernel does not support APM.\n");
    fprintf(stderr, "apmsleep: Recompile kernel with APM and "
	    "/dev/rtc support\n");
    exit(1);
  }
  if (!noapm_mode) {
    apmfd=open(APM_DEVICE, O_WRONLY);
    if (apmfd < 0) {
      fprintf(stderr, "apmsleep: Cannot open APM device.\n");
      exit(1);
    }
  }

  rtcfd = open ("/dev/rtc", O_RDONLY);
  if (errno == EACCES) {
    fprintf(stderr, "apmsleep: You don't have permission to "
	    "access /dev/rtc\n");      
    fprintf(stderr, "apmsleep: The program must be run as root "
	    "or have the SUID attribute set\n");      
    exit(errno);
  }
  if (rtcfd ==  -1) {
    perror("/dev/rtc");

    fprintf(stderr, "apmsleep: Your kernel does not support "
	    "the real time clock device\n");
    fprintf(stderr, "apmsleep: Recompile kernel with APM and "
	    "/dev/rtc support\n");
    exit(errno);
  }
  
  /* Read the RTC time/date */
  retval = ioctl(rtcfd, RTC_RD_TIME, &rtc_tm);
  if (retval == -1) {
    perror("ioctl");
    exit(errno);
  }
  /* Read Kernel time/date */
  time(&kernel_time);
  kernel_tm=*localtime(&kernel_time);
  
  if (debug_mode) {
    printf("Current RTC time is %02d:%02d:%02d.\n",
	   rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
    printf("Current local time is %02d:%02d:%02d.\n",
	   kernel_tm.tm_hour, kernel_tm.tm_min, kernel_tm.tm_sec);
  }

  /* set alarm time in RTC */
  if (isoffset) {
    if (hour == 0 && minute == 0)
      exit(0);
    hour+=(int)rtc_tm.tm_hour;
    minute+=(int)rtc_tm.tm_min;
    hour1+=(int)kernel_tm.tm_hour;
    minute1+=(int)kernel_tm.tm_min;
  } else {
    /* Special case - wake-up time equal current time
       this may cause to systems to crash according to
       a user report. Therefore, we do not suspend in this case.  */
    if (hour == kernel_tm.tm_hour && minute == kernel_tm.tm_min) {
      printf("Wake-up time equal to current time. Suspend not required\n");
      exit(0);
    }
    rtc_tm.tm_sec=0;
    /* here we take difference between RTC and localtime into account */
    minute+=(int)rtc_tm.tm_min - (int)kernel_tm.tm_min;
    hour+=(int)rtc_tm.tm_hour - (int)kernel_tm.tm_hour;
  }
  while (minute<0) {minute +=60; hour--; }
  while (minute>=60) {minute -=60; hour++; }
  while (hour < 0) hour += 24;
  hour %= 24;
  rtc_tm.tm_min = minute;
  rtc_tm.tm_hour = hour;
  if (debug_mode)
    printf("Setting RTC alarm time to %02d:%02d:%02d.\n",
	   hour, minute, rtc_tm.tm_sec);

  retval = ioctl(rtcfd, RTC_ALM_SET, &rtc_tm);
  if (retval == -1) {
    perror("ioctl");
    exit(errno);
  }

  /* Read back the current alarm settings */
  retval = ioctl(rtcfd, RTC_ALM_READ, &rtc_tm);
  if (retval == -1) {
    perror("ioctl");
    exit(errno);
  }

  if (debug_mode) 
    printf("RTC alarm time now set to %02d:%02d:%02d.\n",
	   rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);
  
  /* Enable alarm interrupts */
  retval = ioctl(rtcfd, RTC_AIE_ON, 0);
  if (retval == -1) {
    perror("ioctl");
    exit(errno);
  }

  /* Save time before suspending */
  time(&new_time);
  kernel_time=new_time;

  if (fork()==0) {
    /* suspend system -- called from within a child, to make
     * sure we don't miss the time leap
     */
    switch(suspend_mode) {
    case 0:
      apm_suspend(apmfd);
      break;
    case 1:
      apm_standby(apmfd);
      break;
    }
    exit(0);
  }

  if (debug_mode) {
    printf("Waiting until clock jumps\n");
    fflush(stdout);
  }


  flag=1;  /* Note: flag is used as exit value */
  if (precise_mode) {
    /* wait for clock match alarm time */
    if (debug_mode) printf("Wait for wakeup time\n");
    do {
      int diff;
      /* Read Kernel time/date */
      time(&kernel_time);
      kernel_tm=*localtime(&kernel_time);
      if (debug_mode) 
	printf("Time: %02d:%02d:%02d, wakeup time: %02d:%02d:%02d\n",
	       kernel_tm.tm_hour, kernel_tm.tm_min, kernel_tm.tm_sec,
	       hour1, minute1, 0);
      diff= hour1*60 + minute1 - kernel_tm.tm_hour*60 - kernel_tm.tm_min;
      diff %= (24*60);
      flag=(diff!=0 && diff!=-1);
      if (flag) sleep(10);
    } while (flag);
  } else {
    /* wait for the actual sleep event */
    do {
      old_time=new_time;
      sleep(2);
      time(&new_time);
      flag=difftime(new_time,old_time)<10.0;
      /* check for time out */
      if (flag && wait_mode==0 && difftime(new_time,kernel_time)>120.0)
	break; /* timeout, flag is 1 */
    } while (flag);

    if (flag) 
      fprintf(stderr,"Time out -- no time leap happened\n");
    else if (debug_mode)
      printf("Time leap detected\n");
  }

  /* Waiting for child to finish */
  wait(&i);

  /* Disable alarm interrupts */
  retval = ioctl(rtcfd, RTC_AIE_OFF, 0);
  if (retval == -1) {
    perror("ioctl");
    exit(errno);
  }
  close(rtcfd);
  if (!noapm_mode) close(apmfd);
  
  return flag;
}
