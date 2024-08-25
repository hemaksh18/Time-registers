#include "TIMER_DEAKIN.h"
#include <sam.h>

// setup GCLK for TC3 timer
void TIMER_DEAKIN::setup_GCLK() {
    
    PM->APBCMASK.reg |= PM_APBCMASK_TC3;  // Enable the TC3 module clock in power manager

  
    GCLK->GENDIV.reg = GCLK_GENDIV_ID(4) | GCLK_GENDIV_DIV(48);  // to get 1 MHz clock 
    while (GCLK->STATUS.bit.SYNCBUSY);  // waiting for synchronisation

    GCLK->GENCTRL.reg = GCLK_GENCTRL_ID(4) | GCLK_GENCTRL_SRC_DFLL48M | GCLK_GENCTRL_GENEN;  // Use GCLK4 for TC3, 
    while (GCLK->STATUS.bit.SYNCBUSY);  

    
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TCC2_TC3 | GCLK_CLKCTRL_GEN_GCLK4 | GCLK_CLKCTRL_CLKEN; // clock control ID for TC3
    while (GCLK->STATUS.bit.SYNCBUSY);  
}

// Configuring the timer by setting the prescaler
bool TIMER_DEAKIN::config_timer(uint16_t start_count, uint16_t end_count, float resolution) {

    setup_GCLK(); // calling the function that configures GCLK for TC3 
    
    uint16_t prescaler;

    // Calculate prescaler based on resolution 
    if (resolution <= 1.0) {
        prescaler = TC_CTRLA_PRESCALER_DIV1;
    } else if (resolution <= 2.0) {
        prescaler = TC_CTRLA_PRESCALER_DIV2;
    } else if (resolution <= 4.0) {
        prescaler = TC_CTRLA_PRESCALER_DIV4;
    } else if (resolution <= 8.0) {
        prescaler = TC_CTRLA_PRESCALER_DIV8;
    } else if (resolution <= 16.0) {
        prescaler = TC_CTRLA_PRESCALER_DIV16;
    } else if (resolution <= 64.0) {
        prescaler = TC_CTRLA_PRESCALER_DIV64;
    } else if (resolution <= 256.0) {
        prescaler = TC_CTRLA_PRESCALER_DIV256;
    } else {
        prescaler = TC_CTRLA_PRESCALER_DIV1024; 
    }

    configure_timer_registers(start_count, end_count, prescaler);
    return true;
}

// Configuring timer registers
void TIMER_DEAKIN::configure_timer_registers(uint16_t start_count, uint16_t end_count, uint16_t prescaler) {
    TcCount16* TC = (TcCount16*) TC3; 

    // Reset the timer
    TC->CTRLA.reg = TC_CTRLA_SWRST;
    while (TC->STATUS.bit.SYNCBUSY); 

    // Configure the timer
    TC->CTRLA.reg = TC_CTRLA_MODE_COUNT16; //setting timer to 16-bit mode
    TC->CTRLA.reg |= prescaler; // Set the prescaler
    while (TC->STATUS.bit.SYNCBUSY); 

    TC->COUNT.reg = start_count; // start count
    while (TC->STATUS.bit.SYNCBUSY); 

    TC->CC[0].reg = end_count; // ending the count
    while (TC->STATUS.bit.SYNCBUSY); 

    TC->CTRLA.reg |= TC_CTRLA_ENABLE; // Enable the timer
    while (TC->STATUS.bit.SYNCBUSY); // waiting for synchronization
}

// Count value returned
uint16_t TIMER_DEAKIN::get_count() {
    TcCount16* TC = (TcCount16*) TC3;
    return TC->COUNT.reg;
}

// Waiting for a specific period (here 0.1 ticks)
void TIMER_DEAKIN::wait(float period) {
    uint16_t ticks = (uint16_t)(period * 10); // Convert period to 0.1ms ticks

    TcCount16* TC = (TcCount16*) TC3;

    // Reset the counter
    TC->COUNT.reg = 0;
    while (TC->STATUS.bit.SYNCBUSY); 

    
    while (TC->COUNT.reg < ticks); // waiting till counter reaches ticks 
}
