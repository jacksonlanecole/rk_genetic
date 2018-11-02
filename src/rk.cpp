// rk.cpp
#include "rk.hpp"
namespace py = boost::python;

/* ------------------------------------------------------------------------- */
// Constructors, Destructors
/* ------------------------------------------------------------------------- */
RKIntegrator::RKIntegrator() {};

RKIntegrator::RKIntegrator(py::list& btToSet, double steps, double ti, double tf, double xi) {
	/* This is a constructor purely for testing the boost module */
	h = (tf - ti)/steps;
	x = xi;
	t = ti;
	bt.reset(btToSet);
	//initkVecTo0();

} // end constructor

RKIntegrator::RKIntegrator(function_type f, vec2D& btToSet,
		double _steps, double ti, double tf, double xi) {
	h = (tf - ti)/_steps;
	x = xi;
	t = ti;
	steps = _steps;
	func = f;
	bt.reset(btToSet);
	initkVecTo0();

} // end constructor

RKIntegrator::RKIntegrator(function_type f, py::list& btToSet,
		double _steps, double ti, double tf, double xi) {
	h = (tf - ti)/_steps;
	x = xi;
	t = ti;
	steps = _steps;
	func = f;
	bt.reset(btToSet);
	initkVecTo0();

} // end constructor

RKIntegrator::RKIntegrator(const RKIntegrator& rkArg) :
	h(rkArg.h), bt(rkArg.bt), t(rkArg.t), x(rkArg.x), stages(rkArg.stages),
	steps(rkArg.steps), func(rkArg.func) {
}

RKIntegrator::~RKIntegrator() {
} // end default destructor
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// H
/* ------------------------------------------------------------------------- */
void RKIntegrator::setTimeStep(double timeStep) {
	h = timeStep;
} // end setStages

double RKIntegrator::getTimeStep() {
	return h;
}
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Initializing ks as zero
/* ------------------------------------------------------------------------- */
void RKIntegrator::initkVecTo0() {
	stages = bt.getStages();
	kVec.resize(stages);
	for (int i = 0; i < stages; i++)
		kVec[i] = 0.;
} // end initKsTo0
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Step
/* ------------------------------------------------------------------------- */
double RKIntegrator::step(double& t, double& x) {
	double kSum;
	double kFin = 0.;
	int stages = bt.getStages();
	vDoub nodes = bt.getNodes();
	vec2D rkMat = bt.getrkMat();
	vDoub weights = bt.getWeights();

	initkVecTo0();

	for (int i = 0; i < stages; i++) {
		kSum = 0;
		for (int j = 0; j <= i; j++) {
			kSum += h*kVec[j]*rkMat[i][j];
		}

		kVec[i] = func(t + nodes[i]*h, x + kSum);
	}

	for (int i = 0; i < stages; i++) {
		kFin += kVec[i] * weights[i];
	}
	dxVec.push_back(kFin);

	x = x + h*(kFin);
	t = t + h;

	return x;
} // end RKIntegrator::step
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
// Run
/* ------------------------------------------------------------------------- */
double RKIntegrator::run() {
	for (int i = 0; i < steps; i++) {
		xVec.push_back(x);
		tVec.push_back(t);
		x = step(t, x);
	}
	return x;
}
/* ------------------------------------------------------------------------- */
