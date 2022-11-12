
/*
2022 Nov
This project is the KMITL university microprocessor class

This project is simple version of the arcade game called "cyclone"
https://www.betsonparts.com/media/hbi/service-manuals/355/Cyclone%20Service%20Manual%2012-1-09.pdf

*/

#include "mbed.h"
#include <vector>


// DEFINING and objects

std::vector<DigitalOut> LEDLine_led_vector;
int LEDLine_current_index = 0;


void LEDLine_hop(){
	LEDLine_led_vector.at(LEDLine_current_index).write(0);
	LEDLine_current_index++;
	LEDLine_current_index %= LEDLine_led_vector.size();
	LEDLine_led_vector.at(LEDLine_current_index).write(1);
}


int main(){
	LEDLine_led_vector.push_back(DigitalOut(P0_9));
	LEDLine_led_vector.push_back(DigitalOut(P0_8));
	LEDLine_led_vector.push_back(DigitalOut(P0_7));
	LEDLine_led_vector.push_back(DigitalOut(P0_6));
	LEDLine_led_vector.push_back(DigitalOut(P0_0));
	LEDLine_led_vector.push_back(DigitalOut(P0_1));
	LEDLine_led_vector.push_back(DigitalOut(P0_17));

	// The while loop keep the program running
	DigitalOut board_led2(P0_22);
	while(1){
		board_led2 = !board_led2;
		LEDLine_hop();
		wait(0.25);

	}
}
