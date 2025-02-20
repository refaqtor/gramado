/*
 * File: netdev/e1000/nicintel.c   
 * 
 * // Driver para o NIC intel. (8086:100E)
 * 
 * Descrição:
 *     Network interface controller
 *     Network card driver.
 *     Algum gerenciamento de adaptador de network.
 *     Esse é o header do driver da placa de rede.
 *
 * 8086/100e network controller encontrado na oracle virtual box.
 * 
 * History:
 *     2016 - Created by Fred Nora.
 *     2018 - Credits: Ítalo Lima Marconato Matias, Chicago OS.
 */


 /*
   wikipedia - NIC
   A network interface controller 
   (NIC, also known as a network interface card, network adapter, 
   LAN adapter or physical network interface,[1] and by similar terms) 
   is a computer hardware component that connects a computer to a computer network.[2]
  
Connects to Motherboard via one of:
    Integrated, PCI Connector, ISA Connector, PCI-E, FireWire, USB, Thunderbolt.

Network via one of:
    Ethernet, Wi-Fi, Fibre Channel, ATM, FDDI, Token ring.

Speeds:
    10 Mbit/s, 100 Mbit/s, 1 Gbit/s, 10 Gbit/s, up to 160 Gbit/s.

Common manufacturers:
    Intel, Realtek, Broadcom, Marvell Technology Group, QLogic, Mellanox.
 
techniques:
    Polling, Interrupt-driven I/O, Programmed input/output, Direct memory access.
 
 */
 
// See:
// https://wiki.osdev.org/Intel_8254x

 
#include <kernel.h>


// How many buffers.
#define SEND_BUFFER_MAX       8
#define RECEIVE_BUFFER_MAX   32



// irq handler.
void 
PCIRegisterIRQHandler ( 
    uint16_t bus, 
    uint8_t slot, 
    uint8_t func, 
    unsigned long handler,
    void *priv ) 
{
    panic ("E1000-PCIRegisterIRQHandler: [FIXME]\n");
}



/*
 *****************************************************
 * e1000_init_nic:
 *     Inicializando o controlador NIC da Intel.
 *
 * #importante:
 * Essa rotina é chamada quando sondamos para encontrar os dispositivos pci. 
 */

// Pegaremos mais informações sobre o dispositivo pci e colocaremos na 
// estrutura.
// 8086:100e
// 82540EM Gigabit Ethernet Controller

int 
e1000_init_nic ( 
    unsigned char bus, 
    unsigned char dev, 
    unsigned char fun, 
    struct pci_device_d *pci_device )
{

    // loop
    register uint32_t i=0; 
    
    // pci info.
    uint32_t data=0;

    unsigned short Vendor = 0;
    unsigned short Device = 0;

    unsigned long phy_address  = 0;
    unsigned long virt_address = 0;

    unsigned short tmp16=0;

    uint32_t Val=0;

    // #debug
    debug_print ("e1000_init_nic:\n");
    printf      ("e1000_init_nic:\n");


    // NIC Intel.
    // #importante
    // Devemos falhar antes de alocarmos memória para a estrutura.
    // #todo
    // Fazer uma lista de dispositivos Intel suportados por esse driver.
    // +usar if else.

    data = (uint32_t) diskReadPCIConfigAddr ( bus, dev, fun, 0 );

    Vendor = (unsigned short) (data       & 0xffff);
    Device = (unsigned short) (data >> 16 & 0xffff);

    if ( Vendor != 0x8086 || Device != 0x100E )
    {
        debug_print ("e1000_init_nic: [FAIL] Device not found\n");

        // #todo
        // #bugbug
        // Temos que sinalizar que o dispositivo nao foi inicializado.
        
        return (int) (-1);
    }

    // #debug
    printf ("Vendor=%x ",   (data       & 0xffff) );
    printf ("Device=%x \n", (data >> 16 & 0xffff) );


	//
	// pci_device structure.
	//


	// pci device struct
	// passado via argumento. 

