/*
 * API - This is the header for 32bit applications.
 * Copyright (c) Fred Nora.
 *
 * File: api.h 
 *
 *   
 * Description:
 *    Header da API do sistema.
 *    API - ( Application Programming Interface ).
 *    The functions here, call the services of the Kernel via interrupts. 
 *    So and application in User Mode can uses the Kernel resources and 
 * the resources of the modules.
 *
 * Sobre as interrupções:
 *     +As primeiras são interrupções de hardware.
 *     +As interrupções de serviços do KERNEL vão de 48 à 199.
 *     +A interrupções principal é a *200.
 *     +As interrupções de chamadas especiais vão de 201 até 215. 
 *     +As interrupções de serviços da GUI vão de 216 à 219.
 *     +As outras estão indefinidas 220 até 255.
 *
 * Patterns:
 *     + My Own.
 *     + Single UNIX Specification (SUS).
 *     + Portable Operating System Interface (POSIX).
 *     + C99. 
 *     + ...  
 *
 * Sequência:
 *     +Os números das interrupções suportadas.
 *     +Os números dos serviços da interrupção 200.
 *
 *     *IMPORTANTE, as definições de chamadas não podem ser minúsculas.
 *
 * Obs: No futuro pode haver alguma reorganização desses números. 
 *      a mesma ordem deverá se obedecida no kernel base. 
 *
 * History:
 *     2013 - Created by Fred Nora.
 *     2014 ~ 2020 New functions.
 */


// rtl
#include <types.h>
#include <stddef.h>


// Modes.
#define GDE_GRAMADO_JAIL        0x00
#define GDE_GRAMADO_P1          0x01
#define GDE_GRAMADO_HOME        0x02
#define GDE_GRAMADO_P2          0x03
#define GDE_GRAMADO_CASTLE      0x04
#define GDE_GRAMADO_CALIFORNIA  0x05
// ...


//
// == events ===========================================
//

// System messages in the thread's event queue.

#define LIBCORE_WAIT_FOR_EVENTS  1
#define LIBCORE_POOL_FOR_EVENTS  2

//  The buffer for the event elements.
unsigned long LibCoreEventBuffer[32];

// Get an event from the thread's event queue.
// That old 'get system message'
// Using a buffer
int libcore_get_event (void);

// ===========================================================




//
// #todo: 
// Fazer suporte a objetos. estruturas.
//


//
// getpid_t:
//     Índices usados pela função gde_get_pid(x)
//     Usada para obter o pid de alguns drivers e servidores
// do sistema. Aqueles servidores que só podem ter um do tipo 
// rodando ao mesmo tempo.
//

typedef enum {

    GETPID_NULL,
    GETPID_WS,
    GETPID_WM
    //...

}getpid_t;



 



// 
//Interrupções. 
//



/*
 * SYSTEM:
 *     Serviços básicos do sistema.
 *     Com essas chamas uma aplicações poderá utilizar recursos
 * de de sistema oferecidos pelo kernel base. São recurso diversos
 * porém não se espera que essas chamas tenham um bom desmpenho, mas
 * sim que ofereçam um grande número de serviços básicos.
 * Obs: A interrupção 200 é a interrupção principal, ela ofereçe
 * vários recursos básicos e fundamentais, portanto não se espera 
 * desempenho nessa chamada, mas sim uma grande quantidade de serviços
 * básicos. Já as interrupções que vão de 201 até 215 oferecem recursos 
 * específicos com o propósito de ganhar desempenho em serviços considerados 
 * importantes para o desempenho geral do sistema.
 *
 */
 
#define  IA32_SYSCALL_VECTOR    0x80 


// List of services for the system interrupt (0x80).
// arch x86.

// NULL.
#define  SYSTEMCALL_NULL  0

// Disk.
#define  SYSTEMCALL_READ_LBA    1
#define  SYSTEMCALL_WRITE_LBA   2
#define  SYSTEMCALL_READ_FILE   3
#define  SYSTEMCALL_WRITE_FILE  4


// Graphics.
#define  SYSTEMCALL_VSYNC                5
#define  SYSTEMCALL_BUFFER_PUTPIXEL      6
#define  SYSTEMCALL_BUFFER_DRAWCHAR      7
#define  SYSTEMCALL_BUFFER_DRAWLINE      8
#define  SYSTEMCALL_BUFFER_DRAWRECT      9
#define  SYSTEMCALL_BUFFER_CREATEWINDOW  10
#define  SYSTEMCALL_REFRESHSCREEN        11


// Network.
#define  SYSTEMCALL_REDE_RES1  12
#define  SYSTEMCALL_REDE_RES2  13
#define  SYSTEMCALL_REDE_RES3  14
#define  SYSTEMCALL_REDE_RES4  15


// libc
#define  SYS_OPEN     16    // open()
#define  SYS_CLOSE    17    // close()
#define  SYS_READ     18    // read()
#define  SYS_WRITE    19    // write()



// Buffer.
#define  SYSTEMCALL_REFRESH_BUFFER1  20
#define  SYSTEMCALL_REFRESH_BUFFER2  21
#define  SYSTEMCALL_REFRESH_BUFFER3  22


// Screen.
#define  SYSTEMCALL_REFRESH_SCREEN2  23


// 24-28 WINDOW SUPPORT
#define  SYSTEMCALL_SHOWWINDOW   24
#define  SYSTEMCALL_25           25
#define  SYSTEMCALL_26           26
#define  SYSTEMCALL_27           27
#define  SYSTEMCALL_28           28


// Buffer: Print string.
#define  SYSTEMCALL_BUFFER_PRINTSTRING  29


//Buffer: Put pixel on buffer.
#define  SYSTEMCALL_BUFFER1_PUTPIXEL  30
#define  SYSTEMCALL_BUFFER2_PUTPIXEL  31
#define  SYSTEMCALL_BUFFER3_PUTPIXEL  32


// Livre: Usar para outra coisa.
#define  SYSTEMCALL_33 33


//Cursor.
#define  SYSTEMCALL_SETCURSOR  34


// Livre: Usar para outra coisa.
#define  SYSTEMCALL_35  35


// ?? rever
// O teclado envia essa mensagem para o procedimento ativo.
#define  SYSTEMCALL_KSENDMESSAGE  36 


// #importante
// Chama o procedimento padrao do sistema.
// Fica no fim da thread.
#define  SYSTEMCALL_CALL_SYSTEMPROCEDURE  37  


// Hostname 
#define  SYS_GETHOSTNAME    38
#define  SYS_SETHOSTNAME    39

// Username.
#define  SYS_GETUSERNAME    40
#define  SYS_SETUSERNAME    41


// Bitmap img: 
// Load bitmap image, size=16x16.
#define  SYSTEMCALL_LOAD_BITMAP_16x16  42  

// Livre. ?
#define  SYSTEMCALL_43  43


// ?? 
// Next app.
#define  SYSTEMCALL_G_NEXT_APP  44


// Message box.
#define  SYSTEMCALL_MESSAGE_BOX  45


// Client area.
#define  SYSTEMCALL_SET_CLIENT_AREA    46


// Create Window support.
// Rever o uso dessas chamadas.
#define  SYSTEMCALL_CREATEWINDOW0  47  
#define  SYSTEMCALL_CREATEWINDOW1  48 
#define  SYSTEMCALL_CREATEWINDOW2  49 


//
// (50~64) 
// Window suppot: 
// manipulação de janelas.
//

#define  SYSTEMCALL_RESIZEWINDOW    50 //resize. 
#define  SYSTEMCALL_REDRAWWINDOW    51 //redraw.
#define  SYSTEMCALL_REPLACEWINDOW   52 //replace.
#define  SYSTEMCALL_MAXIMIZEWINDOW  53 //MAXIMIZE
#define  SYSTEMCALL_MINIMIZEWINDOW  54 //MINMIZE
#define  SYSTEMCALL_GETFOREGROUNDWINDOW  55   //GET FOREGROUND
#define  SYSTEMCALL_SETFOREGROUNDWINDOW  56   // SET FOREGROUND
#define  SYSTEMCALL_REGISTERWINDOW  57  
#define  SYSTEMCALL_CLOSEWINDOW     58
#define  SYSTEMCALL_DESTROYWINDOW   59
#define  SYSTEMCALL_SETACTIVEWINDOW  60
#define  SYSTEMCALL_GETACTIVEWINDOW  61
#define  SYSTEMCALL_SETFOCUS   62
#define  SYSTEMCALL_GETFOCUS   63
#define  SYSTEMCALL_KILLFOCUS  64


// Putchar.
// Muito usada.
#define  SYSTEMCALL_SYS_PUTCHAR  65


// ??
#define  SYSTEMCALL_66  66
#define  SYSTEMCALL_67  67
#define  SYSTEMCALL_68  68
#define  SYSTEMCALL_69  69


//
// Process and Thread.
//


//++
#define  SYSTEMCALL_EXIT             70
#define  SYSTEMCALL_FORK             71  
#define  SYSTEMCALL_CREATETHREAD     72
#define  SYSTEMCALL_CREATEPROCESS    73
#define  SYSTEMCALL_CLOSEALLPROCESS  74
#define  SYSTEMCALL_75               75   //?? livre.
#define  SYSTEMCALL_GETNEXTPROCESS   76
#define  SYSTEMCALL_SETNEXTPROCESS   77
#define  SYSTEMCALL_GETNEXTTHREAD    78
#define  SYSTEMCALL_SETNEXTTHREAD    79
#define  SYSTEMCALL_CURRENTPROCESSINFO  80 
#define  SYSTEMCALL_GETPPID  81
#define  SYSTEMCALL_SETPPID  82
#define  SYSTEMCALL_WAIT4    83 
#define  SYSTEMCALL_84       84    // ?? Livre.
#define  SYSTEMCALL_GETPID   85
#define  SYSTEMCALL_SETPID   86
#define  SYSTEMCALL_SEMAPHORE_DOWN  87
#define  SYSTEMCALL_88              88  //Em uso. não mudar.
#define  SYSTEMCALL_SEMAPHORE_UP    89
#define  SYSTEMCALL_DEAD_THREAD_COLLECTOR  90
#define  SYSTEMCALL_ALERTTHREAD  91
#define  SYSTEMCALL_92  92    // ??
#define  SYSTEMCALL_93  93    // ??
#define  SYSTEMCALL_STARTTHREAD  94
#define  SYSTEMCALL_95  95  // ??livre
#define  SYSTEMCALL_96  96  // ??livre
#define  SYSTEMCALL_97  97  // ??livre
#define  SYSTEMCALL_RESUMETHREAD  98
#define  SYSTEMCALL_99            99  // ??livre
//--

//
// ipc
//

// (100~109) ipc
// Rotinas de mensagens. Channels, Sockets.
#define  SYSTEMCALL_100                100  // ??livre
#define  SYSTEMCALL_101                101  // ??livre
#define  SYSTEMCALL_102                102  // ??livre
#define  SYSTEMCALL_RECEIVEMESSAGE     103  //Pega uma mensagem no PCB de um processo.
#define  SYSTEMCALL_SENDMESSAGE        104  //Envia uma mensagem para o PCB de um processo.
#define  SYSTEMCALL_REGISTERPROCEDURE  105
#define  SYSTEMCALL_CREATECHANNEL      106
#define  SYSTEMCALL_DESTROYCHANNEL     107
#define  SYSTEMCALL_OPENCHANNEL        108
#define  SYSTEMCALL_CLOSECHANNEL       109



// Reboot.
#define  SYSTEMCALL_REBOOT  110


// 111- get single message.
// Ótima. Muito usada.
#define  SYSTEMCALL_111  111

// 112 - Send message to process.
#define  SYSTEMCALL_112  112


// 113 - Update window.
// Envia uma mensagem PAINT para o aplicativo atualizar a 
// área de cliente.
#define  SYSTEMCALL_113  113

// 114 - Send message to thread.
#define  SYSTEMCALL_114  114

// 115 - free slot.
#define  SYSTEMCALL_115  115


// 116 - free slot.
#define  SYSTEMCALL_116  116


// 117 - Send message to thread.
#define  SYSTEMCALL_117  117


// 118 - Create window.
// Cria uma janela usando o kgws.
// Muito usada.
#define  SYSTEMCALL_118  118


// Select color scheme.
#define  SYSTEMCALL_119  119




// livres.
#define  SYSTEMCALL_120    120
#define  SYSTEMCALL_121    121
#define  SYSTEMCALL_122    122
#define  SYSTEMCALL_123    123


// test:
// kernel request para defered system window procedure.
#define  SYSTEMCALL_124    124


// Calling system procedure.
// Muito usada.
#define  SYSTEMCALL_125    125


//
// 126 ~ 129 
// (RESERVADO PARA COMUNICAÇÃO COM DRIVER.)
//


// User mode port IN.
#define  SYSTEMCALL_126 126

// User mode port OUT.
#define  SYSTEMCALL_127 127

// Livre (usar para comunicação com drivers)
#define  SYSTEMCALL_128 128

// 129: 
// Inicializando um driver. 
// Um driver enviou uma systemcall confirmando a 
// inicialização de um driver.
// #bugbug: Talvez isso será feito via socket.
// Ou essa rotina pode ser usado para registrar
// no kernel.
#define  SYSTEMCALL_DRIVERINITIALIZED 129


//
// 130-139 
// Text support.
//


// draw text
#define  SYSTEMCALL_DRAWTEXT  130

// draw text wwf
#define  SYSTEMCALL_131       131

// livre
#define  SYSTEMCALL_132       132

// ??
#define  SYSTEMCALL_133       133

