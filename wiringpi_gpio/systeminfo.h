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

#ifndef SystemInfo_H
#define SystemInfo_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <cstring>

using namespace std;

class SystemInfo
{

    public:
        SystemInfo();
        virtual ~SystemInfo();

        // OS info
        string GetOSPlatform();
        string GetOSDistro();
        string GetOSCodename();

        // INDI info
        string GetINDILibraryVersion();
        string GetINDIProtocolVersion();

        // CPU Status
        int GetClockFrequencyCPU();
        int GetCPULoad();

        // CPU Info
        int GetCPUTotalCPUs();
        int GetCPUSocets();
        int GetCPUCoresPerSocet();
        int GetCPUThreadsPerCore();

        // Memory
        int GetMemoryTotal();
        int GetMemoryUsed();
        int GetMemoryFree();
        int GetMemoryBuffers();
        int GetMemoryCached();

        // Uptime
        double GetSystemUptime();
        double GetSystemIdleUptime();
        string GetSystemUptimeAsString(); // Day: x, Hour: x, Min: x, Sec: x
        string GetSystemIdleUptimeAsString(); // -------- | | --------

    private:
        string PopenString(const char* cmd);
        int PopenInt(const char* cmd);
        double PopenDouble(const char* cmd);
        string DoubleToUptimeString(double uptime);
};

#endif
