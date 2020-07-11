/*
 * File: termios.c 
 * 
 */


#include <stddef.h>
#include <types.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/ioctls.h>
#include <termios.h>



/*
int
tcgetwinsize(int fd, struct winsize *ws)
{
	return ioctl(fd, TIOCGWINSZ, ws);
}
*/


/*
The function tcgetpgrp() returns the process group ID of the 
foreground process group on the terminal associated to fd, 
which must be the controlling terminal of the calling process. 
See: https://linux.die.net/man/3/tcsetpgrp
*/
pid_t tcgetpgrp (int fd){

    int s=0;

    //#todo: work in ring0 to implement this.
    if ( ioctl(fd, TIOCGPGRP, &s) < 0 ){
        debug_print ("tcgetpgrp: error\n");
        return ((pid_t)-1);
    }

    return ((pid_t) s);
}


/*
The function tcsetpgrp() makes the process group with process 
group ID pgrp the foreground process group on the terminal 
associated to fd, which must be the controlling terminal 
of the calling process, and still be associated with its session. 
Moreover, pgrp must be a (nonempty) process group belonging 
to the same session as the calling process.   
See: https://linux.die.net/man/3/tcsetpgrp 
 */
int tcsetpgrp (int fd, pid_t pgrp){

    int s=0;
    s = pgrp;

    //#todo: work in ring0 to implement this.
    return ( ioctl(fd, TIOCSPGRP, &s) );
}



int tcgetattr (int fd, struct termios *termios_p){

    // TIOCGETA ?
    return ioctl (fd, TCGETS, termios_p);
}


int 
tcsetattr ( 
    int fd, 
    int optional_actions,
    const struct termios *termios_p )
{

    switch ( optional_actions)
    {
        case TCSANOW:
            return ioctl (fd, TCSETS , termios_p);

        case TCSADRAIN:
            return ioctl (fd, TCSETSW, termios_p);

        case TCSAFLUSH:
            return ioctl (fd, TCSETSF, termios_p);

        default:
            debug_print ("tcsetattr: default\n");
            break;
    };

    errno = EINVAL;

    return (int) (-1);
}


int tcsendbreak (int fd, int duration)
{
    debug_print ("tcsendbreak: [TODO]\n");
    return -1; 
}


int tcdrain (int fd)
{
    return ( ioctl(fd, TIOCDRAIN, 0) );
}


int tcflush (int fd, int queue_selector){

    debug_print ("tcflush: [TODO]\n"); 
    return -1;


	/*
    int com;

	switch (queue_selector){

	case TCIFLUSH:
		com = FREAD;
		break;
	case TCOFLUSH:
		com = FWRITE;
		break;
	case TCIOFLUSH:
		com = FREAD | FWRITE;
		break;

	default:
		errno = EINVAL;
		return (-1);
	}
	return ( ioctl(fd, TIOCFLUSH, &com) );
	*/
}


int tcflow (int fd, int action)
{
    debug_print ("tcflow: [TODO]\n");  
    return -1; 
}


void cfmakeraw (struct termios *termios_p){

    if ( (void *) termios_p == NULL ){
        debug_print ("cfmakeraw: termios_p\n");  
        return;
    }


    termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP |
                             INLCR | IGNCR | ICRNL | IXON);


    termios_p->c_oflag &= ~OPOST;

    termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);

    termios_p->c_cflag &= ~(CSIZE | PARENB);
    termios_p->c_cflag |= CS8;

    termios_p->c_cc[VMIN] = 1;
    termios_p->c_cc[VTIME] = 0;
}


speed_t cfgetispeed (const struct termios* tp)
{
    if ( (void *) tp == NULL ){
        debug_print ("cfgetispeed: tp\n");  
        return 0;
    }

    return tp->c_ispeed;
}


speed_t cfgetospeed (const struct termios* tp)
{
    if ( (void *) tp == NULL ){
        debug_print ("cfgetospeed: tp\n");  
        return 0;
    }

    return tp->c_ospeed;
}



int cfsetispeed (struct termios *termios_p, speed_t speed)
{
    if ( (void *) termios_p == NULL ){
        debug_print ("cfsetispeed: termios_p\n");  
        return -1;
    }

    return -1;
}


int cfsetospeed (struct termios *termios_p, speed_t speed){

    if ( (void *) termios_p == NULL ){
        debug_print ("cfsetospeed: termios_p\n");  
        return -1;
    }


    return -1;
}


int cfsetspeed(struct termios *termios_p, speed_t speed)
{
    if ( (void *) termios_p == NULL ){
        debug_print ("cfsetspeed: termios_p\n");  
        return -1;
    }

	return -1;
}





