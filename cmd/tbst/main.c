/*
 * File: main.c 
 *
 * Descri��o: 
 *     Arquivo principal de IDLE.BIN.
 *     IDLE.BIN � um processo com apenas uma thread.
 *     IDLE.BIN � um aplicativo de 32bit em user mode. 
 *
 *     Esse programa dever� ser chamado sempre que o sistema estiver ocioso,
 * ou com falta de op��es vi�veis. Ent�o esse programa deve ficar respons�vel 
 * por alguma rotina de manuten��o do equil�brio de sitema, ou por ger�ncia de 
 * energia, com o objetivo de poupar energia nesse momento de ociosidade.
 *
 * O processo idle pode solicitar que processos de gerencia de energia entrem em
 * atua��o. Pois a chamada do processo idle em si j� � um indicativo de ociosidade
 * do sistema. Pode-se tamb�m organizar bancos de dados, registros, mem�ria, buffer,
 * cache etc.
 *
 *     O sistema pode configurar o que esse processo faz quando a m�quina 
 * est� em idle mode. Quando n�o h� nenhum processo pra rodar ou a cpu se 
 * encontra ociosa, pode-se usar alguma rotina otimizada presente neste 
 * programa. Parece que a intel oferece sujest�es pra esse caso, n�o sei.
 * 
 * Obs: O entry point est� em head.s
 *      Agora idle pode fazer systemcalls. 
 *
 * @todo: Criar argumento de entrada.
 *
 * Hist�rico:
 *     Vers�o 1.0, 2015 - Esse arquivo foi criado por Fred Nora.
 *     Vers�o 1.0, 2016 - Revis�o.
 *     ...
 */
 
 
//
// Includes.
// 
 
#include <types.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>   
#include <ctype.h>

#include "api.h"      

#include "tbst.h" 


//===============================================================================

//buffer pra fazer conta usando pos order
int POS_BUFFER[32];

int buffer_offset = 0;

//===============================================================================



 
//
//  ## STACK ##
//

struct stack
{
	int top;
    int items[32];
};



struct node 
{ 
	int key; 
	struct node *left, *right; 
}; 


int my_isdigit(char ch){
	
    return(ch>='0' && ch<='9');
};


// A utility function to create a new BST node 
struct node *newNode (int item){
	
	struct node *temp = (struct node *)malloc ( sizeof(struct node) );
    
	//#todo: check validation
	
	temp->key = item; 
	temp->left = temp->right = NULL;
	
	return temp; 
}; 


// A utility function to do inorder traversal of BST 
void inorder (struct node *root){
	
	if (root != NULL) 
	{ 
		inorder(root->left); 
		printf("%d \n", root->key); 
		inorder(root->right); 
	} 
}; 


//Em ordem  a+b
//desce at� o �ltimo pela esquerda, n�o havendo esquerda vai pra direita.
void exibirEmOrdem (struct node *root){
	
    if (root != NULL)
	{
		//visita a esquerda do pr�ximo
		//s� retorna no �ltimo ent�o printf funciona 
		//mostrando o conte�do do �ltimo 
		//ai visita a direita do �ltimo e desce pela esquerda,
		//n�o havendo esquerda vai pra direita.
        exibirEmOrdem (root->left);
        printf("%d ", root->key);
        
		exibirEmOrdem (root->right);
    }
};



//Pr�-ordem  +ab
void exibirPreOrdem (struct node *root){
    
	if (root != NULL)
	{
        //imprime o conte�do
		//desce at� o �ltimo pela esquerda
		//visita a direita e desce at� o �ltimo pela esquerda.
		printf("%d ", root->key);
        exibirPreOrdem(root->left);
        exibirPreOrdem(root->right);
    }
};


//P�s-ordem ab+
void exibirPosOrdem (struct node *root){
	
	//#importante
	//exibe em n�veis. de baixo para cima.
    
	if (root != NULL)
	{
		//desce at� o ultimo pela esquerda
		//visita o da direita e imprime;
        exibirPosOrdem(root->left);
        exibirPosOrdem(root->right);
		printf("%d ", root->key);
		
		if(buffer_offset<0 || buffer_offset >32)
		{
			printf("*buffer fail\n");
			return;
		}
		
		//
		//  ## IMPORTANTE ##
		//
		
		//colocar num buffer pra usar no c�lculo 
		//isso simula uma digita��o
		POS_BUFFER[buffer_offset] = (int) (root->key + '0');
		buffer_offset++;
    };
};


