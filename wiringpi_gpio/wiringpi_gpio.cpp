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
#include "wiringpi_gpio.h"

#include <memory>
#include <climits>
#include <stdexcept>

#define CONFIGURATION_TAB "Configuration"
#define SYSTEM_TAB "System"
#define SENSORS_TAB "Sensors"

// We declare an auto pointer to WiringPiGPIO.
std::unique_ptr<WiringPiGPIO> wiringPiGPIO(new WiringPiGPIO());

void ISGetProperties(const char *dev)
{
        wiringPiGPIO->ISGetProperties(dev);
}

void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int num)
{
        wiringPiGPIO->ISNewSwitch(dev, name, states, names, num);
}

void ISNewText(const char *dev, const char *name, char *texts[], char *names[], int num)
{
        wiringPiGPIO->ISNewText(dev, name, texts, names, num);
}

void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int num)
{
        wiringPiGPIO->ISNewNumber(dev, name, values, names, num);
}

void ISNewBLOB (const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[], 
        char *formats[], char *names[], int n)
{
  INDI_UNUSED(dev);
  INDI_UNUSED(name);
  INDI_UNUSED(sizes);
  INDI_UNUSED(blobsizes);
  INDI_UNUSED(blobs);
  INDI_UNUSED(formats);
  INDI_UNUSED(names);
  INDI_UNUSED(n);
}

void ISSnoopDevice (XMLEle *root)
{
    wiringPiGPIO->ISSnoopDevice(root);
}

/************************************************************************************
 * WiringPi GPIO
* ***********************************************************************************/
WiringPiGPIO::WiringPiGPIO()
{
    setVersion(0,1);
    setDriverInterface(AUX_INTERFACE);
}

WiringPiGPIO::~WiringPiGPIO()
{
    for (int i = 0; i < NUMBER_OF_PINS; i++)
    {
        delete PinsConfigSP[i];
        delete InputDigitalLP[i];
        delete OutputDigitalSP[i];
    }
    delete OutputPWMNP;
}

bool WiringPiGPIO::saveConfigItems(FILE *fp)
{
    for (int i = 0; i < NUMBER_OF_PINS; i++)
        IUSaveConfigSwitch(fp, PinsConfigSP[i]);
    return true;
}

