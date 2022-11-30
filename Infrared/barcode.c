
/****************************************************
barcode.c
- HASHMAP
- Main code need to run adc and infrared to keep detecting, once white go into infrared function
- Detect White to START
	- But if too MUCH Black need to reset the START
- Add Voltages to form a Numerical value
- Keyboard Interrupt i redo the barcode function
- Add B/W String
****************************************************/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define NUMBER_OF_CHARS 44
#define MAX_STRING_SIZE 12

char * stringAnalysis(char colorString[]);
char compareString(char charSequence[]);

// -- 12-bit conversion, assume max value == ADC_VREF == 3.3 V
// [OLD] const float CONVERSION_FACTOR = 3.3f / (1 << 12);
const float CONVERSION_FACTOR = 3.3f / 16384;

// -- BIGGER MARGIN_MULTIPLIER == CLEARER LOWS & HIGHS
const int MARGIN_MULTIPLIER = 20;
const float THRESHOLD_VALUE = 7.5;
const float UPPER_BLACK_LIMIT = 1000;

const int BARCODE_SIZE = 31;
const int PATTERN_SIZE = 38;
const int PADDING_VALUE = 5;
const int NUMBER_OF_SEQUENCES = 3;

extern struct COMMS_DATA;

const char COLOR_ARRAY[NUMBER_OF_CHARS][MAX_STRING_SIZE] = {"BWBWWBBWBBWB","BBWBWWBWBWBB","BWBBWWBWBWBB","BBWBBWWBWBWB","BWBWWBBWBWBB","BBWBWWBBWBWB","BWBBWWBBWBWB","BWBWWBWBBWBB","BBWBWWBWBBWB","BWBBWWBWBBWB","BBWBWBWWBWBB","BWBBWBWWBWBB","BBWBBWBWWBWB","BWBWBBWWBWBB","BBWBWBBWWBWB","BWBBWBBWWBWB","BWBWBWWBBWBB","BBWBWBWWBBWB","BWBBWBWWBBWB","BWBWBBWWBBWB","BBWBWBWBWWBB","BWBBWBWBWWBB","BBWBBWBWBWWB","BWBWBBWBWWBB","BBWBWBBWBWWB","BWBBWBBWBWWB","BWBWBWBBWWBB","BBWBWBWBBWWB","BWBBWBWBBWWB","BWBWBBWBBWWB","BBWWBWBWBWBB","BWWBBWBWBWBB","BBWWBBWBWBWB","BWWBWBBWBWBB","BBWWBWBBWBWB","BWWBBWBBWBWB","BWWBWBWBBWBB","BBWWBWBWBBWB","BWWBBWBWBBWB","BWWBWWBWWBWB","BWWBWWBWBWWB","BWWBWBWWBWWB","BWBWWBWWBWWB","BWWBWBBWBBWB"};
const char CHAR_ARRAY[NUMBER_OF_CHARS][1] = {"0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","-","."," ","$","/","+","%","*"};


char compareString(char charSequence[]){
	
	char (*cArray)[MAX_STRING_SIZE];
	cArray = COLOR_ARRAY;

	char *startPosition = cArray;

	char sequenceChar;
	char currentChar;

	for(int i = 0; i < NUMBER_OF_CHARS; i++)
	{
		for(int j = 0; j < MAX_STRING_SIZE; j++)
		{
			sequenceChar = charSequence[j];
			currentChar = *(*(cArray+i)+j);
			// printf("Resp %c | %c Curr\n", sequenceChar, currentChar);
			if(sequenceChar != currentChar){
				// printf("(%c) Not a Match!\n", charArray[i][0]);
				break;
			}
			if(j == MAX_STRING_SIZE - 1){
				sleep_ms(50);
				char matchedChar = CHAR_ARRAY[i][0];
				printf("\n MATCH FOUND: %c", matchedChar);
				cArray = startPosition;
				return matchedChar;
			}    
		}
		sleep_ms(50);
	}
	char error = '*';
	cArray = startPosition;     // -- RESET pointer location to start
	return error;
}


