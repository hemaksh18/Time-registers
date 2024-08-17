#include "TIMER_DEAKIN.h"
#include <sam.h>

// configuring the timer by setting the prescalar
bool TIMER_DEAKIN::config_timer(uint16_t start_count, uint16_t end_count, float resolution) {
    uint16_t prescaler = (uint16_t)(resolution * 10000); // Calculating prescaler based on resolution
    configure_timer_registers(start_count, end_count, prescaler);
    return true;
}

// configuring timer registers
void TIMER_DEAKIN::configure_timer_registers(uint16_t start_count, uint16_t end_count, uint16_t prescaler) {
    TcCount16* TC = (TcCount16*) TC3; // Use TC3

    // Reset the timer
    TC->CTRLA.reg = TC_CTRLA_SWRST;
    while (TC->STATUS.bit.SYNCBUSY);

    TC->CTRLA.reg = TC_CTRLA_MODE_COUNT16; //setting timer to 16-bit mode
    TC->CTRLA.reg = TC_CTRLA_MODE_COUNT16;   
    TC->CTRLA.reg |= prescaler;  // Set the prescaler
    TC->COUNT.reg = start_count; // start count    
    TC->CC[0].reg = end_count; // end

    // Enabling the timer
    TC->CTRLA.reg |= TC_CTRLA_ENABLE;

    while (TC->STATUS.bit.SYNCBUSY);// waiting for synchronisation
}

// count value returned 
uint16_t TIMER_DEAKIN::get_count() {
    TcCount16* TC = (TcCount16*) TC3;
    return TC->COUNT.reg;
}

// waiting for 0.1 ticks 
void TIMER_DEAKIN::wait(float period) {
    uint16_t ticks = (uint16_t)(period * 10); // Convert period to 0.1ms ticks

    TcCount16* TC = (TcCount16*) TC3;

    // Reset the counter
    TC->COUNT.reg = 0;
    while (TC->STATUS.bit.SYNCBUSY);

    // Wait until the counter reaches the required number of ticks
    while (TC->COUNT.reg < ticks);
}
