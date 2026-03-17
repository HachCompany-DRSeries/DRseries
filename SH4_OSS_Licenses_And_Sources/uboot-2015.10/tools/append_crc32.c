/*
 *  tools/append_crc32.c
 *
 *  Copyright (C) 2007 by Digi International Inc.
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version2  as published by
 *  the Free Software Foundation.
*/
/*
 *  !Revision:   $Revision: 1.3 $
 *  !Author:     Markus Pietrek
 *  !Descr:      appends the CRC32 (same endianess as host) of the image to the
 *               image.
 *               U-Boot may use it to check itself
*/

#include <stdlib.h>             /* EXIT_SUCCESS */
#include <stdio.h>              /* printf */
#include <fcntl.h>              /* O_RDWR */
#include <unistd.h>             /* read */
#include <stdint.h>             /* uint32_t */
#include <netinet/in.h>		/* ntohl */

#define CA_S( sCmd ) \
        do { \
                if( (sCmd) < 0 )  { \
                        perror( #sCmd ); \
                        exit( EXIT_FAILURE ); \
                } \
        } while( 0 )

#define CLOSE( iFd ) \
        do {            \
                CA_S( close( iFd ) ); \
                iFd = -1; \
        } while( 0 )

#define IO_BLOCK_SIZE	4096

/* Provided by crc32.c */
extern unsigned long crc32( unsigned long, const unsigned char *, unsigned int );

/**
 * AppendCRC32 - Appends CRC32 to file
 * @szFileName: filename who's CRC32 needs to be calculated and where to add it
 */
static void AppendCRC32( const char* szFileName )
{
        unsigned char aucBuffer[ IO_BLOCK_SIZE ];
        unsigned long ulCRC32 = 0;
        int           iFd = -1;
        size_t   iBytesWritten;
        uint32_t uiCRC32;

        CA_S( iFd = open( szFileName, O_RDWR | O_APPEND ) );

        /* read the file and calculate CRC32 */
        do {
                size_t iBytesRead;

                CA_S( iBytesRead = read( iFd, aucBuffer, IO_BLOCK_SIZE ) );

                if( !iBytesRead )
                        /* EOF */
                        break;

                ulCRC32 = crc32( ulCRC32, aucBuffer, iBytesRead );
        } while( 1 );

        /* If the host is 64bit, unsigned long is 64bit as well. Convert it to
         * 32bit */
        uiCRC32 = ntohl( ulCRC32 );
        printf( "appending CRC32 0x%08x to %s\n", uiCRC32, szFileName );

        CA_S( iBytesWritten = write( iFd, &uiCRC32, sizeof( uiCRC32 ) ) );
        if( sizeof( uiCRC32 ) != iBytesWritten ) {
                perror( "write" );
                exit( EXIT_FAILURE );
        }

        CLOSE( iFd );
}

/**
 * main - do the work
 */
int main( int argc, char* argv[] )
{
        if( argc != 2 ) {
                fprintf( stderr, "Usage: %s <file_to_append_crc>\n",
                         argv[ 0 ] );
                return EXIT_FAILURE;
        }

        AppendCRC32( argv[ 1 ] );

        return EXIT_SUCCESS;
}
