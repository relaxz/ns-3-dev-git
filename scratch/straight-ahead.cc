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
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("StraightAhead");

int 
main (int argc, char *argv[])
{
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (1);

  ObjectFactory mobilityFactory;
  mobilityFactory.SetTypeId ("ns3::WaypointMobilityModel");
  mobilityFactory.Set ("LazyNotify", BooleanValue (false));
  Ptr<MobilityModel> model = mobilityFactory.Create ()->GetObject<MobilityModel> ();

  Waypoint wpt1 (Seconds (0.0), Vector (0.0, 5.0, 0.0));
  Waypoint wpt2 (Seconds (5.0), Vector (10.0, 5.0, 0.0));

  Ptr<WaypointMobilityModel> mob = model->GetObject<WaypointMobilityModel> ();
  mob->AddWaypoint (wpt1);
  mob->AddWaypoint (wpt2);

  Ptr<Object> object = wifiStaNodes.Get(0);
  object->AggregateObject(model);

  AnimationInterface anim ("straight-ahead.xml");

  Simulator::Stop (Seconds (15.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
