/*
 *     Copyright (c) 2013 Battelle Memorial Institute
 *     Licensed under modified BSD License. A copy of this license can be found
 *     in the LICENSE file in the top level directory of this distribution.
 */
/*
 * PSSE33Export.hpp
 *
 *  Export .RAW files using PSS/E version 23 format
 *
 *  Created on: July 9, 2021
 *      Author: Bruce Palmer
 */

#ifndef PSSE23EXPORT_HPP_
#define PSSE23EXPORT_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "gridpack/export/data_blocks/export_bus23.hpp"
#include "gridpack/export/data_blocks/export_gen23.hpp"
#include "gridpack/export/data_blocks/export_line23.hpp"
#include "gridpack/export/data_blocks/export_xform23.hpp"
#include "gridpack/export/data_blocks/export_area23.hpp"
#include "gridpack/export/data_blocks/export_2term23.hpp"

namespace gridpack {
namespace expnet {

template <class _network>
class PSSE23Export
{
  public:

    /**
     * Constructor
     */
    explicit PSSE23Export(boost::shared_ptr<_network> network) :
      p_network(network), p_comm(network->communicator())
    {
    }

    /**
     * Destructor
     */
    virtual ~PSSE23Export()
    {
    }

    /**
     * Write out data in PSS/E v23 format to a file
     * @param filename name of file that contains output
     */
    void  writeFile(std::string filename) {
      int me = p_comm.rank();
      std::ofstream fout;
      if (me == 0) {
        fout.open(filename.c_str());
      }
      // Write out individual data blocks
      ExportBus23<_network> buses(p_network);
      buses.writeBusBlock(fout);
      ExportGen23<_network> generators(p_network);
      generators.writeGenBlock(fout);
      ExportLine23<_network> branches(p_network);
      branches.writeLineBlock(fout);
      ExportXform23<_network> xforms(p_network);
      xforms.writeXformBlock(fout);
      
      if (me == 0) {
		fout << "0 / END OF TRANSFORMER ADJUSTMENT DATA, BEGIN AREA DATA" << std::endl;
		fout << "0 / END OF AREA DATA, BEGIN TWO-TERMINAL DC DATA" << std::endl;
		fout << "0 / END OF TWO-TERMINAL DC DATA, BEGIN SWITCHED SHUNT DATA" << std::endl;
		fout << "0 / END OF SWITCHED SHUNT DATA, BEGIN IMPEDANCE CORRECTION DATA" << std::endl;
		fout << "0 / END OF IMPEDANCE CORRECTION DATA, BEGIN MULTI-TERMINAL DC DATA" << std::endl;
		fout << "0 / END OF MULTI-TERMINAL DC DATA, BEGIN MULTI-SECTION LINE DATA" << std::endl;
		fout << "0 / END OF MULTI-SECTION LINE DATA, BEGIN ZONE DATA" << std::endl;
		fout << "0 / END OF ZONE DATA, BEGIN INTER-AREA TRANSFER DATA" << std::endl;
		fout << "0 / END OF INTER-AREA TRANSFER DATA, BEGIN OWNER DATA" << std::endl;
		fout << "0 / END OF OWNER DATA, BEGIN FACTS DEVICE DATA" << std::endl;

        // Write closing 'Q'
        // fout << "Q" << std::endl;
        fout.close();
      }
    }

  private:
    boost::shared_ptr<_network>      p_network;

    gridpack::parallel::Communicator p_comm;
};

} /* namespace export */
} /* namespace gridpack */

#endif /* PSSE23EXPORT_HPP_ */
