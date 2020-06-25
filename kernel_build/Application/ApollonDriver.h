#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include <stdlib.h>
#include <stdint.h>

void SensorInitialization();
void SensorFinalization();


/* Read the Ptolemy sensor Value                                                                       *
 * (i)   Input: The Instance Name of Sensor (the same name with the Ptolemy environment)               *
 * (ii)  Input: The Attribute Name (the same name with the Ptolemy environment)                        *
 * Output: The value of sensor (in double)                                                             */
double PtolemySensor(char * instanceName, char * attributeName);


/* Write the Ptolemy actuator Values                                                                   *
 * (i)   Input: The Instance Name of Sensor (the same name with the Ptolemy environment)               *
 * (ii)  Input: The Attribute Name (the same name with the Ptolemy environment)                        *
 * (iii) Input: The Value of each attribute Name                                                       */
void PtolemyActuator(char * instanceName, char * attributeName1, double attributeValue1, char * attributeName2, double attributeValue2);


/* Pauses the program for the amount of time (in miliseconds) specified as parameter.                  *
 * (There are 1000 milliseconds in a second.)                                                          *
 * Input: the number of milliseconds to pause                                                          */
void delay(uint64_t val);


#endif

