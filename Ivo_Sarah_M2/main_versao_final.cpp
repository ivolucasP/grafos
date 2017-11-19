#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio2.h>

#define INFINITO 2147483647 //N�mero m�ximo que um integer consegue armazenar. Constante Simb�lica.

using namespace std;

struct Vertice;
struct Arco;
struct TElementoVertice;

struct Arco{
    string origem;
    string destino;
    double valor;
};

struct TElementoArco{
    Arco dado;
    TElementoArco *prox;
    TElementoArco *anterior;
};

struct TListaArco{
    TElementoArco *inicio;
    TElementoArco *fim;
    int qtd;
};

struct Vertice{
    string nome;
    TElementoVertice *verticeAnterior;
    double distancia;
    TListaArco arcos;
    bool visitado;
    bool aberto;
    double grau;
    string cor;
};

struct TElementoVertice {
    Vertice dado;
    TElementoVertice *prox;
    TElementoVertice *ante;
};

struct TListaVertice {
    TElementoVertice *inicio;
    TElementoVertice *fim;
    int qtd;
};

struct Grafo{
    bool isPonderado;
    bool isOrientado;
    TListaVertice vertices;
    int Arestas;
};

void menu(Grafo &grafo);

void inicializaArco(TListaArco &l){
    l.inicio = NULL;
    l.fim = NULL;
    l.qtd = 0;
}

void inicializaVertice(TListaVertice &l) {
    l.inicio = NULL;
    l.fim = NULL;
    l.qtd = 0;

}

void bubbleSortNomeVertice(TListaVertice &l){
    TElementoVertice *aux;
    TElementoVertice *teste = l.inicio;
    TElementoVertice *save = new TElementoVertice;
    while (teste->prox != NULL){
        aux = teste->prox;
        while (aux != NULL){
            if (teste->dado.nome > aux->dado.nome){
                save->dado = teste->dado;
                teste->dado = aux->dado;
                aux->dado = save->dado;
            }
            aux = aux->prox;
        }
        teste = teste->prox;
    }
    return;
}

void insereVertice(Grafo &grafo){
    TElementoVertice *novo = new TElementoVertice;
    string nome;
    if (novo == NULL)
        menu(grafo);
    cout << "Informe o nome do v�rtice\n";
    getline(cin, nome);
    novo->dado.nome = nome;
    inicializaArco(novo->dado.arcos);
    novo->prox = NULL;
    novo->ante = grafo.vertices.fim;
    novo->dado.visitado = false;
    novo->dado.grau = 0;
    novo->dado.cor = "null";
    if(grafo.vertices.qtd == 0)
        grafo.vertices.inicio = novo;
    else
        grafo.vertices.fim->prox = novo;
    grafo.vertices.fim = novo;
    grafo.vertices.qtd++;
    bubbleSortNomeVertice(grafo.vertices);
    menu(grafo);
}

TElementoVertice *retornaVertice(Grafo &grafo, string nome){
    TElementoVertice *nav = grafo.vertices.inicio;
    if (nav == NULL)
        return NULL;
    for(int i = 0; i < grafo.vertices.qtd; i++){
        if (nav->dado.nome == nome)
            return nav;
        nav = nav->prox;
    }
    return NULL;
}

void bubbleSortNome(TListaArco &l){
    TElementoArco *aux;
    TElementoArco *teste = l.inicio;
    TElementoArco *save = new TElementoArco;
    while (teste->prox != NULL){
        aux = teste->prox;
        while (aux != NULL){
            if (teste->dado.destino > aux->dado.destino){
                save->dado = teste->dado;
                teste->dado = aux->dado;
                aux->dado = save->dado;
            }
            aux = aux->prox;
        }
        teste = teste->prox;
    }
    return;
}

void bubbleSortValor(TListaArco &l){
    TElementoArco *aux;
    TElementoArco *teste = l.inicio;
    TElementoArco *save = new TElementoArco;
    while (teste->prox != NULL){
        aux = teste->prox;
        while (aux != NULL){
            if (teste->dado.valor > aux->dado.valor){
                save->dado = teste->dado;
                teste->dado = aux->dado;
                aux->dado = save->dado;
            }
            aux = aux->prox;
        }
        teste = teste->prox;
    }
    return;
}

void insereAresta(Grafo &grafo, string origem, string destino, int valor){
    TElementoArco *novo = new TElementoArco;
    if (novo == NULL)
        menu(grafo);
    novo->dado.origem = origem;
    novo->dado.destino = destino;
    novo->dado.valor = valor;
    novo->prox = NULL;
    TElementoVertice *nav = retornaVertice(grafo, origem);
    if(nav == NULL)
        menu(grafo);
    novo->anterior = nav->dado.arcos.fim;
    if(nav->dado.arcos.qtd == 0)
        nav->dado.arcos.inicio = novo;
    else
        nav->dado.arcos.fim->prox = novo;
    nav->dado.arcos.fim = novo;
    nav->dado.arcos.qtd++;
    nav->dado.grau++;
    if(grafo.isPonderado)
        bubbleSortValor(nav->dado.arcos);
    else
        bubbleSortNome(nav->dado.arcos);
    nav = retornaVertice(grafo, destino);
    nav->dado.grau++;
}

