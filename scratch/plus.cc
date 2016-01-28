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
#include <iostream>
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Plus");
std::string outputfile = "test.txt"; //name of file to write to

class RendezvousPoint
{
  //construct
  public:
    RendezvousPoint(Vector pos);
    Vector pos = pos;
    Vector paths;
    Vector nodes;

  void setPath(Vector path, RendezvousPoint rp) {

  }
};








/*
 * Logs and trace upon update in course ie when node reaches checkpoint
 * NodeID, Time, (x,y,z)-pos, Speed
 */
void
CourseChange (std::string context, Ptr<const MobilityModel> model)
{
  Vector position = model->GetPosition ();
  Vector velocity = model->GetVelocity();
  double speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));
  Time time = Simulator::Now();

  std::string str = context; //extracts node ID
  str.erase(0,10);
  int index = str.find_first_of("/");
  str.erase(index, str.size());


  //logs data (console)
  NS_LOG_UNCOND ("NodeID = " << str << ", Time: " << time.GetSeconds() <<
    ", x = " << position.x << ", y = " << position.y << ", z = " << position.z <<
    ", Speed: " << speed);

  std::ofstream myfile;	//puts trace to file
  myfile.open (outputfile.c_str(), std::ios_base::app);
  myfile << "NodeID = " << str << ", Time: " << time.GetSeconds() <<
      ", x = " << position.x << ", y = " << position.y << ", z = " << position.z <<
      ", Speed: " << speed << "\n";
  myfile.close();

}

int 
main (int argc, char *argv[])
{
  remove(outputfile.c_str()); //Removes tracefile duplicate
  int NodeCount = 2;
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (NodeCount);

  ObjectFactory mobilityFactory;
  mobilityFactory.SetTypeId ("ns3::WaypointMobilityModel");
  mobilityFactory.Set ("LazyNotify", BooleanValue (false));
  Ptr<MobilityModel> model = mobilityFactory.Create ()->GetObject<MobilityModel> ();

  Waypoint wpt1 (Seconds (0.0), Vector (0.0, 5.0, 0.0));
  Waypoint wpt2 (Seconds (5.0), Vector (10.0, 5.0, 1.0));

  Ptr<WaypointMobilityModel> mob = model->GetObject<WaypointMobilityModel> ();
  mob->AddWaypoint (wpt1);
  mob->AddWaypoint (wpt2);

  Ptr<Object> object = wifiStaNodes.Get(0);
  object->AggregateObject(model);

  // second node
  model = mobilityFactory.Create ()->GetObject<MobilityModel> ();
  mob = model->GetObject<WaypointMobilityModel> ();
  Waypoint wpt3 (Seconds (0.0), Vector (10.0, 5.0, 0.0));
  Waypoint wpt4 (Seconds (5.0), Vector (0.0, 5.0, 0.0));
  Waypoint wpt5 (Seconds (10.0), Vector (5.0, 5.0, 0.0));
  mob->AddWaypoint (wpt3);
  mob->AddWaypoint (wpt4);
  mob->AddWaypoint(wpt5);
  object = wifiStaNodes.Get(1);
  object->AggregateObject(model);

  for(int i=0; wifiStaNodes;i++) {

  }

  AnimationInterface anim ("plus.xml");

  Simulator::Stop (Seconds (15.0));

  //for every node add coursechange as listener
  for(int i=0; i<NodeCount; i++) {
      std::ostringstream oss;
      oss << "/NodeList/" << wifiStaNodes.Get (i)->GetId () << "/$ns3::MobilityModel/CourseChange";
      Config::Connect (oss.str (), MakeCallback (&CourseChange));

  }
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