    if ( (void *) pci_device ==  NULL ){
        panic ("e1000_init_nic: pci_device\n");
    }else{
        pci_device->used   = TRUE;
        pci_device->magic  = 1234;
        pci_device->bus    = (unsigned char) bus;
        pci_device->dev    = (unsigned char) dev;
        pci_device->func   = (unsigned char) fun;
        pci_device->Vendor = (unsigned short) (data       & 0xffff);
        pci_device->Device = (unsigned short) (data >> 16 & 0xffff);


		// #IMPORTANTE
		// #bugbug:
		// Esse driver é para placa Intel, vamos cancelar a inicialização 
		// do driver se a placa não for Intel.

		// 8086:100e
		// 82540EM Gigabit Ethernet Controller

		// #todo
		// Fazer uma lista de dispositivos Intel suportados por esse driver.
		// +usar if else.
		// já fizemos essa checagem antes.

        if ( pci_device->Vendor != 0x8086 || 
             pci_device->Device != 0x100E )
        {
            panic ("e1000_init_nic: 82540EM not found\n");
            // #bugbug
            // Maybe only return.
            // debug_print ("e1000_init_nic: 82540EM not found\n");
            // return -1;
        }


		//
		// BARs
		//

        pci_device->BAR0 = (unsigned long) diskReadPCIConfigAddr ( 
                                               bus, dev, fun, 0x10 );
        pci_device->BAR1 = (unsigned long) diskReadPCIConfigAddr ( 
                                               bus, dev, fun, 0x14 ); 
        pci_device->BAR2 = (unsigned long) diskReadPCIConfigAddr ( 
                                               bus, dev, fun, 0x18 );
        pci_device->BAR3 = (unsigned long) diskReadPCIConfigAddr ( 
                                               bus, dev, fun, 0x1C );
        pci_device->BAR4 = (unsigned long) diskReadPCIConfigAddr ( 
                                               bus, dev, fun, 0x20 );
        pci_device->BAR5 = (unsigned long) diskReadPCIConfigAddr ( 
                                               bus, dev, fun, 0x24 );

        // IRQ.

        // irq
        pci_device->irq_line = (uint8_t) pciConfigReadByte ( 
                                             bus, dev, fun, 0x3C );
        // letras
        pci_device->irq_pin  = (uint8_t) pciConfigReadByte ( 
                                             bus, dev, fun, 0x3D ); 


        // The physical address!
        // #importante:
        // Grab the Base I/O Address of the device
        // Aqui nós pegamos o endereço dos registadores na BAR0,
        // Então mapeamos esse endereço físico para termos um 
        // endereço virtual para manipularmos os registradores. 

        phy_address = (unsigned long) ( pci_device->BAR0 & 0xFFFFFFF0 );

        if (phy_address == 0){
            panic ("e1000_init_nic: Invalid phy_address\n");
        }
        // ...
    };


    // Base address
    // #importante:
    // Mapeando para obter o endereço virtual que 
    // o kernel pode manipular.
    // pages.c
    // #bugbug: 
    // >> Isso é um improviso. Ainda falta criar rotinas melhores.

    virt_address = (unsigned long) mapping_nic1_device_address (phy_address);

    if (virt_address == 0){
        panic ("e1000_init_nic: Invalid virt_address\n");
    }

    // Endereço base.
    // Preparando a mesma base de duas maneiras.

    unsigned char *base_address   = (unsigned char *) virt_address;
    unsigned long *base_address32 = (unsigned long *) virt_address;


    //
    // == NIC =========================
    //

	// #todo: 
	// Checar essa estrutura.

    currentNIC = (void *) kmalloc ( sizeof( struct intel_nic_info_d ) );

    if ( (void *) currentNIC ==  NULL ){
        panic ("e1000_init_nic: currentNIC struct\n");
    } else {
        currentNIC->used  = TRUE;
        currentNIC->magic = 1234;

        currentNIC->interrupt_count = 0;

        currentNIC->pci = (struct pci_device_d *) pci_device;

        // Salvando o endereço para outras rotinas usarem.
        currentNIC->registers_base_address = (unsigned long) &base_address[0];
        currentNIC->mem_base = (uint32_t) &base_address[0];

        currentNIC->use_io = 0; //False;

		//
		// Get info.
		//

		// Device status.
        currentNIC->DeviceStatus = base_address[0x8];


		//
		// ## EEPROM ##
		//
		
		//False;
		//Como ainda não sabemos, vamos dizer que não.
		currentNIC->eeprom = 0; 

		// Let's try to discover reading the status field!
        for ( i=0; i < 1000 && !currentNIC->eeprom; ++i ) 
        {
            Val = E1000ReadCommand ( currentNIC, 0x14 );

            // We have? Yes!.
            if ( (Val & 0x10) == 0x10) { currentNIC->eeprom = 1; }
        };


		//
		// ## MAC ##
		//


		// Let's read the MAC Address!


	    // We can use the EEPROM!
        if (currentNIC->eeprom == 1) 
        {
			//printf("MAC from eeprom \n");  
			//refresh_screen();
			//while(1){}
			 
		    uint32_t tmp = E1000ReadEEPROM ( currentNIC, 0 );
		    currentNIC->mac_address[0] = (uint8_t)(tmp & 0xFF);
		    currentNIC->mac_address[1] = (uint8_t)(tmp >> 8);

		    tmp = E1000ReadEEPROM ( currentNIC, 1);
		    currentNIC->mac_address[2] = (uint8_t)(tmp & 0xFF);
		    currentNIC->mac_address[3] = (uint8_t)(tmp >> 8);
		
		    tmp = E1000ReadEEPROM ( currentNIC, 2);
		    currentNIC->mac_address[4] = (uint8_t)(tmp & 0xFF);
		    currentNIC->mac_address[5] = (uint8_t)(tmp >> 8);


        // We can't use the EEPROM :(
        }else{

			//printf("MAC from registers \n"); 
			//refresh_screen();
			//while(1){}

            // MAC - pegando o mac nos registradores.
            currentNIC->mac_address[0] = base_address[ 0x5400 + 0 ];
            currentNIC->mac_address[1] = base_address[ 0x5400 + 1 ];
            currentNIC->mac_address[2] = base_address[ 0x5400 + 2 ];
            currentNIC->mac_address[3] = base_address[ 0x5400 + 3 ];
            currentNIC->mac_address[4] = base_address[ 0x5400 + 4 ];
            currentNIC->mac_address[5] = base_address[ 0x5400 + 5 ];
        };
    };



