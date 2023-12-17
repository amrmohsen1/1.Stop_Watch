#include<avr\io.h>
#include<avr\interrupt.h>
#include<util\delay.h>

unsigned char sec1=0;
unsigned char sec2=0;
unsigned char min1=0;
unsigned char min2=0;
unsigned char hour1=0;
unsigned char hour2=0;



void StopWatch(void)
{
	sec1++;
	if(sec1 > 9)
	{
		sec1=0;
		sec2++;
		if(sec2==6)
		{
			sec2=0;
			min1++;
		}
	}

	if(min1 > 9)
	{
		min1=0;
		min2++;
		if(min2==6)
		{
			min2=0;
			hour1++;
		}
	}

	if(hour1 > 9)
	{
		hour1=0;
		hour2++;
		if(hour2 > 9)
		{
			sec1=0;
			sec2=0;
			min1=0;
			min2=0;
			hour1=0;
			hour2=0;
		}

	}
}

void Timer1_CTC_StopWatch_Init()
{
	//COM1A1 ,COM1A0 =0   (DISCONNET OC1A)

	TCCR1A=(1<<FOC1A);           //FORCE OUTPUT COMPARE MODE FOR NON PWM
	TCCR1B=(1<<WGM12)|(1<<CS10)|(1<<CS11)|(1<<CS10);     // SET CTC AND CLOCK

	OCR1A=15625;               //SET COMPARE VALUE
	TCNT1=0;                   //SET INITIAL COUNT

	TIMSK|=(1<<OCIE1A);          //ENABLE TIMER INTERRUPT MODULE
	SREG|=(1<<7);                //ENABLE I-BIT

}

void INT0_Init(void)
{
	DDRD&=~(1<<PD2);     //SET PD2 AS INPUT
	PORTD|=(1<<PD2);     //ENABLE PULL-UP INTERNAL RESISTOR
	MCUCR|=(1<<ISC01);   //TRIGGER FALLING EDGE
	MCUCR&=~(1<<ISC00);
	GICR|=(1<<INT0);     //ENABLE INT0 MODULE
	SREG|=(1<<7);        //ENABLE I-BIT

}

void INT1_Init(void)
{
	DDRD&=~(1<<PD3);                //SET PD3 AS INPUT
	MCUCR|=(1<<ISC11)|(1<<ISC10);   //TRIGGER THE RISING EDGE
	GICR|=(1<<INT1);                //ENABLE INT1 MODULE
	SREG|=(1<<7);                   //ENABLE I-BIT

}

void INT2_Init(void)
{
	DDRB&=~(1<<PB2);         //SET PB2 INPUT
	PORTB|=(1<<PB2);         //ENABLE INTERNAL PULL-UP RESISTOR
	MCUCSR&=~(1<<ISC2);      //TRIGGER FALLING EDGE
	GICR|=(1<<INT2);         //ENABLE INT2 MODULE
	SREG|=(1<<7);            //ENABLE I-BIT

}


ISR(TIMER1_COMPA_vect)
{
	StopWatch();
}

ISR(INT0_vect)
{
	TCNT1=0;        //RESET TIMER INITIAL COUNT

	sec1=0;
	sec2=0;
	min1=0;
	min2=0;
	hour1=0;
	hour2=0;
}

ISR(INT1_vect)
{
	SREG|=(1<<7);
	TCCR1B&=~(1<<CS12) & ~(1<<CS11) & ~(1<<CS10);      //TURN OFF CLOCK

}

ISR(INT2_vect)
{
	SREG|=(1<<7);
	TCCR1B|=  (1<<CS11) | (1<<CS10);              //ENABLE CLOCK PRESCALAR 64
}


void Display_first_7Seg()
{
	PORTA=(PORTA& 0XC0)|(0x01);          //SET Corresponding PIN TO ENABLE(DISPLAY) 7-SEG

	PORTC=(PORTC & 0XF0)|(sec1 & 0x0F);  //Display number corresponding to 7-SEG
}

void Display_second_7Seg()
{
	PORTA=(PORTA& 0XC0)|(0x02);
	PORTC=(PORTC & 0XF0)|(sec2 & 0x0F);

}

void Display_third_7Seg()
{
	PORTA=(PORTA& 0XC0)|(0x04);
	PORTC=(PORTC & 0XF0)|(min1 & 0x0F);

}

void Display_fourth_7Seg()
{
	PORTA=(PORTA& 0XC0)|(0x08);
	PORTC=(PORTC & 0XF0)|(min2 & 0x0F);

}

void Display_fifth_7Seg()
{
	PORTA=(PORTA& 0XC0)|(0x10);
	PORTC=(PORTC & 0XF0)|(hour1 & 0x0F);

}

void Display_sixth_7Seg()
{
	PORTA=(PORTA& 0XC0)|(0x20);
	PORTC=(PORTC & 0XF0)|(hour2 & 0x0F);

}



int main(void)
{
	DDRC|=0X0F;         //FIRST 4 PINS OUTPUT
	PORTC&=0XF0;        //SET THEM TO ZERO AT BEGINING

	DDRA|=0X3F;         //FIRST 6 PINS OUTPUT
	PORTA&=0XC0;        //SET THEM TO ZERO AT BEGINING
	Timer1_CTC_StopWatch_Init();
	INT0_Init();
	INT1_Init();
	INT2_Init();

	while(1)
	{
		Display_first_7Seg();
		_delay_ms(2);

		Display_second_7Seg();
		_delay_ms(2);

		Display_third_7Seg();
		_delay_ms(2);

		Display_fourth_7Seg();
		_delay_ms(2);

		Display_fifth_7Seg();
		_delay_ms(2);

		Display_sixth_7Seg();
		_delay_ms(2);

	}

}



