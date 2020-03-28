/*
 * File: stdio.c
 *
 *     I/O routines support.
 *     c99 - ISO/IEC 9899:1999
 *
 * Environment: ring3.
 *
 * Credits:
 *     printf: https://www.menie.org/georges/embedded/small_printf_source_code.html
 *     stdio_fntos: Luiz Felipe
 *     Serenity OS. (bsd)
 *
 * History:
 *     2015 - Created by Fred Nora.
 *     2020 - A lot of new functions.
 */


// See:
// http://kirste.userpage.fu-berlin.de/chemnet/use/info/libc/libc_7.html 



#include <limits.h>
#include <stdio.h>
#include <types.h> 
#include <stdarg.h> 
#include <stdio_ext.h>
#include <stddef.h>
#include <ctype.h>
#include <fcntl.h>

// System calls.
#include <stubs/gramado.h> 


// Usada por uma função nesse documento. 
char **environ;



// #importante: 
// Tem que inicializar isso.

static int __libc_output_mode = 0;
static int terminal___PID;



// Testando define.

#define  SYSTEMCALL_READ_LBA    1
#define  SYSTEMCALL_WRITE_LBA   2
#define  SYSTEMCALL_READ_FILE   3
#define  SYSTEMCALL_WRITE_FILE  4
// ...


// Why ?
#define  VK_RETURN      0x1C 
#define  VK_BACKSPACE   0x0E 
#define  VK_BACK        0x0E  
#define  VK_TAB         0x0F 



//++
// Internas.
// estamos falando do posicionamento do cursor dentro da janela 
// e não dentro do terminal. 
void stdioSetCursor( unsigned long x, unsigned long y );
unsigned long stdioGetCursorX(); 
unsigned long stdioGetCursorY(); 
static size_t stdio_strlen (const char *s);
//--



// stdio_atoi:
// Talvez isso possa ir para o topo do 
// arquivo para servir mais funções.

int stdio_atoi (char *s){

    int rv=0; 
    char sign = 0;

    // Skip till we find either a digit or '+' or '-'.
    
    while (*s) 
    {
        if (*s <= '9' && *s >= '0')
            break;


        if (*s == '-' || *s == '+') 
            break;


        s++;
    };


    if (*s == '-')
        sign=1;


    //     sign = (*s == '-');
    if (*s == '-' || *s == '+') 
        s++;


    while (*s && *s >= '0' && *s <= '9') 
    {
        rv = (rv * 10) + (*s - '0');
        s++;
    };


    if (sign){ 
        return (-rv);

    }else{ 
        return (rv);
    };

    // return (sign ? -rv : rv);
}


// stdio_fntos:
// rotina interna de support.
// isso deve ir para bibliotecas depois.
// não tem protótipo ainda.
// Credits: Luiz Felipe

void stdio_fntos (char *name){

    int  i, ns = 0;

    char ext[4];


    ext[0] = 0;
    ext[1] = 0;
    ext[2] = 0;
    ext[3] = 0;


    //const char ext[4];

    //Transforma em maiúscula enquanto não achar um ponto.

    while ( *name && *name != '.' )
    {
        if ( *name >= 'a' && *name <= 'z' )
            *name -= 0x20;

        name++;
        ns++;
    };


    // Aqui name[0] é o ponto.
    // Então constrói a extensão.

    for ( i=0; i < 3 && name[i+1]; i++ )
    {
        //Transforma uma letra da extensão em maiúscula.

        //if (name[i+1] >= 'a' && name[i+1] <= 'z')
        //    name[i+1] -= 0x20;

        //ext[i] = name[i+1];


        // #testando
        //Se não for letra então não colocamos no buffer de extensão;

        if (name[i+1] >= 'a' && name[i+1] <= 'z')
        {
            name[i+1] -= 0x20;
            ext[i] = name[i+1];
        }
    };


	// Acrescentamos ' ' até completarmos as oito letras do nome.


    while (ns < 8)
    {
        *name++ = ' ';
        ns++;
    };

	//Acrescentamos a extensão

    for (i=0; i < 3; i++)
        *name++ = ext[i];


    *name = '\0';
}



/*
 # todo
void __init_FILE (FILE *fp, int fd, unsigned char *buffer, int flags );
void __init_FILE (FILE *fp, int fd, unsigned char *buffer, int flags )
{
    if(!fp)
        return;
        
    fp->used = 1;
    fp->magic = 1234;        
        
    //fp->iopl = 3;
            
    fp->fd = fd;
    fp->_base = (unsigned char *) buffer;
    fp->_lbfsize = BUFSIZ;
    fp->_flags = flags;
}
*/


/*
 # todo
FILE *__make_FILE (int fd)
{
    FILE *fp = (FILE *) malloc ( sizeof(FILE) );
    
    if (!fp)
        return NULL;
    
    //fp->used = 1;
    //fp->magic = 1234;        
     
    // CLean 
    memset (fp, 0, sizeof(FILE));
    
    unsigned char *buffer;
    
    buffer = (unsigned char *) malloc (BUFSIZ);
    
    __init_FILE ( (FILE *) fp, (int) fd, (unsigned char *) buffer, 0 );
    
    return (FILE *) fp;
}
*/


// #todo
// https://linux.die.net/man/3/remove
int remove (const char *pathname)
{
    debug_print ("remove: [TODO] \n");
    return (int) (-1);
}



// Adapted from unix v32.
// Not tested yet.
void
_strout ( 
    int count, 
    char *string, 
    int adjust, 
    FILE *file, 
    int fillch )
{

    while (adjust < 0)
    {
        if (*string=='-' && fillch=='0')
        {
            putc (*string++, file);
            count--;
        }

        putc (fillch, file);
        adjust++;
    };

    while (--count>=0)
        putc (*string++, file);

    while (adjust) {
        putc (fillch, file);
        adjust--;
    };
}


//
// ================= low level =====================
//


// #todo:
// Se for NULL faz flush em todos.
int fflush (FILE *stream){

    return (int) __fflush (stream);
}


// real flush.
int __fflush (FILE *stream){

     //debug_print( "__fflush:\n");
	
    // FIXME: fflush(NULL) should flush all open output streams.
    //ASSERT(stream);
    if ( (void *) stream == NULL ){
        debug_print( "__fflush: stream\n");
        return -1;
    }
 
    //if ( !stream->_w )
        //return 0;
        
        
    if ( (void *) stream->_base == NULL ){
        debug_print( "__fflush: _base\n");
        return -1;
    } 


    if ( stream->_w <= 0 ){ 
        stream->_w = 0; 
        debug_print( "__fflush: _w\n");
        return -1;
    } 


    // #todo: 
    // This is the desired way.
    // Nesse teste escreveremos em stdout. Seu descritor indicará
    // que é um dispositivo do tipo console. O kernel escreverá no 
    // console 0.  

    int rc = write ( fileno(stream), stream->_base, stream->_w );


    // ISSO FUNCIONA.
    // vamos testar no console virtual.
    //int rc = write_VC ( 0, stream->_base, stream->_w ); 
 
    stream->_w = 0;
    //stream->error = 0;
    //stream->eof = 0;
    //stream->have_ungotten = false;
    //stream->ungotten = 0;
    
    if (rc < 0){
        //stream->error = errno;
        return EOF;
    }


    return 0;
}


//
// ============= Root 1 =================
//


// Buffer fill.
// Precisamos pegar o conteúdo do arquivo que está em ring0 e
// colocar no buffer em ring3 para o __getc pegar um byte.
// OUT: nreads. (quantos bytes foram lidos.)

int ____bfill (FILE *stream){

    int nbyte = 0;


    // struct
    if ( (void *) stream == NULL ){
        debug_print("____bfill: struct\n");
        printf ("____bfill: struct\n");
        return -1;
    }

    // buffer
    if ( stream->_lbfsize != BUFSIZ ){
        debug_print("____bfill: _lbfsize fail\n");
        printf ("____bfill: _lbfsize fail\n");
        return -1;
    }


    //
    // # Read!
    //
    
    nbyte = (int) read ( fileno(stream), 
                     stream->_p, 
                     stream->_lbfsize );

    
    if (nbyte<0){
        debug_print("____bfill: [DANGER] read fail\n");
        printf ("____bfill: [DANGER] read fail\n");
        return -1;
    }


    if (nbyte == 0){
        debug_print ("____bfill: [DEBUG] eof? empty file ?\n");
        printf ("____bfill: [DEBUG] eof? empty file ?\n");
        return EOF;
    }


    // OK. 
    // Read funcionou. 
    // :^)

    
    // Estamos cheios.
    stream->_cnt = BUFSIZ;
    
    return (int) nbyte;
}



// #todo:
// #importante
// Se tem alguma coisa no buffer em ring3 a gente pega,
// se não tem, então precisamos usar o read() e copiar do arquivo
// em ring0 para o buffer em ring3.
// >>>> _fillbuf()


// Cada processo pode ter uma 'vista' do arquivo em ring3.
// Então:
// + Se acabou nosso buffer em ring3, enchemos o buffer novamente.
// + Se não acabou então pegamos um byte no buffer.
// Isso vale para arquivos craidos com fopen cujo buffer
// ja começa vazio.

int __getc ( FILE *stream ){

    int ch = 0;
    int nreads = 0;


    // struct
    if ( (void *) stream == NULL ){
        debug_print ("__getc: stream struct fail\n");
        printf ("__getc: stream struct fail\n");
        return EOF;   
    }


	// Se acabou o buffer!!
	// cnt decrementou e chegou a zero.
	// Não há mais caracteres disponíveis entre 
	// stream->_ptr e o tamanho do buffer.

    if ( stream->_cnt <= 0 )
    {
        debug_print("__getc: [EMPTY BUFFER?] _cnt\n");
        
        // #todo:
        // We need this routine to call getc() after fopen().
  
        // coloque bytes no buffer dessa stream.
        nreads = (int) ____bfill (stream);
                  
            //nreads = read (fileno(stream), stream->_p, 1 );
            
        // fail.
        if (nreads <= 0){
            debug_print ("__getc: [BUFFER?] ____bfill fail\n");
            stream->_flags = (stream->_flags | _IOEOF); 
            stream->_cnt = 0;
            return EOF;
        }
        
        // Ok
        // Temos bytes no buffer.
        // Então vamos pegar um.
        
        ch = (int) *stream->_p;
        stream->_p++;
        stream->_cnt--;
    
        return (int) ch;
    }


    //
    // Ok. 
    // Vamos ler o buffer, porque ele está cheio.
    //

    // Se o ponteiro de leitura for inválido.
    // Não podemos acessar um ponteiro nulo ... 
    // no caso endereço.
    
    if ( stream->_p == 0 ){
        debug_print ("__getc: [BUFFER POINTER] stream struct fail\n");
        printf ("__getc: stream struct fail\n");
        return EOF;
    }

    //
    // # Read!
    //
    
    // Pega o char no posicionamento absoluto do arquivo.
    // Ajust file.
        
    ch = (int) *stream->_p;
    stream->_p++;
    stream->_cnt--;

    return (int) ch;
}



int __putc (int ch, FILE *stream){
     
    //assert (stream);
    //assert (stream->_w < stream->_lbfsize);
    
    if ( (void *) stream == NULL ){   
       debug_print( "__putc: stream\n");
       return -1;
    } 

    //if (stream->_w > stream->_lbfsize)
    if (stream->_w > BUFSIZ){   
       debug_print( "__putc: overflow\n");
       return -1;
    } 
    
    stream->_base[stream->_w++] = ch;

    if (stream->_w >= BUFSIZ){
        fflush (stream);
        return ch;
    }
    
    //if (stream->_flags == _IONBF || (stream->_flags == _IOLBF && ch == '\n'))
    if ( ch == '\n'){ 
        fflush (stream);
        return ch;
    }


    //if (stream->eof || stream->error)
        //return EOF;
    
    return ch;
}


// don't change it
int getc (FILE *stream){
    return (int) __getc (stream);
}


// don't change it
int putc (int ch, FILE *stream){
    return (int) __putc (ch, stream);
}


// don't change it
// See: unix v7
int fgetc ( FILE *stream )
{
    return (int) getc (stream);
}


// don't change it
// See: unix v7
int fputc ( int ch, FILE *stream )
{
    return (int) putc (ch, stream);
}



//
// Root 2
//

// don't change it
int getchar (void){
    return (int) getc (stdin);
}

// don't change it
int putchar (int ch){
    return (int) putc ( (int) ch, stdout );
}



//
// Root 3
//

char *gets (char *s){

    register c;
    register char *cs;

    cs = s;
    while ((c = getchar()) != '\n' && c >= 0)
        *cs++ = c;
    if (c<0 && cs==s)
        return (NULL);

    *cs++ = '\0';


    return (s);
}


int puts (const char *s){

    register c;

    while (c = *s++)
        putchar(c);
    return (putchar('\n'));
}


//
// Root 4
//

