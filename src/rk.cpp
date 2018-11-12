// rk.cpp
#include "rk.hpp"
#include <iostream>
#include "converters.hpp"
namespace py = boost::python;

/* ------------------------------------------------------------------------- */
// Constructors, Destructors
/* ------------------------------------------------------------------------- */
RKIntegrator::RKIntegrator() {};

RKIntegrator::RKIntegrator(py::object& f, py::list& btToSet, double steps,
	double ti, double tf, double xi, bool explicit_t_dependence) {
	h_ = (tf - ti)/steps;
	x_ = xi;
	t_ = ti;
	tf_ = tf;
	steps_ = steps;
	finished_ = false;
	explicit_t_dependence_ = explicit_t_dependence;
	func_ = f;
	bt_.reset(btToSet);
	initkVecTo0();

	using_vector_ = false;

} // end constructor

RKIntegrator::RKIntegrator(py::object& f, py::list& btToSet,
		double steps, double ti, double tf, bool explicit_t_dependence) {
	h_ = (tf - ti)/steps;
	t_ = ti;
	tf_ = tf;
	steps_ = steps;
	finished_ = false;
	explicit_t_dependence_ = explicit_t_dependence;
	func_ = f;
	bt_.reset(btToSet);
	initkVecTo0();

	using_vector_ = true;

} // end constructor

RKIntegrator::RKIntegrator(const RKIntegrator& rkArg) :
	h_(rkArg.h_), bt_(rkArg.bt_), t_(rkArg.t_), x_(rkArg.x_), stages_(rkArg.stages_),
	steps_(rkArg.steps_), func_(rkArg.func_) {
}

RKIntegrator::~RKIntegrator() {
} // end default destructor
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// H
/* ------------------------------------------------------------------------- */
void RKIntegrator::setTimeStep(double timeStep) {
	h_ = timeStep;
} // end setStages

double RKIntegrator::getTimeStep() {
	return h_;
}
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Initializing ks as zero
/* ------------------------------------------------------------------------- */
void RKIntegrator::initkVecTo0() {
	stages_ = bt_.getStages();
	k_vec_.resize(stages_);
	for (int i = 0; i < stages_; i++)
		k_vec_[i] = 0.;
} // end initKsTo0
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Step
/* ------------------------------------------------------------------------- */
void RKIntegrator::stepWrap() {
	x_ = step(x_);
}


void RKIntegrator::stepAugWrap(double x) {
	x_ = step(x);
}

//void RKIntegrator::stepAugWrap(py::list& xList) {
//	x = step(t, xList);
//}


double RKIntegrator::step(double xVal) {
	double kSum;
	double kFin = 0.;
	//int stages_ = bt_.getStages();
	vDoub nodes = bt_.getNodes();
	vec2D rkMat = bt_.getrkMat();
	vDoub weights = bt_.getWeights();

	x_vec_.push_back(xVal);
	t_vec_.push_back(t_);

	initkVecTo0();

	for (int i = 0; i < stages_; i++) {
		kSum = 0;
		for (int j = 0; j <= i; j++) {
			kSum += h_*k_vec_[j]*rkMat[i][j];
		}

		if (explicit_t_dependence_) {
			k_vec_[i] = py::extract<double>(func_(t_ + nodes[i]*h_, xVal + kSum));
		}
		else {
			k_vec_[i] = py::extract<double>(func_(xVal + kSum));
		}
	}

	for (int i = 0; i < stages_; i++) {
		kFin += k_vec_[i] * weights[i];
	}
	dx_vec_.push_back(kFin);

	xVal = xVal + h_*(kFin);
	t_ = t_ + h_;
	if (t_ >= tf_)
		finished_ = true;

	return xVal;
} // end RKIntegrator::step

//std::vector< double > RKIntegrator::vecStep(py::list& pyxVec) {
py::list RKIntegrator::vecStep(py::list& pyxVec) {
	double kSum;
	double kFin = 0.;
	int stages = bt_.getStages();
	std::vector< double > nodes = bt_.getNodes();
	vec2D rkMat = bt_.getrkMat();
	std::vector< double > weights = bt_.getWeights();

	std::vector< double > xVec;

	xVec = converters::pyListTodVec(pyxVec);
	if (dxVec2D_.size() == 0) {
		dxVec2D_.resize(xVec.size());
	}
	xVec2D_.push_back(xVec);
	t_vec_.push_back(t_);
	initkVecTo0();

	for (int k = 0; k < xVec.size(); k++) {
		initkVecTo0();
		kFin = 0.;
		for (int i = 0; i < stages; i++) {
			kSum = 0;
			for (int j = 0; j <= i; j++) {
				kSum += h_*k_vec_[j]*rkMat[i][j];
			}

			if (explicit_t_dependence_) {
				k_vec_[i] = py::extract<double>(func_(t_ + nodes[i]*h_, xVec[k] + kSum, k));
			}
			else {
				k_vec_[i] = py::extract<double>(func_(xVec[k] + kSum, k));
			}
		}

		for (int i = 0; i < stages; i++) {
			kFin += k_vec_[i] * weights[i];
		}
		dxVec2D_[k].push_back(kFin);

		xVec[k] = xVec[k] + h_*(kFin);
	}

	t_ = t_ + h_;

	if (t_ >= tf_)
		finished_ = true;

	return converters::vecToPyList(xVec);
} // end RKIntegrator::step
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
// isFinished
/* ------------------------------------------------------------------------- */
bool RKIntegrator::isFinished() {
	/* Returns the finished_ boolean indicating that the integration has completed
	 * base on the upper bound on time entered.
	 */
	return finished_;
}
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
// Run
/* ------------------------------------------------------------------------- */
double RKIntegrator::run() {
	/* run does the full integration under the assumption that the variables do
	 * not need to be modified.
	 */
	for (int i = 0; i < steps_; i++) {
		x_vec_.push_back(x_);
		t_vec_.push_back(t_);
		x_ = step(x_);
	}
	return x_;
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
	return converters::vecToPyList(x_vec_);
}

py::list RKIntegrator::get_dxVec() {
//vDoub RKIntegrator::get_dxVec() {
	return converters::vecToPyList(dx_vec_);
}

//vDoub RKIntegrator::get_tVec() {
py::list RKIntegrator::get_tVec() {
	return converters::vecToPyList(t_vec_);
}

py::list RKIntegrator::get_xVec2D() {
	return converters::vec2DToPyList(xVec2D_);
}

py::list RKIntegrator::get_dxVec2D() {
//vDoub RKIntegrator::get_dxVec() {
	return converters::vec2DToPyList(dxVec2D_);
}

/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
// Get lasts
/* ------------------------------------------------------------------------- */
double RKIntegrator::get_last(int choice) {
	/* Returns either the last value of either the independent vector,
	 * dependent vector, or the vector of derivatives
	 */
	switch (choice) {
		case 0 :
			return x_vec_.back();
		case 1 :
			return dx_vec_.back();
		case 2 :
			return t_vec_.back();
		default:
			throw;
	}
}


py::list RKIntegrator::get_lastVec(int choice) {
	/* Returns either the independent vector, dependent vector, or the
	 * vector of derivatives
	 */
	switch (choice) {
		case 0 :
			return converters::vecToPyList(xVec2D_.back());
		case 1 :
			return converters::vecToPyList(dxVec2D_.back());
		case 2 :
			return converters::vecToPyList(t_vec_);
		default:
			return converters::vecToPyList(std::vector< double > {-500, -250, -100});
	}
}
/* ------------------------------------------------------------------------- */
