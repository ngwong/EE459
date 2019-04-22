/***************************************************
 * Backup plan to I2C code
 * Uses polling to achieve changes to led and buzzer
 *
 * PIN2:PD0 LED1 RED
 * PIN3:PD1 LED1 GREEN
 * PIN4:PD2 LED2 RED
 * PIN5:PD3 LED2 GREEN
 * PIN6:PD4 LED3 RED
 * PIN10:PB7
 * PIN11:PD5 LED3 GREEN
 * PIN12:PD6
 * PIN13:PD7
 * PIN14:PB0 PROXIMITY 1 
 * PIN15:PB1 PROXIMITY 2
 * PIN16:PB2 PROXIMITY 3 
 * PIN24:PC1 INFECTED/~NOT INFECTED
 * PIN25:PC2
 * PIN26:PC3
 *
 *****************************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h> //this will be if we get to interrupt system


int main(void){
	DDRD |= (1 << DDD0 | 1 << DDD1 | 1 << DDD2 | 1 << DDD3 | 1 << DDD4 | 1 << DDD5); // SET REGISTER PINS OUT FOR LED
	//DDRB |= (1 << DDB0); // SET INPUT PINS WILL DEFAULT TO LOW
	
	//int state;
	//int proxy;
	char infected;
	char prox1, prox2, prox3;
	while(1){
		infected = (PINC & (1 << PC1));
		prox1 = (PINB & (1 << PB0));
		prox2 = (PINB & (1 << PB1));
		prox3 = (PINB & (1 << PB2));
		if(prox1){//checks for prox3 signal and turns on appropriate led
			if(infected){
				PORTD |= 1 << PD0;
				PORTD &= ~(1 << PD1);
			}
			else{
			       	PORTD |= 1 << PD1;
				PORTD &= ~(1 << PD0);	
			}
		}
		else {//if no prox3 turns off led
			PORTD &= ~(1 << PD1);
			PORTD &= ~(1 << PD0);	
		}


		if(prox2){//checks for prox3 signal and turns on appropriate led
			if(infected){
			       	PORTD |= 1 << PD2;
				PORTD &= ~(1 << PD3);
			}
			else{
				PORTD |= 1 << PD3;
				PORTD &= ~(1 << PD2);
			}
		}
		else{//if no prox3 turns off led
			PORTD &= ~(1 << PD3);
			PORTD &= ~(1 << PD2);

		}


		if(prox3){ //checks for prox3 signal and turns on appropriate led
			if(infected){
				PORTD |= 1 << PD4;
				PORTD &= ~(1 << PD5);
			}
			else{ 
				PORTD |= 1 << PD5;
				PORTD &= ~(1 << PD4);
			}

		}
		else{ //if no prox3 turns off led
			PORTD &= ~(1 << PD5);
			PORTD &= ~(1 << PD4);
		}
	}	
}
