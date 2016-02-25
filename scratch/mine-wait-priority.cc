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
  mobileWifiNodes.Create (3); //nr of nodes
  MineMobilityPaths pts;

/*
  NS_LOG_UNCOND("AB " << &pointA.GetConnectionTo(&pointB));
  NS_LOG_UNCOND("BC " << &pointB.GetConnectionTo(&pointC));

  NS_LOG_UNCOND("BA " << &pointB.GetConnectionTo(&pointA));
  NS_LOG_UNCOND("CB " << &pointC.GetConnectionTo(&pointB));
*/

  //node 0
  Ptr<MineMobilityModel> minemob = CreateObjectWithAttributes<MineMobilityModel>("Speed", DoubleValue(10), "Priority", IntegerValue(5));
  minemob->SetPath(pts.pathFH); //set path
  Ptr<MobilityModel> model = minemob->GetObject<MobilityModel>();
  Ptr<Object> object = mobileWifiNodes.Get(0);
  object->AggregateObject(minemob);

  //node 1
  minemob = CreateObjectWithAttributes<MineMobilityModel>("Speed", DoubleValue(5), "Priority", IntegerValue(10));
  minemob->SetPath(pts.pathHF);
  model = minemob->GetObject<MobilityModel>();
  object = mobileWifiNodes.Get(1);
  object->AggregateObject(minemob);

  //node 2
  minemob = CreateObjectWithAttributes<MineMobilityModel>("Speed", DoubleValue(4.6), "Priority", IntegerValue(0));
  minemob->SetPath(pts.pathHF);
  model = minemob->GetObject<MobilityModel>();
  object = mobileWifiNodes.Get(2);
  object->AggregateObject(minemob);

  AnimationInterface anim ("mine_test.xml"); //change this!

  Simulator::Run ();
  Simulator::Destroy ();
}
