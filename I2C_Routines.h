/********************************************************************************
*Module name:
* I2C_ROUTINES.
*
*Module version:
* 1.00. Initial version.
*
*Module description:
* Includes the necessary functions the I2C protocol needs for its correct 
* functionality
********************************************************************************/

/*
*Circular preventing errors by using macros.
*/ 
#ifndef I2C_ROUTINES_H
#define I2C_ROUTINES_H

/*Header files.*/
/********************************************************************************/
/********************************************************************************/
/*
*Not exist.
*/
#include <xc.h>


/*Type definition.*/
/********************************************************************************/
/********************************************************************************/


/*Constant definition.*/
/********************************************************************************/
/********************************************************************************/

/* Definitions for 0 and 1 in 12C routines */

#define I2CLOW 0 /* Puts pin into output/low mode */
#define I2CHIGH 1 /* Puts pin into Input/high mode */


/*Macro definition.*/
/********************************************************************************/
/********************************************************************************/

/* SDA and SCL TRIS directions */
#define SCLDir TRISBbits.TRISB0
#define SDADir TRISBbits.TRISB1
 
/* SDA and SCL LAT pins */
#define SDAOut LATBbits.LATB1
#define SCLOut LATBbits.LATB0
 
/* SDA and SCL PORT pins */
#define SDA_INPUT PORTBbits.RB1
#define SCL_INPUT PORTBbits.RB0


/*Global functions. Extern character respect this module.*/
/********************************************************************************/
/********************************************************************************/

/* Write I/O expander */
uint8_t DRIVERI2C_Write(uint8_t lAddressI2C,uint8_t lMemAdd, uint8_t lBytesNumber, uint8_t * pStartBytes);

/* Read from I/O Expander */
uint8_t DRIVERI2C_Read(uint8_t lAddressI2C,uint8_t lMemAdd, uint8_t lBytesNumber, uint8_t * pStartBytes);


/*Global variables. Extern character respect this module.*/
/********************************************************************************/
/********************************************************************************/
/*
*Not exist.
*/


#endif



/*******************************************************************************
*
*End of File.
*
*******************************************************************************/