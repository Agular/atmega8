// Compile with: gcc sendrecv.c -o sendrecv
// Execute to receive with  ./sendrecv -r 
// Execute to send    with  ./sendrecv -s
 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BAUDRATE B9600
char MODEMDEVICE[32]= "/dev/ttyS0";	// !!!

int	fd;				// File descriptor
struct	termios newtio={};

unsigned char send(char c)
{
    int res=write(fd, &c, 1);    if (res<0) printf("Fehler beim Senden\n");
    return res;
}

unsigned char receive()
{
    int res;
    unsigned char buffer;

    res = read(fd, &buffer, 1);
    return buffer;
}

int init()
{
    /*** Init ***/
    
    //O_RDONLY, O_WRONLY or O_RDWR -
    //O_NDELAY (geht weiter, wenn keine Daten da sind und gibt "-1" zurueck)
    // man 2 open fuer mehr Infos - see "man 2 open" for more info
    // O_NOCTTY No ControllTeleType 
    fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY);
    if (fd < 0){
        printf("Fehler beim oeffnen von %s\n", MODEMDEVICE);
        exit(-1);
    }
    memset(&newtio, 0, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;	//setzt die neuen Porteinstellungen
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;         /* set input mode (non-canonical, no echo, ...) */
    newtio.c_cc[VTIME] = 0;     /* inter-character timer unused */
    newtio.c_cc[VMIN] = 1;	/* blocking read until 1 chars received */

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
    return fd;
}

void usage(char * name)
{
    fprintf(stderr, "usage:\n");
    fprintf(stderr, " %s -s  [device] for send\n", name);
    fprintf(stderr, " %s -r  [device] for receive\n", name);
    fprintf(stderr, " %s --help  this help\n\n", name);
    exit(-1);
}


int main(int argc, char** argv)
{
    char c;
    int  i;
    char vBuf[42]="",*pBuf;
    if (argc > 2) strcpy(MODEMDEVICE,argv[2]);
    if((argc < 2) || ( (argc >=2) && (strcmp(argv[1], "-r") == 0) ) ){
	init();
	while (1)
        {
	    c=receive();
            putchar(c);
        }
    } 
    if((argc>=2) && strcmp(argv[1], "-s") == 0){
	init();
        while (1)
        {
	  printf("Enter String to send(max.40):\n");
          pBuf=fgets(vBuf,41,stdin);
          while(pBuf && *pBuf)
            /*if (*pBuf=='\n'){send(0xa);pBuf++;}
            else             */send(*pBuf++);
        }
    }else{
	usage(argv[0]);
    }
    close (fd);
    return 0;
}
