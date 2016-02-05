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
#include "ns3/vector.h"
#include "ns3/mobility-module.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Waypoints");

int 
main (int argc, char *argv[])
{
  NS_LOG_UNCOND ("Waypoint Simulator");



  RendezvousPoint pointA(Vector(0.0,500.0,0.0));
  RendezvousPoint pointB(Vector(500.0,500.0,0.0));
  RendezvousPoint pointC(Vector(500.0,0.0,0.0));
  std::vector<Vector> vectorer;

  for(int i=0;i<=500;i+=100){
	  vectorer.push_back(Vector((double(i)),0.0,0.0));
	}
  for(int i=0; i<(int)vectorer.size();++i){
	  std::cout << "\n";
	  std::cout << vectorer[i].x;}


  pointA.Connect(&pointB,vectorer);



  Simulator::Run ();
  Simulator::Destroy ();
}
