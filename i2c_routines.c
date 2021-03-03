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



/*Header files.*/
/********************************************************************************/
/********************************************************************************/
#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include "I2C_Routines.h"
 

#define _XTAL_FREQ 4000000

/*Global variables of module.*/
/********************************************************************************/
/********************************************************************************/

/* Buffer for I2C reception */
uint8_t RX_BUFFER_I2C2[6];

/* Buffer for I2C transmission */
uint8_t TX_BUFFER_I2C3[5];



/*Local functions at module*/
/********************************************************************************/
/********************************************************************************/
void I2C__P_StartCondition(void);
void I2C__P_ReStartCondition(void);
uint8_t I2C__P_SendByte(uint8_t Byte);
uint8_t I2C__P_GetACK(void);
void I2C__P_StopCondition(void);
void I2C__P_Clock(void);
uint8_t I2C__P_GetByte(void);
void I2C__P_SendACK(void);
void I2C__P_NACK(void);






/*******************************************************************************
*Function description:
* Generates the start condition of the I2C protocol.
*
*Input parameters:
* Not has input parameters.
*
*Return:
* Not return.
*
*Notes:
* It will be executed before any data transmission
********************************************************************************/
void I2C__P_StartCondition(void)
{ 
 /*
 Local variables
 */
 
 
 /* No variables */
 
 
 /*
 Procedure
 */
 
 
 /*
 To start communication
 */
 
 
 /*Pull down de data signal*/
 SDADir = I2CLOW;
 
 /*Delay to give it time to settle*/
 __delay_us(1);
 
 /*Pull down the clock signal*/
 SCLDir = I2CLOW;
}/* void I2C__P_StartCondition(void) */



/*******************************************************************************
*Function description:
* Generates the start condition of the I2C protocol in the read routine after
* relinquishing the SDA and SCL signals.
*
*Input parameters:
* Not has input parameters.
*
*Return:
* Not return.
*
*Notes:
* It will be executed after sending lMemAdd in read routine.
********************************************************************************/
void I2C__P_ReStartCondition(void)
{ 
 /*
 Local variables
 */
 
 
 /* No variables */
 
 
 /*
 Procedure
 */
 
 
 /*
 To restart communication
 */
 
 
 /* Pull up data line */
 SDADir = I2CHIGH;
 
 /* Give it time to settle */
 __delay_us(1);
 
 /* Pull up clock line */
 SCLDir = I2CHIGH; 
 
 /* Give it time to settle */
 __delay_us(1);
 
 /* Pull down data line */
 SDADir = I2CLOW; 
 
 /* Give it time to settle */
 __delay_us(1);
 
 /* Pull up clock line */
 SCLDir = I2CLOW; 

} /* void I2C__p_ReStartCondition(void) */


