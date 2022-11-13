/*
2022 Nov
This project is the KMITL university microprocessor class

This project is simple version of the arcade game called "cyclone"
https://www.betsonparts.com/media/hbi/service-manuals/355/Cyclone%20Service%20Manual%2012-1-09.pdf

*/

#include "mbed.h"
#include <vector>

// Defining macros and definitions
DigitalOut ALWAYS_HIGH(P2_13);

const int LED_LINE_ARRAY_SIZE = 7;
std::vector<DigitalOut> led_line_digital_out_vector;
PinName led_line_pin_name_array[LED_LINE_ARRAY_SIZE] = {
	P0_9,
	P0_8,
	P0_7,
	P0_6,
	P0_0,
	P0_1,
	P0_17
};
int led_line_current_index = 0;

Ticker hopper_ticker;
float hopper_delay = 0.2;

Ticker blinker_ticker;
const int BLINKER_BLINK_AMOUNT = 6;

Timeout clicker_timeout;
const float CLICKER_COOLDOWN = 2; // in seconds
DigitalOut clicker_led(P0_15, 1);
InterruptIn clicker_interrupt(P0_16); // this will be connected to button


// Defining functions
void led_line_initialize();
void led_line_hop();
void led_line_toggle_current_led();

void hopper_ticker_routine();
void hopper_ticker_enable();
void hopper_ticker_disable();
void hopper_delay_set();

void blinker_ticker_routine();
void blinker_ticker_enable();
void blinker_ticker_disable();
float _blinker_blink_delay_get();

void clicker_interrupt_enable();
void clicker_interrupt_disable();
void clicker_interrupt_routine();
float clicker_cooldown_get();
void _clicker_interrupt_routine_begin();
void _clicker_interrupt_routine_end();



// Function for the led_line
void led_line_initialize(){
	for(int i = 0; i < LED_LINE_ARRAY_SIZE; i++){
		led_line_digital_out_vector.push_back(DigitalOut(led_line_pin_name_array[i]));
	}
}

void led_line_hop(){
	led_line_digital_out_vector.at(led_line_current_index).write(0);
	led_line_current_index++;
	led_line_current_index %= LED_LINE_ARRAY_SIZE;
	led_line_digital_out_vector.at(led_line_current_index).write(1);
}

void led_line_toggle_current_led(){
	led_line_digital_out_vector.at(led_line_current_index) = !led_line_digital_out_vector.at(led_line_current_index);
}

// Functions for hopper
void hopper_ticker_routine(){
	led_line_hop();
}

void hopper_ticker_enable(){
	hopper_ticker.attach(hopper_ticker_routine, hopper_delay);
}

void hopper_ticker_disable(){
	hopper_ticker.detach();
}

void hopper_delay_set(float second){
	hopper_delay = second;
}


// Function for blinker 
void blinker_ticker_routine(){
	led_line_toggle_current_led();
}

void blinker_ticker_enable(){
	blinker_ticker.attach(blinker_ticker_routine, _blinker_blink_delay_get());
}

void blinker_ticker_disable(){
	blinker_ticker.detach();
}

float _blinker_blink_delay_get(){
	float output;
	float duration_to_blink = clicker_cooldown_get();
	output = duration_to_blink / ((BLINKER_BLINK_AMOUNT * 2) + 1); // The + 1 is for final toggle off
	return output;
}


// Function for clicker
void clicker_interrupt_enable(){
	clicker_interrupt.enable_irq();
	clicker_interrupt.rise(clicker_interrupt_routine);
}

void clicker_interrupt_disable(){
	clicker_interrupt.disable_irq();
}

void clicker_interrupt_routine(){
	_clicker_interrupt_routine_begin();
	clicker_timeout.attach(_clicker_interrupt_routine_end, CLICKER_COOLDOWN);
}

float clicker_cooldown_get(){
	return CLICKER_COOLDOWN;
}

void _clicker_interrupt_routine_begin(){
	clicker_interrupt_disable();
	hopper_ticker_disable();
	blinker_ticker_enable();
	clicker_led.write(0);
}

void _clicker_interrupt_routine_end(){
	blinker_ticker_disable();
	hopper_ticker_enable();
	clicker_interrupt_enable();
	clicker_led.write(1);
}


// This function can be deleted
// This was for testing and check purposes
void temp_test(){
	for(int i = 0; i < 5; i++){
		hopper_ticker_disable();
		blinker_ticker_enable();
		wait(2);
		blinker_ticker_disable();
		hopper_ticker_enable();
		wait(2);
	}
}

int main(){
	ALWAYS_HIGH.write(1);
	led_line_initialize();
	hopper_ticker_enable();
	clicker_interrupt_enable();

	temp_test();

	// The while loop keep the program running
	DigitalOut board_led2(P0_22);
	while(1){
		board_led2 = !board_led2;
		wait(1);
	}
}