	//
	// ## bus mastering ##
	//

	// Let's enable bus mastering!
	// #define PCI_COMMAND 0x04
	// We really need to do it?
	// Yes, set the bus mastering bit
	// And write back 
	//( bus, slot, func, PCI_COMMAND )
    
    uint16_t cmd=0;
    cmd = (uint16_t) pciConfigReadWord ( 
                         (unsigned char) bus, 
                         (unsigned char) dev, 
                         (unsigned char) fun, 
                         (unsigned char) 0x04 );

    // IN: (bus, slot, func, PCI_COMMAND, cmd);
    if ( (cmd & 0x04) != 0x04 )
    {
        cmd |= 0x04;
        diskWritePCIConfigAddr ( 
            (int) bus, (int) dev, (int) fun, 
            (int) 0x04, (int) cmd ); 
    }

    printf ("Done\n");

    return 0;
}


/*
 *******************************************
 *    >>>> HANDLER <<<<
 *******************************************
 * irq_E1000:
 *     
 *     Esse é o handler da interrupção para o NIC intel 8086:100E.
 *     Esse é o driver do controlador, ele não atua sobre protocolos 
 * de rede, então deve-se enviar uma mensagem para o servidor de rede 
 * para ele analizar o conteúdo do buffer, para assim decidir qual 
 * é o protocolo e redirecionar para a rotina de tratamento do 
 * protocolo específico.
 *     Esse é o driver do controlador, ele deve solicitar ao kernel
 * qual é o PID do processo que é o servidor de rede, e enviar
 * a mensagem para ele, contendo o endereço do buffer.
 */

// Isso é chamado pelo assembly.

__VOID_IRQ 
irq_E1000 (void)
{

    uint32_t status=0;
    uint32_t val=0;
    uint16_t old=0;
    uint32_t len=0;

    unsigned char *buffer;
    
    
    //debug_print ("xxxe1000handler: ");
    
    //
    // Profiler
    //
    
    // Contando as interrupções desse tipo.
    g_profiler_ints_irq9++;


    
	// #importante:
	// #flag 
	// Essa flag precisa ser acionada para a rotina funcionar.
	// F6 tem acionado essa flag.

    if ( e1000_interrupt_flag != TRUE )
    {
        printf ("irq_E1000: locked\n");
        refresh_screen();
        return;
    }else{

		//#debug
        //printf ("xxxe1000handler: unlocked\n");
        //refresh_screen ();
    };



	// See: intel.h
    e1000_irq_count++;


    // Interrupt count.
    if ( (void *) currentNIC != NULL ){ 
        currentNIC->interrupt_count++; 
    }

    // Without this, the card may spam interrupts...
    E1000WriteCommand( currentNIC, 0xD0, 1 );


    //
    // Status.
    //

    // Status
    status = E1000ReadCommand( currentNIC, 0xC0 ); 

    // 0x04 - Linkup
    // Start link.
    if (status & 0x04){
        printf ("Start link\n");
        refresh_screen();
        val = E1000ReadCommand ( currentNIC, 0 );
        E1000WriteCommand ( currentNIC, 0, val | 0x40 );
        return;

    // 0x10 - Good threshold (limite)
    // Isso apresentou problemas. Pensaremos nisso no futuro.
    //} else if (status & 0x10){
    //    printf ("Good threshold\n");
    //    refresh_screen();
    //    return;


    // 0x80 - Reveive.
    } else if (status & 0x80){
        //printf("xxxe1000handler: handler for NIC e1000");
        //printf("e1000 handler ");
        //refresh_screen();

        // #todo
        // Esse sequência está funcionando. Não mudar.
        // Precisamos entender ela melhor.
        // Todos os buffers de recebimento.
        // Olhamos um bit do status de todos os buffers.
   
        while ( (currentNIC->legacy_rx_descs[currentNIC->rx_cur].status & 0x01) == 0x01 ) 
        {
             old = currentNIC->rx_cur;
             len = currentNIC->legacy_rx_descs[old].length;

             //#test: Apenas pegando o buffer para usarmos lodo adinate.
             buffer = (unsigned char *) currentNIC->rx_descs_virt[old];

             //se a inicialização está completa.
             //if(____network_late_flag == 1){
             //    network_buffer_in ( (void *) buffer, (int) len );
             //}  
                  
            //#bugbug: Não mais chamaremos a rotina de tratamento nesse momento.
            //chamaremos logo adiante, usando o buffer que pegamos acima.

            // Our Net layer should handle it
            // NetHandlePacket(dev->ndev, len, (PUInt8)dev->rx_descs_virt[old]);

            // zera.
            currentNIC->legacy_rx_descs[old].status = 0;
            
            // circula. (32 buffers)
            currentNIC->rx_cur = (currentNIC->rx_cur + 1) % RECEIVE_BUFFER_MAX; 

            // ?? Provavelmente seleciona o buffer.
            E1000WriteCommand ( currentNIC, 0x2818, old );
        };

        //
        // == ## Reagindo ## ===========================
        //
        
        // Vamos copiar o pacote para alguma fila de buffers.
        // A rotina de decodificar o pacote pegará o
        // pacote em alguma fila de buffers.
        
        // len: 
        //     Tamanho do pacote. Temos que considerar limites.
        // buffer:
        //     Endereço do pacote.
        
        //network_buffer_in ( (void *) buffer, (int) len );

        // #importante
        // Qual buffer?
        // Atenção:
        // Nesse momento checaremos se no início do buffer temos o header ethernet.
        // Em seguida o 'switch' chama as rotinas apropriadas para cada tipo 
        // de pacode.
        // Os tipos são: IPV4, ARP, IPV6 e default.

        //unsigned char *buffer = (unsigned char *) currentNIC->rx_descs_virt[old];
        //buffer = (unsigned char *) currentNIC->rx_descs_virt[old];

        // #importante
        // Agora que temos o buffer podemos enviar para o servidor de rede.
        // Pois o driver do controlador não lida com protocolos.
        // See: network.c
        // Estamos chamando um serviço no diálogo que decodifica o buffer.
        // Mas poderíamos simplesmente chamar um serviço no diálogo
        // que mandasse o buffer para o network server em ring3.

        // #bugbug
        // #todo
        // Antes de chamarmos essa rotina de diálogo
        // é bom checarmos a flag da inicialização do sistema.
        // Pois senão seremos interrompidos muitas vezes durante a 
        // inicialização ...
        // bom seria que o processo init ou o network server acionasse
        // a flag que libera esse diálogo.

        //#todo
        if(____network_late_flag == TRUE)
        {
            // #importante
            // Coloca o pacote em um dos buffers de entrada.
            // Esses buffers são compartilhados e os aplicativos poderão ler.
            // Enfileirar o buffer

            network_buffer_in ( (void *) buffer, (int) len );
            
            // Decodificar o buffer.
            // 8000 - decode buffer.
            //network_driver_dialog ( NULL, (int) 8000, 
                //(unsigned long) &buffer[0], (unsigned long) &buffer[0] );
        }

        return;
    };
    
    // Outro status qualquer.
}




