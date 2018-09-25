#ifndef RK_H_
#define RK_H_
#include <vector>
#include <iostream> //DELETE

typedef std::vector< std::vector<double> > ButcherTableau;
typedef std::vector<double> vDoub;
typedef std::vector< std::vector<double> > vec2D;
//typedef std::vector<double> kVec;

class RKIntegrator {
	private:
		double t;
		int stages;
		int steps;
		ButcherTableau bt;
		vDoub weights;
		vec2D rkMat;
		vDoub nodes;
		vDoub kVec;

	public:
		RKIntegrator();
		~RKIntegrator();

		void RK(double (*f)(double, double), ButcherTableau&, double, double,
				double, double);
		double step(double (*)(double, double), double, double&, double, vDoub&);

		void setButcherTableau(ButcherTableau&);
		ButcherTableau getButcherTableau();

		void setStages(int);
		int getStages();

		void setNodes();
		vDoub getNodes();

		void setrkMat();
		vec2D getrkMat();

		void setWeights();
		vDoub getWeights();

		void initkVecTo0();


};

#endif
