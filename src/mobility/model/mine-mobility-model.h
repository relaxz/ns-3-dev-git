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
   * Set the path the object will follow and move the object to the start of the path
   */
  void SetPath (const std::vector<RendezvousPoint> &path);
  MineMobilityModel ();
  virtual ~MineMobilityModel ();
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
  /**
   * \brief The double ended queue containing ns3::Vector objects that describe that path that will be followed
   */
  mutable std::vector<Vector> m_path;
  mutable double m_speed;
  Ptr<WaypointMobilityModel> m_waypointMobility;
  uint32_t m_priority;
};
} // namespace ns3

#endif /* MINE_MOBILITY_MODEL_H */
