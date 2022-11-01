// STM32F401RE_USART.c
// Source code for USART functions

#include "STM32F401RE_USART.h"
#include "STM32F401RE_GPIO.h"
#include "STM32F401RE_RCC.h"

/*
    id2Port takes a USART_ID (index of USART peripheral) and returns a pointer
    to a USART-sized chunk of memory at the correct base address for that
    peripheral.
*/
USART_TypeDef * id2Port(int USART_ID){
    USART_TypeDef * USART;
    USART = USART1;
    switch(USART_ID){
        case(USART1_ID) :
            USART = USART1;
            break;
        case(USART2_ID) :
            USART = USART2;
            break;
    }
    return USART;
}

/*
    initUSART takes the ID of a USART peripheral and returns a pointer to the 
    USART instance. It sets up the USART peripheral and configures the pins,
    sets the appropriate configuration values, and enables the peripheral.
*/
USART_TypeDef * initUSART(int USART_ID){
    RCC->AHB1ENR.GPIOAEN = 1; // Enable GPIO port A

    USART_TypeDef * USART = id2Port(USART_ID); // Get pointer to USART peripheral

    switch(USART_ID){
        case USART1_ID :
            RCC->APB2ENR |= (1 << 4); // Set USART1EN

            // Configure pin modes as ALT function
            // USART1_TX on PA9
            // USART1_RX on PA10
            pinMode(GPIOA, 9, GPIO_ALT);
            pinMode(GPIOA, 10, GPIO_ALT);

            GPIOA->AFRH |= (0b111 << 4*2) | (0b111 << 4*1);
            break;
        case USART2_ID :
            RCC->APB1ENR |= (1 << 17); // Set USART2EN

            // Configure pin modes as ALT function
            // USART2_TX on PA2
            // USART2_RX on PA3
            pinMode(GPIOA, 2, GPIO_ALT);
            pinMode(GPIOA, 3, GPIO_ALT);

            // Configure correct alternate functions (AF07)
            GPIOA->AFRL |= (0b111 << 4*3) | (0b111 << 4*2);
            break;
    }

    /* TODO: Configure USART
       Read the datasheet and figure out the procedures. Configure for the following settings:
       * 8 data bits
       * 1 Stop bit
       * 16x oversampling frequency
       * Baud rate of 9.6 Kbps (9600 baud)
    */

    USART->CR1.UE = 1;     // Enable USART
    USART->CR1.M = 0;      // 8 bits per frame
    USART->CR2.STOP = 0b00;   // 1 stop bit
    USART->CR1.OVER8 = 0;  // oversampling by 16

    // Set baud rate to 9600 bps
    // Tx/Rx baud = (f_CK)/(8*(2-OVER8)*USARTDIV) = Tx/Rx baud = (f_CK)/(16*USARTDIV)
    if(USART_ID == USART1_ID){
      // // TODO: Finish this
      // // f_CK = 84e6 Hz on APB2 (USART1)
      // // USARTDIV = 84e6/(16*BAUD_RATE) = XX
      // // DIV_Mantissa = XX 
      // // DIV_Fraction = XX
      //   USART->BRR.DIV_Mantissa = ;
      //   USART->BRR.DIV_Fraction = ;
    }
    else if (USART_ID == USART2_ID){
      // f_CK = 42e6 on APB1 (USART2)
      // USARTDIV = 42e6/(16*BAUD_RATE) = 273.4375
      // DIV_Mantissa = 273
      // DIV_Fraction = 7
        USART->BRR.DIV_Mantissa = 273;
        USART->BRR.DIV_Fraction = 7;  // 0b0111
    }

   // TODO: Enable transmission and reception
    USART->CR1.TE = 1; // Enable transmission
    USART->CR1.RE = 1; // Enable reception


   return USART;

}

/*
    sendChar takes a pointer to a USART instance and an 8-bit data packet
    and outputs the data packet over the serial interface.
    It should wait for the transmission to be complete to exit the function.
*/
void sendChar(USART_TypeDef * USART, uint8_t data){
    // TODO: Add code to send packet and wait for transmission to complete
    USART->DR.DR = data;
    while(!USART->SR.TC); // USART->SR.TC != 1
}