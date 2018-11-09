// rk.cpp
#include "rk.hpp"
#include <iostream>
#include "converters.hpp"
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

	usingVector = false;

} // end constructor

RKIntegrator::RKIntegrator(py::object& f, py::list& btToSet,
		double _steps, double ti, double _tf) {
	h = (_tf - ti)/_steps;
	t = ti;
	tf = _tf;
	steps = _steps;
	finished = false;
	func = f;
	bt.reset(btToSet);
	initkVecTo0();

	usingVector = true;

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
	x = step(x);
}


void RKIntegrator::stepAugWrap(double x) {
	x = step(x);
}

//void RKIntegrator::stepAugWrap(py::list& xList) {
//	x = step(t, xList);
//}


double RKIntegrator::step(double xVal) {
	double kSum;
	double kFin = 0.;
	int stages = bt.getStages();
	vDoub nodes = bt.getNodes();
	vec2D rkMat = bt.getrkMat();
	vDoub weights = bt.getWeights();

	xVec.push_back(xVal);
	tVec.push_back(t);

	initkVecTo0();

	for (int i = 0; i < stages; i++) {
		kSum = 0;
		for (int j = 0; j <= i; j++) {
			kSum += h*kVec[j]*rkMat[i][j];
		}

		kVec[i] = py::extract<double>(func(t + nodes[i]*h, xVal + kSum));
	}

	for (int i = 0; i < stages; i++) {
		kFin += kVec[i] * weights[i];
	}
	dxVec.push_back(kFin);

	xVal = xVal + h*(kFin);
	t = t + h;
	if (t >= tf)
		finished = true;

	return xVal;
} // end RKIntegrator::step

//std::vector< double > RKIntegrator::vecStep(py::list& pyxVec) {
py::list RKIntegrator::vecStep(py::list& pyxVec) {
	double kSum;
	double kFin = 0.;
	int stages = bt.getStages();
	std::vector< double > nodes = bt.getNodes();
	vec2D rkMat = bt.getrkMat();
	std::vector< double > weights = bt.getWeights();

	std::vector< double > xVec;

	xVec = converters::pyListTodVec(pyxVec);
	if (dxVec2D.size() == 0) {
		dxVec2D.resize(xVec.size());
	}
	xVec2D.push_back(xVec);
	tVec.push_back(t);
	initkVecTo0();

	for (int k = 0; k < xVec.size(); k++) {
		initkVecTo0();
		kFin = 0.;
		for (int i = 0; i < stages; i++) {
			kSum = 0;
			for (int j = 0; j <= i; j++) {
				kSum += h*kVec[j]*rkMat[i][j];
			}

			kVec[i] = py::extract<double>(func(t + nodes[i]*h, xVec[k]) + kSum);
		}

		for (int i = 0; i < stages; i++) {
			kFin += kVec[i] * weights[i];
		}
		dxVec2D[k].push_back(kFin);

		xVec[k] = xVec[k] + h*(kFin);
	}

	t = t + h;

	if (t >= tf)
		finished = true;

	return converters::vecToPyList(xVec);
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
		x = step(x);
	}
	return x;
}
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// get_vecs
/* ------------------------------------------------------------------------- */
//std::vector< double > RKIntegrator::get_xVec() {
//	return xVec;
//}
//
//std::vector< double > RKIntegrator::get_dxVec() {
////vDoub RKIntegrator::get_dxVec() {
//	return dxVec;
//}
//
////vDoub RKIntegrator::get_tVec() {
//std::vector< double > RKIntegrator::get_tVec() {
//	return tVec;
//}
//
//std::vector< std::vector< double > > RKIntegrator::get_xVec2D() {
//	return xVec2D;
//}
//
//std::vector< std::vector< double > > RKIntegrator::get_dxVec2D() {
////vDoub RKIntegrator::get_dxVec() {
//	return dxVec2D;
//}

py::list RKIntegrator::get_xVec() {
	return converters::vecToPyList(xVec);
}

py::list RKIntegrator::get_dxVec() {
//vDoub RKIntegrator::get_dxVec() {
	return converters::vecToPyList(dxVec);
}

//vDoub RKIntegrator::get_tVec() {
py::list RKIntegrator::get_tVec() {
	return converters::vecToPyList(tVec);
}

py::list RKIntegrator::get_xVec2D() {
	return converters::vec2DToPyList(xVec2D);
}

py::list RKIntegrator::get_dxVec2D() {
//vDoub RKIntegrator::get_dxVec() {
	return converters::vec2DToPyList(dxVec2D);
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


py::list RKIntegrator::get_lastVec(int choice) {
	switch (choice) {
		case 0 :
			return converters::vecToPyList(xVec2D.back());
		case 1 :
			return converters::vecToPyList(dxVec2D.back());
		case 2 :
			return converters::vecToPyList(tVec);
		default:
			return converters::vecToPyList(std::vector< double > {-500, -250, -100});
	}
}
/* ------------------------------------------------------------------------- */