void removeAresta(Grafo &grafo, string origem, string destino){
    TElementoVertice *nav = grafo.vertices.inicio;
    TElementoArco *navnav;
    TElementoArco *aux;
    bool flag = false;
    if (nav != NULL){
        for(int i = 0; i < grafo.vertices.qtd and !flag ; i++){
            navnav = nav->dado.arcos.inicio;
            if (navnav != NULL){
                for(int j = 0; j < nav->dado.arcos.qtd; j++){
                    if(nav->dado.nome == origem){
                        if(navnav->dado.destino == destino){
                            aux = navnav->prox;
                            if (navnav->anterior != NULL)
                                navnav->anterior->prox = aux;
                            else
                                nav->dado.arcos.inicio = aux;
                            if (aux != NULL)
                                aux->anterior = navnav->anterior;
                            else
                                nav->dado.arcos.fim = navnav->anterior;
                            delete navnav;
                            nav->dado.arcos.qtd--;
                            nav->dado.grau--;
                            flag = true;
                            break;
                        }
                    }
                    else
                        break;
                    navnav = navnav->prox;
                }
            }
            nav = nav->prox;
        }
    }
}

void removeVertice(Grafo &grafo, string nome){
    TElementoVertice *nav = grafo.vertices.inicio;
    TElementoVertice *aux;
    TElementoArco *navnav;
    if(nav != NULL){
        for(int i = 0; i < grafo.vertices.qtd; i++){
            navnav = nav->dado.arcos.inicio;
            if (navnav != NULL){
                for (int j = 0; j < nav->dado.arcos.qtd; j++){
                    if(navnav->dado.origem == nome or navnav->dado.destino == nome){
                        removeAresta(grafo, navnav->dado.origem, navnav->dado.destino);
                    }
                    navnav = navnav->prox;
                }
            }
            nav = nav->prox;
        }
        nav = grafo.vertices.inicio;
        for (int i = 0; i < grafo.vertices.qtd; i++){
            if (nav->dado.nome == nome){
                aux = nav->prox;
                if (nav->ante != NULL)
                    nav->ante->prox = aux;
                else
                    grafo.vertices.inicio = aux;
                if(aux != NULL)
                    aux->ante = nav->ante;
                else
                    grafo.vertices.fim = nav->ante;
                delete nav;
                grafo.vertices.qtd--;
                break;
            }
            nav = nav->prox;
        }
    }
}

bool existeAresta(Grafo &grafo, string origem, string destino){
    TElementoVertice *nav = grafo.vertices.inicio;
    TElementoArco *navnav;

    if(nav != NULL){
        for (int i = 0; i < grafo.vertices.qtd; i++){
            if(nav->dado.nome == origem){
                navnav = nav->dado.arcos.inicio;
                for (int j = 0; j < nav->dado.arcos.qtd; j++){
                    if(navnav->dado.destino == destino)
                        return true;
                    navnav = navnav->prox;
                }
                break;
            }
            nav = nav->prox;
        }
    }
    return false;
}

void resetaVerticesVisitados(Grafo &grafo){
    TElementoVertice *nav = grafo.vertices.inicio;
    for (int i = 0; i < grafo.vertices.qtd; i++){
        nav->dado.visitado = false;
        nav = nav->prox;
    }
}

void DFS(Grafo &grafo, TElementoVertice* origem){
    TElementoArco *navnav = origem->dado.arcos.inicio;
    TElementoVertice *nav;
    cout << origem->dado.nome << ", ";
    origem->dado.visitado = true;
    if(navnav != NULL){
        for(int i = 0; i < origem->dado.arcos.qtd; i++){
            nav = retornaVertice(grafo, navnav->dado.destino);
            if(!nav->dado.visitado){
                DFS(grafo, nav);
            }
            navnav = navnav->prox;
        }
    }
}

void insereNaLista(TListaVertice &l, Vertice dado){
    TElementoVertice *novo = new TElementoVertice;
    if(novo == NULL)
        return;
    novo->dado = dado;
    novo->prox = NULL;
    novo->ante = l.fim;
    if(l.inicio == NULL)
        l.inicio = novo;
    else
        l.fim->prox = novo;
    l.fim = novo;
    l.qtd++;
}

