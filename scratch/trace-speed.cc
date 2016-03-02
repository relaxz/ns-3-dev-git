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

NS_LOG_COMPONENT_DEFINE ("trace-rendezvous");

void
Rendezvous (Ptr <const MobilityModel> mob)
{
  NS_LOG_UNCOND ("A RENDEZVOUS POINT WAS REACHED!!!!!");

  uint32_t nodeID = mob->GetObject<Node> ()->GetId ();

  std::stringstream ss_nodeID;
  ss_nodeID << nodeID;
  std::string str_nodeID;
  ss_nodeID >> str_nodeID;
/*
  str_nodeID = str_nodeID + " qwerty";
  NS_LOG_UNCOND (str_nodeID);
*/
  std::string outputfile = str_nodeID + ".dat"; //name of file to write to
  //NS_LOG_UNCOND (outputfile);

  /* "0123QWERTYUIOP_" +*/
  Vector velocity = mob->GetVelocity();
  double speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2));
  Time time = Simulator::Now();

  std::ofstream myfile;
  myfile.open (outputfile.c_str(), std::ios_base::app);
  myfile << time.GetSeconds() << '\t' << speed << '\n';
  myfile.close();

}

int
main (int argc, char *argv[])
{
  NodeContainer mobileWifiNodes;
  uint32_t node_count = 3;
  mobileWifiNodes.Create (node_count); //nr of nodes
  MineMobilityPaths pts;
  MineMobilityHelper mHelper;
  mHelper.Install (mobileWifiNodes.Get (0), pts.pathFH, mHelper.CAR);
  mHelper.Install (mobileWifiNodes.Get (1), pts.pathIG, mHelper.DUMPER);
  mHelper.Install (mobileWifiNodes.Get (2), pts.pathGI, mHelper.LOADER);
  //mHelper.Install (mobileWifiNodes.Get (3), pts.pathHF, 7, 1);

  pts.DisplayMinePaths ();

  AnimationInterface anim ("trace-rendezvous.xml"); //change this!


  //-------------------------------------------
  Ptr<MineMobilityModel> mob;
  for (uint32_t i = 0; i < node_count; i++){
      mob = mobileWifiNodes.Get (i)->GetObject<MineMobilityModel> ();
      mob->TraceConnectWithoutContext ("CourseChange", MakeCallback (&Rendezvous));
  }
  //-------------------------------------------

  Simulator::Run ();
  Simulator::Destroy ();
}
