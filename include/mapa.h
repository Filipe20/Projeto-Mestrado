#pragma once
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;
using namespace cv;

class mapa
{
private:
    vector<pair<double, double>> pontos;
    int n_crianca, n_motor, n_escola, n_pontos=-1;
    pair<double,double> media_pts;
    pair<double,double> desvio_pts;
    vector<vector<double>> distancias;
    vector<vector<int>> adj;
    vector<int> crianca_escola;


public:
    mapa() {}
    mapa(string, string);
    void imprimir();
    void media_pontos();
    void desvio_pontos();
    void requisicao(ofstream &os);
    double get_distancia(int, int);
    void plotar(Mat img, float, bool);
    void mat_adj();
    void definir_escola();
    int get_ncrianca();
    int get_nmotor();
    int get_nescola();
    int get_escola(int);
    void plotar(vector<int> ind, Mat img, float a);
};