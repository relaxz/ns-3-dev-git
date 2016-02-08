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

  RendezvousPoint pointA(Vector(0.0,500.0,0.0));
  RendezvousPoint pointB(Vector(500.0,500.0,0.0));
  RendezvousPoint pointC(Vector(500.0,0.0,0.0));

  std::vector<Vector> a_to_b;
  a_to_b.push_back(Vector(100,450,0));
  a_to_b.push_back(Vector(200,550,0));
  a_to_b.push_back(Vector(300,450,0));
  a_to_b.push_back(Vector(400,550,0));
  pointA.Connect(&pointB, a_to_b);

  std::vector<Vector> b_to_c;
  b_to_c.push_back(Vector(450,400,0));
  b_to_c.push_back(Vector(550,300,0));
  b_to_c.push_back(Vector(450,200,0));
  b_to_c.push_back(Vector(550,100,0));
  pointB.Connect(&pointC, b_to_c);

  std::vector<RendezvousPoint*> pathABC;
  pathABC.push_back(&pointA);
  pathABC.push_back(&pointB);
  pathABC.push_back(&pointC);

  std::vector<RendezvousPoint*> pathCBA;
  pathCBA.push_back(&pointC);
  pathCBA.push_back(&pointB);
  pathCBA.push_back(&pointA);

  NodeContainer mobileWifiNodes;
  mobileWifiNodes.Create (1);
  Ptr<MineMobilityModel> minemob = CreateObjectWithAttributes<MineMobilityModel>("Speed", DoubleValue(100), "Priority", IntegerValue(0));
  minemob->SetPath(pathABC);
  Ptr<MobilityModel> model = minemob->GetObject<MobilityModel>();
  Ptr<Object> object = mobileWifiNodes.Get(0);
  object->AggregateObject(model);

  AnimationInterface anim ("mine_test.xml");

  Simulator::Run ();
  Simulator::Destroy ();
}
