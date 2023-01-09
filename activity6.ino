// 0 = 0111111 = 63
// 1 = 0000110 = 6
// 2 = 1011011 = 91
// 3 = 1001111 = 79
// 4 = 1100110 = 102
// 5 = 1101101 = 109
// 6 = 1111101 = 125
// 7 = 0000111 = 7
// 8 = 1111111 = 127
// 9 = 1101111 = 111 


  
//millis variableS
int millisTime;
int pressTime;
int millisTime2;
int pressTime2;
int x = 0;
int y = 0;
int a = 2;

//7SD variables
int latchPin1 = 7;
int clockPin1 = 8;
int dataPin1 = 6;
int number = 0;
int numList[] = {6, 91, 79, 102, 109, 125, 7, 127, 111};
//led variables
int latchPin2 = 4;
int clockPin2 = 5;
int dataPin2 = 3;
int leds = 0;
int ledList[] = {1, 3, 7, 15, 31, 63, 127};
//button variables
int b1 = 9;
int b2 = 10;
int b3 = 11;
int b4 = 12;
int flag = 0;
int i;

void testDelay(int delayTime, int test)
{
  for (int d = 0; d < delayTime; d++)
  {
    delay(1);
    updateFlag();
    if (flag != test) break;
  }
}

void button1()
{
  number = 63; // 63 binary displays 0
  leds = 0;
  updateShiftRegister();
  updateFlag();
  testDelay(500, 1);
  for (int i = 0; i < 15; i++)
  {    
    int numList[] = {6, 91, 79, 102, 109, 125, 7, 127, 7, 125, 109, 102, 79, 91, 6};
    int ledList[] = {1, 3, 7, 15, 31, 63, 127, 255, 254, 252, 248, 240, 224, 192, 128};
    number = numList[i];
    leds = ledList[i];
    updateShiftRegister();
    testDelay(500, 1);
  }

}

void button2()
{

  
  leds = 0; // turns all lights off
  updateShiftRegister();
  updateFlag();
  
 
  
  testDelay(350, 2); 
  
  for (int i = 0; i < 7; i++)
  {
    int ledList[] = {129, 195, 231, 255, 231, 195, 129};
    leds = ledList[i];
    updateShiftRegister();
    testDelay(350, 2);
  }
 
}
  
void button3()
{
  int numList[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};
  int ledList[] = {1, 2, 4, 8, 16, 32, 64, 128, 64, 32, 16, 8, 4, 2, 1};
  
  leds = 0;
  updateShiftRegister();
 
  for (int i = 9; i > 0; i--)
  {    
    number = numList[i];
    updateShiftRegister();
    testDelay(1000, 3);
  }
  
  
  
  a = 0;
    
  while(flag == 3)

  {
    for (int i = 0; i < 15; i++)
    {
      leds = ledList[i];
      updateShiftRegister();
      testDelay(100, 3);
    }
  }
  
}
  
  
void button4()
{
  leds = 0;
  number = 63;
  updateShiftRegister();
}

void timer()
{
  
  int numList[] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};
  
  int interval = 1000;
  if (flag == 2 && millisTime - pressTime >= interval)
  {
    number = numList[x];
    y += 1; // since millis never ends, y will increase forever...
    x = (y % 10); // this is to take only the one digit of y value... (bonus: this stays unaccfected incase of millis turnover)
    pressTime = millisTime;
    updateShiftRegister();
  }
  else if (flag != 2) y = -1; // resetting mechanism 
  
    
}

void flash()
{
  if (flag != 3) a = 2;

  int interval = 250;
  if (a == 0 && flag == 3 && millisTime2 - pressTime2 >= interval)   
  {
    Serial.println(a);
    number = 63; 
    updateShiftRegister();
    pressTime2 = millisTime2;
    a = 1;
  }
  
  
  if (a == 1 && flag == 3 && millisTime2 - pressTime2 >= interval)
  {
    Serial.println(a);
    number = 0; 
    updateShiftRegister();
    pressTime2 = millisTime2;
    a = 0;               
  }  
}


void setup()
{
  Serial.begin(9600);
  
  pinMode(latchPin1, OUTPUT);
  pinMode(dataPin1,  OUTPUT);  
  pinMode(clockPin1, OUTPUT);
  
  pinMode(latchPin2, OUTPUT);
  pinMode(dataPin2,  OUTPUT);  
  pinMode(clockPin2, OUTPUT);

  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(b4, INPUT_PULLUP);

}

void updateShiftRegister()
{
   digitalWrite(latchPin1, LOW);
   shiftOut(dataPin1, clockPin1, MSBFIRST, number);
   digitalWrite(latchPin1, HIGH);
  
   digitalWrite(latchPin2, LOW);
   shiftOut(dataPin2, clockPin2, LSBFIRST, leds);
   digitalWrite(latchPin2, HIGH);
  
}

void loop()
{
 
  
  updateFlag();
  while (flag == 1)
  {
    updateFlag();
    exeb1();
  }
  while (flag == 2)
  {
    updateFlag();
    exeb2();
  }
  while (flag == 3)
  {
    updateFlag();
    exeb3();
  }
  while (flag == 4)
  {
    updateFlag();
    exeb4();
  }
}
void updateFlag()
{
  if (digitalRead(b1) == LOW) flag = 1;
  if (digitalRead(b2) == LOW) flag = 2;
  if (digitalRead(b3) == LOW) flag = 3;
  if (digitalRead(b4) == LOW) flag = 4;
  
  //saves the time b2 is pressed first 
  if (digitalRead(b2) == LOW && flag != 2) pressTime = millis();
  if (digitalRead(b3) == LOW && flag != 3) pressTime2 = millis();
  //sets the timer
  if ( flag == 2) millisTime = millis(); 
  if ( flag == 3) millisTime2 = millis(); 
  
  timer();
  flash();


}


void exeb1()
{
  button1();
  updateFlag();
}

void exeb2()
{
  button2();
  updateFlag();
}

void exeb3()
{
 button3();
 
}
void exeb4()
{
  button4();
}




