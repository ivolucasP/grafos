#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio2.h>

#define INFINITO 2147483647 //Número máximo que um integer consegue armazenar. Constante Simbólica.

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
    cout << "Informe o nome do vértice\n";
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

    //Obter Posição do Vertice Origem para iniciar
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

    //Obter Posição do Vertice de Origem para comparacao
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

            // Obtem posição do vertice correspondente a ligação da aresta para manipulá-lo
            navAux = g.vertices.inicio;

            while(navAux!= NULL){

                if(navAux->dado.nome == navArco->dado.destino){

                    aux = navAux;

                }
                navAux = navAux->prox;
            }

            // Se é infinito, é porque não foi explorado ainda.
            //Então recebe o valor da vertice em ligação + o valor da aresta.

            if (aux != NULL &&
                aux->dado.distancia == INFINITO){

                aux->dado.distancia = navVertice->dado.distancia + navArco->dado.valor;

                aux->dado.verticeAnterior = navVertice;

            // Se não é infinito, significa que já foi explorado alguma vez
            // Então verifica se o caminho atual (Vertice de ligação + valor da aresta)
            // é menor que o caminho registrado atualmente. Se for menor, recebe o novo caminho.
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

    //Obtem posição do vertice de destino
    navAux = g.vertices.inicio;
    while(navAux!= NULL){
        if(navAux->dado.nome == destino_p){
            navVertice = navAux;
        }
        navAux = navAux->prox;
    }

    vet[0] = navVertice->dado.nome;

    // Navega do destino à origem

    while (navVertice != NULL){


        static int i = 1;

        //Faz o caminho do destino até a origem. (Sim, de tras pra frente)
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


    //Imprime o vetor de tras pra frente, ou seja, da origem até o destino
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

void preenche_Q(Grafo &grafo, string *Q){
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

bool removeDeQ(string *Q, string dado, int qtd){
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
    preenche_Q(grafo, Q);
    vertice_inicial = Q[0];
    if(removeDeQ(Q, vertice_inicial, countQ))
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
                        cout << tmp << " " << S_Vertices[k] << " " << Q[i] << "\n";
                        if(tmp > 0 && tmp < menor_valor){
                            menor_valor = tmp;
                            menor_destino = Q[i];
                            menor_origem = S_Vertices[k];
                            cout << tmp << " " << menor_origem << "-" << menor_destino;
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
        if(removeDeQ(Q, menor_destino, countQ))
            countQ--;

        ///area de testes
        cout << "S: ";
        for(int i = 0; i < countS; i++){
            cout << S[i] << ", ";
        }
        cout << "\nQ: ";
        for(int i = 0; i < countQ; i++){
            cout << Q[i] << ", ";
        }
        cout << "\nS_V: ";
        for(int i = 0; i < countS_V; i++){
            cout << S_Vertices[i] << ", ";
        }
        system("pause");
        system("cls");
        ///area de testes
    }
    for(int i = 0; i < countS; i++){
        cout << S[i] << ", ";
    }
    cout << "\n";
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

    cout << "1.\tInsere Vértice\n"
         << "2.\tRemover Vértice\n";
    if (!grafo.isOrientado){
        cout << "3.\tInserir Aresta\n"
             << "4.\tRemover Aresta\n";
    }
    else{
        cout << "3.\tInserir Arco\n"
             << "4.\tRemover Arco\n";
    }
    cout << "5.\tRetornar Vértice\n"
         << "6.\tExiste Aresta\n"
         << "7.\tRetornar Arestas\n"
         << "8.\tImprimir Grafo\n"
         << "9.\tDijkstra\n"
         << "10.\tPlanaridade\n"
         << "11.\tPrim\n"
         << "0.\tSair\n";
    cin >> opcao;
    cin.ignore();
    system("cls");
    switch (opcao){
        case 1:
            insereVertice(grafo);
            break;
        case 2:
            cout << "Informe o vértice que deseja remover\n";
            getline(cin, origem);
            removeVertice(grafo, origem);
            menu(grafo);
            break;
        case 3:
            cout << "Informe o nome do vértice de origem\n";
            getline(cin, origem);
            cout << "Informe o nome do vértice de destino\n";
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
            cout << "Informe o nome do vértice de origem\n";
            getline(cin, origem);
            cout << "Informe o nome do vértice de destino\n";
            getline(cin, destino);
            if(!grafo.isOrientado)
                removeAresta(grafo, destino, origem);
            removeAresta(grafo, origem, destino);
            menu(grafo);
            break;
        case 5:
            cout << "Informe o nome do vértice de origem\n";
            getline(cin, origem);
            nav = retornaVertice(grafo, origem);
            if (nav == NULL){
                cout << "Vértice não encontrado\n";
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
            cout << "Informe o vértice de origem\n";
            getline(cin, origem);
            cout << "Informe o vértice de destino\n";
            getline(cin, destino);
            if(!existeAresta(grafo, origem, destino))
                cout << "Não ";
            cout << "Existe!\n";
            system("pause");
            menu(grafo);
            break;
        case 7:
            cout << "Informe o vértice de origem\n";
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
                cout << "É planar\n";
            else if (TemCicloTres(grafo)){
                if (grafo.Arestas <= ((grafo.vertices.qtd * 3) - 6))
                    cout << "É planar\n";
                else
                    cout << "Não é planar\n";
            }
            else{
                if(grafo.Arestas <= ((grafo.vertices.qtd * 2) - 4))
                    cout << "É planar\n";
                else
                    cout << "Não é planar\n";
            }
            system("pause");
            menu(grafo);
            break;
        case 11:
            prim(grafo);
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
    cout << "O grafo será orientado?\n"
         << "1. Sim\n"
         << "2. Não\n";
    cin >> aux;
    if (aux == 2)
        grafo->isOrientado = false;
    else
        grafo->isOrientado = true;
    cout << "O grafo será ponderado?\n"
         << "1. Sim\n"
         << "2. Não\n";
    cin >> aux;
    if (aux == 2)
        grafo->isPonderado = false;
    else
        grafo->isPonderado = true;
    menu(*grafo);
    return 0;
}