//s n iop
char *fgets (char *s, int size, FILE *stream){

    register c;
    register char *cs;

    cs = s;
    while (--size>0 && (c = getc(stream))>=0) 
    {
        *cs++ = c;
        if (c=='\n')
            break;
    };

    if (c<0 && cs==s)
        return (NULL);
    
    *cs++ = '\0';

    return (s);
}


//s iop
int fputs ( const char *s, FILE *stream ){

    register r;
    register c;

    while (c = *s++)
        r = putc(c,stream);
    return(r);
}


//
// Root 5
//


int getw (FILE *stream){

    register i;

    i = getc (stream);

	//#todo
	//if (stream->_flags&_IOEOF)
		//return(-1);
		
    if (stream->eof == 1 )
        return EOF;


    return (i | (getc(stream)<<8));
}


//#test
int putw (int w, FILE *stream)
{
    putc (w, stream);
    putc (w>>8, stream);
    
    // ?? rever retorno.
    return 0;
}



/*
 ****************************************
 * fclose:
 *     Close a file.
 *     If work, return 0. 
 */

// linux klibc style
// See: unistd.c for close().

int fclose (FILE *stream){

    int __ret = -1;

    if ( (void *) stream == NULL )
       return EOF;


    // Isso deve fechar o arquivo na lista de arqquivo abertos.
    __ret = (int) close ( fileno(stream) );


    stream->_base = NULL;
    stream->_p = NULL;
    
    //?? What
    //#todo
    //stream->_flags &= ~(_IOREAD|_IOWRT|_IONBF|_IOMYBUF|_IOERR|_IOEOF);

    stream->_cnt = 0;


    return (int) __ret;
}


// Carrega um arquivo e retorna o ponteiro para a estrutura de stream.
// #bugbug
// We just have the pointer for a structure.
// The file was loaded in ring0 by fopen(), using open()
// So, for now, we simply can't access the data of the file.
// The only way is using read() and the fd given by open().

FILE *fopen ( const char *filename, const char *mode ){

    FILE *__stream;   // Return this pointer.
    
    int fd;       /* File descriptor.  */
    int flags;    /* Stream flags.     */
    int oflags;   /* Flags for open(). */
    
    // #todo:
    // The 'mode' passed via argment will give us the 'flags'
    // used in open().
    
    /*
    int flags = 0;
    // NOTE: rt is a non-standard mode which opens a file for read, explicitly
    // specifying that it's a text file
    if (!strcmp(mode, "r") || !strcmp(mode, "rb") || !strcmp(mode, "rt"))
        flags = O_RDONLY;
    else if (!strcmp(mode, "r+") || !strcmp(mode, "rb+"))
        flags = O_RDWR;
    else if (!strcmp(mode, "w") || !strcmp(mode, "wb"))
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else if (!strcmp(mode, "w+") || !strcmp(mode, "wb+"))
        flags = O_RDWR | O_CREAT | O_TRUNC;
    else if (!strcmp(mode, "a") || !strcmp(mode, "ab"))
        flags = O_WRONLY | O_APPEND | O_CREAT;
    else if (!strcmp(mode, "a+") || !strcmp(mode, "ab+"))
        flags = O_RDWR | O_APPEND | O_CREAT;
    else {
        fprintf(stderr, "FIXME(LibC): fopen('%s', '%s')\n", pathname, mode);
        ASSERT_NOT_REACHED();
    }
    */


    // Open the file. 
    // No flags for now!

    // fd = open (filename, oflags, MAY_READ | MAY_WRITE) )
    fd = open (filename, 0, 0);  
    
    if (fd < 0){
        printf (" fopen: open() fail\n");
        return NULL;
    }

    //
    // Stream
    //

    __stream = (FILE *) malloc ( sizeof(FILE) );
    
    if ( (void *) __stream == NULL )
        return NULL;


    __stream->used = 1;
    __stream->magic = 1234;

    __stream->_file = fd;
    __stream->_flags = flags;

    __stream->_tmpfname = (char *) strdup(filename);


	// #importante:
	// Dessa forma fopen não permite que os aplicativos
	// leiam o conteúdo do arquivo no buffer.
	// Então o aplicativo terá que usar read pra ler 
	// o conteúdo no buffer em ring0.
	

    __stream->_base = (char *) malloc (BUFSIZ); 
    
     if ( (void *) __stream->_base == NULL ){
          debug_print("fopen: stream buffer fail\n");
          printf("fopen: stream buffer fail\n");
          return NULL;
          //exit (-1)
     }
    
    __stream->_p = __stream->_base; 
    __stream->_lbfsize = BUFSIZ;
    
    __stream->_cnt = 0;        // ??
    
    __stream->_r = 0;
    __stream->_w = 0;

    return (__stream);
}


/*
 ***************************************
 * fopen2:
 *     Open a file.
 *     @todo: Abrir onde? saída padrão?
 *     @retorna o que? o endereço do buffer?
 *     Obs: Tem função na api que carrega um arquivo em um buffer.
 *
 * #obs: Deveríamos checar o tamanho do arquivo antes de criar o buffer.
 *
 * #todo: E se o ponteiro para o nome do arquivo for inválido? tem que  
 * falhar e retornar null.
 */

FILE *fopen2 ( const char *filename, const char *mode ){

    FILE *__stream;
    
    int f = -1;
    
    __stream = (FILE *) malloc( sizeof(FILE) );
    
    if ( (void *) __stream == NULL )
        return NULL;
        
    
    
    /*
    if(*mode == 'w')
        f = creat(filename, 0600);
    else if (*mode == 'a'){

		//if ((f = open(filename, 1)) < 0)
			//f = creat(filename, 0600);
		//seek(f, 0, 2);
    
    }else{

	    //f = open(filename, 0);
	    //if (f < 0)
		    //return(NULL);

    };
    */
    
    
    //
    // size
    //
    
    stdio_fntos( (char *) filename);
    
    // get file size
    size_t s = (size_t) gramado_system_call ( 178, 
                            (unsigned long) filename,
                            0,
                            0 );
    
    
    if ( s <= 0 || s > 1024*1024 )
    {
        printf ("fopen: size\n");
        return NULL;
    }
    
    // endereço desejando.
    // ring 3.
    unsigned long address = (unsigned long) malloc(s);
    
    if (address == 0){
        printf ("fopen: address\n");
        return NULL;
    }


    // load the file into the address.
    // Vai retornar o fd.
    int fd = -1;
    
    //IN: service, name, address, 0, 0 
    fd = (int) gramado_system_call ( 3, 
                  (unsigned long) filename, 
                  (unsigned long) address,  
                  0 );

    if (fd < 0){
        printf ("fopen: Couldn't load the file\n");
        return NULL;
    }


    //
    // _flags
    //

    // #todo
	//__stream->_flags &= ~(_IOREAD|_IOWRT);


    if (*mode != 'r'){
        __stream->_flags |= _IOWRT;
    }else{
        __stream->_flags |= _IOREAD;
    };


    // #todo    
    // Isso deve ser o retorno de open() ou creat()
    // Me parece que a chamada acima também retorna o fd.
    
    __stream->_file = fd;    


    //base.
    __stream->_base = (unsigned char *) address; 
    __stream->_p = __stream->_base;
        
    // size
    __stream->_lbfsize = (int) s;   
    __stream->_cnt = __stream->_lbfsize;
    
    

    // retornar a stream que criamos aqui. 
    
    return (FILE *) __stream;
}





/*
 *****************************************
 * scroll:
 *     Scroll the screen in (text mode).
 *     @todo: Mudar para tmScroll(void);
 *     @todo: Criar gmScroll(void) talvez;
 *     @todo: Essa rotina precisa ser revisada e aprimorada.
 *
 *     #todo: Talvez deva levar em conta o modo de operação.
 *            Se estamos ou full screen ou não.
 */

void scroll (void){

	//Início da tela.
    unsigned short *p1 = (unsigned short *) SCREEN_START;

	//Início da segunda linha.
    unsigned short *p2 = (unsigned short *) (SCREEN_START + 2 * SCREEN_WIDTH);
    unsigned short i, j;


	// Linhas.
	// Usa o valor default se passar dos limites.

    if ( g_rows == 0 || g_rows >= SCREEN_MAX_HEIGHT )
    {
        g_rows = SCREEN_HEIGHT;
    }


	// 24 vezes.

    for ( i=0; i < g_rows -1; i++ )
    {
	    //80 vezes.
		
		//Se a variável nao foi inicializada, usar valor default.
        
        if (g_columns == 0 || g_columns >= SCREEN_MAX_WIDTH)
        { 
            g_columns = SCREEN_WIDTH;
        }

		//modo texto
		//if(g_using_gui == 0)
		//{
		//    for( j=0; j < g_columns; j++ ){
		//        *p1++ = *p2++;
		//    };
		//};


		// modo gráfico.

        if (g_using_gui == 1)
        {

            for ( j=0; j < g_columns; j++ )
            {
                //*p1++ = *p2++;
                //@todo:
            };
        }
    };


	// Limpando a última linha.

	// modo gráfico.

    if (g_using_gui == 1)
    {
        for ( i=0; i < g_columns; i++ )
        { 
            //*p1++ = 0x07*256 + ' ';
            // @todo:
        };    
    }
}



void clearerr (FILE* stream){

    if ( (void *) stream == NULL )
        return;
    
    //stream->_flags &= ~(_IOERR|_IOEOF);
    stream->eof = FALSE;
    stream->error = 0;
}



/*
 ************************
 * fread:
 *
 */

// #importante
// Ler uma certa quantidade de chars de uma stream e coloca-los no buffer.
// Isso vai ser usado pelo terminal por exemplo.
// Assim o terminal pode pegar uma string que esteja no arquivo.

// See:
// http://man7.org/linux/man-pages/man3/fread.3.html
// https://www.tutorialspoint.com/c_standard_library/c_function_fread.htm 

// #todo:
// Check the sizes.

size_t fread (void *ptr, size_t size, size_t n, FILE *fp){

    int nreads = 0;
    
    // Se não temos o ponteiro, então não teremos o fd
    // para usarmos em read().
    if ( (void *) fp == NULL ){
        printf ("fread: fp \n");
        return (size_t) -1;
    }
 
    // Quantidade inválida.
    if ( n <= 0 ){
        printf ("fread: n \n");
        return (size_t) -1;
    }

    if ( (void *) ptr == NULL ){
        printf ("fread: ptr \n");
        return (size_t) -1;
    }
    
    // Read.
     
    nreads = read ( fileno(fp), ptr, sizeof(ptr) );

    if (nreads <= 0){
        printf ("fread: read() fail \n");
        return (size_t) -1;   
    }

    return (size_t) nreads;
}


/*
 ************************
 * fwrite:
 *
 */

size_t fwrite (const void *ptr, size_t size, size_t n, FILE *fp){

    int nwrite = 0;

    
    // Se não temos o ponteiro, então não teremos o fd
    // para usarmos em read().
    if ( (void *) fp == NULL ){
        printf ("fwrite: fp \n");
        return (size_t) -1;
    }
 
    // Quantidade inválida.
    if ( n <= 0 ){
        printf ("fwrite: n \n");
        return (size_t) -1;
    }

    if ( (void *) ptr == NULL ){
        printf ("fwrite: ptr \n");
        return (size_t) -1;
    }

    nwrite = write( fileno(fp), ptr, sizeof(ptr) );
    
    if (nwrite <= 0){
        printf ("fwrite: write() fail \n");
        return (size_t) -1;   
    }

    return (size_t) nwrite;    
}



// O prompt precisa ser inicializado pelo crt0 e
// quando ele sobre flush.
int prompt_putchar ( int c, int con_id )
{

    if ( con_id < 0 )
        return -1;
    
    if ( con_id >= 4 )
        return -1;


   //#todo: overflow do prompt[]

    // ??
    //eol
    if ( c != '\n' ){
        input ( (unsigned long) c );
        return c;
    }
    

    // se \n
    if ( c == '\n' ){
       prompt_flush (con_id);
    }

    // ??
    // #bugbug: We need a return here.
    return 0;  //??
} 



int prompt_put_string ( char *string )
{
    if ( (void *) string == NULL )
        return -1;

    sprintf(prompt,(const char *) string);
    return 0;
}


int prompt_strcat (char *string)
{
	if ( (void *) string == NULL )
	    return -1;

    strcat(prompt,(const char *) string);
    return 0;
}


int prompt_flush ( int con_id )
{
    size_t len;

    if ( con_id < 0 )
        return -1;
    
    if ( con_id >= 4 )
        return -1;

    //finaliza.
    input ('\0'); 
    len = strlen( (const char *) prompt );  

    gramado_system_call ( 66, (unsigned long) prompt, con_id, len );  
    prompt_clean ();
    
    return 0; 
}


void prompt_clean (){

    int i=0;

	// Linpando o buffer de entrada.
	
    for ( i=0; i<PROMPT_MAX_DEFAULT; i++ ){
        prompt[i] = (char) '\0';
    }


    prompt[0] = (char) '\0';
    prompt_pos = 0;
    prompt_status = 0;
    prompt_max = PROMPT_MAX_DEFAULT;  
}



