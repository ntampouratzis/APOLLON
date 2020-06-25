#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
#include <linux/ioctl.h>
 
#include "../../../cgem5/src/dev/arm/ApollonStructs.h"


#define QUERY_DELAY_REQUEST 	  _IOW('p', 1, uint64_t *)
#define QUERY_SENSOR_WRITE_DATA   _IOW('p', 2, PtolemySensorAttr_t *)
#define QUERY_SENSOR_READ_DATA    _IOR('p', 3, PtolemySensorData_t *)
#define QUERY_ACTUATOR_WRITE_DATA _IOW('p', 4, PtolemyActuator_t *)
#define DMA_FROM_DEVICE_WAIT      _IOR('q', 5, uint8_t *)
#define QUERY_BUS_ADDR_SENSOR     _IO('q', 6)

#endif