//=======================================================
// e1000_setup_irq:
//     Setup nic irq 
//
// #importante
// Isso é usado por uma hotina em headlib.s para 
// configurar uma nova entrada na idt. 
//

// Essa função é chamada pelo driver de PCI quando encontrar
// o dispositivo Intel apropriado.

// #bugbug
// Called by pciHandleDevice.

//o assembly tem que pegar aqui.
uint8_t nic_idt_entry_new_number;
uint32_t nic_idt_entry_new_address;


void e1000_setup_irq (int irq_line){

    debug_print ("e1000_setup_irq: [FIXME]\n");

    // #test


	// pegando o número da irq.

    //uint8_t irq = (uint8_t) currentNIC->pci->irq_line;
    uint8_t irq = (uint8_t) irq_line;
	
	// handler address
	
    uint32_t handler = (uint32_t) &irq_E1000; 

	// #importante
	// Transformando irq em número de interrupção.
	// 9+32=41.

    uint8_t idt_num = (irq + 32);

	// Chamando asm:
	// número e endereço.
	// #obs: Essas variáveis são declaradas nesse arquivo
	// o assembly terá que pegar.

    nic_idt_entry_new_number  = (uint8_t) idt_num; 
    nic_idt_entry_new_address = (unsigned long) handler; 


	//#debug OK (irq=9) 
    printf ("e1000_setup_irq: irq={%d}\n",irq);
    printf ("e1000_setup_irq: handler={%x}\n", handler);
	//printf ("PCIRegisterIRQHandler: pin={%d}\n",currentNIC->pci->irq_pin);//shared INTA#
	//refresh_screen();
	//while(1){}


	//#debug interrupção=41 
    printf ("e1000_setup_irq: interrupt={%d}\n", 
        nic_idt_entry_new_number );
    printf ("e1000_setup_irq: handler={%x}\n", 
        nic_idt_entry_new_address );
     
	//refresh_screen();
	//while(1){}

	//
	// Creating IDT entry.
	//
	
	//Essa é a rotina em assembly que cria uma entrada na idt para 
	//o nic, com base nas variáveis que são importadas pelo assembly.
	
	//headlib.asm
	//deveria ir para hwlib.asm

    extern void asm_nic_create_new_idt_entry (void);

    asm_nic_create_new_idt_entry();
}



//====================================================
// ## reset ##
//

// Essa função é chamada pelo driver de PCI quando encontrar
// o dispositivo Intel apropriado.

