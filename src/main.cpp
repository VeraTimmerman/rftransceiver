#include <Arduino.h>
#define USER_LED    13

// put function declarations here:
int myFunction(int, int);
void write();
void led();

int current_state = LOW;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello world");
  pinMode(USER_LED, OUTPUT);
  led();
}

void loop() {
  // put your main code here, to run repeatedly:
  while(true)
  {
    write();
    led();
    delay(1000);
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

void write()
{
  int result = myFunction(2, 3);
  char buffer[32];
  memset (buffer, 0x00, 32);
  snprintf(buffer, 32, "Hello World %u", result);
  Serial.println(buffer);
}

void led()
{
  digitalWrite(USER_LED, current_state);

  char buffer[32];
  memset (buffer, 0x00, 32);
  snprintf(buffer, 32, "current_state %u", current_state);
  Serial.println(buffer);

  if(current_state == LOW)
  {
    current_state = HIGH;
  }
  else
  {
    current_state = LOW;
  }
}