void BFS(Grafo &grafo, TElementoVertice* origem){
    TListaVertice listaBFS;
    inicializaVertice(listaBFS);
    insereNaLista(listaBFS, origem->dado);
    TElementoVertice *nav;
    TElementoArco *navnav;
    TElementoVertice *navInterno;
    origem->dado.visitado = true;
    nav = listaBFS.inicio;
    for(int i = 0; i < listaBFS.qtd; i++){
        navnav = nav->dado.arcos.inicio;
        for(int j = 0; j < nav->dado.arcos.qtd; j++){
            navInterno = retornaVertice(grafo, navnav->dado.destino);
            if(!navInterno->dado.visitado){
                insereNaLista(listaBFS, navInterno->dado);
                navInterno->dado.visitado = true;
            }
            navnav = navnav->prox;
        }
        cout << nav->dado.nome << ", ";
        nav = nav->prox;
    }
}

void imprimeMatriz(Grafo &grafo){
    TElementoVertice *navLinha = grafo.vertices.inicio;
    TElementoVertice *navColuna;
    TElementoArco *navnav;
    cout << "\t";
    for (int i = 0; i < grafo.vertices.qtd; i++){
        cout << navLinha->dado.nome << "\t";
        navLinha = navLinha->prox;
    }
    cout << "\n";
    navLinha = grafo.vertices.inicio;
    for (int linha = 0; linha < grafo.vertices.qtd; linha++){
        navColuna = grafo.vertices.inicio;
        cout << navLinha->dado.nome << "\t";
        for (int coluna = 0; coluna < grafo.vertices.qtd; coluna++){
            if(existeAresta(grafo, navLinha->dado.nome, navColuna->dado.nome)){
                navnav = navLinha->dado.arcos.inicio;
                for(int k = 0; k < navLinha->dado.arcos.qtd; k++){
                    if(navnav->dado.destino == navColuna->dado.nome)
                        cout << navnav->dado.valor << "\t";
                    navnav = navnav->prox;
                }
            }else
                cout << "0\t";
            navColuna = navColuna->prox;
        }
        cout << "\n";
        navLinha = navLinha->prox;
    }
}

void dijkstra(Grafo g, string origem_p, string destino_p){
    TElementoVertice *navVertice;

    //Obter Posi��o do Vertice Origem para iniciar
    TElementoVertice *navAux = g.vertices.inicio;
    while(navAux!= NULL){
        if(navAux->dado.nome == origem_p){
            navVertice = navAux;
            navVertice->dado.verticeAnterior = NULL;
        }
        navAux = navAux->prox;
    }

    TElementoVertice *aux;
    TElementoVertice *origem;

    //Obter Posi��o do Vertice de Origem para comparacao
    navAux = g.vertices.inicio;
    while(navAux!= NULL){
        if(navAux->dado.nome == origem_p){
            origem = navAux;
        }
        navAux = navAux->prox;
    }

    while (navVertice != NULL){

        TElementoArco *navArco = navVertice->dado.arcos.inicio;

        if(navVertice->dado.nome == origem->dado.nome){

            navVertice->dado.distancia = 0;

        }

        while(navArco != NULL){

            // Obtem posi��o do vertice correspondente a liga��o da aresta para manipul�-lo
            navAux = g.vertices.inicio;

            while(navAux!= NULL){

                if(navAux->dado.nome == navArco->dado.destino){

                    aux = navAux;

                }
                navAux = navAux->prox;
            }

            // Se � infinito, � porque n�o foi explorado ainda.
            //Ent�o recebe o valor da vertice em liga��o + o valor da aresta.

            if (aux != NULL &&
                aux->dado.distancia == INFINITO){

                aux->dado.distancia = navVertice->dado.distancia + navArco->dado.valor;

                aux->dado.verticeAnterior = navVertice;

            // Se n�o � infinito, significa que j� foi explorado alguma vez
            // Ent�o verifica se o caminho atual (Vertice de liga��o + valor da aresta)
            // � menor que o caminho registrado atualmente. Se for menor, recebe o novo caminho.
            } else if (aux != NULL &&
                       aux->dado.distancia != INFINITO &&
                      (navVertice->dado.distancia + navArco->dado.valor) < aux->dado.distancia){

                aux->dado.distancia = navVertice->dado.distancia + navArco->dado.valor;

                aux->dado.verticeAnterior = navVertice;

            }

            navArco = navArco->prox;
        }

        navVertice->dado.visitado = true;
        navVertice = navVertice->prox;


    }

    // Vetor responsavel pelo cout do caminho de origem ao destino
    string vet[g.vertices.qtd];

    for (int i=0; i < g.vertices.qtd; i++){
        vet[i] = " ";
    }

    //Obtem posi��o do vertice de destino
    navAux = g.vertices.inicio;
    while(navAux!= NULL){
        if(navAux->dado.nome == destino_p){
            navVertice = navAux;
        }
        navAux = navAux->prox;
    }

    vet[0] = navVertice->dado.nome;

    // Navega do destino � origem

    while (navVertice != NULL){


        static int i = 1;

        //Faz o caminho do destino at� a origem. (Sim, de tras pra frente)
        navAux = g.vertices.inicio;
        while(navAux!= NULL){

            if(navVertice->dado.verticeAnterior != NULL &&
               navAux->dado.nome == navVertice->dado.verticeAnterior->dado.nome){
                vet[i] = navAux->dado.nome;
            }

            navAux = navAux->prox;
        }

            navVertice = navVertice->dado.verticeAnterior;
            i++;
    }


    //Imprime o vetor de tras pra frente, ou seja, da origem at� o destino
    for(int i = g.vertices.qtd-1; i>=0; i--){

        if (vet[i] != " "){
            cout << vet[i] << " ";
        }

    }
    cout << endl;
    system("pause");
    menu(g);

}

