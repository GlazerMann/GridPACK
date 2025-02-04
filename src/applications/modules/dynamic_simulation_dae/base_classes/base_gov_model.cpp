/*
 *     Copyright (c) 2013 Battelle Memorial Institute
 *     Licensed under modified BSD License. A copy of this license can be found
 *     in the LICENSE file in the top level directory of this distribution.
 */
// -------------------------------------------------------------
/**
 * @file   base_gov_model.cpp
 * @author Shuangshuang Jin 
 * @Last modified:   10/02/19
 *  
 * @brief  
 *
 *
 */

#include <base_gov_model.hpp>
#include <gridpack/include/gridpack.hpp>

BaseGovModel::BaseGovModel(void)
{
  mbase = DEFAULT_MVABASE;
  sbase = DEFAULT_MVABASE;
}

BaseGovModel::~BaseGovModel(void)
{
}

/**
 * Load parameters from DataCollection object into governor model
 * @param data collection of governor parameters from input files
 * @param index of goviteer on bus
 * TODO: might want to move this functionality to BaseGovModel
 */
void BaseGovModel::load(const boost::shared_ptr<gridpack::component::DataCollection>
        data, int idx)
{
  data->getValue(CASE_SBASE,&sbase); // System MVAbase, used in conversion from machine base to system base.
  data->getValue(GENERATOR_MBASE,&mbase,idx); // Machine base (in MVA)
}

/**
 * Initialize governor model before calculation
 * @param [output] values - array where initialized governor variables should be set
 */
void BaseGovModel::init(gridpack::ComplexType *values)
{
}

/**
 * Write output from governors to a string.
 * @param string (output) string with information to be printed out
 * @param bufsize size of string buffer in bytes
 * @param signal an optional character string to signal to this
 * routine what about kind of information to write
 * @return true if bus is contributing string to output, false otherwise
 */
bool BaseGovModel::serialWrite(char *string, const int bufsize,
			       const char *signal)
{
  return false;
}

/**
 *  Set the number of variables for this governor model
 *  @param [output] number of variables for this model
 */
bool BaseGovModel::vectorSize(int *nvar) const
{
  *nvar = 0;
  return true;
}

/**
 * Write out governor state
 * @param signal character string used to determine behavior
 * @param string buffer that contains output
 */
void BaseGovModel::write(const char* signal, char* string)
{
}

/**
 * Set the internal values of the voltage magnitude and phase angle. Need this
 * function to push values from vectors back onto governors
 * @param values array containing governor state variables
*/
void BaseGovModel::setValues(gridpack::ComplexType *values)
{
}

/**
 * Return the values of the governor vector block
 * @param values: pointer to vector values
 * @return: false if governor does not contribute
 *        vector element
 */
bool BaseGovModel::vectorValues(gridpack::ComplexType *values)
{
  return false;
}

/**
 * Set Jacobian block
 * @param values a 2-d array of Jacobian block for the bus
 */
bool BaseGovModel::setJacobian(gridpack::ComplexType **values)
{
  return false;
}


/**
 * Set the mechanical power parameter inside the governor
 * @param pmech value of the mechanical power
 */
void BaseGovModel::setInitialMechanicalPower(double pmech)
{
}

/** 
 * Get the value of the mechanical power parameter
 * @return value of the mechanical power
 */
double BaseGovModel::getMechanicalPower()
{
  return 0.0;
}

/**
 * Partial derivatives of Mechanical Power Pmech w.r.t. governor variables
 * @param xgov_loc locations of governor variables
 * @param dPmech_dxgov partial derivatives of mechanical power Pmech w.r.t governor variables
 */
bool BaseGovModel::getMechanicalPowerPartialDerivatives(int *xgov_loc,double *dPmech_dxgov)
{
  return false;
}


/** 
 * Set the value of VComp
 * @return value of Vcomp
 */
void BaseGovModel::setVcomp(double Vcomp)
{
}

/**
 * Set Event
 */
void BaseGovModel::setEvent(gridpack::math::DAESolver::EventManagerPtr eman)
{
}


void BaseGovModel::setGenerator(BaseGenModel *generator)
{
  p_gen = generator;
}

BaseGenModel* BaseGovModel::getGenerator(void)
{
  return p_gen;
}
 
