/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef MINE_MOBILITY_MODEL_H
#define MINE_MOBILITY_MODEL_H

#include <vector>
#include "mobility-model.h"
#include "waypoint-mobility-model.h"
#include "ns3/vector.h"
#include "ns3/uinteger.h"
#include "rendezvous-point.h"

namespace ns3 {
/**
 * \ingroup mobility
 * \brief Mobility model for vehicles in mines.
 *
 * The object tries to follow a path, but may need to stop and wait at
 * rendezvous points if an object with higher priority needs to pass. After
 * the higher priority object has passed, movement is resumed. A low priority
 * value is considered low priority.
 *
 * Uses WaypointMobilityModel to do the work with following a path.
 */

class MineMobilityModel : public MobilityModel
{
public:
  /**
   * Register this type with the TypeId system.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  /**
   * \param path The path the object will follow
   *
   * Set the path the object will follow and move the object to the start of
   * the path
   */
  void SetPath (std::vector<RendezvousPoint*> &path);
  /**
   * Called when the object reaches a rendezvous point
   */
  void Rendezvous ();
  MineMobilityModel ();
  virtual ~MineMobilityModel ();
  /**
   *  TracedCallback signature.
   *
   * \param [in] model Value of the MineMobilityModel.
   */
  typedef void (* TracedCallback)(Ptr<const MineMobilityModel> model);
private:
  virtual void DoDispose (void);
  /**
   * \brief Get current position.
   * \return A vector with the current position of the node.
   */
  virtual Vector DoGetPosition (void) const;
  /**
   * \brief Sets a new position for the node
   * \param position A vector to be added as the new position
   */
  virtual void DoSetPosition (const Vector &position);
  /**
   * \brief Returns the current velocity of a node
   * \return The velocity vector of a node.
   */
  virtual Vector DoGetVelocity (void) const;
  /*
   * returns the distance between two vectors.
   * ref_point is the point where the roads intersect.
   * e.g. center of the cross-roads.
   */
  uint32_t DistancePointToPoint(Vector A, Vector B, std::vector<Vector> path_A, std::vector<Vector> path_B, Vector ref_point);
  /*
   * returns the distance between a vector and the reference point.
   */
  uint32_t GetDistanceRefToPoint(Vector ref_point, Vector x,std::vector<Vector> path);
  /*
   * checks if the two vectors are on the same road section for calculating purposes.
   */
  bool CheckIfOnTheSameRoadSection(Vector ref_point, Vector x1, Vector x2, std::vector<Vector> A, std::vector<Vector> B);
  /*
   * (should)return(s) an array with the overlapping points.
   */
  std::vector<Vector>
  GetOverlappingPaths(std::vector<Vector> Path_A, std::vector<Vector> Path_B);
  /*b
   * \brief Returns the Waypoint with a correct time so that the object will
   * travel at the set m_speed
   * \param destination The position vector of the new waypoint
   */
  Waypoint CalculateWaypoint (Vector destination);
  /*
   * Returns the time to travel between two points
   */
  Time TravelTime (Vector v1, Vector v2);
  /*
   * Returns the time to travel from the current position of the mobile
   * to the RendezvousPoint rp
   */
  Time TravelTime (RendezvousPoint* rp);
  /*
   * Adds a waypoint to the underlying WaypointMobilityModel
   */
  void AddWaypoint (const Waypoint& wpt);
  /**
   * Callback for when the WaypointMobilityModel has a course change
   */
  void CourseChange (Ptr<const MobilityModel> model);
  /**
   * Adds waypoints along the path to the next RendezvousPoint
   */
  void MoveNextRP ();
  /**
   * Check if we the connection will be clear of higher priority mobs until
   * we reach the next rp. Checks only mobs that are on connections
   * adjacent to the next rp
   */
  bool IsNextConnectionClearUntilPassed ();
  /**
   * Returns true if this MineMobilityModel has a higher priority than the other
   * MineMobilityModel.
   */
  bool IsPriorityLowerThan (MineMobilityModel* other);
  /**
     * Returns true if this MineMobilityModel has a higher priority than the other
     * MineMobilityModel.
     */
  bool IsPriorityHigherThan (MineMobilityModel* other);
  /**
   * Returns true if this MineMobilityModel has a higher priority than all
   * MineMobilityModels in the vector others.
   */
  bool IsPriorityHigherThan (std::vector<MineMobilityModel*> others);
  /**
   * Returns true if this mobile will reach rp before other
   * \param other The mobile that this mobile competes with
   * \param rp The RendezvousPoint that the mobiles are racing to
   */
  bool IsOtherTooSlow (MineMobilityModel* other, RendezvousPoint* rp);
  /**
   * Returns true if this mobile and other mobile want to use this mobile's
   * next connection in different directions "soon".
   */
  bool IsGoingToCollideSoon (MineMobilityModel* other);
  /**
   * Returns true if there is a mobile with higher priority than this mobile
   * waiting for this mobile's next connection to clear.
   */
  bool IsHigherPriorityWaiting ();

  /**
   * \brief Converts time relative to simulation start into time relative to
   * current simulation time (the form that Simulator::Schedule expects).
   * \param t The time relative to simulation start
   */
  Time time2timeleft (Time t);
  /**
   * \brief std::vector containing ns3::Vector objects that describe that
   * path that will be followed
   */
  std::vector<RendezvousPoint*> m_path;
  uint32_t m_next_rendezvous_point;
  Waypoint m_last_waypoint;
  double m_speed;
  Ptr<WaypointMobilityModel> m_waypointMobility;
  uint32_t m_priority;
  /**
   * False if we are already at the RendezvousPoint we are scheduled to reach.
   * This will happen when we stop to wait for a busy connection.
   */
  bool m_new_rp;
  /**
   * Used to alert subscribers that a rendezvous point was reached.
   */
  ns3::TracedCallback<Ptr<const MineMobilityModel> > m_rendezvousTrace;
};
} // namespace ns3

#endif /* MINE_MOBILITY_MODEL_H */
