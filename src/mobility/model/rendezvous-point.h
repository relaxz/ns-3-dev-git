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
class MineMobilityModel;
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
   * Returns a list of all mobile objects on this connection
   */
  std::vector<MineMobilityModel*> GetMobiles();

  /*
   * Adds a mobile object to the connection and schedules its removal
   */
  void AddMobile(MineMobilityModel* mob, Time arrival_time);

private:
  /*
   * Removes a mobile object from the list of objects using
   * this connection
   */
  void RemoveMobile(MineMobilityModel* mob);
  /*
   * A list of all mobile objects currently using this connection
   */
  std::vector<MineMobilityModel*> m_mobiles;
  /*
   * A list of the position vectors that describe the path
   * to the destination RendezvousPoint
   */
  std::vector<Vector> m_waypoints;
  /*
   * The RendezvousPoint that will be reached if one follows this connection
   */
  RendezvousPoint* m_target;

};

class RendezvousPoint
{
public:
  /*
   * Defines a Rendezvous point. A location where two nodes can meet to avoid collision.
   * Holds its position and all connections from it.
   */
  RendezvousPoint(Vector pos);

  /*
   * Adds a connection, with given waypoints, from this to another RendezvousPoint rp, and the other way around
   */
  void Connect(RendezvousPoint* rp, std::vector<Vector> waypoints);

  /*
   * Returns the connection from this RendezvousPoint to another RendezvousPoint rp.
   * The connection must exist.
   */
  Connection& GetConnectionTo(RendezvousPoint* rp);

  /*
   * Returns the connection from the RendezvousPoint rp to this RendezvousPoint
   * The connection must exist.
   */
  Connection& GetConnectionFrom(RendezvousPoint* rp);

  /*
    * returns the waypoints of a connection from this to RendezvousPoint rp
    */
  std::vector<Vector> GetConnectionPoints(RendezvousPoint* rp);

  /*
   * Returns a list of all objects approaching on the connection from a
   * RendezvousPoint rp
   */
  std::vector<MineMobilityModel*> GetApproachingMobilesFrom(RendezvousPoint* rp);

  /*
   * Returns a list of all objects approaching on any connection
   */
  std::vector<MineMobilityModel*> GetAllApproachingMobiles();

  /*
   * Returns a list of all objects approaching on any connection except rp
   */
  std::vector<MineMobilityModel*> GetApproachingMobilesExceptFrom(RendezvousPoint* rp);

  /*
   * Returns true if an object is approaching on the path from rp
   */
  bool IsConnectionBusyFrom(RendezvousPoint* rp);

  /*
   * Returns the time left until this connection is expected to
   * be usable from this RendezvousPoint to RendezvousPoint rp.
   */
  Time GetTimeLeftUntilClear(RendezvousPoint* rp);

  /*
   * Returns position of this RendezvousPoint
   */
  Vector GetPosition(void);

  /*
   * Reverses the given vector and returns it.
   */
  std::vector<Vector> ReverseVector(std::vector<Vector>);
  /*
   * Two RendezvousPoints are considered equal if their positions
   * are the same
   */
  bool operator== (RendezvousPoint & o);
private:
  Vector m_pos;
  std::vector<Connection> m_connections;
  void ConnectOneWay(RendezvousPoint* rp, std::vector<Vector> waypoints);
};
}

#endif /* SRC_MOBILITY_MODEL_RENDEZVOUS_POINT_H_ */
