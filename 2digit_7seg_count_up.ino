
#define A   PD3
#define B   PD2
#define C   PD6
#define D   PB0
#define E   PD7
#define F   PD4
#define G   PD5

#define DIG1  PB1
#define DIG2  PB2

#define BTN   PC0

bool btnState = false;
bool prevBtnState = true;
unsigned long lastBtnChangeMillis = millis();

int count = 0;
int digit1 = 0;
int digit2 = 0;

uint8_t digitBitMask[10] = {
  0000001,      // digit 0
  1001111,      // digit 1
  0010010,      // digit 2
  0000110,      // digit 3
  1001100,      // digit 4
  0100100,      // digit 5
  0100000,      // digit 6
  0001111,      // digit 7
  0000000,      // digit 8
  0000100       // digit 9
};

void setup() {
  initializeSevenSegment();
  initializeButton();
}

void loop() {
  // Read button state
  btnState = readBtnState();
  // Do this only if the button has changed state
  if (btnState != prevBtnState)
  {
    // Long press detected if the press is greater than 2000ms (Reset)
    if ((millis() - lastBtnChangeMillis) > 2000)
    {
      // Button is released if the button state is true (HIGH)
      if (!btnState)
      {
        count = 0;
        updateDigit1();
        updateDigit2(); 
      }
    }
    // Otherwise, it's a short press (Count up) 200ms for debounce
    else if (millis() - lastBtnChangeMillis > 200)
    {
      // Button is released if the button state is true (HIGH)
      if (!btnState)
      {
        // Set the maximum value (99) and increment
        count = count + 1 <= 99 ? count + 1 : 99;
        updateDigit1();
        updateDigit2(); 
      }
    }
    lastBtnChangeMillis = millis();    
  }
  
  prevBtnState = btnState;
}

void initializeSevenSegment(void)
{
  // Set all the Seven segment display pins as outputs
  DDRB |= (1 << D) | (1 << DIG1) | (1 << DIG2);
  DDRD |= (1 << A) | (1 << B) | (1 << C) | (1 << E) | (1 << F) | (1 << G);
}

void initializeButton(void)
{
  // Set the button pin as an input
  DDRC &= ~(1 << BTN);
}

bool readBtnState(void)
{
  return (PINB & (1 << BTN));
}

void updateDigit1(void)
{
  // Get the ones value
  digit1 = count%10;
  // Set DIG2 high and DIG1 low to enable output on the 1st digit
  PORTB |= (1 << DIG2);
  PORTB &= ~(1 << DIG1); 

  // Display segments of the 1st digit
  uint8_t testBitMask = 0;
  for (int i = 6; i >= 0; i++)
  {
    // Generate a bit mask with 1's at the test positions
    testBitMask = (1 << i);
    // Use the XOR operator to look for the zeros in the digit
    if (testBitMask ^ digitBitMask[digit2])
    {
      updateSegment(i, HIGH);
    }
    else 
    {
      updateSegment(i, LOW);
    }
  }
}

void updateDigit2(void)
{
  // Get the tens value
  digit2 = count/10;
  // Set DIG1 high and DIG2 low to enable output on the 2nd digit
  PORTB |= (1 << DIG1);
  PORTB &= ~(1 << DIG2);

  // Display segments of the 2nd digit
  uint8_t testBitMask = 0;
  for (int i = 6; i >= 0; i++)
  {
    // Generate a bit mask with 1's at the test positions
    testBitMask = (1 << i);
    // Use the XOR operator to look for the zeros in the digit
    if (testBitMask ^ digitBitMask[digit2])
    {
      updateSegment(i, HIGH);
    }
    else 
    {
      updateSegment(i, LOW);
    }
  }
}

void updateSegment(int i, int state)
{
  switch (i)
  {
    case 0:
      if (state == HIGH) digitalWrite(A ,HIGH);
      else digitalWrite(A, LOW);
      break;
    case 1:
      if (state == HIGH) digitalWrite(B ,HIGH);
      else digitalWrite(B, LOW);
      break;
    case 2:
      if (state == HIGH) digitalWrite(C ,HIGH);
      else digitalWrite(C, LOW);
      break;
    case 3:
      if (state == HIGH) digitalWrite(D ,HIGH);
      else digitalWrite(D, LOW);
      break;
    case 4:
      if (state == HIGH) digitalWrite(E ,HIGH);
      else digitalWrite(E, LOW);
      break;
    case 5:
      if (state == HIGH) digitalWrite(F ,HIGH);
      else digitalWrite(F, LOW);
      break;
    case 6:
      if (state == HIGH) digitalWrite(G ,HIGH);
      else digitalWrite(G, LOW);
      break;
  }
}
