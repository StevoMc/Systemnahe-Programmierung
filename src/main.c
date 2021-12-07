#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>

int Ampel(void);
int Input(void);
int delay = 250;

int main(void)
{

    Input();
    // Ampel();
}

int Ampel(void)
{
    DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB4);

    /* RESTET OUTPUT TO 0 */
    PORTB &= ~(1 << PORTB2);
    PORTB &= ~(1 << PORTB3);
    PORTB &= ~(1 << PORTB4);
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