/* A utility function to insert a new node with given key in BST */
struct node* insert ( struct node* node, int key ){
	
	/* If the tree is empty, return a new node */
	if (node == NULL) 
        return newNode(key); 

	/* Otherwise, recur down the tree */
	if (key < node->key) 
		node->left = insert(node->left, key); 
	else if (key > node->key) 
		node->right = insert(node->right, key); 

	/* return the (unchanged) node pointer */
	return node; 
} 



/*
 *********************************************************
 *
 */
 
// Driver Program to test above functions 
// C program to demonstrate insert operation in binary search tree 

	/* Let us create following BST 
           - 
          /  \ 
         +     * 
        / \   / \ 
       4   3 2   5 */
	   
	   //4+3 - 2*5 = 12
	
int bst_main (){
	
	buffer_offset = 0;
		
	struct node *root = NULL; 
	
	//insert 111. 
	// � um finalizador, representa o igual
	//depois vamos usar o igual =
	// x = 4+3 - 2*5
	root = insert ( root, '?' ); //3 

	insert(root, '+'); //
	insert(root, '-'); //
	insert(root, '*'); //
	
	insert(root, 5);   // 
	insert(root, 2);   //	
	insert(root, 3);   // 
	insert(root, 4);   //	
	
 
    // ??
	// print inoder traversal of the BST 
	// inorder(root); 
		
	printf("\n\n em ordem: ");
	exibirEmOrdem(root);
	
	printf("\n\n pre ordem: ");
	exibirPreOrdem(root);
	
	printf("\n\n pos ordem: ");
	exibirPosOrdem(root);

	printf("\n\n");
	
	return 0; 
}; 


//===============================================================================


void push ( struct stack *s, int x ){
	
    if ( s->top > 32 )
    {
		printf("Stack Overflow�!\n");
        return;
		
    }else{
		
		s->items[ ++s->top ] = x;
    }
};


int pop (struct stack *s){
	
    if ( s->top == -1)
	{
		printf("Stack Underflow�!\n");
        return 0; //??
		
    }else{
		
		return ( s->items[ s->top-- ] );
    };
};


int oper(char c,int opnd1,int opnd2){
	
    switch (c)
    {	
		//case '*': 
        case 90:  		
		    return (opnd1*opnd2);
			
        //case '+': 
		case 91:    
			return(opnd1+opnd2);        		
			
		//case '-': 
		case 93:
		    return(opnd1-opnd2);
			
		//case '/': 
		case 95:
		    return(opnd1/opnd2);
        
		//#todo
		case '^': 
		    return 0; //return(pow(opnd1,opnd2));
        
		default: 
		    printf("oper: Invalid operator�! %d\n", c);
			return 0;
    };
};


int eval ( int *str ){
	
    int i;
    int opnd1, opnd2, val;
	char c;
	
	struct stack stk;
    
	stk.top = -1;
	
    printf("\n eval:\n");	
	
    //for ( i=0; (c = str[i]) != '?'; i++ )
    for ( i=0; (c = str[i]) != 111; i++ )		
    {
		if ( c>='0' && c<='9' )
		{
            push ( &stk, (int)( c - '0' ) );
        
		}else{
            
			//O problema � a ordem em que os operandos aparecem 
			//o �ltimo � a raiz.
			//e aqui a o operando raiz aparece no meio da express�o.
			
			opnd2 = pop (&stk);
            opnd1 = pop (&stk);
			
            val = oper ( c, opnd1, opnd2 );
            
			push ( &stk, val );
        }
    }
	
	//o resultado � o que sobrou na pilha
    return ( pop(&stk) );
}


/*
 ==================================================================================
 * iNICIALIZA��O GEN�RICA PARA TESTE DE APLICATIVO.
 */

int testtest_main (){

	int i;	
	int max;
	
	printf("\n");
	printf("Initilizing TBST.BIN ...\n\n");
	
	
    libcInitRT();
    stdioInitialize();		

	bst_main(); 
	
	//finalizador
	POS_BUFFER[buffer_offset] = '?';	
	
	max = buffer_offset;
	
	printf("\n the pos order is: "); 
	if(max >32 || max <=0)
		printf("max fail");
	
	for (i=0; i<max; i++)
	{
		printf("%d ",POS_BUFFER[i]);
	}
    
    printf ("\n \n Result after evaluation is: %d \n", eval ( (int*) &POS_BUFFER[0] ) );	
	
	printf("DONE!");	
	
	
	//refresh_screen();
	return 0;
};


//
// End.
//

