#include <avr/io.h>
#include <util/delay.h>

#define DELAY_MS 100
#define LED_PIN 	(1 << 5)
#define SIGNAL_RED 	(1 << 5) // arduino pin 13
#define SIGNAL_YELLOW 	(1 << 4) // arduino pin 12
#define SIGNAL_GREEN 	(1 << 3) // arduino pin 11

#define FLAG_BLINKED 	(1)	 // should not be equal to any signal



typedef enum
{
	stRed = SIGNAL_RED,
	stRedYellow = SIGNAL_RED + SIGNAL_YELLOW,
	stGreen = SIGNAL_GREEN,
	stGreenBlinked =  FLAG_BLINKED + SIGNAL_GREEN,
	stYellow = SIGNAL_YELLOW,
	stYellowBlinked = SIGNAL_YELLOW | FLAG_BLINKED
} State_t;


State_t g_state;
int g_time;

int StateTime(State_t state)
{
    	switch (state)
	{
	case stRed:
		return 80;
	case stRedYellow:
		return 20;
	case stGreen:
		return 50;
	case stGreenBlinked:
		return 20;
	case stYellow:
		return 30;
	default:
		return 10;
	}
}
void SwitchOnZeroTime(State_t newState);
void BlinkOrSwitch(State_t newState);

void TrySwithState()
{
	switch (g_state)
	{
	case stRed:
		return SwitchOnZeroTime(stRedYellow);
	case stRedYellow:
		return SwitchOnZeroTime(stGreen);
	case stGreen:
		return SwitchOnZeroTime(stGreenBlinked);
	case stGreenBlinked:
		return BlinkOrSwitch(stYellow);
	case stYellow:
		return SwitchOnZeroTime(stRed);
	case stYellowBlinked:
		return BlinkOrSwitch(stYellowBlinked);
	}
}
	


void Hide(State_t state)
{
	PORTB &= ~(state & ~FLAG_BLINKED);	// exclude FLAG_BLINKED from state
}

void Show(State_t state)
{
	PORTB |= (state & ~FLAG_BLINKED);
}

void SwitchOnZeroTime(State_t newState)
{
	if (g_time == 0)
	{
		Hide(g_state);
		Show(newState);
		g_time = StateTime(newState);
		g_state = newState;
	}
}

void BlinkOrSwitch(State_t newState)
{
	if (g_time == 0)
		return SwitchOnZeroTime(newState);
	
	if (g_time % 4 == 0)		// show every 4 tick
		Show(g_state);
		
	if (g_time % 2 == 0)		// hide since 2 tick after show
		Hide(g_state);
}

int main ()
{
    DDRB |= SIGN;
    while (1)
    {
	PORTB |= LED_PIN;
	_delay_ms(1000);
	PORTB &= ~LED_PIN;
	_delay_ms(1000);	
    }
    return 0;
}

