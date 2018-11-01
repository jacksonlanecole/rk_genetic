// rk.hpp
#ifndef RK_H_
#define RK_H_
/* ------------------------------------------------------------------------- */
// Includes
/* ------------------------------------------------------------------------- */
#include <vector>
#include "tableau.hpp"
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
// Type Definitions
/* ------------------------------------------------------------------------- */
typedef std::vector<double> vDoub;
typedef std::vector< std::vector<double> > vec2D;
typedef double (*function_type)(double, double);
/* ------------------------------------------------------------------------- */


class RKIntegrator {
	private:
		double t;
		double h;
		double x;
		int stages;
		int steps;
		vDoub kVec;
		function_type func;

		vDoub xVec;
		vDoub dxVec;
		vDoub tVec;


	public:
		Tableau bt;
		/* ----------------------------------------------------------------- */
		// Constructors
		/* ----------------------------------------------------------------- */
		RKIntegrator();
		RKIntegrator(double, double, double, double);// test constructor
		RKIntegrator(
				function_type func, vec2D&, double, double,
				double, double);
		//RKIntegrator(
		//		double (*f)(double, double), ButcherTableau&, double, double,
		//		double, double);
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
