/*
 * project.c
 *
 *  Created on: Sep 20, 2021
 *      Author: Mahmoud
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define SIZE 6
unsigned char num[SIZE] ={0,0,0,0,0,0};
unsigned char LOOP;
ISR(TIMER1_COMPA_vect)
{
	num[0]++;
	if (num[0]==10)
	{
		num[0]=0;
		num[1]++;
	}
	if (num[1]==7)
	{
		num[1]=0;
		num[2]++;
	}
	if (num[2]==10)
	{
		num[2]=0;
		num[3]++;
	}
	if (num[3]==7)
	{
		num[3]=0;
		num[4]++;
	}
	if (num[4]==10)
	{
		num[4]=0;
		num[5]++;
	}
	if (num[5]==10)
		for(LOOP=0;LOOP<SIZE;LOOP++)
		{
			num[LOOP]=0;
		}

}
void Timer1_init_CTC_Mode(void)
{
	TCNT1 = 0; //intial value =0
	TCCR1A|=1<<FOC1A; //non pwm mode
	TCCR1B |= (1<<WGM12) | (1<<CS10)|(1<<CS12); //CTC MODE  (clkI/O/1024 (From prescaler))
	OCR1A =1000; //top value (1 SECOND)
	TIMSK|=(1<<OCIE1A); //Output Compare A Match Interrupt Enable

}
ISR(INT0_vect)
{
	for(LOOP=0;LOOP<SIZE;LOOP++)
			{
				num[LOOP]=0;
			}

}
void INT_0(void)
{
	GICR|=(1<<6);
	MCUCR|=(1<<ISC01);
	PORTD|=(1<<PD2);		//INT0 internal-Pull-Up Enable
}
ISR(INT1_vect)
{
	TCCR1B&=~(0x07);
}
void INT_1(void)
{
	GICR|=(1<<7);
	MCUCR|=(1<<ISC11)|(1<<ISC10);
}

ISR(INT2_vect)
{
	TCCR1B |= (1<<CS10)|(1<<CS12);

}
void INT_2(void)
{
	GICR|=(1<<5);
	MCUCSR&=(~1<<ISC2);
	PORTB|=(1<<PB2); //INT2 internal-Pull-Up Enable
}
void main(void)
{
	SREG|=(1<<7); //enable global interrupt
	DDRC|=0x0F; //Connect 7447 decoder 4-pins to the first 4-pins in PORTC
	//Use first 6-pins in PORTA as the enable/disable pins for the six 7-segments
	DDRC|=0x3F;
	PORTA&=~(0x3F);
	INT_0();
	INT_1();
	INT_2();
	Timer1_init_CTC_Mode(); // Start the timer

	while (1)
	{
		for(LOOP=0;LOOP<SIZE;LOOP++)
		{
			PORTA=(1<<LOOP);
			PORTC = (PORTC & 0xF0) | (num[LOOP] & 0x0F);
			_delay_ms(4);
		}

	}
}


