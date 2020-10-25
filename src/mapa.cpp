#include "../include/mapa.h"
mapa::mapa(string pts, string dist)
{
    int i = 0;
    double x, y, d;
    ifstream file(pts);
    ifstream file2(dist);
    if (!file)
    {
        cout << "erro1" << endl;
        return;
    }
    if (!file2)
    {
        cout << "erro2" << endl;
        return;
    }
    while (true)
    {
        if (i == 0)
        {
            file >> n_crianca;
            file >> n_escola;
            file >> n_motor;
            n_pontos = n_escola + n_crianca + n_motor;
        }
        else
        {
            file >> x;
            file >> y;
            pontos.push_back(make_pair(x, y));
        }
        i++;
        if (i > n_pontos)
            break;
    }
    for (int j = 0; j < n_pontos; j++)
    {
        vector<double> aux;
        for (int k = 0; k < n_pontos; k++)
        {
            file2 >> d;
            aux.push_back(d);
        }
        distancias.push_back(aux);
    }
}
void mapa::imprimir()
{
    cout << media_pts.first << " " << media_pts.second << endl;
    cout << n_crianca << ", " << n_motor << ", " << n_escola << ", " << n_pontos << endl;
    for (int i = 0; i < n_pontos; i++)
        cout << pontos[i].first << " " << pontos[i].second << endl;

    for (int j = 0; j < n_pontos; j++)
    {
        for (int k = 0; k < n_pontos; k++)
        {
            cout << distancias[j][k] << " ";
        }
        cout << endl;
    }
}
void mapa::media_pontos()
{
    double media_x = 0, media_y = 0;
    for (int i = 0; i < n_pontos; i++)
    {
        media_x += pontos[i].first;
        media_y += pontos[i].second;
    }
    media_pts = make_pair(abs(media_x) / n_pontos, abs(media_y) / n_pontos);
}
void mapa::desvio_pontos()
{
    double desvio_x = 0, desvio_y = 0;
    for (int i = 0; i < n_pontos; i++)
    {
        desvio_x += pow(media_pts.first - abs(pontos[i].first), 2);
        desvio_y += pow(media_pts.second - abs(pontos[i].second), 2);
    }
    desvio_pts = make_pair(desvio_x / n_pontos, desvio_y / n_pontos);
}
void mapa::requisicao(ofstream &os)
{
    os << "{\"location\":[";
    for (int i = 0; i < n_pontos; i++)
        os << "[" << pontos[i].first << "," << pontos[i].second << "],";
    os << "],\"metrics\":[\"distance\"],\"units\":\"km\"}" << endl;
}
double mapa::get_distancia(int v1, int v2)
{
    return distancias[v1][v2];
}
void mapa::mat_adj()
{
    for (int j = 0; j < n_pontos; j++)
    {
        vector<int> aux;
        for (int k = 0; k < n_pontos; k++)
        {
            if (distancias[j][k] > 0)
                aux.push_back(1);
            else
                aux.push_back(0);
        }
        adj.push_back(aux);
    }
}
void mapa::definir_escola()
{
    for (int i = 0; i < n_crianca; i++)
    {
        crianca_escola.push_back(rand() % n_escola + n_crianca + n_motor);
        //cout << crianca_escola[i] << " ";
    }
}
int mapa::get_ncrianca()
{
    return n_crianca;
}
int mapa::get_nmotor()
{
    return n_motor;
}
int mapa::get_nescola()
{
    return n_escola;
}
int mapa::get_escola(int x)
{
    return crianca_escola[x];
}
/*
void mapa::plotar(vector<int> ind, Mat img, float a)
{
    int b = 0, g = 0, r = 0, aux = 0;
        for (int i = 0; i < ind.size() - 1; i++)
        {
            line(img, Point(500 + (abs(pontos[ind[i]].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[ind[i]].second) - media_pts.second) / (a * desvio_pts.second)),
                 Point(500 + (abs(pontos[ind[i+1]].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[ind[i + 1]].second) - media_pts.second) / (a * desvio_pts.second)),
                 Scalar(b, g, r));
            if(ind[i+2] >= n_crianca)
            {
                i++;
                aux++;
                if (aux == 1)
                {b = 255; g = 0; r = 0;}
                if (aux == 2)
                {b = 0; g = 255; r = 0;}
                if (aux == 3)
                {b = 0; g = 0; r = 255;}
            }    
        }
}
void mapa::plotar(Mat img, float a, bool b)
{
    for (int i = 0; i < n_crianca + n_motor; i++)
    {
        if (i < n_crianca)
        {
            switch (crianca_escola[i])
            {
            case 44:
                circle(img, Point(500 + (abs(pontos[i].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[i].second) - media_pts.second) / (a * desvio_pts.second)), 5, Scalar( 0, 0, 0), CV_FILLED, 8, 0);
                break;
            case 45:
                circle(img, Point(500 + (abs(pontos[i].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[i].second) - media_pts.second) / (a * desvio_pts.second)), 5, Scalar(0, 0, 0), CV_FILLED, 8, 0);
                break;
            case 46:
                circle(img, Point(500 + (abs(pontos[i].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[i].second) - media_pts.second) / (a * desvio_pts.second)), 5, Scalar(0, 0, 0), CV_FILLED, 8, 0);
                break;
            case 47:
                circle(img, Point(500 + (abs(pontos[i].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[i].second) - media_pts.second) / (a * desvio_pts.second)), 5, Scalar(0, 0, 0), CV_FILLED, 8, 0);
                break;
            case 48:
                circle(img, Point(500 + (abs(pontos[i].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[i].second) - media_pts.second) / (a * desvio_pts.second)), 5, Scalar(0, 0, 0), CV_FILLED, 8, 0);
                break;
            default:
                break;
            }
        }
        else
            circle(img, Point(500 + (abs(pontos[i].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[i].second) - media_pts.second) / (a * desvio_pts.second)), 8, Scalar(0, 0, 255), CV_FILLED, 8, 0);
    }
}
*/
void mapa::plotar(vector<int> ind, Mat img, float a)
{
    int b = 0, g = 0, r = 0, aux = 0;
        for (int i = 0; i < ind.size() - 1; i++)
        {
            line(img, Point(500 + (abs(pontos[ind[i]].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[ind[i]].second) - media_pts.second) / (a * desvio_pts.second)),
                 Point(500 + (abs(pontos[ind[i+1]].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[ind[i + 1]].second) - media_pts.second) / (a * desvio_pts.second)),
                 Scalar(b, g, r));
            if(ind[i+2] >= n_crianca)
            {
                i++;
                aux++;
                if (aux == 1)
                {b = 255; g = 0; r = 0;}
                if (aux == 2)
                {b = 0; g = 255; r = 0;}
                if (aux == 3)
                {b = 0; g = 0; r = 255;}
            }    
        }
}
void mapa::plotar(Mat img, float a, bool b)
{
    for (int i = 0; i < n_crianca + n_motor; i++)
    {
        if (i < n_crianca)
        {
            switch (crianca_escola[i])
            {
            case 48:
                circle(img, Point(500 + (abs(pontos[i].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[i].second) - media_pts.second) / (a * desvio_pts.second)), 5, Scalar( 0, 0, 0), CV_FILLED, 8, 0);
                break;
            case 49:
                circle(img, Point(500 + (abs(pontos[i].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[i].second) - media_pts.second) / (a * desvio_pts.second)), 5, Scalar(0, 0, 0), CV_FILLED, 8, 0);
                break;
            case 50:
                circle(img, Point(500 + (abs(pontos[i].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[i].second) - media_pts.second) / (a * desvio_pts.second)), 5, Scalar(0, 0, 0), CV_FILLED, 8, 0);
                break;
            default:
                break;
            }
        }
        else
            circle(img, Point(500 + (abs(pontos[i].first) - media_pts.first) / (a * desvio_pts.first), 500 + (abs(pontos[i].second) - media_pts.second) / (a * desvio_pts.second)), 8, Scalar(0, 0, 255), CV_FILLED, 8, 0);
    }
}