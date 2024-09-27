/*
core 0 gets values from sensor and converts 
them into usable measurements

core 1 looks at the Converted values and acts 
accordingly ok
*/

const short readerX = 2; //input pin for angle on X axis
const short s_BlnkrL = 1; // Left warn lights pin
const short s_BlnkrR = 11; // Right warn lights pin
const unsigned short conversion = 123123; // find out the range of gyroscope  then divide by 255 and the number as outcome put in unsigned short
short convAngleX = 0; //converted angle X
int angleX = 0; //raw data angle X
bool is_negative = false;
const short negative = -1;

int setup(){
	
	pinMode(readerX,INPUT);
	pinMode(s_BlnkrL,OUTPUT);
	pinMode(s_BlnkrR,OUTPUT);


    xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
    delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}
void Task1code(void * pvParameters){
    for(;;){
        angleX = analogRead(readerX);
        convAngleX = angleX / conversion;
        is_negative = convAngleX < 0;
    }
}
void Task2code(void * pvParameters){
    for(;;){
        switch(is_negative){
            case 1:
                analogWrite(s_BlnkrL, convAngleX * negative)
                break;
            case 0:
                analogWrite(s_BlnkrR, convAngleX)
                break;
        }
    }
}