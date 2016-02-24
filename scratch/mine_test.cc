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
  MineMobilityHelper mHelper;
  mHelper.Install (mobileWifiNodes.Get (0), pts.pathFH, mHelper.CAR);
  mHelper.Install (mobileWifiNodes.Get (1), pts.pathIG, mHelper.DUMPER);
  mHelper.Install (mobileWifiNodes.Get (2), pts.pathGI, mHelper.LOADER);
  //mHelper.Install (mobileWifiNodes.Get (3), pts.pathHF, 7, 1);

  pts.DisplayMinePaths ();

  AnimationInterface anim ("mine_test.xml"); //change this!

  Simulator::Run ();
  Simulator::Destroy ();
}