bool WiringPiGPIO::initProperties()
{
    INDI::DefaultDevice::initProperties();

    /************************************************************************************
    * Main Tab
    * ***********************************************************************************/
    for (int i = 0; i < NUMBER_OF_PINS; i++)
    {
        InputDigitalLP[i] = new ILightVectorProperty;
        OutputDigitalSP[i] = new ISwitchVectorProperty;
    }
    OutputPWMNP = new INumberVectorProperty;

    /************************************************************************************
    * Configuration Tab
    * ***********************************************************************************/
    for (int i = 0; i < NUMBER_OF_PINS; i++)
        PinsConfigSP.push_back(new ISwitchVectorProperty);

    for (int i = 0; i < PINS_GPIO; i++)
    {
        int pinNr = (i > 5) ? i + 11 : (i > 0) ? i + 1 : i; 
        InitConfigTab(i, PINS_GPIO, MODES_GPIO, pinNr, PinsConfigGPIOS);
    }
    for (int i = 0; i < PINS_1_7; i++)
    {
        int pinNr = (i > 0) ? 7 : 1; 
        InitConfigTab(i, PINS_1_7, MODES_1_7, pinNr, PinsConfig1_7S);
    }
    for (int i = 0; i < PINS_8_9; i++)
    {
        int pinNr = (i > 0) ? 9 : 8; 
        InitConfigTab(i, PINS_8_9, MODES_8_9, pinNr, PinsConfig8_9S);
    }
    for (int i = 0; i < PINS_10to16; i++)
    {
        int pinNr = i + 10; 
        InitConfigTab(i, PINS_10to16, MODES_10to16, pinNr, PinsConfig10to16S);
    }

    /*************************************************************************************
    * System Tab
    * ************************************************************************************/
    IUFillText(&OSInfoT[0],"OS_PLATFORM","os platform",sysInfo.GetOSPlatform().c_str());
    IUFillText(&OSInfoT[1],"OS_DISTRO","os distro",sysInfo.GetOSDistro().c_str());
    IUFillText(&OSInfoT[2],"OS_CODENAME","os codename",sysInfo.GetOSCodename().c_str());
    IUFillTextVector(&OSInfoTP,OSInfoT,3,getDeviceName(),"OS_INFO","OS Info",
        SYSTEM_TAB,IP_RO,60,IPS_OK);

    IUFillText(&INDIInfoT[0],"INDI_LIBRARY","indi library version",sysInfo.GetINDILibraryVersion().c_str());
    IUFillText(&INDIInfoT[1],"INDI_PROTOCOL","indi protocol version",sysInfo.GetINDIProtocolVersion().c_str());
    IUFillTextVector(&INDIInfoTP,INDIInfoT,2,getDeviceName(),"INDI_INFO","INDI Info",
        SYSTEM_TAB,IP_RO,60,IPS_OK);

    IUFillNumber(&CPUInfoN[0], "CPU_TOTAL", "total cpu's", "%6.0f", 0, 100, 1, sysInfo.GetCPUTotalCPUs());
    IUFillNumber(&CPUInfoN[1], "CPU_SOCETS", "cpu socets", "%6.0f", 0, 100, 1, sysInfo.GetCPUSocets());
    IUFillNumber(&CPUInfoN[2], "CPU_CORES", "cores per socet", "%6.0f", 0, 100, 1, sysInfo.GetCPUCoresPerSocet());
    IUFillNumber(&CPUInfoN[3], "CPU_THREADS", "threads per core", "%6.0f", 0, 100, 1, sysInfo.GetCPUThreadsPerCore());
    IUFillNumberVector(&CPUInfoNP, CPUInfoN, 4, getDeviceName(), "CPU_INFO",
        "CPU Info", SYSTEM_TAB, IP_RO, 60, IPS_OK);

    /*************************************************************************************
    * Sensors Tab
    * ************************************************************************************/
    IUFillNumber(&CPUStatusN[0], "CPU_CLOCK", "cpu clock freq (GHz)", "%6.3f", 0, INT_MAX, 1, 0.00);
    IUFillNumber(&CPUStatusN[1], "CPU_LOAD", "cpu load (%)", "%6.0f", 0, 100, 1, 0);
    IUFillNumberVector(&CPUStatusNP, CPUStatusN, 2, getDeviceName(), "CPU_STATUS",
        "CPU Status", SENSORS_TAB, IP_RO, 60, IPS_IDLE);

    IUFillNumber(&MemoryN[0], "MEMORY_TOTAL", "total memory (MiB)", "%6.3f", 0, INT_MAX, 1, 0);
    IUFillNumber(&MemoryN[1], "MEMORY_USED", "used memory (MiB)", "%6.3f", 0, INT_MAX, 1, 0);
    IUFillNumber(&MemoryN[2], "MEMORY_FREE", "free memory (MiB)", "%6.3f", 0, INT_MAX, 1, 0);
    IUFillNumber(&MemoryN[3], "MEMORY_BUFFERS", "buffers memory (MiB)", "%6.3f", 0, INT_MAX, 1, 0);
    IUFillNumber(&MemoryN[4], "MEMORY_CACHED", "cached memory (MiB)", "%6.3f", 0, INT_MAX, 1, 0);
    IUFillNumberVector(&MemoryNP, MemoryN, 5, getDeviceName(), "MEMORY_STATUS",
        "Memory Status", SENSORS_TAB, IP_RO, 60, IPS_IDLE);

    IUFillNumber(&UptimeN[0], "UPTIME_UP", "uptime (sec)", "%6.0f", 0, INT_MAX, 1, 0);
    IUFillNumber(&UptimeN[1], "UPTIME_IDLE", "idle uptime (sec)", "%6.0f", 0, INT_MAX, 1, 0);
    IUFillNumberVector(&UptimeNP, UptimeN, 2, getDeviceName(), "SYSTEM_UPTIMES",
        "System Uptimes",SENSORS_TAB, IP_RO, 60, IPS_OK);
    
    IUFillText(&UptimeTextT[0],"UPTIME_TEXT_UP","uptime","");
    IUFillText(&UptimeTextT[1],"UPTIME_TEXT_IDLE","idle uptime","");
    IUFillTextVector(&UptimeTextTP,UptimeTextT,2,getDeviceName(),"SYSTEM_UPTIMES_TEXT",
            "System Uptimes (Readable)",SENSORS_TAB,IP_RO,60,IPS_IDLE);

    return true;
}

