/*
First set of 2 sensors sending a positive signal to main controller if read value is less then the set maximum range
Const Short Numbers have to be changed before running according to the ESP controller used and change short to int if sensor output too high
*/

const short input_range_L = 2; //input for range
const short input_range_R = 5; //input for range
const short Tx_main0 = 16; //TX_0
const short Rx_main0 = 6; //RX_0

const bool F = false;
const bool T = true;

unsigned short ragne_L_0 = 0;
unsigned short range_R_0 = 0;
bool i0 = F;
bool accept0 = F;
bool accept1 = F;

const unsigned short max = 20; //the maximum range counted as true (needs to be calibrated if == 20)
//const unsigned short max1 = 10;
//const unsigned short max2 = 5;

int setup(){
	
	pinMode(input_range_L,INPUT);
	pinMode(input_range_R,INPUT);
	pinMode(Tx_main0,OUTPUT);
	pinMode(Rx_main0,INPUT);

	//create a task that will be executed in the Task2code() function, with priority 1 and executed on core 0
    xTaskCreatePinnedToCore(Task1code, "Task0", 10000, NULL, 1, &Task0, 0);                       
    delay(500); 

  	//create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  	xTaskCreatePinnedToCore(Task2code, "Task1", 10000, NULL, 1, &Task1, 1);           
    delay(500); 
}
// constantly reads values
void Task0code(void * pvParameters){
	for(;;)
		while (stop_range == F){
			ragne_L = analogRead(input_range_L);
			accept0 = (ragne_L_0 < max) ? T : F;
			ragne_R = analogRead(input_range_R);
			accept1 = (ragne_R_0 < max) ? T : F;
		}

		while (stop_range == T){
			i0 = (DigitalRead(Rx_main0) == HIGH) ? T : F;
			if(i0 == T){
				stop_range = F;
			}
		}
}
//check Right faar sensor and locks itself if something has been spotted calling for other conntoller to verify, if verification fails returns back to the main programm
void Task1code(void * pvParameters){
	for(;;){
		switch(accept0){
			case T:
				DigitalWrite(Tx_main0, HIGH);
				stop_range = T;
				
				for (i0 = F; T; ){
				}

				DigitalWrite(Tx_main0, LOW);
				accept0 = F;
				break;

			default:
				switch(accept1){
					case true:
						DigitalWrite(Tx_main0, HIGH);
						stop_range = T;

						for (i0 = F; T; ){
						}

						DigitalWrite(Tx_main0, LOW);
						accept1 = F;
						break;

					default:
						DigitalWrite(Tx_main0, LOW);
						break;
				}
				break;
		}
	}
}