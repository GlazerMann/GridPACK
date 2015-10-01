// -------------------------------------------------------------
/*
 *     Copyright (c) 2013 Battelle Memorial Institute
 *     Licensed under modified BSD License. A copy of this license can be found
 *     in the LICENSE file in the top level directory of this distribution.
 */
// -------------------------------------------------------------
// -------------------------------------------------------------
/**
 * @file   variable_test.cpp
 * @author William A. Perkins
 * @date   2015-10-01 09:23:58 d3g096
 * 
 * @brief  
 * 
 * 
 */
// -------------------------------------------------------------

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <boost/bind.hpp>
#include "gridpack/utilities/exception.hpp"
#include "gridpack/parallel/parallel.hpp"
#include "variable.hpp"

#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

/// The type of (non-MPI) serialization archive to use
typedef boost::archive::binary_oarchive oArchive;
typedef boost::archive::binary_iarchive iArchive;

#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/included/unit_test.hpp>

namespace go = gridpack::optimization;

#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT(go::Variable);
BOOST_CLASS_EXPORT(go::BoundedVariableT<double>);
BOOST_CLASS_EXPORT(go::BoundedVariableT<int>);
BOOST_CLASS_EXPORT(go::BinaryVariable);

// -------------------------------------------------------------
//  class VariableNamePrinter
// -------------------------------------------------------------
class VariablePrinter 
  : public go::VariableVisitor
{
public:

  /// Default constructor.
  VariablePrinter(void)
    : VariableVisitor()
  {}

  /// Destructor
  ~VariablePrinter(void)
  {}

  void visit(go::Variable& var)
  {
    std::cout << var.name() << std::endl;
  }
  void visit(go::RealVariable& var)
  {
    std::cout << var.name() << ": real"
              << ": (" << var.lowerBound() << ":" << var.upperBound() << ")"
              << std::endl;
  }

  void visit(go::IntegerVariable& var)
  {
    std::cout << var.name()  << ": integer"
              << ": (" << var.lowerBound() << ":" << var.upperBound() << ")"
              << std::endl;
  }

  void visit(go::BinaryVariable& var)
  {
    std::cout << var.name() << ": binary"
              << ": (" << var.lowerBound() << ":" << var.upperBound() << ")"
              << std::endl;
  }
};

BOOST_AUTO_TEST_SUITE( VariableTest )

BOOST_AUTO_TEST_CASE( create )
{
  std::vector<go::VariablePtr> vlist;
  vlist.push_back(go::VariablePtr(new go::RealVariable(13.0)));
  vlist.push_back(go::VariablePtr(new go::RealVariable(0.0, -1.0, 1.0)));
  vlist.push_back(go::VariablePtr(new go::IntegerVariable(0, -1, 1)));
  vlist.push_back(go::VariablePtr(new go::BinaryVariable(1)));

  {
    go::VariableCounter cnt;
    for_each(vlist.begin(), vlist.end(),
             boost::bind(&go::Variable::accept, _1, boost::ref(cnt)));
    BOOST_CHECK_EQUAL(cnt.numVar, 4);
    BOOST_CHECK_EQUAL(cnt.numReal, 2);
    BOOST_CHECK_EQUAL(cnt.numInt, 1);
    BOOST_CHECK_EQUAL(cnt.numBin, 1);
  }

  {
    go::VariableTable vt(std::cout);
    for_each(vlist.begin(), vlist.end(),
             boost::bind(&go::Variable::accept, _1, boost::ref(vt)));
  }
}

BOOST_AUTO_TEST_CASE( serialization )
{
  std::vector<go::VariablePtr> vlist0, vlist1;
  vlist0.push_back(go::VariablePtr(new go::RealVariable(13.0)));
  vlist0.push_back(go::VariablePtr(new go::RealVariable(0.0, -1.0, 1.0)));
  vlist0.push_back(go::VariablePtr(new go::IntegerVariable(0, -1, 1)));
  vlist0.push_back(go::VariablePtr(new go::BinaryVariable(1)));

  std::string buffer;
  {
    std::ostringstream ostr(std::ios::binary);
    oArchive oarch(ostr);
    oarch & vlist0;
    buffer = ostr.str();
  }

  
  {
    std::istringstream is(buffer, std::ios::binary);
    iArchive iarch(is);
    iarch & vlist1;
  }

  // hopefully we got the same number of variables
  BOOST_CHECK_EQUAL(vlist0.size(), vlist1.size());

  // the variable names should match
  for (unsigned int i = 0; i < vlist0.size(); ++i) {
    BOOST_CHECK_EQUAL(vlist0[i]->name(), vlist1[i]->name());
  }

  go::VariableCounter cnt0, cnt1;
  for_each(vlist0.begin(), vlist0.end(),
           boost::bind(&go::Variable::accept, _1, boost::ref(cnt0)));
  for_each(vlist1.begin(), vlist1.end(),
           boost::bind(&go::Variable::accept, _1, boost::ref(cnt1)));
  BOOST_CHECK_EQUAL(cnt0.numVar, cnt1.numVar);
  BOOST_CHECK_EQUAL(cnt0.numReal, cnt1.numReal);
  BOOST_CHECK_EQUAL(cnt0.numInt, cnt1.numInt);
  BOOST_CHECK_EQUAL(cnt0.numBin, cnt1.numBin);
}


BOOST_AUTO_TEST_SUITE_END()

// -------------------------------------------------------------
// init_function
// -------------------------------------------------------------
bool init_function()
{
  return true;
}

// -------------------------------------------------------------
//  Main Program
// -------------------------------------------------------------
int
main(int argc, char **argv)
{
  gridpack::parallel::Environment env(argc, argv);
  int result = ::boost::unit_test::unit_test_main( &init_function, argc, argv );
  return result;
}
