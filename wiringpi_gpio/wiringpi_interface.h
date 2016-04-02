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
#ifndef WiringPiInterface_H
#define WiringPiInterface_H

#include <wiringPi.h>

class WiringPiInterface
{
    public:
        WiringPiInterface() { WiringPiSetup(); };
        virtual ~WiringPiInterface();

        bool WiringPiSetup();
        bool PinMode(int pin, int mode);
        bool PullUpDn(int pin, int pud);
        bool PWMWrite(int pin, int value);
        bool DigiWrite(int pin, int value);
        int DigiRead(int pin);

    private:
};
#endif
