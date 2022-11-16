/*
2022 Nov
This project is the KMITL university microprocessor class
This project is simple version of the arcade game called "cyclone"
https://www.betsonparts.com/media/hbi/service-manuals/355/Cyclone%20Service%20Manual%2012-1-09.pdf
*/

#include "mbed.h"
#include <vector>

// Defining macros and definitions
DigitalOut ALWAYS_HIGH(P2_13, 1);

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

const int SCORE_MAPPER_ARRAY_SIZE = 7;
const int SCORE_MAPPER_SCORE_ARRAY[SCORE_MAPPER_ARRAY_SIZE] = {
	0, 1, 2, 3, 4, 5, 6
};

Timeout clicker_timeout;
const float CLICKER_COOLDOWN = 2; // in seconds
DigitalOut clicker_led(P0_15, 1);
InterruptIn clicker_interrupt(P0_16); // this will be connected to button

// servo_line will use HiTec HS-311 servo motor
// HiTEC HS-311 servo motor
// https://hitecrcd.com/products/servos/analog/sport-2/hs-311/product
const float HS_311_PWM_PERIOD_SECOND = 0.020;
const float HS_311_PWM_PULSEWIDTH_MICROSECOND_AT_ZERO_DEGREE = 600;
const float HS_311_PWM_PULSEWIDTH_MICROSECOND_PER_DEGREE = 10;

const int SERVO_LINE_ARRAY_SIZE = 4;
const PinName SERVO_LINE_PIN_NAME_ARRAY[SERVO_LINE_ARRAY_SIZE] = {
	P2_0,
	P2_1,
	P2_2,
	P2_3
};
std::vector<PwmOut> servo_line_pwm_out_vector;

// stud for display; since display is more of an interface





// Defining functions
void led_line_initialize();
void led_line_hop();
void led_line_current_led_toggle();
int led_line_current_led_index_get();

void hopper_ticker_routine();
void hopper_ticker_enable();
void hopper_ticker_disable();
void hopper_delay_set();

void blinker_ticker_routine();
void blinker_ticker_enable();
void blinker_ticker_disable();
float _blinker_blink_delay_get();

int score_mapper_score_get();

void clicker_interrupt_enable();
void clicker_interrupt_disable();
void clicker_interrupt_routine();
float clicker_cooldown_get();
void _clicker_interrupt_routine_begin();
void _clicker_interrupt_routine_end();

float HS_311_pwm_period_second_get();
int HS_311_pwm_pulsewidth_us_get_from_position_degree(int);

void servo_line_initialize();
void servo_line_position_degree_set(int index, int degree);

void display_update();

void temp_test();



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

void led_line_current_led_toggle(){
	led_line_digital_out_vector.at(led_line_current_index) = !led_line_digital_out_vector.at(led_line_current_index);
}

int led_line_current_led_index_get(){
	return led_line_current_index;
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
	led_line_current_led_toggle();
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


// Function for score mapper
int score_mapper_score_get(){
	int index_to_map_to_score = led_line_current_led_index_get();
	return SCORE_MAPPER_SCORE_ARRAY[index_to_map_to_score];
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
	temp_test();
	clicker_led.write(0);
}

void _clicker_interrupt_routine_end(){
	blinker_ticker_disable();
	hopper_ticker_enable();
	clicker_interrupt_enable();
	clicker_led.write(1);
}


// HS_311 servo motor functions
float HS_311_pwm_period_second_get(){
	return HS_311_PWM_PERIOD_SECOND;
}

int HS_311_pwm_pulsewidth_us_get_from_position_degree(int degree){
	return HS_311_PWM_PULSEWIDTH_MICROSECOND_AT_ZERO_DEGREE + (degree * HS_311_PWM_PULSEWIDTH_MICROSECOND_PER_DEGREE);
}


// servo_line (which use HS_311 servo motor) functions
void servo_line_initialize(){
	for(int i = 0; i < SERVO_LINE_ARRAY_SIZE; i++){
		servo_line_pwm_out_vector.push_back(PwmOut(SERVO_LINE_PIN_NAME_ARRAY[i]));
		servo_line_pwm_out_vector.at(i).period(HS_311_pwm_period_second_get());
	}
}

void servo_line_position_degree_set(int index, int degree){
	int pulsewidth_us_for_pwm_out = HS_311_pwm_pulsewidth_us_get_from_position_degree(degree);
	servo_line_pwm_out_vector.at(index).pulsewidth_us(pulsewidth_us_for_pwm_out);
}


// Display interface function
void display_update(){
	;//stud
}



// This function can be deleted
// This was for testing and check purposes
void temp_test(){
	int score = score_mapper_score_get();
	servo_line_position_degree_set(0, score * 20);
	servo_line_position_degree_set(1, score * 20);
	servo_line_position_degree_set(2, score * 20);
	servo_line_position_degree_set(3, score * 20);
}

int main(){
	led_line_initialize();
	hopper_ticker_enable();
	clicker_interrupt_enable();

	servo_line_initialize();

	

	temp_test();

	// The while loop keep the program running
	DigitalOut board_led2(P0_22);
	while(1){
		board_led2 = !board_led2;
		wait(1);
	}
}