void inicializaDijkstra(Grafo &g, string origem, string destino){
    TElementoVertice *nav = g.vertices.inicio;

    while (nav != NULL){
        nav->dado.distancia = INFINITO;
        nav->dado.aberto = true;
        nav->dado.verticeAnterior = NULL;

        nav = nav->prox;
    }

    dijkstra(g, origem, destino);

}

void ordenaParaColorir(TListaVertice &l){ ///Welsh e Powell
    TElementoVertice *aux;
    TElementoVertice *teste = l.inicio;
    TElementoVertice *save = new TElementoVertice;
    while (teste->prox != NULL){
        aux = teste->prox;
        while (aux != NULL){
            if (teste->dado.grau < aux->dado.grau){
                save->dado = teste->dado;
                teste->dado = aux->dado;
                aux->dado = save->dado;
            }
            aux = aux->prox;
        }
        teste = teste->prox;
    }
    return;
}

void colore(Grafo &grafo, string vertice){
    TElementoVertice *nav = retornaVertice(grafo, vertice);
    string listaCores[5] = {"null", "null", "null", "null", "null"};
    string listaCoresPadrao[5] = {"vermelho", "verde", "azul", "amarelo", "laranja"};
    TElementoArco *navnav;
    TElementoVertice *temp;
    TElementoVertice *navAux = grafo.vertices.inicio;
    bool flag = false, insereCor;
    int k = 0, contadorCor = 0;

    if (navAux != NULL){
        for(int i = 0; i < grafo.vertices.qtd and !flag ; i++){
            navnav = navAux->dado.arcos.inicio;
            if (navnav != NULL){
                for(int j = 0; j < navAux->dado.arcos.qtd; j++){
                    insereCor = false;
                    if(navnav->dado.destino == nav->dado.nome){
                        insereCor = true;
                        for (int k = 0; k < contadorCor; k++){
                            if(listaCores[k] == navAux->dado.cor){
                                insereCor = false;
                            }
                        }
                    }
                    else if (navnav->dado.origem == nav->dado.nome){
                        temp = retornaVertice(grafo, navnav->dado.destino);
                        insereCor = true;
                        for (int k = 0; k < contadorCor; k++){
                            if(listaCores[k] == temp->dado.cor){
                                insereCor = false;
                            }
                        }
                    }
                    if(insereCor){
                        if(navAux->dado.cor != "null"){
                            listaCores[contadorCor] = navAux->dado.cor;
                            contadorCor++;
                        }
                    }
                    navnav = navnav->prox;
                }
            }
            navAux = navAux->prox;
        }
    }

    flag = false;
    bool flag2 = false;

    for (int i = 0; i < 5 and !flag2; i++){
        flag = false;
        for (int j = 0; j < 5 and !flag; j++){
            if (listaCoresPadrao[i] == listaCores[j]){
                flag = true;
            }
        }
        if(!flag){
            nav->dado.cor = listaCoresPadrao[i];
            flag2 = true;
        }
    }
}

bool TemCicloTres(Grafo &grafo){
    TElementoVertice *NavegadorPrincipal = grafo.vertices.inicio;
    TElementoArco *ArcosPrincipal;
    TElementoVertice *Navegador2;
    TElementoArco *Arcos2;
    TElementoVertice *Navegador3;
    TElementoArco *Arcos3;

    for(int i = 0; i < grafo.vertices.qtd; i++){
        ArcosPrincipal = NavegadorPrincipal->dado.arcos.inicio;
        for(int j = 0; j < NavegadorPrincipal->dado.arcos.qtd; j++){
            Navegador2 = retornaVertice(grafo, ArcosPrincipal->dado.destino);
            Arcos2 = Navegador2->dado.arcos.inicio;
            for(int k = 0; k < Navegador2->dado.arcos.qtd; k++){
                Navegador3 = retornaVertice(grafo, Arcos2->dado.destino);
                Arcos3 = Navegador3->dado.arcos.inicio;
                for(int l = 0; l < Navegador3->dado.arcos.qtd; l++){
                    if(Arcos3->dado.destino == NavegadorPrincipal->dado.nome)
                        return true;
                    Arcos3 = Arcos3->prox;
                }
                Arcos2 = Arcos2->prox;
            }
            ArcosPrincipal = ArcosPrincipal->prox;
        }
        NavegadorPrincipal = NavegadorPrincipal->prox;
    }

    return false;
}

