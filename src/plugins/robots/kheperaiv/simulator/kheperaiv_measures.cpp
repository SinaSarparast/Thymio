/**
 * @file <argos3/plugins/robots/kheperaiv/simulator/kheperaiv_measures.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "kheperaiv_measures.h"

/****************************************/
/****************************************/

const Real THYMIO_LENGHT 	= 0.11;
const Real THYMIO_WIDTH 	= 0.112;
const Real THYMIO_HEIGHT 	= 0.053;
const Real INTERWHEEL_DISTANCE = 0.1;
const Real WHEEL_RADIUS		= 0.02;
const Real WHEEL_WIDTH		= 0.01;
const Real THYMIO_BASE_ELEVATION = 0.0047;
const Real THYMIO_SENSORS_RING_RADIUS = 0.11;
const Real THYMIO_PROXIMITY_SENSOR_RANGE = 0.10;
const Real THYMIO_PROXIMITY_SENSOR_HEIGHT = 0.034;

const Real KHEPERAIV_BASE_RADIUS    = 0.0704;
const Real KHEPERAIV_BASE_ELEVATION = 0.0047;
const Real KHEPERAIV_BASE_HEIGHT    = 0.053;
const Real KHEPERAIV_BASE_TOP       = KHEPERAIV_BASE_ELEVATION + KHEPERAIV_BASE_HEIGHT;
const Real THYMIO_BASE_TOP       	= THYMIO_BASE_ELEVATION + THYMIO_HEIGHT;

const Real KHEPERAIV_WHEEL_RADIUS        = 0.021;
const Real KHEPERAIV_WHEEL_DISTANCE      = 0.1054;
const Real KHEPERAIV_HALF_WHEEL_DISTANCE = KHEPERAIV_WHEEL_DISTANCE * 0.5;

const Real KHEPERAIV_IR_SENSORS_RING_ELEVATION = 0.0145;
const Real KHEPERAIV_IR_SENSORS_RING_RADIUS    = KHEPERAIV_BASE_RADIUS;
const Real KHEPERAIV_IR_SENSORS_RING_RANGE     = 0.12;

const Real KHEPERAIV_ULTRASOUND_SENSORS_RING_ELEVATION = 0.0145;
const Real KHEPERAIV_ULTRASOUND_SENSORS_RING_RADIUS    = KHEPERAIV_BASE_RADIUS;
const CRange<Real> KHEPERAIV_ULTRASOUND_SENSORS_RING_RANGE(0.25, 2.0);

const CVector2 THYMIO_IR_SENSORS_GROUND_OFFSET[2] = {
   CVector2(0.0115,  0.072),
   CVector2(0.0115,  0.072),
};

const CVector3 KHEPERAIV_LEDS_OFFSET[3] = {
   CVector3( 0.04,  0.025, KHEPERAIV_BASE_TOP),
   CVector3(-0.05,  0.000, KHEPERAIV_BASE_TOP),
   CVector3( 0.04, -0.025, KHEPERAIV_BASE_TOP)
};

const Real KHEPERAIV_LIDAR_ELEVATION          = KHEPERAIV_BASE_TOP;
const Real KHEPERAIV_LIDAR_SENSORS_FAN_RADIUS = KHEPERAIV_BASE_RADIUS;
const CRadians 		KHEPERAIV_LIDAR_ANGLE_SPAN(ToRadians(CDegrees(210.0)));
const CRange<Real> 	KHEPERAIV_LIDAR_SENSORS_RING_RANGE(0.02, 4.0);

/****************************************/
/****************************************/
