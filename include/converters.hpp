// converters.hpp
#ifndef CONVERTERS_H_
#define CONVERTERS_H_
#include <boost/python.hpp>
#include <vector>
#include <iostream>

namespace py = boost::python;

namespace converters {
/* ------------------------------------------------------------------------- */
// Typed Converters
/* ------------------------------------------------------------------------- */
inline std::vector< double > pyListTodVec(py::list& pyList) {
	/* python list -> vector< doubles > */
	std::vector< double > dVec;

	for (int i = 0; i < len(pyList); i++)
        dVec.push_back(py::extract<double>(pyList[i]));

	return dVec;
} // pyListTodVec


inline std::vector< std::vector< double > > pyListTodVec2D(py::list& pyList) {
	/* 2D python list -> vector< vector< doubles > > */
	std::vector< std::vector< double > > dVec2D;

	for (int i = 0; i < len(pyList); i++) {
		for (int j = 0; j < len(pyList[i]); j++)
			dVec2D[i].push_back(py::extract<double>(pyList[i][j]));
	}

	return dVec2D;
} // pyListTodVec2D
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
// Non-typed Converters
/* ------------------------------------------------------------------------- */
template <typename T>
inline py::list vecToPyList(std::vector< T > vec) {
	/* vector -> python list */
	py::list pyList;

	for (int i = 0; i < vec.size(); i++)
		pyList.append(vec[i]);

	return pyList;
} // end vecToPyList


template <typename T>
inline py::list vec2DToPyList(std::vector< std::vector< T > > vec2D) {
	/* 2D vector -> python list */
	py::list pyList;
	for (int i = 0; i < vec2D.size(); i++) {
		for (int j = 0; j < vec2D[i].size(); j++)
			pyList.append(vec2D[i][j]);
	}

	return pyList;
} // end vec2DToPyList
/* ------------------------------------------------------------------------- */

} // end namespace converters

#endif