// ??
#define  SYSTEMCALL_134       134

// livres
#define  SYSTEMCALL_135       135  
#define  SYSTEMCALL_136       136


// Thread get char.
#define  SYSTEMCALL_137       137


//
// Keyboard.
//

// 138 - Keyboard. Get key state.
#define  SYSTEMCALL_138       138    

// 139 - Keyboard. Get scancode.
#define  SYSTEMCALL_139       139   

//keyboard responder
#define	SYSTEMCALL_SET_CURRENT_KEYBOARD_RESPONDER 140
#define	SYSTEMCALL_GET_CURRENT_KEYBOARD_RESPONDER 141

//
// Mouse
//

//mouse responder
#define	SYSTEMCALL_SET_CURRENT_MOUSE_RESPONDER 142
#define	SYSTEMCALL_GET_CURRENT_MOUSE_RESPONDER 143



//
// 144-147
// Recursos da area do Cliente.
//

#define  SYSTEMCALL_GETCLIENTAREARECT 144    // Get client Area rect.
#define  SYSTEMCALL_SETCLIENTAREARECT 145    // Set Client Area rect.
#define  SYSTEMCALL_146 146                  // kgws screen window
#define  SYSTEMCALL_147 147                  // kgws main window


// grid
#define  SYSTEMCALL_148 148

// menu
#define  SYSTEMCALL_149 149


//150-159 user and group support.
#define	SYSTEMCALL_CREATEUSER         150
#define	SYSTEMCALL_SETCURRENTUSERID   151
#define	SYSTEMCALL_GETCURRENTUSERID   152
#define	SYSTEMCALL_SETCURRENTGROUPID  153
#define	SYSTEMCALL_GETCURRENTGROUPID  154
#define	SYSTEMCALL_UPDATEUSERINFO     155
#define	SYSTEMCALL_SHOWUSERINFO       156
#define	SYSTEMCALL_GETCURRENTUSERSESSION   157
#define	SYSTEMCALL_GETCURRENTWINDOWSTATION 158
#define	SYSTEMCALL_GETCURRENTDESKTOP       159


//160-169 network support
#define	SYSTEMCALL_160 160   //create socket (retorna o ponteiro para a estrutura)
#define	SYSTEMCALL_161 161   //get socket IP	
#define	SYSTEMCALL_162 162   //get socket port
#define	SYSTEMCALL_163 163   //update socket  
#define	SYSTEMCALL_164 164   //netStream
#define	SYSTEMCALL_165 165   //netSocket
#define	SYSTEMCALL_166 166   //netBuffer

#define	SYSTEMCALL_167 167    //GRAMADOCORE_INIT_EXECVE_BIN
#define	SYSTEMCALL_168 168    //GRAMADOCORE_INIT_EXECVE_EXE
#define	SYSTEMCALL_169 169    //reservado par outro formato

//170-179 network support
#define	SYSTEMCALL_170 170  // Print Working Directory. (pwd)
#define	SYSTEMCALL_171 171  // Get current volume id.
#define	SYSTEMCALL_172 172  // Listar os arquivos do diretório atual.
#define	SYSTEMCALL_173 173  // Procurar arquivo. 
#define	SYSTEMCALL_174 174
#define	SYSTEMCALL_175 175
#define	SYSTEMCALL_176 176
#define	SYSTEMCALL_177 177
#define	SYSTEMCALL_178 178
#define	SYSTEMCALL_179 179

//180-189 Memory support.
#define	SYSTEMCALL_CREATEPAGEDIRECTORY 180
#define	SYSTEMCALL_CREATEPAGETABLE 181
#define	SYSTEMCALL_SHOWMEMORYSTRUCTS 182
#define	SYSTEMCALL_SETCR3 183
#define	SYSTEMCALL_GETPROCESSHEAPPOINTER 184
#define	SYSTEMCALL_SETKERNELHEAP 185
#define	SYSTEMCALL_ALLOCATEKERNELHEAP 186
#define	SYSTEMCALL_FREEKERNELHEAP 187
#define	SYSTEMCALL_GETPROCESSDIRECTORY 188
#define	SYSTEMCALL_SETPROCESSDIRECTORY 189

//190-199 memory support.
#define	SYSTEMCALL_SYS_GETPAGEDIRVALUE 190
#define	SYSTEMCALL_ALLOCATEVIRTUALMEMORY 191
#define	SYSTEMCALL_192 192
#define	SYSTEMCALL_193 193
#define	SYSTEMCALL_194 194
#define	SYSTEMCALL_195 195
#define	SYSTEMCALL_196 196
#define	SYSTEMCALL_197 197
#define	SYSTEMCALL_198 198
#define	SYSTEMCALL_199 199   //@todo: GC

//Outros. (sinais, alertas, mesagens)
#define	SYSTEMCALL_SENDSIGNAL         200  //??reservar.
#define	SYSTEMCALL_WAIT4OBJECT        201
#define	SYSTEMCALL_WAIT4EVENT         202
#define	SYSTEMCALL_WAIT4DEVICE        203
#define	SYSTEMCALL_WAIT4SIGNAL        204
#define	SYSTEMCALL_GENERICWAIT        205
#define	SYSTEMCALL_TIMERGETTICKCOUNT  206
#define	SYSTEMCALL_TIMERGETTIMEOUT    207
#define	SYSTEMCALL_TIMERSETTIMEOUT    208
#define	SYSTEMCALL_CALLIOSERVICES     209

//tty;console;terminal - terminal emulator support.
#define	SYSTEMCALL_CREATETERMINAL          210
#define	SYSTEMCALL_GETCURRENTTERMINAL      211
#define	SYSTEMCALL_SETCURRENTTERMINAL      212
#define	SYSTEMCALL_GETTERMINALINPUTBUFFER  213
#define	SYSTEMCALL_SETTERMINALINPUTBUFFER  214
#define	SYSTEMCALL_GETTERMINALWINDOW       215
#define	SYSTEMCALL_SETTERMINALWINDOW       216
#define	SYSTEMCALL_GETTERMINALRECT         217
#define	SYSTEMCALL_SETTERMINALRECT         218
#define	SYSTEMCALL_DESTROYTERMINAL         219

//Reboot.
#define	SYSTEMCALL_REBOOT2  220

//program support.
#define	SYSTEMCALL_EXECUTEPROGRAM  221  //execute program.
#define	SYSTEMCALL_222             222  
#define	SYSTEMCALL_223  223
#define	SYSTEMCALL_224  224
#define	SYSTEMCALL_225  225

//Critical section. (kernel semaphore)
//Um semáforo do kernel para uso geral.

#define SYSTEMCALL_GET_KERNELSEMAPHORE    226
#define SYSTEMCALL_CLOSE_KERNELSEMAPHORE  227
#define SYSTEMCALL_OPEN_KERNELSEMAPHORE   228

// fixing the names above.

#define SYSTEMCALL_GET_GATE_VALUE  226
#define SYSTEMCALL_CLOSE_GATE      227
#define SYSTEMCALL_OPEN_GATE       228


//debug stuff
#define	SYSTEMCALL_KERNELDEBUG  229

//230-239 (Boot support)
//logon logoff support.
#define	SYSTEMCALL_STARTLOGON     230
#define	SYSTEMCALL_STARTLOGOFF    231
#define	SYSTEMCALL_232            232
#define	SYSTEMCALL_233            233
#define	SYSTEMCALL_234            234
#define	SYSTEMCALL_235            235
#define	SYSTEMCALL_236            236
#define	SYSTEMCALL_237            237
#define	SYSTEMCALL_238            238
#define	SYSTEMCALL_SETBOOTOPTIONS 239


//240-249 Reservar próximos para gerenciamento de cursor e ponteiros.
#define	SYSTEMCALL_GETCURSORX  240
#define	SYSTEMCALL_GETCURSORY  241
#define	SYSTEMCALL_242         242
#define	SYSTEMCALL_243         243
#define	SYSTEMCALL_244         244
#define	SYSTEMCALL_245         245
#define	SYSTEMCALL_246         246
#define	SYSTEMCALL_247         247
#define	SYSTEMCALL_248         248
#define	SYSTEMCALL_249         249


//(250 ~ 255) Info support, get info.
//Ou últimos oferecem informações sobre o sistema.
#define	SYSTEMCALL_GETSYSTEMMETRICS  250 
#define	SYSTEMCALL_251               251
#define	SYSTEMCALL_252               252
#define	SYSTEMCALL_MEMORYINFO        253
#define	SYSTEMCALL_SHOWPCIINFO       254
#define	SYSTEMCALL_SHOWKERNELINFO    255


//
// Ok.
// A interrupção 0x80 tem muitos outros serviços.
// 

/*
 
  260    Deprecated. 
  261    Deprecated.
  
  262     console_read
  263     console_write
  264     ??
  265     yield
  266     process_get_tty
  267     pty_link_by_pid
  268     tty_read_ttyList              kdrivers/tty/tty.c
  269     tty_write_ttyList             kdrivers/tty/tty.c
  270     vt_create
  271     vt_set_child
  272     tty_read                      kdrivers/tty/tty.c
  273     tty_write                     kdrivers/tty/tty.c
  
  
  277     console_get_current_virtual_console
  278     console_set_current_virtual_console 
  279     windowUpdateWindow
  
  289     sys_serial_debug_printk       sci/sys/sys.c
  
  300     UpdateStatusBar
  
  518     redraw_screen
  
  600     sys_dup
  601     sys_dup2
  602     sys_dup3
  
  
  770     systemShowDevicesInfo
  
  801     __gethostname
  801     __sethostname
  803     __getusername
  804     __setusername
  
  808     __ptsname ?
  809     __ptsname ?
  880     __GetProcessStats
  881     __GetThreadStats
  882     getprocessname
  883     getthreadname
  
  
  900     do_clone_execute_process
  901     fork again ?
  
  
  967     network_procedure
  
  4002    sys_fopen
  4003    sys_fopen
  
  7000    sys_socket                    syslib/libcore/socket.c
  7001    sys_connect                   syslib/libcore/socket.c
  7002    sys_accept                    syslib/libcore/socket.c
  7003    sys_bind                      syslib/libcore/socket.c
  7004    sys_listen                    syslib/libcore/socket.c
  //...
  
  
  8000    sys_ioctl                     sci/sys/sys.c
  8001    sys_fcntl                     sci/sys/sys.c
  
  9700    raise_window
  
  9800    ps2_mouse_dialog
  
  
  9900    button_down
  9901    button_up
  
  9999    system_get_pid

*/


//
// #todo
// Temos a interrupção 133.
// 




//
// ====================================
//


/*
 ***********************************************
 * Messages.
 * mensagens para procedimentos de janelas e 
 * para diálogos dentro do gws.
 * Obs: Isso refere-se principalmente à janelas.
 */

 
 //??tipos de mensagens ??
#define MSG_NULL          0 
#define SIGNAL_NULL       0 
#define STREAM_MSG_NULL   0 
#define BUFFER_MSG_NULL   0 
#define CAT_MSG_NULL      0  

 
//window (1-19)  
#define MSG_CREATE        1
#define MSG_DESTROY       2
#define MSG_MOVE          3 
#define MSG_SIZE          4
#define MSG_RESIZE        5
//#define MSG_OPEN        6
#define MSG_CLOSE         7
#define MSG_PAINT         8
#define MSG_SETFOCUS      9
#define MSG_KILLFOCUS     10
#define MSG_ACTIVATE      11
#define MSG_SHOWWINDOW    12 
#define MSG_SETCURSOR     13
#define MSG_HIDE          14
#define MSG_MAXIMIZE      15
#define MSG_RESTORE       16
#define MSG_SHOWDEFAULT   17

//keyboard (20-29)
#define MSG_KEYDOWN       20
#define MSG_KEYUP         21
#define MSG_SYSKEYDOWN    22
#define MSG_SYSKEYUP      23



//mouse (30 - 39)
//tem uma lista de eventos de mouse em events.h
#define MSG_MOUSEKEYDOWN  30
#define MSG_MOUSEKEYUP    31
#define MSG_MOUSEBUTTONDOWN	30
#define MSG_MOUSEBUTTONUP	31 
#define MSG_MOUSEMOVE	    32
#define MSG_MOUSEOVER	    33
#define MSG_MOUSEWHEEL	    34
#define MSG_MOUSEPRESSED	  35
#define MSG_MOUSERELEASED	  36
#define MSG_MOUSECLICKED	  37
#define MSG_MOUSEENTERED	  38
#define MSG_MOUSEEXITED	       39
//#define MSG_MOUSEMOVEBYOFFSET
//#define MSG_MOUSEMOVETOELEMENT



//outros (40 - ...)
#define MSG_COMMAND       40
#define MSG_CUT           41
#define MSG_COPY          42
#define MSG_PASTE         43
#define MSG_CLEAR         44 
#define MSG_UNDO          45
#define MSG_INSERT        46
#define MSG_RUN_PROCESS   47
#define MSG_RUN_THREAD    48
//Quando um comando é enviado para o console. ele será atendido pelo
//módulo /sm no procedimento de janela do sistema.
//Todas as mensagens de console serão atencidas pelo procedimento de janela 
//nessa mensagem.
#define MSG_CONSOLE_COMMAND 49
#define MSG_CONSOLE_SHUTDOWN 50
#define MSG_CONSOLE_REBOOT   51
#define MSG_DEVELOPER 52
//...


//UM TIMER SE ESGOTOU,
#define MSG_TIMER 53   


//o servidor de rede se comunica com o processo.
#define MSG_AF_INET 54
#define MSG_NET_DATA_IN 55

