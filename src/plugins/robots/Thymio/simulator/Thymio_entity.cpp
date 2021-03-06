/**
 * @file <argos3/plugins/robots/Thymio/simulator/Thymio_entity.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "Thymio_entity.h"
#include "Thymio_measures.h"

#include <argos3/core/utility/math/matrix/rotationmatrix3.h>
#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/plugins/simulator/entities/battery_equipped_entity.h>
#include <argos3/plugins/simulator/entities/ground_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/entities/light_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/proximity_sensor_equipped_entity.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>


namespace argos {

   /****************************************/
   /****************************************/

   CThymioEntity::CThymioEntity() :
      CComposableEntity(NULL),
      m_pcControllableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcGroundSensorEquippedEntity(NULL),
      m_pcLEDEquippedEntity(NULL),
      m_pcProximitySensorEquippedEntity(NULL),
      m_pcWheeledEntity(NULL),
      m_pcBatteryEquippedEntity(NULL) {
   }

   /****************************************/
   /****************************************/
   
   CThymioEntity::CThymioEntity(const std::string& str_id,
                                      const std::string& str_controller_id,
                                      const CVector3& c_position,
                                      const CQuaternion& c_orientation,
                                      const std::string& str_bat_model) :
      CComposableEntity(NULL, str_id),
      m_pcControllableEntity(NULL),
      m_pcEmbodiedEntity(NULL),
      m_pcGroundSensorEquippedEntity(NULL),
      m_pcLEDEquippedEntity(NULL),
      m_pcProximitySensorEquippedEntity(NULL),
      m_pcWheeledEntity(NULL),
      m_pcBatteryEquippedEntity(NULL) {
      try {
         /*
          * Create and init components
          */
         /* Embodied entity */

         m_pcEmbodiedEntity = new CEmbodiedEntity(this, "body_0", c_position, c_orientation);
         AddComponent(*m_pcEmbodiedEntity);

         /* Wheeled entity and wheel positions (left, right) */
         m_pcWheeledEntity = new CWheeledEntity(this, "wheels_0", 2);
         AddComponent(*m_pcWheeledEntity);
         m_pcWheeledEntity->SetWheel(0, CVector3(-THYMIO_XOFFSET,  Thymio_HALF_WHEEL_DISTANCE, 0.0f), Thymio_WHEEL_RADIUS);
         m_pcWheeledEntity->SetWheel(1, CVector3(-THYMIO_XOFFSET, -Thymio_HALF_WHEEL_DISTANCE, 0.0f), Thymio_WHEEL_RADIUS);

         /* LED equipped entity */
         m_pcLEDEquippedEntity = new CLEDEquippedEntity(this, "thymio_led");
         AddComponent(*m_pcLEDEquippedEntity);
         for(int i=0; i<8; i++){
         m_pcLEDEquippedEntity->AddLED(Thymio_LEDS_OFFSET[i],
                                       m_pcEmbodiedEntity->GetOriginAnchor());
         }

         /* Proximity sensor equipped entity */
         m_pcProximitySensorEquippedEntity = new CProximitySensorEquippedEntity(this, "proximity");
         AddComponent(*m_pcProximitySensorEquippedEntity);
         for(UInt32 i = 0; i < 8; ++i) {
            m_pcProximitySensorEquippedEntity->AddSensor(
               PROXIMITY_SENSOR_OFFSET[i], // offset
               CVector3(1.0 , CRadians::PI_OVER_TWO , PROXIMITY_SENSOR_ANGLES[i]), // direction
                THYMIO_PROXIMITY_SENSOR_RANGE,
                    m_pcEmbodiedEntity->GetOriginAnchor());
         }

         /* Ground sensor equipped entity */
         m_pcGroundSensorEquippedEntity = new CGroundSensorEquippedEntity(this,"ground_0");
         AddComponent(*m_pcGroundSensorEquippedEntity);
         m_pcGroundSensorEquippedEntity->AddSensor(THYMIO_IR_SENSORS_GROUND_OFFSET[0],
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE,
                                                   m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcGroundSensorEquippedEntity->AddSensor(THYMIO_IR_SENSORS_GROUND_OFFSET[1],
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE,
                                                   m_pcEmbodiedEntity->GetOriginAnchor());

         /* Accelerometer */
//         m_pcThymioAccSensor = new CThymioAccSensor();
//         m_pcThymioAccSensor->SetRobot(*this);

         /* Battery equipped entity */
         m_pcBatteryEquippedEntity = new CBatteryEquippedEntity(this, "battery_0", str_bat_model);
         AddComponent(*m_pcBatteryEquippedEntity);

         /* Controllable entity
            It must be the last one, for actuators/sensors to link to composing entities correctly */
         m_pcControllableEntity = new CControllableEntity(this, "controller_0");
         AddComponent(*m_pcControllableEntity);
         m_pcControllableEntity->SetController(str_controller_id);
         /* Update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CThymioEntity::Init(TConfigurationNode& t_tree) {
      try {
         /*
          * Init parent
          */
         CComposableEntity::Init(t_tree);
         /*
          * Create and init components
          */
         /* Embodied entity */
         m_pcEmbodiedEntity = new CEmbodiedEntity(this);
         AddComponent(*m_pcEmbodiedEntity);
         m_pcEmbodiedEntity->Init(GetNode(t_tree, "body"));


         /* Wheeled entity and wheel positions (left, right) */
         m_pcWheeledEntity = new CWheeledEntity(this, "wheels_0", 2);
         AddComponent(*m_pcWheeledEntity);
         m_pcWheeledEntity->SetWheel(0, CVector3(-THYMIO_XOFFSET,  Thymio_HALF_WHEEL_DISTANCE, 0.0f), Thymio_WHEEL_RADIUS);
         m_pcWheeledEntity->SetWheel(1, CVector3(-THYMIO_XOFFSET, -Thymio_HALF_WHEEL_DISTANCE, 0.0f), Thymio_WHEEL_RADIUS);

         /* LED equipped entity, with LEDs [0-8] */
         m_pcLEDEquippedEntity = new CLEDEquippedEntity(this, "thymio_led");
         AddComponent(*m_pcLEDEquippedEntity);
         for(int i=0; i<8; i++){
         m_pcLEDEquippedEntity->AddLED(Thymio_LEDS_OFFSET[i],
                                       m_pcEmbodiedEntity->GetOriginAnchor());
         }

         /* Proximity sensor equipped entity */
         m_pcProximitySensorEquippedEntity = new CProximitySensorEquippedEntity(this,"proximity");
         AddComponent(*m_pcProximitySensorEquippedEntity);
        for(UInt32 i = 0; i < 7; ++i) {
            m_pcProximitySensorEquippedEntity->AddSensor(
               PROXIMITY_SENSOR_OFFSET[i], // offset
               CVector3(1.0 , CRadians::PI_OVER_TWO , PROXIMITY_SENSOR_ANGLES[i]), // direction
                THYMIO_PROXIMITY_SENSOR_RANGE,
               m_pcEmbodiedEntity->GetOriginAnchor());
         }

         /* Ground sensor equipped entity */
         m_pcGroundSensorEquippedEntity = new CGroundSensorEquippedEntity(this,"ground_0");
         AddComponent(*m_pcGroundSensorEquippedEntity);
         m_pcGroundSensorEquippedEntity->AddSensor(THYMIO_IR_SENSORS_GROUND_OFFSET[0],
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE,
                                                   m_pcEmbodiedEntity->GetOriginAnchor());
         m_pcGroundSensorEquippedEntity->AddSensor(THYMIO_IR_SENSORS_GROUND_OFFSET[1],
                                                   CGroundSensorEquippedEntity::TYPE_GRAYSCALE,
                                                   m_pcEmbodiedEntity->GetOriginAnchor());



         /* Battery equipped entity */
         m_pcBatteryEquippedEntity = new CBatteryEquippedEntity(this, "battery_0");
         if(NodeExists(t_tree, "battery"))
            m_pcBatteryEquippedEntity->Init(GetNode(t_tree, "battery"));
         AddComponent(*m_pcBatteryEquippedEntity);

         /* Controllable entity
            It must be the last one, for actuators/sensors to link to composing entities correctly */
         m_pcControllableEntity = new CControllableEntity(this);
         AddComponent(*m_pcControllableEntity);
         m_pcControllableEntity->Init(GetNode(t_tree, "controller"));
         /* Update components */
         UpdateComponents();
      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CThymioEntity::Reset() {
      /* Reset all components */
      CComposableEntity::Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CThymioEntity::Destroy() {
      CComposableEntity::Destroy();
   }

   /****************************************/
   /****************************************/

   void CThymioEntity::UpdateComponents() {
       if(m_pcLEDEquippedEntity->IsEnabled())
          m_pcLEDEquippedEntity->Update();
      if(m_pcBatteryEquippedEntity->IsEnabled())
         m_pcBatteryEquippedEntity->Update();
   }

   /****************************************/
   /****************************************/
   
   REGISTER_ENTITY(CThymioEntity,
                   "Thymio",
                   "Sina Sarparast [ss4g17@soton.ac.uk]",
                   "1.0",
                   "The Thymio robot.",
                   "The Thymio is a commercial, extensible robot produced by Mobsya More\n"
                   "information is available at https://www.thymio.org.\n\n"
                   "REQUIRED XML CONFIGURATION\n\n"
                   "  <arena ...>\n"
                   "    ...\n"
                   "    <Thymio id=\"eb0\">\n"
                   "      <body position=\"0.4,2.3,0.25\" orientation=\"45,90,0\" />\n"
                   "      <controller config=\"mycntrl\" />\n"
                   "    </Thymio>\n"
                   "    ...\n"
                   "  </arena>\n\n"
                   "The 'id' attribute is necessary and must be unique among the entities. If two\n"
                   "entities share the same id, initialization aborts.\n"
                   "The 'body/position' attribute specifies the position of the pucktom point of the\n"
                   "Thymio  in the arena. When the robot is untranslated and unrotated, the\n"
                   "pucktom point is in the origin and it is defined as the middle point between\n"
                   "the two wheels on the XY plane and the lowest point of the robot on the Z\n"
                   "axis, that is the point where the wheels touch the floor. The attribute values\n"
                   "are in the X,Y,Z order.\n"
                   "The 'body/orientation' attribute specifies the orientation of the Thymio.\n"
                   "All rotations are performed with respect to the pucktom point. The order of the\n"
                   "angles is Z,Y,X, which means that the first number corresponds to the rotation\n"
                   "around the Z axis, the second around Y and the last around X. This reflects\n"
                   "the internal convention used in ARGoS, in which rotations are performed in\n"
                   "that order. Angles are expressed in degrees. When the robot is unrotated, it\n"
                   "is oriented along the X axis.\n"
                   "The 'controller/config' attribute is used to assign a controller to the\n"
                   "Thymio . The value of the attribute must be set to the id of a previously\n"
                   "defined controller. Controllers are defined in the <controllers> XML subtree.\n\n"
                   "OPTIONAL XML CONFIGURATION\n\n",
                   "Under development"
      );

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CThymioEntity);

   /****************************************/
   /****************************************/

}
