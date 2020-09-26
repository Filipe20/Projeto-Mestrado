#include "../include/ga.h"

using namespace std;
using namespace cv;

ga::ga(mapa m, float mut, float cru, int pop, int n_ger, int ntorneio)
{
	this->m = m;
	fator_cruzamento = cru;
	fator_mutacao = mut;
	tam_populacao = pop;
	numero_geracoes = n_ger;
	tam_torneio = ntorneio;
	int tam = 40;
	srand(time(NULL));
	for (size_t i = 0; i < 3; i++)
	{
		pontos.push_back(rand() % tam * .85 + tam * .15);
		//pontos.push_back(tam / 2);
	}
	sort(pontos.begin(), pontos.end());
}
void ga::processo(ofstream &out)
{
	int ger = 0;
	srand(time(NULL));
	gerar_populacao_inicial();
	//imprimir_populacao();
	while (ger++ < numero_geracoes)
	{
		fitness();
		elitismo();
		pior();
		media();
		torneio();
		fitness();
		cruzamento();
		mutacao();
		//imprimir_objetivos();
		//imprimir_elite();
		imprimir_arquivos(out, ger);
		//cout << endl;
	}

}
individuo ga::gerar_inicial()
{
	//srand(time(NULL));
	random_device rd;
	mt19937 mt19(rd());
	individuo tmp;
	int nc_max, aux, x, y, nc_restante;
	vector<int> pont;
	int nm = m.get_nmotor();
	int ne = m.get_nescola();
	nc_max = m.get_ncrianca();
	for (int i = 0; i < nc_max; i++)
		pont.push_back(i);
	nc_max = 40;
	nc_restante = nc_max;
	for (int i = 0; i < nc_max; i++)
	{
		uniform_int_distribution<int> r(0, nc_restante - 1);
		x = r(mt19);
		y = pont[x];
		pont.erase(pont.begin() + x);
		nc_restante--;
		tmp.inserir(y);
	}
	for (int i = 0; i < nm; i++)
		tmp.inserir_v(nc_max + i);
	for (int i = 0; i < ne; i++)
		tmp.inserir_e(nc_max + nm + i);
	tmp.calcularnc_v();
	return tmp;
}
void ga::gerar_populacao_inicial()
{
	for (int i = 0; i < tam_populacao; i++)
	{
		individuo in = gerar_inicial(); // gera individuo aleatório
		populacao.push_back(in); // População inicial aleatório 
	}
}
void ga::fitness()
{
	fit.clear();
	objetivos.clear();
	for (int i = 0; i < populacao.size(); i++)
	{
		funcao_objetivo(i);
		fit.push_back(objetivos[i][populacao[i].veiculos.size()]);
	}
}
void ga::elitismo()
{
	elite_valor = fit[0];
	for (int i = 1; i < populacao.size(); i++)
	{
		if (elite_valor > fit[i])
		{
			elite_valor = fit[i];
			elite = populacao[i];
		}
	}
}
void ga::pior()
{
	pior_valor = fit[0];
	for (int i = 1; i < populacao.size(); i++)
	{
		if (pior_valor < fit[i])
		{
			pior_valor = fit[i];
		}
	}
}
void ga::media()
{
	media_valor = 0;
	for (int i = 0; i < populacao.size(); i++)
	{
		media_valor += fit[i];
	}
	media_valor /= fit.size();
}
void ga::torneio()
{
	int a, b;
	double aux;
	vector<individuo> selec;
	for (size_t i = 0; i < tam_populacao; i++)
	{
		a = rand() % tam_populacao;
		b = a;
		aux = fit[a];
		for (size_t j = 1; j < tam_torneio; j++)
		{
			a = rand() % tam_populacao;
			if (aux > fit[a])
			{
				aux = fit[a];
				b = a;
			}
		}
		selec.push_back(populacao[b]);
	}
	populacao = selec;
}
void ga::cruzamento()
{
    double taxa;
	for (size_t i = 0; i < populacao.size(); i = i+2)
	{
		taxa = (rand() % 100) / 100;
		if (taxa < fator_cruzamento)
		{
			cruzamento(i, i + 1);
		}
	}
}
void ga::cruzamento(int x, int y)
{
	vector<bool> pontos;
	individuo filho1, filho2;
	bool aux = true;
	int a = 0, b = 0;
	for (size_t i = 0; i < populacao[x].ind.size(); i++)
	{
		pontos.push_back(false);
		filho1.inserir(0);
		filho2.inserir(0);
	}
	for (size_t i = 0; i < n_cruzamento; i++)
	{
		pontos[rand() % populacao[x].ind.size()] = true;
	}
	for (size_t i = 0; i < populacao[x].ind.size(); i++)
	{
		filho1.ind[i] = (populacao[x].ind[i] * pontos[i]);
		filho2.ind[i] = (populacao[y].ind[i] * pontos[i]);
	}
	for (size_t i = 0; i < populacao[x].ind.size(); i++)
	{
		if (pontos[i] == 0)
		{
			for (size_t j = a; j < populacao[y].ind.size(); j++)
			{
				for (size_t k = 0; k < filho1.ind.size(); k++)
				{
					if (populacao[y].ind[j] == filho1.ind[k])
					{
						aux = false;
						k = filho1.ind.size();
					}
					else
					{
						aux = true;
						a = j;
					}
				}
				if (aux)
				{
					j = populacao[y].ind.size();
				}
			}
			if (aux)
			{
				filho1.ind[i] = populacao[y].ind[a];
			}
		}
	}
	aux = true;
	for (size_t i = 0; i < populacao[y].ind.size(); i++)
	{
		if (pontos[i] == 0)
		{
			for (size_t j = b; j < populacao[x].ind.size(); j++)
			{
				for (size_t k = 0; k < filho2.ind.size(); k++)
				{
					if (populacao[x].ind[j] == filho2.ind[k])
					{
						aux = false;
						k = filho2.ind.size();
					}
					else
					{
						aux = true;
						b = j;
					}
				}
				if (aux)
				{
					j = populacao[x].ind.size();
				}
			}
			if (aux)
			{
				filho2.ind[i] = populacao[x].ind[b];
			}
		}
	}
	for (size_t i = 0; i < populacao[x].ind.size(); i++)
	{
		populacao[x].ind[i] = filho1.ind[i];
		populacao[y].ind[i] = filho2.ind[i];
	}
}
void ga::mutacao()
{
	double taxa;
	taxa = (rand() % 100) / 100;
	for (size_t i = 0; i < populacao.size(); i = i + 2)
	{
		mutacao(i, taxa);
	}
}
void ga::mutacao(int x, double taxa)
{
	int aux, par;
	for (size_t i = 0; i < populacao[x].ind.size(); i++)
	{
		if (taxa < fator_mutacao)
		{
			par = rand() % populacao[x].ind.size();
			aux = populacao[x].ind[i];
			populacao[x].ind[i] = populacao[x].ind[par];
			populacao[x].ind[par] = aux;
		}
	}
}
//---------------------------Funções de visualização-----------------------
void ga::imprimir_populacao()
{
	for (size_t i = 0; i < tam_populacao; i++)
	{
		populacao[i].imprimir();
		cout << endl;
	}
}
void ga::imprimir_fitness()
{
	for (size_t i = 0; i < tam_populacao; i++)
	{
		cout << fit[i];
		cout << endl;
	}
}
void ga::imprimir_elite()
{
	//elite.imprimir();
	cout << elite_valor << endl;
}
void ga::imprimir_objetivos()
{
	for (size_t i = 0; i < populacao.size(); i++)
	{
		for (size_t j = 0; j < objetivos[i].size(); j++)
		{
			cout << objetivos[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
void ga::imprimir_arquivos(ofstream &out, int ger)
{
	for (size_t i = 0; i < populacao.size(); i++)
	{
		out << ger <<"," << i;
		for (size_t j = 0; j < objetivos[i].size(); j++)
		{
			out <<","<<objetivos[i][j];
		}
		out << "," << elite_valor;
		out << "," << pior_valor;
		out << "," << media_valor;
		out << endl;
	}
}

//------------------------------------//-----------------------------------------
//------------------------------Funções Objetivo---------------------------------
void ga::funcao_objetivo(int i)
{
	double dist = 0;
	double total = 0;
	double variancia = 0;
	double media = 0;
	int n = 0;
	vector<double> objetivo_aux;
	for (int j = 0; j < populacao[i].ind.size() - 1; j++)
	{
		int aux = 0;
		if ((j % populacao[i].nc_v) == 0)
		{
		   if(j!=0)
		   objetivo_aux.push_back(dist);
		   total += dist;
		   dist = m.get_distancia(populacao[i].veiculos[aux++], populacao[i].ind[j]);
		}
		dist += m.get_distancia(populacao[i].ind[j], populacao[i].ind[j + 1]);
	}
	total += dist;
	objetivo_aux.push_back(dist);
	objetivo_aux.push_back(total);
	media = total / populacao[i].veiculos.size();
	objetivo_aux.push_back(media);
	for (size_t j = 0; j < populacao[i].veiculos.size(); j++)
	{
		variancia += pow((objetivo_aux[j] - media), 2);
	}
	objetivo_aux.push_back(sqrt(variancia));
	objetivos.push_back(objetivo_aux);
}
void ga::plotar(Mat img, float a)
{
	elite.montar_rota();
    m.plotar(elite.rota, img, a);
}