//o driver de network está notificando um processo em ring3.
#define MSG_NETWORK_NOTIFY_PROCESS 56


//
// mouse support: continuação ...
//
#define MSG_MOUSE_DOUBLECLICKED   60
#define MSG_MOUSE_DRAG            61
#define MSG_MOUSE_DROP            62
//...



//
//  terminal commands
//

#define MSG_TERMINALCOMMAND      100
#define TERMINALCOMMAND_PRINTCHAR 1000
//#define TERMINALCOMMAND_PRINT??? 1001
//...



// o evento de rolagem aconteceu ...
// O número do evento será entregue em long1.
#define MSG_HSCROLL 2000
#define MSG_VSCROLL 2001





#define CMD_ABOUT 1000
#define CMD_EXIT 1001
//#define CMD_??? 1002
//... 
 




//
// Process and threads priorities.
//

/*
 * Constantes para níveis de prioridade.
 * Se um processo quiser alterar a prioridade de outro processo.
 * Qualquer processo pode obter sua prioridade e analizar através 
 * dessas definições.
 */
 
//Definições principais. 
#define PRIORITY_LOW4      1  //4
#define PRIORITY_LOW3      2  //3
#define PRIORITY_LOW2      3  //2
#define PRIORITY_LOW1      4  //1 
#define PRIORITY_NORMAL    5  //0 (Normal).
#define PRIORITY_HIGH1     6  //1
#define PRIORITY_HIGH2     7  //2
#define PRIORITY_HIGH3     8  //3
#define PRIORITY_HIGH4     9  //4
//Definições secundárias.
#define PRIORITY_LOW        PRIORITY_LOW1
#define PRIORITY_SUPERLOW   PRIORITY_LOW4
#define PRIORITY_MIN        PRIORITY_SUPERLOW
#define PRIORITY_HIGH       PRIORITY_HIGH1 
#define PRIORITY_SUPERHIGH  PRIORITY_HIGH4
#define PRIORITY_MAX        PRIORITY_SUPERHIGH    
//Definição especial.
#define PRIORITY_REALTIME  10


 
//
//  ## COLOR DEFINES ##
// 


// basic colors
#define rgbBlack    0x000000
#define rgbBlue     0x0000FF
#define rgbCyan     0x00FFFF
#define rgbGreen    0x00FF00
#define rgbMagenta  0xFF00FF
#define rgbRed      0xFF0000
#define rgbYellow   0xFFFF00
#define rgbWhite    0xFFFFFF 

// ## blue support ##
#define COLOR_BLUE1        0x067AB5
#define COLOR_BLUE2CYAN    0x1BA1E2
#define COLOR_BLUE3COBALT  0x0050EF
#define COLOR_BLUE4INDIGO  0x6A00FF

// ## gray support ## 
//cinza para pintar janela
//a janela tem camadas que vai do mais escuro para o mais claro.
#define xCOLOR_BLACK  0x000000  //preto
#define xCOLOR_GRAY1  0x202020  //cinza mais escuro
#define xCOLOR_GRAY2  0x404040  //cinza
#define xCOLOR_GRAY3  0x606060  //cinza
#define xCOLOR_GRAY4  0x808080  //cinza
#define xCOLOR_GRAY5  0xa0a0a0  //cinza
#define xCOLOR_GRAY6  0xc0c0c0  //cinza
#define xCOLOR_GRAY7  0xe0e0e0  //cinza mais clarinho
#define xCOLOR_WHITE  0xffffff  //branco  
 

//WIN23 - TRÊS TIPOS DE CINZA.
#define COLOR_LITBUTTON  0xE0E0E0
#define COLOR_LTGRAY     0xC0C0C0
#define COLOR_GRAY       0x808080 


// ## black support ##
// Preto. 
//50% 	#808080
//45% 	#737373
//40% 	#666666
//35% 	#595959
//30% 	#4d4d4d
//25% 	#404040
//20% 	#333333
//15% 	#262626
//10%   #1a1a1a
//5%    #0d0d0d
//0%    #000000

//w2k - system colors
//#bugbug delete;
#define COLOR_TEST_0  0x8080FF  // COLOR_BACKGROUND
#define COLOR_TEST_1  0x80FFFF  // COLOR_APPWORKSPACE, MDIWINDOW 
#define COLOR_TEST_2  0x80FF80  // COLOR_WINDOW, CLIENT
#define COLOR_TEST_3  0x80FF00  // COLOR_WINDOWTEXT, CLIENTTEXT 
#define COLOR_TEST_4  0xFFFF80  // COLOR_MENU, MENUBAR
#define COLOR_TEST_5  0xFF8000  // COLOR_MENUTEXT (laranjado+-)
#define COLOR_TEST_6  0xC080FF  // COLOR_ACTIVECAPTION, MYCAPTION
#define COLOR_TEST_7  0xFF80FF  // COLOR_INACTIVECAPTION, CAPTION2
#define COLOR_TEST_8  0x0000FF  // COLOR_CAPTIONTEXT, CAPTIONTEXT(azul)highlight
#define COLOR_TEST_9  0x00FFFF  // COLOR_ACTIVEBORDER, BORDER
#define COLOR_TEST_10 0x00FF80  // COLOR_INACTIVEBORDER, BORDER2
#define COLOR_TEST_11 0x40FF00  // COLOR_WINDOWFRAME
#define COLOR_TEST_12 0xFFFF00  // COLOR_SCROLLBAR (amarelo)
#define COLOR_TEST_13 0xC08000  // COLOR_BTNFACE
#define COLOR_TEST_14 0xC08080  // COLOR_BTNSHADOW (vinho)
#define COLOR_TEST_15 0xFF00FF  // COLOR_BTNTEXT (rosa)
#define COLOR_TEST_16 0x404080  // COLOR_HIGHLIGHT ~ COLOR_GRAYTEXT (azul escuro)
#define COLOR_TEST_17 0x4080FF  // COLOR_HIGHLIGHT
#define COLOR_TEST_18 0x00FF00  // COLOR_HIGHLIGHTTEXT (verde)
#define COLOR_TEST_19 0x808000  // COLOR_INACTIVECAPTIONTEXT
#define COLOR_TEST_20 0x804000  // COLOR_BTNHIGHLIGHT
#define COLOR_TEST_21 0xFF8080 
#define COLOR_TEST_22 0x400080 
#define COLOR_TEST_23 0x8000FF
#define COLOR_TEST_24 0x000080  //COLOR_HIGHLIGHT ~ hot track (azul)
#define COLOR_TEST_25 0x0080FF 
#define COLOR_TEST_26 0x008000 
#define COLOR_TEST_27 0x408000 
#define COLOR_TEST_28 0xFF0000  //red
#define COLOR_TEST_29 0xA00000 
#define COLOR_TEST_30 0x800080 
#define COLOR_TEST_31 0xFF0080
#define COLOR_TEST_32 0x000040
#define COLOR_TEST_33 0x004080 
#define COLOR_TEST_34 0x004000 
#define COLOR_TEST_35 0x404000 
#define COLOR_TEST_36 0x800000 
#define COLOR_TEST_37 0x400000 
#define COLOR_TEST_38 0x400040
#define COLOR_TEST_39 0x800040
#define COLOR_TEST_40 0x000000  //COLOR_BTNSHADOW ~ preto text
#define COLOR_TEST_41 0x008080  //verde escuro
#define COLOR_TEST_42 0x408080  //verde escuro
#define COLOR_TEST_43 0x808080  //COLOR_APPWORKSPACE (cinza)
#define COLOR_TEST_44 0x808040 
#define COLOR_TEST_45 0xC0C0C0  //silver
#define COLOR_TEST_46 0x400040 
 
//outros teste
#define COLOR_TEST_47 0x3366FF  //azul claro

    
// # testes #
#define COLOR_ALICEBLUE           0xF0F8FF
#define COLOR_ANTIQUEWHITE        0xFAEBD7
#define COLOR_AQUA                0x00FFFF
#define COLOR_AQUAMARINE          0x7FFFD4 //verde claro.
#define COLOR_AZURE               0xF0FFFF //azul, quase branco. 
#define COLOR_BEIGE               0xF5F5DC
#define COLOR_BISQUE              0xFFE4C4
#define COLOR_BLANCHEDALMOND      0xFFEBCD
#define COLOR_BLUEVIOLET          0x8A2BE2
#define COLOR_BROWN               0xA52A2A
#define COLOR_BURLYWOOD           0xDEB887
#define COLOR_CADETBLUE           0x5F9EA0
#define COLOR_CHARTREUSE          0x7FFF00
#define COLOR_CHOCOLATE           0xD2691E
#define COLOR_CORAL               0xFF7F50
#define COLOR_CORNFLOWERBLUE      0x6495ED
#define COLOR_CORNSILK            0xFFF8DC
#define COLOR_CRIMSON             0xDC143C
#define COLOR_CYAN                0x00FFFF
#define COLOR_DARKCYAN            0x008B8B
#define COLOR_DARKGOLDENROD       0xB8860B
#define COLOR_DARKGRAY            0xA9A9A9
#define COLOR_DARKKHAKI           0xBDB76B
#define COLOR_DARKMAGENTA         0x8B008B
#define COLOR_DARKOLIVEGREEN      0x556B2F
#define COLOR_DARKORANGE          0xFF8C00
#define COLOR_DARKORCHID          0x9932CC
#define COLOR_DARKSALMON          0xE9967A
#define COLOR_DARKSEAGREEN        0x8FBC8B
#define COLOR_DARKSLATEBLUE       0x483D8B
#define COLOR_DARKSLATEGRAY       0x2F4F4F
#define COLOR_DARKTURQUOISE       0x00CED1
#define COLOR_DARKVIOLET          0x9400D3
#define COLOR_DEEPPINK            0xFF1493
#define COLOR_DEEPSKYBLUE         0x00BFFF
#define COLOR_DIMGRAY             0x696969
#define COLOR_DODGERBLUE          0x1E90FF
#define COLOR_FIREBRICK           0xB22222
#define COLOR_FLORALWHITE         0xFFFAF0
#define COLOR_FORESTGREEN         0x228B22
#define COLOR_FUCHSIA             0xFF00FF
#define COLOR_GAINSBORO           0xDCDCDC
#define COLOR_GHOSTWHITE          0xF8F8FF
#define COLOR_GOLD                0xFFD700
#define COLOR_GOLDENROD           0xDAA520
#define COLOR_GRADIENTACTIVECAPTION   0xB9D1EA
#define COLOR_GRADIENTINACTIVECAPTION 0xD7E4F2
#define COLOR_GREENYELLOW         0xADFF2F
#define COLOR_HONEYDEW            0xF0FFF0
#define COLOR_HOTPINK             0xFF69B4 //rosa chiclete.
#define COLOR_HOTTRACK            0x0066CC //azul legal.
#define COLOR_INDIANRED           0xCD5C5C
#define COLOR_INDIGO              0x4B0082 //violeta
#define COLOR_IVORY               0xFFFFF0
#define COLOR_KHAKI               0xF0E68C
#define COLOR_LAVENDER            0xE6E6FA
#define COLOR_LAVENDERBLUSH       0xFFF0F5
#define COLOR_LAWNGREEN           0x7CFC00
#define COLOR_LEMONCHIFFON        0xFFFACD
#define COLOR_LIGHTCORAL          0xF08080
#define COLOR_LIGHTCYAN           0xE0FFFF
#define COLOR_LIGHTGOLDENRODYELLOW 0xFAFAD2
#define COLOR_LIGHTPINK           0xFFB6C1
#define COLOR_LIGHTSALMON         0xFFA07A
#define COLOR_LIGHTSEAGREEN       0x20B2AA
#define COLOR_LIGHTSKYBLUE        0x87CEFA  //azul clarinho
#define COLOR_LIGHTSLATEGRAY      0x778899
#define COLOR_LIGHTSTEELBLUE      0xB0C4DE
#define COLOR_LIGHTYELLOW         0xFFFFE0
#define COLOR_LIME                0x00FF00
#define COLOR_LIMEGREEN           0x32CD32
#define COLOR_LINEN               0xFAF0E6
#define COLOR_MAGENTA             0xFF00FF
#define COLOR_MAROON              0x800000
#define COLOR_MEDIUMAQUAMARINE    0x66CDAA
#define COLOR_MEDIUMBLUE          0x0000CD
#define COLOR_MEDIUMORCHID        0xBA55D3
#define COLOR_MEDIUMPURPLE        0x9370DB
#define COLOR_MEDIUMSEAGREEN      0x3CB371
#define COLOR_MEDIUMSLATEBLUE     0x7B68EE
#define COLOR_MEDIUMSPRINGGREEN   0x00FA9A
#define COLOR_MEDIUMTURQUOISE     0x48D1CC
#define COLOR_MEDIUMVIOLETRED     0xC71585
#define COLOR_MIDNIGHTBLUE        0x191970
#define COLOR_MINTCREAM           0xF5FFFA
#define COLOR_MISTYROSE           0xFFE4E1
#define COLOR_MOCCASIN            0xFFE4B5
#define COLOR_NAVAJOWHITE         0xFFDEAD   // branco areioso.
#define COLOR_NAVY                0x000080
#define COLOR_OLDLACE             0xFDF5E6
#define COLOR_OLIVE               0x808000
#define COLOR_OLIVEDRAB           0x6B8E23
#define COLOR_ORANGE              0xFFA500
#define COLOR_ORANGERED           0xFF4500
#define COLOR_ORCHID              0xDA70D6
#define COLOR_PALEGOLDENROD       0xEEE8AA
#define COLOR_PALEGREEN           0x98FB98
#define COLOR_PALETURQUOISE       0xAFEEEE
#define COLOR_PALEVIOLETRED       0xDB7093
#define COLOR_PAPAYAWHIP          0xFFEFD5
#define COLOR_PEACHPUFF           0xFFDAB9
#define COLOR_PERU                0xCD853F
#define COLOR_PINK                0xFFC0CB
#define COLOR_PLUM                0xDDA0DD
#define COLOR_POWDERBLUE          0xB0E0E6
#define COLOR_PURPLE              0x800080
#define COLOR_ROSYBROWN           0xBC8F8F
#define COLOR_ROYALBLUE           0x4169E1
#define COLOR_SADDLEBROWN         0x8B4513
#define COLOR_SALMON              0xFA8072
#define COLOR_SANDYBROWN          0xF4A460
#define COLOR_SCROLLBAR           0xC8C8C8
#define COLOR_SEAGREEN            0x2E8B57
#define COLOR_SEASHELL            0xFFF5EE  //branco sujinho
#define COLOR_SIENNA              0xA0522D
#define COLOR_SKYBLUE             0x87CEEB  //azul claro
#define COLOR_SLATEBLUE           0x6A5ACD  //azul/violeta
#define COLOR_SLATEGRAY           0x708090
#define COLOR_SNOW                0xFFFAFA //branco
#define COLOR_SPRINGGREEN         0x00FF7F
#define COLOR_STEELBLUE           0x4682B4 //azul legal.
#define COLOR_TAN                 0xD2B48C
#define COLOR_TEAL                0x008080
#define COLOR_THISTLE             0xD8BFD8
#define COLOR_TOMATO              0xFF6347
#define COLOR_TRANSPARENT         0xFFFFFF
#define COLOR_TURQUOISE           0x40E0D0
#define COLOR_VIOLET              0xEE82EE
#define COLOR_WHEAT               0xF5DEB3
#define COLOR_WHITESMOKE          0xF5F5F5 //branco enfumaçado.
#define COLOR_YELLOW              0xFFFF00
#define COLOR_YELLOWGREEN         0x9ACD32
#define STEALTH_ORANGE            0xFF8800
#define STEALTH_OLIVE             0x666600
#define STEALTH_GREEN             0x33DD11
#define STEALTH_PINK              0xFF22EE
#define STEALTH_BLUE              0x0077BB
 
