#include "rk.hpp"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

BOOST_PYTHON_MODULE(rk_ext)
{
	using namespace boost::python;
	class_<std::vector<double> >("PyDVec")
		.def(boost::python::vector_indexing_suite<std::vector<double> >())
		.def(boost::python::vector_indexing_suite<std::vector< std::vector< double > > >());

	class_<RKIntegrator>("RKIntegrator")
		.def(init<object&, list&, double, double, double, bool>())
		.def(init<object&, list&, double, double, double, double, bool>())
		.def("step", &RKIntegrator::step)
		.def("step", &RKIntegrator::stepWrap)
		//.def("step", &RKIntegrator::stepAugWrap)
		.def("step", &RKIntegrator::vecStep)
		.def("run", &RKIntegrator::run)
		.def("isFinished", &RKIntegrator::isFinished)
		.def("get_xVec", &RKIntegrator::get_xVec)
		.def("get_dxVec", &RKIntegrator::get_dxVec)
		.def("get_tVec", &RKIntegrator::get_tVec)
		.def("get_last", &RKIntegrator::get_last)
		;
}