/*
 ****************************************************************
 *    Building the function printf;
 ****************************************************************
 */


/*
 **********************************************************************
 * prints:
 *
 */
 
// #bubgug:
// Esses argumentos podem estar fora dos padrões.
// Mas a implementação feita pelo altor é assim mesmo. 

static int 
prints ( 
    char **out, 
    const char *string, 
    int width, 
    int pad )
{
	
    register int pc = 0, padchar = ' ';

    
	// filtro: e se width for menor que 0??
	
	//if(width < 0){
	//	return (int) 1;
	//}
	
	if ( width > 0 ) 
	{
	    register int len = 0;
		register const char *ptr;
		
		for(ptr = string; *ptr; ++ptr) ++len;
		if(len >= width) width = 0;
		else width -= len;
		if(pad & PAD_ZERO) padchar = '0';
	};


	if ( !(pad & PAD_RIGHT) ) 
	{
		for ( ; width > 0; --width ){
		    printchar ( out,padchar );
			++pc;
		};
	};


    for ( ; *string ; ++string )
    {
        printchar (out, *string);
        ++pc;
    };


    for ( ; width > 0; --width )
    {
        printchar (out,padchar);
        ++pc;
    };


	return pc;
}


/*
 ***************************************
 * printi:
 *     used by printf.
 */

static int 
printi ( 
    char **out, 
    int i, 
    int b, 
    int sg, 
    int width, 
    int pad, 
    int letbase )
{
    register char *s;
    register int t, neg = 0, pc = 0;
    register unsigned int u = i;

    char print_buf[PRINT_BUF_LEN];


    if (i == 0){
        print_buf[0] = '0';
        print_buf[1] = '\0';
        return prints (out, print_buf, width, pad);
    }


    if ( sg && b == 10 && i < 0) 
    {
        neg = 1;
        u = -i;
    }

    s = print_buf + PRINT_BUF_LEN-1;
    *s = '\0';

    while (u) 
    {
        t = u % b;

        if (t >= 10)
            t += letbase - '0' - 10;
        *--s = t + '0';
        u /= b;
    };


    if (neg) 
    {
        if ( width && (pad & PAD_ZERO) ) 
        {
            printchar (out, '-');
            ++pc;
            --width;

        }else{
            *--s = '-';
        };
    }


    // Done.

    return pc + prints(out, s, width, pad);
}


/*
 *****************************************
 * print:
 *     Used by printf.
 */

static int print ( char **out, int *varg ){

    register int width, pad;
    register int pc = 0;
    register char *format = (char *) (*varg++);

    char scr[2];


    for ( ; *format != 0; format++ ) 
    {

		//switch.
        if ( *format == '%' ) 
        {

			++format;
			width = pad = 0;
			
			if ( *format == '\0' ) 
			    break;
			
			if ( *format == '%' ) 
			    goto out;
			
			if ( *format == '-' ){
				++format;
				pad = PAD_RIGHT;
			};
			
			while ( *format == '0' ){
				++format;
				pad |= PAD_ZERO;
			};
			
			for ( ; *format >= '0' && *format <= '9'; ++format ){
				width *= 10;
				width += *format - '0';
			};
			
			//string
			if ( *format == 's' ){
				
				register char *s = *( (char **) varg++ );
				pc += prints ( out, s?s:"(null)", width, pad);
				continue;
			}
			
			//int
			if ( *format == 'd' ){
				pc += printi ( out, *varg++, 10, 1, width, pad, 'a');
				continue;
			}
			
			//hexa
			if ( *format == 'x' ){
				pc += printi ( out, *varg++, 16, 0, width, pad, 'a');
				continue;
			}
			
			if ( *format == 'X' ){
				pc += printi ( out, *varg++, 16, 0, width, pad, 'A');
				continue;
			}
			
			if ( *format == 'u' ){
				pc += printi ( out, *varg++, 10, 0, width, pad, 'a');
				continue;
			}
			
			//char
			//char are converted to int then pushed on the stack. 
			if ( *format == 'c' ){
				
				scr[0] = *varg++;
				scr[1] = '\0';
				pc += prints ( out, scr, width, pad);
				continue;
			}
			
			//Nothing.
		
		}else{
			
		    out:
			    printchar ( out, *format );
			    ++pc;
		};

		//Nothing.
	};


    if (out) 
        **out = '\0';


    return (int) pc;
}


/* 
 ****************************************************
 * printf:
 *     Imprime uma string formatada.
 *     Assuming sizeof(void *) == sizeof(int).
 * Obs: Temos uma chamada ao sistema para cada caractere.
 * 
 * Obs: Se passássemos apenas o ponteiro da string para o kernel pintá-la,
 * não estariamos usando o printf da biblioteca em user mode e sim 
 * a printf do kernel.
 */
 
// Padrão não tradicional, mas funciona. 

int printf3 ( const char *format, ... ){

    register int *varg = (int *)(&format);

    return (int) print ( 0, varg );
}


//
//=============================================================
//

// usada na printf2
void printf_atoi (int value, char* valuestring){

    int min_flag;
    char swap, *p;

    min_flag = 0;


    if (0 > value)
    {
        *valuestring++ = '-';

        value = -INT_MAX> value ? min_flag = INT_MAX : -value;
    }

    p = valuestring;


    do {

        *p++ = (char)(value % 10) + '0';

        value /= 10;
      
    } while (value);


    if (min_flag != 0)
    {
        ++*valuestring;
    }


    *p-- = '\0';


    while (p > valuestring)
    {
        swap = *valuestring;
        
        *valuestring++ = *p;
        
        *p-- = swap;
    }
}


//usada na printf2
void printf_i2hex (uint32_t val, char *dest, int len){

    char *cp;
    char x;
    uint32_t n;


    n = val;
    cp = &dest[len];


    while (cp > dest)
    {
        x = n & 0xF;
        n >>= 4;

        *--cp = x + ((x > 9) ? 'A' - 10 : '0');
    };

    dest[len+1] = '\0';
}



// #test 
// tentando implementar a printf do nelson cole.
// padrão tradicional, incompleta, não funciona ainda,
// estamo implementando

int printf2 ( const char *format, ... ){

    char *ap;
    va_start (ap,format);

    int index = 0;
    uint8_t u;
    int d;
    char c, *s;

    char buffer[256];


    while ( format[index] )
    {

		switch (format[index])
		{
		    case '%':
			    ++index;
			    switch (format[index])
			    {
			
                    case 'C':
			        case 'c':
				        c = (int8_t) va_arg (ap, int32_t);
				        putchar(c);
				        break;
     
                    case 'S':
			        case 's':
				        s = va_arg (ap, int8_t*);
				        //puts(s);
				        printf2(s);
						break;

			        case 'd':
			        case 'i':
				        d = va_arg (ap, int32_t);
				        //atoi(d, buffer);
				        printf_atoi(d, buffer);
						//puts(buffer);
						printf2(buffer);
				        break;

			        case 'U':
			        case 'u':
				        u = va_arg (ap, uint32_t);
				        //atoi(u, buffer);
				        printf_atoi (u, buffer);
						//puts(buffer);
				        printf2 (buffer);
						break;

			        case 'X':
			        case 'x':
				        d = va_arg (ap, int32_t);
				        //i2hex(d, buffer,8);
						//printf_i2hex(d, buffer,8);
				        //puts(buffer);
				        printf ("%x",d);
						break;
			
			        default:
				        putchar ('%');
				        putchar ('%');
				        break;
				
			    }
			    break;

		    default:
			    putchar ( format[index] );
			    break;
        };

        ++index;
    };


    return 0;
}



//
//=============================================================
//

//find next line
char *stdio_nextline ( char *string ){

    char *p;


    p = (char *) strchr (string, '\n');
    
    if (p==NULL){
        return (p);
    }

    ++p;

    
    if (*p=='\0')
    {
		//não há próxima linha
        return (NULL);
    
    } else {
		
		//retorna o ponteiro para a próxima linha.
        return (p);
    }
}


//next line sprintf
int nlsprintf ( char *out, const char *format, ... ){

    char *tmp;


    tmp = (char *) stdio_nextline (out);

    if (tmp == NULL ){
        return -1;
    }

    register int *varg = (int *)(&format);

    return (int) print( &tmp, varg );
}


/*
 *****************************************************************
 * sprintf:
 *     Emprime uma string formatada em 'out'.
 *     Escreve em um buffer de caracteres.
 * @todo: ??
 * Em caso de sucesso, a função retorna o número de caracteres 
 * da string formatada. Em caso de erro, um valor negativo é retornado.
 */
 
//opção 
//int sprintf(char *str, const char *format, ...) ?? 

int sprintf ( char *out, const char *format, ... ){

    register int *varg = (int *)(&format);

    return (int) print( &out, varg );
}


static void printchar ( char **str, int c ){

    if (str) 
    {
        **str = c;

        ++(*str);

    } else (void) putchar (c);
}






// Setup libc mode.
void libc_set_output_mode ( int mode ){

    switch (mode)
    { 
		case LIBC_DRAW_MODE:
			__libc_output_mode = mode;
			break;

		case LIBC_NORMAL_MODE:
			__libc_output_mode = mode;
			break;

		default:
			__libc_output_mode = LIBC_DRAW_MODE;
			printf ("libc_set_output_mode: fail");
			break;
    };
}



/*
 **********
 * outbyte:
 * @todo: Colocar no buffer de arquivo.
 * #obs: essa função chamará _outbyte.
 *
 * #importante: Não me lebro se o kernel efetua o refresh do char 
 * nesse caso.
 */
 
void outbyte (int c){

    static char prev = 0;

	// spaces.

    if ( c <  ' ' && 
        c != '\r' && 
        c != '\n' && 
        c != '\t' && 
        c != '\b' )
    {
        return;
    }


	//#testando.
	//nada para imprimir.
	//?? onde fica o tratamento de backspace.??
	//if( c == '\b' )
    //{
	//	return;
	//}


	// carriege return 
	// Volta ao início da linha.
    if ( c == '\r' )
    {
        g_cursor_x = 0;
        prev = c;
        return;    
    }


	//Próxima linha e não início da linha.   
    if ( c == '\n' && prev != '\r' ) 
    {
        g_cursor_y++; 
        g_cursor_x = 0;  
        prev = c;
        return;
    }


    //Próxima linha e início da linha. 	   
    if ( c == '\n' && prev == '\r' ) 
    {
        g_cursor_y++;  
        prev = c;
        return;
    }


	//Tab.
	//@todo: Criar a var -> 'g_tab_size'.
    if ( c == '\t' )  
    {
        g_cursor_x += (4);    
        prev = c;
        return;         
    }


	//Space 
	//#todo:
	// ?? talvez devêssemos imprimir o caractere espaço. ??    
    if ( c == ' ' )  
    {
        g_cursor_x++; 
        prev = c;
        return; 
    }


    //Delete. 
    if ( c == 8 )  
    {
        g_cursor_x--; 
        prev = c;
        return; 
    }


    // Filtra as dimensões da janela onde esta pintando.
 
//checkLimits:

	//
	// Colunas.
	//

	//Definindo a largura que usaremos.
	//A largura precisa ser maior que '0' e menor que o limite máximo.
	//Obs: @todo: Essa rotina de definição pode ir para o momento da inicialização
	//da biblioteca. Ela contunua aqui porque está funcionando como um filtro.

    if ( g_columns == 0 || g_columns >= SCREEN_MAX_WIDTH )
    {
        g_columns = COLUMNS;
    }


	//O cursor não pode ultrapassar a largura definida.
    if ( g_cursor_x > g_columns )
    {
        g_cursor_x = 0;
        g_cursor_y++;  
    }else{
		//Se não alcançamos o limite, apenas incrementa o x.
        g_cursor_x++;    
    };


	// Linhas.
	
	//Definindo a altura que usaremos.
	//A altura precisa ser maior que '0' e menor que o limite máximo.
    if ( g_rows == 0 || g_rows >= SCREEN_MAX_HEIGHT )
    {
        g_rows = ROWS;
    }

    //O cursor não pode ultrapassar a altura definida.
	//se ultrapassar, chamaremos o scroll.
	//Obs: O scroll ainda não está implementado.
	//O scroll será feito depois que implementarmos o array de ponteiros
	//para estruturas de linha.
	
    if ( g_cursor_y > g_rows )
    { 
        scroll ();

        g_cursor_x = 0;             //O cursor deve fica na primeira coluna.
		g_cursor_y = (g_rows-1);    //O cursor deve ficar na última linha.
    }

    // Imprime os caracteres normais.

    _outbyte (c);

	//Atualisa o prev.
    prev = c; 
}


