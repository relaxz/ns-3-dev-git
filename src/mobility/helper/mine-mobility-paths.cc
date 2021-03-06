#include "ns3/core-module.h"
#include "ns3/mine-mobility-paths.h"
#include "ns3/rendezvous-point.h"
#include "ns3/node-container.h"
#include "ns3/mine-mobility-helper.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
namespace ns3 {

NS_LOG_COMPONENT_DEFINE  ("mine-mobility-paths");

//defines RendezvousPoints  (globally because of unknown construct error)
RendezvousPoint pointA (Vector (1000.0,1500.0,0.0));
RendezvousPoint pointB (Vector (1500.0,1500.0,0.0));
RendezvousPoint pointC (Vector (1500.0,1000.0,0.0));
RendezvousPoint pointD (Vector (1750.0,1500.0,0.0));
RendezvousPoint pointE (Vector (1500.0,2000.0,0.0));
RendezvousPoint pointF (Vector (500.0,1500.0,0.0));
RendezvousPoint pointG (Vector (1500.0,250.0,0.0));
RendezvousPoint pointH (Vector (2250.0,1500.0,0.0));
RendezvousPoint pointI (Vector (1500.0,2500.0,0.0));


MineMobilityPaths::MineMobilityPaths  ()
{
  //defines connections
  a_to_b.push_back (Vector (1100,1490,0));
  a_to_b.push_back (Vector (1200,1510,0));
  a_to_b.push_back (Vector (1300,1490,0));
  a_to_b.push_back (Vector (1400,1510,0));
  pointA.Connect (&pointB, a_to_b);

  b_to_c.push_back (Vector (1490,1400,0));
  b_to_c.push_back (Vector (1510,1300,0));
  b_to_c.push_back (Vector (1490,1200,0));
  b_to_c.push_back (Vector (1510,1100,0));
  pointB.Connect (&pointC, b_to_c);

  b_to_d.push_back (Vector (1600.0,1500.0,0.0));
  b_to_d.push_back (Vector (1700.0,1500.0,0.0));
  pointB.Connect (&pointD, b_to_d);

  b_to_e.push_back (Vector (1500.0,1600.0,0.0));
  b_to_e.push_back (Vector (1500.0,1700.0,0.0));
  b_to_e.push_back (Vector (1500.0,1800.0,0.0));
  b_to_e.push_back (Vector (1500.0,1900.0,0.0));
  pointB.Connect (&pointE, b_to_e);

  a_to_f.push_back (Vector (900.0,1500.0,0.0));
  a_to_f.push_back (Vector (800.0,1500.0,0.0));
  a_to_f.push_back (Vector (700.0,1500.0,0.0));
  a_to_f.push_back (Vector (600.0,1500.0,0.0));
  pointA.Connect (&pointF, a_to_f);

  c_to_g.push_back (Vector (1500.0,900.0,0.0));
  c_to_g.push_back (Vector (1500.0,800.0,0.0));
  c_to_g.push_back (Vector (1500.0,700.0,0.0));
  c_to_g.push_back (Vector (1500.0,600.0,0.0));
  c_to_g.push_back (Vector (1500.0,500.0,0.0));
  c_to_g.push_back (Vector (1500.0,400.0,0.0));
  c_to_g.push_back (Vector (1500.0,300.0,0.0));
  pointC.Connect (&pointG, c_to_g);

  d_to_h.push_back (Vector (1850.0,1500.0,0.0));
  d_to_h.push_back (Vector (1950.0,1500.0,0.0));
  d_to_h.push_back (Vector (2050.0,1500.0,0.0));
  d_to_h.push_back (Vector (2150.0,1500.0,0.0));
  pointD.Connect (&pointH, d_to_h);

  e_to_i.push_back (Vector (1500.0,2100.0,0.0));
  e_to_i.push_back (Vector (1500.0,2200.0,0.0));
  e_to_i.push_back (Vector (1500.0,2300.0,0.0));
  e_to_i.push_back (Vector (1500.0,2400.0,0.0));
  pointE.Connect (&pointI, e_to_i);

  //defines paths
  pathABC.push_back (&pointA);
  pathABC.push_back (&pointB);
  pathABC.push_back (&pointC);

  pathCBA.push_back (&pointC);
  pathCBA.push_back (&pointB);
  pathCBA.push_back (&pointA);

  pathFH.push_back (&pointF);
  pathFH.push_back (&pointA);
  pathFH.push_back (&pointB);
  pathFH.push_back (&pointD);
  pathFH.push_back (&pointH);

  pathHF.push_back (&pointH);
  pathHF.push_back (&pointD);
  pathHF.push_back (&pointB);
  pathHF.push_back (&pointA);
  pathHF.push_back (&pointF);

  pathGI.push_back (&pointG);
  pathGI.push_back (&pointC);
  pathGI.push_back (&pointB);
  pathGI.push_back (&pointE);
  pathGI.push_back (&pointI);

  pathIG.push_back (&pointI);
  pathIG.push_back (&pointE);
  pathIG.push_back (&pointB);
  pathIG.push_back (&pointC);
  pathIG.push_back (&pointG);
}

void
MineMobilityPaths::DisplayMinePaths  ()
{
  pathA.push_back (&pointA);
  pathB.push_back (&pointB);
  pathC.push_back (&pointC);
  pathD.push_back (&pointD);
  pathE.push_back (&pointE);
  pathF.push_back (&pointF);
  pathG.push_back (&pointG);
  pathH.push_back (&pointH);
  pathI.push_back (&pointI);

  NodeContainer rps;
  MineMobilityHelper mH;
  rps.Create  (9);
  mH.Install (rps.Get (0), pathA, 0, 0);
  mH.Install (rps.Get (1), pathB, 0, 0);
  mH.Install (rps.Get (2), pathC, 0, 0);
  mH.Install (rps.Get (3), pathD, 0, 0);
  mH.Install (rps.Get (4), pathE, 0, 0);
  mH.Install (rps.Get (5), pathF, 0, 0);
  mH.Install (rps.Get (6), pathG, 0, 0);
  mH.Install (rps.Get (7), pathH, 0, 0);
  mH.Install (rps.Get (8), pathI, 0, 0);
}

}//ns3