void preenche_Q_Prim(Grafo &grafo, string *Q){
    TElementoVertice *nav = grafo.vertices.inicio;
    for(int i = 0; i < grafo.vertices.qtd; i++){
        Q[i] = nav->dado.nome;
        nav = nav->prox;
    }
}

int retornaValorAresta(TElementoVertice *vertice, string destino){
    TElementoArco *Arcos = vertice->dado.arcos.inicio;
    for(int i = 0; i < vertice->dado.arcos.qtd; i++){
        if(Arcos->dado.destino == destino)
            return Arcos->dado.valor;
        Arcos = Arcos->prox;
    }
    return 0;
}

bool removeDeQ_Prim(string *Q, string dado, int qtd){
    for (int i = 0; i < qtd; i++){
        if (Q[i] == dado){
            for(int j = i; j < qtd-1; j++){
                Q[j] = Q[j+1];
            }
            return true;
        }
    }
    return false;
}

void prim(Grafo &grafo){
    string S[grafo.vertices.qtd], S_Vertices[grafo.vertices.qtd], Q[grafo.vertices.qtd], vertice_inicial, menor_origem, menor_destino;
    int countS = 0, countS_V = 0, countQ = grafo.vertices.qtd, menor_valor, tmp;
    TElementoVertice *vertice;
    TElementoArco *Arcos;
    preenche_Q_Prim(grafo, Q);
    vertice_inicial = Q[0];
    if(removeDeQ_Prim(Q, vertice_inicial, countQ))
        countQ--;
    S_Vertices[countS_V] = vertice_inicial;
    countS_V++;
    while(countQ > 0){
        menor_valor = INFINITO;
        for(int i = 0; i < countQ; i++){
            vertice = retornaVertice(grafo, Q[i]);
            Arcos = vertice->dado.arcos.inicio;
            for(int j = 0; j < vertice->dado.arcos.qtd; j++){
                for(int k = 0; k < countS_V; k++){
                    if(Arcos->dado.destino == S_Vertices[k]){
                        tmp = retornaValorAresta(vertice, Arcos->dado.destino);
                        if(tmp > 0 && tmp < menor_valor){
                            menor_valor = tmp;
                            menor_destino = Q[i];
                            menor_origem = S_Vertices[k];
                        }
                    }
                }
                Arcos = Arcos->prox;
            }
        }
        S[countS] = menor_origem + menor_destino;
        countS++;
        S_Vertices[countS_V] = menor_destino;
        countS_V++;
        if(removeDeQ_Prim(Q, menor_destino, countQ))
            countQ--;
    }
    for(int i = 0; i < countS; i++){
        cout << S[i];
        if(i < countS-1)
            cout << ", ";
    }
    cout << "\n";
    system("pause");
    menu(grafo);
}

void criaMatrizDesenho(Grafo g){
    string m[g.vertices.qtd][g.vertices.qtd];

    for (int i=0; i<g.vertices.qtd; i++){
        for (int j=0; j<g.vertices.qtd; j++){
            m[i][j] = '0';
        }
    }

    for (int i=0; i<g.vertices.qtd; i++){
        for (int j=0; j<g.vertices.qtd; j++){
            cout << m[i][j] << "\t";
        } cout << "\n";
    } cout << "\n";

    TElementoVertice *vet = g.vertices.inicio;
    for (int i=0; i<g.vertices.qtd; i++){
        for (int j=0; j<g.vertices.qtd; j++){
            if(i%2 == 0){
                if (j==0 || j == g.vertices.qtd-1){
                    if (vet != NULL){
                        m[i][j] = vet->dado.nome;
                        vet = vet->prox;
                    }
                }
            }
        }
    }


    for (int i=0; i<g.vertices.qtd; i++){
        for (int j=0; j<g.vertices.qtd; j++){
            cout << m[i][j] << "\t";
        } cout << "\n";
    }

}

void preenche_Q_Kruskal(Grafo &grafo, TListaArco &Q){
    TElementoArco *novo;
    TElementoVertice *vertice = grafo.vertices.inicio;
    TElementoArco *arco;
    TElementoArco *navQ;
    bool flag;

    for(int i = 0; i < grafo.vertices.qtd; i++){
        arco = vertice->dado.arcos.inicio;
        for(int j = 0; j < vertice->dado.arcos.qtd; j++){
            navQ = Q.inicio;
            flag = true;
            for(int k = 0; k < Q.qtd; k++){
                if((navQ->dado.destino == arco->dado.origem)&&(navQ->dado.origem == arco->dado.destino))
                    flag = false;
                navQ = navQ->prox;
            }
            if (flag){
                novo = new TElementoArco;
                novo->dado.origem = arco->dado.origem;
                novo->dado.destino = arco->dado.destino;
                novo->dado.valor = arco->dado.valor;
                novo->prox = NULL;
                novo->anterior = Q.fim;
                if(Q.qtd == 0)
                    Q.inicio = novo;
                else
                    Q.fim->prox = novo;
                Q.fim = novo;
                Q.qtd++;
                bubbleSortValor(Q);
            }
            arco = arco->prox;
        }
        vertice = vertice->prox;
    }
}