/*
 ***********************************
 * _outbyte:
 *     Just output a byte on the screen.
 *
 *     Obs: A função não se preocupa com o cursor.
 *          Essa rotina está preparada somente par ao modo gráfico.
 *          Talvez usaremos um selecionador de modo.   
 *
 * #obs: 
 * #importante: Não me lebro se o kernel efetua o refresh do char 
 * nesse caso.
 */

	// #obs: 
	// Tamanho do char constante = 8. 
	// O que queremos é usar uma variável.

void _outbyte ( int c ){

	// #bugbug
	// Essa funçao nao 'e usada ... NAO funciona.
	// printf usa outra coisa (65).

    gramado_system_call ( 7, 
        8*g_cursor_x,  
        8*g_cursor_y, 
        (unsigned long) c ); 


    //if ( (void *) stdout == NULL )
       //return;

	//#todo
	//putc ( ch, stdout );
}


/*
 ************************************************************
 * input:
 *     Coloca os caracteres digitados em uma string. 'prompt[]'
 *     #bugbug: 
 *     Deveríamos considerar o posicionamento dentro do arquivo.
 *     Dentro da stream.
 */

unsigned long input ( unsigned long ch ){

	//save cursor position.
	unsigned long tmpX, tmpY;
	
	// Convert.
	char c = (char) ch;    


	
	// Ajust prompt max.
	if ( prompt_max == 0 || prompt_max >= PROMPT_MAX_DEFAULT )
	{
		prompt_max = PROMPT_MAX_DEFAULT;
	}
	
	//Filtra limite.
	//retornar 1??

    if ( prompt_pos > prompt_max ){
        printf ("input: Full buffer!\n");
        return (unsigned long) 0;   
    }

	// Trata caractere digitado.

    switch (c)
    {
       //Enter.
		case VK_RETURN:
		    prompt[prompt_pos] = (char ) '\0';
            goto input_done;
		    break;

	    // Obs: O tab avança o cursor, mas precisamos 
		// pintar o espaço onde estava o cursor.
		// Por isso talvez seja bom um while de espaços.
		// Mas no buffer fica apenas o '\t'.
		case VK_TAB:
		    prompt[prompt_pos] = (char ) '\t';
			//for( i=0; i<4; i++)
			//{ printf("%c", (char) ' '); }
		    goto input_done;
			break;

		case VK_BACKSPACE:
            
			// Se estamos no início da linha.
			if (prompt_pos <= 0){
                prompt_pos = 0; 
			    prompt[prompt_pos] = (char ) '\0';
				break; 
			}


			//altera o buffer.
			//Apaga o anterior.
			prompt_pos--;                        //volta um no buffer.
			prompt[prompt_pos] = (char ) '\0';   //apaga no buffer.
			
			//altera a tela no modo gráfico com janelas.
			tmpX = stdioGetCursorX(); 
			tmpY = stdioGetCursorY();
			
			//Volta um na tela.
			tmpX--;
			stdioSetCursor(tmpX,tmpY);
			
			//apaga a tela.
			printf ("%c", (char) ' '); 
			
			//altera a tela no modo gráfico com janelas.
			tmpX = stdioGetCursorX(); 
			tmpY = stdioGetCursorY();
			
			//Volta um na tela.
			tmpX--;
			stdioSetCursor(tmpX,tmpY);
			break;
			
		//Continua ...
		
		// Vamos deixar o sistema mostrar o cursor.
		// Coloca carctere normal no buffer.
		// Incrementa posicionamento no buffer.
		default:
		    prompt[prompt_pos] = c;
		    prompt_pos++;
			break;
	};


    return (unsigned long) 0;

input_done:

    return VK_RETURN;
}






/*
 * #opção.
int fprintf(FILE *fp, const char *fmt, ...);
int fprintf(FILE *fp, const char *fmt, ...)
{
    va_list ap;
    int err;

    va_start(ap, fmt);

    if ( (void *) fp == NULL )
       return EOF;

    err = vfprintf(fp, fmt, ap);
    va_end(ap);
    return err;
}
*/


/*
 ********************************
 * fprintf:     
 */

// #bugbug
// We just have the pointer for a structure.
// The file was loaded in ring0 by fopen(), using open()
// So, for now, we simply can't access the data of the file.
// The only way is using read() and the fd given by open().

int fprintf ( FILE *stream, const char *format, ... ){


    if ( (void *) stream == NULL )
       return EOF;

    
   debug_print ("fprintf: TODO. Not implemented yet\n");
  
   // Suspendendo a notificação do terminal.
   return 0;
}




//#todo: testar.
//Credits: Sombra OS.
void nputs (char *cp, int len){
	
    int i = len;
    char *str;

    for (str = cp; i; str++, i--)
        putchar ( (int) *str);
}


/*uClib style*/
	/* 
	 * Strictly speaking, this implementation is incorrect as the number
	 * of chars gets can read should be unlimited.  However, I can't
	 * imagine anyone wanting to gets() into a buffer bigger than INT_MAX.
	 *
	 * Besides, this function is inherently unsafe and shouldn't be used.
	 */
/* This is an UNSAFE function! */
char *uclib_gets (char *str) 
{
    if ( (void *) stdin == NULL )
       return (char *) 0;


    return fgets (str, INT_MAX, stdin);
}



/*
//unix v32 like
char *unixv32_gets (char *s);
char *unixv32_gets (char *s)
{
    register c;
    register char *cs;

    cs = s;
    while ((c = getchar()) != '\n' && c >= 0)
        *cs++ = c;
    if (c<0 && cs==s)
        return(NULL);

    *cs++ = '\0';

    return (s);
}
*/


// glibc
/* Read a newline-terminated string from stdin into S,
   removing the trailing newline.  Return S or NULL.  */
/*
char *glibc_gets (char *s);
char *glibc_gets (char *s)
{
  register char *p = s;
  register int c;
  FILE *stream = stdin;

    if ( (void *) stream == NULL )
       return (char *) 0;


    if (!__validfp(stream) || p == NULL)
    {
        errno = EINVAL;
         return NULL;
    }

    if (feof(stream) || ferror(stream))
        return NULL;

    while ((c = getchar()) != EOF)
        if (c == '\n')
            break;
        else
            *p++ = c;

     *p = '\0';

     //Return null if we had an error, or if we got EOF
     //  before writing any characters.  

     if (ferror (stream) || (feof (stream) && p == s))
        return NULL;

     return s;
}
*/


/*
ssize_t getline(char** lineptr, size_t* n, FILE* stream)
{
    return getdelim(lineptr, n, '\n', stream);
}
*/






/*
 *********************************
 * ungetc:
 */

int ungetc ( int c, FILE *stream ){

    //ASSERT(stream);
    if ( (void *) stream == NULL )
        return EOF;
    
    if (c == EOF)
        return EOF;
    
    // Não precisamos mudar.
    if (stream->have_ungotten)
        return EOF;
    
    // Mudando.
    stream->have_ungotten = TRUE;
    
    // Salva.
    stream->ungotten = c;
    
    
    stream->eof = FALSE;

    return c;
}




/*linux - klibc style*/
/*
static __inline__ off_t ftell(FILE *__f)
{
  extern off_t lseek(int, off_t, int);
  return lseek(fileno(__f), 0, SEEK_CUR);
}
*/



// Dixaremos o kernel manipular a estrutura.
//This function returns the current file position of the stream stream. 
long ftell (FILE *stream){

    if ( (void *) stream == NULL )
        return 0;
        
    fflush (stream);
    
    return lseek ( fileno(stream) , 0, SEEK_CUR);
}


/*
 * fileno: 
 *     Gets the file id.
 *     The kernel gets this value from the stream struct.
 */
int fileno ( FILE *stream ){

    //assert(stream);
 
    if ( (void *) stream == NULL )
       return EOF; 
       
    return (int) stream->_file;
}



/*linux klibc style.*/
// Isso vai ler no arquivo que está em ring0.
int linux_fgetc (FILE *f){

    unsigned char ch=0;

  
    if ( (void *) f == NULL )
       return EOF;
  
    
    return ( fread (&ch, 1, 1, f) == 1) ? (int) ch : EOF;
}



/*uClib style*/
char *fgets2 (char *s, int count, FILE *fp){

    char *p;
    int ch=0;


    p = s;

    if ( (void *) fp == NULL )
       return (char *) 0;


    // Guard against count arg == INT_MIN. 
    while (count-- > 1) 
    {		
		ch = getc (fp);

        if (ch == EOF){
            break;
        }

		*p++ = ch;
		
		if (ch == '\n') { break; }
    }

	if ( ferror(fp) || (s == p) ) 
	{
		return 0;
	}

    *p = 0;

    return s;
}



/*
 ********************************
 * fputs2:      
 */

int fputs2 ( const char *str, FILE *stream )
{

    if ( (void *) stream == NULL )
        return EOF;


    for (; *str; ++str) {

        int rc = putc (*str, stream);

        if (rc == EOF)
            return EOF;
    }

    return 1;
}




/*
 *********************************
 * gets:
 *     gets() devolve um ponteiro para string
 */
 
char *gets2 (char *s){

    char *p;
    int ch=0;
    int t = 0;


	//printf("gets:\n");

    //salva
    p = s; 

    while (1)
    {
        ch = (int) getchar ();
		
        if ( ch != -1 )
		{
            			
			switch (ch) 
		    {
				/* termina a string */
			    case '\n':
				case VK_RETURN: 
                    s[t] =  (char) '\0'; 
                    goto done;
				    break;
            
                case '\b':
				case VK_BACKSPACE:
			        if(t > 0){ 
					    t--;
					};
                    break;
            
			    default:
                    //s[t] = (char) ch;
					//t++;
					break;
            };
			
			printf ("%c",ch);
			s[t] = (char) ch;
			t++;
			
		};
		
		// #bugbug
        // IF X86
        asm ("pause");
    };


done:

    //s[t] = (char) '\0';

    return (char *) p;
}


/*
 ***********************
 * puts:
 */

// #bugbug
// isso deve escrever no arquivo, assim como
// tudo em libc.

//  wtf ??
int puts2 ( const char *str ){
    return (int) printf ("%s",str);
}




/*
 *************************************
 * getchar2:
 *       O kernel pega no stdin que é a fila do teclado.
 *       Isso funciona.
 */

int getchar2 (void){

    int Ret = 0;

	// #todo: 
	// ? Já temos uma função para essa chamada ? 137.

Loop:

    Ret = (int) gramado_system_call ( 137, 0, 0, 0 ); 

    if (Ret > 0)
    {
        return (int) Ret;    
    }


    goto Loop;
    

    //if ( (void *) stdin == NULL )
       //return EOF;

    // se glibc
    //return __getc(stdin);
}



/*
int feof(FILE *fp)
{
    if ( (void *) fp == NULL )
       return EOF;

    return fp->mode & __MODE_EOF;
}
*/


/*
 *********************************
 * feof:
 */

int feof ( FILE *stream ){

    //assert(stream);

    if ( (void *) stream == NULL )
       return EOF;
       
       
    return (int) stream->eof;
}



/*
 //This function clears the end-of-file and error indicators for the stream stream. 
void clearerr(FILE *fp)
{
    if ( (void *) fp == NULL )
       return;

    //#bugbug: Isso precisa ser feito em ring0;
	//fp->mode &= ~(__MODE_EOF | __MODE_ERR);
}
*/


/*
 *********************************
 * ferror:
 */

int ferror ( FILE *stream ){

    if ( (void *) stream == NULL )
       return EOF;


    return (int) stream->error;
}



/*linux - klibc style*/
/*
static __inline__ int fseek(FILE *__f, off_t __o, int __w)
{
  extern off_t lseek(int, off_t, int

    if ( (void *) __f == NULL )
       return EOF;

  return (lseek(fileno(__f), __o, __w) == (off_t)-1) ? -1 : 0;
}
*/
	
	
/*
 **************************************
 * fseek:
 *     offset argument is the position that you want to seek to,
 *     and whence is what that offset is relative to.
 */

// The fseek function is used to change the file position of the stream stream. 
int fseek ( FILE *stream, long offset, int whence ){

    debug_print ("fseek: TODO. Not implemented yet\n");

    return -1;
	/*
    assert(stream);
    fflush(stream);
    off_t off = lseek(stream->fd, offset, whence);
    if (off < 0)
        return off;
    stream->eof = false;
    stream->error = 0;
    stream->have_ungotten = false;
    stream->ungotten = 0;
    return 0;
    */
}



/*linux klibc style*/
// Isso vai escrever no arquivo que está em ring0.
int linux_fputc (int c, FILE *f){

    unsigned char ch = c;

    if ( (void *) f == NULL )
       return EOF;
  
  
    return fwrite(&ch, 1, 1, f) == 1 ? ch : EOF;
}



/*
 * #todo: Implementar isso.
int putw (int w, FILE *stream);
int putw (int w, FILE *stream)
{

    if ( (void *) stream == NULL )
       return EOF;

    if ( fwrite ( (const char*) &w, sizeof (w), 1, stream ) != 1 )
        return EOF;

    return 0;
}
*/


