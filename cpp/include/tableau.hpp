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
typedef std::vector< double > vec1D;
typedef std::vector< std::vector< double > > vec2D;
/* ------------------------------------------------------------------------- */

class Tableau {
	private:
		vec2D tableau_;
		int stages_;
		vec1D weights_;
		vec2D rk_mat_;
		vec1D nodes_;

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
		vec1D getNodes();

		void setrkMat();
		vec2D getrkMat();

		void setWeights();
		vec1D getWeights();
};

#endif
