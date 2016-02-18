#include "ns3/core-module.h"
#include "mine-mobility-paths.h"
#include "ns3/rendezvous-point.h"
namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("mine-mobility-paths");

//defines RendezvousPoints (globally because of unknown construct error)
RendezvousPoint pointA(Vector(0.0,500.0,0.0));
RendezvousPoint pointB(Vector(500.0,500.0,0.0));
RendezvousPoint pointC(Vector(500.0,0.0,0.0));
RendezvousPoint pointD(Vector(750.0,500.0,0.0));
RendezvousPoint pointE(Vector(500.0,1000.0,0.0));
RendezvousPoint pointF(Vector(-500.0,500.0,0.0));
RendezvousPoint pointG(Vector(500.0,-750.0,0.0));
RendezvousPoint pointH(Vector(1250.0,500.0,0.0));
RendezvousPoint pointI(Vector(500.0,1500.0,0.0));


MineMobilityPaths::MineMobilityPaths(void) {

  //defines connections
   a_to_b.push_back(Vector(100,490,0));
   a_to_b.push_back(Vector(200,510,0));
   a_to_b.push_back(Vector(300,490,0));
   a_to_b.push_back(Vector(400,510,0));
   pointA.Connect(&pointB, a_to_b);

   b_to_c.push_back(Vector(490,400,0));
   b_to_c.push_back(Vector(510,300,0));
   b_to_c.push_back(Vector(490,200,0));
   b_to_c.push_back(Vector(510,100,0));
   pointB.Connect(&pointC, b_to_c);

   b_to_d.push_back(Vector(600.0,500.0,0.0));
   b_to_d.push_back(Vector(700.0,500.0,0.0));
   pointB.Connect(&pointD, b_to_d);

   b_to_e.push_back(Vector(500.0,600.0,0.0));
   b_to_e.push_back(Vector(500.0,700.0,0.0));
   b_to_e.push_back(Vector(500.0,800.0,0.0));
   b_to_e.push_back(Vector(500.0,900.0,0.0));
   pointB.Connect(&pointE, b_to_e);

   a_to_f.push_back(Vector(-100.0,500.0,0.0));
   a_to_f.push_back(Vector(-200.0,500.0,0.0));
   a_to_f.push_back(Vector(-300.0,500.0,0.0));
   a_to_f.push_back(Vector(-400.0,500.0,0.0));
   pointA.Connect(&pointF, a_to_f);

   c_to_g.push_back(Vector(500.0,-100.0,0.0));
   c_to_g.push_back(Vector(500.0,-200.0,0.0));
   c_to_g.push_back(Vector(500.0,-300.0,0.0));
   c_to_g.push_back(Vector(500.0,-400.0,0.0));
   c_to_g.push_back(Vector(500.0,-500.0,0.0));
   c_to_g.push_back(Vector(500.0,-600.0,0.0));
   c_to_g.push_back(Vector(500.0,-700.0,0.0));
   pointC.Connect(&pointG, c_to_g);

   d_to_h.push_back(Vector(850.0,500.0,0.0));
   d_to_h.push_back(Vector(950.0,500.0,0.0));
   d_to_h.push_back(Vector(1050.0,500.0,0.0));
   d_to_h.push_back(Vector(1150.0,500.0,0.0));
   pointD.Connect(&pointH, d_to_h);

   e_to_i.push_back(Vector(500.0,1100.0,0.0));
   e_to_i.push_back(Vector(500.0,1200.0,0.0));
   e_to_i.push_back(Vector(500.0,1300.0,0.0));
   e_to_i.push_back(Vector(500.0,1400.0,0.0));
   pointE.Connect(&pointE, e_to_i);

   //defines paths
   pathABC.push_back(&pointA);
   pathABC.push_back(&pointB);
   pathABC.push_back(&pointC);

   pathCBA.push_back(&pointC);
   pathCBA.push_back(&pointB);
   pathCBA.push_back(&pointA);

   pathFH.push_back(&pointF);
   pathFH.push_back(&pointA);
   pathFH.push_back(&pointB);
   pathFH.push_back(&pointD);
   pathFH.push_back(&pointH);

   pathFH.push_back(&pointH);
   pathFH.push_back(&pointD);
   pathFH.push_back(&pointB);
   pathFH.push_back(&pointA);
   pathFH.push_back(&pointF);

   pathGI.push_back(&pointG);
   pathGI.push_back(&pointC);
   pathGI.push_back(&pointB);
   pathGI.push_back(&pointE);
   pathGI.push_back(&pointI);

   pathIG.push_back(&pointI);
   pathIG.push_back(&pointE);
   pathIG.push_back(&pointB);
   pathIG.push_back(&pointC);
   pathIG.push_back(&pointG);
}

}//ns3