/*
ssize_t getline (char **lineptr, size_t *n, FILE *stream); 
ssize_t getline (char **lineptr, size_t *n, FILE *stream)
{
    if ( (void *) stream == NULL )
       return -1;

  return getdelim (lineptr, n, '\n', stream);
}  
*/



// interna
// #todo: criar essa rotina na libc.
void debug_print (char *string)
{
    gramado_system_call ( 289, 
        (unsigned long) string,
        (unsigned long) string,
        (unsigned long) string );
}



/*
 ********************************
 * stdioSetCursor:
 *     estamos falando do posicionamento do cursor dentro da janela
 *     e não dentro do terminal.
 */

//34 - set cursor.

void stdioSetCursor ( unsigned long x, unsigned long y )
{
    gramado_system_call ( 34, x, y, 0 );
}


/*
 * stdioGetCursorX:
 *     Get cursor x. 
 *     estamos falando do posicionamento do cursor dentro da janela
 *     e não dentro do terminal.
 */ 

unsigned long stdioGetCursorX ()
{
    return (unsigned long) gramado_system_call ( 240, 0, 0, 0 );
}


/*
 * stdioGetCursorY:
 *     Get cursor y.
 *     estamos falando do posicionamento do cursor dentro da janela
 *     e não dentro do terminal. 
 */

unsigned long stdioGetCursorY ()
{
    return (unsigned long) gramado_system_call ( 241, 0, 0, 0 );
}


//======================================================================
// scanf support (start)
//======================================================================


int scanf ( const char *fmt, ... ){

    va_list ap;
    va_start (ap, fmt);

    int d;
    char *s;

	//%c
    int ch;

	//usado em %d
    int *i;
    char tmp[80];


	//o char.
    int c=0;

    char *cp;

    char *t;

    int nread;
    int size = (int) stdio_strlen (fmt);



	nread = 0;
	
    while (*fmt)
    {

		c = *fmt;
		if (c == 0)
			return (0); //erro
		
		// pulando os espaços
		if ( isspace (c) ) 
		{
			while ( size > 0 && isspace(*fmt) )
			{
				nread++; 
				size--; 
				fmt++;
			};
			
			continue;
		}
		
        switch (*fmt++) 
		{
			/* string */
            case 's':              
				//pego o ponteiro para  a string, depois é só usar o gets. ??
				s = va_arg(ap, char *);
				
				//usaremos esse ponteiro para colocar uma string digitada.
				if ( (void *) s != NULL )
				{
				    gets (s);
				}else{
				    printf ("scanf: s null pointer\n");	
				}
				
				//testar ...
				//printf("string %s\n", s);
                break;
            
			/* int */
			case 'd': 
				//pego o ponteiro para  a string, depois é só usar o gets. ??
				i = va_arg(ap, int *);
				
				//usaremos esse ponteiro para colocar uma string digitada.
				if( (void *) i != NULL )
				{
				    //pego uma string de caracteres, que são números digitados.
					gets (tmp);
					
					//converte essa string em dígito
					i[0] = (int) stdio_atoi (tmp);                     
					
				}else{
				    printf ("scanf: s null pointer\n");	
				}
				
				//testar ...
				//printf("string %s\n", s);
			
                //d = va_arg(ap, int);
                //printf("int %d\n", d);
                break;
            
			/* char */
			case 'c':    
                t = va_arg (ap, char *);
				if ( (void *) t != NULL )
				{
				
                    while (1)
                    {
			            ch = (int) getchar ();
						
						if ( ch != -1 )
						{
						    t[0] = ch;
                            //printf("scanf ch={%c}",ch);
						    break;	
						}			
					};	
					
				}else{
					printf ("scanf: c null pointer\n");
				}; 				 
                break;
				
			//default:
            //    break; 
				
        };//switch
    };


   //va_end (ap);
   
   return 0;
}
//======================================================================
// scanf support (end)
//======================================================================



/*
 * sscanf: 
 * Scan items from a string in accordance with a format.  This is much
 * simpler than the C standard function: it only recognises %d without a
 * field width, and does not treat space in the format string or the
 * input any differently from other characters.  The return value is the
 * number of characters from the input string that were successfully
 * scanned, not the number of format items matched as in standard sscanf.
 * e.mcmanus@opengroup.org, 12 Feb 97
 * Credits: apple open source.
 */

int sscanf ( const char *str, const char *format, ... ){

    const char *start = str;
    
    va_list args;
    va_start(args, format);


    for ( ; *format != '\0'; format++ )
    {

		if ( *format == '%' && format[1] == 'd' ){
			
			int positive;
			int value;
			int *valp;
			
			if (*str == '-') {
				positive = 0;
				str++;
			} else
				positive = 1;
			if (!isdigit(*str))
				break;
			value = 0;
			do {
				value = (value * 10) - (*str - '0');
				str++;
			} while (isdigit(*str));
			if (positive)
				value = -value;
			valp = va_arg(args, int *);
			*valp = value;
			format++;
		} else if (*format == *str) {
			str++;
		} else
			break;
	};

    va_end (args);

    return str - start;
}



//=============================================================
// printf (start)
//=============================================================
/*-
 * Copyright (c) 1986, 1988, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)subr_prf.c	8.3 (Berkeley) 1/21/94
 */

//typedef unsigned long size_t;
//typedef long ssize_t;
//#ifdef __64BIT__
//typedef unsigned long long uintmax_t;
//typedef long long intmax_t;
//#else
typedef unsigned int uintmax_t;
typedef int intmax_t;
//#endif
typedef unsigned char u_char;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned short u_short;
typedef unsigned long long u_quad_t;
typedef long long quad_t;
typedef unsigned long uintptr_t;
//typedef long ptrdiff_t;
//#define NULL ((void*)0)
#define NBBY    8               /* number of bits in a byte */
char const hex2ascii_data[] = "0123456789abcdefghijklmnopqrstuvwxyz";
#define hex2ascii(hex)  (hex2ascii_data[hex])
//#define va_list __builtin_va_list
//#define va_start __builtin_va_start
//#define va_arg __builtin_va_arg
//#define va_end __builtin_va_end

//#obs:
//vamos usar o que está em ctype.h
//#define toupper(c)  ((c) - 0x20 * (((c) >= 'a') && ((c) <= 'z')))

static size_t stdio_strlen (const char *s)
{
    size_t l = 0;

    while (*s++)
        l++;
    return l;
}


/* Max number conversion buffer length: 
 a u_quad_t in base 2, plus NUL byte. */
#define MAXNBUF	(sizeof(intmax_t) * NBBY + 1)


/*
 * ksprintn:
 *     Put a NUL-terminated ASCII number (base <= 36) in a buffer 
 * in reverse order; return an optional length and a pointer to 
 * the last character written in the buffer 
 * (i.e., the first character of the string).
 * The buffer pointed to by `nbuf' must have length >= MAXNBUF.
 */
 
static char *
ksprintn ( 
    char *nbuf, 
    uintmax_t num, 
    int base, 
    int *lenp, 
    int upper )
{
    char *p, c;


    p = nbuf;

	//*p = ' ';
	*p = 0;


    do {

		c = hex2ascii (num % base);
		
		*++p = upper ? toupper(c) : c;
		
    } while (num /= base);


    if (lenp)
        *lenp = (p - nbuf);

    return (p);
}


/*
 * kvprintf:
 *     Scaled down version of printf(3).
 *
 * Two additional formats:
 *     The format %b is supported to decode error registers.
 *     Its usage is:
 *
 *	printf("reg=%bn", regval, "*");
 *
 * where  is the output base expressed as a control character, e.g.
 * 10 gives octal; 20 gives hex.  Each arg is a sequence of characters,
 * the first of which gives the bit number to be inspected (origin 1), and
 * the next characters (up to a control character, i.e. a character <= 32),
 * give the name of the register.  Thus:
 *
 *	kvprintf("reg=%bn", 3, "102BITTWO1BITONEn");
 *
 * would produce output:
 *
 *	reg=3
 *
 * XXX:  %D  -- Hexdump, takes pointer and separator string:
 *		("%6D", ptr, ":")   -> XX:XX:XX:XX:XX:XX
 *		("%*D", len, ptr, " " -> XX XX XX XX ...
 */

// #bugbug
// Passing a function via argument is not a good thing for 
// our compiler.

int 
kvprintf ( 
    char const *fmt, 
    void (*func)( int, void * ), 
    void *arg, 
    int radix, 
    va_list ap )
{
    
    //#define PCHAR(c) { int cc=(c); if(func) (*func)(cc,arg); else *d++ = cc; retval++; }
    #define PCHAR(c) { int cc=(c); if(func) (*func)(cc,arg); else *d++ = cc; retval++; }


	char nbuf[MAXNBUF];
	char *d;
	const char *p, *percent, *q;
	u_char *up;

	int ch, n;
	uintmax_t num;
	int base, lflag, qflag, tmp, width, ladjust, sharpflag, neg, sign, dot;
	int cflag, hflag, jflag, tflag, zflag;
	int dwidth, upper;
	char padc;
	int stop = 0, retval = 0;


	num = 0;
	
	if (!func)
		d = (char *) arg;
	else
		d = NULL;



	if (fmt == NULL)
		fmt = "(fmt null)n";



	if (radix < 2 || radix > 36)
		radix = 10;


    for (;;)
    {

		padc = ' ';
		width = 0;
		
		while ( ( ch = (u_char) *fmt++ ) != '%' || stop ) 
		{
			//if (ch == '')
			//	return (retval);
			
			if (ch == 0)
			    return (retval);
			
			PCHAR (ch);
		}
		
		percent = fmt - 1;
		
		qflag = 0; 
		lflag = 0; 
		ladjust = 0; 
		sharpflag = 0; 
		neg = 0;
		
		sign = 0; 
		dot = 0; 
		dwidth = 0; 
		upper = 0;
		
		cflag = 0; 
		hflag = 0; 
		jflag = 0; 
		tflag = 0; 
		zflag = 0;
		
        reswitch:  
    
	    switch ( ch = (u_char) *fmt++ ){
			
		case '.':
			dot = 1;
			goto reswitch;
			
		case '#':
			sharpflag = 1;
			goto reswitch;
			
		case '+':
			sign = 1;
			goto reswitch;
			
		case '-':
			ladjust = 1;
			goto reswitch;
			
		case '%':
			PCHAR(ch);
			break;
			
		case '*':
			if (!dot) 
			{
				width = va_arg(ap, int);
				
				if (width < 0)
				{
					ladjust = !ladjust;
					width = -width;
				};
				
			} else {
				
				dwidth = va_arg(ap, int);
			};
			goto reswitch;
			
		case '0':
			if (!dot) 
			{
				padc = '0';
				goto reswitch;
			}
			
		case '1': 
		case '2': 
		case '3': 
		case '4':
		case '5': 
		case '6': 
		case '7': 
		case '8': 
		case '9':
			for ( n = 0;; ++fmt ) 
			{
				n = n * 10 + ch - '0';
				ch = *fmt;
				
				if (ch < '0' || ch > '9')
					break;
			}
			
			if (dot)
				dwidth = n;
			else
				width = n;
			goto reswitch;
			
		case 'b':
			num = (u_int)va_arg(ap, int);
			p = va_arg(ap, char *);
			
			for (q = ksprintn(nbuf, num, *p++, NULL, 0); *q;)
				PCHAR(*q--);

			if (num == 0)
				break;

			for (tmp = 0; *p;) 
			{
				n = *p++;
				
				if (num & (1 << (n - 1))) 
				{
					PCHAR(tmp ? ',' : '<');
					
					for (; (n = *p) > ' '; ++p)
						PCHAR(n);
					tmp = 1;
				} else
					for (; *p > ' '; ++p)
						continue;
			};
			
			if (tmp)
				PCHAR('>');
			break;
			
		case 'c':
			PCHAR(va_arg(ap, int));
			break;
			
		case 'D':
			up = va_arg(ap, u_char *);
			p = va_arg(ap, char *);
			
			if (!width)
				width = 16;
			while(width--) 
			{
				PCHAR(hex2ascii(*up >> 4));
				PCHAR(hex2ascii(*up & 0x0f));
				up++;
				
				if (width)
					for (q=p;*q;q++)
						PCHAR(*q);
			};
			break;
			
		case 'd':
		case 'i':
			base = 10;
			sign = 1;
			goto handle_sign;
			
		case 'h':
			if (hflag) 
			{
				hflag = 0;
				cflag = 1;
			} else
				hflag = 1;
			goto reswitch;
			
		case 'j':
			jflag = 1;
			goto reswitch;
			
		case 'l':
			if (lflag) 
			{
				lflag = 0;
				qflag = 1;
			} else
				lflag = 1;
			goto reswitch;
			
		case 'n':
			if (jflag)
				*(va_arg(ap, intmax_t *)) = retval;
			else if (qflag)
				*(va_arg(ap, quad_t *)) = retval;
			else if (lflag)
				*(va_arg(ap, long *)) = retval;
			else if (zflag)
				*(va_arg(ap, size_t *)) = retval;
			else if (hflag)
				*(va_arg(ap, short *)) = retval;
			else if (cflag)
				*(va_arg(ap, char *)) = retval;
			else
				*(va_arg(ap, int *)) = retval;
			break;
			
		case 'o':
			base = 8;
			goto handle_nosign;
			
		case 'p':
			base = 16;
			sharpflag = (width == 0);
			sign = 0;
			num = (uintptr_t)va_arg(ap, void *);
			goto number;
			
		case 'q':
			qflag = 1;
			goto reswitch;
			
		case 'r':
			base = radix;
			if (sign)
				goto handle_sign;
			goto handle_nosign;
			
		case 's':
			p = va_arg(ap, char *);
			if (p == NULL)
				p = "(null)";
			if (!dot)
				n = stdio_strlen (p);
			else
				for (n = 0; n < dwidth && p[n]; n++)
					continue;

			width -= n;

			if (!ladjust && width > 0)
				while (width--)
					PCHAR(padc);
			while (n--)
				PCHAR(*p++);
			if (ladjust && width > 0)
				while (width--)
					PCHAR(padc);
			break;
			
		case 't':
			tflag = 1;
			goto reswitch;
			
		// ?? case 'U': ??
		case 'u':
			base = 10;
			goto handle_nosign;
			
		case 'X':
			upper = 1;
		case 'x':
			base = 16;
			goto handle_nosign;
			
		case 'y':
			base = 16;
			sign = 1;
			goto handle_sign;
			
		case 'z':
			zflag = 1;
			goto reswitch;
			
        handle_nosign:
			sign = 0;
			if (jflag)
				num = va_arg(ap, uintmax_t);
			else if (qflag)
				num = va_arg(ap, u_quad_t);
			else if (tflag)
				num = va_arg(ap, ptrdiff_t);
			else if (lflag)
				num = va_arg(ap, u_long);
			else if (zflag)
				num = va_arg(ap, size_t);
			else if (hflag)
				num = (u_short)va_arg(ap, int);
			else if (cflag)
				num = (u_char)va_arg(ap, int);
			else
				num = va_arg(ap, u_int);
			goto number;
			
        handle_sign:
			if (jflag)
				num = va_arg(ap, intmax_t);
			else if (qflag)
				num = va_arg(ap, quad_t);
			else if (tflag)
				num = va_arg(ap, ptrdiff_t);
			else if (lflag)
				num = va_arg(ap, long);
			else if (zflag)
				num = va_arg(ap, ssize_t);
			else if (hflag)
				num = (short)va_arg(ap, int);
			else if (cflag)
				num = (char)va_arg(ap, int);
			else
				num = va_arg(ap, int);
			
        number:
			if (sign && (intmax_t)num < 0) {
				neg = 1;
				num = -(intmax_t)num;
			}
			p = ksprintn(nbuf, num, base, &tmp, upper);
			if (sharpflag && num != 0) {
				if (base == 8)
					tmp++;
				else if (base == 16)
					tmp += 2;
			}
			if (neg)
				tmp++;

			if (!ladjust && padc != '0' && width
			    && (width -= tmp) > 0)
				while (width--)
					PCHAR(padc);
			if (neg)
				PCHAR('-');
			if (sharpflag && num != 0) {
				if (base == 8) {
					PCHAR('0');
				} else if (base == 16) {
					PCHAR('0');
					PCHAR('x');
				}
			}
			if (!ladjust && width && (width -= tmp) > 0)
				while (width--)
					PCHAR(padc);

			while (*p)
				PCHAR(*p--);

			if (ladjust && width && (width -= tmp) > 0)
				while (width--)
					PCHAR(padc);

			break;
			
		default:
			
			while (percent < fmt)
				PCHAR(*percent++);
			
			/* Since we ignore an formatting argument it is no
			 * longer safe to obey the remaining formatting
			 * arguments as the arguments will no longer match
			 * the format specs. */
			 
			stop = 1;
			break;
		};
	};
#undef PCHAR
};


