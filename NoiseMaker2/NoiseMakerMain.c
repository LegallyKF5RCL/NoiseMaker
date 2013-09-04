/* 
 * File:   Main.c
 * Author: Brian
 *
 * Created on May 20, 2013, 12:27 PM
 *
 * Project: PIC24HJ128GP202_StartUp
 * Purpose: Establish a working build for the PIC24HJ128GP202 for further dev
 *          -includes
 *              -maxing out chip speed
 *              -establish GPIO testing (full output, all high)(full digital output, blink)
 *              -establish Functioning UART interface
 */

#include <stdio.h>
#include <stdlib.h>

#include <Generic.h>
#include <outcompare.h>
#include <p24hxxxx.h>
#include <uart.h>
#include <pps.h>
#include <timer.h>

void StartUp (void);
void Chip_Go_Fast(void);

#define SONG_LENGTH 384

WORD Song[SONG_LENGTH] = {6377,7587,10121,12755,15198,12755,10121,7587
            ,10121,12755,6378,7587,10121,7587,10121,12755
            ,6378,7587,10121,12755,15198,12755,10121,7587
            ,6378,7587,10121,12755,11364,6757 ,8651,10121
            ,6377,7587 ,10121 ,12755 ,15198,12755,10121,7587
            ,10121,12755,6378,7587,10121,7587 ,10121,12755
            ,6378,7587,10121,12755,15198,12755,10121,7587
            ,6378,7587,10121,12755,11364,6757 ,8651,10121
            ,6378,7587,9560,12755,15198,12755,9560,7587
            ,9560,12755,6378,7587,9560,7587,9560,12755
            ,7587,9560,12755,15198,19084,15198,12755,9560
            ,7587,9560,6757,9560,7587,9560,12755,15198
            ,6757,8518,11364,13514,17007,13514,11364,8518
            ,11364 ,13514 ,6757,8518,11364,8518,11364,13514
            ,6757,8039,11364,13514,16077,13514,11364,13514
            ,20243,16077,13514,10121,8039,10121 ,13514,16077
            ,6377,7587,10121,12755,15198,12755,10121,7587
            ,10121,12755,6378,7587,10121,7587,10121,12755
            ,6378,7587,10121,12755,15198,12755,10121,7587
            ,6378,7587,10121,12755,11364,6757 ,8651,10121
            ,6377,7587 ,10121 ,12755 ,15198,12755,10121,7587
            ,10121,12755,6378,7587,10121,7587 ,10121,12755
            ,6378,7587,10121,12755,15198,12755,10121,7587
            ,6378,7587,10121,12755,11364,6757 ,8651,10121
            ,6378,7587,9560,12755,15198,12755,9560,7587
            ,9560,12755,6378,7587,9560,7587,9560,12755
            ,7587,9560,12755,15198,19084,15198,12755,9560
            ,7587,9560,6757,9560,7587,9560,12755,15198
            ,6757,8518,11364,13514,17007,13514,11364,8518
            ,11364 ,13514 ,6757,8518,11364,8518,11364,13514
            ,6757,8039,11364,13514,16077,13514,11364,13514
            ,20243,16077,13514,10121,8039,10121 ,13514,16077
            ,6017,7587 ,10121 ,12048 ,15198 ,12048 ,10121,7587
            ,20243,16077,13514,10121,8039,10121,13514,16077
            ,7587,9025,12048,15198,18051,15198,12048,9025
            ,24038,20243,16077,12048,10121,12048,16077,20243
            ,9025,11364,15198,18051,22727,18051,15198,11364
            ,30303,24038,20243,15198,12048,15198,20243,24038
            ,9025,11364,15198,18051,22727,18051,15198,11364
            ,20243,16077,13514,10121,8039,10121,13514,16077
            ,6017,7587 ,10121 ,12048 ,15198 ,12048 ,10121,7587
            ,20243,16077,13514,10121,8039,10121,13514,16077
            ,7587,9025,12048,15198,18051,15198,12048,9025
            ,24038,20243,16077,12048,10121,12048,16077,20243
            ,9025,11364,15198,18051,22727,18051,15198,11364
            ,30303,24038,20243,15198,12048,15198,20243,24038
            ,9025,11364,15198,18051,22727,18051,15198,11364
            ,20243,16077,13514,10121,8039,10121,13514,16077
            };



