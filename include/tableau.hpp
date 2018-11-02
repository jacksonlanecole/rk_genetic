// tableau.hpp
#ifndef TABLEAU_H_
#define TABLEAU_H_
/* ------------------------------------------------------------------------- */
// Includes
/* ------------------------------------------------------------------------- */
#include <vector>
#include <boost/python.hpp>
/* ------------------------------------------------------------------------- */

namespace py = boost::python;

/* ------------------------------------------------------------------------- */
// Type Definitions
/* ------------------------------------------------------------------------- */
typedef std::vector<double> vDoub;
typedef std::vector< std::vector<double> > vec2D;
/* ------------------------------------------------------------------------- */

class Tableau {
	private:
		vec2D tableau;
		int stages;
		vDoub weights;
		vec2D rkMat;
		vDoub nodes;

	public:
		/* ----------------------------------------------------------------- */
		// Constructors, destructor
		/* ----------------------------------------------------------------- */
		Tableau();
		Tableau(vec2D&);
		Tableau(py::list&);
		Tableau(const Tableau&);
		~Tableau();

		void reset(vec2D&); // "constructor" to use after declaration
		void reset(py::list&); // "constructor" to use after declaration
		/* ----------------------------------------------------------------- */

		void setTableau(vec2D&);
		void setTableau(py::list&);

		vec2D getTableau();


		void setStages();
		int getStages();

		void setNodes();
		vDoub getNodes();

		void setrkMat();
		vec2D getrkMat();

		void setWeights();
		vDoub getWeights();
};

#endif
