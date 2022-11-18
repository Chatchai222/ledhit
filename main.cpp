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
DigitalOut BOARD_LED2(P0_22);

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
int hopper_hop_amount = 1;

Ticker blinker_ticker;
const int BLINKER_BLINK_AMOUNT = 6;

const int SCORE_MAPPER_ARRAY_SIZE = 7;
const int SCORE_MAPPER_SCORE_ARRAY[SCORE_MAPPER_ARRAY_SIZE] = {
	-3, 4, -3, -1, 2, 2, -1
};

Timeout clicker_timeout;
const float CLICKER_COOLDOWN = 2; // in seconds
DigitalOut clicker_led(P0_15, 1);
InterruptIn clicker_interrupt(P0_16); // this will be connected to button

// servo_line will use HiTec HS-311 servo motor
// HiTEC HS-311 servo motor
// https://hitecrcd.com/products/servos/analog/sport-2/hs-311/product
const float HS_311_PWM_PERIOD_SECOND = 0.020;
const int HS_311_MAX_DEGREE = 180;
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

const int SERVO_DISPLAY_GAME_STAGE_SERVO_LINE_INDEX = 0;
const int SERVO_DISPLAY_SCORE_TENS_DIGIT_SERVO_LINE_INDEX = 1;
const int SERVO_DISPLAY_SCORE_ONES_DIGIT_SERVO_LINE_INDEX = 2;
const int SERVO_DISPLAY_ROUND_TIME_SERVO_LINE_INDEX = 3;

// stud for display; since display is more of an interface

const int GAME_INITIAL_ROUND_TIME = 90;
const int GAME_INITIAL_SCORE = 0;
const int GAME_STAGE_IDLE = 0;
const int GAME_STAGE_PLAYING = 1;
int game_score = GAME_INITIAL_SCORE;
int game_round_time = GAME_INITIAL_ROUND_TIME;
int game_stage = GAME_STAGE_IDLE;
Ticker game_round_time_ticker;
const float GAME_ROUND_TIME_TICKER_DELAY = 1;
Timeout game_game_over_timeout;
Ticker game_observer_updater_ticker;
const float GAME_OBSERVER_UPDATER_TICKER_DELAY = 0.2;

Timeout round_starter_timeout;
InterruptIn round_starter_interrupt(P0_2); // this will be connected to button
DigitalOut round_starter_led(P0_3, 1);



// Defining functions
void led_line_initialize();
void led_line_hop();
void led_line_hop_by(int);
void led_line_current_led_toggle();
int led_line_current_led_index_get();

void hopper_ticker_routine();
void hopper_ticker_enable();
void hopper_ticker_disable();
void hopper_delay_set(float);
void hopper_hop_amount_set(int);

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
int HS_311_pwm_pulsewidth_microsecond_get_from_position_degree(int);
int HS_311_pwm_pulsewidth_microsecond_get_from_position_percent(float);
int HS_311_max_degree_get();

void servo_line_initialize();
void servo_line_position_degree_set(int index, int degree);
void servo_line_position_percent_set(int index, float percent);

void servo_display_update();
void _servo_display_update_game_stage();
void _servo_display_update_score();
void _servo_display_update_score_tens_digit(int);
void _servo_display_update_score_ones_digit(int);
void _servo_display_update_round_time();

void game_start();
void game_observer_update();
void game_display_update();
void game_difficulty_update();
void game_add_score_from_score_mapper();
int game_score_get();
int game_round_time_get();
int game_stage_get();
int game_initial_round_time_get();
void _game_round_time_ticker_enable();
void _game_round_time_ticker_disable();
void _game_round_time_ticker_routine();
void _game_round_time_decrement();
void _game_game_start_routine();
void _game_game_over_timeout_routine();
void _game_observer_updater_ticker_enable();
void _game_observer_updater_ticker_disable();
void _game_observer_updater_ticker_routine();

void round_starter_interrupt_enable();
void round_starter_interrupt_disable();
void round_starter_interrupt_routine();
void _round_starter_interrupt_routine_begin();
void _round_starter_interrupt_routine_end();

void difficulty_setter_update();

void temp_test();




// Function for the led_line
void led_line_initialize(){
	for(int i = 0; i < LED_LINE_ARRAY_SIZE; i++){
		led_line_digital_out_vector.push_back(DigitalOut(led_line_pin_name_array[i]));
	}
}

void led_line_hop(){
	led_line_hop_by(1);
}