void removeDeQ_Kruskal(TListaArco &Q){
    TElementoArco *save = new TElementoArco;
    save = Q.inicio;
    Q.inicio = Q.inicio->prox;
    Q.qtd--;
    if(Q.qtd == 0)
        Q.fim = NULL;
    delete save;
}

void kruskal(Grafo &grafo){
    int qtd = grafo.vertices.qtd, x_origem, x_destino, countDestino, countS = 0;
    string F[qtd][qtd], S[grafo.Arestas], origem, destino;
    TListaArco Q;
    TElementoVertice *vertice = grafo.vertices.inicio;
    TElementoArco *navQ;

    inicializaArco(Q);
    preenche_Q_Kruskal(grafo, Q);
    navQ = Q.inicio;

    for(int i = 0; i < qtd; i++){
        for(int j = 0; j < qtd; j++){
            if(j == 0){
                F[i][j] = vertice->dado.nome;
                vertice = vertice->prox;
            }
            else
                F[i][j] = "v�zio";
        }
    }

    while(Q.qtd > 0){
        origem = Q.inicio->dado.origem;
        destino = Q.inicio->dado.destino;
        removeDeQ_Kruskal(Q);
        for(int i = 0; i < qtd; i++){
            for(int j = 0; j < qtd; j++){
                if(F[i][j] == origem){
                    x_origem = i;
                }
                if(F[i][j] == destino){
                    x_destino = i;
                }
            }
        }

        if(x_origem != x_destino){
            countDestino = 0;
            S[countS] = origem + "-" + destino;
            countS++;
            for(int i = 0; i < qtd; i++){
                if(F[x_origem][i] == "v�zio"){
                    if(F[x_destino][countDestino] != "v�zio"){
                        F[x_origem][i] = F[x_destino][countDestino];
                        F[x_destino][countDestino] = "v�zio";
                        countDestino++;
                    }
                }
            }
        }
    }

    cout << "F: ";
    for(int i = 0; i < qtd; i++){
        for(int j = 0; j < qtd; j++){
            cout << F[i][j] << ", ";
        }
        cout << "\n";
    }
    cout << "\nS: ";
    for(int i = 0; i < countS; i++)
        cout << S[i] << ", ";
    system("pause");
    menu(grafo);
}

void insereVertice(Grafo &grafo, string nome){
    TElementoVertice *novo = new TElementoVertice;
    if (novo == NULL)
        menu(grafo);
    novo->dado.nome = nome;
    inicializaArco(novo->dado.arcos);
    novo->prox = NULL;
    novo->ante = grafo.vertices.fim;
    novo->dado.visitado = false;
    novo->dado.grau = 0;
    novo->dado.cor = "null";
    if(grafo.vertices.qtd == 0)
        grafo.vertices.inicio = novo;
    else
        grafo.vertices.fim->prox = novo;
    grafo.vertices.fim = novo;
    grafo.vertices.qtd++;
    bubbleSortNomeVertice(grafo.vertices);
}

void preencheGrafoResidual(Grafo &grafo, Grafo &grafoResidual){
    TElementoVertice *nav = grafo.vertices.inicio;
    TElementoArco *navArco;
    for(int i = 0; i < grafo.vertices.qtd; i++){
        insereVertice(grafoResidual, nav->dado.nome);
        nav = nav->prox;
    }
    nav = grafo.vertices.inicio;
    for(int i = 0; i < grafo.vertices.qtd; i++){
        navArco = nav->dado.arcos.inicio;
        for(int j = 0; j < nav->dado.arcos.qtd; j++){
            insereAresta(grafoResidual, navArco->dado.origem, navArco->dado.destino, navArco->dado.valor);
            navArco = navArco->prox;
        }
        nav = nav->prox;
    }
}

