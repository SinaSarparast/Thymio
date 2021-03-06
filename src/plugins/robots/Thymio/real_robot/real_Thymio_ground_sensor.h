#ifndef REAL_Thymio_GROUND_SENSOR_H
#define REAL_Thymio_GROUND_SENSOR_H

#include <argos3/plugins/robots/Thymio/control_interface/ci_Thymio_ground_sensor.h>
#include <argos3/plugins/robots/Thymio/real_robot/real_Thymio_device.h>
#include <aseba/switches/asebacppapi/thymio_interface.h>

using namespace argos;

class CRealThymioGroundSensor :
   public CCI_ThymioGroundSensor,
   public CRealThymioDevice {

public:

   CRealThymioGroundSensor(Aseba::ThymioInterface* ThymioInterface);
   
   virtual ~CRealThymioGroundSensor();

   virtual void Do();
   
};

#endif // REAL_Thymio_GROUND_SENSOR_H
