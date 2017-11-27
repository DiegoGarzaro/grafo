/***********************************
Grafos

Autor: Arthur M. Amaral
       Diego R. Garzaro
       Eder E. Hamasaki
       
Criado em: 15/11/2017
***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// ********** Definições ***********

#define true   		1
#define false  		0
#define TAM_GRAFO  21

// ********** Estruturas ***********

typedef struct vizinho{
	int no;
	int peso;
	char cor[20];
	struct vizinho *prox;
} Vizinho;

typedef struct no{
	// Dados a serem Armazenados
	Vizinho *lista_conexao;
} No;

typedef struct grafo{
	int nos;
	int caminhos;
	No *arranj_nos;
} Grafo;

// *********** Funções *************

Grafo *cria_grafo(int no);
Vizinho *cria_vizinho(int no, int peso, char cor[20]);
int cria_conexao(Grafo *gr, int no_ini, int no_fim, int peso, char cor[20]);
void inicializa_grafo(Grafo *g, int *d, int *p, int s);
void relaxa(Grafo *g, int *d, int *p, int u, int v, char last_point[20]);
int *dijkstra(Grafo *g, int in, int fim, int caminho[TAM_GRAFO]);
int existe_aberto(Grafo *g, int *aberto);
int menorDist(Grafo *g, int *aberto, int *d);
void imprime_grafo(Grafo *gr);
void imprime_caminho(Grafo *gr, int caminho[TAM_GRAFO]);

// ************* Main **************

int main(){

	Grafo *gr = cria_grafo(TAM_GRAFO);

	cria_conexao(gr,1,2,16,"Vermelho");
	cria_conexao(gr,2,3,14,"Vermelho");
	cria_conexao(gr,3,4,12,"Vermelho");
	cria_conexao(gr,4,5,12,"Vermelho");
	cria_conexao(gr,5,6,14,"Vermelho");

	cria_conexao(gr,2,15,7,"Amarelo");
	cria_conexao(gr,2,8,8,"Amarelo");
	cria_conexao(gr,8,12,11,"Amarelo");
	cria_conexao(gr,15,16,7,"Amarelo");
	cria_conexao(gr,16,17,12,"Amarelo");
	cria_conexao(gr,17,18,9,"Amarelo");
	
	cria_conexao(gr,3,16,11,"Roxo");
	cria_conexao(gr,3,13,13,"Roxo");
	cria_conexao(gr,10,13,11,"Roxo");
	cria_conexao(gr,16,19,13,"Roxo");
	cria_conexao(gr,19,20,12,"Roxo");
	
	cria_conexao(gr,5,17,10,"Azul");
	cria_conexao(gr,5,14,9,"Azul");
	cria_conexao(gr,10,12,17,"Azul");
	cria_conexao(gr,9,13,7,"Azul");
	cria_conexao(gr,13,14,9,"Azul");

	cria_conexao(gr,6,11,15,"Verde");
	cria_conexao(gr,7,8,15,"Verde");
	cria_conexao(gr,8,9,11,"Verde");
	cria_conexao(gr,9,10,13,"Verde");
	cria_conexao(gr,10,11,16,"Verde");


//	imprime_grafo(gr);

	printf("\n\n");

	int in, fim, caminho[TAM_GRAFO], i;

	for(i=0;i<TAM_GRAFO;i++){
		caminho[i]=-1;
	}

	printf("Digite o ponto inicial: ");
	do{
		scanf("%d", &in);
	}while(in<0 || in>20);
	printf("Digite o ponto final: ");
	do{
		scanf("%d", &fim); 
	}while(fim<0 || fim>20);

	int *r = dijkstra(gr,(in),(fim),caminho);

	printf("\nIr do ponto %d ao %d:\n", in, fim);

	imprime_caminho(gr,caminho);

	printf("Tempo Total: %d minutos\n", r[fim]);

//	for(i=1;(in+i)<fim;i++){
//		printf("Distancia entre os pontos: %d -> %d) = %d\n", in, (in+i), r[(in+i)-1]);
//	}

	return 0;
}

// *********** Funções *************

Grafo *cria_grafo(int no){
	Grafo *g = (Grafo *) malloc(sizeof(Grafo));
	g->nos = no;
	g->caminhos = 0;
	g->arranj_nos = (No *) malloc(no*sizeof(No));
	int i;
	for(i=0;i<no;i++)
		g->arranj_nos[i].lista_conexao = NULL;
	return(g);
}

Vizinho *cria_vizinho(int no, int peso, char cor[20]){
	Vizinho *temp = (Vizinho *) malloc(sizeof(Vizinho));
	temp->no = no;
	temp->peso = peso;
	strcpy(temp->cor, cor);
	temp->prox = NULL;
	return(temp);
}

int cria_conexao(Grafo *gr, int no_ini, int no_fim, int peso, char cor[20]){
	if(!gr) return(false);
	if((no_fim<0) || (no_fim>=gr->nos)) return(false);
	if((no_ini<0) || (no_ini>=gr->nos)) return(false);
	Vizinho *novo = cria_vizinho(no_fim, peso, cor);
	novo->prox = gr->arranj_nos[no_ini].lista_conexao;
	gr->arranj_nos[no_ini].lista_conexao = novo;
	novo = cria_vizinho(no_ini, peso, cor);
	novo->prox = gr->arranj_nos[no_fim].lista_conexao;
	gr->arranj_nos[no_fim].lista_conexao = novo;
	gr->caminhos++;
	return(true);
}

void inicializa_grafo(Grafo *g, int *d, int *p, int s){
	int v;
	for(v=0;v<g->nos;v++){
		d[v] = INT_MAX/2;
		p[v] = 0;
	}
	d[s] = 0;
}

void relaxa(Grafo *g, int *d, int *p, int u, int v, char last_point[20]){
	static char cor_ant[3];
	static int flag=0;
	int adicional = 0;
	Vizinho *ad = g->arranj_nos[u].lista_conexao;
	while(ad && ad->no != v) {
		ad = ad->prox;
	}
	if(ad){
		if(d[v] > d[u] + ad->peso + adicional){
			d[v] = d[u] + ad->peso + adicional;
			p[v] = u;
		}
	}
	printf("Caminho Novo: %d %s %d\n", ad->no, ad->cor, d[v]);
}

int *dijkstra(Grafo *g, int in, int fim, int caminho[TAM_GRAFO]){
	int *d = (int *) malloc(g->nos*sizeof(int));
	int p[g->nos];
	int aberto[g->nos], ult_ponto;
	char cor_ant[20];
	inicializa_grafo(g,d,p,in);

	int i;
	for(i=0;i<g->nos;i++) aberto[i] = true;

	while(existe_aberto(g,aberto)){
		int u = menorDist(g,aberto,d);
		aberto[u] = false;
//		printf("%d: ", u+1);
		Vizinho *ad = g->arranj_nos[u].lista_conexao;
		while(ad){
			relaxa(g,d,p,u,ad->no,cor_ant);
			ad = ad->prox;
		}
		printf("\n");
		if(!existe_aberto(g,aberto)){
		int j, k, z=0;
				caminho[TAM_GRAFO]=fim;
				for(j=fim,k=TAM_GRAFO-1;j!=in;k--){
					caminho[k] = p[j];
					j = p[j];
					z++;
					caminho[0]=z;
				}
		}
	}
	return(d);
}

int existe_aberto(Grafo *g, int *aberto){
	int i;
	for(i=0;i<g->nos;i++)
		if(aberto[i]) return(true);
	return(false);
}

int menorDist(Grafo *g, int *aberto, int *d){
	int i;
	for(i=0;i<g->nos;i++)
		if(aberto[i]) break;
	if(i==g->nos) return(-1);
	int menor = i;
	for(i=menor+1;i<g->nos;i++)
		if(aberto[i] && (d[menor]>d[i]))
			menor = i;
	return(menor);
}

void imprime_grafo(Grafo *gr){
	printf("Nos: %d. Caminhos: %d.\n", gr->nos, gr->caminhos);
	int i;
	for(i=0;i<gr->nos;i++){
		printf("No %d: ", i);
		Vizinho *ad = gr->arranj_nos[i].lista_conexao;
		while(ad){
			printf("n%d(%d - %s) ", ad->no, ad->peso, ad->cor);
			ad=ad->prox;
		}
		printf("\n");
	}
}

void imprime_caminho(Grafo *gr, int caminho[TAM_GRAFO]){
	int i, k, z = caminho[0], dist=0, j;																			// Declaração das variáveis auxiliares
	char linha_ant[20];
	k = TAM_GRAFO-caminho[0];																			// Posição do vetor do primeiro ponto
	printf("\n");

	for(i=0;i<20;i++) linha_ant[i]=0;

	for(i=0;i<z;i++){	// Loop
		Vizinho *ad = gr->arranj_nos[caminho[k+i]].lista_conexao;										// Cria variável tipo estrutura dos pontos adjacentes (vizinhos)
		while(ad->no != caminho[k+(i+1)] && i<z){														// Enquanto ponto adjacente != do proximo ponto do grafo
			ad=ad->prox;																				// Muda de adjacente
		}
		if(i==0) linha_ant[3] = ad->cor[3];
			printf("%d --%s(%d)--> %d\n", caminho[k+i], ad->cor, ad->peso, caminho[k+(i+1)]);		//Imprime bonito
	}
}
