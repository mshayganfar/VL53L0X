/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F25Q10
        Driver Version    :  2.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include "mcc_generated_files/mcc.h"
#include "VL53L0X.h"
#include "I2C_Routines.h"


uint16_t millisecond_count=0;
char buffer[50]="This is a test";
uint16_t sensor_range1 =0;
uint16_t sensor_range2 =0;
uint8_t tmp;
uint16_t tmp16=0;
uint16_t tmp2=0;
uint32_t tmp32=0x01020304;
char buffer[50];
uint8_t result1, result2;

extern uint8_t address;
//Main application
 
void main(void)
{
    
    // Initialize the device
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();
    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();
    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();
    RANGE1_EN_SetLow();
    RANGE2_EN_SetLow();
 
    //Enabling Sensor 1
    RANGE1_EN_SetHigh();
   
    setTimeout(500);
    result1 = init(1);
    __delay_ms(2);
    setAddress(0x50);
    
    __delay_ms(2);
    DRIVERI2C_Read(0x50, 0x8A, 1, &tmp);
    startContinuous(0);
    
    
   //Enabling Sensor 2
    RANGE2_EN_SetHigh();
    address = 0x29;
    //Setting Timeout
    result2 = init(1);
    __delay_ms(2);
    setAddress(0x60);
    
    __delay_ms(2);
    DRIVERI2C_Read(0x60, 0x8A, 1, &tmp);
    
    startContinuous(0);
    
    
    while (1)
    {
        __delay_ms(10);
        address=0x50;
        sensor_range1=readRangeContinuousMillimeters();
            
        __delay_ms(10);
        address=0x60;
        sensor_range2=readRangeContinuousMillimeters();
        
        sprintf(buffer,"Sensor1 Range is: %d   Sensor2 Range is %d \r\n", sensor_range1, sensor_range2);
        printf("%s", buffer);
        //Delay 200ms
        __delay_ms(200);
    }
}
/**
 End of File
*/