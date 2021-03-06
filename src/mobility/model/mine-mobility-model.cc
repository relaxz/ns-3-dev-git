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

//todo replace UNCOND logging with other log levels
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
		   MakeDoubleChecker<double> (0.0))
    .AddAttribute ("Priority", "The importance that the object does not "
		   "have to stop. High values lead to fewer stops.",
		   IntegerValue (0),
		   MakeIntegerAccessor (&MineMobilityModel::m_priority),
		   MakeIntegerChecker<uint32_t> ())
    .AddTraceSource ("Rendezvous",
		     "A rendezvous point was reached.",
		     MakeTraceSourceAccessor (&MineMobilityModel::m_rendezvousTrace),
		     "ns3::MineMobilityModel::TracedCallback")
  ;
  return tid;
}

void
MineMobilityModel::SetPath (std::vector<RendezvousPoint*> &path)
{
  // Schedule it instead of calling it to prevent CourseChanges before
  // the simulation starts
  Simulator::Schedule (Time(0), &MineMobilityModel::DoSetPath, this, path);
}

void
MineMobilityModel::DoSetPath (std::vector<RendezvousPoint*> &path)
{
  m_next_rendezvous_point = 0;
  m_path = path;
  SetPosition (m_path[m_next_rendezvous_point]->GetPosition ());
  m_new_rp = true;
  Simulator::Schedule (Time(0), &MineMobilityModel::Rendezvous, this);
}

