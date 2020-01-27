// -------------------------------------------------------------
// file: gridpack.cpp
// -------------------------------------------------------------
// -------------------------------------------------------------
/*
 *     Copyright (c) 2013 Battelle Memorial Institute
 *     Licensed under modified BSD License. A copy of this license can be found
 *     in the LICENSE file in the top level directory of this distribution.
 */
// -------------------------------------------------------------
// -------------------------------------------------------------
// Created January 24, 2020 by Perkins
// Last Change: 2020-01-27 08:04:49 d3g096
// -------------------------------------------------------------

#include <pybind11/pybind11.h>
namespace py = pybind11;

#include <gridpack/environment/environment.hpp>
#include <gridpack/parallel/communicator.hpp>
namespace gp = gridpack;
namespace gpp = gridpack::parallel;

// GridPACK uses Boost smart pointers, so let's use those here
PYBIND11_DECLARE_HOLDER_TYPE(T, boost::shared_ptr<T>, false);

PYBIND11_MODULE(gridpack, gpm) {
  gpm.doc() = "GridPACK module";

  // gridpack::Envronment class
  py::class_<gp::Environment, boost::shared_ptr<gp::Environment> >(gpm, "Environment")
  ;

  // gridpack::parallel::Communicator class
  py::class_<gpp::Communicator>(gpm, "Communicator")
    .def("size", &gpp::Communicator::size)
    .def("rank", &gpp::Communicator::rank)
    .def("worldRank", &gpp::Communicator::worldRank)
    .def("barrier", &gpp::Communicator::barrier)
    .def("sync", &gpp::Communicator::sync)
    .def("divide", &gpp::Communicator::divide)
    .def("split", &gpp::Communicator::split)
    ;
    
  // gridpack::parallel::TaskManager class
  
}
