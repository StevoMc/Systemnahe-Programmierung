#include <avr/io.h>
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
int delay = 250;

int main(void)
{
    lrint(GET_BIT(0b01, 0));

    // Input();
    // Ampel();
}

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
            //PORTB |= (1 << PORTB5);
            PORTB = (1 << PORTB3);
        }
        if (!(PIND & (1 << PIND3)))
        {
            //PORTB &= ~(1 << PORTB5);
            PORTB = 0;
        }
    }
}
