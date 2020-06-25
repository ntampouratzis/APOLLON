/*
 * Copyright (c) 2010 ARM Limited
 * All rights reserved
 *
 * The license below extends only to copyright in the software and shall
 * not be construed as granting a license to any other intellectual
 * property including but not limited to intellectual property relating
 * to a hardware implementation of the functionality of the software
 * licensed hereunder.  You may use the software subject to the license
 * terms below provided that you ensure that this notice is replicated
 * unmodified and in its entirety in all distributions of the software,
 * modified or unmodified, in source code or in binary form.
 *
 * Copyright (c) 2005 The Regents of The University of Michigan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Tampouratzis Nikolaos
 *
 */

#include "base/trace.hh"
#include "debug/ApollonDevice.hh"
#include "dev/arm/amba_device.hh"
#include "dev/arm/ApollonDevice.hh"
#include "mem/packet.hh"
#include "mem/packet_access.hh"

#include "sim/system.hh"

#include <unistd.h>


using namespace std;




ApollonDevice::ApollonDevice(const Params *p)
    : AmbaDmaDevice(p), nodeNumber(p->nodeNum), synchEvent(this), DMARcvPktActuatorEvent(this), DMARcvPktSensorEvent(this), DMASendPktSensorEvent(this)
{
    //ACTUATOR
    PtolemyActuator = (PtolemyActuator_t*) malloc (sizeof(PtolemyActuator_t));
    MemSimToDevActuator = (uint8_t*) malloc (DEVICE_MEMORY_ENTRY_SIZE);
    
    //SENSOR
    PtolemySensorAttr = (PtolemySensorAttr_t*) malloc (sizeof(PtolemySensorAttr_t));
    PtolemySensorData = (PtolemySensorData_t*) malloc (sizeof(PtolemySensorData_t));
    MemSimToDevSensor = (uint8_t*) malloc (DEVICE_MEMORY_ENTRY_SIZE);
    MemDevToSimSensor = (uint8_t*) malloc (DEVICE_MEMORY_ENTRY_SIZE);
    
    pioSize = p->pio_size;
    //schedule(synchEvent, curTick());
}

void
ApollonDevice::Synch()
{
    printf("Synch with tick: %ld\n",curTick());
    schedule(synchEvent, curTick() + 1000000000000);
}


Tick
ApollonDevice::read(PacketPtr pkt)
{
    pkt->makeAtomicResponse();
    assert(pkt->getAddr() >= pioAddr && pkt->getAddr() < pioAddr + pioSize); 
    
    return pioDelay;
}


void
ApollonDevice::DMARcvPktActuatorComplete()
{
  printf("\n----- DMA Read Actuator Completed!!! (curTick(): %ld) -----\n", curTick());
  memcpy(PtolemyActuator, MemSimToDevActuator, sizeof(PtolemyActuator_t));
  
  printf("PtolemyActuator.instanceName %s\n", (*PtolemyActuator).instanceName);
  for(int i=0; i<(*PtolemyActuator).NoOfActiveAttributes; i++){
      printf("PtolemyActuator.attributeName %s: %f\n", (*PtolemyActuator).attributeName[i], (*PtolemyActuator).value_double[i]);
  }    
}


void
ApollonDevice::DMARcvPktSensorComplete()
{
  printf("\n----- DMA Read SENSOR Completed!!! (curTick(): %ld) -----\n", curTick());
  memcpy(PtolemySensorAttr, MemSimToDevSensor, sizeof(PtolemySensorAttr_t));
  
  printf( "PtolemySensor.instanceName %s\n", (*PtolemySensorAttr).instanceName);
  printf( "PtolemySensor.attributeName %s\n", (*PtolemySensorAttr).attributeName);
  
  //FIXME SEARCH for values from HLA-Ptolemy
  if(strcmp ((*PtolemySensorAttr).attributeName,"PositionX") == 0 )
    (*PtolemySensorData).value_double = 987654.321;
  else
    (*PtolemySensorData).value_double = 887654.321;  
  

  
  memcpy(MemDevToSimSensor, PtolemySensorData, sizeof(PtolemySensorData_t));
  dmaWrite((Addr) bus_addr_sensor_from_device, sizeof(PtolemySensorData_t), &DMASendPktSensorEvent, MemDevToSimSensor , DELAY_PER_DMA_MEMORY_ENTRY);
    
}

void
ApollonDevice::DMASendPktSensorComplete()
{
  printf("\n----- DMA Write Completed!!! (curTick(): %ld) -----\n", curTick());    
  gic->sendInt(intNum); //# Send Interrupt if the DMA Write Completed #
}



Tick
ApollonDevice::write(PacketPtr pkt)
{
    
    uint32_t bus_addr_actuator;
  
    pkt->makeAtomicResponse();    
    switch (pkt->getAddr() - pioAddr) {
          
        case DMA_TRANSFER_ACTUATOR_TO_DEVICE:
            bus_addr_actuator = pkt->get<uint32_t>();
            dmaRead((Addr) bus_addr_actuator, sizeof(PtolemyActuator_t), &DMARcvPktActuatorEvent, MemSimToDevActuator, DELAY_PER_DMA_MEMORY_ENTRY);
            break;  
        case DMA_TRANSFER_SENSOR_FROM_DEVICE:
            //printf("\n\nDMA_TRANSFER_SENSOR_FROM_DEVICE curTick(): %ld \n", curTick());
            bus_addr_sensor_from_device = pkt->get<uint32_t>();
            break;       
        case DMA_TRANSFER_SENSOR_TO_DEVICE:
            //printf("\n\nDMA_TRANSFER_SENSOR_TO_DEVICE curTick(): %ld \n", curTick());
            bus_addr_sensor_to_device = pkt->get<uint32_t>();
            dmaRead((Addr) bus_addr_sensor_to_device, sizeof(PtolemySensorAttr_t), &DMARcvPktSensorEvent, MemSimToDevSensor, DELAY_PER_DMA_MEMORY_ENTRY);
            break;     
        default:
          panic("Device: Unrecognized Sensor Value!\n");
    }
           
    return pioDelay;
}

AddrRangeList
ApollonDevice::getAddrRanges() const
{
    AddrRangeList ranges;
    ranges.push_back(RangeSize(pioAddr, pioSize));
    return ranges;
}


ApollonDevice *
ApollonDeviceParams::create()
{
    return new ApollonDevice(this);
}