void WiringPiGPIO::InitConfigTab(int i, int pins, int modes, int pinNr, ISwitch PinsToInitS[][MAX_MODES])
{
    for (int j = 0; j < modes; j++)
    {
        string mode_name;
        if (pins == PINS_GPIO && modes == MODES_GPIO)
            mode_name = (j == 0) ? MODE_NOT_USED 
                : (j == 1) ? MODE_INPUT 
                : (j == 2) ? MODE_INPUT_DOWN 
                : (j == 3) ? MODE_INPUT_UP 
                : MODE_OUTPUT;
        else if (pins == PINS_1_7 && modes == MODES_1_7)
            mode_name = (j == 0) ? MODE_NOT_USED 
                : (j == 1) ? MODE_INPUT 
                : (j == 2) ? MODE_INPUT_DOWN 
                : (j == 3) ? MODE_INPUT_UP 
                : (j == 4) ? MODE_OUTPUT 
                : (i == 0) ? MODE_OUTPUT_PWM 
                : MODE_GPIO_CLOKC;
        else if (pins == PINS_8_9 && modes == MODES_8_9)
            mode_name = (j == 0) ? MODE_NOT_USED 
                : (j == 1) ? MODE_INPUT_UP 
                : MODE_OUTPUT;
        else if (pins == PINS_10to16 && modes == MODES_10to16)
            mode_name = (j == 0) ? MODE_NOT_USED 
                : (j == 1) ? MODE_INPUT 
                : MODE_OUTPUT;

        string pin_mode = "PIN_" + to_string(pinNr) + "_MODE_" + mode_name;
        IUFillSwitch(&PinsToInitS[i][j], pin_mode.c_str(), mode_name.c_str(),
            (j == 0) ? ISS_ON : ISS_OFF);
    }
    string pin_config = "PIN_" + to_string(pinNr) + "_CONFIG";
    string pin_nr = "wiPi-pin: (" + to_string(pinNr) + ")";

    IUFillSwitchVector(PinsConfigSP[pinNr]
        , &PinsToInitS[i][0], modes
        , getDeviceName(), pin_config.c_str(), pin_nr.c_str()
        , CONFIGURATION_TAB, IP_RW, ISR_1OFMANY, 60, IPS_IDLE);
}

bool WiringPiGPIO::Connect()
{
    if (isConnected()) return true; // no need to do anything if allready connected

    if (INDI::DefaultDevice::isSimulation())
    {
        IDMessage(getDeviceName(),"%s, does not support simulation\n", getDeviceName());
        return false;
    }

    IDMessage(getDeviceName(),"%s, initialized\n", getDeviceName());
    startUp = true;
    SetTimer(0);
    return true;
}

const char * WiringPiGPIO::getDefaultName()
{
        return (char *)"WiringPi GPIO";
}

bool WiringPiGPIO::updateProperties()
{
    DefaultDevice::updateProperties();

    if (isConnected())
    {
        if (startUp)
        {
            defineText(&OSInfoTP);
            defineText(&INDIInfoTP);
            defineNumber(&CPUInfoNP);
            defineNumber(&CPUStatusNP);
            defineNumber(&MemoryNP);
            defineNumber(&UptimeNP);
            defineText(&UptimeTextTP);
        }
        for (int i = 0; i < NUMBER_OF_PINS; i++)
        {
            if (startUp)
                defineSwitch(PinsConfigSP[i]);
            
            // Dynamic properties must be "redifined" to be kept in order and avoid duplicate
            deleteProperty(InputDigitalLP[i]->name);
            deleteProperty(OutputDigitalSP[i]->name);
            if (i == 1)
            {
                deleteProperty(OutputPWMNP->name);
                if (OutputPWMNP->np != NULL)
                    defineNumber(OutputPWMNP);
            }

            if (InputDigitalLP[i]->lp != NULL)
                defineLight(InputDigitalLP[i]);
            if (OutputDigitalSP[i]->sp != NULL)
                defineSwitch(OutputDigitalSP[i]);
        }
    } else
    {
        deleteProperty(OSInfoTP.name);
        deleteProperty(INDIInfoTP.name);
        deleteProperty(CPUInfoNP.name);
        deleteProperty(CPUStatusNP.name);
        deleteProperty(MemoryNP.name);
        deleteProperty(UptimeNP.name);
        deleteProperty(UptimeTextTP.name);        
        for (int i = 0; i < NUMBER_OF_PINS; i++)
        {
            deleteProperty(PinsConfigSP[i]->name);
            deleteProperty(InputDigitalLP[i]->name);
            deleteProperty(OutputDigitalSP[i]->name);
            deleteProperty(OutputPWMNP->name);
        }
    }
    return true;
}