int e1000_reset_controller (void){

    int i=0;
    //register int i=0;


    debug_print ("e1000_reset_controller\n");

	//unsigned long tmp;
	
	//#debug
	//printf("e1000_reset_controller: Reseting controller ... \n");
	
	/*
	if ( (void *) currentNIC ==  NULL )
	{
		printf("e1000_reset_controller: currentNIC struct\n");
	    return (int) 1;	
		
	}	
    */
	
	//#todo: precisamos checar a validade dessa estrutura e do endereço.
	
    //esse será o endereço oficial.
    //currentNIC->mem_base	

    if ( currentNIC->mem_base == 0 )
    {
        panic ("e1000_reset_controller: [FAIL] currentNIC->mem_base\n");
    }


	//endereço base.
	//unsigned char *base_address = (unsigned char *) currentNIC->registers_base_address;
	//unsigned long *base_address32 = (unsigned long *) currentNIC->registers_base_address;	

	//unsigned char *base_address = (unsigned char *) currentNIC->mem_base;
	//unsigned long *base_address32 = (unsigned long *) currentNIC->mem_base;	
		
	//
	//===========================================
	//
	
	//
	//    ## TX ##
	//

    // And alloc the phys/virt address of the transmit buffer
    // tx_descs_phys conterá o endereço físico e
    // legacy_tx_descs conterá o endereço virtual.

    currentNIC->tx_descs_phys = E1000AllocCont ( 0x1000, (uint32_t *)(&currentNIC->legacy_tx_descs) );

	// We failed, unmap everything

    if ( currentNIC->tx_descs_phys == 0 )
    {
        panic ("e1000_reset_controller: [FAIL] currentNIC->tx_descs_phys\n");
    }


	//tx
	//i já foi declarado

    for ( i=0; i < 8; i++ ) 
    {
        // Alloc the phys/virt address of this transmit desc
        // alocamos memória para o buffer, salvamos o endereço físico do buffer e 
        // obtemos o endereço virtual do buffer.		
        currentNIC->legacy_tx_descs[i].addr  = E1000AllocCont ( 0x3000, &currentNIC->tx_descs_virt[i] );
        currentNIC->legacy_tx_descs[i].addr2 = 0;

		// We failed, unmap everything

        if (currentNIC->legacy_tx_descs[i].addr == 0)
        {
            panic ("e1000_reset_controller: [FAIL] dev->rx_descs[i].addr\n");
        }

        //cmd: bits
        //IDE VLE DEXT RSV RS IC IFCS EOP
        //IDE (bit 7) - Interrupt Delay Enable
        //VLE (bit 6) - VLAN Packet Enable
        //DEXT (bit 5) - Descriptor extension (#importante: '0' for legacy mode)
        //RSV (bit 4) - Reserved
        //RS (bit 3) - Report status
        //IC (bit 2) - Insert checksum
        //IFCS (bit 1) - Insert FCS (CRC)
        //EOP (bit 0) - End of packet

        currentNIC->legacy_tx_descs[i].cmd = 0;
        currentNIC->legacy_tx_descs[i].status = 1;
    };

	//#debug 
	//Vamos imprimir os endereços usados pelos buffers para teste.	
    //for ( i=0; i < 8; i++ )
    //    printf ("PA={%x} VA={%x} \n",currentNIC->legacy_tx_descs[i].addr, currentNIC->tx_descs_virt[i]);


	//
	//=============================================
	//

	//
	//    ## RX ##
	//
	
	
    // And alloc the phys/virt address of the transmit buffer

    currentNIC->rx_descs_phys = E1000AllocCont (0x1000, (uint32_t *)(&currentNIC->legacy_rx_descs));

    // We failed, unmap everything
    if (currentNIC->rx_descs_phys == 0)
    {
        panic ("e1000_reset_controller: [FAIL] currentNIC->rx_descs_phys\n");
    }


	//rx
	//i já foi declarado

    for ( i=0; i < 32; i++ ) 
    {
        // Alloc the phys/virt address of this transmit desc
        currentNIC->legacy_rx_descs[i].addr  = E1000AllocCont ( 0x3000, (uint32_t *) &currentNIC->rx_descs_virt[i] );
        currentNIC->legacy_rx_descs[i].addr2 = 0;

        // Buffer null.
        // We failed, unmap everything
        if (currentNIC->legacy_rx_descs[i].addr == 0)
        {
            panic ("e1000_reset_controller: [FAIL] dev->rx_descs[i].addr\n");
        }

        currentNIC->legacy_rx_descs[i].status = 0;
    };


	//#debug 
	//Vamos imprimir os endereços edereços físicos dos buffers 
	//e os edereços virtuais dos descritores.
	//for ( i=0; i < 32; i++ )
    //    printf ("PA={%x} VA={%x} \n",currentNIC->legacy_rx_descs[i].addr, currentNIC->rx_descs_virt[i]);
	

	// ???

    for (i=0; i < 0x80; i++){
        E1000WriteCommand ( currentNIC, 0x5200 + (i * 4), 0 );
    };

    currentNIC->rx_cur = 0;
    currentNIC->tx_cur = 0;

	//irq #todo
	//PCIRegisterIRQHandler ( bus, dev, fun, (unsigned long) E1000Handler, currentNIC );
	
	
    /* Transmit Enable. */
    //#define E1000_REG_TCTL_EN	(1 << 1)

    /* Pad Short Packets. */
    //#define E1000_REG_TCTL_PSP	(1 << 3)


   //#define E1000_ICR      0x000C0  /* Interrupt Cause Read - R/clr */
   //#define E1000_ITR      0x000C4  /* Interrupt Throttling Rate - RW */
   //#define E1000_ICS      0x000C8  /* Interrupt Cause Set - WO */
   //#define E1000_IMS      0x000D0  /* Interrupt Mask Set - RW */
   //#define E1000_IMC      0x000D8  /* Interrupt Mask Clear - WO */
   //#define E1000_IAM      0x000E0  /* Interrupt Acknowledge Auto Mask */	

   // #define E1000_IAC      0x04100  /* Interrupt Assertion Count */
   //#define E1000_ICRXPTC  0x04104  /* Interrupt Cause Rx Packet Timer Expire Count */
   //#define E1000_ICRXATC  0x04108  /* Interrupt Cause Rx Absolute Timer Expire Count */
   //#define E1000_ICTXPTC  0x0410C  /* Interrupt Cause Tx Packet Timer Expire Count */
   //#define E1000_ICTXATC  0x04110  /* Interrupt Cause Tx Absolute Timer Expire Count */
   //#define E1000_ICTXQEC  0x04118  /* Interrupt Cause Tx Queue Empty Count */
   //#define E1000_ICTXQMTC 0x0411C  /* Interrupt Cause Tx Queue Minimum Threshold Count */
   //#define E1000_ICRXDMTC 0x04120  /* Interrupt Cause Rx Descriptor Minimum Threshold Count */
   //#define E1000_ICRXOC   0x04124  /* Interrupt Cause Receiver Overrun Count */  


   // (*((uint32_t *) (start + E1000_IMS))) |= E1000_IMS_RXT0;
   //(*((uint32_t *) (start + E1000_IMS))) |= E1000_IMS_RXO;
   // (*((uint32_t *) (start + E1000_IMS))) |= E1000_IMS_RXDMT0;
   // (*((uint32_t *) (start + E1000_IMS))) |= E1000_IMS_RXSEQ;
   // (*((uint32_t *) (start + E1000_IMS))) |= E1000_IMS_LSC;	

	//E1000WriteCommand(currentNIC, 0xD0, E1000_IMS_RXT0 | E1000_IMS_RXO );

	// Enable interrupts
	//0xD0 Message Control (0x0080) Next Pointer (0xE0) Capability ID (0x05)
	//    0000 0001  1111 0111  0000   0010  1101   0111
	//0x1F6DC, 1f72d7
    E1000WriteCommand (currentNIC, 0xD0, 0x1F6DC);


	//?
	//E1000WriteCommand(currentNIC, 0xD0, 0xFB);
    
    E1000ReadCommand (currentNIC, 0xC0);

	
	// ## RX ##
	
	// receive
	// Setup the (receive) ring registers.
	// Pass the physical address (and some other informations) of the receive buffer
	
    E1000WriteCommand (currentNIC, 0x2800, currentNIC->rx_descs_phys );  // low
    E1000WriteCommand (currentNIC, 0x2804, 0);                           // high 
    E1000WriteCommand (currentNIC, 0x2808, 512);    // 32*16
    E1000WriteCommand (currentNIC, 0x2810, 0);      // head
    E1000WriteCommand (currentNIC, 0x2818, 31);     // tail
    E1000WriteCommand (currentNIC, 0x100, 0x602801E);  // RCTL	= 0x0100,	/* Receive Control */
	
	
	// ## TX ##
	
	//transmit
	//Setup the (transmit) ring registers.
	// Pass the physical address (and some other informations) of the transmit buffer
	
	//TDBAL	= 0x3800,	/* Tx Descriptor Base Address Low */
	//TDBAH	= 0x3804,	/* Tx Descriptor Base Address High */
	
    E1000WriteCommand (currentNIC, 0x3800, currentNIC->tx_descs_phys );  //low (endereço do ring)
    E1000WriteCommand (currentNIC, 0x3804, 0);                           //high
    E1000WriteCommand (currentNIC, 0x3808, 128);    //8*16
    E1000WriteCommand (currentNIC, 0x3810, 0);      //head
    E1000WriteCommand (currentNIC, 0x3818, 7); //0);      //tail

	//#define E1000_TCTL     0x00400  /* TX Control - RW */
    //• CT = 0x0F (16d collision)
    //• COLD: HDX = 511 (0x1FF); FDX = 63 (0x03F)
    //• PSP = 1b
    //• EN=1b
    //• All other fields 0b	
     /* Transmit Control */
    //#define E1000_TCTL_RST    0x00000001    /* software reset */
    //#define E1000_TCTL_EN     0x00000002    /* enable tx */
    //#define E1000_TCTL_BCE    0x00000004    /* busy check enable */
    //#define E1000_TCTL_PSP    0x00000008    /* pad short packets */
    //#define E1000_TCTL_CT     0x00000ff0    /* collision threshold */
    //#define E1000_TCTL_COLD   0x003ff000    /* collision distance */
    //#define E1000_TCTL_SWXOFF 0x00400000    /* SW Xoff transmission */
    //#define E1000_TCTL_PBE    0x00800000    /* Packet Burst Enable */
    //#define E1000_TCTL_RTLC   0x01000000    /* Re-transmit on late collision */
    //#define E1000_TCTL_NRTU   0x02000000    /* No Re-transmit on underrun */
    //#define E1000_TCTL_MULR   0x10000000    /* Multiple request support */

	//habilita esses dois campos e o resto é zero.
    //• GRAN = 1b (descriptors)
    //• WTHRESH = 1b
    //• All other fields 0b.	
	//#define E1000_TXDCTL_WTHRESH 0x003F0000 /* TXDCTL Writeback Threshold */
	//#define E1000_TXDCTL_GRAN    0x01000000 /* TXDCTL Granularity */
	//#define E1000_TXDCTL   0x03828  /* TX Descriptor Control - RW */
	
    E1000WriteCommand (currentNIC, 0x3828, (0x01000000 | 0x003F0000)); 

    E1000WriteCommand ( currentNIC,  0x400, ( 0x00000ff0 | 0x003ff000 | 0x00000008 | 0x00000002) ); 
	
	//?
	//E1000WriteCommand(currentNIC, 0x400, 0x10400FA);  //TCTL	= 0x0400,	/* Transmit Control */
	//E1000WriteCommand(currentNIC, 0x400, 0x3003F0FA);
	//E1000WriteCommand(currentNIC, 0x400, (1 << 1) | (1 << 3) );
 
    //• IPGT = 8
    //• IPGR1 = 2
    //• IPGR2 = 10
    //#define E1000_TIPG     0x00410  /* TX Inter-packet gap -RW */	
    E1000WriteCommand ( currentNIC,  0x410, (  0x0000000A | 0x00000008 | 0x00000002) ); 	


	//talvez ja fizemos isso. 
	//Initialize the transmit descriptor registers (TDBAL, TDBAH, TDL, TDH, and TDT).


    //eth_write(base_addr, REG_ADDR_MAC_CONF,
	//	  /* Set the RMII speed to 100Mbps */
	//	  MAC_CONF_14_RMII_100M |
	//	  /* Enable full-duplex mode */
	//	  MAC_CONF_11_DUPLEX |
	//	  /* Enable transmitter */
	//	  MAC_CONF_3_TX_EN |
	//	  /* Enable receiver */
	//	  MAC_CONF_2_RX_EN);	
    
	
	//iow32(dev, TCTL, TCTL_EN);
	
	//printf("nic_i8254x_reset: Done\n");
	//refresh_screen();	
	
	//endereço físico  dos rings;
	//printf("tx_ring_pa=%x rx_ring_pa=%x \n", 
	//    currentNIC->rx_descs_phys, 
	//	currentNIC->tx_descs_phys );


	// Linkup

    uint32_t val = E1000ReadCommand (currentNIC, 0);
	
	//(1 << 6)
    E1000WriteCommand (currentNIC, 0, val | 0x40);

    return 0;
}


