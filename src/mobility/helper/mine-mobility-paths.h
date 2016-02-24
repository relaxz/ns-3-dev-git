/*
 * mine-mobility-paths.h
 *
 *  Created on: Feb 18, 2016
 *      Author: joakim
 */

#ifndef SRC_MOBILITY_HELPER_MINE_MOBILITY_PATHS_H_
#define SRC_MOBILITY_HELPER_MINE_MOBILITY_PATHS_H_

#include "ns3/core-module.h"
#include "ns3/rendezvous-point.h"

namespace ns3 {

/**
 * Predefines public RendezvousPoints(global), connections and paths for MineMobilityModel.
 * Known paths are: ABC, CBA, FH, HF, GI, IG
 * 	     G
 * 	     |
 * 	     C
 * 	     |
 * F -- A -- B -- D -- H
 * 	     |
 * 	     E
 * 	     |
 * 	     I
 */
class MineMobilityPaths {
public:
  /**
   * Sets all Paths, Connections and RendenzvousPoints
   */
  MineMobilityPaths ();

  std::vector<Vector> a_to_b;
  std::vector<Vector> b_to_c;
  std::vector<Vector> b_to_d;
  std::vector<Vector> b_to_e;
  std::vector<Vector> a_to_f;
  std::vector<Vector> c_to_g;
  std::vector<Vector> d_to_h;
  std::vector<Vector> e_to_i;

  std::vector<RendezvousPoint*> pathABC;
  std::vector<RendezvousPoint*> pathCBA;
  std::vector<RendezvousPoint*> pathFH;
  std::vector<RendezvousPoint*> pathHF;
  std::vector<RendezvousPoint*> pathGI;
  std::vector<RendezvousPoint*> pathIG;

  std::vector<RendezvousPoint*> pathA;
  std::vector<RendezvousPoint*> pathB;
  std::vector<RendezvousPoint*> pathC;
  std::vector<RendezvousPoint*> pathD;
  std::vector<RendezvousPoint*> pathE;
  std::vector<RendezvousPoint*> pathF;
  std::vector<RendezvousPoint*> pathG;
  std::vector<RendezvousPoint*> pathH;
  std::vector<RendezvousPoint*> pathI;


  /**
   * Function used for visualizing RendezvousPoints and connections in NetAnim
   * Sets stationary nodes in RendezvousPoints and wired connection between them.
   */
  void DisplayMinePaths ();

};

}//ns3


#endif /* SRC_MOBILITY_HELPER_MINE_MOBILITY_PATHS_H_ */