/*******************************************************************************
*Function description:
* Writes the data held in TX_BUFFER_I2C3[5] to the I/O Expander
*
*Input parameters:
* lAddressI2C: Address to write data to
* pStartBytes: Pointer to buffer
* lBytesNumber: Number of bytes to send
* lMemAdd: Command for polarity inversion of I/O expander to allow writting
*
*Return:
* Returns a 1 to ACK
*
*Notes:
*.
********************************************************************************/
uint8_t DRIVERI2C_Write(uint8_t lAddressI2C,uint8_t lMemAdd, uint8_t lBytesNumber, uint8_t * pStartBytes)
{ 
 /*
 Local variables
 */
 
 
 /* No variables */
 
 
 /*
 Procedure
 */
 
 
 /*
 To write bytes
 */
 
 
 /* Generates start condition to initiate communication */
 I2C__P_StartCondition();

 /* Sends I/O expander address, shifting 1 bit into position */
 I2C__P_SendByte((uint8_t)(lAddressI2C<<1));

 /* Checks if ACK is received */
 if(!I2C__P_GetACK())
 {
 /* If not, stops communication and returns 0 for NACK */
 I2C__P_StopCondition();
 return(0);
 }
 
 /* Sends lMemAdd to allow writting */
 I2C__P_SendByte(lMemAdd);

 /* Checks if ACK is received */
 if(!I2C__P_GetACK())
 {
 /* If not, stops communication and returns 0 for NACK */ 
 I2C__P_StopCondition();
 return(0);
 }

 /* Initiates reverse counting to send lBytesNumber bytes */
 while(lBytesNumber--)
 {
 /* Sends the pointer to TX_BUFFER_I2C3[5] */
 I2C__P_SendByte(*pStartBytes);

 /* Checks if ACK is received */
 if(!I2C__P_GetACK())
 {
 /* If not, stops communication and returns 0 for NACK */ 
 I2C__P_StopCondition();
 return(0);
 }
 /* Selects the next bit to be sent */
 pStartBytes++;
 }
 /* Stops communication after the writting is complete */
 I2C__P_StopCondition();

 return(1);


} /* uint8_t I2C__P_WriteIOExpander(uint8_t lAddressI2C,uint8_t lMemAdd, uint8_t lBytesNumber, uint8_t * pStartBytes) */



/*******************************************************************************
*Function description:
* Sends the bytes specified in the write function
*
*Input parameters:
* Byte: Variable in which the bytes are held to be sent in write function
*
*Return:
* Returns 1 to ACK
*
*Notes:
*.
********************************************************************************/
uint8_t I2C__P_SendByte(uint8_t Byte)
{
 /*
 Local variables
 */
 
 
 /* Variable for counting the bits sent*/
 uint8_t count;
 
 
 /*
 Procedure
 */
 
 
 /*
 To send byte
 */
 
 
 /* Loop used for sending eight bits, one at a time */
 for(count = 0;count < 8; count++)
 {
 /* If the byte received is 0 holds the SDA signal down */
 if((Byte&0x80) == 0)
 {
 SDADir = I2CLOW;
 }
 /* Else release the line so data can be received */
 else
 {
 SDADir = I2CHIGH;
 }
 /* Saves the data in the variable and shifts next bit into position */
 Byte = (uint8_t)(Byte <<1);
 /* Delay and clock pulse to ensure the data reception */
 __delay_us(1);
 I2C__P_Clock();
 }
 /* Release data line for ACK */
 SDADir = I2CHIGH;
 return(1);
} 


 
/*******************************************************************************
*Function description:
* Checks the status of SDA LAT and sends ACK or NACK depending of that status
*
*Input parameters:
* Has not input parameters
*
*Return:
* Returns 1 to ACK
* Return 0 to NACK
*
*Notes:
*.
********************************************************************************/
uint8_t I2C__P_GetACK(void)
{
 /*
 Local variables
 */
 
 
 /* No variables */
 
 
 /*
 Procedure
 */
 
 
 /*
 To check SDA and send ACK or NACK
 */


 /* Samples SDA LAT */
 if(SDAOut == 1)
 {
 /*If 1, return NACK*/
 return(0);
 }
 else
 {
 /* Else, pulses de clock and returns ACK */
 I2C__P_Clock();
 __delay_us(1); //-- Minimum Clock Low Time
 }
 
 return(1);
 
} /* uint8_t I2C__P_GetACK(void) */



/*******************************************************************************
*Function description:
* Generates the stop condition to end communication
*
*Input parameters:
* Has not input parameters
*
*Return:
* Has no return
*
*Notes:
*.
********************************************************************************/
void I2C__P_StopCondition(void)
{
 /*
 Local variables
 */
 
 
 /* No variables */
 
 
 /*
 Procedure
 */
 
 
 /*
 To generate stop condition
 */


 /* Pull down SDA line */
 SDADir = I2CLOW;
 /* Pull down SCL line */
 SCLDir = I2CLOW;
 /* Give it time to settle */ 
 __delay_us(1);
 /* Releases SCL line */
 SCLDir = I2CHIGH;
 /* Give it time to settle */
 __delay_us(1);
 /* Releases SDA line */
 SDADir = I2CHIGH;

} /* void I2C__P_StopRoutine(void) */