void
MineMobilityModel::Rendezvous (){
  NS_LOG_UNCOND ("-----------------------------------");
  NS_LOG_UNCOND (this << ": Priority=" << m_priority);
  NS_LOG_UNCOND (this << ": Reached rendezvous point " << m_next_rendezvous_point <<  " at " << Simulator::Now().GetSeconds() << " s");
  Vector pos = GetPosition ();
  NS_LOG_UNCOND ("position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")");

  RendezvousPoint* rp_current = m_path[m_next_rendezvous_point];
  RendezvousPoint* rp_target = m_path[m_next_rendezvous_point + 1];

  if (m_new_rp)
    {
      // notify listeners
      m_rendezvousTrace (this);
      m_new_rp = false;
    }
  // Decide if we need to wait at this rp
  // If we are out of places to go, just stop
  if (m_next_rendezvous_point >= m_path.size () - 1)
    {
      m_waypointMobility->EndMobility ();
      NS_LOG_UNCOND ("Reached destination.");
    }
  // check if the connection is clear
  else if (rp_current->IsConnectionBusyFrom(rp_target))
    {
      NS_LOG_UNCOND(this << ": Stop and wait, path to next rp is in use");
      std::vector<MineMobilityModel*> others = rp_current->GetApproachingMobilesFrom (rp_target);
      //NS_ASSERT (!IsPriorityHigherThan (others));
      if (IsPriorityHigherThan(others))
	{
	  NS_LOG_UNCOND("High priority mobile is forced to wait for low priority mobile!");
	}
      Time timeleft = rp_current->GetTimeLeftUntilClear (rp_target);
      // list us as waiting
      rp_current->AddWaitingMobile (this, timeleft + Simulator::Now ());
      // Try again later
      Simulator::Schedule (timeleft,
			   &MineMobilityModel::Rendezvous,
			   this);
    }
  else if (IsHigherPriorityWaiting ())
    {
      NS_LOG_UNCOND ("Stop and wait for higher priority mobiles that are waiting on the other side");
      Time timeleft = rp_current->GetTimeLeftUntilClear (rp_target);
      // List us as waiting
      rp_current->AddWaitingMobile (this, timeleft + Simulator::Now ());
      // Try again later
      Simulator::Schedule (timeleft,
			   &MineMobilityModel::Rendezvous,
			   this);
    }
  // check if the connection will be clear of higher priority mobs until we reach the next rp
  // by checking the nodes approaching next rp
  else if (!IsNextConnectionClearUntilPassed ())
    {
      NS_LOG_UNCOND ("Stop and wait for higher priority mobiles");
      Time timeleft = TravelTime (rp_target);
      // List us as waiting
      rp_current->AddWaitingMobile (this, timeleft + Simulator::Now ());
      // Try again later
      Simulator::Schedule (timeleft,
			   &MineMobilityModel::Rendezvous,
			   this);
    }
  else
    {
      // Continue along the path
      MoveNextRP ();
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
  AddWaypoint (Waypoint (Simulator::Now (), position));
}
Vector
MineMobilityModel::DoGetVelocity (void) const
{
  return m_waypointMobility->GetVelocity ();
}

Waypoint
MineMobilityModel::CalculateWaypoint(Vector destination)
{
  Time arrival_time;
  arrival_time = TravelTime (m_last_waypoint.position, destination) + m_last_waypoint.time;
  return Waypoint (arrival_time, destination);
}

Time
MineMobilityModel::TravelTime(Vector v1, Vector v2)
{
  return Seconds (CalculateDistance (v1, v2) / m_speed);
}

Time
MineMobilityModel::TravelTime (RendezvousPoint* rp){
  //return Seconds (GetDistance (rp)); //todo distance to rp function, waiting for ravve
  return Seconds (1);
}

void
MineMobilityModel::AddWaypoint (const Waypoint& wpt)
{
  m_last_waypoint = wpt;
  m_waypointMobility->AddWaypoint (wpt);
}

void
MineMobilityModel::CourseChange (Ptr<const MobilityModel> model)
{
  MobilityModel::NotifyCourseChange ();
}

void
MineMobilityModel::MoveNextRP (){
  m_next_rendezvous_point++;
  // The current position must be added as a waypoint again to prevent
  // teleportation to the first waypoint on the path
  AddWaypoint (Waypoint (Simulator::Now( ), GetPosition ()));
  std::vector<Vector> points = m_path[m_next_rendezvous_point - 1]->GetConnectionPoints (m_path[m_next_rendezvous_point]);
  for (uint32_t i = 0; i < points.size (); i++)
    {
      AddWaypoint (CalculateWaypoint (points[i]));
    }
  AddWaypoint (CalculateWaypoint (m_path[m_next_rendezvous_point]->GetPosition ()));
  Time timeleft = time2timeleft (m_last_waypoint.time);
  Simulator::Schedule (timeleft, &MineMobilityModel::Rendezvous, this);
  //list this mobile as traveling along the path
  m_path[m_next_rendezvous_point - 1]->GetConnectionTo (m_path[m_next_rendezvous_point]).AddMobile (this, m_last_waypoint.time);
  m_new_rp = true;
}

bool
MineMobilityModel::IsNextConnectionClearUntilPassed (){
  // it is assumed that we are at an rp but have not incremented m_next_rendezvous_point yet
  RendezvousPoint* rp_current = m_path[m_next_rendezvous_point];
  RendezvousPoint* rp_target = m_path[m_next_rendezvous_point + 1];
  std::vector<MineMobilityModel*> candidate_collisions = rp_target->GetApproachingMobilesExceptFrom (rp_current);
  for (uint32_t i = 0; i < candidate_collisions.size(); i++)
    {
      MineMobilityModel* other = candidate_collisions[i];
      if (IsPriorityLowerThan (other)
	  && IsGoingToCollideSoon (other)
	  && !IsOtherTooSlow (other, rp_target))
	{
	  return false;
	}
    }
  return true;
}

bool
MineMobilityModel::IsPriorityLowerThan (MineMobilityModel* other){
  return m_priority < other->m_priority;
}

bool
MineMobilityModel::IsPriorityHigherThan (MineMobilityModel* other){
  return m_priority > other->m_priority;
}

bool
MineMobilityModel::IsPriorityHigherThan (std::vector<MineMobilityModel*> others){
  for (uint32_t i = 0; i < others.size (); i++)
    {
    if (IsPriorityHigherThan (others[i]))
      {
	return true;
    }
  }
  return false;
}

bool
MineMobilityModel::IsOtherTooSlow (MineMobilityModel* other, RendezvousPoint* rp){
  return TravelTime (rp) < other->TravelTime (rp);
}

bool
MineMobilityModel::IsGoingToCollideSoon (MineMobilityModel* other){
  // if other is going to stop, it will not collide with this
  if (other->m_next_rendezvous_point == other->m_path.size () - 1)
    {
      return false;
    }
  // if this mob is at rp A and is going to rp B next,
  // then we check if other is going to B then A
  // if this is true, then they are going to collide soon
  return other->m_path[other->m_next_rendezvous_point] == m_path[m_next_rendezvous_point + 1]
         && other->m_path[other->m_next_rendezvous_point + 1] == m_path[m_next_rendezvous_point];
}

bool
MineMobilityModel::IsHigherPriorityWaiting ()
{
  std::vector<MineMobilityModel*> mobs;
  mobs = m_path[m_next_rendezvous_point + 1]->GetWaitingMobiles ();
//  NS_LOG_UNCOND("IsHigherPriorityWaiting (rp " << m_path[m_next_rendezvous_point + 1] << ") mobs.size()=" << mobs.size());
//  NS_LOG_UNCOND(m_path[m_next_rendezvous_point] << " " << m_path[m_next_rendezvous_point + 1] << " " << m_path[m_next_rendezvous_point + 2]
//		<< " " << m_path[m_next_rendezvous_point + 3]);
  for (uint32_t i = 0; i < mobs.size (); i++)
    {
      if (IsPriorityLowerThan (mobs[i])
	  && IsGoingToCollideSoon (mobs[i]))
	{
	  return true;
	}
    }
  return false;
}

Time
MineMobilityModel::time2timeleft (Time t){
  return t - Simulator::Now ();
}

} // namespace ns3