static void xxxputchar ( int c, void *arg ){
	
	/* add your putchar here */
	
	//printf("%c",c);
    putchar ( (int) c );
}


/*
 *===========================================
 * printf:
 *     http://www.pagetable.com/?p=298
 */

int printf ( const char *fmt, ... ){
	
	// #todo
	// Talvez usar semáforo aqui.
	
    va_list ap;
    va_start(ap, fmt);

	//int 
	//kvprintf ( char const *fmt, 
    //       void (*func)( int, void* ), 
	//	   void *arg, 
	//	   int radix, //??10 gives octal; 20 gives hex.
	//	   va_list ap );	
	
	kvprintf ( fmt, xxxputchar, NULL, 10, ap );
	
	//#todo.
	va_end (ap);
	
	//#test
	//vamos pedir pro terminal virtual imprimir o conteúdo do buffer. 
	//MSG_TERMINALCOMMAND = 100
	//__SendMessageToProcess ( terminal___PID, 0, 100, 2008, 2008 );

    return 0;
}
//=============================================================
// printf end
//=============================================================


/*
 *===========================================
 * printf_draw:
 *     http://www.pagetable.com/?p=298
 */

// Nessa versão a rotina pintará na tela ao invés de
// colocar o char no arquivos.
// para isso ela configura o modo de output, voltando ao normal
// ao fim da rotina.
// >> Essa rotina é usada para debug no caso de não termos acesso
// a rotinas da api ou do x-server para pintura na tela.


int printf_draw ( const char *fmt, ... ){
	
	//Habilita o modo draw.
	libc_set_output_mode ( LIBC_DRAW_MODE );
	
	// #todo
	// Talvez usar semáforo aqui.
	
	va_list ap;
	va_start(ap, fmt);
	
	//int 
	//kvprintf ( char const *fmt, 
    //       void (*func)( int, void* ), 
	//	   void *arg, 
	//	   int radix, //??10 gives octal; 20 gives hex.
	//	   va_list ap );	
	
	kvprintf ( fmt, xxxputchar, NULL, 10, ap );
	
	//#todo.
	va_end (ap);
	
	
	//reabilita o modo normal. Onde os caracteres serão colocados 
	//no stdout.
	libc_set_output_mode ( LIBC_NORMAL_MODE );



    return 0;
}





/*glibc style*/
/* Optimizing.  */
/*
#ifdef	__OPTIMIZE__
extern __inline int vprintf (const char *__fmt, __gnuc_va_list __arg)
{
    if ( (void *) stdout == NULL )
       return EOF;

  return vfprintf (stdout, __fmt, __arg);
}
#endif 
*/



/*
 **********************
 * vfprintf:
 */

// #bugbug
// Estamos em ring3, não devemos acessar os elementos da estrutura de stream.


int 
vfprintf ( 
    FILE *stream, 
    const char *format, 
    stdio_va_list argptr )
{

	//#suspenso.
	//return (int) kvprintf ( format, NULL, stream->_ptr, 10, argptr );
	
    int size;

    //if ( (void *) stream == NULL )
       //return EOF;

    if ( (void *) stream == NULL )
    {
		return (int) (-1);
		
    } else {

		size = (int) stdio_strlen (format);
		
		//Se a string for maior que o tanto de bytes
		//disponíveis no arquivo
		if ( size > stream->_cnt )
		{
			return (int) (-1);
		}
		
		//recalcula o tanto de bytes disponíveis
		stream->_cnt = (int) (stream->_cnt - size);
		
		//coloca no buffer, usando o ponteiro atual.
		//#obs: isso provavelmente funcione, 
		//mas vamos tentar o segundo método.
		//sprintf ( stream->_ptr, format );
		//kvprintf ( format, NULL, stream->_ptr, 10, argptr );
		kvprintf ( format, NULL, stream->_p, 10, argptr );
		
		//atualiza o ponteiro atual.
		//stream->_ptr = stream->_ptr + size;
		stream->_p = stream->_p + size;
        
		return 0;
    };


    return (int) (-1);
} 


/* #bsd style */

int vprintf (const char *fmt, va_list ap){

    if ( (void *) stdout == NULL )
       return EOF;

    return vfprintf (stdout, fmt, ap);
}



//printf que escreve no stdout. 
//#bugbug: não devemos usar stream em ring3.
int stdout_printf (const char *format, ...){

    va_list arg;
    int done;

    va_start (arg, format);
    
    if ( (void *) stdout == NULL )
       return EOF;

    done = vfprintf (stdout, format, arg);
    va_end (arg);

    return done;
}


//printf que escreve no stderr. 
//#bugbug: não devemos usar stream em ring3.
int stderr_printf (const char *format, ... ){
	
    int done=0;

    va_list arg;
    va_start (arg, format);
    

    if ( (void *) stderr == NULL )
       return EOF;

    done = vfprintf (stderr, format, arg);

    va_end (arg);

    return done;
}



/*
 * perror:
 *
 */

// #todo:
// Oh Jees..., this function deserves a little bit of respect.

// The perror() function produces a message on standard error describing
// the last error encountered during a call to a system or library
// function.
// See: http://man7.org/linux/man-pages/man3/perror.3.html

void perror (const char *str){

    //#todo: check errno, and the string for the current error.
    printf ("FIXME: perror() \n");
    printf (str);
}


// O ponto de leitura e escrita volta a ser a base.
// Isso é válido apenas para a stream aqui em ring3.
void rewind (FILE *stream){


    if( (void*) stream == NULL )
        return;
        
    //:^)
    stream->_p = stream->_base;

	//unix 32V
	//fflush(stream);
	//lseek( fileno(stream), 0, 0);
	//stream->_p = stream->_base;
	//stream->_flag &= ~(_IOERR|_IOEOF);

    //se glibc
    //clearerr(stream);
    //(void) fseek(stream, 0L, SEEK_SET);
    //clearerr(stream);
}


/*
void rewind(FILE* stream)
{
    ASSERT(stream);
    int rc = fseek(stream, 0, SEEK_SET);
    ASSERT(rc == 0);
}
*/



/*
static void buffer_putch(char*& bufptr, char ch)
{
    *bufptr++ = ch;
}
*/


/*
static size_t __vsnprintf_space_remaining;
static void sized_buffer_putch(char*& bufptr, char ch)
{
    if (__vsnprintf_space_remaining) {
        *bufptr++ = ch;
        --__vsnprintf_space_remaining;
    }
}
*/






// #todo
int snprintf ( char *str, size_t count, const char *fmt, ... ){

    size_t ret;

    va_list ap;
    va_start (ap, fmt);


	//#todo 
	//Isso parece fácil
	//ret = vsnprintf(str, count, fmt, ap);


    va_end (ap);
    
    return ret;
}


//?? #todo
// inicializa o fluxo padrão para o processo.
int 
stdio_initialize_standard_streams (void)
{

    return (int) gramado_system_call ( 700, 
                     (unsigned long) stdin, 
                     (unsigned long) stdout, 
                     (unsigned long) stderr ); 
}



// wtf ... it looks so cool!
/*
file *file_purpose (file *f); 
file *file_purpose (file *f) 
{
	if (f->purpose == purpose_recv) {
		return stdin;
	}
	else if (f->purpose == purpose_send) {
		return stdout;
	}
	else if (f->purpose == (purpose_send | purpose_dbg)) {
		return dbgout;
	}
	else if (f->purpose == (purpose_send | purpose_err)) {
		return stderr;
	}
	else {
		// TODO: Warn about unknown file for pipe. Default to stderr
		return stderr;
	}
}
*/






/*credits: bsd*/
/*
 * Return the (stdio) flags for a given mode.  Store the flags
 * to be passed to an open() syscall through *optr.
 * Return 0 on error.
 */

/*
int
__sflags(const char *mode, int *optr);
int
__sflags(const char *mode, int *optr)
{
	int ret, m, o;

	//#todo: Falta essa macro. ela fica em assert.h
	//_DIAGASSERT(mode != NULL);

	switch (*mode++) {

	case 'r':	// open for reading 
		ret = __SRD;
		m = O_RDONLY;
		o = 0;
		break;

	case 'w':	// open for writing 
		ret = __SWR;
		m = O_WRONLY;
		o = O_CREAT | O_TRUNC;
		break;

	case 'a':	// open for appending 
		ret = __SWR;
		m = O_WRONLY;
		o = O_CREAT | O_APPEND;
		break;

	default:	// illegal mode 
		errno = EINVAL;
		return 0;
	}

	
	 //[rwa]\+ or [rwa]b\+ means read and write 
	 //e means set close on exec.
	 //f means open only plain files.
	 //l means don't follow symlinks.
	 //x means exclusive open.
	 
	for (; *mode; mode++)
		switch (*mode) {
		case '+':
			ret = __SRW;
			m = O_RDWR;
			break;
		case 'b':
			break;
		case 'e':
			o |= O_CLOEXEC;
			break;
		case 'f':
			o |= O_REGULAR;
			break;
		case 'l':
			o |= O_NOFOLLOW;
			break;
		case 'x':
			o |= O_EXCL;
			break;
		default:	// We could produce a warning here 
			break;
		}

	*optr = m | o;
	return ret;
}
*/


