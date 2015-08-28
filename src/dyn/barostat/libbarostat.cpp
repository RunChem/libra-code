/*********************************************************************************
* Copyright (C) 2015 Alexey V. Akimov
*
* This file is distributed under the terms of the GNU General Public License
* as published by the Free Software Foundation, either version 2 of
* the License, or (at your option) any later version.
* See the file LICENSE in the root directory of this distribution
* or <http://www.gnu.org/licenses/>.
*
*********************************************************************************/

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include "libbarostat.h"

using namespace boost::python;


namespace libdyn{
namespace libbarostat{



void export_Barostat_objects(){

  void (Barostat::*expt_set_Nf_t_v1)(int nf_t) = &Barostat::set_Nf_t;
  void (Barostat::*expt_set_Nf_t_v2)(double nf_t) = &Barostat::set_Nf_t;
  void (Barostat::*expt_set_Nf_r_v1)(int nf_t) = &Barostat::set_Nf_r;
  void (Barostat::*expt_set_Nf_r_v2)(double nf_t) = &Barostat::set_Nf_r;
  void (Barostat::*expt_set_Nf_b_v1)(int nf_t) = &Barostat::set_Nf_b;
  void (Barostat::*expt_set_Nf_b_v2)(double nf_t) = &Barostat::set_Nf_b;


  class_<Barostat>("Barostat",init<>())
      .def(init<boost::python::dict>())

      .def_readwrite("ksi_eps",&Barostat::ksi_eps)
      .def_readwrite("G_eps",&Barostat::G_eps)
      .def_readwrite("eps_iso",&Barostat::eps_iso)
      .def_readwrite("ksi_eps_iso",&Barostat::ksi_eps_iso)
      .def_readwrite("G_eps_iso",&Barostat::G_eps_iso)
      .def_readwrite("Wg",&Barostat::Wg)
      .def_readwrite("nu_baro",&Barostat::nu_baro)
      .def_readwrite("Pressure",&Barostat::Pressure)
      .def_readwrite("barostat_type",&Barostat::barostat_type)

      .def("show_info",&Barostat::show_info)
      .def("set_Nf_t", expt_set_Nf_t_v1)
      .def("set_Nf_t", expt_set_Nf_t_v2)
      .def("set_Nf_r", expt_set_Nf_r_v1)
      .def("set_Nf_r", expt_set_Nf_r_v2)
      .def("set_Nf_b", expt_set_Nf_b_v1)
      .def("set_Nf_b", expt_set_Nf_b_v2)
      .def("get_Nf_t", &Barostat::get_Nf_t)
      .def("get_Nf_r", &Barostat::get_Nf_r)
      .def("get_Nf_b", &Barostat::get_Nf_b)

      .def("ekin_baro", &Barostat::ekin_baro)
      .def("apply_barostat_force", &Barostat::apply_barostat_force)
      .def("scale_velocity", &Barostat::scale_velocity)
      .def("propagate_velocity", &Barostat::propagate_velocity)      
      .def("cool", &Barostat::cool)

  ;


}// export_Barostat_objects



#ifdef CYGWIN
BOOST_PYTHON_MODULE(cygbarostat){
#else
BOOST_PYTHON_MODULE(libbarostat){
#endif

  // Register converters:
  // See here: https://misspent.wordpress.com/2009/09/27/how-to-write-boost-python-converters/
  //to_python_converter<std::vector<DATA>, VecToList<DATA> >();

  export_Barostat_objects();

}


}// namespace libbarostat
}// namespace libdyn

