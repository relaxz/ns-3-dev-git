/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/object.h"
#include "ns3/simulator.h"
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

void MineMobilityModel::SetPath (std::vector<RendezvousPoint*> &path)
{
  //m_first = true;
  m_next_rendezvous_point = 0;
  m_path = path;
  //AddWaypoint(Waypoint(Simulator::Now(), m_path[m_next_rendezvous_point]->GetPosition()));
  SetPosition(m_path[m_next_rendezvous_point]->GetPosition());
  Rendezvous();
}

void
MineMobilityModel::Rendezvous(){
  NS_LOG_UNCOND("Reached rendezvous point " << m_next_rendezvous_point);
  Vector pos = GetPosition();
  NS_LOG_UNCOND("position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")");
  //todo logic for deciding if we should stop and wait
  m_next_rendezvous_point++;
  //if we are out of places to go, just stop
  if (m_next_rendezvous_point >= m_path.size()){
      m_waypointMobility->EndMobility();
      NS_LOG_UNCOND("Reached destination.");
  }
  else{
      // The current position must be added as a waypoint again to prevent
      // teleportation to the first waypoint on the path
      AddWaypoint(Waypoint(Simulator::Now(), GetPosition()));
      std::vector<Vector> points = m_path[m_next_rendezvous_point - 1]->GetConnectionPoints(m_path[m_next_rendezvous_point]);
      for (uint32_t i = 0; i < points.size(); i++){
	  AddWaypoint(CalculateWaypoint(points[i]));
      }
      AddWaypoint(CalculateWaypoint(m_path[m_next_rendezvous_point]->GetPosition()));
      Simulator::Schedule(m_last_waypoint.time, &MineMobilityModel::Rendezvous, this);
  }
}

MineMobilityModel::MineMobilityModel ()
  : m_speed (10.0)
{
  m_priority = 0;
  m_next_rendezvous_point = 0;
  m_waypointMobility = CreateObjectWithAttributes<WaypointMobilityModel> ();
  // make couse changes in the waypoint mobility model triggers a course change in this model.
  m_waypointMobility->TraceConnectWithoutContext ("CourseChange", MakeCallback (&MineMobilityModel::CourseChange, this));
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
  //m_waypointMobility->SetPosition(position);
  AddWaypoint(Waypoint(Simulator::Now(), position));
}
Vector
MineMobilityModel::DoGetVelocity (void) const
{
  return m_waypointMobility->GetVelocity ();
}

Waypoint
MineMobilityModel::CalculateWaypoint (Vector destination)
{
  Time arrival_time;
  arrival_time = TravelTime (m_last_waypoint.position, destination) + m_last_waypoint.time;
  //NS_LOG_UNCOND("Travel time: " << (arrival_time - m_last_waypoint.time).GetSeconds());
  return Waypoint (arrival_time, destination);
}

Time
MineMobilityModel::TravelTime(Vector v1, Vector v2)
{
  //NS_LOG_UNCOND("TravelTime: v1=(" << v1.x << ", " << v1.y << ", " << v1.z << ")");
  //NS_LOG_UNCOND("TravelTime: v2=(" << v2.x << ", " << v2.y << ", " << v2.z << ")");
  return Seconds(CalculateDistance(v1, v2) / m_speed);
}

void
MineMobilityModel::AddWaypoint(const Waypoint& wpt)
{
  NS_LOG_UNCOND("AddWaypoint: wpt=(" << wpt.time.GetSeconds() << ", ("<< wpt.position.x << ", "
		<< wpt.position.y << ", " << wpt.position.z << "))");
  m_last_waypoint = wpt;
  m_waypointMobility->AddWaypoint(wpt);
}

void
MineMobilityModel::CourseChange(Ptr<const MobilityModel> model)
{
  MobilityModel::NotifyCourseChange ();
}

} // namespace ns3
