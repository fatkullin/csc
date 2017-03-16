#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 1000
#define LED_PIN 5

int main ()
{
    DDRB |= 1 << LED_PIN;
    while (1)
    {
	PORTB |= 1 << LED_PIN;
	_delay_ms(BLINK_DELAY_MS);
	PORTB &= ~(1 << LED_PIN);
	_delay_ms(BLINK_DELAY_MS);	
    }
    return 0;
}

