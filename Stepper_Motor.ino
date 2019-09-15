int motorSpeed = 5000;
//int movToFinish = 512; // number of steps per full revolution 0,5mm
int perMiniMeter = 1024;
int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};
int invlookup[8] = {B01001,B00001,B00011,B00010,B00110,B00100,B01100,B01000,}; //Ters lookup
int sensorVal = LOW;
unsigned long currentPos = 0;
int currentPosMM = 0;
int thresholdVal = 51200; //50mm
int setToHome = 0;
unsigned long targetPos = 0; //kullanicidan alinacak
char serialByte[7];
unsigned long readVal[4];
char whereMM[4];
int arrCount = 0;

void setup() {
  pinMode(7,INPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);
}

// To Set Output //
void setOutput(int out)
{
  if (out == 9) {
    digitalWrite(8, 0);
    digitalWrite(9, 0);
    digitalWrite(10, 0);
    digitalWrite(11, 0);
  }
  else {
    digitalWrite(8, bitRead(lookup[out], 0));
    digitalWrite(9, bitRead(lookup[out], 1));
    digitalWrite(10, bitRead(lookup[out], 2));
    digitalWrite(11, bitRead(lookup[out], 3));
    delayMicroseconds(motorSpeed);
  }
}


// To Make System Go Home
void goHome()
{
  sensorVal = digitalRead(7);
  while (sensorVal == LOW) {
      for (int i = 8; i > 0; i--) {
        setOutput(i);
      }
      if (motorSpeed > 1000) {
        motorSpeed = motorSpeed - 1000;  
    }
    sensorVal = digitalRead(7);
    //Serial.println("<b00000>");
  }
  motorSpeed = 5000;
  sensorVal = digitalRead(7);
  while(sensorVal == HIGH) {
    for (int i = 0; i < 8; i++) {
      setOutput(i);
    }
    sensorVal = digitalRead(7);
   //Serial.println("<b00000>");
  }
  setOutput(9);
  setToHome = 1; // Home değerimiz 1 current position değeri de zaten programda 1 olduğu için ve bir daha tekrarlanmayacagi icin kalicak
  delayMicroseconds(2000);
  Serial.println("<athom>");
}
void loop() {
  if (setToHome == 0) {
    goHome();
    motorSpeed = 5000;
  }
  if (Serial.available() > 0) {
  Serial.readBytesUntil(' ',serialByte,8);
  if(serialByte[1]=='A' && serialByte[6] == '>'){
  Serial.println("<reciv>");
  for(int i =2;i<6;i++){readVal[i-2]=(uint8_t)serialByte[i] - 0x30;}
  targetPos = readVal[0]*10*perMiniMeter+readVal[1]*perMiniMeter+(readVal[2]*10+readVal[3])*0.01*perMiniMeter;
  arrCount = 1;
  }
  else if(serialByte[1]=='H' && serialByte[6] == '>'){
    setToHome = 0;
    Serial.println("<gohom>");
    }
  else if(serialByte[1]=='W' && serialByte[6] == '>'){
    currentPosMM = currentPos * 100 / perMiniMeter;
    sprintf(whereMM, "%04d", currentPosMM);
    Serial.print("<w");
    Serial.print(whereMM);
    Serial.println(">");
    }
  else{Serial.println("<err00>");}
  }
  //if(currentPos != targetPos){Serial.println("<b00000>");}
  if (targetPos > thresholdVal) {
    targetPos = thresholdVal; //max gidebileceği yeri geçemesin diye
  }
  if (targetPos < 0) {
    targetPos = 0; // - değer girilemez olsun diye
  }
  if (currentPos < targetPos) {
    for (int i = 0; i < 8; i++) {
      setOutput(i);
    }
    currentPos++;
    if (motorSpeed > 1000) {
      motorSpeed = motorSpeed - 1000;
    }
  }
  if (currentPos > targetPos) {
    for (int i = 8; i > 0; i--) {
      setOutput(i);
    }
    currentPos--;
    if (motorSpeed > 1000) {
      motorSpeed = motorSpeed - 1000;
    }
  }
  if (currentPos == targetPos && arrCount == 1) {
    Serial.println("<arriv>");
    arrCount = 0;
  }
  if (currentPos == targetPos) {
    setOutput(9);
  }
}