// ## text support ##
#define COLOR_TEXT2  0x00404040   //25%
#define COLOR_TEXT1  0x00333333   //20% 
#define COLOR_TEXT   COLOR_TEXT1 
//#define COLOR_TEXT  0x00000000  

 
#define COLOR_WINDOW              0x00FFFFFF
#define COLOR_BLACK               0x000000
#define COLOR_WHITE               0xFFFFFF 
#define COLOR_RED                 0xFF0000 
#define COLOR_GREEN               0x00FF00
#define COLOR_BLUE                0x0000FF
#define COLOR_DARKRED             0x8B0000
#define COLOR_DARKGREEN           0x006400
#define COLOR_DARKBLUE            0x00008B
//#define COLOR_GRAY                0x808080
#define COLOR_GREY                0xAFAFAF
#define COLOR_LIGHTGRAY           0xD3D3D3
#define COLOR_LIGHTGREEN          0x90EE90
#define COLOR_LIGHTBLUE           0xADD8E6
#define COLOR_SILVER              0xC0C0C0
 


//
// ## WINDOW COLORS ##
//   
 
#define COLOR_ACTIVEBORDER        0xB4B4B4

#define COLOR_ACTIVECAPTION       0x99B4D1
#define COLOR_ACTIVECAPTION_TEXT   0x000000

#define COLOR_APPWORKSPACE  COLOR_SILVER 

// A cor padrão para o Bg é azul quase escuro,
// o azul escuro é usado em seleção de item.

#define COLOR_BACKGROUND  0x008080  
#define COLOR_BACKGROUNDTEXT  COLOR_TEXT  

#define COLOR_BORDER  COLOR_SILVER 



// ## button support ##
#define COLOR_BUTTONFACE 0xF0F0F0
#define COLOR_BUTTONFACE2 0xE0E0E0
#define COLOR_BUTTONFACE3 0xD0D0D0
#define COLOR_BUTTONHIGHLIGHT 0xFFFFFF
#define COLOR_BUTTONHIGHLIGHT2 0x404040
#define COLOR_BUTTONHIGHLIGHTTEXT COLOR_WHITE
#define COLOR_BUTTONHIGHLIGHTTEXT2 COLOR_WHITE //@todo: branco menos intenso.
#define COLOR_BUTTONSHADOW 0xA0A0A0
#define COLOR_BUTTONSHADOW2 0x303030  
#define COLOR_BUTTONTEXT COLOR_TEXT


// # control support #
#define COLOR_CONTROL             0xF0F0F0
#define COLOR_CONTROLTEXT COLOR_TEXT
#define COLOR_CONTROLDARK         0xA0A0A0
#define COLOR_CONTROLDARKDARK     0x696969
#define COLOR_CONTROLLIGHT        0xE3E3E3
#define COLOR_CONTROLLIGHTLIGHT   0xFFFFFF
#define COLOR_CONTROLLIGHTLIGHTTEXT COLOR_WHITE

#define COLOR_DESKTOP  0x000000

#define COLOR_FOCUS_TOPBAR  0x7FFF00



//TEXT SUPPORT
#define COLOR_GRAYTEXT  0x808080
#define COLOR_HIGHLIGHT           0x3399FF
#define COLOR_HIGHLIGHTTEXT       0xFFFFFF


#define COLOR_INACTIVEBORDER      0xF4F7FC
#define COLOR_INACTIVECAPTION     0xBFCDDB
#define COLOR_INACTIVECAPTIONTEXT 0x434E54

#define COLOR_INFO  0xFFFFE1
#define COLOR_INFOTEXT  TEXT_COLOR

#define COLOR_MENU  COLOR_GRAY
#define COLOR_MENUTEXT  COLOR_TEXT

#define COLOR_MENUITEM  COLOR_WHITE
#define COLOR_MENUITEMTEXT  COLOR_TEXT

#define COLOR_MENUITEMSELECTED  COLOR_BLUE
#define COLOR_MENUITEMSELECTEDTEXT  COLOR_WHITE

#define COLOR_MENUBAR  COLOR_GRAY
#define COLOR_MENUBARTEXT  COLOR_TEXT

#define COLOR_MENUHIGHLIGHT  0x003399FF
#define COLOR_MENUHIGHLIGHTTEXT  COLOR_WHITE

#define COLOR_NAVIGATIONBAR       COLOR_BLACK
#define COLOR_NAVIGATIONBARTEXT  COLOR_WHITE

//status bar
#define COLOR_STATUSBAR  COLOR_WINDOW   //branca
#define COLOR_STATUSBAR2 xCOLOR_GRAY6   //cinza
#define COLOR_STATUSBAR3 0x83FCFF       //verde
//...
#define COLOR_STATUSBARTEXT      COLOR_TEXT

#define COLOR_TASKBAR             COLOR_WINDOW
#define COLOR_TASKBARTEXT        COLOR_TEXT

#define COLOR_TITLEBAR            COLOR_BLUE
#define COLOR_TITLEBARTEXT       COLOR_TEXT

#define COLOR_TOPBAR  0x83F52C
#define COLOR_TOPBARTEXT  COLOR_TEXT

#define COLOR_WINDOWFRAME  COLOR_GRAY


// NÃO FAREMOS JANELAS PRETAS
// PRETO É O BACKGROUND
#define COLOR_WINDOWTEXT COLOR_TEXT  

//area de trabalho
#define COLOR_WORKSPACE 0x0000FF00 
#define COLOR_WORKSPACETEXT COLOR_TEXT


// ## virtual terminal support ##
#define COLOR_TERMINAL  COLOR_BLACK
#define COLOR_TERMINAL2 0x303030
//...

#define COLOR_TERMINALTEXT COLOR_WHITE
#define COLOR_TERMINALTEXT2 xCOLOR_GRAY7
//...

//
// ========
//  
 
 
/*
 * (w) Colors, futuristic GUI. (Dark blu and Black)
 */ 
 
//#define COLOR_DARKBLUE   0x00008B00 
//#define COLOR_BLACK      0x00000000 
 
 
#define COLOR_KERNEL_BACKGROUND COLOR_BLUE   


static unsigned long cga_16colors_palette[16] = {
	
	//Paleta do paint para 4bpp
	0x00000000, // 
	0x80000000, // 
	0x00800000, // 
	0x80800000, // 
	0x00008000, // 
	0x80008000, // 
	0x00808000, // 
	0x80808000, // 
	
	0xC0C0C000, // 
	0xFF000000, // 
	0x00FF0000, // 
	0xFFFF0000, // 
	0x0000FF00, // 
	0xFF00FF00, // 
	0x00FFFF00, // 
	0xFFFFFF00  // 
};



static unsigned long vga_256colors_palette[256] = {
	
//black
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 
0x00000000, 

//blue	
0x0000CC00, 
0x0000CD00,	
0x0000D000,	
0x0000D100,	
0x0000D200,	
0x0000D300,	
0x0000D400,	
0x0000D500,	
0x0000D600,
0x0000D700,
0x0000D800,
0x0000D900,
0x0000DA00,
0x0000DB00,
0x0000DC00,
0x0000DD00,	

 //green
0x00CC0000, 
0x00CD0000,	
0x00D00000,	
0x00D10000,	
0x00D20000,	
0x00D30000,	
0x00D40000,	
0x00D50000,	
0x00D60000,
0x00D70000,
0x00D80000,
0x00D90000,
0x00DA0000,
0x00DB0000,
0x00DC0000,
0x00DD0000,	

//cyan	
0x00AAAA00,  
0x00AAAA00, 
0x00AAAA00,  
0x00AAAA00,  	
0x00AAAA00,  
0x00AAAA00,  
0x00AAAA00,  
0x00AAAA00,  	
0x00AAAA00,  
0x00AAAA00,  
0x00AAAA00,  
0x00AAAA00,  	
0x00AAAA00,  
0x00AAAA00,  
0x00AAAA00,  
0x00AAAA00,
	
//red	
0xCC000000, 
0xCD000000,	
0xD0000000,	
0xD1000000,	
0xD2000000,	
0xD3000000,	
0xD4000000,	
0xD5000000,	
0xD6000000,
0xD7000000,
0xD8000000,
0xD9000000,
0xDA000000,
0xDB000000,
0xDC000000,
0xDD000000,	

//magenta
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 
0xAA00AA00, 

//brown
0xAA550000,  
0xAA550000,
0xAA550000, 
0xAA550000, 
0xAA550000, 
0xAA550000, 
0xAA550000, 
0xAA550000, 
0xAA550000, 
0xAA550000,  
0xAA550000, 
0xAA550000, 
0xAA550000, 
0xAA550000, 
0xAA550000,  
0xAA550000,

//light gray
0xAAAAAA00,  
0xAAAAAA00,  
0xAAAAAA00,  
0xAAAAAA00,  
0xAAAAAA00,  
0xAAAAAA00,  
0xAAAAAA00, 
0xAAAAAA00, 
0xAAAAAA00, 
0xAAAAAA00, 
0xAAAAAA00, 
0xAAAAAA00, 
0xAAAAAA00, 
0xAAAAAA00, 
0xAAAAAA00,
0xAAAAAA00,  
	
//gray
0x00000000, //#define xCOLOR_BLACK   //preto
0x20202000, //#define xCOLOR_GRAY1   //cinza mais escuro
0x40404000, //#define xCOLOR_GRAY2   //cinza
0x60606000, //#define xCOLOR_GRAY3   //cinza
0x80808000, //#define xCOLOR_GRAY4   //cinza
0xa0a0a000, //#define xCOLOR_GRAY5   //cinza
0xb0b0b000,
0xc0c0c000, //#define xCOLOR_GRAY6   //cinza
0xd8d8d800,
0xd9d9d900,
0xdadada00,
0xdbdbdb00,
0xdcdcdc00,
0xdddddd00,
0xe0e0e000, //#define xCOLOR_GRAY7   //cinza mais clarinho
0xffffff00, //#define xCOLOR_WHITE   //branco 	

//light blue
0x5555CB00, 
0x5555CC00,
0x5555D000,
0x5555D100,
0x5555D200,
0x5555D300,
0x5555D400,
0x5555D500,
0x5555D600,
0x5555D700,
0x5555D800,
0x5555D900,
0x5555DA00,	
0x5555DB00,
0x5555DC00,
0x5555DD00,	

//light green	
0x55CB5500, 
0x55CC5500,
0x55D05500,
0x55D15500,
0x55D25500,
0x55D35500,
0x55D45500,
0x55D55500,
0x55D65500,
0x55D75500,
0x55D85500,
0x55D95500,
0x55DA5500,	
0x55DB5500,
0x55DC5500,
0x55DD5500,	
	
//light cyan	
0x55FFFF00, 
0x55FFFF00, 
0x55FFFF00, 
0x55FFFF00,
0x55FFFF00,
0x55FFFF00, 
0x55FFFF00,
0x55FFFF00,
0x55FFFF00, 
0x55FFFF00, 
0x55FFFF00,
0x55FFFF00, 
0x55FFFF00, 
0x55FFFF00, 
0x55FFFF00,
0x55FFFF00, 	
	
//light red
0xCB555500, 
0xCC555500,
0xD0555500,
0xD1555500,
0xD2555500,
0xD3555500,
0xD4555500,
0xD5555500,
0xD6555500,
0xD7555500,
0xD8555500,
0xD9555500,
0xDA555500,	
0xDB555500,
0xDC555500,
0xDD555500,	
	
//light magenta
0xFF55FF00, 
0xFF55FF00,
0xFF55FF00, 
0xFF55FF00,
0xFF55FF00,
0xFF55FF00,
0xFF55FF00,
0xFF55FF00,
0xFF55FF00,
0xFF55FF00,
0xFF55FF00,
0xFF55FF00,
0xFF55FF00,
0xFF55FF00,
0xFF55FF00,
0xFF55FF00,

//yellow
0xFFFF5500, 
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
0xFFFF5500,
	
//white
0xFFFFFF00, 
0xFFFFFF00, 
0xFFFFFF00, 
0xFFFFFF00, 
0xFFFFFF00, 
0xFFFFFF00,
0xFFFFFF00,
0xFFFFFF00,
0xFFFFFF00,
0xFFFFFF00,
0xFFFFFF00,
0xFFFFFF00,
0xFFFFFF00,
0xFFFFFF00,
0xFFFFFF00,
0xFFFFFF00  
	
};


