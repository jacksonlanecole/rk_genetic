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
		bool usingVector;
		std::vector< double > kVec;
		py::object func;

		std::vector< double > xVec;
		std::vector< double > dxVec;

		std::vector< std::vector< double > > xVec2D;
		std::vector< std::vector< double > > dxVec2D;

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
		RKIntegrator(
				py::object&, py::list&, double, double,
				double);
		RKIntegrator(const RKIntegrator& rkArg);
		~RKIntegrator();
		/* ----------------------------------------------------------------- */
		/*
		void RK(double (*f)(double, double), ButcherTableau&, double, double,
			double, double);
		*/


		void stepWrap();
		void stepAugWrap(double x);
		double step(double);
		//std::vector< double > vecStep(py::list&);
		py::list vecStep(py::list&);
		double run();
		bool isFinished();


		void setTimeStep(double);
		double getTimeStep();



		void initkVecTo0();


		//std::vector< double > get_xVec();
		//std::vector< double > get_dxVec();
		//std::vector< double > get_tVec();

		//std::vector< std::vector< double > > get_xVec2D();
		//std::vector< std::vector< double > > get_dxVec2D();

		py::list get_xVec();
		py::list get_dxVec();
		py::list get_tVec();
		py::list get_xVec2D();
		py::list get_dxVec2D();

		double get_last(int);

		py::list get_lastVec(int);
};

#endif
