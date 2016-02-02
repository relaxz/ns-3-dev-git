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

/*
 * Defines the connection through different waypoints between two RendezvousPoints
 * Hold pointers to nodes on connection and a target
 * Nodes, waypoints, target
 */
class Connection {
public:
  Connection(RendezvousPoint* target, std::vector<Vector> waypoints);

  /*
   * Returns waypoints in connection
   */
  std::vector<Vector> GetPoints(void);

  /*
   * returns the pointer for target RendezvousPoint of the connection
   */
  RendezvousPoint* GetTarget(void);

  /*
   * Returns the pointer for the nodes on the connection
   */
  std::vector<Node*> GetNodes(void);

  /*
   * adds a node to connection
   */
  void AddNode(Node*);

  /*
   * Removes a node from connection
   */
  void RemoveNode(Node*);


private:
  std::vector<Node*> m_nodes;
  std::vector<Vector> m_waypoints;
  RendezvousPoint* m_target;

};

class RendezvousPoint
{
public:
  /*
   * Defines a Rendezvous point. A location where two nodes can meet to avoid collision.
   * Holds its position and all connections from it.
   */
  RendezvousPoint(std::vector<double> pos);

  /*
   * Adds a connection, with given waypoints, from this to another RendezvousPoint rp, and the other way around
   */
  void Connect(RendezvousPoint* rp, std::vector<Vector> waypoints);

  /*
   * Returns the connection to another RendezvousPoint rp
   */
  Connection GetConnection(RendezvousPoint* rp);

  /*
    * returns the waypoints of a connection from this to RendezvousPoint rp
    */
  std::vector<Vector> GetConnectionPoints(RendezvousPoint* rp);

  /*
   * Returns position of this RendezvousPoint
   */
  std::vector<double> GetPosition(void);

  /*
   * Reverses the given vector and returns it.
   */
  std::vector<Vector> ReverseVector(std::vector<Vector>);

private:
  std::vector<double> m_pos;
  std::vector<Connection*> m_connections;

};
}

#endif /* SRC_MOBILITY_MODEL_RENDEZVOUS_POINT_H_ */