//
// Linha e coluna.
//

//Suporte para linha e coluna.
//#define COL_MAX      800  //1024,640, provisório.
//#define LINHA_MAX    600  //768,480, provisório. 
//...


//
// Client Area support.
//

//Suporte para a área de cliente.
#define CLIENT_X_START   0
#define CLIENT_X_END     800    //640.
#define CLIENT_Y_START   40
#define CLIENT_Y_END     508    //420.
//...


//
// Message Box support.
//

//bg
#define MB_X              100
#define MB_Y              100
#define MB_LARGURA        200
#define MB_ALTURA         90
//title
#define MB_TITULO_X       (MB_X + 2 )
#define MB_TITULO_Y       (MB_Y + 2 )
#define MB_TITULO_LARGURA  (200 - 4)
#define MB_TITULO_ALTURA   30
//client
#define MB_CLIENT_X       (MB_X + 2)              
#define MB_CLIENT_Y       (MB_Y + MB_TITULO_ALTURA + 2)
#define MB_CLIENT_LARGURA ( MB_LARGURA - 4 )
#define MB_CLIENT_ALTURA  ( MB_ALTURA - MB_TITULO_ALTURA - 2)
//string       
#define MB_STRING_X       110  
#define MB_STRING_Y       110


//
// Title Bar support.
//

//#define TB_X           0
//#define TB_Y           0
//#define TB_LARGURA     800    //1024,640.
//#define TB_ALTURA      40
//#define TB_STRING_X    10  
//#define TB_STRING_Y    5


//
// Char support.
//

//Suporte para caracteres.
#define CHAR_WIDTH   8
#define CHAR_HEIGHT  8
//...

//
// Bars support.
//

// #define BAR_STEPS   46


//
// Video memory support.
//

//Suporte para memória de vídeo.
#define MEM_BASE 0x000A0000  //VGA.
//...

//
// Linhas e colunas.
//

//Suporte ao gerenciamento de linhas e colunas.
#define LINE    1024    //640.
//...


//
// Window Style support.
//

//Suporte ao estilo de janela.
#define WS_SIMPLES       1
#define WS_POPUP         2
#define WS_BUTTON_DOWN   3
#define WS_BUTTON_UP     4
//Continua...


//
// Task Bar support.
//

//Suporte a barra de tarefas.
#define BARRA_DE_TAREFAS_X        0
#define BARRA_DE_TAREFAS_Y        560    //748,460.
#define BARRA_DE_TAREFAS_LARGURA  800    //1024,640.
#define BARRA_DE_TAREFAS_ALTURA   20
#define AREA_DE_TAREFAS_X  MENU_INICIAR_LARGURA
#define AREA_DE_TAREFAS_Y  548    
#define AREA_DE_TAREFAS_LARGURA  (BARRA_DE_TAREFAS_LARGURA - MENU_INICIAR_LARGURA)
#define AREA_DE_TAREFAS_ALTURA   20



//
// Desktops support.
//

//
// Suporte à desktop:
// Obs: Uma user session contém uma window station, uma window station
// contém um desktop e um desktop contém janelas.
// 
//Dimensões da tela.
#define DESKTOP_X        0
#define DESKTOP_Y        0
#define DESKTOP_LARGURA    800    //1024,640.
#define DESKTOP_ALTURA     600    //768,480.


//
// Menu support.
//

//Suporte geral à menus.
#define MENU_X  0
#define MENU_Y  600    //768          //460 
#define MENU_LARGURA  200
#define MENU_ALTURA   200
#define MENU_INICIAR_X  0
#define MENU_INICIAR_Y  600    //748    //460
#define MENU_INICIAR_LARGURA  60
#define MENU_INICIAR_ALTURA   20
#define menu_size  (MENU_ALTURA * 8)


//
// task Bar support.
//

//Suporte à barra de tarefas.
#define TASKBUTTONS_BASE 40      //Botão.
#define CLOCK_BASE  (800-40)    //Relógio.   
//...


//
// Disk sectors support.
//

//Suporte à diso.
#define INIT_SECTOR 20    //??
//...


// #bugbug: 
// Isso não faz parte da API de aplicativos.
// Pode ir para a API de drivers.

//
// Floppy Disk Controller support.
//
//Floppy disk support. (@todo:).
#define DOR1  0x3F2    /* Digital Output Register */
#define DOR2  0x372  
#define MSR1  0x3F4    /* Main Status Register */
#define MSR2  0x374  
#define DR1   0x3F5    /* Data Register */
#define DR2   0x375
#define DIR1  0x3F7    /* Digital Input Register */
#define DIR2  0x377
#define CCR1  0x3F7    /* Configuration Contorol Register */
#define CCR2  0x377
#define DOR   DOR1
#define DSR   MSR1


//
// Keyboard LEDs support.
//

#define LED_SCROLLLOCK  1
#define LED_NUMLOCK     2
#define LED_CAPSLOCK    4  


//
// Virtual Keys support.
//


//ASCII
#define KEY_RETURN   13     //sc 28
#define KEY_TAB	     15
#define KEY_SHIFT	 42     //16
#define KEY_CONTROL	 29     //17
#define KEY_CTRL	 29     //17
#define KEY_MENU	 56     //18
#define KEY_ALT  	 56     //18
#define KEY_CAPITAL	 20     //58
#define KEY_SPACE	 57     //32
#define KEY_WINKEY	 91   
#define KEY_DELETE   127    //sc 83
#define KEY_F1  0x80
#define KEY_F2  (KEY_F1 + 1)
#define KEY_F3  (KEY_F2 + 1)
#define KEY_F4  (KEY_F3 + 1)
#define KEY_F5  (KEY_F4 + 1)
#define KEY_F6  (KEY_F5 + 1)
#define KEY_F7  (KEY_F6 + 1)
#define KEY_F8  (KEY_F7 + 1)
#define KEY_F9  (KEY_F8 + 1)
#define KEY_F10 (KEY_F9 + 1)
#define KEY_F11 (KEY_F10 + 1)
#define KEY_F12 (KEY_F11 + 1)
//Mouse
#define KEY_MOUSEBUTTON1   5000
#define KEY_MOUSEBUTTON2   5000
#define KEY_MOUSEBUTTON3   5000


//****

//
// Virtua keys.
#define VK_LBUTTON	0x01
#define VK_RBUTTON	0x02
#define VK_CANCEL	0x03
#define VK_POWER    0x03  //??
#define VK_MBUTTON	0x04
#define VK_XBUTTON1	0x05
#define VK_XBUTTON2	0x06
#define VK_CLEAR	0x0C  //12
#define VK_BACK	    0x0E  //14  //8
#define VK_TAB      0x0F  //15  //9
#define VK_KANA	    0x15
#define VK_HANGEUL  0x15
#define VK_HANGUL	0x15
#define VK_JUNJA	0x17
#define VK_FINAL	0x18
#define VK_HANJA	0x19
#define VK_KANJI	0x19
#define VK_ACENTO_AGUDO 0x1A  //26 //0xb4 abnt	
#define VK_ACENTO_GRAVE 0x1A  //26  //0x60	abnt
#define VK_ESCAPE   0x1B  //27 
#define VK_RETURN   0x1C  //28  //13    //Return.
#define VK_CONVERT	0x1C  //??
#define VK_CONTROL     0x1D  //29
#define VK_CTRL        0x1D  //29   
#define VK_LCONTROL	   0x1D  //0xA2
#define VK_NONCONVERT  0x1D  //??
#define VK_ACCEPT      0x1E
#define VK_MODECHANGE  0x1F
#define VK_ACENTO_TIL  0x28  //40 //0x7e	abnt
#define VK_LSHIFT	   0x2A  //0xA0 
#define VK_PRINT	   0x2A  //42 ??
#define VK_EXECUTE	   0x2B  //43
#define VK_SNAPSHOT    0x2C  //44
#define VK_VIRGULA     0x33       //51 abnt
#define VK_PONTO       0x34       //52 abnt
#define VK_PONTOVIRGULA  0x35  //53 abnt
#define VK_RSHIFT	   0x36  //54  //0xA1
#define VK_ALT         0x38  //56   
#define VK_LMENU	   0x38  //56   //Não mudar.
#define VK_MENU        0x38  //56 
#define VK_RMENU	   0x38  //56   //@todo:Mudar esse
#define VK_ALTGR       0x38  //56 abnt //100 //56
#define VK_SPACE       0x39  //57  //Space.
#define VK_CAPITAL	   0x3A  //58  //20  
#define VK_F1	0x3B  //59    // No DOS é 0x170.  
#define VK_F2	0x3C  //60 
#define VK_F3	0x3D  //61 
#define VK_F4	0x3E  //62 
#define VK_F5	0x3F  //63 
#define VK_F6	0x40  //64 
#define VK_F7	0x41  //65 
#define VK_F8	0x42  //66 
#define VK_F9	0x43  //67 
#define VK_F10	0x44  //68
#define VK_NUMLOCK  0x45  //69 //0x90
#define VK_SCROLL	0x46  //70 //0x91
#define VK_HOME	  0x47  //71


// keyboad arrow keys
#define VK_ARROW_UP     0x48
#define VK_ARROW_LEFT   0x4B
#define VK_ARROW_RIGHT  0x4D
#define VK_ARROW_DOWN   0x50


          
#define VK_END	  0x4F  //79
#define VK_DELETE_TECLADONUMERICO 0x53  //83  
#define VK_F11	  0x57  //87 
#define VK_F12	  0x58  //88    
#define VK_LWIN	  0x5B
#define VK_WINKEY 0x5B  //91    
#define VK_RWIN	  0x5C
#define VK_APPS	  0x5D
#define VK_CONTROL_MENU 0x5D  //93
#define VK_ACENTO_CIRCUNFLEXO 0x5E  //abnt	
#define VK_SLEEP     0x5F
#define VK_NUMPAD0	 0x60
#define VK_NUMPAD1	 0x61
#define VK_NUMPAD2	 0x62
#define VK_NUMPAD3	 0x63
#define VK_NUMPAD4	 0x64
#define VK_BREAK     0x65  //101
#define VK_NUMPAD5	 0x65
#define VK_FIND      0x66  //102
#define VK_NUMPAD6	 0x66
#define VK_UP	     0x67  //103 //72 
#define VK_NUMPAD7	 0x67  //??
#define VK_PRIOR     0x68  //104 //33 (prior).
#define VK_NUMPAD8	 0x68  //??
#define VK_LEFT	     0x69  //105 //75 
#define VK_NUMPAD9   0x69  //??
#define VK_RIGHT     0x6A  //106 //77	
#define VK_MULTIPLY  0x6A  //??
#define VK_SELECT	 0x6B  //107 //41
#define VK_ADD       0x6B  //??
#define VK_DOWN	     0x6C  //108 //80
#define VK_SEPARATOR 0x6C  //??
#define VK_NEXT	     0x6D  //109 //34
#define VK_SUBTRACT  0x6D  //??
#define VK_INSERT	 0x6E  //110 //45
#define VK_DECIMAL	 0x6E  //??
#define VK_REMOVE    0x6F  //111
#define VK_DIVIDE	 0x6F  //??
#define VK_HELP	     0x73  //115 //47
#define VK_PAUSE  0x77  //119 //19 
#define VK_F13	0x7C
#define VK_F14	0x7D
#define VK_F15	0x7E
#define VK_F16	0x7F
#define VK_DELETE  0x7F  //127 (delete)
#define VK_F17	0x80
#define VK_F18	0x81
#define VK_F19	0x82
#define VK_F20	0x83
#define VK_F21	0x84
#define VK_F22	0x85
#define VK_F23	0x86
#define VK_F24  0x87
#define VK_RCONTROL         0xA3  //0x1D  //0xA3
#define VK_BROWSER_BACK     0xA6
#define VK_BROWSER_FORWARD  0xA7
#define VK_ACENTO_TREMA     0xA8  //abnt
#define VK_BROWSER_REFRESH  0xA8  //??
#define VK_BROWSER_STOP	    0xA9
#define VK_BROWSER_SEARCH   0xAA
#define VK_BROWSER_FAVORITES  0xAB
#define VK_BROWSER_HOME	0xAC
#define VK_VOLUME_MUTE	0xAD
#define VK_VOLUME_DOWN	0xAE
#define VK_VOLUME_UP	0xAF
#define VK_MEDIA_NEXT_TRACK	 0xB0
#define VK_MEDIA_PREV_TRACK  0xB1
#define VK_MEDIA_STOP  0xB2
#define VK_MEDIA_PLAY_PAUSE	0xB3
#define VK_LAUNCH_MAIL  0xB4
#define VK_LAUNCH_MEDIA_SELECT  0xB5
#define VK_LAUNCH_APP1	0xB6
#define VK_LAUNCH_APP2  0xB7
#define VK_OEM_1	  0xBA
#define VK_OEM_PLUS	  0xBB
#define VK_OEM_COMMA  0xBC
#define VK_OEM_MINUS  0xBD
#define VK_OEM_PERIOD 0xBE
#define VK_OEM_2	  0xBF
#define VK_OEM_3	  0xC0
#define VK_OEM_4	  0xDB
#define VK_OEM_5	  0xDC
#define VK_OEM_6	  0xDD
#define VK_OEM_7	  0xDE
#define VK_OEM_8	  0xDF
#define VK_OEM_102	  0xE2
#define VK_PROCESSKEY 0xE5
#define VK_PACKET	  0xE7
#define VK_ATTN	      0xF6
#define VK_CRSEL	  0xF7
#define VK_EXSEL	  0xF8
#define VK_EREOF	  0xF9
#define VK_PLAY	      0xFA
#define VK_ZOOM	      0xFB
#define VK_NONAME	  0xFC
#define VK_PA1	      0xFD
#define VK_OEM_CLEAR  0xFE
//#define VK_
//#define VK_
//#define VK_
//#define VK_


