// rk.hpp
#ifndef RK_H_
#define RK_H_
/* ------------------------------------------------------------------------- */
// Includes
/* ------------------------------------------------------------------------- */
#include <vector>
#include <boost/python.hpp>
#include "tableau.hpp"
/* ------------------------------------------------------------------------- */

namespace py = boost::python;

/* ------------------------------------------------------------------------- */
// Type Definitions
/* ------------------------------------------------------------------------- */
typedef std::vector<double> vDoub;
typedef std::vector< std::vector<double> > vec2D;
/* ------------------------------------------------------------------------- */


class RKIntegrator {
	private:
		double t;
		double h;
		double x;
		int stages;
		int steps;
		vDoub kVec;
		py::object func;

		vDoub xVec;
		vDoub dxVec;
		vDoub tVec;


	public:
		Tableau bt;
		/* ----------------------------------------------------------------- */
		// Constructors
		/* ----------------------------------------------------------------- */
		RKIntegrator();
		RKIntegrator(
				py::object&, py::list&, double, double,
				double, double);
		RKIntegrator(const RKIntegrator& rkArg);
		~RKIntegrator();
		/* ----------------------------------------------------------------- */
		/*
		void RK(double (*f)(double, double), ButcherTableau&, double, double,
			double, double);
		*/


		double step(double&, double&);
		double run();


		void setTimeStep(double);
		double getTimeStep();


		void initkVecTo0();


};

#endif
