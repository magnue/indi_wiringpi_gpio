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
#include "systeminfo_updater.h"

SystemInfoUpdater::~SystemInfoUpdater()
{
}

void SystemInfoUpdater::UpdateDynamicProperties()
{
    UpdateCPUStatus();
    UpdateMemory();
    UpdateUptime();
    UpdateUptimeText();
}

void SystemInfoUpdater::UpdateCPUStatus()
{
    _CPUStatusN[0].value = sysInfo.GetClockFrequencyCPU() / 1000; // KHz / 1000 = MHz
    _CPUStatusN[1].value = sysInfo.GetCPULoad();
    _CPUStatusNP->s = (_CPUStatusN[0].value == 0) ?
        IPS_ALERT : IPS_OK; 
    IDSetNumber(_CPUStatusNP, NULL);
}

void SystemInfoUpdater::UpdateMemory()
{
    // Let's convert kibibytes ti mibibytes, ie / 1024
    _MemoryN[0].value = sysInfo.GetMemoryTotal() / 1024.000;
    _MemoryN[1].value = sysInfo.GetMemoryFree() / 1024.000;
    _MemoryN[2].value = sysInfo.GetMemoryUsed() / 1024.000;
    _MemoryN[3].value = sysInfo.GetMemoryBuffers() / 1024.000;
    _MemoryN[4].value = sysInfo.GetMemoryCached() / 1024.000;
    int updated = 5;
    bool alert = false;
    for (int i = 0; i < updated; ++i)
    {
        if (_MemoryN[i].value == 0)
            alert = true;
    }
    _MemoryNP->s = alert ? IPS_ALERT : IPS_OK;
    IDSetNumber(_MemoryNP, NULL);
}

void SystemInfoUpdater::UpdateUptime()
{
    _UptimeN[0].value = sysInfo.GetSystemUptime();
    _UptimeN[1].value = sysInfo.GetSystemIdleUptime();
    _UptimeNP->s = (_UptimeN[0].value == 0 || _UptimeN[1].value == 0) ?
        IPS_ALERT : IPS_OK;
    IDSetNumber(_UptimeNP, NULL);
}

void SystemInfoUpdater::UpdateUptimeText()
{
    _UptimeTextT[0].text = strdup(sysInfo.GetSystemUptimeAsString().c_str());
    _UptimeTextT[1].text = strdup(sysInfo.GetSystemIdleUptimeAsString().c_str());
    _UptimeTextTP->s = (_UptimeTextT[0].text == "" || _UptimeTextT[1].text == "") ?
        IPS_ALERT : IPS_OK;
    IDSetText(_UptimeTextTP, NULL);
}
