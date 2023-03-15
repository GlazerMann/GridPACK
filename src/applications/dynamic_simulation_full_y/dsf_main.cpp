/*
 *     Copyright (c) 2013 Battelle Memorial Institute
 *     Licensed under modified BSD License. A copy of this license can be found
 *     in the LICENSE file in the top level directory of this distribution.
 */

#include "mpi.h"
#include <ga.h>
#include <macdecls.h>
#include "gridpack/parser/dictionary.hpp"
#include "gridpack/math/math.hpp"
#include "gridpack/applications/modules/dynamic_simulation_full_y/dsf_app_module.hpp"
#include <vector>

// Main driver for dynamic simulation application

int
main(int argc, char **argv)
{
  // Noprint when set to true suppresses "All" stdout output
  gridpack::NoPrint *noprint_ins = gridpack::NoPrint::instance();
  noprint_ins->setStatus(false);
  
  // Initialize MPI libraries
  int ierr = MPI_Init(&argc, &argv);

  GA_Initialize();
  int stack = 200000, heap = 200000;
  MA_init(C_DBL, stack, heap);

  // Intialize Math libraries
  gridpack::math::Initialize(&argc,&argv);

  // Overall applicaton timer
  gridpack::utility::CoarseTimer *timer =
    gridpack::utility::CoarseTimer::instance();
  int t_total = timer->createCategory("Dynamic Simulation: Total Application");
  timer->start(t_total);

  gridpack::parallel::Communicator world;

  // read configuration file 
  int t_config = timer->createCategory("Dynamic Simulation: Config");
  timer->start(t_config);
  gridpack::utility::Configuration *config =
    gridpack::utility::Configuration::configuration();

  if (argc >= 2 && argv[1] != NULL) { 
    char inputfile[256]; 
    sprintf(inputfile,"%s",argv[1]);
    config->open(inputfile,world);
  } else {
    config->open("input.xml",world);
  }
  timer->stop(t_config);

  // setup and run powerflow calculation
  gridpack::utility::Configuration::CursorPtr cursor;
  cursor = config->getCursor("Configuration.Powerflow");
  bool useNonLinear = false;
  useNonLinear = cursor->get("UseNonLinear", useNonLinear);

  boost::shared_ptr<gridpack::powerflow::PFNetwork>
    pf_network(new gridpack::powerflow::PFNetwork(world));

  gridpack::powerflow::PFAppModule* pf_app(new gridpack::powerflow::PFAppModule);
  pf_app->readNetwork(pf_network, config);
  pf_app->initialize();
  if (useNonLinear) {
    pf_app->nl_solve();
  } else {
    pf_app->solve();
  }
  pf_app->write();
  pf_app->saveData();
   
  // setup and run dynamic simulation calculation
  boost::shared_ptr<gridpack::dynamic_simulation::DSFullNetwork>
    ds_network(new gridpack::dynamic_simulation::DSFullNetwork(world));

  // Power flow and dynamics simulation application have the same network.
  // So we just clone it.
  gridpack::dynamic_simulation::DSFullApp* ds_app(new gridpack::dynamic_simulation::DSFullApp);
  pf_network->clone<gridpack::dynamic_simulation::DSFullBus,
		    gridpack::dynamic_simulation::DSFullBranch>(ds_network);

  // transfer results from PF calculation to DS calculation
  ds_app->transferPFtoDS(pf_network, ds_network); 

  // read in faults from input file
  cursor = config->getCursor("Configuration.Dynamic_simulation");
  std::vector<gridpack::dynamic_simulation::Event> faults;
  faults = ds_app->getFaults(cursor);

  // run dynamic simulation

  // Set network and set up simulation parameters from config options
  ds_app->setNetwork(ds_network, config);

  // Read generators
  ds_app->readGenerators();

  // For future use
  ds_app->readSequenceData();

  // set up dynamics simulation objects
  ds_app->initialize();

  // Set up output files
  ds_app->setGeneratorWatch();

  // Set up
  ds_app->solvePreInitialize(faults[0]);
	
  while(!ds_app->isDynSimuDone()){
    ds_app->executeOneSimuStep( );
  }

  timer->stop(t_total);
  timer->dump();

  delete(pf_app);
  delete(ds_app);
  
  // Terminate Math libraries
  gridpack::math::Finalize();

  // Terminate GA
  GA_Terminate();

  // Clean up MPI libraries
  ierr = MPI_Finalize();

  return 0;
}

