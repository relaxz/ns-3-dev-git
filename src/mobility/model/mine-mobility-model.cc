/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/double.h"
#include "ns3/integer.h"
#include "ns3/object-factory.h"
#include "mine-mobility-model.h"

namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("MineMobilityModel");

NS_OBJECT_ENSURE_REGISTERED (MineMobilityModel);

TypeId
MineMobilityModel::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MineMobilityModel")
    .SetParent<MobilityModel> ()
    .SetGroupName ("Mobility")
    .AddConstructor<MineMobilityModel> ()
    .AddAttribute ("Speed", "The speed this object will have when moving.",
                   DoubleValue (10.0),
                   MakeDoubleAccessor (&MineMobilityModel::m_speed),
		   MakeDoubleChecker<double>(0.0))
    .AddAttribute ("Priority", "The importance that the object does not "
		   "have to stop. High values lead to fewer stops.",
		   IntegerValue (0),
		   MakeIntegerAccessor (&MineMobilityModel::m_priority),
		   MakeIntegerChecker<uint32_t>())
  ;
  return tid;
}

void MineMobilityModel::SetPath (std::vector<RendezvousPoint> &path)
{
  /*m_path = path;
  std::vector<Vector> points = m_path[m_last_rendezvous_point].GetConnectionPoints(m_path[m_last_rendezvous_point + 1]);
  for (uint32_t i = 0; i < points.size(); i++){
      m_waypointMobility->AddWaypoint(CalculateWaypoint(points[i]));
  }*/

}

MineMobilityModel::MineMobilityModel ()
  : m_speed (10.0)
{
  m_priority = 0;
  m_last_rendezvous_point = 0;
  ObjectFactory mobilityFactory;
  mobilityFactory.SetTypeId ("ns3::WaypointMobilityModel");
  m_waypointMobility = mobilityFactory.Create ()->GetObject<WaypointMobilityModel>();
}
MineMobilityModel::~MineMobilityModel ()
{
}
void
MineMobilityModel::DoDispose (void)
{
  MobilityModel::DoDispose ();
}
Vector
MineMobilityModel::DoGetPosition (void) const
{
  return m_waypointMobility->GetPosition ();
}
void
MineMobilityModel::DoSetPosition (const Vector &position)
{
  //NS_FATAL_ERROR("MineMobilityModel::DoSetPosition not implemented");
  m_waypointMobility->SetPosition(position);
}
Vector
MineMobilityModel::DoGetVelocity (void) const
{
  return m_waypointMobility->GetVelocity ();
}

Waypoint
MineMobilityModel::CalculateWaypoint (Vector destination)
{
  Time arrival_time = TravelTime (m_last_waypoint.position, destination) + m_last_waypoint.time;
  return Waypoint (arrival_time, destination);
}

Time
MineMobilityModel::TravelTime(Vector v1, Vector v2)
{
  return Seconds(Distance(v1, v2) / m_speed);
}

double
MineMobilityModel::Distance(Vector v1, Vector v2)
{
  double dx = v1.x-v2.x;
  double dy = v1.y-v2.y;
  double dz = v1.z-v2.z;
  return sqrt(dx*dx + dy*dy + dz*dz);
}

void
MineMobilityModel::AddWaypoint(const Waypoint& wpt)
{
  m_last_waypoint = wpt;
  m_waypointMobility->AddWaypoint(wpt);
}

} // namespace ns3
