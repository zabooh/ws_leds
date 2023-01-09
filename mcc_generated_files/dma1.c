/**
  DMA1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    dma1.c

  @Summary
    This is the generated driver implementation file for the DMA1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for DMA1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.7
        Device            :  PIC18F46K42
        Driver Version    :  1.0.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.31 and above
        MPLAB 	          :  MPLAB X 5.45
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

/**
  Section: Included Files
 */

#include <xc.h>
#include "dma1.h"
#include "../rgb_led.h"
#include "device_config.h"


extern FRAME_BUF Stripe;

//------------------------------------------------------------------------------
#define m_ARBITER_LOCK()    \
      do{ \
           asm("BANKSEL PRLOCK");\
           asm("MOVLW   0x55");\
           asm("MOVWF   (PRLOCK & 0xFF)");\
           asm("MOVLW   0xAA");\
           asm("MOVWF   (PRLOCK & 0xFF)");\
           asm("BSF     (PRLOCK & 0xFF),0");\
        }while(0)
//------------------------------------------------------------------------------
#define m_ARBITER_UNLOCK()  \
      do{ \
           asm("BANKSEL PRLOCK");\
           asm("MOVLW   0x55");\
           asm("MOVWF   (PRLOCK & 0xFF)");\
           asm("MOVLW   0xAA");\
           asm("MOVWF   (PRLOCK & 0xFF)");\
           asm("BCF     (PRLOCK & 0xFF),0");\
        }while(0)


void (*DMA1_SCNTI_InterruptHandler)(void);

/**
  Section: DMA1 APIs
 */

void DMA1_Initialize(void) {

    /* stop and clear DMA 1  */
    DMA1CON0 = 0;

    /* configure DMA 1 */

    /* 1. Program the appropriate Source and Destination addresses for the
     * transaction into the DMAxSSA and DMAxDSA registers
     */
    DMA1SSA = (uint8_t) & Stripe.Led[0][0]; /* source address */
    DMA1DSA = (uint8_t) & SPI1TXB; /* destination address */

    /* 2. Select the source memory region that is being addressed by DMAxSSA
     * register, using the SMR<1:0> bits.
     */
    DMA1CON1bits.SMR = 0b00; /* source pointer points to GPR space */

    /* 3. Program the SMODE and DMODE bits to select the addressing mode.
     */
    DMA1CON1bits.SMODE = 0b01; /* source is incremented after each transfer completion */
    DMA1CON1bits.DMODE = 0b00; /* destination remains unchanged after each transfer completion */

    /* 4. Program the Source size DMAxSSZ and Destination size DMAxDSZ
     * registers with the number of bytes to be transferred. It is recommended
     * for proper operation that the size registers be a multiple of each other.
     */
    DMA1SSZ = 150; //ORG: RGB_LED_CNT; /* source transfer size is size of array */
    DMA1DSZ = 1; /*  destination size 1 */


    /* 5. If the user desires to disable data transfers once the message has
     * completed, then the SSTP and DSTP bits in DMAxCON0 register need to be set
     */
    DMA1CON1bits.SSTP = 1; /* SIRQEN bit is cleared when source counter reloads */
    DMA1CON1bits.DSTP = 0; /* SIRQEN bit is not cleared when Destination Counter reloads */

    /* 6. If using hardware triggers for data transfer, setup the hardware
     * trigger interrupt sources for the starting and aborting DMA transfers
     * (DMAxSIRQ and DMAxAIRQ), and set the corresponding interrupt request
     * enable bits (SIRQEN and AIRQEN).
     */
    DMA1SIRQ = 21; /* HW trigger is SPI1TX */
    DMA1AIRQ = 0; /* none */

    /* enable hardware source trigger  */
    DMA1CON0bits.DMA1SIRQEN = 0;


    /* 7. Select the priority level for the DMA
     * (see Section 3.1 ?System Arbitration?) and lock the priorities
     * (see Section 3.1.1 ?Priority Lock?)
     * 0 being the highest priority and 4 being the lowest priority.
     */
    m_ARBITER_UNLOCK(); /* unlock Arbiter settings */
    MAINPR = 3;
    ISRPR = 2;
    DMA1PR = 0;
    DMA2PR = 1;
    SCANPR = 4;
    m_ARBITER_LOCK(); /* lock Arbiter settings */

    /* 8. Enable the DMA (DMAxCON0bits. EN = 1)
     */
    DMA1CON0bits.EN = 1;

    /* 9. If using software control for data transfer, set the DGO bit,
     * else this bit will be set by the hardware trigger.
     */
    //DMA1CON0bits.DGO = 1;

    /*
     * DMA Transfer is started with 
     *    MA1CON0bits.DMA1SIRQEN = 1;
     */


    // Clear Destination Count Interrupt Flag bit
    PIR2bits.DMA1DCNTIF = 0;
    // Clear Source Count Interrupt Flag bit
    PIR2bits.DMA1SCNTIF = 0;
    // Clear Abort Interrupt Flag bit
    PIR2bits.DMA1AIF = 0;
    // Clear Overrun Interrupt Flag bit
    PIR2bits.DMA1ORIF = 0;

    PIE2bits.DMA1DCNTIE = 0;
    PIE2bits.DMA1SCNTIE = 1;
    DMA1_SetSCNTIInterruptHandler(DMA1_DefaultInterruptHandler);
    PIE2bits.DMA1AIE = 0;
    PIE2bits.DMA1ORIE = 0;

    //EN enabled; SIRQEN disabled; DGO not in progress; AIRQEN disabled; 
    DMA1CON0 = 0x80;

}

