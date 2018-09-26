#include <iostream>
#include <iomanip>
#include <string>
#include <math.h>
#include "rk.hpp"

using namespace std;

void PrintTest(ButcherTableau, vec2D, vDoub, vDoub);
void PrintrkMat(vec2D&);
void PrintButcherTableau(ButcherTableau&);
void PrintWeights(vDoub&);
void PrintSection(string);
void PrintNodes(vDoub&);
double testODE(double, double);

int main() {
	ButcherTableau bt {{   0.,   0.,   0.,   0.,   0.},
	                   { 1/2., 1/2.,   0.,   0.,   0.},
	                   { 1/2.,   0., 1/2.,   0.,   0.},
	                   {   1.,   0.,   0.,   1.,   0.},
	                   {   0., 1/6., 1/3., 1/3., 1/6.}};

	RKIntegrator RK4;
	RK4.RK(testODE, bt, 10000, 0, 2, 1);

	ButcherTableau new_bt = RK4.getButcherTableau();
	vDoub weights = RK4.getWeights();
	vec2D rkMat = RK4.getrkMat();
	vDoub nodes = RK4.getNodes();

	PrintTest(new_bt, rkMat, weights, nodes);

	return 0;

} // end main

double testODE(double t, double x) {
	return x/(1 + t);
} // end testODE

void PrintTest(ButcherTableau bt, vec2D rkMat, vDoub weights, vDoub nodes) {
	PrintSection("Butcher Tableau");
	PrintButcherTableau(bt);
	cout << endl;

	PrintSection("Runge-Kutta Matrix");
	PrintrkMat(rkMat);
	cout << endl;

	PrintSection("Weights");
	PrintWeights(weights);
	cout << endl;

	PrintSection("Nodes");
	PrintNodes(nodes);
	cout << endl;
}

void PrintrkMat(vec2D& rkMat) {
	int i, j;
	int size = rkMat[0].size();
	int width = 10;
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			cout << setw(width) << right;
			cout << rkMat[i][j];
		}
		cout << endl;
	}
} // end PrintrkMat

void PrintButcherTableau(ButcherTableau& bt) {
	int i, j;
	int size = bt[0].size();
	int width = 10;
	for (i = 0; i < size; i++) {
		if (i == size - 1)
			cout << string(width*size + 4, '-') << endl;

		for (j = 0; j < size; j++) {
			cout << setw(width) << right;
			if (j == 0)
				cout << bt[i][j] << " | ";
			else
				cout << bt[i][j];
		}
		cout << endl;
	}
} // end PrintButcherTableau

void PrintNodes(vDoub& nodes) {
	for (int i = 0; i < nodes.size(); i++)
		cout << "c" << i + 1 << ": " << nodes[i] << endl;
} // end PrintkWeights

void PrintWeights(vDoub& weights) {
	for (int i = 0; i < weights.size(); i++)
		cout << "k" << i + 1 << ": " << weights[i] << endl;
} // end PrintkWeights

void PrintSection(string name) {
	string DIVIDER = string(80, '*');
	cout << DIVIDER << endl;
	cout << name << endl;
	cout << DIVIDER << endl;
} // end PrintSection
