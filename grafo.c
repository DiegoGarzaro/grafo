/***********************************
Grafos

Autor: Diego R.
Criado em: 15/11/2017
***********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// ********** Definições ***********

#define true   1
#define false  0

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
void relaxa(Grafo *g, int *d, int *p, int u, int v);
int *dijkstra(Grafo *g, int s);
int existe_aberto(Grafo *g, int *aberto);
int menorDist(Grafo *g, int *aberto, int *d);
void imprime_grafo(Grafo *gr);

// ************* Main **************

int main(){

	Grafo *gr = cria_grafo(20);

	cria_conexao(gr,0,1,16,"Vermelho");
	cria_conexao(gr,1,2,14,"Vermelho");
	cria_conexao(gr,2,3,12,"Vermelho");
	cria_conexao(gr,3,4,12,"Vermelho");
	cria_conexao(gr,4,5,14,"Vermelho");

	cria_conexao(gr,1,14,7,"Amarelo");
	cria_conexao(gr,1,7,8,"Amarelo");
	cria_conexao(gr,7,11,11,"Amarelo");
	cria_conexao(gr,14,15,7,"Amarelo");
	cria_conexao(gr,15,16,12,"Amarelo");
	cria_conexao(gr,16,17,9,"Amarelo");
	
	cria_conexao(gr,2,15,11,"Roxo");
	cria_conexao(gr,2,12,13,"Roxo");
	cria_conexao(gr,9,12,11,"Roxo");
	cria_conexao(gr,15,18,13,"Roxo");
	cria_conexao(gr,18,19,12,"Roxo");
	
	cria_conexao(gr,4,16,10,"Azul");
	cria_conexao(gr,4,13,9,"Azul");
	cria_conexao(gr,9,11,17,"Azul");
	cria_conexao(gr,8,12,7,"Azul");
	cria_conexao(gr,12,13,9,"Azul");

	cria_conexao(gr,5,10,15,"Verde");
	cria_conexao(gr,6,7,15,"Verde");
	cria_conexao(gr,7,8,11,"Verde");
	cria_conexao(gr,8,9,13,"Verde");
	cria_conexao(gr,9,10,16,"Verde");


//	imprime_grafo(gr);

	printf("\n\n");

	int *r = dijkstra(gr,3);

	int i;
	for(i=0;i<gr->nos;i++)
		printf("D(v1 -> v%d) = %d\n", i+1, r[i]);

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

void relaxa(Grafo *g, int *d, int *p, int u, int v){
	Vizinho *ad = g->arranj_nos[u].lista_conexao;
	while(ad && ad->no != v) ad = ad->prox;
	if(ad){
		if(d[v] > d[u] + ad->peso){
			d[v] = d[u] + ad->peso;
			p[v] = u;
		}
	}
}

int *dijkstra(Grafo *g, int s){
	int *d = (int *) malloc(g->nos*sizeof(int));
	int p[g->nos];
	int aberto[g->nos];
	inicializa_grafo(g,d,p,s);

	int i;
	for(i=0;i<g->nos;i++) aberto[i] = true;

	while(existe_aberto(g,aberto)){
		int u = menorDist(g,aberto,d);
		aberto[u] = false;

		Vizinho *ad = g->arranj_nos[u].lista_conexao;
		while(ad){
			relaxa(g,d,p,u,ad->no);
			ad = ad->prox;
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