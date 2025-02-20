


// libc03
#include <types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

//#bugbug
//#include <netdb.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>

// libgws
#include "include/connect.h"  
#include "include/gws.h"  


char __gws_events_message_buffer[512];


// get next event.
// the window server return the next event
// from the queue of a client.

struct gws_event_d *gws_next_event(int fd)
{

    //
    
    gws_debug_print ("gws_next_event: [TODO]\n"); 
    
    return (struct gws_event_d *) 0;
}


// Send event.
// It is gonna be used by the window manager to send 
// events to the window server.
// Remember: The window manager gets the system messages
// end sends window events to the window server.
void 
gws_send_event ( 
    int fd, 
    int window, 
    struct gws_event_d *event )
{

    // Isso permite ler a mensagem na forma de longs.
    unsigned long *message_buffer = (unsigned long *) &__gws_events_message_buffer[0];   

    int n_writes = 0;   // For sending requests.

    int wid=-1;
    int msg=-1;
    unsigned long long1=0; 
    unsigned long long2=0;


    if ( (void*) event == NULL )
    {
        gws_debug_print ("gws_send_event: fail\n");
        return; 
        
    }else{

        wid = (int)              event->wid;
        msg    = (int)           event->msg; 
        long1  = (unsigned long) event->long1;
        long2  = (unsigned long) event->long2;
    };
    
    
    gws_debug_print ("gws_send_event: Writing ...\n"); 
    
    while (1)
    {
        message_buffer[0] = (unsigned long) 0;     // Nothing 
        message_buffer[1] = (unsigned long) 2030;  // The service number. 
        message_buffer[2] = (unsigned long) 0;     // Nothing 
        message_buffer[3] = (unsigned long) 0;     // Nothing

        // The standard stack.
        message_buffer[4] = (unsigned long) wid;   //  
        message_buffer[5] = (unsigned long) msg;   //  
        message_buffer[6] = (unsigned long)long1;  // 
        message_buffer[7] = (unsigned long)long2;  // 
        //...


        // Write!
        // Se foi possível enviar, então saimos do loop.  

        // n_writes = write (fd, __buffer, sizeof(__buffer));
        n_writes = send ( fd,
                        __gws_events_message_buffer, 
                        sizeof(__gws_events_message_buffer), 
                        0 );

        if (n_writes>0){ break; }
    }

    return;         
}



// let's manage the input inside a window.
// the client gets some keyboard event
// and change the position of the input pointer
// inside the window
// but remeber, the window server has the window structure,
// so we need to tell him to draw the input pointer
// inside the window limits.
// the window server will draw the next char inside the window with focus.




















