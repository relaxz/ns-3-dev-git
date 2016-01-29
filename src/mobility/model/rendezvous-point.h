/*
 * rendezvous-point.h
 *
 *  Created on: Jan 29, 2016
 *      Author: joakim
 */

#ifndef SRC_MOBILITY_MODEL_RENDEZVOUS_POINT_H_
#define SRC_MOBILITY_MODEL_RENDEZVOUS_POINT_H_

#include "ns3/attribute.h"
#include "ns3/attribute-helper.h"
#include "ns3/nstime.h"
#include "ns3/vector.h"

namespace ns3 {

class Connection {
public:
  Connection();

  /*
   * Returns waypoints in connection
   */
  std::vector<Vector> GetPoints(void);

  /*
   * returns the target RendezvousPoint of the connections
   */
  RendezvousPoint GetTarget(void);

private:
  std::vector nodes;
  std::vector waypoints;
  RendezvousPoint target;
};

class RendezvousPoint
{
public:
  RendezvousPoint(std::vector pos);

  /*
   * Returns the connection to another RendezvousPoint
   */
  Connection GetConnection(RendezvousPoint rp);

   /*
    * returns the waypoints of a connection
    */
  std::vector<Vector> GetConnectionPoints(RendezvousPoint rp);

private:
  std::vector pos = pos;
  std::vector<Connection> connections;

};
}

#endif /* SRC_MOBILITY_MODEL_RENDEZVOUS_POINT_H_ */
