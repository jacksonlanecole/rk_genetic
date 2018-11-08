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
		double tf;
		double h;
		double x;
		int stages;
		int steps;
		bool finished;
		std::vector< double > kVec;
		py::object func;

		std::vector< double > xVec;
		std::vector< double > dxVec;
		std::vector< double > tVec;


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


		void stepper();
		double step(double&, double&);
		double run();
		bool isFinished();


		void setTimeStep(double);
		double getTimeStep();



		void initkVecTo0();


		//vDoub get_xVec();
		//vDoub get_dxVec();
		//vDoub get_tVec();
		std::vector< double > get_xVec();
		std::vector< double > get_dxVec();
		std::vector< double > get_tVec();

};

#endif
