/*
 * File: memmap.h
 *
 * Descri��o:
 *     Header para informa��es sobre a mem�ria. 
 *     Refer�ncia para localiza��o de VBR, FAT, ROOT, DATAAREA e outros.
 *
 * ATEN��O: 
 *     Kernel base = 0x00100000.
 *     vbr  = 0x001A0000;
 *     fat  = 0x001B0000;
 *     root = 0x001C0000;
 *
 *     Cuidado na escolha dos endere�os para fat, root.
 *     @todo Deixar espa�o o bastante para um kernel grande. 
 *           Come�ando em 0x00100000
 *
 * Vers�o 1.0, 2015 - Created.
 * Vers�o 1.0, Oct 2016 - Revision. 
 */
 

/*
    Memory map.
	Para o processo do kernel.
	Obs: Algumas �reas ser�o comuns entre processos
	     Como o LFB, o BackBuffer, o Kernel. 
	
    Mapeamento padr�o de mem�ria virtual para todos os processos:
    ============================================================	


		     +------------------------------------+
	FFFFFFFF |             FIM                    |
		     +------------------------------------+
		     +------------------------------------+
		     +------------------------------------+ 
		     +------------------------------------+
	         |         User Mode access           | @todo: Mudar de lugar.  
		     |                                    |        Seder espa�o para lfb.  
		     |                                    |
    C0800000 |           Back Buffer              |  			 
		     +------------------------------------+	
	         |        User Mode access            |	 Mem�ria da placa de v�deo.
		     |             (4MB)                  |  @todo Ampliar (PRECISA SER MAIOR)
			 |             ...                    |  obs: Tamanho do monitor.
	C0400000 |             LFB                    |
		     +------------------------------------+
			 +====================================+
             |           Kernel land              |
	         |                                    | 
             |  Stack = 0xC0300000 ~ 0xC03FFFF0   | Pilha do Kernel. 
	         |  Heap  = 0xC0100000 ~ 0xC02FFFF0   |	Obs: O Heap do kernel precisa ser maior.
             |                                    | 			 
			 |  Kernel Entry point = 0xC0001000   |      Entry point do kernel.
	         |  Kernel Base = 0xC0000000          |	     In�cio da im�gem do processo kernel. 
	C0000000 |         Kernel Mode access         |	 	   
	         +------------------------------------+
             |           User Land                |	 
	         |                                    |
             |  Stack = 0x40300000 ~ 0x403FFFF0   | @todo  In�cio da pilha em user mode do proesso.
	         |  Heap  = 0x40100000 ~ 0x402FFFF0   | @todo: In�cio do heap em user mode do processo.
             |                                    | ### Por enquando cada processo tem sua pr�pria
             |                                    |     pilha e heap no fim da imagem do processo.   			 
             | 40000000 = Process Base.           | ??
			 |                                    |
			 | 00041000 = Process entry point     | Entrypoint da imagem.
			 | 00040000 = Process image base      | Onde se carrega uma imagem de processo.
             |                                    |  			 
             | 00000000 = Dinamic Library Base    |
			 | 00000000 = Dinamic Library image   |
			 |                                    |
             |              ...                   | @todo: ampliar heap.   			 
	00000000 |       User Mode access             |	 
			 +====================================+			 
			 
 
 
*/


// 
// Bug Bug !! 
// @todo: 
//     Endere�os n�o implementados, usando VBR ?? por enquanto.
//
//

#define MBR_ADDRESS     0x001A0000
#define FS_MBR_ADDRESS  0x001A0000
 
#define FS_VBR_ADDRESS      0x001A0000 
#define FS_FAT_ADDRESS      0x001B0000 
#define FS_ROOTDIR_ADDRESS  0x001C0000 


//
// Fat support. fat32, fat16, fat12.
//

#define FAT12_VBR_ADDRESS     FS_VBR_ADDRESS
#define FAT12_FAT_ADDRESS     FS_FAT_ADDRESS
#define FAT12_ROOTDIR_ADDRESS FS_ROOTDIR_ADDRESS
#define FAT16_VBR_ADDRESS     FS_VBR_ADDRESS
#define FAT16_FAT_ADDRESS     FS_FAT_ADDRESS
#define FAT16_ROOTDIR_ADDRESS FS_ROOTDIR_ADDRESS
#define FAT32_VBR_ADDRESS     FS_VBR_ADDRESS
#define FAT32_FAT_ADDRESS     FS_FAT_ADDRESS
#define FAT32_ROOTDIR_ADDRESS FS_ROOTDIR_ADDRESS


/*
 * Refer�ncias para mem�ria de v�deo.
 */
#define BL_TEXT_MODE_BASE    0x000B8000
#define BL_GRAPHIC_MODE_BASE 0x000A0000

//
// End.
//

