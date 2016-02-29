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
#include "ns3/stats-module.h"
#include "ns3/vector.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/string.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("trace-rendezvous");

void
//OutputSpeed (Ptr <const MineMobilityModel> mob)
OutputSpeed (double oldValue, double newValue)
{
  NS_LOG_UNCOND ("speedprobe " << oldValue << ", "<< newValue << " =======================================");
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

  pts.DisplayMinePaths ();

  AnimationInterface anim ("trace-rendezvous.xml"); //change this!


  //-------------------------------------------
  Ptr<MobilityModel> mob;
  for (uint32_t i = 0; i < node_count; i++){
      mob = mobileWifiNodes.Get (i)->GetObject<MobilityModel> ();
      Ptr<SpeedProbe> speedProbe = CreateObject<SpeedProbe> ();
      bool connected = speedProbe->ConnectByObject ("CourseChange", mob);
      if (!connected)
	{
	  NS_FATAL_ERROR ("Failed to connect SpeedProbe to a MineMobilityModel.");
	}
      mob->AggregateObject(speedProbe);
  }

//  "NodeList/*/$ns3::MineMobilityModel/Rendezvous"
//  $ns3::MineMobilityModel/
  Config::ConnectWithoutContext ("/NodeList/*/$ns3::SpeedProbe/OutputSpeed",
				MakeCallback (&OutputSpeed));
  // Use GnuplotHelper to plot the speed over time
  GnuplotHelper plotHelper;

  // Configure the plot.  The first argument is the file name prefix
  // for the output files generated.  The second, third, and fourth
  // arguments are, respectively, the plot title, x-axis, and y-axis labels
  plotHelper.ConfigurePlot ("plot-example",
                            "Speed vs. Time",
                            "Time (Seconds)",
                            "Speed (Meters per second)");

  std::string probeType;
  std::string tracePath;
  probeType = "ns3::DoubleProbe";
  tracePath = "/NodeList/*/$ns3::SpeedProbe/OutputSpeed";

  // Specify the probe type, trace source path (in configuration namespace), and
  // probe output trace source ("Output") to plot.  The fourth argument
  // specifies the name of the data series label on the plot.  The last
  // argument formats the plot by specifying where the key should be placed.
  plotHelper.PlotProbe (probeType,
                        tracePath,
                        "Output",
                        "Speed",
                        GnuplotAggregator::KEY_BELOW);
  //-------------------------------------------

  Simulator::Run ();
  Simulator::Destroy ();
}
