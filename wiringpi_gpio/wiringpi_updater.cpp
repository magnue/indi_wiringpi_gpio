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
#include "wiringpi_updater.h"
#include <string.h>

using std::string;

WiringPiUpdater::~WiringPiUpdater()
{

}

void WiringPiUpdater::UpdateAllRead(ILightVectorProperty* InputsLP[NUMBER_OF_PINS])
{
    for (int i = 0; i < NUMBER_OF_PINS; ++i)
    {
        if (InputsLP[i]->lp != NULL)
        {
            ILight* light = InputsLP[i]->lp;
            string lightPropName = InputsLP[i]->name;
            int pinNr = atoi(lightPropName.substr(13,lightPropName.length()).c_str()); 
            if (_wiPiInterface->DigiRead(pinNr) == 1)
                light->s = IPS_OK;
            else
                light->s = IPS_ALERT;
            InputsLP[i]->s = IPS_OK;
            IDSetLight(InputsLP[i], NULL);
        }
    }
}
