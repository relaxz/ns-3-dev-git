/*
 * mine-mobility-helper.h
 *
 *  Created on: Feb 19, 2016
 *      Author: joakim
 */

#ifndef SRC_MOBILITY_HELPER_MINE_MOBILITY_HELPER_H_
#define SRC_MOBILITY_HELPER_MINE_MOBILITY_HELPER_H_

#include "ns3/node.h"
#include "ns3/vector.h"
#include "ns3/rendezvous-point.h"

namespace ns3 {

/**
* \ingroup mobility
* \brief Helper class used to assign positions and movement patterns(paths) for nodes
* MineMobilityHelper::Install is the most important method here.
*/
class MineMobilityHelper
{
public:

  /**
   * Construct a Mine Mobility Helper which is used to make life easier when working
   * with Mine mobility. Default values for vehicles:  DUMPER[Speed = 3; Priority = 1], LOADER[Speed = 7; Priority = 2], CAR[Speed = 15; Priority = 3]
   */
  MineMobilityHelper ();

  /**
   * Destroy a Mine Mobility Helper
   */
  ~MineMobilityHelper ();

  /*
   * enum which declares different vehicle types, used for install.
   * Default values: (speed=m/s)
   * DUMPER[Speed = 3; Priority = 1], LOADER[Speed = 7; Priority = 2], CAR[Speed = 15; Priority = 3]
   */
  enum vehicle_type {DUMPER, LOADER, CAR};

  /*
  * This method creates an instance of a ns3::MineMobilityModel subclass, aggregates
  * it to the provided node, and sets the provided path, priority and speed.
  *
  * \param node The node on which to Install
  * \param path The desired path for the provided node
  * \param priority The desired priority for the provided node
  * \param speed The desired speed for the provided node
  */
  void Install (Ptr<Node> node, std::vector<RendezvousPoint*> &path, double speed, uint32_t priority);


  /*
  * This method creates an instance of a ns3::MineMobilityModel subclass, aggregates
  * it to the provided node, sets the provided path, and sets priority and speed based on a vehicle enum.
  *
  * \param node The node on which to Install
  * \param path The desired path for the provided node
  * \param vehicle Enum on which priority and speed will be determined.
  */
  void Install (Ptr<Node> node, std::vector<RendezvousPoint*> &path, vehicle_type vehicle);

private:
  double m_dumperSpeed;
  int m_dumperPriority;
  double m_loaderSpeed;
  int m_loaderPriority;
  double m_carSpeed;
  int m_carPriority;
};
}//ns3
#endif /* SRC_MOBILITY_HELPER_MINE_MOBILITY_HELPER_H_ */
