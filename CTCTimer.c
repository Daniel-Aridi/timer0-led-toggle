#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t tick_count = 0;

void timer0_init_ctc() {

    sei();// enable global interrupt

    // Disable power reduction for Timer0
    PRR &= ~(1 << PRTIM0);

    // Enable Timer0 Compare Match A interrupt
    TIMSK0 |= (1 << OCIE0A);

    // Set Timer0 to CTC mode
    TCCR0A |= (1 << WGM01);
    TCCR0A &= ~(1 << WGM00);
    TCCR0B &= ~(1 << WGM02);

    
    OCR0A = 199;  // Triggers interrupt every 200 ticks (approx. every 12.8 ms)

    // Set prescaler to clk/1024
    TCCR0B |= (1 << CS02) | (1 << CS00);
    TCCR0B &= ~(1 << CS01);

    
    
}

void led_gpio_init() {
    
    DDRB |= (1 << PB0);// set led pin to output
}

ISR(TIMER0_COMPA_vect) {

    tick_count++;
    if (tick_count >= 78) {
        PORTB ^= (1 << PB0);  //toggle led 
        tick_count = 0;
    }
}

int main(void) {
    led_gpio_init();
    timer0_init_ctc();

    while (1) {}
}