//****



 
// 
// == Window Type ==
//

#define WT_NULL          0 
#define WT_SIMPLE        1
// igual simples, mais uma bordinha preta.
#define WT_EDITBOX       2
// sobreposta(completa)(barra de titulo + borda +client area)  
#define WT_OVERLAPPED    3  
// um tipo especial de sobreposta,
//usada em dialog ou message box. (com ou sem barra de titulo ou borda)
#define WT_POPUP         4  
#define WT_CHECKBOX      5  // Caixa de seleção. Caixa de verificação. Quadradinho.
#define WT_SCROLLBAR     6  // Cria uma scroll bar. Para ser usada como janela filha.
#define WT_EDITBOX_MULTIPLE_LINES 7
#define WT_BUTTON     8   
#define WT_STATUSBAR  9
#define WT_ICON       10
//... 


//apresentação.
#define VIEW_NULL      0
#define VIEW_FULL      1000
#define VIEW_MAXIMIZED 1001
#define VIEW_MINIMIZED 1002
#define VIEW_NORMAL    1003 //Normal (restaurada)
//...

//botões
#define BN_CLICKED  200
#define BN_DOWN     1
#define BN_UP       2
#define BN_SELECTED 3
//...


//
// ## botoes  ##
//

//button state
#define BS_NULL 0 
#define BS_DEFAULT 1
#define BS_FOCUS   2
#define BS_PRESS   3
#define BS_HOVER   4
#define BS_DISABLED 5
#define BS_PROGRESS 6
//...




typedef enum {
	
	WAIT_REASON_NULL,
	WAIT_REASON_LOOP,           
	WAIT_REASON_EXIT,
	WAIT_REASON_BLOCKED,
	WAIT_REASON_PREMMPTED,      // ?? Esperando a preempção de thread de menor prio.
	WAIT_REASON_SEMAPHORE,      // ?? Semáforo.
	WAIT_REASON_WAIT4PID,       // Esperando o processo filho morrer.
	WAIT_REASON_WAIT4TID,       // Esperando uma thread morrer.
	WAIT_REASON_TEST            // # Usada pelo desenvolvedor para testes.

	//continua... @todo
}thread_wait_reason_t;


//
// object support
//


typedef enum {
	
	// **** EXECUTIVE  ****
	
	//ram
	//Nothing for now.	
    ObjectTypeNull,               //0 Null.
	ObjectTypeASpace,             //1 Address Space. (memory address)
	ObjectTypeDSpace,             //2 Disk Space.
	ObjectTypeBank,               //3 Bank. (banco de dados).
	//sm
	ObjectTypeChannel,            //4 executive sm.
	ObjectTypeDirectory,          //5 diretório de arquivos. executive sm.
	ObjectTypeHeap,               //6 heap executive sm
	ObjectTypeIoBuffer,           //7 i/o buffer executive sm
	ObjectTypeIpAddress,          //8 ip address executivesm.
	ObjectTypeProcessMemoryInfo,  //9 process memory info, executive sm.
	ObjectTypePhysicalMemoryInfo, //10 physical memory info, executive sm.
	ObjectTypeMemoryInfo,         //11 memory info, executive sm.
	ObjectTypeMemoryBlock,        //12 ????? @todo: Ver se esse pode ser Criado sem prejudicar a alocação dinâmica. executive sm mm.	
	ObjectTypePageDirectory,      //13 page directory. executive sm, mm.	
	ObjectTypePageTable,          //14 pagepool , page table, executive ,sm,mm.	
	ObjectTypePageFrame,          //15 page frame ,executive ,sm,mm.
	ObjectTypeFramePool,          //16
	ObjectTypeSocket,             //17 sockets, executive, sm.
    ObjectTypeUserInfo,           //18 userinfo, executive, sm.
	//uigm 
	ObjectTypeDesktop,     //19 desktop, executive,uigm.	
	ObjectTypeMenuItem,        //20 menuitem, executive uigm.
	ObjectTypeMenu,        //21 menu, executive uigm.	
	ObjectTypeUserSession, //22 User session, executive, uigm.	
	ObjectTypeWindowProcedure,  //23 window procedure, executive, uigm.	
	ObjectTypeRectangle,        //24 rectangle, executive, uigm. 	
	ObjectTypeRGBA,             //25 rgba, executive, uigm. 
	ObjectTypeWindow,           //26 window, executive ,uigm.
	ObjectTypeWindowStation,  //27 desktop pool. window station, executive, uigm.
	//uitm.
	ObjectTypeConsole,        //28 console, executive, uitm.
    ObjectTypeLine,           //29 console line, executive, uitm.
	ObjectTypeTerminal,       //30 Terminal, executive, uitm.
	
    // **** MICROKERNEL  ****
	
	//microkernel
	ObjectTypeProcess,  //31 Process, microkernel.
	ObjectTypeSemaphore,  //32 semaphore, microkernel.	
	ObjectTypeThread,     //33 thread, microkernel.
	
	//io 
	//@todo: isso deverá ser criado aqui.
	ObjectTypeCpu,  //34
	ObjectTypeDma,  //35
	
	// **** HAL  ****
	
	ObjectTypeProcessor,  //36 processor, hal (talvez isso devesse ir para io de microkernel.)
	
	//device(unblocked e blocked).
	ObjectTypeDevice,      //37 device, hal
	ObjectTypeDiskInfo,    //38 disk info, hal.
	ObjectTypeProcessorBlock, //39 processor block hal...(pode ir para io,microkernel)
    ObjectTypeHostInfo,  //40 HostInfo, hal. 
	ObjectTypeKeyboard,   //41 keyboard, hal.
	ObjectTypeMacAddress,  //42 MAC Address, hal.
	ObjectTypeMemory,      //43 memory card info, hal.
	ObjectTypePciDevice,   //44 PCI device info, hal.
	ObjectTypePciDeviceDriver,   //45 PCI device driver info, hal.
	ObjectTypePIC, //46 pic controller, hal.
	ObjectTypeRTC, //47 rtc controller, hal.
	ObjectTypeScreen, //48 Screen, hal.
	ObjectTypeTimer,    //49 ?? timer ,hal.	
	ObjectTypeVideo,    //50 video ,hal.
	ObjectTypeVolume,    //51 disk volume ,hal.
	
	//things
	//@todo:
	
	// **** OUTROS  ****
	ObjectTypeCursor,    //52 /include.
	ObjectTypeRequest,   //53 request de kernel /include.
	ObjectTypeSignal,    //54
	ObjectTypeFile,      //55 file, /include. stdio.
	ObjectTypeMetafile, //56 
	ObjectTypeMutex,    //57
	ObjectTypeToken,    //58 Token de acesso à objetos. (access token)
	ObjectTypeFont,    //59
	ObjectTypeIcon,     //60
	ObjectTypePipe,     //61
	
	ObjectTypeGroup,     //62
	ObjectTypeUser,      //63
	
	ObjectTypeComputer,      //64
	ObjectTypeCpuRegister,   //65
	
	
	ObjectTypeDisk,   //66
	ObjectTypeDiskPartition,   //67 //info struct
	ObjectTypeGDT,   //68
	ObjectTypeLDT,   //69
	ObjectTypeIDT,   //70
	ObjectTypeTSS,   //71
	
	ObjectTypePort,  //72 (i/o port)
	ObjectTypeController, //73
	ObjectTypeKM, //74
	ObjectTypeUM, //75
	ObjectTypeColorScheme,  //76
	ObjectTypeButton,       //77
	ObjectTypeGrid,       //78
	//...
}object_type_t;

//
// Enumerando classes de objetos. (categorias)
//
typedef enum {
	ObjectClassKernelObjects,  // Kernel Objects.
	ObjectClassUserObjects,    // User Objects.
	ObjectClassGuiObjects,     // Graphical User Interface Objects.
	ObjectClassCliObjects,     // Command Line Interface Objects.
    //...	
}object_class_t;



// socket structure for the grass api.
// It is not the same of the kernel.
struct socket_d
{
    object_type_t  objectType;
    object_class_t objectClass;

    unsigned long ip_long;
    unsigned short port;

	//...
};


/*
 * ApplicationInfo_d:
 *     Informações básicas sobre uma applicação.
 *     Talvez o nome poderia ser ApplicationHook. 
 */

struct ApplicationInfo_d
{
    int pid;       //Process Id.
    char *name;    //Name.
    //...
};
struct ApplicationInfo_d *ApplicationInfo;
// 


/*
 * buffer_info_d:
 *     Informações básicas sobre um buffer.
 */

struct buffer_info_d
{    
    unsigned long address;
	//...
};
struct buffer_info_d *BufferInfo;
//

// Estruturas para recursos gráficos:
//     Aqui deve ficar apenas as estruturas básicas que o cliente    
// pode acessar através da API. Haverá uma biblioteca exclusivamente 
// para recursos gráficos. Obs: Não pode haver conflito quando o
// cliente incluir a API e a biblioteca de recursos grpaficos.
  

// Client area info.  
struct client_area_info_d 
{    
    struct window_d *window;
    struct rect_d *rect;
    int dummy;
};
struct client_area_info_d *ClientAreaInfo;
//


// Cursor info.
struct cursor_info_d 
{    
    unsigned long x;
    unsigned long y;
    unsigned long width;
    unsigned long height;
    unsigned long image_address;  //x.cur
	//...
};
struct cursor_info_d *CursorInfo;
//


/*
 **************************************************
 * rect_d:
 *     Estrutura para gerenciamento de retângulos.
 *     Um retângulo pertence à uma janela.
 */

// #todo
// Usar isso para lidar com dirty rectangles.

struct rect_d 
{
	//object_type_t objectType;
	//object_class_t objectClass;

    int used;
    int magic;

    int flag;
	
	//estilo de design
    int style;
	
	//int validated;
	//int focus;
	
	//int dirty;
	
    unsigned long x;
    unsigned long y;
    unsigned long cx;
    unsigned long cy;

    unsigned long width;
    unsigned long height;

    unsigned long left;
    unsigned long top;
    unsigned long right;
    unsigned long bottom;

    unsigned long bg_color; //color_bg;
	
	//Essa é  ajanela à qual o retângulo pertence.
    struct window_d *window;
	
    struct rect_d *next;
};
//struct rect_d *rect;

//
// Window support.
//


/*
 *******************************************************
 * window_d:
 *     Window Structure.
 *     @todo: *Deve ficar igual a do kernel.
 *     Obs: Não sei se essa estrutura pode ser pública.
 *     #bugbug: isso ainda não está igual a do kernel, 
 * pois ainda está em transformação e não temos nenhuma 
 * versão definida.
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     Oct 2016 - Revision.
 *     ...
 */

// #todo
// Temos que atualizar essa estrutura para ficar igual
// a que está em ring0.

// Deve estar em conformidade com a estrutura em user mode.
//=========================================================
 
struct window_d
{
    // Identificadores.
    object_type_t  objectType;
    object_class_t objectClass;


	//object control
    struct object_d *object;


    unsigned long id;    //Window Id. 
    char *name;          //Window name.
    unsigned long type;  //tipo ... (editbox, normal, ...)  style???


	//Segurança.
    int used;
    int magic;


	// Características dessa janela..

	//Estado: (Full,Maximized,Minimized...)
    int view; 


	//dimensões e margens.
	
	//#bugbug 
	//Esses conceitos estão sob revisão.
	//Estou aprendendo ...
    
	//+margem é o deslocamento até encontrar a borda.
	//+padding é o deslocamento entre a borda e a janela.

    unsigned long left;        //margem esquerda 
    unsigned long top;         //margem superior
    unsigned long right;       //margem direita  
    unsigned long bottom;      //margem inferior  


    unsigned long width; 
    unsigned long height; 

	//??
	//deslocamento dentro da janela?
    unsigned long x;           //deslocamento x
    unsigned long y;           //deslocamento y 
    int z;   //z order.


    // Background color.
    unsigned long color_bg;   //deletar isso
    unsigned long bg_color;
//==================================================

    // Alertartando que exite uma mensagem para essa janela.
    int msgAlert;  

    //?? rever
    int sendOffset;
    int receiveOffset;


	//Filas para uma janela receber mensagens.
	//São os quatro argumentos de um procedimento de janela padrão.

	//
	// CURRENT MESSAGE !!!
	// 

