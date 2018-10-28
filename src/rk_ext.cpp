#include "rk.hpp"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(rk_ext)
{
	using namespace boost::python;
	class_<RKIntegrator>("RKIntegrator")
<<<<<<< HEAD
		.def("RK", &RKIntegrator::RK)
		;
=======
        .def("RK", &RKIntegrator::RK)
    ;
>>>>>>> 2822de60e8f57250fc89712cc73a53e084cc7546
}
