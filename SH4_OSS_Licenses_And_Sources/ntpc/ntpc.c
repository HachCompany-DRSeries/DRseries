/* This code will query a ntp server for the local time and display

 * it.  it is intended to show how to use a NTP server as a time
 * source for a simple network connected device.
 * This is the C version.  The orignal was in Perl
 *
 * For better clock management see the offical NTP info at:
 * http://www.eecis.udel.edu/~ntp/
 *
 * written by Tim Hogard (thogard@abnormal.com)
 * Thu Sep 26 13:35:41 EAST 2002
 * Converted to C Fri Feb 21 21:42:49 EAST 2003
 * this code is in the public domain.
 * it can be found here http://www.abnormal.com/~thogard/ntp/
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <string.h>

#define TIME_OFFS_FILE "/tmp/timeoffset"
#define NTP_SERVER     "pool.ntp.org"

int ntpdate();

int main() {
  return ntpdate();
}

/*------------------------------------------------------------------*/
void WriteTimeOffs(long lTimeOffs)
{
  FILE *fd;
  char buffer[40];
  
  sprintf(buffer,"%ld",lTimeOffs);
  fd = fopen(TIME_OFFS_FILE, "w");
  if (fd != NULL){
    fwrite(buffer, strlen(buffer)+1, 1, fd);
    fclose(fd);
  }
}

/*------------------------------------------------------------------*/
int hostname_to_ip(char *hostname , char* ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
	
	if ( (he = gethostbyname( hostname ) ) == NULL) {
		// get the host info
		herror("gethostbyname");
		return 1;
	}
  
	addr_list = (struct in_addr **) he->h_addr_list;
  for(i = 0; addr_list[i] != NULL; i++) {
		//Return the first one;
		strcpy(ip , inet_ntoa(*addr_list[i]) );
		return 0;
	}
  return 1;
}


/*------------------------------------------------------------------*/
int ntpdate() {
  char *hostname=NTP_SERVER;
  char  ipaddr[20];
  int   portno=123;  //NTP is port 123
  int   maxlen=1024; //check our buffers
  int   i;           // misc var i
  int   iRtx=0;
  
  /* build a message.  Our message is all zeros except for a one in the
   * protocol version field
   * msg[] in binary is 00 001 000 00000000 
   * it should be a total of 48 bytes long
   */
  unsigned char msg[48];
  memset(msg,0,sizeof(msg));
  msg[0] = 0x8;
  
  unsigned long  buf[maxlen]; // the buffer we get back
  struct protoent    *proto;
  struct sockaddr_in  server_addr;
  int s;    // socket
  int tmit; // time -- This is a time_t sort of
  
  /* repeat, when a single server doesn't answer and we run into timeout */
  while (1){

    /* retry 10 times */
    if (++iRtx > 10){
      printf("ERR NTPC: RTX=10 -> STOP!\n");
      return -1;
    }
    
    /* convert hostname to ip addr */
    if (hostname_to_ip(hostname, ipaddr)){
      printf("ERR: hostname_to_ip %s failed\n",hostname);
      sleep(10);
      continue; // try again (maybe network was not up yet)
    }
    else
      printf("hostname_to_ip: %s -> %s\n",hostname,ipaddr);
    
    /* open socket */
    proto=getprotobyname("udp");
    s=socket(PF_INET, SOCK_DGRAM, proto->p_proto);
    if (s == -1){
      perror("socket");
      continue; // try again
    }
    
    /* set ip, port */
    memset( &server_addr, 0, sizeof( server_addr ));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ipaddr);
    server_addr.sin_port=htons(portno);
    
    /* send request */
    i=sendto(s,msg,sizeof(msg),0,(struct sockaddr *)&server_addr,
             sizeof(server_addr));
    if (i<0){
      perror("sendto");
      continue; // try again (maybe next server ip from pool works)
    }

    /* set timeout=10s for receive operation */
    struct timeval tv;
    tv.tv_sec  = 10;
    tv.tv_usec = 0;
    if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
      perror("set timeout:");
    }
    
    /* get received data */
    struct sockaddr saddr;
    socklen_t saddr_l = sizeof (saddr);
    i=recvfrom(s,buf,48,0,&saddr,&saddr_l);
    if (i<0){
      perror("recvfrom");
      continue; // try again (maybe next server ip from pool works)
    }
    
    /* The high word of transmit time is the 10th word we get back
     * tmit is the time in seconds not accounting for network delays which
     * should be way less than a second if this is a local NTP server
     */
    tmit=ntohl((time_t)buf[10]);
    
    /* Convert time to unix standard time:
     *  NTP       is seconds since 0000 UT on 1 January 1900 
     *  unix time is seconds since 0000 UT on 1 January 1970 
     * There has been a trend to add a 2 leap seconds every 3 years.
     * Leap seconds are only an issue the last second of the month 
     * in June and December if you don't try to set the clock then 
     * it can be ignored but this is importaint to people who coordinate 
     * times with GPS clock sources.
     */
    tmit-= 2208988800U; 
    
    /* use unix library function to show the local time (it takes care
     * of:
     *   - timezone issues for both north and south of the equator and 
     *   - places that do Summer time or Daylight savings time.
     */
    printf("UTC time from NTP server: %s\n",ctime(&tmit));
    
    /* compare to system time and calculate offset*/
    i=time(0);
    printf("System time is %d seconds off\n",i-tmit);
    WriteTimeOffs(i-tmit);
    break; // success
  }
  return 0;
}
