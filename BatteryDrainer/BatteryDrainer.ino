#define EMPTY 0
#define DRAINING 1
#define DRAINED 2
#define FAULT 3
#define threshVoltToStart 4
#define voltAtDrained 3

int voltagePINS[] = {54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69};
int drainPINS[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
int statusLightPINS[] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51};
long startTimes[16];
long currentMillis;
int batteryStatus[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int batVoltage;

void setup() {
  //Analog pins to read battery voltage.
  for (int i = 0; i <= 15; i++) {
    pinMode(voltagePINS[i], INPUT);
    pinMode(drainPINS[i], OUTPUT);
    pinMode(statusLightPINS[i], OUTPUT);
  }
}

void loop() {

  currentMillis = millis();

  for (int i = 0; i <= 15; i++) {  // iterate through all 16 batteries.
    batVoltage = analogRead(voltagePINS[i]);

    if (batteryStatus[i] == EMPTY) {
      if ((batVoltage > (threshVoltToStart*204.6)) && (batVoltage < (4.5*204.6))){ // battery seems charged! Set var to test.
        batteryStatus[i] = DRAINING;
        startTimes[i] = currentMillis;
        digitalWrite(drainPINS[i],HIGH);
      }
      if ((batVoltage >5) && (threshVoltToStart*204.6> batVoltage)){ //Battery not charged enough.
        batteryStatus[i] = FAULT;
        //TODO make statusLightPINS[i] blink!
      }      
      if (batVoltage > (4.5*204.6)){ //Voltage too high?!!
        batteryStatus[i] = FAULT;
        //TODO make statusLightPINS[i] blink!
      }
    }
    
    if (batVoltage < 5) { //assume battery removed.
      batteryStatus[i] = EMPTY;
      digitalWrite(statusLightPINS[i],LOW);
    }    

    if (batteryStatus[i] == DRAINING) {
      if (batVoltage < (voltAtDrained*204.6)){
        batteryStatus[i] = DRAINED; 
        digitalWrite(drainPINS[i],LOW);  //stop draining.
        digitalWrite(statusLightPINS[i],HIGH);  // turn on status light.
        Serial.print("battery ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(currentMillis - startTimes[i]/1000);
        Serial.println(" secs to drained.");
      }else{
        Serial.print("battery ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(batVoltage/204.6);
        Serial.println(" volts.");
      }    
    }        
    
  }
}
