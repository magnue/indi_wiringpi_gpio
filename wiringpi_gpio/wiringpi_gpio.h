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
#ifndef WiringPiGPIO_H
#define WiringPiGPIO_H

#include "systeminfo_updater.h"
#include "wiringpi_updater.h"
#include <libindi/defaultdevice.h>

#include <vector>
using std::vector;

#define NUMBER_OF_PINS  21

#define PINS_GPIO       10
#define MODES_GPIO      5
#define PINS_1_7        2
#define MODES_1_7       6
#define PINS_8_9        2
#define MODES_8_9       3
#define PINS_10to16     7
#define MODES_10to16    3
#define MAX_MODES       MODES_1_7

#define MODE_NOT_USED   "not-used"
#define MODE_INPUT      "input"
#define MODE_INPUT_DOWN "input-pull-down"
#define MODE_INPUT_UP   "input-pull-up"
#define MODE_OUTPUT     "output"
#define MODE_OUTPUT_PWM "output-pwm"
#define MODE_GPIO_CLOKC "gpio-clock"

class WiringPiGPIO : public INDI::DefaultDevice
{

    public:
        WiringPiGPIO();
        virtual ~WiringPiGPIO();

        virtual bool ISNewNumber (const char *dev, const char *name, double values[], char *names[], int n);
        virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n);
        virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int num);
        virtual bool saveConfigItems(FILE *fp);
        virtual bool initProperties();
        virtual const char *getDefaultName();
        virtual bool updateProperties();

    protected:
        virtual bool Connect();
        virtual bool Disconnect();
        virtual void TimerHit();

        /**************************************************************************
         * SENSORS TAB
         * ************************************************************************/
        
        // CPU status
        INumberVectorProperty CPUStatusNP;
        INumber CPUStatusN[2];
         
        // Memory Property
        INumberVectorProperty MemoryNP;
        INumber MemoryN[5];
                 
        // Uptime Property
        INumberVectorProperty UptimeNP;
        INumber UptimeN[2];
                 
        // Uptime (in Text format) Property
        ITextVectorProperty UptimeTextTP;
        IText UptimeTextT[2];

    private:
        int timer{0};
        bool startUp{true};

        // The GPIO interface and updater
        WiringPiInterface wiPiInterface;
        WiringPiUpdater wiPiUpdater {&wiPiInterface};

        // The System info interface and updater
        SystemInfo sysInfo;
        SystemInfoUpdater sysinfoUpdater {&CPUStatusNP, CPUStatusN, &MemoryNP, MemoryN, 
            &UptimeNP, UptimeN, &UptimeTextTP, UptimeTextT};

        /**************************************************************************
         * MAIN TAB
         * ************************************************************************/
        ILightVectorProperty* InputDigitalLP    [NUMBER_OF_PINS];
        ILight InputDigitalL                    [NUMBER_OF_PINS];

        ISwitchVectorProperty* OutputDigitalSP  [NUMBER_OF_PINS];
        ISwitch OutputDigitalS                  [NUMBER_OF_PINS]    [2];

        INumberVectorProperty* OutputPWMNP;
        INumber OutputPWMN;
        
        /**************************************************************************
         * CONFIGURATION TAB
         * ************************************************************************/
        
        /** GPIO Pins Config Property
         * 0 -> 7 - (1 & 7) =   NOT_USED - INN - INN_PULL_DOWN - INN_PULL_UP - OUT
         * 1                =   NOT_USED - INN - INN_PULL_DOWN - INN_PULL_UP - OUT - OUT_PWM
         * 7                =   NOT_USED - INN - INN_PULL_DOWN - INN_PULL_UP - OUT - GPIO_CLOCK
         * 8 & 9            =   NOT_USED - INN_PULL_UP - OUT
         * 10 -> 14         =   NOT_USED - INN - OUT
         * 15 & 16          =   NOT_USED - INN - OUT (Must disable serial console)
         * 17 -> 20         =   NOT_USED - INN - INN_PULL_DOWN - INN_PULL_UP - OUT (Rev.2 only)
         *
         * see http://wiringpi.com/pins/special-pin-functions/ for special pins function
         */
        vector<ISwitchVectorProperty*> PinsConfigSP;
        ISwitch PinsConfigGPIOS     [PINS_GPIO]     [MAX_MODES];
        ISwitch PinsConfig1_7S      [PINS_1_7]      [MAX_MODES];
        ISwitch PinsConfig8_9S      [PINS_8_9]      [MAX_MODES];
        ISwitch PinsConfig10to16S   [PINS_10to16]   [MAX_MODES];
        void InitConfigTab(int i, int pins, int modes, int pinNr, ISwitch PinsToInitS[][MAX_MODES]);

        /**************************************************************************
         * SYSTEM TAB
         * ************************************************************************/
        
        // OS info
        ITextVectorProperty OSInfoTP;
        IText OSInfoT[3];

        // INDI info
        ITextVectorProperty INDIInfoTP;
        IText INDIInfoT[2];

        // CPU info
        INumberVectorProperty CPUInfoNP;
        INumber CPUInfoN[4];
};
#endif
