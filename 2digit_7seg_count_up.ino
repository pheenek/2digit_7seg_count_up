
#define SEG1_IN1	PD6
#define SEG1_IN2	PD7
#define SEG1_IN3	PB0
#define SEG1_IN4	PB1

#define SEG2_IN1	PD2
#define SEG2_IN2	PD3
#define SEG2_IN3	PD4
#define SEG2_IN4	PD5

#define CTL_LATCH	PB2
#define CTL_BLANK	PB3
#define CTL_TEST	PB4

#define LED	PB5

#define BTN   PC0

bool btnState = false;
bool prevBtnState = true;
unsigned long lastBtnChangeMillis = millis();

int count = 0;

void setup() {
  Serial.begin(9600);
  initializeSevenSegment();
  initializeButton();
  DDRB |= (1 << DDB5);
}

void loop() {
  // Read button state
  btnState = readBtnState();
  // Do this only if the button has changed state
  if (btnState != prevBtnState)
  {
    PORTB ^= (1 << PORTB5);
    // Long press detected if the press is greater than 2000ms (Reset)
    if ((btnState) && ((millis() - lastBtnChangeMillis) > 2000))
    {
      // Button is released if the button state is true (HIGH)
      count = 0;
      updateDigit1();
      updateDigit2();
    }
    // Otherwise, it's a short press (Count up) 200ms for debounce
    if ((btnState) && (millis() - lastBtnChangeMillis > 200))
    {
      // Button is released if the button state is true (HIGH)
      // Set the maximum value (99) and increment
      count = count + 1 <= 99 ? count + 1 : 99;
      updateDigit1();
      updateDigit2();
      
      Serial.print("Count: ");
      Serial.println(count);
      Serial.println("Digit display");
    }
    lastBtnChangeMillis = millis();    
  }
  
  prevBtnState = btnState;
}

void initializeSevenSegment(void)
{
  // Set all the Seven segment display pins as outputs
  DDRD |= (1 << SEG1_IN1) | (1 << SEG1_IN2) | (1 << SEG2_IN3) | (1 << SEG2_IN4) | (1 << SEG2_IN1) | (1 << SEG2_IN2);
  DDRB |= (1 << SEG1_IN3) | (1 << SEG1_IN4);
}

void initializeButton(void)
{
  // Set the button pin as an input
  DDRC &= ~(1 << BTN);
}

bool readBtnState(void)
{
  return (PINC & (1 << BTN));
}

void updateDigit1(void)
{
  // Get the ones value
  int digit1 = count%10;

  // Display segments of the 1st digit
  uint8_t testBitMask = 0;
  int state = LOW;
  
  testBitMask = (1 << 0);
  state = (testBitMask & digit1) == testBitMask ? HIGH : LOW;
  digitalWrite(SEG1_IN1, state);
  
  testBitMask = (1 << 1);
  state = (testBitMask & digit1) == testBitMask ? HIGH : LOW;
  digitalWrite(SEG1_IN2, state);
  
  testBitMask = (1 << 2);
  state = (testBitMask & digit1) == testBitMask ? HIGH : LOW;
  digitalWrite(SEG1_IN3, state);
  
  testBitMask = (1 << 3);
  state = (testBitMask & digit1) == testBitMask ? HIGH : LOW;
  digitalWrite(SEG1_IN4, state);
}

void updateDigit2(void)
{
  // Get the tens value
  int digit2 = count/10;

  // Display segments of the 2nd digit
  uint8_t testBitMask = 0;
  int state = LOW;
  
  testBitMask = (1 << 0);
  state = (testBitMask & digit2) == testBitMask ? HIGH : LOW;
  digitalWrite(SEG2_IN1, state);
  
  testBitMask = (1 << 1);
  state = (testBitMask & digit2) == testBitMask ? HIGH : LOW;
  digitalWrite(SEG2_IN2, state);
  
  testBitMask = (1 << 2);
  state = (testBitMask & digit2) == testBitMask ? HIGH : LOW;
  digitalWrite(SEG2_IN3, state);
  
  testBitMask = (1 << 3);
  state = (testBitMask & digit2) == testBitMask ? HIGH : LOW;
  digitalWrite(SEG2_IN4, state);
}