/*
 * E1000WriteCommand:
 * 
 */
 
void 
E1000WriteCommand ( 
    struct intel_nic_info_d *d, 
    uint16_t addr, 
    uint32_t val )
{

    // #todo
    // Check the pointer validation
    
    //if( (void*) d == NULL )
        //return 0;

	// Use the IO ports?
	//if (dev->use_io) 
	//{
	//  Yes
	//	PortOutLong(dev->io_base, addr);	
	//	PortOutLong(dev->io_base + 4, val);
	//} else {
	//  No
	//	*( (volatile unsigned int *)(d->mem_base + addr)) = val;	
	//}


    *( (volatile unsigned int *)(d->mem_base + addr)) = val;
}



uint32_t 
E1000ReadCommand ( 
    struct intel_nic_info_d *d, 
    uint16_t addr )
{

    // #todo
    // Check the pointer validation
    
    //if( (void*) d == NULL )
        //return 0;


	// Use the IO ports?
	//if (dev->use_io) 
	//{	
	//  Yes
	//	PortOutLong(dev->io_base, addr);	
	//	return PortInLong(dev->io_base + 4);
	//} else {
	//  No
	//	return *( (volatile unsigned int *) (d->mem_base + addr));
	//}


    // return (uint32_t) ...
    return *( (volatile unsigned int *) (d->mem_base + addr));
}



