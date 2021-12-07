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

int main(void)
{
    // Ampel();
    // Input();
    // InterruptTask();
    Pipser();
}

// Aufgabe: Programmiere eine Ampel
int Ampel(void)
{

    int delay = 250;

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

// Aufgabe: Pipser mit 440 hz pipsen lasssen
int Pipser(void)
{
    DDRB = 0xFF;  // alle B's sind outputs
    PORTB = 0x00; // set all outputs to 0

    CLEAR_BIT(DDRD, DDD2); // activate input
    CLEAR_BIT(DDRD, DDD3);

    SET_BIT(PORTD, PORTD2); // pull up
    SET_BIT(PORTD, PORTD3);

    // ---------FOLLOWING:-INTERRUPTS-----------------

    SET_BIT(EICRA, ISC00); // interrupt on toggle
    SET_BIT(EICRA, ISC10); // Sagt nur wie sich der Interrupt verhalten soll

    SET_BIT(EIMSK, INT1); // enables specific interrupt on Pin D2
    SET_BIT(EIMSK, INT0); // " on Pin D3

    sei(); // enables all interrupts

    // TIMSK0 = 0xFF;
    // setBit(TIMSK0,OCIE0A) // enables interrupts von timer | gegenstück zu EIMSK
    TIMSK0 |= (1 << OCIE0A);
    SET_BIT(TCCR0A, WGM01); // sets mode to CTC
    OCR0A = 70;             // obere Grenze für coutner

    SET_BIT(TCCR0B, CS02); // set prescaler to clock/256 and activate

    while (1)
    {
        // while true,
    }
}

ISR(TIMER0_COMPA_vect)
{
    // TOOGLE_BIT(PORTB, PORTB5);
    PORTB ^= (1U << PORTB5);
}

ISR(INT0_vect) // Interrrupt INT0
{
    PORTB = 0x00;
}

ISR(INT1_vect)
{
    PORTB = 0xFF;
}
