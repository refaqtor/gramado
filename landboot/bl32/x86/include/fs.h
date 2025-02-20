/*
 * File: fs.h 
 *
 * Descri��o:
 *    Header do sistema de arquivos do Boot Loader.
 *
 * 2015 - Created by Fred Nora.
 */


#ifndef ___FS_H
#define ___FS_H    1



#define SECTOR_SIZE 512 
//#define SECTOR_SIZE 4096  //Novo padr�o. 


//
// Cluster Meaning
//

//FAT Code Range	Meaning
//0000h	Available Cluster
//0002h-FFEFh	Used, Next Cluster in File
//FFF0h-FFF6h	Reserved Cluster
//FFF7h	BAD Cluster
//FFF8h-FFFF	Used, Last Cluster in File

//#define FAT16_CLUSTER_AVAILABLE  0x0000
//#define FAT16_CLUSTER_RESERVED   0xFFF0
//#define FAT16_CLUSTER_BAD        0xFFF7
//#define FAT16_CLUSTER_LAST       0xFFFF


//teste 
#define FAT_ERRO_1   0xffff9     //N�o h� entradas vazias. 
#define FAT_ERRO_2   0xffff8   
#define FAT_ERRO_3   0xffff7
#define FAT_ERRO_4   0xffff6
#define FAT_STATUS_1 0xffff5    //N�o h� mais entradas cheias.
#define FAT_STATUS_2 0xffff4
#define FAT_STATUS_3 0xffff3
#define FAT_STATUS_4 0xffff2



//
// #importante:
// Status de carregamento.
// Se o diret�rio raiz j� est� carregado na mem�ria.
// Se a FAT j� est� carregada na mem�ria.
//

int g_fat16_root_status;
int g_fat16_fat_status;

// 
// Tipo de sistema de arquivos. (Ex: FAT16, EXT2 ...). 
//

int g_file_system_type;

// 
// Buffer para salvar uma entrada de diret�rio.
//

char buffer_dir_entry[512];    //@todo: Tamanho da entrada? Desperd�cio?   

//
// Lista de clusters em um arquivo.
//

unsigned short file_cluster_list[1024];    //@todo: Tamanho de arquivos ?



/*
 * partition_table_d:
 *     Structure for partition table.
 *   
 * @todo:    
 *     Na verdade essa � uma estrutura para um entrada na tabela de 
 * parti��es do MBR.
 */ 

struct partition_table_d
{
    unsigned char boot_indicator;    //80h, active.
    unsigned char start_chs[3];
    unsigned char partition_type;
    unsigned char end_chs[3];
    unsigned long start_sector;
    unsigned long partition_size;    //In sectors.
};
struct partition_table_d partition;
//...


/*
 * Prot�tipos.
 */


//
// File System initialization support.
//


int fsInit();
void fsInitStructures();
void fsInitFat();

//
//  Cluster and LBA support. (Disc parameters)
//


unsigned long 
fatClustToSect ( 
    unsigned short cluster, 
    unsigned long spc, 
    unsigned long first_data_sector );

void 
fatLoadCluster ( 
    unsigned long sector, 
    unsigned long address, 
     unsigned long spc );

 
//
// Loading files.
//

int load_kernel (const char *file_name);



//
// File support.
//

int 
fsLoadFile ( 
    unsigned char *name, 
    unsigned long file_address, 
    unsigned long dir_address );

unsigned long path_count (unsigned char *path);

//esse � o endere�o do arquivo, que � o �ltimo n�vel do path.
int load_path ( unsigned char *path, unsigned long address );


unsigned long fsSearchFile(unsigned char *name);

unsigned long 
fsSaveFile ( 
    unsigned char *file_name, 
    unsigned long file_size, 
    unsigned long file_address );
   
unsigned long fsSearchFileName( unsigned char *name);
void 
fsSaveFileName ( 
    unsigned char *name, 
    unsigned long id, 
    unsigned long eid, 
    unsigned long initial_cluster);

unsigned long fsCreateFile( char *name, unsigned long id);

//int fs_search_file(unsigned char *file_name);


//
// FAT support.
//

int fsCheckFat();
void fsClearFat();


void fs_load_fatEx();
void fs_test_fat_vector();
unsigned long fs_find_n_empty_entries(unsigned long n);
unsigned long fs_get_fat_entry(unsigned long n);
void fs_set_fat_entry(unsigned long n, unsigned long value);
void fs_put_list_on_fat();


//@todo: FAT ou ROOT DIR ?  
unsigned long fs_find_not_empty_entry();
unsigned short fs_find_empty_entry();


//
// Root Dir support.
//

void fs_load_rootdirEx();
void fs_load_rootdir();
void fsSaveRootDir();
void fs_save_entry_on_root(unsigned long eid);

//
// Dir support.
//

unsigned long fsCreateDir( char *name , unsigned long id);
void fsCreateFileName( char *name, unsigned long id,unsigned long eid);

void fs_save_entry_on_disc(unsigned long id, unsigned long eid);
void fs_set_entry(unsigned long id, unsigned long eid);
void fs_get_entry(unsigned long id, unsigned long eid);
void fs_show_dir_entry(unsigned long id,unsigned long eid);
void fs_show_dir(unsigned long id);
unsigned long fs_search_empty_entry(unsigned long id); 
void fs_relood_dir(unsigned long id);
void fs_load_dir(unsigned long id);
void fs_save_dir(unsigned long id);
unsigned long fs_check_cluster(unsigned long id); 
void fs_show_entry(unsigned long id, unsigned long eid);
unsigned long fs_get_entry_status(unsigned long id, unsigned long eid);
void fs_set_entry_status(unsigned long id, unsigned long eid, unsigned long status);

void 
fs_create_entry ( 
    char *name, 
    unsigned long id, 
    unsigned long eid, 
    unsigned long cluster, 
    unsigned long size );

void fs_delete_entry(unsigned long id, unsigned long eid);

//
// Boot support.
//

unsigned long fs_makeboot();
void fs_init_bootfile_struct();


//
// Install support.
//

void fs_install();


//
// Configuration support.
//

void fs_set_structures();
void fs_save_structure();
void fs_format();


#endif    


//
// End.
//



