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
std::vector<Node*> Connection::GetNodes(void) {
  return m_nodes;
}

void Connection::AddNode(Node* node) {
  m_nodes.push_back(node);
}

void Connection::RemoveNode(Node* node) {
    for(unsigned int i=0; i<m_nodes.size(); i++) {
	if(m_nodes[i] == node) {
	    m_nodes.erase(m_nodes.begin() + i);
	}
    }
}


/*
 *=================================================================================================
 */

RendezvousPoint::RendezvousPoint(Vector position) {
  m_pos = position;
}

void RendezvousPoint::Connect(RendezvousPoint* rp, std::vector<Vector> waypoints) {
  Connection conn (rp, waypoints);
  m_connections.push_back(&conn);
  rp->Connect(this, ReverseVector(waypoints));
}

//Connection RendezvousPoint::GetConnection(RendezvousPoint* rp) { //detta behöver lösas
//  for(unsigned int i=0; i < m_connections.size();i++) {
//      if(m_connections[i]->GetTarget() == rp) {
//	  return m_connections[i];
//     }
//  }
//  return NULL;
//}

//std::vector<Vector> RendezvousPoint::GetConnectionPoints(RendezvousPoint* rp) {
//  return GetConnection(rp).GetPoints();
//}

Vector RendezvousPoint::GetPosition(void) {
  return m_pos;
}

std::vector<Vector> RendezvousPoint::ReverseVector(std::vector<Vector> vect) {
  std::vector<Vector> rev;
  unsigned int l = 0;
  for(unsigned int i=vect.size(); i >= 0; i--) {
      rev[l] = vect[i];
      l++;
  }
  return rev;
}

} //ns3







