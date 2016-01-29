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
#include "ns3/mobility-module.h"
#include "ns3/netanim-module.h"
#include "rendezvous-point.h"
namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("rendezvous-point");

//const std::vector<Vector>& Connection::GetPoints(void) {
//  return waypoints;
//}

//const RendezvousPoint& Connection::GetTarget() {
//  return target;
//}

//RendezvousPoint::RendezvousPoint(std::vector<double> position, std::vector<Connection> connectionlist) {
//  m_pos = position;
//  m_connections = connectionlist;
//}

//Connection RendezvousPoint::GetConnection(RendezvousPoint rp) {
//  for(int i=0; i < m_connections.size();i++) {
//      if(m_connections[i].GetTarget() == rp) {
//	  return m_connections[i];
//      }
//  }
//  return NULL;
//}

//std::vector<Vector> RendezvousPoint::GetConnectionPoints(RendezvousPoint rp) {
//  return GetConnection(rp).GetPoints();
//}
}







