/*
 * Serial Port Monitor
 * http://arduino.cc/playground/Csharp/SerialCommsCSharp
 * 
 */
// helicopter controls
//http://codingfun.collinboy.net/

// Connect (+) of IR LED to 5Vcc
// connect (-) to pin 4 with a resistor in line.  For best results make a transistor circuit with external power source

#define RED 4       // the output pin of the IR LED
 
 
//Setup Output
//Setup message bytes
byte inputByte_0;
byte inputByte_1;
byte inputByte_2;
byte inputByte_3;
byte inputByte_4;
//Setup
void setup() {
  pinMode(RED, OUTPUT);            // set IR LED (Pin 4) to Output
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

bool hand_shake_complete = false;
int x1=63, x2=63, x3=0, x4=63, x5=103;
void postHandShakeLoop() {
  if (Serial.available() >= 5) {
    x1=Serial.read();
    x2=Serial.read();
    x3=Serial.read();
    x4=Serial.read();
    x5=Serial.read();
    Serial.write(103);
  }
  if(x5!=103) {
    hand_shake_complete=false;
    clearSerial();
    return;
  }
  Transmit(x1, x2, x3, x4);
}

//Main Loop
void loop() {
  digitalWrite(13, hand_shake_complete);
  if(hand_shake_complete) {
    postHandShakeLoop();
    return;
  }
  
  //Read Buffer
  if (Serial.available() >= 2) {
    inputByte_0 = Serial.read(); 
    inputByte_1 = Serial.read();
  } else return;
  //Check for start of Message
  if(inputByte_0 == 16 && inputByte_1==128) {
    //Say hello
    Serial.print("HELLO FROM ARDUINO");
    hand_shake_complete=true;
  } else {
    clearSerial();
  }
}

void clearSerial() {
  delay(100);
  while(Serial.available()>0) Serial.read();
}


void Transmit(byte rudder, byte elevator, byte throttle, byte trim) {
static byte Code[4];
byte mask = 128;         //bitmask
int i;

Code[0] = rudder; // 0 -> 127; 63 is the midpoint.
Code[1] = elevator; // 0 -> 127; 63 is the midpoint.
Code[2] = throttle; // 0 -> 127; 0 is throttle off
Code[3] = trim;             // Haven't messed with this 

OutPulse(2002);  // Start 38Khz pulse for 2000us (2002us is evenly divided by 26) 
delayMicroseconds(2000);  // 2000us off.

for (i = 0; i<4; i++) {                     // Loops through the Code[i] 
for (mask = 128; mask > 0; mask >>=1) {         // See Arduino reference for bit masking (really cool stuff!) 
OutPulse(312);                     // Sends 312 pulse each loop

if(Code[i] & mask) {          //If both bit positions are 1 you get 1             
delayMicroseconds(688);     // send 1 (700 off)
} 
else {
delayMicroseconds(288);     // send 0 (300 off)
}
} //End mask loop
}  //End i loop

OutPulse(312);  //Send 300 microsecond Closing Pulse 
delay(60);       

} // End Transmit 
void OutPulse(int Pulse) {  //sends 38Khz pulses over the Pulse Length 
int p;

for(p = 0; p < (Pulse / 26) - 1; p++) {  //Takes about 26 microseconds per loop
digitalWrite(RED, HIGH);
delayMicroseconds(10);
digitalWrite(RED, LOW);
delayMicroseconds(10);
}
}  //End OutPulse
