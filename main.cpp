// example to test the TFT Display
// Thanks to the GraphicsDisplay and TextDisplay classes
// test2.bmp has to be on the mbed file system
//Lib for 320*240 Pixel Color LCD with ILI9341 controller. Based on MI0283QT-9 datasheet


#include "mbed.h"
#include <vector>

/*
2022 Nov
This project is the KMITL university microprocessor class

This project is simple version of the arcade game called "cyclone"
https://www.betsonparts.com/media/hbi/service-manuals/355/Cyclone%20Service%20Manual%2012-1-09.pdf


*/

class LEDLine{
	private:
		std::vector<DigitalOut> LED_VECTOR;
		std::vector<int> SCORE_VECTOR;
		int current_led_index;

	public:
		LEDLine(std::vector<PinName> pin_vector, std::vector<int> score_vector){
			// Initailize LED_VECTOR
			for(int i = 0; i < pin_vector.size(); i++){
				LED_VECTOR.push_back(DigitalOut(pin_vector.at(i)));
			}

			// Deep copy of score vector
			SCORE_VECTOR = score_vector;

			current_led_index = 0;
		}

		void hop(){
			LED_VECTOR.at(current_led_index).write(0);
			current_led_index++;
			current_led_index = current_led_index % LED_VECTOR.size();
			LED_VECTOR.at(current_led_index).write(1);
		}

		void toggle_current_led(){
			if(LED_VECTOR.at(current_led_index).read() == 0){
				LED_VECTOR.at(current_led_index).write(1);
			} else{
				LED_VECTOR.at(current_led_index).write(0);
			}
		}

};





int main(){


	std::vector<PinName> led_line_pin_vector;
	led_line_pin_vector.push_back(P0_9);
	led_line_pin_vector.push_back(P0_8);
	led_line_pin_vector.push_back(P0_7);
	led_line_pin_vector.push_back(P0_6);
	led_line_pin_vector.push_back(P0_0);
	led_line_pin_vector.push_back(P0_1);
	led_line_pin_vector.push_back(P0_17);

	std::vector<int> led_line_score_vector;
	LEDLine led_line = LEDLine(led_line_pin_vector, led_line_score_vector);
	// LightRow lightRow = LightRow(led_line, P0_15);
	// lightRow.click_test(); //dd dd



	// The while loop keep the program running
	DigitalOut board_led2(P0_22);
	while(1){
		board_led2 = !board_led2;
		led_line.hop();
		wait(1);

	}


}