char * stringAnalysis(char colorString[]){

	char *finalStr = malloc(3);

	printf("\nStart reading colorString in function!\n");
	for (int k = 0; k < PATTERN_SIZE; k++){
		sleep_ms(50);
		printf("%c",colorString[k]);
	}
	printf("\nFinished reading colorString in function!\n");

	char charSequence[MAX_STRING_SIZE];
	int buffer = 0;
	char matchedChar = ' ';
	for(int i = 0; i < NUMBER_OF_SEQUENCES; i++){
		for(int j = 0; j < MAX_STRING_SIZE; j++){
			charSequence[j] = colorString[buffer+j];
			// printf("color in colorString: %c", colorString[buffer+j]);
		}

		printf("\nStart reading charSequence!\n");
		for (int l = 0; l < MAX_STRING_SIZE; l++){
			sleep_ms(50);
			printf("%c",charSequence[l]);
		}
		printf("\nFinished reading charSequence!\n");
		sleep_ms(50);
		
		finalStr[i] = compareString(charSequence);
		printf("\n(%d) Retrieved Char: %c\n", i+1, finalStr[i]);
		buffer += MAX_STRING_SIZE + 1;
		sleep_ms(50);
	}   

	sleep_ms(100);
	return finalStr;
}


int main() {
	stdio_init_all();
	uint16_t initial_result;
	uint16_t second_result;
	uint16_t third_result;
	uint16_t result;
	float initial_value;
	float second_value;
	float third_value;
	float value;
	float black_threshold;
	float white_threshold;
	bool error_flag = false;

	adc_init();

	// Make sure GPIO is high-impedance, no pullups etc
	adc_gpio_init(28);
	// Select ADC input 0 (GPIO26) - OLD
	// Select ADC input 2 (GPIO28) - CURRENT
	adc_select_input(2);
	float values[BARCODE_SIZE];
	int position = 0;
	for(int i = 0; i < BARCODE_SIZE; i++)
		values[i] = 0;

	while (1) {
		// -- OVERALL READING 
		initial_result = adc_read(); 	// -- RAW VALUE: 0xfe5
		initial_value = initial_result * CONVERSION_FACTOR * MARGIN_MULTIPLIER;

		printf("[*] Looking for white bars...\n");
		// -- WHILE its BLACK (at the start)
		while (initial_value > THRESHOLD_VALUE) {									
			
			// -- Start finding for WHITES
			second_result = adc_read();
			second_value = second_result * CONVERSION_FACTOR * MARGIN_MULTIPLIER;
			
			// -- FIRST WHITE DETECTED
			if (second_value <= THRESHOLD_VALUE){		

				// -- STORE SECOND_VALUE FIRST					
				values[position] += (second_value + PADDING_VALUE);

				// -- TRAP the code to START SENSING BARCODE
				while (1){
					third_result = adc_read(); 	// -- RAW VALUE: 0xfe5
					third_value = third_result * CONVERSION_FACTOR * MARGIN_MULTIPLIER;
					values[position] += (third_value + PADDING_VALUE);
					
					// -- FULL WHITE (LOW LEVEL)
					if (third_value <= THRESHOLD_VALUE) {         							
						printf("\tWHITE 00-> %0.2f\n", third_value + PADDING_VALUE);
						while(1){
							result = adc_read(); 											
							value = result * CONVERSION_FACTOR * MARGIN_MULTIPLIER;

							// -- If WHITE -> CONTINUE
							if(value < THRESHOLD_VALUE){
								printf("\tWHITE 00-> %0.2f\n", value + PADDING_VALUE);
								values[position] += (value + PADDING_VALUE);
							}
							// -- If BLACK -> BREAK
							else{
								if (position == BARCODE_SIZE-1)
									break;
								printf("[!] BLACK DETECTED with value %0.2f\n", value + PADDING_VALUE*3);
								position++;
								printf("[!] Inserting at array %d\n", position);
								values[position] += (value + PADDING_VALUE*3);
								break;
							}
							sleep_ms(50);
						}	// -- END OF FOURTH WHILE LOOP
					}

					// -- FULL BLACK (HIGH VALUE)
					else{                                               						
						printf("\tBLACK 11-> %0.2f\n", third_value + PADDING_VALUE*3);
						while(1){
							result = adc_read(); 											
							value = result * CONVERSION_FACTOR * MARGIN_MULTIPLIER;

							// -- Detect if the previous white detection is a False Positive
							// -- To show that we are back on black main track
							if (values[position] > UPPER_BLACK_LIMIT){
								printf("[!] Detected a False Positive of white detected!\n");
								// -- RESET VALUES ARRAY
								for(int i = 0; i < BARCODE_SIZE; i++)
									values[i] = 0;
								// -- RESET POSITION back to 0
								position = 0;
								// -- SET ERROR FLAG
								error_flag = true;
								// BREAKS out of FOURTH WHILE LOOP -> BACK INTO THIRD WHILE LOOP
								break;
							}

							// -- If BLACK -> CONTINUE
							if(value > THRESHOLD_VALUE){										
								printf("\tBLACK 11-> %0.2f\n", value + PADDING_VALUE*3);
								values[position] += (value + PADDING_VALUE*3);
							}
							// -- If WHITE -> BREAK
							else{
								if (position == BARCODE_SIZE-1)
									break;
								printf("[!] WHITE DETECTED with value %0.2f\n", value + PADDING_VALUE);
								position++;
								printf("[!] Inserting at array %d\n", position);
								values[position] += (value + PADDING_VALUE);
								break;
							}
							if (position == BARCODE_SIZE-1)
								break;
							sleep_ms(50);
						} // -- END OF FOURTH WHILE LOOP
					}

					// -- BREAKS out of THIRD WHILE LOOP -> BACK INTO SECOND WHILE LOOP
					if ((position == BARCODE_SIZE-1) || (error_flag == true))
						break;
					sleep_ms(50);

				} // -- END OF THIRD WHILE LOOP
				
			}
		
			sleep_ms(50);
				
			if ((position == BARCODE_SIZE-1) && (error_flag == false)){
				black_threshold = (((values[1] + values[3] + values[9])/3) + ((values[5] + values[7])/2))/2;
				white_threshold = (((values[2])+ (values[4] + values[6] + values[8] + values[10])/4)/2)-5;
				
				printf("BLACK THRESHOLD: %f\n", black_threshold);
				printf("WHITE THRESHOLD: %f\n", white_threshold);

				char colorString[PATTERN_SIZE];
				int colorPos = -1;
				// -- Position 0 of pattern is always the color WHITE of the paper
				for(int j = 1; j < 31; j++){
					sleep_ms(100); 
					// -- where pattern character IS @ EVEN POSITION 
					if (j % 2 == 0){
						if (values[j] < white_threshold){
							colorPos++;
							colorString[colorPos] = 'W';
						}
						else{
							colorPos++;
							colorString[colorPos] = 'W';
							colorPos++;
							colorString[colorPos] = 'W';

						}
					}
					// -- where pattern character IS @ ODD POSITION 
					else{
						if (values[j] < black_threshold){
							colorPos++;
							colorString[colorPos] = 'B';
						}
						else{
							colorPos++;
							colorString[colorPos] = 'B';
							colorPos++;
							colorString[colorPos] = 'B';
						}
					}
				}

				printf("\n[*] colorString formed!\n\t");
				for (int k = 0; k < PATTERN_SIZE; k++){
					sleep_ms(50);
					printf("%c",colorString[k]);
				}
				char *finalResults = stringAnalysis(colorString);
				sleep_ms(100);
				for (int m; m < NUMBER_OF_SEQUENCES; m++){
					sleep_ms(50);
					printf("Sending Char: %c to comms\n", finalResults[m]);
					COMMS_DATA.barcode[m] = finalResults[m];
				}

				// -- RESET VALUES ARRAY
				for(int i = 0; i < BARCODE_SIZE; i++)
					values[i] = 0;
				// -- RESET POSITION back to 0
				position = 0;
				// BREAKS out of last WHILE LOOP -> BACK INTO FIRST WHILE LOOP
				break;
			}
			else if (error_flag == true){
				printf("[!] Error Flag detected -> Reverting back to scanning...\n");
				// -- RESET VALUES ARRAY
				for(int i = 0; i < BARCODE_SIZE; i++)
					values[i] = 0;
				// -- RESET POSITION back to 0
				position = 0;
				// -- RESET FLAG back to FALSE
				error_flag = false;
				// BREAKS out of last WHILE LOOP -> BACK INTO FIRST WHILE LOOP
				break;
			}

		} 	// -- END OF SECOND WHILE LOOP
		sleep_ms(50);
	}		// -- END OF FIRST WHILE LOOP


	printf("Program terminated...!");
	return 0;
}