/*******************************************************************************
*Function description:
* Pulses SCL to generate a clock signal
*
*Input parameters:
* Has not input parameters
*
*Return:
* Has no return
*
*Notes:
*.
********************************************************************************/
void I2C__P_Clock(void)
{
 /*
 Local variables
 */
 
 
 /* No variables */
 
 
 /*
 Procedure
 */
 
 
 /*
 To generate clock pulses
 */


 
 /* Delay to avoid signals to bed mixed*/
 __delay_us(1);
 /* Pulls up SCL line */
 SCLDir = I2CHIGH;
 /* Give it time to settle */
 __delay_us(2);
 /* Pulls down SCL line */
 SCLDir=I2CLOW;
/* Give it time to settle */
 __delay_us(1);
 
} /* void I2C__P_Clock(void)*/



/*******************************************************************************
*Function description:
* Reads the data received in the I/O expander and stores it in RX_BUFFER_I2C2[5]
*
*Input parameters:
* lAddressI2C: Address to read data from
* pStartBytes: Pointer to buffer
* lBytesNumber: Number of bytes to read
* lMemAdd: Command for enabling the I/O expander data bank to be read
*Return:
* Return 1 for ACK
*
*Notes:
*.
********************************************************************************/
uint8_t DRIVERI2C_Read(uint8_t lAddressI2C,uint8_t lMemAdd, uint8_t lBytesNumber, uint8_t * pStartBytes)
{
 /*
 Local variables
 */
 
 
 /* Variable used for counting the number of bits sent */
 uint8_t lIndex;
 
 
 /*
 Procedure
 */
 
 
 /*
 To read data from I/O expander
 */


 /* Generates start condition */
 I2C__P_StartCondition();

 /* Sends expander address, shifting 1 bit into position */
 I2C__P_SendByte((uint8_t)(lAddressI2C << 1));

 /* Checks if ACK is received */
 if(I2C__P_GetACK()==0)
 {
 /* If not, stops communication and return 0 for NACK */
 I2C__P_StopCondition();
 return(0); 
 }

 /* Sends lMemAdd to access the data bank to be read */
 I2C__P_SendByte(lMemAdd);

 /* Checks if ACK is received */
 if(I2C__P_GetACK()==0)
 {
 /* If not, stops communication and return 0 for NACK */
 I2C__P_StopCondition();
 return(0);
 }
 
 /*Begins reading*/
 
 
 /* Restarts I2C to begin the reading */
 I2C__P_ReStartCondition();

 /* Sends expander addres, shifting 1 bit into position and does an OR with 1 bit to enable reading */
 I2C__P_SendByte((lAddressI2C << 1) | (0x01));

 /* Checks if ACK is received */
 if(I2C__P_GetACK()==0)
 {
 /* If not, stops communication and return 0 for NACK */
 I2C__P_StopCondition();
 return(0);
 }

 /* Generates a reverse counting loop to send lBytesNumber bytes*/ 
 for(lIndex=0;lIndex<lBytesNumber;lIndex++ )
 {
 /* Equals the RX_BUFFER_I2C2[5] to the bytes received in I2C__P_GetByte(); function */
 pStartBytes[lIndex]=I2C__P_GetByte();

 /* While the lIndex countig is below lBytesNumber - 1 sends ACK for each byte received */
 if(lIndex<(lBytesNumber-1))
 {
 /* Forces and ACK if the condition is fulfilled */
 I2C__P_SendACK();
 }
 /* Else sends NACK */ 
 else
 {
 /* Forces NACK if else */ 
 I2C__P_NACK();
 }
 }

 /* Stops communitacion after the reading is completed */ 
 I2C__P_StopCondition();

 return(1);

} /* uint8_t I2C__P_ReadIOExpander(uint8_t lAddressI2C,uint8_t lMemAdd, uint8_t lBytesNumber, uint8_t * pStartBytes) */



