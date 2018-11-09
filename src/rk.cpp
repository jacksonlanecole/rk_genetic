// rk.cpp
#include "rk.hpp"
#include <iostream>
namespace py = boost::python;

/* ------------------------------------------------------------------------- */
// Constructors, Destructors
/* ------------------------------------------------------------------------- */
RKIntegrator::RKIntegrator() {};

RKIntegrator::RKIntegrator(py::object& f, py::list& btToSet,
		double _steps, double ti, double _tf, double xi) {
	h = (_tf - ti)/_steps;
	x = xi;
	t = ti;
	tf = _tf;
	steps = _steps;
	finished = false;
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
void RKIntegrator::stepWrap() {
	x = step(t, x);
}


void RKIntegrator::stepAugWrap(double x) {
	x = step(t, x);
}


double RKIntegrator::step(double& t, double& x) {
	double kSum;
	double kFin = 0.;
	int stages = bt.getStages();
	vDoub nodes = bt.getNodes();
	vec2D rkMat = bt.getrkMat();
	vDoub weights = bt.getWeights();

	xVec.push_back(x);
	tVec.push_back(t);

	initkVecTo0();

	for (int i = 0; i < stages; i++) {
		kSum = 0;
		for (int j = 0; j <= i; j++) {
			kSum += h*kVec[j]*rkMat[i][j];
		}

		kVec[i] = py::extract<double>(func(t + nodes[i]*h, x + kSum));
	}

	for (int i = 0; i < stages; i++) {
		kFin += kVec[i] * weights[i];
	}
	dxVec.push_back(kFin);

	x = x + h*(kFin);
	t = t + h;
	if (t >= tf)
		finished = true;

	return x;
} // end RKIntegrator::step
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
// isFinished
/* ------------------------------------------------------------------------- */
bool RKIntegrator::isFinished() {
	return finished;
}
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
	std::cout << "RESULT = " << x << std::endl;
	return x;
}
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// get_vecs
/* ------------------------------------------------------------------------- */
std::vector< double > RKIntegrator::get_xVec() {
	return xVec;
}

std::vector< double > RKIntegrator::get_dxVec() {
//vDoub RKIntegrator::get_dxVec() {
	return dxVec;
}

//vDoub RKIntegrator::get_tVec() {
std::vector< double > RKIntegrator::get_tVec() {
	return tVec;
}
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Get lasts
/* ------------------------------------------------------------------------- */
double RKIntegrator::get_last(int choice) {
	switch (choice) {
		case 0 :
			return xVec.back();
		case 1 :
			return dxVec.back();
		case 2 :
			return tVec.back();
		default:
			throw;
	}
}
/* ------------------------------------------------------------------------- */
