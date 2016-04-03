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
#include "wiringpi_interface.h"

WiringPiInterface::~WiringPiInterface()
{

}

bool WiringPiInterface::WiringPiSetup()
{
    if (wiringPiSetup() == -1)
        return false;
    return true;
}

bool WiringPiInterface::PinMode(int pin, int mode)
{
    if (mode != INPUT && mode != OUTPUT && mode != PWM_OUTPUT)
        return false;
    pinMode(pin, mode);
    return true;
}

bool WiringPiInterface::PullUpDn(int pin, int pud)
{
    if (pud != PUD_OFF && pud != PUD_UP && pud != PUD_DOWN)
        return false;
    pullUpDnControl(pin, pud);
    return true;
}

bool WiringPiInterface::PWMWrite(int pin, int value)
{
    if (pin != 1 || value < 0 || value > 1024)
        return false;
    pwmWrite(pin, value);
    return true;
}

bool WiringPiInterface::DigiWrite(int pin, int value)
{
    if (value < 0)
        return false;
    digitalWrite(pin, value);
    return true;
}

int WiringPiInterface::DigiRead(int pin)
{
    return digitalRead(pin);   
}
