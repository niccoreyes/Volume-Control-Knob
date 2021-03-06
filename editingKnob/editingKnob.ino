// see tutorial at http://learn.adafruit.com/trinket-usb-volume-knob

#include "TrinketHidCombo.h"
#define turningRight 1
#define turningLeft 0



#define PIN_ENCODER_A 2
#define PIN_ENCODER_B 0
#define ATTINY_PINx  PINB
#define PIN_ENCODER_SWITCH 1
#define numberOfStates 4

static uint8_t enc_prev_pos   = 0;
static uint8_t enc_flags      = 0;
static char    sw_was_pressed = 0;
uint8_t state = 3-1;
bool startup = true;
const char * stateType[] = { "arrow shift", "media", "premiere pro", "photoshop" };

//clicking variables
static long holdTime      = 500;
static long longHoldTime   = 700;
unsigned long buttonTimer = 0;
unsigned long doubleClickTimer = 0;
bool checkForDoubleClick = false;

void setup() {
  // set pins as input with internal pull-up resistors enabled
  pinMode(PIN_ENCODER_A, INPUT);
  pinMode(PIN_ENCODER_B, INPUT);
  digitalWrite(PIN_ENCODER_A, HIGH);
  digitalWrite(PIN_ENCODER_B, HIGH);

  pinMode(PIN_ENCODER_SWITCH, INPUT);
  // the switch is active-high, not active-low
  // since it shares the pin with Trinket's built-in LED
  // the LED acts as a pull-down resistor
  digitalWrite(PIN_ENCODER_SWITCH, HIGH);

  TrinketHidCombo.begin(); // start the USB device engine and enumerate

  // get an initial reading on the encoder pins
  if (digitalRead(PIN_ENCODER_A) == LOW) {
    enc_prev_pos |= (1 << 0);
  }
  if (digitalRead(PIN_ENCODER_B) == LOW) {
    enc_prev_pos |= (1 << 1);
  }
}

void loop() {
  int8_t enc_action = 0; // 1 or -1 if moved, sign is direction

  // note: for better performance, the code will now use
  // direct port access techniques
  // http://www.arduino.cc/en/Reference/PortManipulation
  uint8_t enc_cur_pos = 0;
  // read in the encoder state first
  if (bit_is_clear(ATTINY_PINx, PIN_ENCODER_A)) {
    enc_cur_pos |= (1 << 0);
  }
  if (bit_is_clear(ATTINY_PINx, PIN_ENCODER_B)) {
    enc_cur_pos |= (1 << 1);
  }

  // if any rotation at all
  if (enc_cur_pos != enc_prev_pos) {
    if (enc_prev_pos == 0x00) {
      // this is the first edge
      if (enc_cur_pos == 0x01) {
        enc_flags |= (1 << 0);
      }
      else if (enc_cur_pos == 0x02) {
        enc_flags |= (1 << 1);
      }
    }

    if (enc_cur_pos == 0x03) {
      // this is when the encoder is in the middle of a "step"
      enc_flags |= (1 << 4);
    }
    else if (enc_cur_pos == 0x00) {
      // this is the final edge
      if (enc_prev_pos == 0x02) {
        enc_flags |= (1 << 2);
      }
      else if (enc_prev_pos == 0x01) {
        enc_flags |= (1 << 3);
      }

      // check the first and last edge
      // or maybe one edge is missing, if missing then require the middle state
      // this will reject bounces and false movements
      if (bit_is_set(enc_flags, 0) && (bit_is_set(enc_flags, 2) || bit_is_set(enc_flags, 4))) {
        enc_action = 1;
      }
      else if (bit_is_set(enc_flags, 2) && (bit_is_set(enc_flags, 0) || bit_is_set(enc_flags, 4))) {
        enc_action = 1;
      }
      else if (bit_is_set(enc_flags, 1) && (bit_is_set(enc_flags, 3) || bit_is_set(enc_flags, 4))) {
        enc_action = -1;
      }
      else if (bit_is_set(enc_flags, 3) && (bit_is_set(enc_flags, 1) || bit_is_set(enc_flags, 4))) {
        enc_action = -1;
      }

      enc_flags = 0; // reset for next time
    }
  }

  enc_prev_pos = enc_cur_pos;

  if (enc_action > 0) turnAction(turningRight);
  else if (enc_action < 0) turnAction(turningLeft);


  if (bit_is_set(ATTINY_PINx, PIN_ENCODER_SWITCH)) {
    if (sw_was_pressed == 0) {// only on initial press, so the keystroke is not repeated while the button is held down

    }
    sw_was_pressed = 1;
  }
  else {
    if (sw_was_pressed != 0) {
      buttonTimer = millis();
      while (!digitalRead(PIN_ENCODER_SWITCH));
      if (millis() - buttonTimer > longHoldTime) {
        state++;
        if (state > numberOfStates - 1)state = 0; //cycle modes (n) in n-1
        printOut(stateType[state]);
      }
      else {
        if (!checkForDoubleClick) {
          checkForDoubleClick = true;
          doubleClickTimer = millis();
        }
        else if (millis() - doubleClickTimer < holdTime && checkForDoubleClick) {
          clickAction(true);
          checkForDoubleClick = false;
          doubleClickTimer = 0;
        }
      }
      delay(5); // debounce delay
    }
    sw_was_pressed = 0;
  }
  if (checkForDoubleClick) {
    if (millis() - doubleClickTimer > holdTime) {
      clickAction(false);
      checkForDoubleClick = false;
      doubleClickTimer = 0;
    }
  }
  TrinketHidCombo.poll(); // check if USB needs anything done
}


void printOut(const char* c) {
  TrinketHidCombo.pressKey(KEYCODE_MOD_LEFT_GUI, KEYCODE_R);
  TrinketHidCombo.pressKey(0, 0);
  delay(200);
  TrinketHidCombo.print(c);
  delay(500);
  TrinketHidCombo.pressKey(0,KEYCODE_ESC);
  TrinketHidCombo.pressKey(0, 0);
  delay(5); // debounce delay
}