bool WiringPiGPIO::ISNewNumber (const char *dev, const char *name, double values[], char *names[], int n)
{
    //  first check if it's for our device
    if (strcmp(dev,getDeviceName())==0)
    {
        //  This is for our device
        //  Now lets see if it's something we process here
        IDLog("WiringPiGPIO::ISNewNumber %s\n", name);
        if (strcmp(name,OutputPWMNP->name)==0)
        {
            int val = values[0];
            if (values[0] > 1024 || values[0] < 0)
            {
                IDMessage(dev, "Value: %d, not in range 0 - 1024", val);
                OutputPWMNP->s = IPS_ALERT;
            } else
            {
                IUUpdateNumber(OutputPWMNP, values, names, n);
                wiPiInterface.PWMWrite(1, val);
                OutputPWMNP->s = IPS_OK;
                IDMessage(dev, "Pin: 1, PWM set to %d", val);
            }
            IDSetNumber(OutputPWMNP, NULL);
            return true;
        }
    }
    //  if we didn't process it, continue up the chain, let somebody else give it a shot
    return DefaultDevice::ISNewNumber(dev,name,values,names,n);
}

bool WiringPiGPIO::ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
{
    if (strcmp(dev,getDeviceName())==0)
    {
        //  This is for our device
        //  Now lets see if it's something we process here
        IDLog("WiringPiGPIO::ISNewText %s\n", name);
    }
    //  if we didn't process it, continue up the chain, let somebody else give it a shot
    return DefaultDevice::ISNewText(dev,name,texts,names,n);
}