/*
 * E1000AllocCont: ??
 * retorna o endereço físico e coloca o virtual em *virt
 * ah ... então eu vou alocar usando endereços virtuais
 * ... e traduzir para físico 
 * ... colocar o virtual em *virt e retornar o físico.
 */

// Precisamos de um endereço fisico.
// + alocamos um endereço virtual
// + convertemos para fisico

// IN: size, return pointer.

uint32_t 
E1000AllocCont ( 
    uint32_t amount, 
    uint32_t *virt )
{
    uint32_t va=0;
    uint32_t pa=0;


    if (amount==0){
        panic ("E1000AllocCont: [FAIL] amount\n");
    }

    // ============
    // va
    va = (uint32_t) kmalloc ( (size_t) amount );
    *virt = va;
    if (*virt == 0){
        panic ("E1000AllocCont: [FAIL] va allocation\n");
    }

    
    // ============
    // pa
    // ps: Using the kernel page directory.
    pa = (uint32_t) virtual_to_physical (
                        va, gKernelPageDirectoryAddress ); 
    if (pa == 0){
        panic ("E1000AllocCont: [FAIL] pa\n");
    }

    return (uint32_t) pa;
}


/*
void NetSendEthPacket ( 
    PNetworkDevice dev, 
    uint8_t dest[6], 
    uint16_t type, 
    uint32_t len, 
    uint8_t *buf )
{

	//if ((dev == Null) || (dev->send == Null) || (dest == Null)) 
	//{		
	//	return;
	//}
	
	// Let's build our ethernet frame!
	//PEthFrame frame = (PEthFrame) MemAllocate ( sizeof(EthFrame) + len);	
	PEthFrame frame = (PEthFrame) kmalloc ( sizeof(EthFrame) + len);	
	
	// Failed :(
	//if (frame == Null) {
	//	return;			
	//}
	
	//StrCopyMemory(frame->dst, dest, 6);			// Copy the dest mac address
	//StrCopyMemory(frame->src, dev->mac_address, 6);	// The src mac address (our mac address)
	
	//StrCopyMemory((uint8_t *)(((uint32_t)frame) + sizeof(EthFrame)), buf, len);	// Copy the data/payload
	
	frame->type = ToNetByteOrder16(type);			// Set the type
	
	//void (*send)( void *, uint32_t, uint8_t );
	dev->send ( dev->priv, sizeof(EthFrame) + len, (uint8_t *) frame );		// SEND!
	
	//MemFree((uint32_t)frame);					// Free our eth frame
}
*/

 
// Dispositivo, tamanho, dados a serem copiados no buffer.
void E1000Send ( void *ndev, uint32_t len, uint8_t *data )
{

    uint32_t i=0;
    //register uint32_t i=0;

    uint16_t old=0;


    struct intel_nic_info_d *dev = (struct intel_nic_info_d *) ndev;

    if ( (void *) dev == NULL ){
        debug_print("E1000Send: [FAIL] dev\n");
        return;
    }


	// Quem ?
	// Qual buffer ?
	// ## Copiando o pacote no buffer ##
	// com base no comprimento indicado no argmento.
	// Pegando o endereço virtual do buffer na estrutura do dispositivo.	

    old = dev->tx_cur;
    
    // src and dst.
    unsigned char *src_data = (unsigned char *) data; 
    unsigned char *buffer   = (unsigned char *) currentNIC->tx_descs_virt[old];

    //
    // Copy
    //

    if (len==0){
        debug_print("E1000Send: [FAIL] len\n");
        return;
    }

    for (i=0; i<len; i++){
        buffer[i] = src_data[i];
    };



	//void *memcpy(void *v_dst, const void *v_src, unsigned long c);
	//StrCopyMemory((PUInt8)(dev->tx_descs_virt[old]), data, len);

	//memcpy( (void *)(dev->tx_descs_virt[old]), (const void *) data, (unsigned long) len);


    dev->legacy_tx_descs[old].length = len;

    dev->legacy_tx_descs[old].cmd    = 0x1B;
    dev->legacy_tx_descs[old].status = 0;

    //SEND_BUFFER_MAX = 8
    dev->tx_cur = ( dev->tx_cur + 1 ) % 8; 

    *( (volatile unsigned int *)(dev->mem_base + 0x3818)) = dev->tx_cur;


	//#debug
    printf ("Sending broadcast arp, (while)\n");
    refresh_screen ();


    while ( !(dev->legacy_tx_descs[old].status & 0xFF) )
    {
        // Nothing
    };

    printf ("E1000Send: done\n");
    refresh_screen ();
}


