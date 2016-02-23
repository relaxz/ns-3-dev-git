/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/double.h"
#include "ns3/integer.h"
#include "rendezvous-point.h"
#include <algorithm>
#include <vector>
namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("rendezvous-point");

Connection::Connection(RendezvousPoint* target, std::vector<Vector> waypoints){
  m_target = target;
  m_waypoints = waypoints;
}

std::vector<Vector> Connection::GetPoints(void){
  return m_waypoints;
}

RendezvousPoint* Connection::GetTarget(void) {
  return m_target;
}

/* keeping this for reference until RemoveMobile is tested
void Connection::RemoveNode(Node* node) {
  for(unsigned int i=0; i<m_nodes.size(); i++) {
      if(m_nodes[i] == node) {
	  m_nodes.erase(m_nodes.begin() + i);
      }
  }
}
*/

std::vector<MineMobilityModel*>
Connection::GetMobiles(){
  return m_mobiles;
}

void
Connection::AddMobile(MineMobilityModel* mob, Time arrival_time){
  NS_LOG_UNCOND("AddMobile " << mob << " to connection " << this);
  m_mobiles.push_back(mob);
  Simulator::Schedule(arrival_time - Simulator::Now(), &Connection::RemoveMobile, this, mob);
}

void
Connection::RemoveMobile(MineMobilityModel* mob){
  m_mobiles.erase(std::remove(m_mobiles.begin(), m_mobiles.end(), mob), m_mobiles.end());
}


/*
 *=================================================================================================
 */

RendezvousPoint::RendezvousPoint(Vector position) {
  m_pos = position;
}
void
RendezvousPoint::ConnectOneWay(RendezvousPoint* rp, std::vector<Vector> waypoints) {
  Connection conn (rp, waypoints);
  m_connections.push_back(conn);
}

void
RendezvousPoint::Connect(RendezvousPoint* rp, std::vector<Vector> waypoints) {
  ConnectOneWay(rp,waypoints);
  rp->ConnectOneWay(this, ReverseVector(waypoints));
}

Connection&
RendezvousPoint::GetConnectionTo(RendezvousPoint* rp) {
  for(uint32_t i=0; i < m_connections.size();i++) {
      if(m_connections[i].GetTarget() == rp) {
	  return m_connections[i];
      }
  }
  Vector rpp = rp->GetPosition();
  Vector c0p = m_connections[0].GetTarget()->GetPosition();
  NS_LOG_UNCOND("requested rp: (" << rpp.x << "," << rpp.y << "," << rpp.z << ")"
              "\nfound rp:     (" << c0p.x << "," << c0p.y << "," << c0p.z << ")\n");
  NS_FATAL_ERROR("Connection not found.");
  //return NULL;
}

Connection&
RendezvousPoint::GetConnectionFrom(RendezvousPoint* rp){
  return rp->GetConnectionTo(this);
}

std::vector<Vector>
RendezvousPoint::GetConnectionPoints(RendezvousPoint* rp) {
  return GetConnectionTo(rp).GetPoints();
}


std::vector<MineMobilityModel*>
RendezvousPoint::GetApproachingMobilesFrom(RendezvousPoint* rp){
  NS_LOG_UNCOND("ApproachingMobilesFrom " << &GetConnectionFrom(rp) << ": " << GetConnectionFrom(rp).GetMobiles().size());
  return GetConnectionFrom(rp).GetMobiles();
}

std::vector<MineMobilityModel*>
RendezvousPoint::GetAllApproachingMobiles(){
  std::vector<MineMobilityModel*> all_mobs;
  for(uint32_t i; i<m_connections.size(); i++){
      std::vector<MineMobilityModel*> mobs = m_connections[i].GetMobiles();
      for(uint32_t j; j<mobs.size(); j++){
	  all_mobs.push_back(mobs[j]);
      }
  }
  return all_mobs;
}

//fixme looks like this never finds any mobiles :(
std::vector<MineMobilityModel*>
RendezvousPoint::GetApproachingMobilesExceptFrom(RendezvousPoint* rp){
  std::vector<MineMobilityModel*> result_mobs;
  for(uint32_t i = 0; i<m_connections.size(); i++)
    {
      RendezvousPoint* target = m_connections[i].GetTarget();
      if (target != rp)
	{
	  std::vector<MineMobilityModel*> mobs = GetApproachingMobilesFrom(target);
	  for(uint32_t j = 0; j<mobs.size(); j++){
	      result_mobs.push_back(mobs[j]);
	  }
	}
    }
  NS_LOG_UNCOND("ApproachingMobiles: " << result_mobs.size() << " mobile(s) approaching");
  return result_mobs;
}

bool
RendezvousPoint::IsConnectionBusyFrom(RendezvousPoint* rp){
  Connection conn = GetConnectionFrom(rp);
  return conn.GetMobiles().size() > 0;
}

Time
RendezvousPoint::GetTimeLeftUntilClear(RendezvousPoint* rp){
  //todo give an accurate response
  return Seconds(1);
}

Vector RendezvousPoint::GetPosition(void) {
  return m_pos;
}

void
RendezvousPoint::AddWaitingMobile(MineMobilityModel* mob, Time arrival_time){
  NS_LOG_UNCOND("AddWaitingMobile " << mob << " to rp " << this);
  m_waiting_mobiles.push_back(mob);
  Simulator::Schedule(arrival_time - Simulator::Now(), &RendezvousPoint::RemoveWaitingMobile, this, mob);
}

void
RendezvousPoint::RemoveWaitingMobile(MineMobilityModel* mob){
  NS_LOG_UNCOND("RemoveWaitingMobile " << mob << " from rp " << this);
  m_waiting_mobiles.erase(std::remove(m_waiting_mobiles.begin(), m_waiting_mobiles.end(), mob), m_waiting_mobiles.end());
}

std::vector<MineMobilityModel*>
RendezvousPoint::GetWaitingMobiles ()
{
  return m_waiting_mobiles;
}

std::vector<Vector> RendezvousPoint::ReverseVector(std::vector<Vector> vect) {
  std::vector<Vector> rev;
  uint32_t l = vect.size();
  for(uint32_t i=0; i < l; i++){
	  rev.push_back(vect[l - 1 - i]);
  }
  return rev;
}

bool
RendezvousPoint::operator== (RendezvousPoint & o){
  Vector o_pos = o.GetPosition();
  return m_pos.x == o_pos.x && m_pos.y == o_pos.y && m_pos.z == o_pos.z;
}

} //ns3







