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

#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/string.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Waypoints");

int 
main (int argc, char *argv[])
{
  NodeContainer mobileWifiNodes;
  mobileWifiNodes.Create (); //nr of nodes
  MineMobilityPaths pts;


  RendezvousPoint pointA(Vector(0.0,500.0,0.0));
  RendezvousPoint pointB(Vector(500.0,500.0,0.0));
  RendezvousPoint pointC(Vector(500.0,0.0,0.0));

  std::vector<Vector> a_to_b;
  a_to_b.push_back(Vector(100,490,0));
  a_to_b.push_back(Vector(200,510,0));
  a_to_b.push_back(Vector(300,490,0));
  a_to_b.push_back(Vector(400,510,0));
  pointA.Connect(&pointB, a_to_b);

  std::vector<Vector> b_to_c;
  b_to_c.push_back(Vector(490,400,0));
  b_to_c.push_back(Vector(510,300,0));
  b_to_c.push_back(Vector(490,200,0));
  b_to_c.push_back(Vector(510,100,0));
  pointB.Connect(&pointC, b_to_c);

  std::vector<RendezvousPoint*> pathABC;
  pathABC.push_back(&pointA);
  pathABC.push_back(&pointB);
  pathABC.push_back(&pointC);

  std::vector<RendezvousPoint*> pathCBA;
  pathCBA.push_back(&pointC);
  pathCBA.push_back(&pointB);
  pathCBA.push_back(&pointA);


  NS_LOG_UNCOND("AB " << &pointA.GetConnectionTo(&pointB));
  NS_LOG_UNCOND("BC " << &pointB.GetConnectionTo(&pointC));

  NS_LOG_UNCOND("BA " << &pointB.GetConnectionTo(&pointA));
  NS_LOG_UNCOND("CB " << &pointC.GetConnectionTo(&pointB));


  //node 0
  Ptr<MineMobilityModel> minemob = CreateObjectWithAttributes<MineMobilityModel>("Speed", DoubleValue(10), "Priority", IntegerValue(0));
  minemob->SetPath(pts.pathABC); //set path
  Ptr<MobilityModel> model = minemob->GetObject<MobilityModel>();
  Ptr<Object> object = mobileWifiNodes.Get(0);
  object->AggregateObject(minemob);

  //node 1
  minemob = CreateObjectWithAttributes<MineMobilityModel>("Speed", DoubleValue(5), "Priority", IntegerValue(1));
  minemob->SetPath(pts.pathCBA);
  model = minemob->GetObject<MobilityModel>();
  object = mobileWifiNodes.Get(1);
  object->AggregateObject(minemob);

  AnimationInterface anim ("mine_test.xml"); //change this!

  Simulator::Run ();
  Simulator::Destroy ();
}