/*
 ****************************
 * send_ipv4_packet:
 * 
 */

//enviar um pacote ipv4
//configuramos o buffer
//e arotina de sent faz o trabalho. 
//mas pecisamos passar o tamanho do buffer como argumento.
//o tamanho dobuffer é a soma do ethernet header e a strutura de ipv4.
//14+20 bytes

void 
send_ipv4_packet ( 
    struct intel_nic_info_d *dev, 
    uint32_t len, 
    uint8_t *data )
{

    if ( (void *) dev == NULL ){
        debug_print("send_ipv4_packet: [FAIL] dev\n");
        return;
    }

    if (len==0){
        debug_print("send_ipv4_packet: [FAIL] len\n");
        return;
    }

    // #todo
    // data? pointer validation

    E1000Send ( (void *) dev, (uint32_t) len , (uint8_t *) &data[0] );
}


// #todo: Deletar isso.
void nic_i8254x_transmit (void)
{
    // Cancelada.
    debug_print("nic_i8254x_transmit: [Nothing]\n");
}


uint32_t 
E1000ReadEEPROM ( 
    struct intel_nic_info_d *d, 
    uint8_t addr )
{
    uint32_t data = 0;


    // #todo
    // Check the pointer validation.

    //if ( (void*) d == NULL )
        //return 0;

	// We have the EEPROM?
 
	//#debug
	//printf("E1000ReadEEPROM:\n");

    // Yes :)	
    if (d->eeprom == 1) {
        E1000WriteCommand ( d, 0x14, 1 | (addr << 8) );

        //#bugbug
		//#obs: loop		
        while (( (data = E1000ReadCommand ( d, 0x14)) & 0x10 ) != 0x10 );

	// Nope ...
    } else {
        E1000WriteCommand ( d, 0x14, 1 | (addr << 2) );

		//#bugbug
		//#obs: loop
        while (( (data = E1000ReadCommand(d, 0x14)) & 0x01 ) != 0x01 );
    };

    return (data >> 16) & 0xFFFF; 
}


int 
e1000_ioctl ( 
    int fd, 
    unsigned long request, 
    unsigned long arg )
{
    debug_print ("e1000_ioctl: [TODO] \n");
    return -1;
}



//
// End.
//