	//shared by producer and consumer 
    struct window_d *msg_window;  //arg1.
    int msg;                      //arg2.
    unsigned long long1;          //arg3.
    unsigned long long2;          //arg4.

    //O aplicativo depois de pegar os 4 elementos, autoriza o 
    //kernel a colocar uma nova mensagem.
    //'0' indica que não temos uma mensagem válida.
    //'1' indica que temos uma nova mensagem.
	int newmessageFlag;	

    // MESSAGE QUEUE !!!
    //shared by producer and consumer
    unsigned long hwndList[32];
    unsigned long msgList[32];
    unsigned long long1List[32];
    unsigned long long2List[32];

    //struct msg_d *msg;

	// procedure support. 
	//(poderia ser um argumento ou elemento da classe.)
    unsigned long procedure;               //procedimento da janela
    struct window_procedure_d *wProcedure; //procedure struct	

//==================================================
    int active;    //FAST FLAG. Essa será a flag de ativa ou não. (decidindo isso)
    int focus;     //Se tem o foco de entrada ou não.


	// Parent support
    unsigned long parentid;       //(Número da janela mãe).
    struct window_d *parent;      //Parent window.


	// Child support.
    struct window_d *childListHead;  //Lista encadeada de janelas filhas.
    int childCount;                  //Tamanho da lista.

//==================================================

	// Client window support.

	// É a janela propriamente dita, 
	// excluindo a moldura e a barra de rolagem.
	// Tudo o que há dentro da janela menos o frame.
	// É a parte que é exibida quando a janela está em full screen.
    struct window_d *client_window; 
    struct rect_d *rcClient;     

	//cor do retângulo da área do cliente.
    unsigned long clientrect_color_bg;     //deletar esse.

//cor do retângulo da área do cliente.
    unsigned long clientrect_bg_color;    

//==================================================

    // Cursor.
    unsigned long CursorX;
    unsigned long CursorY;
    unsigned long CursorColor;

//==================================================

// Bars support.
	// Cada tipo de janela tem seus itens específicos.
	// Esses são os status dos ítens. Se eles estão presentes ou não.

    int backgroundUsed;
    int shadowUsed;
    int titlebarUsed;
    int menubarUsed; 
    int toolbarUsed;
    int clientAreaUsed;
    int statusbarUsed;
    int scrollbarUsed;
    int minimizebuttonUsed;
    int maximizebuttonUsed;
    int closebuttonUsed;
    int borderUsed;
	// ...

//==================================================

	// Buffer.
	// DedicatedBuffer
	// DedicatedBuffer --> LFB.
	// Endereço de memória onde a janela foi pintada.
	// Obs: Toda janela deve ter seu próprio buffer de pintura para poder 
	// passar a janela direto de seu próprio buffer para o LFB, sem passar 
	// pelo Backbuffer.

    void *DedicatedBuffer;        //Qual buffer dedicado a janela usa.
    void *BackBuffer;    //Qual backbuffer a janela usa.
    void *FrontBuffer;   //Qual frontbuffer a janela usa. (LFB).

//==================================================

	// Desktop support.
	// A que desktop a janela pertence??

    int desktop_id;
    struct desktop_d *desktop; 

// Gramado v 0.1 usa essa estrutura somente até aqui.
//==================================================

//
// Update 
//

//==================================================

	// Recomeçaremos com a relação entre janelas.
    struct window_d *owner;
    struct window_d *child; 

    //Configuramos isso quando efetuamos um raise_window
    //colocando a janela acima das outras.
    struct window_d *child_with_focus; 

//==================================================

// scroll bar
    struct window_d *scrollbar;
    struct window_d *scrollbar_button1; // ^
    struct window_d *scrollbar_button2; // =
    struct window_d *scrollbar_button3; // V
    
    int isScrollBarButton1;
    int isScrollBarButton2;
    int isScrollBarButton3;

//==================================================

    struct window_d *statusbar;

//==================================================

    struct window_d *toolbar;

//==================================================

    // Buffer para mensagens pequenas.
    // Será usado pelo produtor e pelo consumidor.
    // char read_buf[WINDOW_MSG_BUFFER_SIZE];

//==================================================

	// Cursor.
	// Posição do cursor para texto dentro da janela.
	// Obs: Deve ser multiplo da altura e largura do caractere.
	// Obs: Para cada janela o cursor pode estar em uma posição diferente.
	// @todo: Deletar isso e começar usar a estrutura.

    unsigned long cursor_icon;
    unsigned long cursor_x;
    unsigned long cursor_y;

//==================================================

	// Window Class
    struct window_class_d *window_class;
    
//==================================================

    //unsigned long scancodeList[32];

//==================================================

	// ID da tty usada.
    int tty_id;

//==================================================

	//
	// TERMINAL SUPPORT
	//

    // Obs: 
    // Essas variáveis só serão inicializadas se o 
    // aplicativo decidir que conterá um terminal em sua janela.
    // Um aplicativo só poderá ter um terminal dentro de cada janela.
    // Ou seja, se um aplicativo quiser ter mais de um terminal virtual, 
    // vai precisar de uma janela para cada terminal dentro do aplicativo.
    // isso permite facilmente que um mesmo aplicativo rode vários
    // programas, um em cada aba.
    // Ao invés de criar um frame para cada aplicativo que rode em terminal,
    // é só criar uma nova aba no gerenciador de terminais virtuais ...
    // esse gerenciador de terminais virtuais poderia ser o shell.bin


	//flags
	
	//configura o status do terminal dentro da janela
    int terminal_used;     //Se é um terminal ou não.
	
	//validade e reusabilidade das variáveis de terminal 
	//dentro da estrutura de janela.
    int terminal_magic;
	
	//tab
	//número da tab.
	//indica qual tab o terminal está usando.
	//@todo:
	// Criar uma forma de contar as tabs de terminal 
	// dentro do gerenciador de terminais.
    int terminal_tab; // em qual tab do gerenciador de terminais está o terminal.
	
	//rect
    unsigned long terminal_left;
    unsigned long terminal_top;
    unsigned long terminal_width;
    unsigned long terminal_height;
	
    unsigned long terminal_right;
    unsigned long terminal_bottom;

	//...
	
	//@todo: isso deve pertencer a uma janela.
	//se uma janela tiver o foco de entrada e for um terminal 
	//a disciplica de linhas poderá usar essas carcterística do terminal.
    struct terminal_d *wTerminal; //terminal.h

//==================================================
 
    // ??
    int tab;

//==================================================

    // style: 
    // Isso poderia ser estilo de design ...
    // Qualquer janela pode ter vários estilos de design 
    // ex: um editbox poderá ser de vários estilos.	

	//window style:
	//WINDOW_STYLE_FLOATING (flutuante) 
	//WINDOW_STYLE_DOCKING   (atracada em algum canto)
    int style;   

//==================================================

    //unsigned long Background;

//==================================================

    //int instance; 

//==================================================


	//*full screen mode = modo tela cheia. 
	//( utiliza a resolução atual do dispositivo )
	// deve ser a janela em primeiro plano. acima de todas as outras.
	//mas podemos configurar para que uma jenela esteja em full screen 
	//enquanto outra janela é a janela ativa e ainda outra tenha o foco de entrada.
	//uma janela em modo full screen pode conter barras de rolagem.
	//*embedded mode = dentro de uma janela ou de um navegador. 

    //unsigned long status;              //ATIVA OU NÃO.
    
//==================================================    

    // Seu estatus de relacionamento com outras janelas.
    unsigned long relationship_status;   

//==================================================    

	//ordem na pilha de janelas do eixo z.
	//A janela mais ao topo é a janela foreground.
    int zIndex;    

	//z-order global.
	//Sua ordem em relação a janela gui->main.    
    struct zorder_d *zorder;

//==================================================    

    //Qual buffer dedicado a janela usa.
    //void *buffer;        

//==================================================    

	//?? Se mudar para Rect pode deletar alguns elementos acima
	//como x, y, width ...
    struct rect_d *rcWindow;

	//Lista encadeada dos retângulos que pertencem a essa janela.
	//Quando uma janela for deletada, devemos desalocar toda a memória 
	//uada por esses recursos.
    struct rect_d *rectList;

//==================================================    

	//
	// CLists
	//

	// Um ponteiro para um array de ponteiros de estruturas de linhas
	// Explicando: 
	// Nesse endereço teremos um array. Cada ponteiro armazenado
	// nesse array é um ponteiro para uma estrutura de linha.
	// Obs: 
	// #todo: 
	// Todos esses elementos podem formar uma estrutura e ficaria aqui 
	// apenas um ponteiro para ela.

    void *LineArray;
    int LineArrayIndexTotal;    //Total de índices presentes nesse array.
    int LineArrayIndexMax;      //Número máximo de índices permitidos no array de linhas.
    int LineArrayTopLineIndex;  //Indice para a linha que ficará no topo da página.
    int LineArrayPointerX;      //Em qual linha o ponteiro está. 	
    int LineArrayPointerY;      //em qual coluna o ponteiro está.
	//...

//==================================================    

	//Usada para input de mensagens e sinais.
    struct thread_d *control;

//==================================================    

    //  A que processo a janela pertence??
    struct process_d *process;

//==================================================    

	//
	// Menus support.
	//

	//?? Qual janela o menu usará.
    struct window_d *menu_window;   //Menu Window.

    struct menu_d *sysMenu;         //menu de sistema.(control menu)
    struct menu_d *barMenu;         //menu da barra de menu.
    struct menu_d *defaultMenu;     //menu da janela (*importante)
	//...

    int selected;     //seleção  de item de menu.

//==================================================    

    // Flag par indicar se a janela é um item de menu ou um botão.
    int isMenu;   
    int isMenuItem;
    
    int isControl;  // Window control ...
    int isButton;  //#importante: Indica que a janela é um botão.
    int isEditBox; //#importante: Indica que a janela é um editbox.
    int isIcon;
	//...

//==================================================    

	// Se a janela for do tipo botão, então essa será a 
	// estrutura para o gerenciamento do botão.
    struct button_d *button;
    
//==================================================    

    // ??
    const char *text; 

//==================================================    

    struct window_d *minimize;
    struct window_d *maximize;
    struct window_d *close;
    
 //#teste
    int isMinimize;
    int isMaximize;
    int isClose;

//==================================================    

    // Qual janela é um ícone para essa janela.
    // Por exemplo, o ícone que fica na área de trabalho.
    struct window_d *icon;

//==================================================    

    //
    // Actions.
    //

    int draw; 
    int redraw;
    int show;   //se precisa ou não mostrar a janela.	
    // Continua ...

//==================================================    

	// Text Cursor support.
	// fica para uma versão estendida da estrutura.
	// Estrutura de cursor para a janela.
    struct cursor_d *cursor;

//==================================================    

	//unsigned long bgcolor;		// Background color.
	//unsigned long fgcolor;		// Foreground color. 

//==================================================    

    struct button_d *current_button;  //Botão atual.      
    struct button_d *buttonList;      //Lista encadeada de botões em uma janela.

//==================================================    

	// ??
	// Status do puxador da janela.
	// Se está aberta ou não.
	// HANDLE_STATUS_OPEN ou HANDLE_STATUS_CLOSE
	int handle_status;

//==================================================

	// Trava. Se travada, não pode mudar nada.
	// Enables or disables mouse and keyboard input to the 
	// specified window or control.
    int locked; 

//==================================================

	// Navegation.
    struct window_d *prev; 
    struct window_d *next; 
};

struct window_d *CurrentWindow;
//...


/*
 * semaphore_d:
 *     Estrutura para criação de um semáforo.        
 *
 *     Pode-se criar vários semáforos. Cada semáforo é usado para
 * controlar o fluxo em um determinado recurso.
 *  Obs: Aqui em user mode talvez não precise criar a estrtutura inteira somente o handle.
 */ 

struct semaphore_d 
{
    int used;
    int magic; 

    int id;

    int taskId;

    // Estado do semáforo. (verde, vermelho)
    int color; 
    int status; 
    unsigned int count;

    //...

    struct d_wait_queue *sema_wait; 
    
    // ...
};
struct semaphore_d *current_semaphore;
// ...


//
// BMP support
//

// "MB"
#define BMP_TYPE 0x4D42

//OFFSETS
#define BMP_OFFSET_WIDTH      18
#define BMP_OFFSET_HEIGHT     22
#define BMP_OFFSET_BITPLANES  26
#define BMP_OFFSET_BITCOUNT   28
//...

struct bmp_header_d 
{
    unsigned short bmpType;           /* Magic number for file */
    unsigned long  bmpSize;           /* Size of file */
    unsigned short bmpReserved1;      /* Reserved */
    unsigned short bmpReserved2;      /* ... */
    unsigned long  bmpOffBits;        /* Offset to bitmap data */
};

struct bmp_infoheader_d  
{
    unsigned long  bmpSize;           /* Size of info header */
    unsigned long  bmpWidth;          /* Width of image */
    unsigned long  bmpHeight;         /* Height of image */
    unsigned short bmpPlanes;         /* Number of color planes */
    unsigned short bmpBitCount;       /* Number of bits per pixel */
    unsigned long  bmpCompression;    /* Type of compression to use */
    unsigned long  bmpSizeImage;      /* Size of image data */
    unsigned long  bmpXPelsPerMeter;  /* X pixels per meter */
    unsigned long  bmpYPelsPerMeter;  /* Y pixels per meter */
    unsigned long  bmpClrUsed;        /* Number of colors used */
    unsigned long  bmpClrImportant;   /* Number of important colors */
};
 

 
 /*
 *******************************************************
 * timer_d:
 * Estrutura do objeto timer que será usado pelos aplicativos.
 * 
 * Precisamos identificar quem está usando para podermos enviar 
 * mensagem para quem possui o timer.
 *
 */