/*credits: bsd*/
/*
int
_fwalk(int (*function)(FILE *));
int
_fwalk(int (*function)(FILE *))
{
	FILE *fp;
	int n, ret;
	struct glue *g; //falta isso.

    //#todo: Falta essa macro. ela fica em assert.h
	_DIAGASSERT(function != NULL);

	ret = 0;
	for (g = &__sglue; g != NULL; g = g->next)
		for (fp = g->iobs, n = g->niobs; --n >= 0; fp++)
			if (fp->_flags != 0)
				ret |= (*function)(fp);
	return ret;
}
*/


/* 
 * apiStartTerminal:
 *     
 */

// 'Clona' e executa o noraterm como processo filho. 
// registra o terminal noraterm como terminal atual.
// pega o pid do terminal atual
// manda uma mensagem pedindo para o terminal dizer hello.

// #obs: Isso funcionou.

// #bugbug
// Maybe it's libc isn't the better place for this fucntion.
// Try libgui.

int libcStartTerminal (void){

    int PID;

    // 'Clona' e executa o noraterm como processo filho. 
    //PID = (int) system_call ( 900, (unsigned long) "noraterm.bin", 0, 0 );

    PID = (int) gramado_system_call ( 900, (unsigned long) "noraterm.bin", 0, 0 );

    // Exibe o PID para debug.
    //printf ("PID = %d \n", PID);

    //registra o terminal como terminal atual.
    gramado_system_call ( 1003, PID, 0, 0 ); 

    //invalida a variável.
    PID = -1;

    //pega o pid do terminal atual
    PID = (int) gramado_system_call ( 1004, 0, 0, 0 ); 

    if ( PID <= 0 ){
		printf ("PID fail. We can't send the message\n");
	    return -1;
	}


	//manda uma mensagem pedindo para o terminal dizer hello.
	//__SendMessageToProcess ( PID, NULL, MSG_TERMINALCOMMAND, 2001, 2001 );

	return (int) PID;
}



//
// stream buffer support
//

// see: 
// https://linux.die.net/man/3/setvbuf

void setbuf (FILE *stream, char *buf)
{
    debug_print ("setbuf: [TODO]\n");
    //setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
}


//If buf is a null pointer, this function makes stream unbuffered. 
//Otherwise, it makes stream fully buffered using buf as the buffer. 
//The size argument specifies the length of buf.
//This function is provided for compatibility with old BSD code. 
//Use setvbuf instead. 

void setbuffer (FILE *stream, char *buf, size_t size){

    if ( (void *) stream == NULL )
       return;

    gramado_system_call ( 611, 
        (unsigned long) stream, 
        (unsigned long) buf, 
        (unsigned long) size ); 
}


//This function makes stream be line buffered, and allocates the buffer for you.
//This function is provided for compatibility with old BSD code. 
//Use setvbuf instead. 

void setlinebuf (FILE *stream)
{
    debug_print ("setlinebuf: [TODO]\n");
    //setvbuf(stream, nullptr, _IOLBF, 0);
}


//The setvbuf() function may be used on any open stream to change its buffer.
//If the argument buf is NULL, only the mode is affected; 
//a new buffer will be allocated on the next read or write operation. 
// See: https://linux.die.net/man/3/setvbuf

int setvbuf (FILE *stream, char *buf, int mode, size_t size){

    if (size <= 0)
        return (-1);


    // #bugbug
    // Esse tratamanto de bits pode estar errado.
    // unbuffered, line buffered, fully buffered 
    if ( mode != _IONBF && 
         mode != _IOLBF && 
         mode != _IOFBF )
    {
        debug_print ("setvbuf: mode \n");
        //errno = EINVAL;
        return -1;
    }


    /*
    if (mode == _IONBF)
    {
        // Nesse caso. 
        // Limpa a estrutura e buffer nulo.
    }
    */

    // #todo size limits.


    //if (mode == _IOFBF || mode == _IOLBF){


    // Valid buffer.
    // Se foi passado um buffer válido.
    if ( (void *) buf != NULL ){
        debug_print ("setvbuf: using new buffer \n");
        stream->_base = (char *) buf;
        stream->_lbfsize = size;
    } 

    // Invalid buffer.    
    // Nenhum buffer foi passado.
    // Devemos criar um. Vamos usar o default.
    if ( (void *) buf == NULL ){
        debug_print ("setvbuf: using default buffer \n");
        stream->_base = (char *) stream->default_buffer;
        stream->_lbfsize = BUFSIZ;
    }

    //}



    // #bugbug
    // E se nesse momento ainda temos um ponteiro nulo para buffer?

    // r/w pointers and offsets.
    stream->_p = stream->_base;
    stream->_w = 0;
    stream->_r = 0;
    
    // Unset any buffering flag
    //stream->_flags &= ~(_IOFBF | _IOLBF | _IONBF);

    // And set what the user requested
    stream->_flags |= mode;

    return 0;
}


unsigned int filesize (FILE * fp){

    if (!fp) 
        return 0;

    fseek (fp, 0, SEEK_END);

    unsigned int ret = (unsigned int) ftell(fp);

    rewind (fp);


    return ret;
}




char *fileread (FILE * fp){

    if (!fp) 
        return 0;

    unsigned int buffer_size = filesize (fp);

    char *buff = (char *) malloc (buffer_size);

    fread (buff, sizeof(char), buffer_size, fp);

    return buff;
}



int dprintf (int fd, const char *format, ...)
{ 
    debug_print ("dprintf: [TODO]\n");
	return -1; 
}



int vdprintf (int fd, const char *format, va_list ap)
{ 
    debug_print ("vdprintf: [TODO]\n");
	return -1; 
}

//
//==================================================================
//

/* we use this so that we can do without the ctype library */
#define __is_digit(c)	((c) >= '0' && (c) <= '9')

static int skip_atoi(const char **s)
{
	int i=0;

	while (__is_digit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

#define ZEROPAD  1   /* pad with zero */
#define SIGN     2   /* unsigned/signed long */
#define PLUS     4   /* show plus */
#define SPACE    8   /* space if plus */
#define LEFT     16  /* left justified */
#define SPECIAL  32  /* 0x */
#define SMALL    64  /* use 'abcdef' instead of 'ABCDEF' */


// oh Jeess ? x86 stuff.
#define do_div(n,base) ({ \
int __res; \
__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
__res; })


static char *
number ( 
    char *str, 
    int num, 
    int base, 
    int size, 
    int precision, 
    int type )
{
    
    char c, sign, tmp[36];
    const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i=0;


    if (type&SMALL) digits="0123456789abcdefghijklmnopqrstuvwxyz";
    if (type&LEFT) type &= ~ZEROPAD;
    if (base<2 || base>36)
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ' ;
	if (type&SIGN && num<0) {
		sign='-';
		num = -num;
	} else
		sign=(type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);
	if (sign) size--;
	if (type&SPECIAL)
		if (base==16) size -= 2;
		else if (base==8) size--;
	i=0;
	if (num==0)
		tmp[i++]='0';
	else while (num!=0)
		tmp[i++]=digits[do_div(num,base)];
	if (i>precision) precision=i;
	size -= precision;
	if (!(type&(ZEROPAD+LEFT)))
		while(size-->0)
			*str++ = ' ';
	if (sign)
		*str++ = sign;
	if (type&SPECIAL)
		if (base==8)
			*str++ = '0';
		else if (base==16) {
			*str++ = '0';
			*str++ = digits[33];
		}
	if (!(type&LEFT))
		while(size-->0)
			*str++ = c;
	while(i<precision--)
		*str++ = '0';
	while(i-->0)
		*str++ = tmp[i];
	while(size-->0)
		*str++ = ' ';
	return str;
}


// It was taken from linux 0.01.
// It works yet.
// Just for fun. :^) 

int 
Wirzenius_Torvalds_vsprintf (
    char *buf, 
    const char *fmt, 
    va_list args )
{
	int len;
	int i;
	char * str;
	char *s;
	int *ip;


    /* flags to number() */
    int flags;		

	
    /* width of output field */
    int field_width;	
	
    // min. # of digits for integers; 
    // max number of chars for from string.
    int precision;		
				   
    /* 'h', 'l', or 'L' for integer fields */
    int qualifier;		


	for (str=buf ; *fmt ; ++fmt)
    {
		if (*fmt != '%')
        {
			*str++ = *fmt;
			continue;
		}

	
		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
				}
		
		/* get field width */
		field_width = -1;
		if (__is_digit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;	
			if (__is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
			qualifier = *fmt;
			++fmt;
		}

		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) va_arg(args, int);
			while (--field_width > 0)
				*str++ = ' ';
			break;

		case 's':
			s = va_arg(args, char *);
			len = strlen(s);
			if (precision < 0)
				precision = len;
			else if (len > precision)
				len = precision;

			if (!(flags & LEFT))
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; ++i)
				*str++ = *s++;
			while (len < field_width--)
				*str++ = ' ';
			break;

		case 'o':
			str = number(str, va_arg(args, unsigned long), 8,
				field_width, precision, flags);
			break;

		case 'p':
			if (field_width == -1) {
				field_width = 8;
				flags |= ZEROPAD;
			}
			str = number(str,
				(unsigned long) va_arg(args, void *), 16,
				field_width, precision, flags);
			break;

		case 'x':
			flags |= SMALL;
		case 'X':
			str = number(str, va_arg(args, unsigned long), 16,
				field_width, precision, flags);
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			str = number(str, va_arg(args, unsigned long), 10,
				field_width, precision, flags);
			break;

		case 'n':
			ip = va_arg(args, int *);
			*ip = (str - buf);
			break;

		default:
			if (*fmt != '%')
				*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			break;
		}
	}
	*str = '\0';
	return str-buf;
}


// It's used by printf ?
static char __printbuf[1024];


// It was taken from linux 0.01.
// It works yet.
// Just for fun. :^) 

int Torvalds_printf (const char *fmt, ...){

    va_list args;
    va_start(args, fmt);

    //int this_tty_id = 0;
    int i=0;

    // Pegando o id da tty desse processo.
    // #obs: Isso ja foi feito na inicialização da biblioteca.
    // Deltar isso.
    //this_tty_id = (int) gramado_system_call ( 266, getpid(), 0, 0 );  


    //escrevendo no virtual console. 0.
	write_VC ( 0, 
	    __printbuf,
	    i=Wirzenius_Torvalds_vsprintf(__printbuf, fmt, args) );

	//write ( this_tty_id, 
	//    __printbuf,
	//    i=Wirzenius_Torvalds_vsprintf(__printbuf, fmt, args) );



    // #todo: talvez seja possível usar a base da stream.
    //write ( this_tty_id, 
        //__printbuf,
        //i=Wirzenius_Torvalds_vsprintf(stdout->_base, fmt, args) );


    va_end(args);
    return (int) i;
}





//
//==================================================================
//

int 
vsnprintf ( 
    char *str, 
    size_t size, 
    const char *format, 
    va_list ap )
{
    debug_print ("vsnprintf: [TODO]\n");
	return -1; 
}


int vscanf (const char *format, va_list ap)
{ 
    debug_print ("vscanf: [TODO]\n");
	return -1; 
}


int vsscanf (const char *str, const char *format, va_list ap)
{ 
    debug_print ("vsscanf: [TODO]\n");
	return -1; 
}


int vfscanf (FILE *stream, const char *format, va_list ap)
{ 
    debug_print ("vfscanf: [TODO]\n");

    if ( (void *) stream == NULL )
       return EOF;

    return -1; 
}



FILE *tmpfile (void){

    debug_print ("tmpfile: [TODO]\n");

    // steps:
    // + Salva um arquivo vazio com nome randômico.
    // + Abre esse arquivo com open.
    // + Cria uma stream para esse fd.

	return (FILE *) 0;
}


//tmpnam(): 
//SVr4, 4.3BSD, C89, C99, POSIX.1-2001.  
//POSIX.1-2008 marks tmpnam() as obsolete.
char *tmpnam(char *s){

    debug_print ("tmpnam: [TODO]\n");
	return NULL; 
}




//tmpnam_r() is a nonstandard extension that is 
//also available on a few other systems.
char *tmpnam_r (char *s){

    debug_print ("tmpnam_r: [TODO]\n");
	return NULL; 
}



char *tempnam (const char *dir, const char *pfx){

    debug_print ("tempnam: [TODO]\n"); 
	return NULL; 
}




/*
 ERRORS  for tmpfile();

       EACCES Search permission denied for directory in file's path prefix.

       EEXIST Unable to generate a unique filename.

       EINTR  The call was interrupted by a signal; see signal(7).

       EMFILE The per-process limit on the number of open file descriptors
              has been reached.

       ENFILE The system-wide limit on the total number of open files has
              been reached.

       ENOSPC There was no room in the directory to add the new filename.

       EROFS  Read-only filesystem.

 */


