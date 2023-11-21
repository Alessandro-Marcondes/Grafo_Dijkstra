#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define true 1
#define false 0
#define BRANCO 0
#define AMARELO 1
#define VERMELHO 2


typedef int TIPOPESO;
typedef int TIPOCHAVE;

typedef struct adjacencia{
	int vertices;
	TIPOPESO peso;
	struct adjacencia *prox;
}ADJACENCIA;

typedef struct vertice{
	ADJACENCIA *cab;
}VERTICE;

typedef struct grafo{
	int vertices;
	int arestas;
	VERTICE *adj;
}GRAFO;

typedef struct {
	TIPOCHAVE chave;
}REGISTRO;

typedef struct aux{
	REGISTRO reg;
	struct aux* prox;
}ELEMENTO, *PONT;

typedef struct {
	PONT inicio;
	PONT fim;
}FILA;

GRAFO *criaGrafo(int v){
	GRAFO *g = (GRAFO *)malloc(sizeof(GRAFO));
	g->vertices = v;
	g->arestas = 0;
	g->adj = (VERTICE *)malloc(v*sizeof(VERTICE));
	int i;
	for(i=0; i<v; i++)
		g->adj[i].cab = NULL;
	return(g);
}

ADJACENCIA *criaAdj(int v, int peso){
	ADJACENCIA *temp = (ADJACENCIA *)malloc(sizeof(ADJACENCIA));
	temp->vertices = v;
	temp->peso = peso;
	temp->prox = NULL;
	return(temp);
}

bool criaAresta(GRAFO *gr, int vi, int vf, TIPOPESO p){
	if(!gr) return(false);
	if((vf<0)||(vf >=gr->vertices)) return(false);
	if((vi<0)||(vi >=gr->vertices)) return(false);
	ADJACENCIA *novo = criaAdj(vf,p);
	novo->prox = gr->adj[vi].cab;
	gr->adj[vi].cab = novo;
	gr->arestas++;
	return(true);
}

void imprime(GRAFO *gr){
	printf("Vértice: %d. Arestas: %d. \n", gr->vertices,gr->arestas);
	int i;
	for(i=0; i<gr->vertices; i++){
		printf("v%d: ",i);
		ADJACENCIA *ad = gr->adj[i].cab;
		while(ad) {
			printf("v%d(%d) ",ad->vertices, ad->peso);
			ad = ad->prox;
		}
		printf("\n");
	}
}

//Busca por profundidade

void visitaP(GRAFO *g, int u, int *cor){
	cor[u] = 1;
	ADJACENCIA *v = g->adj[u].cab;
	while(v){
		if(cor[v->vertices] == 0) visitaP(g,v->vertices,cor);
		v = v->prox;
	}
	cor[u] = 2;
}


void profundidade(GRAFO *g){
	int cor[g->vertices];
	
	int u;
	for(u=0; u<g->vertices; u++){
		cor[u] = 0;
	}
	for (u=0; u<g->vertices; u++){
		if (cor[u] == 0)
		visitaP(g,u,cor);
	}
}



//Fila
void inicializarFila(FILA *f){
	f->inicio = NULL;
	f->fim = NULL;
}
bool inserirNaFila(FILA* f,REGISTRO reg){
	PONT novo = (PONT) malloc(sizeof(ELEMENTO));
	novo->reg = reg;
	novo->prox = NULL;
	if(f->inicio == NULL) f->inicio = novo;
	else f->fim->prox = novo;
	f->fim = novo;
	return true;
}

bool excluirDaFila(FILA *f, REGISTRO* reg){
	if(f->inicio == NULL) return false;
	*reg = f->inicio->reg;
	PONT apagar = f->inicio;
	free(apagar);
	if(f->inicio == NULL) f->fim = NULL;
	return true;
}

//busca em Largura