struct timer_d 
{
    // Object.
    object_type_t  objectType;
    object_class_t objectClass;


    int id;

    int used;
    int magic;

    // 1 = one shot 
    // 2 = intermitent
    int type;


    // Owner.
    struct process_d *process;
    struct thread_d *thread;
    struct window_d *window;


    int count_down;     //--
    int initial_count_down; //base fixa

    //quantas vezes o timer se esgotou.
    unsigned long times;

    int status;

    unsigned long flag; 
    unsigned long error; 


    struct timer_d *next;
};


//
// == Prototypes ====================================================
//


// Execute a command.
int gde_system (const char *command);


/*
 ******************************************************
 * system_call: 
 *     Interrupção de sistema, número 0x80, 
 * pode chamar vários serviços do kernel com a 
 * mesma interrupção. O número do serviço é passado via 
 * argumento.
 *     Essa é a chamada mais simples.
 *
 * Argumentos:
 *    eax = arg1, o Número do serviço.
 *    ebx = arg2. 
 *    ecx = arg3.
 *    edx = arg4.
 */

void *system_call ( 
    unsigned long ax, 
    unsigned long bx, 
    unsigned long cx, 
    unsigned long dx );


/*
 ******************************************************
 * gde_system_procedure:
 *     Chama o procedimento de janela padrão do sistema.
 *     Ele está no kernel.
 */

void *gde_system_procedure ( 
    struct window_d *window,
    int msg,
    unsigned long long1,
    unsigned long long2 );
 
 
//Carrega bitmap 16x16.
void 
gde_load_bitmap_16x16 ( 
    unsigned long img_address, 
    unsigned long x, 
    unsigned long y );


// Load path
int 
gde_load_path ( 
    const char *path, 
    unsigned long buffer, 
    unsigned long buffer_len );


//int 0x80 - serviço 241.
void gde_init_background (void);

// #todo: Change type to 'const char *'
int gde_message_box ( int type, char *string1, char *string2 );

// #todo: Change type to 'const char *'
int gde_dialog_box ( int type, char *string1, char *string2 );


//
// ## Window support ##
//


/*
 **********************************************************
 * gde_create_window: 
 *     Cria uma janela com base em uma struct.
 *     Retorna o endereço da estrutura da janela criada. 
 *     Para que possa ser registrada na lista windowList[].
 *
 * Cria a janela dependendo do tipo:                              
 * =================================
 * 1 - POP-UP.
 * 2 - EDIT-BOX.
 * 3 - NORMAL ( bg + client area ).
 * 4 - NORMAL_EDIT ( bg + editbox).
 * 5 - MENUBAR.
 * 6 - TOOLBAR.
 * 7 - STATUSBAR.
 * 8 - TASKBAR.
 * 9 - ? 
 * CONTINUA ...
 */

// Create a window using kgws.

// #todo: Change windowname type to 'const char *'

void *gde_create_window ( 
    unsigned long type,        //1-Tipo de janela, (popup,normal,...).
    unsigned long status,      //2-Estado da janela, (ativa ou não).
    unsigned long view,        //3-(min, max ...).
    char *windowname,          //4-Título.  
    unsigned long x,           //5-Deslocamento em relação às margens do Desktop. 
    unsigned long y,           //6-Deslocamento em relação às margens do Desktop.
    unsigned long width,       //7-Largura da janela.
    unsigned long height,      //8-Altura da janela.
    struct window_d *pWindow,  //9-Endereço da estrutura da janela mãe.
    unsigned long onde,        //10-Ambiente.( Estão no desktop, barra, cliente ...)
    unsigned long clientcolor, //11- Cor da área de cliente.
    unsigned long color );     //12-Color (bg) (para janela simples).


//Register Window.
int gde_register_window (struct window_d *window);

//Close Window.
int gde_close_window (struct window_d *window);

//Set Focus.
int gde_set_focus (struct window_d *window);


// Get Focus.
// Get the pointer for the window with focus.
struct window_d *gde_get_focus (void);

//Kill Focus.
int gde_kill_focus (struct window_d *window);

//Set Active Window.
int gde_set_active_window (struct window_d *window);

//Get Active Window.
struct window_d *gde_get_active_window (void);

//Client area.
void *gde_get_client_area_rect (void);
void gde_set_client_area_rect (struct rect_d *r);


//resize
void 
gde_resize_window ( 
    struct window_d *window, 
    unsigned long x, 
    unsigned long y );


//redraw
//flag= show??
void 
gde_redraw_window ( 
    struct window_d *window, 
    unsigned long flags );



//replace
void 
gde_replace_window ( 
    struct window_d *window, 
    unsigned long x, 
    unsigned long y );


// torna visível uma janela.
void gde_show_window (struct window_d *window);


// max and min
void gde_maximize_window (struct window_d *window);
void gde_minimize_window (struct window_d *window);


// ?? rever isso.
// Envia uma mensagem PAINT para o aplicativo atualizar a 
// área de trabalho.

void gde_update_window (struct window_d *window);


// foreground window.
void *gde_get_foreground_window (void);
void gde_set_foreground_window (struct window_d *window);


//
// Char support.
//

void 
__gde_put_char ( 
    unsigned long x,  
    unsigned long y,  
    unsigned long color, 
    unsigned char *ch );


//
// String support
//


// compare
// Na libcore tem muitas funções da libc
// vamos fazer isso com elas e incorporá-las na api
// sempre que possível.

int gde_strncmp (char *s1, char *s2, int len);


/*
 ***************************************
 * gde_print_string:
 *     int 0x80 - serviço 10.
 */

void 
gde_print_string ( 
    unsigned long x,  
    unsigned long y,  
    unsigned long color, 
    unsigned char *string );


//
// Buffer support.
//

//int 0x80 - serviços - 1,2,3,4,5,6,7,8,9.
void gde_refresh_buffer (unsigned long n);



//
// Screen support.
//


// refresh backbuffer.
// Copy the backbuffer in the lfb.
void gde_show_backbuffer (void);

//int 0x80 - serviço 15. 
int gde_vsync (void);


// Reboot support.
void gde_reboot (void);

// Shutdown support.
void gde_shutdown (void);



//
// Cursor support.
//

void gde_set_cursor ( unsigned long x, unsigned long y );
void gde_get_cursor ( unsigned long *x, unsigned long *y );
unsigned long gde_get_cursor_x(void);
unsigned long gde_get_cursor_y(void);

// set cursor.
int __gde_set_cursor (unsigned long x, unsigned long y);



//
// Process support
//


// Create process.
// #todo: This function needs more parameters.
// #todo: Change name type to 'const char *'
void *gde_create_process ( 
    char *name,
    unsigned long process_priority );

//
// Thread support
//

// Create thread.
// #todo: This function needs more parameters.
// #todo: Change name type to 'const char *'
void *gde_create_thread ( 
    unsigned long init_eip, 
    unsigned long init_stack, 
    char *name );


//Mostra informações sobre o processo atual.
void gde_show_current_process_info (void);

// Torna zombie a thread atual.
void gde_exit (int exit_code);

//Destrói a thread atual.
void gde_kill (int exit_code);

void gde_dead_thread_collector (void);

//coloca no estado standby para executar pela primeira vez.
void gde_start_thread (void *thread); 


//
// File support.
//

// Open file.
void *gde_fopen (const char *filename, const char *mode);


/*
 **********************************************
 * gde_save_file:
 *     Salva um arquivo no diretório raiz do volume de boot.
 */

//size in sectors 
// #todo: Change file_name type to 'const char *'
int
gde_save_file ( 
    char *file_name, 
    unsigned long file_size,  // size in sectors.
    unsigned long size_in_bytes, 
    char *file_address, 
    char flag );
    

int gde_test_save_file(char *file_name);


int gde_create_empty_file (char *file_name);

int gde_create_empty_directory (char *dir_name);

//
// Semaphore support
//

void gde_down (struct semaphore_d *s);
void gde_up (struct semaphore_d *s);

//
// Critical section support.
//

// Inicializa em 1
void gde_initialize_critical_section (void);    

// P (Proberen) testar.
void gde_enter_critical_section (void);
// V (Verhogen) incrementar.
void gde_exit_critical_section (void);
// P (Proberen)
void gde_p (void); 
// V (Verhogen)
void gde_v (void);

// Critical section for paint routines.

void gde_begin_paint (void);
void gde_end_paint (void);

// ====================


/*
 ***********************************************
 * gde_def_dialog:
 *     Simply default procedute call.
 * 
 *     It will call the kernel window procedure ...
 *     #bugbug: In some cases this procedure will run for 
 * the second time.
 *     #bugbug: Actually this is not defered ... 
 *     Procedimento de janela adiado. 
 *     Usado pelos aplicativos ao fim dos seus 
 * procedimentos de janela.
 */

unsigned long 
gde_def_dialog ( 
    struct window_d *window, 
    int msg, 
    unsigned long long1, 
    unsigned long long2 );


// System metrics support.

unsigned long gde_get_system_metrics ( int index );

// ?? what ?
int gde_dialog ( const char *string );



// bmp

int 
gde_display_bmp ( 
    char *address, 
    unsigned long x, 
    unsigned long y );

//
// == IPC ===========================
//

// send message to process
// Envia uma mensagem para a thread de controle de um dado processo.

int 
gde_send_message_to_process ( 
    int pid, 
    struct window_d *window, 
    int message,
    unsigned long long1,
    unsigned long long2 );

// Send message to thread
// Envia uma mensagem para uma thread.
int 
gde_send_message_to_thread ( 
    int tid, 
    struct window_d *window, 
    int message,
    unsigned long long1,
    unsigned long long2 );


// Send message to the current thread.
unsigned long 
gde_send_message ( 
    struct window_d *window, 
    int message,
    unsigned long long1,
    unsigned long long2 );


//
// char support.
//

// put char
void gde_put_char (int c);
// get char
int gde_getchar (void);


// Envia uma string para a porta serial COM1
// #todo: Change type to 'const char *'.
void gde_debug_print (char *string);


//
// text support.
//

int 
gde_draw_char ( 
    unsigned long x, 
    unsigned long y, 
    unsigned long c,
    unsigned long fg_color,
    unsigned long bg_color );

int 
gde_drawchar_transparent ( 
    unsigned long x, 
    unsigned long y, 
    unsigned long color,
    unsigned long c );

int 
gde_draw_text ( 
    struct window_d *window, 
    unsigned long x, 
    unsigned long y, 
    unsigned long color, 
    const char *string );


// ?
struct window_d *gde_get_ws_screen_window (void);
struct window_d *gde_get_ws_main_window (void);


// Create timer;
// IN: pid, ms, type

struct timer_d *gde_create_timer ( 
    int pid, 
    unsigned long ms, 
    int type );


// pega informações varidas sobre o sys time.
unsigned long gde_get_systime_info ( int n );


// #bugbug: rever isso.
// 'Clona' e executa o noraterm como processo filho. 
// registra o terminal noraterm como terminal atual.
// pega o pid do terminal atual
// manda uma mensagem pedindo para o terminal dizer hello.

int gde_start_terminal (void);


// status bar.
// change type to 'const char *'
int 
gde_update_statusbar ( 
    struct window_d *window, 
    unsigned char *string1, 
    unsigned char *string2 );



// Get the PID of some well known registered servers.
// Usada para obter o pid de alguns drivers e servidores
// do sistema. Aqueles servidores que só podem ter um do tipo 
// rodando ao mesmo tempo.

int gde_get_pid (int index);


// The screen window for the kgws display server.
struct window_d *gde_get_screen_window (void);

// The background window for the kgws display server.
struct window_d *gde_get_background_window (void);

// The main window for the kgws display server.
// This is a 'desktop area'. A'area de trabalho'.
struct window_d *gde_get_main_window (void);

//
// process and thread
//

// name
int gde_getprocessname (int pid, char *name, size_t len);
int gde_getthreadname (int tid, char *name, size_t len);

// stats
unsigned long gde_get_process_stats (int pid, int index);
unsigned long gde_get_thread_stats (int tid, int index);

//
// == process support ======================================
//

// Clone and execute a process given it's name.
// It doesn't support pathname yet.
// You can use names in lower case.
// #maybe: You do not need to provide the extension.

// This is the only method that is working for now.
// #todo: Cahnge type to 'const char *'.

int gde_clone_and_execute ( char *name );

// ============================================================


/*
 *************************************************************
 * gde_setup_net_buffer: 
 * 
 */

// #todo
// rever isso.

// Enviamos para o kernel um buffer que está no app em ring3.
// Associando esse buffer ao pid do app.
// Desse modo o driver de rede pode colocar conteúdo no buffer
// do aplicativo que está ouvindo determinada porta.
// Depois mandar uma mensagem para o app dizendo, pode ler seu buffer.
//

// O kernel poderá guardar esse ponteiro para o net_buffer
// na estrutura do processo cujo pid foi passado por essa função.
// Também pode salvar o tamanho do buffer pois ele pode ter
// tamanho padrao. 4KB.

// IN: pid, ponteiro para o buffer, comprimento do buffer;
// OUT: 0=Ok -1=fail

int gde_setup_net_buffer (int pid, char *buffer, size_t len);

// ==============================================


// execute new process.
// cria um novo processo, 
// uma thread e carrega a imagem.
// not tested

int 
execute_new_process ( 
    const char *filename, 
    char *argv[], 
    char *envp[] );

// ==================================

void gde_yield(void);

//
// End.
//

