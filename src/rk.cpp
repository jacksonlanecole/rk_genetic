#include "rk.hpp"

/* ------------------------------------------------------------------------- */
// Constructors, Destructors
/* ------------------------------------------------------------------------- */
RKIntegrator::RKIntegrator() {
} // end default constructor
RKIntegrator::~RKIntegrator() {
} // end default destructor
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
// Standard RK Routine
/* ------------------------------------------------------------------------- */
void RKIntegrator::RK(double (*f)(double, double), ButcherTableau& btToSet,
		double steps, double ti, double tf, double xi) {
	double h   = (tf - ti)/steps;
	double x = xi;
	double t = ti;
	setButcherTableau(btToSet);
	setStages(bt.size() - 1);
	setNodes();
	setrkMat();
	setWeights();
	initkVecTo0();
	std::cout << "h = " << h << std::endl;

	for (int i = 0; i < steps; i++) {
		step(f, h, t, x, kVec);
	}

	std::cout << "t = " << t << "; x = " << x << std::endl;
} // end RK
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Butcher Tableau
/* ------------------------------------------------------------------------- */
void RKIntegrator::setButcherTableau(ButcherTableau& btToSet) {
	bt.resize(btToSet.size());
	bt = btToSet;
} // end setButcherTableau

ButcherTableau RKIntegrator::getButcherTableau() {
	return bt;
} // end getButcherTableau
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Stages
/* ------------------------------------------------------------------------- */
void RKIntegrator::setStages(int s) {
	stages = s;
} // end setStages

int RKIntegrator::getStages() {
	return stages;
}
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Step Coefficients
/* ------------------------------------------------------------------------- */
void RKIntegrator::setNodes() {
	nodes.resize(stages);
	for (int i=0; i < stages; i++) {
		nodes[i] = bt[i][0];
	}
} // end setStepCoeff

vDoub RKIntegrator::getNodes() {
	return nodes;
} // end getStepCoeff()
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// k Coefficients
/* ------------------------------------------------------------------------- */
void RKIntegrator::setrkMat() {
	for (int i = 0; i < stages; i++)
		rkMat.push_back(std::vector<double>(bt[i].begin() + 1, bt[i].end()));
} // end setStepCoeff


vec2D RKIntegrator::getrkMat() {
	return rkMat;
} // end getrkMat()
/* ------------------------------------------------------------------------- */



/* ------------------------------------------------------------------------- */
// Weights
/* ------------------------------------------------------------------------- */
void RKIntegrator::setWeights() {
	for (int i = 1; i <= stages; i++) {
		weights.push_back(bt.back()[i]);
	}
} // end setweights


vDoub RKIntegrator::getWeights() {
	return weights;
} // end getweights
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Initializing ks as zero
/* ------------------------------------------------------------------------- */
void RKIntegrator::initkVecTo0() {
	kVec.resize(stages);
	for (int i = 0; i < stages; i++)
		kVec[i] = 0.;
} // end initKsTo0
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Step
/* ------------------------------------------------------------------------- */
double RKIntegrator::step(
	double (*f)(double, double), double h, double& t, double& x, vDoub& kVec) {
	double kSum;
	double kFin = 0.;
	initkVecTo0();

	for (int i = 0; i < stages; i++) {
		kSum = 0;
		for (int j = 0; j <= i; j++) {
			kSum += h*kVec[j]*rkMat[i][j];
		}

		kVec[i] = f(t + nodes[i]*h, x + kSum);
	}

	for (int i = 0; i < stages; i++) {
		kFin += kVec[i] * weights[i];
	}

	x = x + h*(kFin);
	t = t + h;

	return x;
} // end RKIntegrator::step
/* ------------------------------------------------------------------------- */