//void DMA1_SelectSourceRegion(uint8_t region) {
//    DMA1CON1bits.SMR = region;
//}
//

//void DMA1_SetSourceAddress(uint24_t address) {
//    DMA1SSA = address;
//}
//
//void DMA1_SetDestinationAddress(uint16_t address) {
//    DMA1DSA = address;
//}
//

//void DMA1_SetSourceSize(uint16_t size) {
//    DMA1SSZ = size;
//}
//
//void DMA1_SetDestinationSize(uint16_t size) {
//    DMA1DSZ = size;
//}
//
//uint24_t DMA1_GetSourcePointer(void) {
//    return DMA1SPTR;
//}
//
//uint16_t DMA1_GetDestinationPointer(void) {
//    return DMA1DPTR;
//}
//
//void DMA1_SetStartTrigger(uint8_t sirq) {
//    DMA1SIRQ = sirq;
//}
//
//void DMA1_SetAbortTrigger(uint8_t airq) {
//    DMA1AIRQ = airq;
//}
//
//void DMA1_StartTransfer(void) {
//    DMA1CON0bits.DGO = 1;
//}
//
//void DMA1_StartTransferWithTrigger(void) {
//    DMA1CON0bits.SIRQEN = 1;
//}
//
//void DMA1_StopTransfer(void) {
//    DMA1CON0bits.SIRQEN = 0;
//    DMA1CON0bits.DGO = 0;
//}
//
//void DMA1_SetDMAPriority(uint8_t priority) {
//    // This function is dependant on the PR1WAY CONFIG bit
//    PRLOCK = 0x55;
//    PRLOCK = 0xAA;
//    PRLOCKbits.PRLOCKED = 0;
//    DMA1PR = priority;
//    PRLOCK = 0x55;
//    PRLOCK = 0xAA;
//    PRLOCKbits.PRLOCKED = 1;
//}

void DMA1_DMASCNTI_ISR(void) {
    // Clear the source count interrupt flag
    PIR2bits.DMA1SCNTIF = 0;

    if (DMA1_SCNTI_InterruptHandler)
        DMA1_SCNTI_InterruptHandler();
}

void DMA1_SetSCNTIInterruptHandler(void (* InterruptHandler)(void)) {
    DMA1_SCNTI_InterruptHandler = InterruptHandler;
}

extern uint8_t int_state;

void DMA1_DefaultInterruptHandler(void) {
    static uint16_t led_index = 0;
    int16_t dma_size;

    switch (int_state) {
        case 0:
            LATCbits.LATC1 = 0;
            DMA1CON0bits.EN = 0;
            led_index += LED_DMA_CNT;
            DMA1SSA = ((uint24_t) & Stripe.Led[led_index][0]);
            dma_size = LED_DMA_CNT;
            int16_t check = (int16_t) led_index + (int16_t) dma_size;
            if (check >= LED_MAX_CNT) {
                dma_size = LED_MAX_CNT - LED_DMA_CNT;
                int_state = 1;
                if (dma_size == 0)break;
            }
            DMA1SSZ = 3 * (uint16_t) dma_size;
            DMA1CON0bits.EN = 1;
            DMA1CON0bits.DMA1SIRQEN = 1;
            LATCbits.LATC1 = 1;
            break;
        case 1:
            led_index = 0;
            LATCbits.LATC1 = 0;
            int_state = 2;
            break;
        default:
            break;
    }

}
/**
 End of File
 */