void visitaL(GRAFO *g, int s, bool *expl){
	FILA f;
	inicializarFila(&f);
	expl[s] = true;
	REGISTRO *u = (REGISTRO *)malloc(sizeof(REGISTRO));
	u->chave =s;
	inserirNaFila(&f,*u);
	
	while(f.inicio){
		excluirDaFila(&f,u);
		ADJACENCIA *v = g->adj[u->chave].cab;
		
		while(v) {
			if(!expl[v->vertices]){
				expl[v->vertices] = true;
				u->chave = v->vertices;
				inserirNaFila(&f,*u);
			}
			v = v->prox;
		}
	}
	free(u);
}


void largura(GRAFO *g){
	bool expl[g->vertices];
	
	int u;
	for (u=0; u<g->vertices; u++)
		expl[u] = false;
	for (u=0; u<g->vertices; u++)
		if(!expl[u]) visitaL(g,u,expl);
}




//Algoritmo de Dujkstra
void inicializaD(GRAFO *g, int *d, int *p, int s){
	int v;
	for(v=0; v<g->vertices; v++){
		d[v] = INT_MAX/2;
		p[v] = -1;
	}
	d[s] = 0;
}

void relaxa(GRAFO *g, int *d, int *p, int u, int v){
	ADJACENCIA *ad = g->adj[u].cab;
	while(ad && ad->vertices != v)
		ad = ad->prox;
	if(ad){
		if(d[v] > d[u] + ad->peso){
			d[v] = d[u] + ad->peso;
			p[v] = u;
		}
	}
}

bool existeAberto(GRAFO *g, bool *aberto){
	int i;
	for(i=0; i<g->vertices;i++)
		if(aberto[i]) return(true);
	return(false);
}

int menorDist(GRAFO *g, bool *aberto, int *d){
	int i;
	for(i=0; i<g->vertices; i++)
		if(aberto[i]) break;
	if (i==g->vertices) return(-1);
	int menor = i;
	for(i=menor+1; i<g->vertices; i++)
		if(aberto[i] && (d[menor]>d[i]))
			menor = i;
		return(menor);
}

int *dijkstra(GRAFO *g, int s){
	int *d = (int *)malloc(g->vertices* sizeof(int));
	int p[g->vertices];
	bool aberto[g->vertices];
	inicializaD(g,d,p,s);
	
	int i;
	for(i=0; i<g->vertices;i++)
		aberto[i] = true;
	while (existeAberto(g, aberto)){
		int u = menorDist(g, aberto, d);
		aberto[u] = false;
		ADJACENCIA *ad = g->adj[u].cab;
		while(ad){
			relaxa(g,d,p,u,ad->vertices);
			ad = ad->prox;
		}
	}
	return(d);
}


int main (void){
	GRAFO * gr = criaGrafo(6);
	criaAresta(gr, 0, 1, 1);
    	criaAresta(gr, 0, 2, 4);
    
    	criaAresta(gr, 1, 0, 1);
    	criaAresta(gr, 1, 2, 4);
    	criaAresta(gr, 1, 3, 2);
	criaAresta(gr, 1, 4, 7);
	    
	criaAresta(gr, 2, 0, 4);
    	criaAresta(gr, 2, 1, 4);
    	criaAresta(gr, 2, 3, 3);
	criaAresta(gr, 2, 4, 5);
	
	criaAresta(gr, 3, 1, 2);
    	criaAresta(gr, 3, 2, 3);
    	criaAresta(gr, 3, 4, 4);
	criaAresta(gr, 3, 5, 6);
	
	criaAresta(gr, 4, 1, 5);
    	criaAresta(gr, 4, 2, 6);
    	criaAresta(gr, 4, 3, 4);
	criaAresta(gr, 4, 4, 7);
	
	criaAresta(gr, 5, 3, 5);
    	criaAresta(gr, 5, 4, 7);
    
	

	
	printf("Matriz de adjacência:\n");
	imprime(gr);
    	printf("\n");

	
	int *r = dijkstra(gr,0);
	
	int i;
	for(i=0; i<gr->vertices; i++)
		printf("D(v0 -> v%d) = %d\n", i, r[i]);
	
}
