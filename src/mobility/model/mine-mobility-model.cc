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
  NS_LOG_UNCOND("Reached rendezvous point " << m_next_rendezvous_point <<  " at " << Simulator::Now().GetSeconds() << " s");
  Vector pos = GetPosition();
  NS_LOG_UNCOND("position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")");
  // Decide if we need to wait at this rp
  // If we are out of places to go, just stop
  if (m_next_rendezvous_point >= m_path.size() - 1){
      m_waypointMobility->EndMobility();
      NS_LOG_UNCOND("Reached destination.");
  }
  // check if the path is clear
  //todo also check if we the path will be clear of higher priority mobs until we reach the next rp
  //todo look ahead beyond the next rp
  else if (m_path[m_next_rendezvous_point]->IsConnectionBusyFrom(m_path[m_next_rendezvous_point + 1]))
    {
      NS_LOG_UNCOND("Stop and wait, path to next rp is in use");
      // Try again later
      Time timeleft = m_path[m_next_rendezvous_point]->GetTimeLeftUntilClear(m_path[m_next_rendezvous_point]);
      Simulator::Schedule(timeleft,
			  &MineMobilityModel::Rendezvous,
			  this);
    }
  else{

      // Continue along the path
      MoveNextRP();
  }
}

MineMobilityModel::MineMobilityModel ()
  : m_speed (10.0)
{
  m_priority = 0;
  m_next_rendezvous_point = 0;
  m_waypointMobility = CreateObjectWithAttributes<WaypointMobilityModel> ();
  // make course changes in the waypoint mobility model trigger a course change in this model.
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
  m_last_waypoint = wpt;
  m_waypointMobility->AddWaypoint(wpt);
}

void
MineMobilityModel::CourseChange(Ptr<const MobilityModel> model)
{
  MobilityModel::NotifyCourseChange ();
}

void
MineMobilityModel::MoveNextRP(){
  m_next_rendezvous_point++;
  // The current position must be added as a waypoint again to prevent
  // teleportation to the first waypoint on the path
  AddWaypoint(Waypoint(Simulator::Now(), GetPosition()));
  std::vector<Vector> points = m_path[m_next_rendezvous_point - 1]->GetConnectionPoints(m_path[m_next_rendezvous_point]);
  for (uint32_t i = 0; i < points.size(); i++){
      AddWaypoint(CalculateWaypoint(points[i]));
  }
  AddWaypoint(CalculateWaypoint(m_path[m_next_rendezvous_point]->GetPosition()));
  Time timeleft = time2timeleft(m_last_waypoint.time);
  Simulator::Schedule(timeleft, &MineMobilityModel::Rendezvous, this);
  //list this mobile as traveling along the path
  m_path[m_next_rendezvous_point - 1]->GetConnectionTo(m_path[m_next_rendezvous_point]).AddMobile(this, m_last_waypoint.time);
}

Time
MineMobilityModel::time2timeleft(Time t){
  return t - Simulator::Now();
}

} // namespace ns3
