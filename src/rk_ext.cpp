#include "rk.hpp"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(rk_ext)
{
	using namespace boost::python;
    class_<RKIntegrator>("RKIntegrator")
        .def("RK", &RKIntegrator::RK)
    ;
}
