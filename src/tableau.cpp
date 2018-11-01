// tableau.cpp
#include "tableau.hpp"

/* ------------------------------------------------------------------------- */
// Constructors, destructors
/* ------------------------------------------------------------------------- */
Tableau::Tableau() {}; // default constructor

Tableau::Tableau(vec2D& tableau) {
	setTableau(tableau);
	setStages();
	setNodes();
	setrkMat();
	setWeights();
} // end constructor taking a 2D vector tableau

Tableau::Tableau(const Tableau& tableauArg) :
	tableau(tableauArg.tableau) {
	reset(tableau);
}

Tableau::~Tableau() {};

void Tableau::reset(vec2D& tableau) {
	setTableau(tableau);
	setStages();
	setNodes();
	setrkMat();
	setWeights();
} // end reset
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Tableau
/* ------------------------------------------------------------------------- */
void Tableau::setTableau(vec2D& btToSet) {
	tableau.resize(btToSet.size());
	tableau = btToSet;
} // end setTableau

vec2D Tableau::getTableau() {
	return tableau;
} // end getTableau
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Stages
/* ------------------------------------------------------------------------- */
void Tableau::setStages() {
	stages = tableau.size() - 1;
} // end setStages

int Tableau::getStages() {
	return stages;
} // end getStages
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Nodes
/* ------------------------------------------------------------------------- */
void Tableau::setNodes() {
	nodes.resize(stages);
	for (int i=0; i < stages; i++) {
		nodes[i] = tableau[i][0];
	}
} // end setNodes

vDoub Tableau::getNodes() {
	return nodes;
} // end getNodes
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// k Coefficients
/* ------------------------------------------------------------------------- */
void Tableau::setrkMat() {
	for (int i = 0; i < stages; i++)
		rkMat.push_back(std::vector<double>(tableau[i].begin() + 1, tableau[i].end()));
} // end setrkMat


vec2D Tableau::getrkMat() {
	return rkMat;
} // end getrkMat
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Weights
/* ------------------------------------------------------------------------- */
void Tableau::setWeights() {
	for (int i = 1; i <= stages; i++) {
		weights.push_back(tableau.back()[i]);
	}
} // end setWeights

vDoub Tableau::getWeights() {
	return weights;
} // end getWeights
/* ------------------------------------------------------------------------- */