void DFS_ida_e_volta(Grafo &grafo, TElementoVertice* origem, string destino, int &menor_valor, bool &achou){
    TElementoArco *navnav = origem->dado.arcos.inicio;
    TElementoArco *navAux;
    TElementoVertice *nav;
    int menor_valor_anterior = menor_valor;
    origem->dado.visitado = true;
    if(navnav != NULL){
        for(int i = 0; i < origem->dado.arcos.qtd && !achou; i++){
            nav = retornaVertice(grafo, navnav->dado.destino);
            if(menor_valor > navnav->dado.valor)
                menor_valor = navnav->dado.valor;
            if(nav->dado.nome != destino && !nav->dado.visitado){
                DFS_ida_e_volta(grafo, nav, destino, menor_valor, achou);
            }
            if(nav->dado.nome == destino)
                achou = true;
            if(!achou){
                menor_valor = menor_valor_anterior;
                navnav = navnav->prox;
            }
            if(achou){
                if(existeAresta(grafo, navnav->dado.destino, navnav->dado.origem)){
                    navAux = nav->dado.arcos.inicio;
                    for(int j = 0; j < nav->dado.arcos.qtd; j++){
                        if(navAux->dado.destino == navnav->dado.origem){
                            navAux->dado.valor += menor_valor;
                        }
                    }
                }else{
                    insereAresta(grafo, navnav->dado.destino, navnav->dado.origem, menor_valor);
                }
                navnav->dado.valor -= menor_valor;
                menor_valor_anterior = menor_valor;
                if(navnav->dado.valor == 0)
                    removeAresta(grafo, navnav->dado.origem, navnav->dado.destino);
                break;
            }
        }
    }
    menor_valor = menor_valor_anterior;
}

void FluxoMaximo(Grafo &grafo, string fonte, string sorvedor){
    int S = 0, menor_valor = INFINITO;
    bool achou = true;
    TElementoVertice *nav;
    Grafo *grafoResidual = new Grafo;

    grafoResidual->isOrientado = grafo.isOrientado;
    grafoResidual->isPonderado = grafo.isPonderado;

    inicializaVertice(grafoResidual->vertices);
    preencheGrafoResidual(grafo, *grafoResidual);

    nav = retornaVertice(*grafoResidual, fonte);

    while(achou){
        achou = false;
        resetaVerticesVisitados(*grafoResidual);
        DFS_ida_e_volta(*grafoResidual, nav, sorvedor, menor_valor, achou);
        if(achou)
            S += menor_valor;
    }
    system("cls");
    cout << "S: " << S;
    system("pause");
    menu(grafo);
}

