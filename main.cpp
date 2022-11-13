/*
2022 Nov
This project is the KMITL university microprocessor class

This project is simple version of the arcade game called "cyclone"
https://www.betsonparts.com/media/hbi/service-manuals/355/Cyclone%20Service%20Manual%2012-1-09.pdf

*/

#include "mbed.h"
#include <vector>

// Defining macros and definitions

const int led_line_array_size = 7;
std::vector<DigitalOut> led_line_digital_out_vector;
PinName led_line_pin_name_array[led_line_array_size] = {
	P0_9,
	P0_8,
	P0_7,
	P0_6,
	P0_0,
	P0_1,
	P0_17
};
int led_line_current_index = 0;



void led_line_initialize();
void led_line_hop();

void led_line_initialize(){
	for(int i = 0; i < led_line_array_size; i++){
		led_line_digital_out_vector.push_back(DigitalOut(led_line_pin_name_array[i]));
	}
}

void led_line_hop(){
	led_line_digital_out_vector.at(led_line_current_index).write(0);
	led_line_current_index++;
	led_line_current_index %= led_line_array_size;
	led_line_digital_out_vector.at(led_line_current_index).write(1);
}

int main(){
	led_line_initialize();

	// The while loop keep the program running
	DigitalOut board_led2(P0_22);
	while(1){
		board_led2 = !board_led2;
		wait(1);
	}


}