void led_line_hop_by(int in_hop_by){
	int amount_to_hop = in_hop_by;
	if(amount_to_hop < 0){
		amount_to_hop = 0;
	}
	led_line_digital_out_vector.at(led_line_current_index).write(0);
	led_line_current_index += amount_to_hop;
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
	led_line_hop_by(hopper_hop_amount);
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

void hopper_hop_amount_set(int in_amount){
	hopper_hop_amount = in_amount;
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
	clicker_interrupt.rise(clicker_interrupt_routine);
}

void clicker_interrupt_disable(){
	clicker_interrupt.rise(0); // when interrupt don't call anything
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

	temp_test();
	game_add_score_from_score_mapper();

}

void _clicker_interrupt_routine_end(){
	blinker_ticker_disable();
	hopper_ticker_enable();
	clicker_interrupt_enable();
	clicker_led.write(1);
}


// HS_311 servo motor function
float HS_311_pwm_period_second_get(){
	return HS_311_PWM_PERIOD_SECOND;
}

int HS_311_pwm_pulsewidth_microsecond_get_from_position_degree(int degree){
	return HS_311_PWM_PULSEWIDTH_MICROSECOND_AT_ZERO_DEGREE + (degree * HS_311_PWM_PULSEWIDTH_MICROSECOND_PER_DEGREE);
}

int HS_311_pwm_pulsewidth_microsecond_get_from_position_percent(float in_position_percentage){
	int output;
	float position_percentage = in_position_percentage;
	if(position_percentage < 0){
		position_percentage = 0;
	}
	if(position_percentage > 1){
		position_percentage = 1;
	}
	output = (int)( HS_311_PWM_PULSEWIDTH_MICROSECOND_AT_ZERO_DEGREE + (position_percentage * HS_311_MAX_DEGREE * HS_311_PWM_PULSEWIDTH_MICROSECOND_PER_DEGREE));
	return output;
}

int HS_311_max_degree_get(){
	return HS_311_MAX_DEGREE;
}


// servo_line (which use HS_311 servo motor) functions
void servo_line_initialize(){
	for(int i = 0; i < SERVO_LINE_ARRAY_SIZE; i++){
		servo_line_pwm_out_vector.push_back(PwmOut(SERVO_LINE_PIN_NAME_ARRAY[i]));
		servo_line_pwm_out_vector.at(i).period(HS_311_pwm_period_second_get());
	}
}

void servo_line_position_degree_set(int index, int degree){
	int pulsewidth_us_for_pwm_out = HS_311_pwm_pulsewidth_microsecond_get_from_position_degree(degree);
	servo_line_pwm_out_vector.at(index).pulsewidth_us(pulsewidth_us_for_pwm_out);
}

void servo_line_position_percent_set(int index, float percent){
	int pulsewidth_us_for_pwm_out = HS_311_pwm_pulsewidth_microsecond_get_from_position_percent(percent);
	servo_line_pwm_out_vector.at(index).pulsewidth_us(pulsewidth_us_for_pwm_out);
}


// Function servo_display (which is a concrete implementation of display)
void servo_display_update(){
	_servo_display_update_game_stage();
	_servo_display_update_score();
	_servo_display_update_round_time();
}

void _servo_display_update_game_stage(){
	int game_stage = game_stage_get();
	float position_percent = game_stage * 0.5;
	servo_line_position_percent_set(SERVO_DISPLAY_GAME_STAGE_SERVO_LINE_INDEX, position_percent);
}

void _servo_display_update_score(){
	int game_score = game_score_get();
	int tens_digit_score = 0;
	int ones_digit_score = 0;
	if(game_score < 0){
		game_score = 0;
	}
	if(game_score > 99){
		game_score = 99;
	}
	tens_digit_score = floor(game_score / 10);
	ones_digit_score = game_score % 10;
	_servo_display_update_score_tens_digit(tens_digit_score);
	_servo_display_update_score_ones_digit(ones_digit_score);
}

void _servo_display_update_score_tens_digit(int in_score){
	float position_percent = in_score / 10.0;
	servo_line_position_percent_set(SERVO_DISPLAY_SCORE_TENS_DIGIT_SERVO_LINE_INDEX, position_percent);
}

void _servo_display_update_score_ones_digit(int in_score){
	float position_percent = in_score / 10.0;
	servo_line_position_percent_set(SERVO_DISPLAY_SCORE_ONES_DIGIT_SERVO_LINE_INDEX, position_percent);
}

void _servo_display_update_round_time(){
	int round_time = game_round_time_get();
	int initial_round_time = game_initial_round_time_get();
	float position_percent = round_time / (initial_round_time * 1.0); // the "* 1.0" prevent integer arithmetic 5/2 = 2, 5/(2 * 1.0) = 2.5;
	servo_line_position_percent_set(SERVO_DISPLAY_ROUND_TIME_SERVO_LINE_INDEX, position_percent);
}


// Function for game (state)
void game_start(){
	if(game_stage == GAME_STAGE_PLAYING){
		return;
	} else {
		_game_game_start_routine();
		game_game_over_timeout.attach(_game_game_over_timeout_routine, GAME_INITIAL_ROUND_TIME);
	}
}

void game_observer_update(){
	game_difficulty_update();
	game_display_update();
}

void game_display_update(){
	servo_display_update();
}

void game_difficulty_update(){
	difficulty_setter_update();
}

void game_add_score_from_score_mapper(){
	int score_to_add = score_mapper_score_get();
	game_score += score_to_add;
	if(game_score < 0){
		game_score = 0;
	}
}

int game_score_get(){
	return game_score;
}

int game_round_time_get(){
	return game_round_time;
}

int game_stage_get(){
	return game_stage;
}

int game_initial_round_time_get(){
	return GAME_INITIAL_ROUND_TIME;
}

void _game_round_time_ticker_enable(){
	game_round_time_ticker.attach(_game_round_time_ticker_routine, GAME_ROUND_TIME_TICKER_DELAY);
}

void _game_round_time_ticker_disable(){
	game_round_time_ticker.detach();
}

void _game_round_time_ticker_routine(){
	_game_round_time_decrement();
}

void _game_round_time_decrement(){
	game_round_time--;
	if(game_round_time < 0){
		game_round_time = 0;
	}
}

void _game_game_start_routine(){
	game_score = GAME_INITIAL_SCORE;
	game_round_time = GAME_INITIAL_ROUND_TIME;
	game_observer_update();

	game_stage = GAME_STAGE_PLAYING;
	_game_round_time_ticker_enable();
	_game_observer_updater_ticker_enable();
}

void _game_game_over_timeout_routine(){
	game_stage = GAME_STAGE_IDLE;
	_game_round_time_ticker_disable();
	_game_observer_updater_ticker_disable();

	game_observer_update(); // update observer at the end of round
}

void _game_observer_updater_ticker_enable(){
	game_observer_updater_ticker.attach(_game_observer_updater_ticker_routine, GAME_OBSERVER_UPDATER_TICKER_DELAY);
}

void _game_observer_updater_ticker_disable(){
	game_observer_updater_ticker.detach();
}

void _game_observer_updater_ticker_routine(){
	game_observer_update();
}


// Function for the round_starter
void round_starter_interrupt_enable(){
	round_starter_interrupt.rise(round_starter_interrupt_routine);
}

void round_starter_interrupt_disable(){
	round_starter_interrupt.rise(0); // setting to 0 means don't call anything "none"
}

void round_starter_interrupt_routine(){
	_round_starter_interrupt_routine_begin();
	round_starter_timeout.attach(_round_starter_interrupt_routine_end, game_initial_round_time_get());
}

void _round_starter_interrupt_routine_begin(){
	round_starter_interrupt_disable();
	round_starter_led.write(0);

	game_start();
}

void _round_starter_interrupt_routine_end(){
	round_starter_interrupt_enable();
	round_starter_led.write(1);
}


// Function for difficulty_setter
void difficulty_setter_update(){
	int game_score = game_score_get();
	if(game_score < 10){ // Level 0
		hopper_hop_amount_set(1);
		hopper_delay_set(0.20);
	} else if (game_score < 20){
		hopper_hop_amount_set(1);
		hopper_delay_set(0.15);
	} else if (game_score < 30){
		hopper_hop_amount_set(2);
		hopper_delay_set(0.30);
	} else if (game_score < 40){
		hopper_hop_amount_set(1);
		hopper_delay_set(0.10);
	} else if (game_score < 50){
		hopper_hop_amount_set(1);
		hopper_delay_set(0.08);
	} else if (game_score < 60){
		hopper_hop_amount_set(2);
		hopper_delay_set(0.20);
	} else if (game_score < 70){
		hopper_hop_amount_set(1);
		hopper_delay_set(0.05);
	}
	
}


// This function can be deleted
// This was for testing and check purpose
void temp_test(){

}

int main(){
	led_line_initialize();
	hopper_ticker_enable();
	clicker_interrupt_enable();

	servo_line_initialize();

	round_starter_interrupt_enable();
	

	temp_test();

	// The while loop keep the program running forever
	while(1){
		BOARD_LED2 = !BOARD_LED2;
		wait(3);
	}
}