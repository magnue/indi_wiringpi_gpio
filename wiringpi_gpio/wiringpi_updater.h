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
#ifndef WiringPiUpdater_H
#define WiringPiUpdater_H

#define NUMBER_OF_PINS 21

#include "wiringpi_interface.h"
#include <libindi/defaultdevice.h>

#include <vector>
using std::vector;

class WiringPiUpdater
{
    public:
        WiringPiUpdater(WiringPiInterface *wiPiInterface)
            : _wiPiInterface(wiPiInterface) {};
        virtual ~WiringPiUpdater();
        
        void UpdateAllRead(ILightVectorProperty* InputsSP[NUMBER_OF_PINS]);

    private:
        //const int _NUMBER_OF_PINS;
        WiringPiInterface *_wiPiInterface;
};
#endif
