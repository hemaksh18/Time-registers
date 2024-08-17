#ifndef TIMER_DEAKIN_H
#define TIMER_DEAKIN_H

#include <Arduino.h>
#include <sam.h> 

class TIMER_DEAKIN {
public:
    // Configure the timer 
    bool config_timer(uint16_t start_count, uint16_t end_count, float resolution);

    // Get the current count value
    uint16_t get_count();

    // Wait for a time period of 0.1ms ticks
    void wait(float period);

private:
    // Configuring the timer registers
    void configure_timer_registers(uint16_t start_count, uint16_t end_count, uint16_t prescaler);
};

#endif 
