// tableau.cpp
#include "tableau.hpp"
#include <boost/python.hpp>

namespace py = boost::python;

/* ------------------------------------------------------------------------- */
// Constructors, destructors
/* ------------------------------------------------------------------------- */
Tableau::Tableau() {}; // default constructor

Tableau::Tableau(vec2D& tableauArg) {
	setTableau(tableauArg);
	setStages();
	setNodes();
	setrkMat();
	setWeights();
} // end constructor taking a 2D vector tableau

Tableau::Tableau(py::list& tableauArg) {
	setTableau(tableauArg);
	setStages();
	setNodes();
	setrkMat();
	setWeights();
} // end constructor taking a 2D vector tableau

Tableau::Tableau(const Tableau& tableauArg) :
	tableau_(tableauArg.tableau_) {
	reset(tableau_);
}

Tableau::~Tableau() {};

void Tableau::reset(vec2D& tableauArg) {
	setTableau(tableauArg);
	setStages();
	setNodes();
	setrkMat();
	setWeights();
} // end reset

void Tableau::reset(py::list& tableauArg) {
	setTableau(tableauArg);
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
	tableau_.resize(btToSet.size());
	tableau_ = btToSet;
} // end setTableau

void Tableau::setTableau(py::list& tableau) {
	int listLen = py::len(tableau);
	tableau_.resize(listLen);
	for (int i = 0; i < listLen; i++) {
		for (int j = 0; j < listLen; j++) {
			tableau_[i].push_back(py::extract<double>(tableau[i][j]));
		}
	}
} // end setTableau

vec2D Tableau::getTableau() {
	return tableau_;
} // end getTableau
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Stages
/* ------------------------------------------------------------------------- */
void Tableau::setStages() {
	stages_ = tableau_.size() - 1;
} // end setStages

int Tableau::getStages() {
	return stages_;
} // end getStages
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Nodes
/* ------------------------------------------------------------------------- */
void Tableau::setNodes() {
	nodes_.resize(stages_);
	for (int i=0; i < stages_; i++) {
		nodes_[i] = tableau_[i][0];
	}
} // end setNodes

vDoub Tableau::getNodes() {
	return nodes_;
} // end getNodes
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// k Coefficients
/* ------------------------------------------------------------------------- */
void Tableau::setrkMat() {
	for (int i = 0; i < stages_; i++)
		rk_mat_.push_back(std::vector<double>(tableau_[i].begin() + 1, tableau_[i].end()));
} // end setrkMat


vec2D Tableau::getrkMat() {
	return rk_mat_;
} // end getrkMat
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Weights
/* ------------------------------------------------------------------------- */
void Tableau::setWeights() {
	for (int i = 1; i <= stages_; i++) {
		weights_.push_back(tableau_.back()[i]);
	}
} // end setWeights

vDoub Tableau::getWeights() {
	return weights_;
} // end getWeights
/* ------------------------------------------------------------------------- */
