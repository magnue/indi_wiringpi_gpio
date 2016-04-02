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
#include "systeminfo.h"
#include <math.h>

using namespace std;

SystemInfo::SystemInfo()
{

}

SystemInfo::~SystemInfo()
{

}

/**
 * ***** Get OS info *****
 */
string SystemInfo::GetOSPlatform()
{
    const char* cmd = "uname -i";
    return PopenString(cmd);
}

string SystemInfo::GetOSDistro()
{
    const char* cmd = "cat /etc/lsb-release | grep 'DISTRIB_DESCRIPTION' | cut -d '\"' -f2";
    return PopenString(cmd);
}

string SystemInfo::GetOSCodename()
{
    const char* cmd = "cat /etc/lsb-release | grep 'DISTRIB_CODENAME' | cut -d '=' -f2";
    return PopenString(cmd);
}

/**
 * ***** Get INDI info *****
 */
string SystemInfo::GetINDILibraryVersion()
{
    const char* cmd = "indiserver 2> /tmp/temp; cat /tmp/temp | grep 'Library' | awk '{print $3}'; rm /tmp/temp";
    return PopenString(cmd);
}

string SystemInfo::GetINDIProtocolVersion()
{
    const char* cmd = "indiserver 2> /tmp/temp; cat /tmp/temp | grep 'Protocol' | awk '{ for(i=0;i<=NF;i++) if($i==\"Protocol\") {i++; printf(\"%s\",$i)} }'; rm /tmp/temp";
    return PopenString(cmd);
}

/**
 * ***** Get CPU Status *****
 */
int SystemInfo::GetClockFrequencyCPU()
{
    double totalCpu = GetCPUTotalCPUs();
    int avgFreq = 0;
    for (int i = 0; i < totalCpu; i++)
    {
        string cmdstring = "cat /sys/devices/system/cpu/cpu" + to_string(i) +  "/cpufreq/scaling_cur_freq";
        avgFreq += PopenInt(cmdstring.c_str());
    }
    return avgFreq / totalCpu;
}

int SystemInfo::GetCPULoad()
{
    const char* cmd = "top -bn 2 -d 0.01 | grep '^\%Cpu' | tail -n 1 | gawk '{print $2+$4+$6}'";
    return PopenInt(cmd);
}

/**
 * ***** Get CPU info *****
 */
int SystemInfo::GetCPUTotalCPUs()
{
    const char* cmd = "lscpu | grep 'CPU(s):' | awk '{print $2}' | head -1";
    return PopenInt(cmd);
}

int SystemInfo::GetCPUSocets()
{
    const char* cmd = "lscpu | grep 'Socket' | awk '{print $2}'";
    return PopenInt(cmd);
}

int SystemInfo::GetCPUCoresPerSocet()
{
    const char* cmd = "lscpu | grep 'Core(s)' | awk '{print $4}'";
    return PopenInt(cmd);
}

int SystemInfo::GetCPUThreadsPerCore()
{
    const char* cmd = "lscpu | grep 'Thread(s)' | awk '{print $4}'";
    return PopenInt(cmd);
}

/**
 * ***** Get Memory info *****
 */
int SystemInfo::GetMemoryTotal()
{
    const char* cmd = "cat /proc/meminfo | grep 'MemTotal' | awk '{print $2}'";
    return PopenInt(cmd);
}

int SystemInfo::GetMemoryUsed()
{
    int totalMem = GetMemoryTotal();
    return totalMem - GetMemoryFree();
}

int SystemInfo::GetMemoryFree()
{
    const char* cmd = "cat /proc/meminfo | grep 'MemFree' | awk '{print $2}'";
    return PopenInt(cmd);
}

int SystemInfo::GetMemoryBuffers()
{
    const char* cmd = "cat /proc/meminfo | grep 'Buffers' | awk '{print $2}'";
    return PopenInt(cmd);
}

int SystemInfo::GetMemoryCached()
{
    const char* cmd = "cat /proc/meminfo | grep 'Cached' | awk '{print $2}'";
    return PopenInt(cmd);
}

/**
 * ***** Get Uptime ******
 */
double SystemInfo::GetSystemUptime()
{
    const char* cmd = "cat /proc/uptime | awk '{print $1}'";
    return PopenDouble(cmd);
}

double SystemInfo::GetSystemIdleUptime()
{
    const char* cmd = "cat /proc/uptime | awk '{print $2}'";
    return PopenDouble(cmd);
}

string SystemInfo::GetSystemUptimeAsString()
{
    return DoubleToUptimeString(GetSystemUptime());
}

string SystemInfo::GetSystemIdleUptimeAsString()
{
    return DoubleToUptimeString(GetSystemIdleUptime());
}

string SystemInfo::DoubleToUptimeString(double uptime)
{
    return "Days: " + to_string(static_cast<int>(uptime/60/60/24)) +
         ", Hours: " + to_string(static_cast<int>(fmod(uptime/60/60,24))) +
         ", Minutes: " + to_string(static_cast<int>(fmod(uptime/60,60))) +
         ", Seconds: " + to_string(static_cast<int>(fmod(uptime,60)));
}

/**
 * ***** Popen execute BASH *****
 */
string SystemInfo::PopenString(const char* cmd)
{
    FILE *fp;
    char buffer[512];

    if (!(fp = popen(cmd,"r")))
        return "";

    string str = "";
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
        str += buffer;
    pclose(fp);
    return str;
}

int SystemInfo::PopenInt(const char* cmd)
{
    int integer;
    string str = PopenString(cmd);
    if (str == "")
        return 0;
    istringstream(str) >> integer;
    return integer;
}

double SystemInfo::PopenDouble(const char* cmd)
{
    double decimal;
    string str = PopenString(cmd);
    if (str == "")
        return 0.0;
    istringstream(str) >> decimal;
    return decimal;
}
