/*
 * File: dtext.c 
 * 
 *     string support.
 */


//#include <api.h>

#include <gws.h>



int serviceDrawText(void)
{

	//o buffer é uma global nesse documento.
    unsigned long *message_address = (unsigned long *) &__buffer[0];


    struct gws_window_d *window;
    int window_id = -1;
    unsigned long x;
    unsigned long y;
    unsigned long color;


    // #debug
    gwssrv_debug_print ("gwssrv: serviceDrawText\n");


    // Get

    window_id = message_address[4];
    x         = message_address[5];
    y         = message_address[6]; 
    color     = message_address[7];


    /*
    //size 256 bytes
    unsigned char *text_buffer = (unsigned char *) &message_address[MSG_OFFSET_LONGSTRING];  
   
    int s = sizeof(text_buffer);
   
    if(s<0 || s>= 256)
    {
        gwssrv_debug_print ("gwssrv: serviceDrawText [DEBUG]   SIZE \n");
        exit(1);
    }
    */
   
   //
   // Draw
   //
   
    //#todo
    // Get the window structure given the id.
    //window = (struct gws_window_d *) windowList[window_id];
    
    //#test
    // Usando a janela screen por enquanto.
    
    // OK. string funcionou.
        
    unsigned char buf[256+1];
    int i=0;
    int string_off=8;
    for(i=0; i<256; i++)
    {
         buf[i] = message_address[string_off];
         string_off++;
    }
    buf[i] = 0;
    
    
    //dtextDrawText ( (struct gws_window_d *) gui->screen,
        //x, y, color, buf ); 


    if (window_id>0 && window_id < WINDOW_COUNT_MAX)
    {

        window = (struct gws_window_d *) windowList[window_id];
        
        if((void*)window!=NULL){
            dtextDrawText ( (struct gws_window_d *) window,
                x, y, color, buf );
        }
    
    //#debug
    }else{
        
        dtextDrawText ( (struct gws_window_d *) gui->screen,
            x, y, color, buf ); 
    };


   gws_show_backbuffer (); // for debug   
   
   return 0;
}




/* 
 * dtextDrawString:
 *     Draw a string on the screen. 
 */

void 
dtextDrawString ( 
    unsigned long x,
    unsigned long y,
    unsigned long color,
    unsigned char *string )
{

    int Index=0;

    int cWidth = 8;   //#bugbug  determinando


	//int cWidth = get_char_width ();
	//int cHeight = get_char_height ();
	
	//if ( cWidth == 0 || cHeight == 0 )
    if ( cWidth == 0 )
    {
		//panic ("dtext-draw_string: cWidth");
	    
	    //#debug
        //gde_message_box (3, "xxx","dtext-draw_string: cWidth");
        printf("dtext-draw_string: cWidth\n");
        while(1){}
    }
      
    for ( Index=0; string[Index] != 0; Index++ )
    {
		//@todo: 
		//   Conflito de tipo no argumento 4.
		//   gcharWidth.
		//my_buffer_char_blt( x, y, color, string[Index]);
        
        //drawchar_transparent ( x, y, color, string[Index] );
        charBackbufferDrawcharTransparent ( x, y, color, string[Index] );
        
        
		//#todo: Tamanho do char. Usr variável.
		
        x += cWidth;
    };
}


/* 
 * dtextDrawText:
 *     Draw text on a window.
 *
 * #test: 
 *     Como opção, se nenhuma janela for especificada, 
 *     então pintaremos na janela principal.
 */

void 
dtextDrawText ( 
    struct gws_window_d *window,
    unsigned long x,
    unsigned long y,
    unsigned long color,
    unsigned char *string )
{ 

    struct gws_window_d * __w;

    __w = (struct gws_window_d *) gui->screen;

    if ( (void *) window == NULL )
    {
        dtextDrawString ( __w->left +x, __w->top +y, color, string );
        return;
        
    }else{
        dtextDrawString ( window->left +x, window->top +y, color, string );
    };
}




