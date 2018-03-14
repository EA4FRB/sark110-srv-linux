/**
  ******************************************************************************
  * @file    device.h
  * @author  Melchor Varela - EA4FRB
  * @version V1.0
  * @date    13-March-2018
  * @brief   Platform dependent definitions
  ******************************************************************************
  * @copy
  *
  *  This file is a part of the "SARK110 Antenna Vector Impedance Analyzer" software
  *
  *  "SARK110 Antenna Vector Impedance Analyzer" software is free software: you can redistribute it
  *  and/or modify it under the terms of the GNU General Public License as
  *  published by the Free Software Foundation, either version 3 of the License,
  *  or (at your option) any later version.
  *
  *  "SARK110 Antenna Vector Impedance Analyzer" software is distributed in the hope that it will be
  *  useful,  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with "SARK110 Antenna Vector Impedance Analyzer" firmware.  If not,
  *  see <http://www.gnu.org/licenses/>.
  *
  * <h2><center>&copy; COPYRIGHT 2011-2018 Melchor Varela - EA4FRB </center></h2>
  *  Melchor Varela, Madrid, Spain.
  *  melchor.varela@gmail.com
  */

/** @addtogroup DRV
  * @{
  */

/** @addtogroup DEVICE
  * @{
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_H__
#define __DEVICE_H__

/* Includes ------------------------------------------------------------------*/
#if defined(OS_WINDOWS)
#include <windows.h>
#endif

/* Exported types ------------------------------------------------------------*/

/** @defgroup DEVICE_Exported_Types
  * @{
  */
/* Acceptable types from MISRA-C specifying signedness and size.*/

typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned long           uint32;

typedef signed char             int8;
typedef signed short            int16;
typedef signed long             int32;

typedef unsigned char           bool;

#define FLOATING float

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup DEVICE_Exported_Macro
  * @{
  */

/* Get 8 bits of a 16 bit value. */
#define LO8(x)                  ((uint8) (x))
#define HI8(x)                  ((uint8) ((x) >> 8))

/* Get 16 bits of a 32 bit value. */
#define LO16(x)                 ((uint16) (x))
#define HI16(x)                 ((uint16) ((uint32)(x) >> 16))

#define M_PI		3.14159265358979323846

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */

#endif

/**
  * @}
  */

/**
  * @}
  */

/************* (C) COPYRIGHT 2011-2018 Melchor Varela - EA4FRB *****END OF FILE****/



