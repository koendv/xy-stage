/* 
 * microscope mechanical stage
 * commands:
 * w forward s backward a left d right q home
 * W fast forward S fast backward A fast left D fast right
 */

#include <Arduino.h>

// B (pin D8, D9: endstops)
// C (pin A2..A5: x stepper)
// D (pin D4..D7: y stepper)

// stop homing if homing switch does not change after HOME_MAX_STEPS steps
#define HOME_MAX_STEPS 720

#define X0_MAX 240
#define X0_MIN -240
#define Y0_MAX 240
#define Y0_MIN -240

#define ENDSTOP_X 8
#define ENDSTOP_Y 9

#define STEPPER_X_A1 (0x01 << PINC5)
#define STEPPER_X_A2 (0x01 << PINC4)
#define STEPPER_X_B1 (0x01 << PINC3)
#define STEPPER_X_B2 (0x01 << PINC2)

#define STEPPER_Y_A1 (0x01 << PIND4)
#define STEPPER_Y_A2 (0x01 << PIND5)
#define STEPPER_Y_B1 (0x01 << PIND6)
#define STEPPER_Y_B2 (0x01 << PIND7)

#define STEPPER_PORT_X PORTC
#define STEPPER_PORT_Y PORTD

#define STEPPER_DDR_X DDRC
#define STEPPER_DDR_Y DDRD

#define ALL_STEPPER_PINS_X (STEPPER_X_A1 | STEPPER_X_A2 | STEPPER_X_B1 | STEPPER_X_B2)
#define ALL_STEPPER_PINS_Y (STEPPER_Y_A1 | STEPPER_Y_A2 | STEPPER_Y_B1 | STEPPER_Y_B2)

const int stepper_table_x[8]{ STEPPER_X_A1, STEPPER_X_A1 | STEPPER_X_B1, STEPPER_X_B1, STEPPER_X_B1 | STEPPER_X_A2, STEPPER_X_A2, STEPPER_X_A2 | STEPPER_X_B2, STEPPER_X_B2, STEPPER_X_B2 | STEPPER_X_A1 };
const int stepper_table_y[8]{ STEPPER_Y_A1, STEPPER_Y_A1 | STEPPER_Y_B1, STEPPER_Y_B1, STEPPER_Y_B1 | STEPPER_Y_A2, STEPPER_Y_A2, STEPPER_Y_A2 | STEPPER_Y_B2, STEPPER_Y_B2, STEPPER_Y_B2 | STEPPER_Y_A1 };

static unsigned int step_x = 0;
static unsigned int step_y = 0;

static int x0 = 0;
static int y0 = 0;

bool homed = false;
uint32_t last_update_millis = 0;

void update_pins() {
  step_x %= 8;
  step_y %= 8;
  STEPPER_PORT_X = (STEPPER_PORT_X & ~ALL_STEPPER_PINS_X) | stepper_table_x[step_x];
  STEPPER_PORT_Y = (STEPPER_PORT_Y & ~ALL_STEPPER_PINS_Y) | stepper_table_y[step_y];
  delay(5);  // lower for faster
  last_update_millis = millis();
}

void idle_pins() {
  uint32_t now_millis = millis();
  if (now_millis > last_update_millis + 30000) {
    // sleep
    STEPPER_PORT_X = (STEPPER_PORT_X & ~ALL_STEPPER_PINS_X);
    STEPPER_PORT_Y = (STEPPER_PORT_Y & ~ALL_STEPPER_PINS_Y);
    last_update_millis = millis();
  }
}
void home() {
  int maxcnt;
  homed = false;
  maxcnt = HOME_MAX_STEPS;
  /* x axis  home */
  while (digitalRead(ENDSTOP_X)) {
    if (--maxcnt == 0) return;
    step_x += 7;
    update_pins();
  };
  while (!digitalRead(ENDSTOP_X)) {
    if (--maxcnt == 0) return;
    step_x += 1;
    update_pins();
  };
  /* y axis  home */
  maxcnt = HOME_MAX_STEPS;
  while (digitalRead(ENDSTOP_Y)) {
    if (--maxcnt == 0) return;
    step_y += 7;
    update_pins();
  };
  while (!digitalRead(ENDSTOP_Y)) {
    if (--maxcnt == 0) return;
    step_y += 1;
    update_pins();
  };
  x0 = 0;
  y0 = 0;
  homed = true;
}

// print coordinates in micron
void print_coords() {
  Serial.print(x0 * 5);
  Serial.print(' ');
  Serial.print(y0 * 5);
  Serial.println();
}

void stage_left() {
  if (x0 > X0_MIN) {
    step_x += 7;
    --x0;
    update_pins();
  }
}

void stage_right() {
  if (x0 < X0_MAX) {
    step_x += 1;
    x0++;
    update_pins();
  }
}

void stage_forward() {
  if (y0 > Y0_MIN) {
    step_y += 7;
    --y0;
    update_pins();
  }
}

void stage_backward() {
  if (y0 < Y0_MAX) {
    step_y += 1;
    y0++;
    update_pins();
  }
}

// blink led once per second if homing failed
  void blink_led() {
  static uint8_t led_val = 0xff;
  uint8_t new_val = !homed && ((millis() >> 9) & 0x1);
  if (led_val != new_val) {
    digitalWrite(LED_BUILTIN, new_val);
    led_val = new_val;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ENDSTOP_X, INPUT);
  pinMode(ENDSTOP_Y, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);
  STEPPER_DDR_X |= ALL_STEPPER_PINS_X;
  STEPPER_DDR_Y |= ALL_STEPPER_PINS_Y;
  update_pins();
  Serial.println("xy stage - w forward s backward a left d right x home");
  home();
  Serial.println(homed ? "ok" : "error");
}

void loop() {
  const int repeat_cnt = 10;
  if (Serial.available() > 0) {
    char ch = Serial.read();
    // on qwerty keyboards use WASD keys; on azerty keyboard use ZQSD keys.
    switch (ch) {
      case 'a':
      case 'q':
        stage_left();
        break;
      case 'd':
        stage_right();
        break;
      case 'w':
      case 'z':
        stage_forward();
        break;
      case 's':
        stage_backward();
        break;
      case 'A':
      case 'Q':
        for (int i = 0; i < repeat_cnt; i++)
          stage_left();
        break;
      case 'D':
        for (int i = 0; i < repeat_cnt; i++)
          stage_right();
        break;
      case 'W':
      case 'Z':
        for (int i = 0; i < repeat_cnt; i++)
          stage_forward();
        break;
      case 'S':
        for (int i = 0; i < repeat_cnt; i++)
          stage_backward();
        break;
      case 'X':
      case 'x':
        home();
        break;
      default:
        break;
    }
    print_coords();
  }
  idle_pins();
  blink_led();
}
// not truncated