/*******************************************************************************
*Function description:
* Gets the bytes to be read from the SDA port
*
*Input parameters:
* Has not input parameters
*
*Return:
* Byte: variable used to store the read bytes in
*
*Notes:
*.
********************************************************************************/
uint8_t I2C__P_GetByte(void)
{
 /*
 Local variables
 */
 
 
 /* Variable used for counting the number of bytes read */
 uint8_t count, tmp1;
 
 /* Variable used to store the read bytes in */
 uint8_t Byte = 0;
 
 
 /*
 Procedure
 */
 
 
 /*
 To get the bytes held in SDA port
 */

 
 /* Releases SDA line to allow the bytes to be read */
 SDADir=I2CHIGH; 
 
 /* Initiares a loop to read 8 bits */
 for(count=0;count<8;count++)
 {
 /* Equals the Byte variable to Byte OR data held in SDA port. ****fs the the next bits into position as the counting goes on */
     tmp1 = SDA_INPUT;
     Byte = (uint8_t)(Byte | ( SDA_INPUT << (7-count))); //-- Shift next bit into position
 
 /* Delay and clock pulse to ensure the data reception */
 __delay_us(1);
 I2C__P_Clock(); //-- Pulse the clock 
 }
 /* Returns the data held in Byte variable to be used in I2C__P_ReadIOExpander function */
 return(Byte);

} /* I2C__P_GetByte */




/*******************************************************************************
*Function description:
* Forces an ACK to read incoming data
*
*Input parameters:
* Has not input parameters
*
*Return:
* Has no return
*
*Notes:
*.
********************************************************************************/
void I2C__P_SendACK(void)
{ 
 /*
 Local variables
 */
 
 
 /* No variables */
 
 
 /*
 Procedure
 */
 
 
 /*
 To generate ACK signal
 */
 
 /* Pulls down SDA, generating ACK */
 SDADir=I2CLOW;
 
 /* Give it time to settle */
 __delay_us(1);
 
 /* Pulse the clock */
 I2C__P_Clock();
 
 /* Release SDA */
 SDADir=I2CHIGH;
 
 /* Give it time to settle */
 __delay_us(1);

} /* void I2C__P_SendACK(void) */ 



/*******************************************************************************
*Function description:
* Forces a NACK to end communication
*
*Input parameters:
* Has not input parameters
*
*Return:
* Has no return
*
*Notes:
*.
********************************************************************************/
void I2C__P_NACK(void)
{ 
 /*
 Local variables
 */
 
 
 /* No variables */
 
 
 /*
 Procedure
 */
 
 
 /*
 To generate NACK signal
 */
 
 
 /* Release SDA*/
 SDADir=I2CHIGH;
 
 /* Give it time to settle */ 
 __delay_us(1);
 
 /* Pulse the clock */
 I2C__P_Clock();
 
 /* Pulls down SDA, genrating ACK */
 SDADir=I2CLOW;
 
 /* Give it time to settle */
 __delay_us(1);

} /* void I2C__P_NACK(void) */



/*******************************************************************************
*Function description:
* Defines the delay in microseconds
*
*Input parameters:
* delay_us: The number of microseconds to wait during the delay
*
*Return:
* Has no return
*
*Notes:
*.
********************************************************************************/
/*extern void __delay_us(uint32_t delay_us)
{ 
 

 uint32_t DelayStartTime;


 DelayStartTime = ReadCoreTimer();
 

 while((ReadCoreTimer() - DelayStartTime) < (delay_us * CORE_TIMER_MICROSECONDS));
}*/


/*******************************************************************************
*
*End of File.
*
*******************************************************************************/

