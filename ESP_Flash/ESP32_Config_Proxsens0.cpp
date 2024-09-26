/*
First set of 2 sensors sending a positive signal to main controller if read value is less then the set maximum range
Const Short Numbers have to be changed before running according to the ESP controller used and change short to int if sensor output too high
*/

const short rangeL = 2; //input for range
const short rangeR = 5; //input for range
const short output0 = 16; //TX_0
const short output1 = 1; //TX_1
const short input = 6; //RX_0

short signal = 0;
unsigned short ragne_L_0 = 0;
unsigned short range_R_0 = 0;
bool i0 = false;
bool i1 = false;

const unsigned short max = 20; //the maximum range counted as true (needs to be calibrated if == 20)
//const unsigned short max1 = 10;
//const unsigned short max2 = 5;

int setup(){
	
	pinMode(rangeL,INPUT);
	pinMode(rangeR,INPUT);
	pinMode(output0,OUTPUT);
	pinMode(output1,OUTPUT);
	pinMode(input,INPUT);

	//create a task that will be executed in the Task2code() function, with priority 1 and executed on core 0
    xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1, 0);                       
    delay(500); 

  	//create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  	xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &Task2, 1);           
    delay(500); 
}
void Task1code(void * pvParameters){ //check Left faar sensor and locks itself if something has been spotted calling for other conntoller to verify, if verification fails returns back to the main programm
	ragne_L_0 = analogRead(rangeL);
	if (ragne_L_0 < max){
		digitalWrite(output0, HIGH); //sends signal to the main Controller initiating other sensors
		ragne_L_0 = NULL;
		for (i0, true, ){
			i0 = DigitalRead(input) == HIGH ? true : false;
		}
	}
	else {
		digitalWrite(output0, LOW);
	}
	digitalWrite(output0, LOW);
}
void Task2code(void * pvParameters){ //check Right faar sensor and locks itself if something has been spotted calling for other conntoller to verify, if verification fails returns back to the main programm
	range_R_0 = analogRead(rangeR);
	Calculate(ragne_R_0);
	if (ragne_R_0 < max){
		digitalWrite(output1, HIGH); //sends signal to the main Controller initiating other sensors
		range_R_0 = NULL;
		for (i1, true, ){ //loops untill the initiated other Controller Denies the verification
			i1 = DigitalRead(input) == HIGH ? true : false;
		}
	}
	else {
		digitalWrite(output1, LOW);
	}
	digitalWrite(output1, LOW);
}