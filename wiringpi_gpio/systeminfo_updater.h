 /*******************************************************************************
  WiringPi GPIO # INDI driver for controlling gpio pins on RasperryPi
  (and compatible devices)
   
  This driver is based on the RaspberryPi GPIO Driver buildt for "INDI for Java"
  http://indilib.org/develop/indiforjava/i4jdrivers/raspberry-pi-gpio-driver.html
   
  Copyright(c) 2016 Magnus W. Eriksen
   
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.
   
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
   
  You should have received a copy of the GNU Lesser General Public License
  along with this library; see the file LICENSE.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
  *******************************************************************************/
#ifndef SystemInfoUpdater_H
#define SystemInfoUpdater_H

#include "systeminfo.h"
#include <libindi/defaultdevice.h>

class SystemInfoUpdater
{
    public:
        
        SystemInfoUpdater(INumberVectorProperty *CPUStatusNP, INumber *CPUStatusN,
                    INumberVectorProperty *MemoryNP, INumber *MemoryN,
                        INumberVectorProperty *UptimeNP, INumber *UptimeN,
                            ITextVectorProperty *UptimeTextTP, IText *UptimeTextT)
            : _CPUStatusNP(CPUStatusNP), _CPUStatusN(CPUStatusN),
                _MemoryNP(MemoryNP), _MemoryN(MemoryN),
                    _UptimeNP(UptimeNP), _UptimeN(UptimeN),
                        _UptimeTextTP(UptimeTextTP), _UptimeTextT(UptimeTextT) {};
        virtual ~SystemInfoUpdater();
        void UpdateDynamicProperties();

    private:
        
        SystemInfo sysInfo;
        void UpdateCPUStatus();
        void UpdateMemory();
        void UpdateUptime();
        void UpdateUptimeText();

        // CPU status
        INumberVectorProperty *_CPUStatusNP;
        INumber *_CPUStatusN;
         
        // Memory Property
        INumberVectorProperty *_MemoryNP;
        INumber *_MemoryN;
         
        // Uptime Property
        INumberVectorProperty *_UptimeNP;
        INumber *_UptimeN;
        
        // Uptime (in Text format) Property
        ITextVectorProperty *_UptimeTextTP;
        IText *_UptimeTextT;
};
#endif