FILE *stdio_make_file( int fd, const char *mode ){

    FILE *__file;

    
    __file = (FILE *) malloc ( sizeof(FILE) );

    if ( (void *) __file == NULL )
        return NULL;
        
 
    __file->used = 1;
    __file->magic = 1234;
    __file->_cnt = 0;  
    __file->_file = fd;
    
    // flags
    __file->_flags = 0;
    
    /*
    switch (*mode) {

    case 'r':
        __file->_flags |= _IOREAD;
        break;

    case 'a':
        lseek(fd, 0L, 2);
        //No break
    case 'w':
        __file->_flags |= _IOWRT;
        break;

    default:
        return(NULL);
    }
    */


    /*
	if (mode[1] == '+') {
		__file->_flags &= ~(_IOREAD|_IOWRT);
		__file->_flags |= _IORW;
	}
    */

    //__file->_tmpfname = (char *) strdup(filename);

    __file->_base = NULL;    // ??

    
    __file->_r = 0;
    __file->_w = 0;

   
    //#todo: Initializa structure.
    //...

    return (FILE *) __file;
}


// #test
// Cria uma nova stream para o fd.
// O fd foi obtido anteriormente,
FILE *fdopen (int fd, const char *mode)
{
    if(fd<0)
        return NULL;

        
    return (FILE *) stdio_make_file (fd, (const char *) mode);
}



FILE *freopen (const char *pathname, const char *mode, FILE *stream)
{

    debug_print ("freopen: TODO: \n");

    if ( (void *) stream == NULL )
       return (FILE *) 0;



	return (FILE *) 0;
}



/* #todo it needs 'cookie_io_functions_t'
FILE *fopencookie(void *cookie, const char *mode,
                         cookie_io_functions_t io_funcs);
FILE *fopencookie(void *cookie, const char *mode,
                         cookie_io_functions_t io_funcs)
{
    debug_print ("fopencookie: TODO: \n");
	return (FILE *) 0;
}
*/




FILE *open_memstream (char **ptr, size_t *sizeloc)
{
    debug_print ("open_memstream: TODO: \n");
	return (FILE *) 0;
}


    

FILE *open_wmemstream (wchar_t **ptr, size_t *sizeloc)
{
    debug_print ("open_wmemstream: TODO: \n");
	return (FILE *) 0;
}



FILE *fmemopen (void *buf, size_t size, const char *mode)
{
    debug_print ("fmemopen: TODO: \n");
	return (FILE *) 0;
}



/*
int fgetpos(FILE* stream, fpos_t* pos)
{
    assert(stream);
    assert(pos);

    long val = ftell(stream);
    if (val == -1L)
        return 1;

    *pos = val;
    return 0;
} 
*/


int fgetpos (FILE *stream, fpos_t *pos ){ 

    if ( (void *) stream == NULL )
       return EOF;

    // #todo
    /*
    if ( !__validfp(stream) )
    {
        errno = EINVAL;
        return EOF;
    }
    */
    
    *pos = ftell (stream);

    if (*pos < 0L)
        return(-1);
 
    return(0);
}


int fsetpos (FILE *stream, const fpos_t *pos){
 
    if ( (void *) stream == NULL )
       return EOF;

    return fseek (stream, (long) *pos, SEEK_SET);
}



int fpurge (FILE *stream){

    debug_print ("fpurge: TODO: \n");
    
    if ( (void *) stream == NULL )
       return EOF;
 
    return -1; 
}




//#todo: esse protótipo pertence à stdio_ext.h
void  __fpurge (FILE *stream)
{
    debug_print ("__fpurge: TODO: \n");

    if ( (void *) stream == NULL )
       return;
}


//ctermid - get controlling terminal name
//POSIX.1-2001, POSIX.1-2008, Svr4.
char *ctermid (char *s)
{
    debug_print ("ctermid: TODO: \n");	
    return NULL; 
}


/*
 **************************************************
 * stdioInitialize:
 *     Inicializa stdio para usar o fluxo padrão.
 *     O retorno deve ser (int) e falhar caso dê algo errado.
 */

// #bugbug
// Essa estrutura lida com elementos de estrutura em ring3.
// #atenção: Algumas rotinas importantes estão usando esses elementos.


// #bugbug
// Precisamos usar os arquivos herdados do processo pai.
// Eles estão na estrutura de processo desse processo.

void stdioInitialize (){


	// Buffers para as estruturas.
    unsigned char buffer0[BUFSIZ];
    unsigned char buffer1[BUFSIZ];
    unsigned char buffer2[BUFSIZ];

    // Buffers.
    unsigned char buffer0_data[BUFSIZ];
    unsigned char buffer1_data[BUFSIZ];
    unsigned char buffer2_data[BUFSIZ];


    int status = 0;
    int i=0;


    //debug_print ("stdioInitialize: TODO: \n");  
    
    //
    // Pointers.
    //    
    
    stdin  = (FILE *) &buffer0[0];
    stdout = (FILE *) &buffer1[0];
    stderr = (FILE *) &buffer2[0];


    // Buffers.
    stdin->_base  = &buffer0_data[0];
    stdout->_base = &buffer1_data[0];
    stderr->_base = &buffer2_data[0];

    // p
    stdin->_p  = stdin->_base;
    stdout->_p = stdout->_base;
    stderr->_p = stderr->_base;

    // cnt    
    //stdin->_cnt  = BUFSIZ;
    //stdout->_cnt = BUFSIZ;
    //stderr->_cnt = BUFSIZ;    
    
    
    //#todo
    //stdin->_lbfsize  = 128;
    //stdout->_lbfsize = 128;
    //stderr->_lbfsize = 128;    

    stdin->_w  = 0;
    stdout->_w = 0;
    stderr->_w = 0;    

    stdin->_r  = 0;
    stdout->_r = 0;
    stderr->_r = 0;    

    
    // o kernel ainda não sabe disso.
    stdin->_file = 0;
    stdout->_file = 1;
    stderr->_file = 2;
    
	
	//
	// # libc mode #
	//
	
	// #bugbug:
	// Vamos usar o modo draw até implementarmos o modo normal.
	
	//Os caracteres são colocados em stdout.
    //__libc_output_mode = LIBC_NORMAL_MODE;
	
	//Os caracteres são pintados na tela.
	__libc_output_mode = LIBC_DRAW_MODE;
	

      // #importante:
      // Vamos conectar o processo filho ao processo pai
      // atraves das ttys dos processos.
      // o processo pai eh o terminal.
      // #bugbug:
      // Esse metodo nao funcionara no caso
      // do processo filho do shell
      
      gramado_system_call ( 267,
           getpid(),    //master
           getppid(),   //slave pai(terminal)
           0 );


    
    __libc_tty_id = (int) gramado_system_call ( 266, getpid(), 0, 0 );        


	// Alocando espaço para as estruturas.
	// Mas não usaremos a estrutura em ring3, somente o ponteiro.
	// O ponteiro apontará para um estrutura em ring0.

	//stdin  = (FILE *) &buffer0[0];
	//stdout = (FILE *) &buffer1[0];
	//stderr = (FILE *) &buffer2[0];


	// #bugbug
	//precisamos usar ponteiros de arquivos
	//que estão em ring0, senão fprintf não funcionará;
	
    //
    // stdout.
    //
	
	// Pegamos uma stdout
	// Isso está pegando a stdout da tty atual. CurrentTTY->stdout.
	// Mas deveríamos pegar o stdout na estrutura de processo.
	

    //_fp = (FILE *) gramado_system_call ( 1000, getpid(), 0, 0 );
    
    
    // pegando s stream na lista de arquivos do processo.
    // O ponteiro apontará para um estrutura em ring0.
    // service, pid, fd, 0.
    
    //stdin  = (FILE *) gramado_system_call ( 167, getpid(), 0, 0 );
    //stdout = (FILE *) gramado_system_call ( 167, getpid(), 1, 0 );
    //stderr = (FILE *) gramado_system_call ( 167, getpid(), 2, 0 );


    // limpando o prompt;
    prompt_clean();


	// #bugbug
	// talvez seja melhor o aplicativo decidir se precisa de um terminal ou não.
	// talvez o aplicativo ou o crt0.o precisam configurar o terminal
	// o terminal será configurado de acordo com o tipo de crt0.o.
	
	/*
	//Inicialziamos o terminal que serr'usado pelo aplicativo.
	//terminal___PID = (int) apiStartTerminal ();
	

    // 'Clona' e executa o noraterm como processo filho. 
    terminal___PID = (int) system_call ( 900, (unsigned long) "noraterm.bin", 0, 0 );


		
	// Exibe o PID para debug.
	//printf ("PID = %d \n", PID);

    //registra o terminal como terminal atual.
	system_call ( 1003, terminal___PID, 0, 0 ); 
		
	//invalida a variável.
	terminal___PID = -1;
		
	//pega o pid do terminal atual
	terminal___PID = (int) system_call ( 1004, 0, 0, 0 ); 
		
    if ( terminal___PID <= 0 )
	{
        printf ("stdioInitialize: PID fail. We can't start terminal for this app *hang\n");
		while(1){}
    }
	*/
	
	
	
	// # fluxo padrão.
	// Aqui temos os ponteiros em ring3. Mas precisamos
	// configurar os ponteiros que estão na estrutura do processo em ring0.

    // #importante
    // Esses arquivos em ring3 devem ser tratados como buffers.
	// fflush deve copiar o conteúdo desses arquivos
	// para os arquivos do fluxo padrão que são gerenciados pela
	// estrutura do processo.

	
	
	// #importante
	// #teste:
	// vamos chamar uma systemcall que coloca na
	// estrutura do processo atual esses ponteiros
	// para o fluxo padrão.
	
	

    
	/*
	status =  (int) gramado_system_call ( 700, 
					(unsigned long) stdin, 
					(unsigned long) stdout, 
					(unsigned long) stderr ); 
    */
	
	/* Initialize the Process Standard Streams */
	
	//status = stdio_initialize_standard_streams ();
	
	/*
	//#todo
	if (status != 0)
    {
	}
	*/
	
	
	/*
	// >>>> #bugbug
	//  Em ring 3 não temos acesso aos elementos da estrutura.

	// A biblioteca tem 3 pequenos buffers,
	// que serão usados como base para os stream.
	// ?? Podemos almentar esses buffers ?? @todo: testar.

	//stdin - Usando o buffer 'prompt[.]' como arquivo.
	stdin->_base = &prompt[0];
	
	//stdin->_ptr = stdin->_base;
	stdin->_p = stdin->_base;
	
	//stdin->_bufsiz = PROMPT_MAX_DEFAULT; 
	stdin->_lbfsize = PROMPT_MAX_DEFAULT; 
	
	//stdin->_cnt = stdin->_bufsiz;
	stdin->_cnt = stdin->_lbfsize;
	
	stdin->_file = 0;
	stdin->_tmpfname = "stdin";
	//...
	*/
 
	
	/*
	stdout->_base = &prompt_out[0];
	stdout->_p = stdout->_base;
	stdout->_lbfsize = PROMPT_MAX_DEFAULT; 
	*/
	
	
	//stdin->_cnt = stdout->_lbfsize;	
	
	/*
	stdout->_file = 1;
	stdout->_tmpfname = "stdout";
	*/
	//...
	
	//stderr - Usando o buffer 'prompt_err[.]' como arquivo.
	//stderr->_base = &prompt_err[0];
	
	//stderr->_ptr = stderr->_base;
	//stderr->_p = stderr->_base;
	
	//stderr->_bufsiz = PROMPT_MAX_DEFAULT; 
	//stderr->_lbfsize = PROMPT_MAX_DEFAULT; 	
	
	//stdin->_cnt = stderr->_bufsiz;
	//stdin->_cnt = stderr->_lbfsize;	
	//stderr->_file = 2;
	//stderr->_tmpfname = "stderr";	
	//...
	
	// Limpando os buffers.
    
    /*
	for ( i=0; i < BUFSIZ; i++ )
	{
	    stdin->_base[i] = (char) '\0';
	    //stdout->_base[i] = (char) '\0';
	    stderr->_base[i] = (char) '\0';
	};
	*/
	
    //stdin->_ptr = stdin->_base;	
    //stdin->_p = stdin->_base;		
    
	//stdin->_bufsiz = BUFSIZ; 
    //stdin->_lbfsize = BUFSIZ; 
	
	
	//stdin->_cnt = stdin->_bufsiz;
	//stdin->_cnt = stdin->_lbfsize;
	

	/*
    stdout->_p = stdout->_base;
	stdout->_lbfsize = BUFSIZ; 
	stdout->_cnt = stdout->_lbfsize;
	*/
	
    //stderr->_ptr = stderr->_base;
    //stderr->_p = stderr->_base;
    //stderr->_bufsiz = BUFSIZ; 
    //stderr->_lbfsize = BUFSIZ; 
	
	
	//stderr->_cnt = stderr->_bufsiz;
	//stderr->_cnt = stderr->_lbfsize;
}



//
// End.
//


