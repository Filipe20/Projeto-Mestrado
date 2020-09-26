#include "../include/mapa.h"
#include <stdlib.h>     
#include <time.h>
#include <random> 
#include <algorithm>

using namespace std;
using namespace cv;

struct individuo
{
    vector<int> ind;
    vector<int> veiculos;
    vector<int> escola;
    vector<int> rota;
    int nc_v;
    void inserir(int g)
    {
        ind.push_back(g);
    }
    void inserir_v(int v)
    {
        veiculos.push_back(v);
    }
    void inserir_e(int e)
    {
        escola.push_back(e);
    }
    void calcularnc_v()
    {
        nc_v = ind.size() / veiculos.size();
    }
    void imprimir()
    {
        int aux = 0;
        for (int i = 0; i < ind.size(); i++)
        {
            if ((i % nc_v) == 0)
                cout << veiculos[aux++] << " ";
           cout << ind[i] << " "; 
        }
    }
    void montar_rota()
    {
        int aux = 0;
        for (int i = 0; i < ind.size(); i++)
        {
            if ((i % nc_v) == 0)
                rota.push_back(veiculos[aux++]);
           rota.push_back(ind[i]); 
        }
    }
};

class ga
{
private:
    mapa m;
    vector<individuo> populacao_auxiliar;
    vector<individuo> populacao;
    individuo elite;
    vector<double> fit;
    vector<vector<double>> objetivos;
    double elite_valor, pior_valor, media_valor;
    float fator_mutacao, fator_cruzamento;
    int tam_populacao, numero_geracoes, tam_torneio;
    int tamanho = 0;
    int n_cruzamento = 30;
    vector<int> pontos;
public:
    //-------------Processo
    ga(mapa m, float, float, int, int, int);
    void processo(ofstream&);
    individuo gerar_inicial();
    void gerar_populacao_inicial();
    //-------------Sele��o 
    void elitismo();
    void torneio();
    void fitness();
    void pior();
    void media();
    //-------------Operadores Gen�ticos
    void cruzamento();
    void cruzamento(int , int);
    void mutacao();
    void mutacao(int, double);
    //------------Visualiza��o 
    void imprimir_populacao();
    void imprimir_fitness();
    void imprimir_elite();
    void imprimir_objetivos();
    void imprimir_arquivos(ofstream&, int);
    void imprimir_escola();
    void plotar(Mat, float);
    //------------Objetivo
    void funcao_objetivo(int);
    void media(int);
    void desvio_padrao(int);
    
};