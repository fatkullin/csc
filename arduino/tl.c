#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DELAY_MS 100
#define BLINK_TACTS 3	

#define SIGNAL_RED 	(1 << 5) // arduino pin 13
#define SIGNAL_YELLOW 	(1 << 4) // arduino pin 12
#define SIGNAL_GREEN 	(1 << 3) // arduino pin 11
#define SIGNAL_ALL (SIGNAL_RED | SIGNAL_YELLOW | SIGNAL_GREEN)


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


volatile State_t g_state;
volatile int g_time;

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
		return 59;
	}
}
void SwitchOnZeroTimeTo(State_t newState);
void BlinkOrSwitchTo(State_t newState);

void TrySwithState()
{
	switch (g_state)
	{
	case stRed:
		return SwitchOnZeroTimeTo(stRedYellow);
	case stRedYellow:
		return SwitchOnZeroTimeTo(stGreen);
	case stGreen:
		return SwitchOnZeroTimeTo(stGreenBlinked);
	case stGreenBlinked:
		return BlinkOrSwitchTo(stYellow);
	case stYellow:
		return SwitchOnZeroTimeTo(stRed);
	case stYellowBlinked:
		return BlinkOrSwitchTo(stYellowBlinked);
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

void SwitchTo(State_t newState)
{
	Hide(g_state);
	Show(newState);
	g_time = StateTime(newState);
	g_state = newState;
}

void SwitchOnZeroTimeTo(State_t newState)
{
	if (g_time == 0)
	{
		SwitchTo(newState);
	}
}

void BlinkOrSwitchTo(State_t newState)
{
	if (g_time == 0)
		return SwitchTo(newState);

	if (g_time % (BLINK_TACTS * 2) == 0)	// light every 2 tacts
		return Show(g_state);
		
	if (g_time % BLINK_TACTS  == 0)		// dark since 1 tact after light
		Hide(g_state);
}

void OnTimer()
{
	cli();
	g_time -= 1;
	TrySwithState();
	sei();
}

ISR(INT0_vect)
{
	if (g_state != stYellowBlinked)
		 SwitchTo(stYellowBlinked);
	else
		SwitchTo(stRed);

	_delay_ms(300);	
	EIFR |= (1 << INTF0);	
}

int main ()
{
	DDRB |= SIGNAL_ALL;
	SwitchTo(stRed);

	sei(); // Enable global interrupts
	EIMSK |= (1 << INT0); // Enable external interrupt INT0
	EICRA |= (1 << ISC11);    // Trigger INT0 on rising edge

	while (1)
	{
		_delay_ms(DELAY_MS);
		OnTimer();
	}
	return 0;
}