void menu (Grafo &grafo){
    string origem, destino;
    int opcao = 0, valor;
    TElementoVertice *nav;
    TElementoArco *navnav;

    textcolor(BLACK);

    system("cls");

    cout << "1.\tInsere V�rtice\n"
         << "2.\tRemover V�rtice\n";
    if (!grafo.isOrientado){
        cout << "3.\tInserir Aresta\n"
             << "4.\tRemover Aresta\n";
    }
    else{
        cout << "3.\tInserir Arco\n"
             << "4.\tRemover Arco\n";
    }
    cout << "5.\tRetornar V�rtice\n"
         << "6.\tExiste Aresta\n"
         << "7.\tRetornar Arestas\n"
         << "8.\tImprimir Grafo\n"
         << "9.\tDijkstra\n"
         << "10.\tPlanaridade\n"
         << "11.\tPrim\n"
         << "12.\tKruskal\n"
         << "13.\tFluxo M�ximo\n"
         << "0.\tSair\n";
    cin >> opcao;
    cin.ignore();
    system("cls");
    switch (opcao){
        case 1:
            insereVertice(grafo);
            break;
        case 2:
            cout << "Informe o v�rtice que deseja remover\n";
            getline(cin, origem);
            removeVertice(grafo, origem);
            menu(grafo);
            break;
        case 3:
            cout << "Informe o nome do v�rtice de origem\n";
            getline(cin, origem);
            cout << "Informe o nome do v�rtice de destino\n";
            getline(cin, destino);
            if(grafo.isPonderado){
                cout << "Informe o peso do arco\n";
                cin >> valor;
                cin.ignore();
            }else
                valor = 1;
            if (!grafo.isOrientado)
                insereAresta(grafo, destino, origem, valor);
            insereAresta(grafo, origem, destino, valor);
            grafo.Arestas++;
            menu(grafo);
            break;
        case 4:
            cout << "Informe o nome do v�rtice de origem\n";
            getline(cin, origem);
            cout << "Informe o nome do v�rtice de destino\n";
            getline(cin, destino);
            if(!grafo.isOrientado)
                removeAresta(grafo, destino, origem);
            removeAresta(grafo, origem, destino);
            menu(grafo);
            break;
        case 5:
            cout << "Informe o nome do v�rtice de origem\n";
            getline(cin, origem);
            nav = retornaVertice(grafo, origem);
            if (nav == NULL){
                cout << "V�rtice n�o encontrado\n";
                system("pause");
                menu(grafo);
            }
            cout << nav->dado.nome;
            if (nav->dado.arcos.qtd > 0){
                navnav = nav->dado.arcos.inicio;
                for (int i = 0; i < nav->dado.arcos.qtd; i++){
                    cout << " | " << navnav->dado.destino << navnav->dado.valor;
                    navnav = navnav->prox;
                }
                cout << "\n";
            }
            system("pause");
            menu(grafo);
            break;
        case 6:
            cout << "Informe o v�rtice de origem\n";
            getline(cin, origem);
            cout << "Informe o v�rtice de destino\n";
            getline(cin, destino);
            if(!existeAresta(grafo, origem, destino))
                cout << "N�o ";
            cout << "Existe!\n";
            system("pause");
            menu(grafo);
            break;
        case 7:
            cout << "Informe o v�rtice de origem\n";
            getline(cin, origem);
            nav = retornaVertice(grafo, origem);
            cout << "DFS:\n";
            if(nav != NULL){
                DFS(grafo, nav);
            }
            nav = grafo.vertices.inicio;
            for(int i = 0; i < grafo.vertices.qtd; i++){
                if (!nav->dado.visitado){
                    DFS(grafo, nav);
                }
                nav = nav->prox;
            }
            resetaVerticesVisitados(grafo);
            cout << "\nBFS:\n";
            nav = retornaVertice(grafo, origem);
            if (nav != NULL){
                BFS(grafo, nav);
            }
            nav = grafo.vertices.inicio;
            for (int i = 0; i < grafo.vertices.qtd; i++){
                if (!nav->dado.visitado){
                    BFS(grafo, nav);
                }
                nav = nav->prox;
            }
            resetaVerticesVisitados(grafo);
            cout << "\n";
            system("Pause");
            menu(grafo);
            break;

        case 8:
            nav = grafo.vertices.inicio;
            ordenaParaColorir(grafo.vertices);
            for(int i = 0; i < grafo.vertices.qtd; i++){
                colore(grafo, nav->dado.nome);
                nav = nav->prox;
            }
            bubbleSortNomeVertice(grafo.vertices);
            nav = grafo.vertices.inicio;
            for (int i = 0; i < grafo.vertices.qtd; i++){
                if(nav->dado.cor == "vermelho"){
                    textcolor(RED);
                    cout << nav->dado.nome;
                }
                else if(nav->dado.cor == "verde"){
                    textcolor(GREEN);
                    cout << nav->dado.nome;
                }
                else if(nav->dado.cor == "amarelo"){
                    textcolor(YELLOW);
                    cout << nav->dado.nome;
                }
                else if(nav->dado.cor == "azul"){
                    textcolor(BLUE);
                    cout << nav->dado.nome;
                }
                else if(nav->dado.cor == "magenta"){
                    textcolor(MAGENTA);
                    cout << nav->dado.nome;
                }
                textcolor(BLACK);
                navnav = nav->dado.arcos.inicio;
                for (int j = 0; j < nav->dado.arcos.qtd; j++){
                    cout << " | " << navnav->dado.destino << navnav->dado.valor;
                    navnav = navnav->prox;
                }
                cout << "\n";
                nav = nav->prox;
            }
            cout << "\n";
            imprimeMatriz(grafo);
            system("pause");
            menu(grafo);
            break;

        case 9:
            system("cls");
            cout << endl << "Origem: " << endl;
            cin >> origem;
            cout << endl << "Destino: " << endl;
            cin >> destino;
            inicializaDijkstra(grafo, origem, destino);
            break;
        case 10:
            if(grafo.vertices.qtd <= 2)
                cout << "� planar\n";
            else if (TemCicloTres(grafo)){
                if (grafo.Arestas <= ((grafo.vertices.qtd * 3) - 6))
                    cout << "Pode ser planar\n";
                else
                    cout << "N�o � planar\n";
            }
            else{
                if(grafo.Arestas <= ((grafo.vertices.qtd * 2) - 4))
                    cout << "Pode ser planar\n";
                else
                    cout << "N�o � planar\n";
            }
            system("pause");
            menu(grafo);
            break;
        case 11:
            prim(grafo);
            break;
        case 12:
            kruskal(grafo);
            break;
        case 13:
            system("cls");
            cout << endl << "Fonte: " << endl;
            cin >> origem;
            cout << endl << "Sorvedouro: " << endl;
            cin >> destino;
            FluxoMaximo(grafo, origem, destino);
            break;
    }
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    Grafo *grafo = new Grafo;
    inicializaVertice(grafo->vertices);
    grafo->Arestas = 0;
    int aux;
    textbackground(LIGHTGRAY);
    system("cls");
    textcolor(BLACK);
    cout << "O grafo ser� orientado?\n"
         << "1. Sim\n"
         << "2. N�o\n";
    cin >> aux;
    if (aux == 2)
        grafo->isOrientado = false;
    else
        grafo->isOrientado = true;
    cout << "O grafo ser� ponderado?\n"
         << "1. Sim\n"
         << "2. N�o\n";
    cin >> aux;
    if (aux == 2)
        grafo->isPonderado = false;
    else
        grafo->isPonderado = true;
    menu(*grafo);
    return 0;
}
