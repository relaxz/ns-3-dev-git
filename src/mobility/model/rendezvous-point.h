/*
 * rendezvous-point.h
 *
 *  Created on: Jan 29, 2016
 *      Author: joakim
 */

#ifndef SRC_MOBILITY_MODEL_RENDEZVOUS_POINT_H_
#define SRC_MOBILITY_MODEL_RENDEZVOUS_POINT_H_

#include "ns3/node.h"
#include "ns3/vector.h"

namespace ns3 {
class RendezvousPoint;

class Connection {
public:
  Connection();

  /*
   * Returns waypoints in connection
   */
  const std::vector<Vector>& GetPoints(void);

  /*
   * returns the target RendezvousPoint of the connections
   */
  const RendezvousPoint& GetTarget(void);


private:
  std::vector<Node> nodes;
  std::vector<double> waypoints;
  const RendezvousPoint& target;
};

class RendezvousPoint
{
public:
  RendezvousPoint(std::vector<double> pos, std::vector<Connection> Connectionlist);

  /*
   * Returns the connection to another RendezvousPoint rp
   */
  Connection GetConnection(RendezvousPoint rp);

   /*
    * returns the waypoints of a connection
    */
  std::vector<Vector> GetConnectionPoints(RendezvousPoint rp);

  /*
   * Returns position of a RendezvousPoint
   */
  std::vector<double> GetPosition();

private:
  std::vector<double> m_pos;
  std::vector<Connection> m_connections;

};
}

#endif /* SRC_MOBILITY_MODEL_RENDEZVOUS_POINT_H_ */
