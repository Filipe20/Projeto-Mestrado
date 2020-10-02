#include "../include/mapa.h"
#include "../include/ga.h"

using namespace cv;
using namespace std;
int main(void)
{
	Mat img(1000, 1000, CV_8UC3, Scalar(255, 255, 255));
	ofstream out;
	out.open("/mnt/d/Documents/Mestrado/Projeto/R/teste2.csv");
	mapa G("../dados/pontos", "../dados/distancias");
	G.media_pontos();
	G.desvio_pontos();
	G.definir_escola();
	//ga(mapa m, float mut, float cru, int pop, int n_ger, int ntorneio)
	//for (size_t i = 0; i < 5; i++)
	{
		ga GA(G, .08, .8, 300, 1000, 6);
		//out << "geracao,individuo,d_veiculo1,d_veiculo2,d_veiculo3,d_veiculo4,d_total,media,desvio,elite,pior,med" << endl;
		out << "geracao,individuo,d_total,media,desvio,elite,pior,med" << endl;
		GA.processo(out);
		G.plotar(img, .45, false);
		GA.plotar(img, .45);
		GA.imprimir_elite();
	}
	imwrite("../imagens/Rotas.png", img);
	out.close();
	return (0);
}