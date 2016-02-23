/*
 * mine-mobility-helper.cc
 *
 *  Created on: Feb 19, 2016
 *      Author: joakim
 */

#include "ns3/core-module.h"
#include "ns3/mine-mobility-model.h"
#include "ns3/mine-mobility-helper.h"

namespace ns3 {
NS_LOG_COMPONENT_DEFINE ("mine-mobility-helper");


MineMobilityHelper::MineMobilityHelper ()
{
  m_dumperSpeed = 3.0;
  m_dumperPriority = 0;
  m_loaderSpeed = 7.0;
  m_loaderPriority = 1;
  m_carSpeed = 15.0;
  m_carPriority = 2;
}

MineMobilityHelper::~MineMobilityHelper ()
{
}

void
MineMobilityHelper::Install(Ptr<Node> node, std::vector<RendezvousPoint*> &path, double speed, uint32_t priority)
{
  Ptr<Object> object = node;
  Ptr<MineMobilityModel> model = object->GetObject<MineMobilityModel> ();
  if (model == 0)
    {
      Ptr<MineMobilityModel> minemob = CreateObjectWithAttributes<MineMobilityModel> ("Speed", DoubleValue(speed), "Priority", IntegerValue(priority));
      minemob->SetPath(path);
      Ptr<MobilityModel> model = minemob->GetObject<MobilityModel>();
      object->AggregateObject(minemob);
    }
  else
    {
      NS_FATAL_ERROR ("The requested node already has a mobilty model.");
    }
}

void
MineMobilityHelper::Install(Ptr<Node> node, std::vector<RendezvousPoint*> &path, vehicle_type vehicle)
{
  switch(vehicle)
  {
    case DUMPER:
      Install(node, path, m_dumperSpeed, m_dumperPriority);
      break;
    case LOADER:
      Install(node, path, m_loaderSpeed, m_loaderPriority);
      break;
    case CAR:
      Install(node, path, m_carSpeed, m_carPriority);
      break;
    default:
      NS_FATAL_ERROR ("Undefined vehicle type");
  }
}


}//ns3
