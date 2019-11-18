/*
 * Copyright (C) 2013 emtrion GmbH.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 */

#include <asm/io.h>
#include <asm/errno.h>
#include <asm/processor.h>
#include <stdlib.h>
#include <common.h>
#include <command.h>
#include <environment.h>
#include <image.h>
#include "display.h"

extern display_info_t *display_glob;

char strcpy_s(char *dest, const char *src, size_t max)
{
	size_t srclen;

	if ((src == NULL) || (dest == NULL))
	{
		return 0;
	}

	srclen = strlen(src);
	if (srclen > max) 
	{
		srclen = max - 1;
	}
	memset( dest, 0x00, max );
	memcpy( dest, src, srclen );
	
	return (srclen);
}

int do_load_splash(cmd_tbl_t* cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int src_addr = 0;
	image_header_t tmp_header;
	unsigned char *splash_zip;
	unsigned long lenp;
    
	if (argc > 1) {
	      src_addr = simple_strtoul(argv[1], NULL, 16);
	      memcpy(&tmp_header, (unsigned char *)(src_addr), sizeof(tmp_header));
	      if ( (ntohl(tmp_header.ih_size) + sizeof(tmp_header)) < display_glob->fbsize ) {
	    
	/* prepare some memory and copy image to this memory space */
		splash_zip = (unsigned char *)malloc(ntohl(tmp_header.ih_size) + sizeof(tmp_header));
		memcpy(splash_zip, (unsigned char *)(src_addr), ntohl(tmp_header.ih_size) + sizeof(tmp_header));
		
		lenp = ntohl(tmp_header.ih_size);
		if (gunzip((unsigned char *)display_glob->fbbuf, display_glob->fbsize, (unsigned char *)(splash_zip + sizeof(tmp_header)), &lenp) != 0) {
		      printf("GUNZIP ERROR - must RESET board to recover\n");
		      do_reset(NULL, 0, 0, NULL);
		}
		printf("Splash Screen updated\n");
	      }
	      else {
		printf("no valid uzb file found\n");
	      }
	}
	else {
	  printf("Source address not set\n");
	}
        return 0;
}

U_BOOT_CMD (
        load_splash, 2, 1, do_load_splash,
        "load splash screen at address splashimage into frame buffer",
        ""
);


int do_start_wdt(cmd_tbl_t* cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int time_out = 0;
    
	if (argc > 1) {
	  time_out = simple_strtoul(argv[1], NULL, 10);
	  if ( time_out <= 32 && time_out > 0 ) {
	    printf("Starting watchdog with timeout of %dsec\n", time_out);
	    iowrite16(RWTCNT_MAGIC | (256 - RWTCNT_CNT(time_out * 8)) , RWTCNT);
	    iowrite16(RWTCSR_MAGIC | RWTCSR_CKS_32S | RWTCSR_TME, RWTCSR);
	  }
	  else {
	    printf("Timeout value allowed from 1 sec to 32 sec\n");
	  }
	}
	else {
	  printf("Timeout value not set\n");
	}
        return 0;
}


U_BOOT_CMD (
        start_wdt, 2, 1, do_start_wdt,
        "starting watchdog with timeout (seconds)",
        ""
);
