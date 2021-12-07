/**
 * @author StevoMc
 * @date 07.12.2021
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 16000000UL

/*
 ?   GET_BIT
 *  -> Macro for retrieving bit from given position
 */
#define GET_BIT(x, pos) (((x) & (1 << pos)) >> pos)

/*
 ?  SET_BIT
 *  -> Macro for setting bit in given position to 1 by generating a mask
 *     in the proper bit location and ORing (|) x with the mask.
 */
#define SET_BIT(x, pos) (x |= (1U << pos))

/*
 ?   TOOGLE_BIT
 *  -> Macro for toggling bit in given position to 1 by generating a mask
 *     in the proper bit location and ex-ORing x with the mask.
 */
#define TOOGLE_BIT(x, pos) (x ^= (1U << pos))

/*
 ?  CLEAR_BIT
 *  -> Macro for clearing bit in given position to 0 by generating a mask
 *     in the proper bit location and Anding x with the mask.
 */
#define CLEAR_BIT(x, pos) (x &= (~(1U << pos)))

int Ampel(void);
int Input(void);
int InterruptTask(void);
int Pipser(void);

int delay = 250;
int frequency = 440;

int main(void)
{
    Pipser();
    // InterruptTask();
    // Input();
    // Ampel();
}

// Aufgabe: Programmiere eine Ampel
int Ampel(void)
{
    DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB4);

    /* Clear Bits */
    CLEAR_BIT(PORTB, PORTB2);
    CLEAR_BIT(PORTB, PORTB3);
    CLEAR_BIT(PORTB, PORTB4);

    /* RESTET OUTPUT TO 0 */
    // PORTB &= ~(1 << PORTB2);
    // PORTB &= ~(1 << PORTB3);
    // PORTB &= ~(1 << PORTB4);

    while (1)
    {
        // red
        PORTB |= (1 << PORTB2);
        _delay_ms(delay * 4);
        PORTB = 0;

        // yellow red
        PORTB |= (1 << PORTB2);
        PORTB |= (1 << PORTB3);
        _delay_ms(delay);
        PORTB = 0;

        // green
        PORTB |= (1 << PORTB4);
        _delay_ms(delay * 4);
        PORTB = 0;

        // yellow
        PORTB |= (1 << PORTB3);
        _delay_ms(delay);
        PORTB = 0;
    }
}

// Aufgabe: Toggle Onboard Lamp with button
int Input(void)
{
    DDRD = (1 << DDB2);
    DDRB |= (1 << DDB3);

    PORTD &= ~(1 << DDD2);
    PORTB &= ~(1 << PORTB3);

    // Pull up

    while (1)
    {
        if (!(PIND & (1 << PIND2)))
        {
            // PORTB |= (1 << PORTB5);
            PORTB = (1 << PORTB3);
        }
        if (!(PIND & (1 << PIND3)))
        {
            // PORTB &= ~(1 << PORTB5);
            PORTB = 0;
        }
    }
}

// Aufgabe: Schalte mit einem Taster alle Lampen an und mit dem anderen Taster aus
int InterruptTask(void)
{

    /**
    // TODO:
    - Pull-UP ?
        -> Reaction auf Erdung (Kein Strom)
    */

    DDRB = 0xFF;
    PORTB = 0xFF;

    CLEAR_BIT(DDRD, DDD2);
    // DDRD &= ~(1 << DDD2); // Clear Pin PD2
    // PD2 (PCINT0 pin) is now an input

    CLEAR_BIT(DDRD, DDD3);
    // DDRD &= ~(1 << DDD3); // Clear Pin PD3
    // PD3 (PCINT1 pin) is now an input

    SET_BIT(PORTD, PORTD2);
    // PORTD |= (1 << PORTD2); // Turn on the Pull-up
    // PD2 is now  an input with pull-up enabled

    SET_BIT(PORTD, PORTD3);
    // PORTD |= (1 << PORTD3); // Turn on the Pull-up
    // PD3 is now an input with pull-up enabled

    /*
     ! EICRA: THIS IS IMPORTANT
     */
    EICRA |= (1 << ISC00); // Set INT0 to trigger on ANY logic change
    EICRA |= (1 << ISC10); // Set INT1 to trigger on ANY logic change

    EIMSK |= (1 << INT0); // Turn on INT0
    EIMSK |= (1 << INT1); // Turn on INT1

    sei(); // Turn on interrupts

    while (1)
    {
        // While true,
    }
}

ISR(INT0_vect) // Interrrupt INT0
{
    PORTB = 0x00;
}

ISR(INT1_vect)
{
    PORTB = 0xFF;
}

// Aufgabe: Pipser mit 440 hz pipsen lasssen
int Pipser(void)
{
    TCCR0A |= (1 << WGM01); // Set the Timer Mode to CTC

    OCR0A = 70; // Set the value that you want to count to

    TIMSK0 |= (1 << OCIE0A); // Set the ISR COMPA vect

    sei(); // enable interrupts

    TCCR0B |= (1 << CS02);

    while (1)
    {
        // while true
    }
}

ISR(TIMER0_COMPA_vect)
{
    // TOOGLE_BIT(PORTB, PORTB5);
    PORTB ^= (1U << PORTB5);
}
