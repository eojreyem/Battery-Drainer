#define EMPTY 0
#define DRAINING 1
#define DRAINED 2
#define FAULT 3
#define threshVoltToStart 3.9
#define voltAtDrained 3.75

int voltagePINS[] = {A0, A1, A2, A3, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A1};
int drainPINS[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
int statusLightPINS[] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51};

int ampHours[16];
long startTimes[16];
int batteryStatus[16];
long currentMillis;
int batVoltage;

void setup() {
  for (int i = 0; i <= 15; i++) {
    pinMode(voltagePINS[i], INPUT);   //Analog pins to read battery voltage.
    pinMode(drainPINS[i], OUTPUT);   //digital pins to turn on MOSFET and drain batteries.
    pinMode(statusLightPINS[i], OUTPUT);  // LEDs to indicate status
    digitalWrite(drainPINS[i], LOW);
    digitalWrite(statusLightPINS[i], LOW);
  }
  Serial.begin(9600);
  Serial.println("18650 Drainer");
}

void loop() {

  currentMillis = millis();

  for (int i = 0; i <= 15; i++) {  // iterate through all 16 batteries.
  }
  int i = 1;
    batVoltage = analogRead(voltagePINS[i]);
    
    if (batVoltage < 5) { //assume no battery present.
      if (batteryStatus[i] != EMPTY){
        Serial.print("Battery ");
        Serial.print(i);
        Serial.println(" empty.");
        batteryStatus[i] = EMPTY;
      }
      digitalWrite(statusLightPINS[i],LOW);
      digitalWrite(drainPINS[i],LOW);
    }    

    if (batteryStatus[i] == EMPTY) {
      if ((batVoltage > (threshVoltToStart*204.6)) && (batVoltage < (4.5*204.6))){ // battery seems charged! Set var to test.
        batteryStatus[i] = DRAINING;
        startTimes[i] = currentMillis;
        digitalWrite(drainPINS[i],HIGH);
      }
      if ((batVoltage >5) && ((threshVoltToStart*204.6)> batVoltage)){ //Battery not charged enough.
        batteryStatus[i] = FAULT;
        Serial.print("Battery ");
        Serial.print(i);
        Serial.println(" not charged enough");
        //TODO make statusLightPINS[i] blink!
      }      
      if (batVoltage > (4.5*204.6)){ //Voltage too high?!!
        batteryStatus[i] = FAULT;
        Serial.print("Battery ");
        Serial.print(i);
        Serial.println(" above 4.5V?!");
        //TODO make statusLightPINS[i] blink!
      }
    }

    if (batteryStatus[i] == DRAINING) {
      if (batVoltage < (voltAtDrained*204.6)){
        batteryStatus[i] = DRAINED; 
        digitalWrite(drainPINS[i],LOW);  //stop draining.
        digitalWrite(statusLightPINS[i],HIGH);  // turn on status light.
        Serial.print("battery ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print((currentMillis - startTimes[i])/1000);
        Serial.println(" secs to drained.");
      }else{
        Serial.print("battery ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(batVoltage/204.6);
        Serial.println(" volts.");
      }    
    }        
    delay(1000);   
}