_FBS( BWRP_WRPROTECT_OFF )
_FSS( SWRP_WRPROTECT_OFF )
_FGS(GWRP_OFF & GCP_OFF )
_FOSCSEL( FNOSC_FRCPLL  & IESO_OFF )
_FOSC( POSCMD_HS & OSCIOFNC_OFF & IOL1WAY_OFF & FCKSM_CSDCMD )
_FWDT( WDTPOST_PS8192 & WDTPRE_PR32 & WINDIS_OFF & FWDTEN_OFF)
_FPOR( FPWRT_PWR128 & ALTI2C_ON )
_FICD( ICS_PGD1 & JTAGEN_OFF )

int main(int argc, char** argv) {

    TRISA = 0;
    TRISB = 0;
    
    Chip_Go_Fast();     //max out chipspeed
    StartUp();      //run a setup of chosen modules and debug states (see "StartUp.c")

    PPSUnLock;
    PPSOutput(OUT_FN_PPS_OC1,OUT_PIN_PPS_RP6);
    PPSLock;

    ConfigIntTimer1(T1_INT_PRIOR_1 &
                T1_INT_ON
                );

    OpenTimer1(T1_ON &
            T1_IDLE_CON &
            T1_GATE_OFF &
            T1_PS_1_8 &
            T1_SYNC_EXT_OFF &
            T1_SOURCE_INT
            ,
            //tone
            60975
            );

    OpenTimer2(
            //config
            T2_ON &
            T2_IDLE_CON &
            T2_GATE_OFF &
            T2_PS_1_256 &
            T2_32BIT_MODE_OFF &
            T2_SOURCE_INT
            ,
            //tempo
            14648
            );

    ConfigIntTimer2( T2_INT_PRIOR_3 & T2_INT_ON );

    //OpenOC1( (OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE) , 0x00FF, 0x00FF);

    //OC1RS = 10;

    while(1);

    return (EXIT_SUCCESS);
}

inline void Chip_Go_Fast()      /*Maxs out the chip speed. Blocking*/
{
    // Configure PLL prescaler, PLL postscaler, PLL divisor
        PLLFBD = 41; // M = 43
        CLKDIVbits.PLLPOST = 0; // N2 = 2
        CLKDIVbits.PLLPRE = 0; // N1 = 2
    // Initiate Clock Switch to Internal FRC with PLL (NOSC = 0b001)
        __builtin_write_OSCCONH(0x01);
        __builtin_write_OSCCONL(0x01);
    // Wait for Clock switch to occur
        while (OSCCONbits.COSC != 0b001);
    // Wait for PLL to lock
        while(OSCCONbits.LOCK != 1) {};
    return;
}

//////////
//ISR/////
//////////

void __attribute__ ((auto_psv))     _ISR    _T1Interrupt(void)
{
    _T1IF = 0;          //clear interrupt flag
    
    LATBbits.LATB7 ^= 0xFFFF;
          
    return;
}

void __attribute__ ((auto_psv))     _ISR    _T2Interrupt(void)
{
    _T2IF = 0;          //clear interrupt flag

    static WORD i = 0;

        OpenTimer1(T1_ON &
            T1_IDLE_CON &
            T1_GATE_OFF &
            T1_PS_1_8 &
            T1_SYNC_EXT_OFF &
            T1_SOURCE_INT
            ,
            //tone
                Song[i] / 4
            );

        i++;

        if (i == SONG_LENGTH)
        {
            i = 0;
        }
    return;
}