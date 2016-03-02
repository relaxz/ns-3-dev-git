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
Rendezvous (Ptr <const MineMobilityModel> mob)
{
  NS_LOG_UNCOND ("A RENDEZVOUS POINT WAS REACHED!!!!!");
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
//  Ptr<MineMobilityModel> mob;
//  for (uint32_t i = 0; i < node_count; i++){
//      mob = mobileWifiNodes.Get (i)->GetObject<MineMobilityModel> ();
//      mob->TraceConnectWithoutContext ("Rendezvous", MakeCallback (&Rendezvous));
//  }

  // this way is easier, and it seems to deal with situations where not all nodes have MineMobilityModels
  Config::ConnectWithoutContext ("/NodeList/*/$ns3::MineMobilityModel/Rendezvous",
				 MakeCallback (&Rendezvous));
  //-------------------------------------------

  Simulator::Run ();
  Simulator::Destroy ();
}
