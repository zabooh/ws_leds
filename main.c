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
        Device            :  PIC18F46K42
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
#include "gfx_mono/gfx_mono_generic.h"

#include "rgb_led.h"

FRAME_BUF Stripe;

uint32_t m_milli_sec_time = 0;
uint32_t m_milli_sec_count = 0;

volatile uint8_t int_state = 0;

uint8_t l_green = 0;
uint8_t l_red = 0;
uint8_t l_blue = 0;

uint8_t demo_state = 0;
uint16_t demo_length = 0;

void PutPixel(uint8_t x, uint8_t y);
void SetColor(uint8_t green, uint8_t red, uint8_t blue);

typedef struct {
    int16_t position;
    int16_t position_old;
    int16_t radius;
    int8_t speed;
    int8_t decimate;
    int8_t direction;
    uint8_t green;
    uint8_t red;
    uint8_t blue;
} BALL;

BALL Ball_1;
BALL Ball_2;
BALL Ball_3;

void main(void) {
    int16_t ix;


    // Initialize the device
    SYSTEM_Initialize();


    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    /* clear the entire RGB matrix */
    for (ix = 0; ix < LED_MAX_CNT; ix++) {
        Stripe.Led[ix][L_BLUE] = 0;
        Stripe.Led[ix][L_GREEN] = 0;
        Stripe.Led[ix][L_RED] = 0;
    }
    /* Trigger the SPI to start DMA */
    SPI1INTFbits.SPI1TXUIF = 1;
    __delay_ms(10);

    TRISCbits.TRISC1 = 0;
    m_milli_sec_time = 0;

    Ball_1.position = 10;
    Ball_1.position_old = 0;
    Ball_1.blue = 50;
    Ball_1.green = 0;
    Ball_1.red = 0;
    Ball_1.radius = 5;
    Ball_1.speed = 0;
    Ball_1.decimate = 0;
    Ball_1.direction = 1;


    Ball_2.position = 40;
    Ball_2.position_old = 50;
    Ball_2.blue = 00;
    Ball_2.green = 50;
    Ball_2.red = 0;
    Ball_2.radius = 5;
    Ball_2.speed = 0;
    Ball_2.decimate = 0;
    Ball_2.direction = -1;


    Ball_3.position = 100;
    Ball_3.position_old = 80;
    Ball_3.blue = 00;
    Ball_3.green = 00;
    Ball_3.red = 50;
    Ball_3.radius = 5;
    Ball_3.speed = 1;
    Ball_3.decimate = 0;
    Ball_3.direction = 1;

    ix = 1;
    while (1) {
        
        /*=== wait ===*/
        m_milli_sec_time = 40;
        do {
            uint16_t m_calib_milli_sec = 443;
            do {
                while (TMR2_HasOverflowOccured() != true);
            } while (--m_calib_milli_sec);
        } while (--m_milli_sec_time);

        /*=== Paint ===*/
        Stripe.Led[Ball_1.position_old][L_BLUE] = 0;
        Stripe.Led[Ball_1.position][L_BLUE] = Ball_1.blue;

        if (Ball_1.decimate-- == 0) {
            Ball_1.decimate = Ball_1.speed;
            Ball_1.position_old += Ball_1.direction;
            if (Ball_1.position_old >= LED_MAX_CNT) Ball_1.position_old = 0;
            Ball_1.position += Ball_1.direction;
            if (Ball_1.position >= LED_MAX_CNT) Ball_1.position = 0;
        }


        Stripe.Led[Ball_2.position_old][L_GREEN] = 0;
        Stripe.Led[Ball_2.position][L_GREEN] = Ball_2.green;
        if (Ball_2.decimate-- == 0) {
            Ball_2.decimate = Ball_2.speed;
            Ball_2.position_old += Ball_2.direction;
            
            if (Ball_2.position_old >= LED_MAX_CNT) Ball_2.position_old = 0;
            if (Ball_2.position_old < 0) Ball_2.position_old = LED_MAX_CNT;
            
            Ball_2.position += Ball_2.direction;
            if (Ball_2.position >= LED_MAX_CNT) Ball_2.position = 0;
            if (Ball_2.position < 0 ) Ball_2.position = LED_MAX_CNT;            
        }

        Stripe.Led[Ball_3.position_old][L_RED] = 0;
        Stripe.Led[Ball_3.position][L_RED] = Ball_3.red;
        if (Ball_3.decimate-- == 0) {
            Ball_3.decimate = Ball_3.speed;
            Ball_3.position_old += Ball_3.direction;
            if (Ball_3.position_old >= LED_MAX_CNT) Ball_3.position_old = 0;
            Ball_3.position += Ball_3.direction;
            if (Ball_3.position >= LED_MAX_CNT) Ball_3.position = 0;
        }
        
        /*=== Transport ===*/
        int_state = 0;
        DMA1CON0bits.EN = 0;
        DMA1SSA = ((uint24_t) & Stripe.Led[0][0]);
        DMA1SSZ = (3 * LED_DMA_CNT);
        DMA1CON0bits.EN = 1;
        LATCbits.LATC1 = 1;
        DMA1CON0bits.DMA1SIRQEN = 1;

    }
}

/**
End of File
 */