bool WiringPiGPIO::ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n)
{
    if (strcmp(dev,getDeviceName())==0)
    {
        // This is for our device
        // Now lets see if it's something we process here
        IDLog("WiringPiGPIO::ISNewSwitch %s\n", name);
        
        // This is for MAIN_CONTROL_TAB
        string fullString = name;
        string subString = "";
        try {
            subString = fullString.substr(0,13);
        } catch (const std::out_of_range& ex) {
            ; // Nothing to do, this is expected some times
        }
        if (subString == "OUTPUT_VECTOR")
        {
            string pinNrString = fullString.substr(14, fullString.length() - 14);
            int pinNr = atoi(pinNrString.c_str());
            
            IUUpdateSwitch(OutputDigitalSP[pinNr], states, names, n);
            bool high = false;
            if (OutputDigitalS[pinNr][0].s == ISS_ON)
            {
                wiPiInterface.DigiWrite(pinNr, 1);
                high = true;
            }else
                wiPiInterface.DigiWrite(pinNr, 0);

            OutputDigitalSP[pinNr]->s = IPS_OK;
            IDSetSwitch(OutputDigitalSP[pinNr],NULL);
            IDMessage(dev, "Output: %s written to pin: %d\n", ((high) ? "HIGH" : "LOW"), pinNr);
            return true;
        }
        // This is for CONFIG_TAB
        try {
            subString = fullString.substr(6,(fullString.length() - 6));
        } catch (const std::out_of_range& ex) {
            ; // Nothing to do, this is expected
        }
        if (subString == "_CONFIG" || subString == "CONFIG")
        {
            string pinNrString = (subString == "CONFIG") ? fullString.substr(4,1)
                : fullString.substr(4,2);
            int pinNr = atoi(pinNrString.c_str());

            string modeString;
            for (int i = 0; i < n; i++)
                if (states[i] == ISS_ON)
                    modeString = names[i];

            string mode = (pinNr < 10) ? modeString.substr(11, (modeString.length() - 11))
                    : modeString.substr(12, (modeString.length() - 12));
            bool notUsed = false;
            if (mode ==  MODE_NOT_USED)
            {
                deleteProperty(InputDigitalLP[pinNr]->name);
                deleteProperty(OutputDigitalSP[pinNr]->name);
                if (pinNr == 1)
                    deleteProperty(OutputPWMNP->name);
                notUsed = true;
            }
            else if (mode == MODE_INPUT || mode == MODE_INPUT_DOWN || mode == MODE_INPUT_UP)
            {
                wiPiInterface.PinMode(pinNr, INPUT);
                wiPiInterface.PullUpDn(pinNr, (mode == MODE_INPUT) ? PUD_OFF 
                        : (mode == MODE_INPUT_DOWN) ? PUD_DOWN 
                        : PUD_UP);
                IUFillLight(&InputDigitalL[pinNr], ("INPUT_" + to_string(pinNr)).c_str()
                        , mode.c_str(), IPS_IDLE);
                
                if (InputDigitalLP[pinNr] == NULL)
                    InputDigitalLP[pinNr] = new ILightVectorProperty;

                IUFillLightVector(InputDigitalLP[pinNr], &InputDigitalL[pinNr], 1
                        , dev, ("INPUT_VECTOR_" + to_string(pinNr)).c_str()
                        , ("Pin: " + to_string(pinNr)).c_str(), MAIN_CONTROL_TAB, IPS_IDLE);

                OutputDigitalSP[pinNr]->sp = NULL;
                if (pinNr == 1)
                    OutputPWMNP->np = NULL;
            }
            else if (mode == MODE_OUTPUT)
            {
                wiPiInterface.PinMode(pinNr, OUTPUT);
                IUFillSwitch(&OutputDigitalS[pinNr][0], ("OUTPUT_ACTIVE_" + to_string(pinNr)).c_str()
                        , "activate", ISS_OFF);
                IUFillSwitch(&OutputDigitalS[pinNr][1], ("OUTPUT_INACTIVE_" + to_string(pinNr)).c_str()
                        , "deactivate", ISS_ON);
                IUFillSwitchVector(OutputDigitalSP[pinNr], &OutputDigitalS[pinNr][0], 2
                        , dev, ("OUTPUT_VECTOR_" + to_string(pinNr)).c_str()
                        , ("Pin: " + to_string(pinNr)).c_str(), MAIN_CONTROL_TAB
                        , IP_RW, ISR_1OFMANY, 60, IPS_IDLE);
                
                InputDigitalLP[pinNr]->lp = NULL;
                if (pinNr == 1)
                    OutputPWMNP->np = NULL;
            }
            else if (mode == MODE_OUTPUT_PWM && pinNr == 1)
            {
                wiPiInterface.PinMode(pinNr, PWM_OUTPUT);
                IUFillNumber(&OutputPWMN, ("OUTPUT_PWM_" + to_string(pinNr)).c_str(), mode.c_str(), "%6.0f", 0, 1024, 1, 0);
                IUFillNumberVector(OutputPWMNP, &OutputPWMN, 1, dev
                        , ("OUTPUT_PWM_VECTOR_" + to_string(pinNr)).c_str(), ("Pin: " + to_string(pinNr)).c_str()
                        , MAIN_CONTROL_TAB, IP_RW, 60, IPS_IDLE);

                InputDigitalLP[pinNr]->lp = NULL;
                OutputDigitalSP[pinNr]->sp = NULL;
            }
            else if (mode == MODE_GPIO_CLOKC && pinNr == 7)
                wiPiInterface.PinMode(pinNr, GPIO_CLOCK);

            if ( (startUp && pinNr == 20) || (!startUp && !notUsed) )
            {
                startUp = false;
                updateProperties();
            }

            IDMessage(dev, "Pin: %d, is set to mode: %s\n", pinNr, mode.c_str()); 
            IUUpdateSwitch(PinsConfigSP[pinNr], states, names, n);
            PinsConfigSP[pinNr]->s = notUsed ? IPS_IDLE : IPS_OK;
            IDSetSwitch(PinsConfigSP[pinNr], NULL);
            return true;
        }
        IDLog("WiringPiGPIO::ISNewSwitch %s\n", name);
    }
    //  if we didn't process it, continue up the chain, let somebody else give it a shot
    return DefaultDevice::ISNewSwitch(dev,name,states,names,n);
}

bool WiringPiGPIO::Disconnect()
{
    if (!isConnected()) return true; // Allready disconnected

    IDMessage(getDeviceName(), "%s, disconnected", getDeviceName());
    return true;
}


void WiringPiGPIO::TimerHit()
{
    //  No need to reset timer if we are not connected anymore 
    if (!isConnected()) return;    

    wiPiUpdater.UpdateAllRead(InputDigitalLP);
    if (timer > 2500)
    {
        sysinfoUpdater.UpdateDynamicProperties();
        timer = 0;
    }
    timer += 250;
    SetTimer(250);
}
