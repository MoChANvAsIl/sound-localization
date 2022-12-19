#include <Arduino.h>

#define NUM_LEDS 24
#define TRIGGERVALUE 700
#define TRIANGLESIDE 3430 // found by measuring time delay between 2 microphones in a straigt line
//  or by measuring the distance using the speed of sound (343m/s)
#define MAXTIME 2 * TRIANGLESIDE
#define DELAYTIME 20000 // wait before it can respond again
#define LEDDECAY 100

int micA = PA3;
int micB = PC0;
int micC = PC3;

bool micA_trigger = false;
bool micB_trigger = false;
bool micC_trigger = false;

unsigned long micA_timestamp, micB_timestamp, micC_timestamp;

uint8_t ledValues[NUM_LEDS - 1][3];

int testarr1[100];
int testarr2[100];
int testarr3[100];

int mic_value = 500;

bool test = false;
int x = 0;
void loop1();

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  loop1();
}

void loop1()
{
  // delay(10);

  x = 0;
  if (analogRead(micA) > mic_value )
  {
    test = true;
  }

  while (test)
  {
    testarr1[x] = analogRead(micA);
    testarr2[x] = analogRead(micB);
    testarr3[x] = analogRead(micC);

    x++;

    // Serial.println("OK");
    // Serial.println(analogRead(micA));

    if (analogRead(micA) < 500)
    {
      test = false;
    }
  }
  Serial.println("        ");

  int A_micArr[x];
  int B_micArr[x];
  int C_micArr[x];
  int xe = 1;

  if (x != 0)
  {
    for (int i = 0; i <= 100; i++)
    {
      // Serial.println(testarr[i]);
      if (testarr1[i] != 0)
      {
        A_micArr[xe] = testarr1[i];
        xe++;
      }
    }
  }

  for (size_t i = 0; i < x; i++)
  {
    Serial.println(A_micArr[i]);
  }
}

/*
if (!micA_trigger and analogRead(micA) > TRIGGERVALUE and micros() > micA_timestamp + DELAYTIME) {
    micA_timestamp = micros();
    micA_trigger = true;
  }
  if (!micB_trigger and analogRead(micB) > TRIGGERVALUE and micros() > micB_timestamp + DELAYTIME) {
    micB_timestamp = micros();
    micB_trigger = true;
  }
  if (!micC_trigger and analogRead(micC) > TRIGGERVALUE and micros() > micC_timestamp + DELAYTIME) {
    micC_timestamp = micros();
    micC_trigger = true;
  }

  if ((micros() - micA_timestamp > MAXTIME and micA_trigger) or
      (micros() - micB_timestamp > MAXTIME and micB_trigger) or
      (micros() - micC_timestamp > MAXTIME and micC_trigger)) {
    micA_trigger = false;
    micB_trigger = false;
    micC_trigger = false;
  } else if (micA_trigger and micB_trigger and micC_trigger) {
    if (micA_timestamp < micB_timestamp and micA_timestamp < micC_timestamp) {
      // micA triggered first
      ori = calculate(0, micB_timestamp - micA_timestamp, micC_timestamp - micA_timestamp, TRIANGLESIDE);
    } else if (micB_timestamp < micC_timestamp) {
      // micB triggered first
      ori = calculate(micA_timestamp - micB_timestamp, 0, micC_timestamp - micB_timestamp, TRIANGLESIDE);
    } else {
      // micC triggered first
      ori = calculate(micA_timestamp - micC_timestamp, micB_timestamp - micC_timestamp, 0, TRIANGLESIDE);
    }
    micA_trigger = false;
    micB_trigger = false;
    micC_trigger = false;

    if (ori.angle != -1) {
      // uint8_t led = map(ori.angle, 0, 360, NUM_LEDS, 0);
      // ledValues[(led - 1) % (NUM_LEDS)][2] = 150;
      // ledValues[led % (NUM_LEDS)][2] = 255;
      // ledValues[(led + 1) % (NUM_LEDS)][2] = 150;
      Serial.println(ori.angle);
    }
  }



*/

struct Origin
{
  double angle; // in degree
  double distance;
};

Origin ori = {0, 0};
int ledDecayValue = 0;

Origin calculate(double a, double b, double c, double G)
{
  double r_1 = (-2 * a * a * a + a * a * (b + c) - sqrt(3) * sqrt(-((a - b) * (a - b) - G * G) * ((a - c) * (a - c) - G * G) * ((b - c) * (b - c) - G * G)) - (b + c) * (2 * b * b - 3 * b * c + 2 * c * c - G * G) + a * (b * b + c * c + G * G)) / (4 * (a * a + b * b - b * c + c * c - a * (b + c)) - 3 * G * G);
  double r_2 = (-2 * a * a * a + a * a * (b + c) + sqrt(3) * sqrt(-((a - b) * (a - b) - G * G) * ((a - c) * (a - c) - G * G) * ((b - c) * (b - c) - G * G)) - (b + c) * (2 * b * b - 3 * b * c + 2 * c * c - G * G) + a * (b * b + c * c + G * G)) / (4 * (a * a + b * b - b * c + c * c - a * (b + c)) - 3 * G * G);

  if (r_1 == 0 and r_2 == 0)
  {
    Serial.println("ZERO");
    Origin ori = {-1, -1};
    return ori;
  }

  double i_1 = 2 * a * a * a * (b - c) + 2 * b * b * b * c + 4 * c * c * G * G - 3 * G * G * G * G;
  double i_2 = 3 * a * a * (2 * c * (-b + c) + G * G) + b * b * (-6 * c * c + 5 * G * G);
  double i_3 = sqrt(3) * sqrt(-((a - b) * (a - b) - G * G) * ((a - c) * (a - c) - G * G) * ((b - c) * (b - c) - G * G));
  double i_4 = b * b * b - 3 * b * b * c + 2 * c * c * c + (2 * b + c) * G * G;
  double i_5 = 8 * (a * a + b * b - b * c + c * c - a * (b + c)) * G - 6 * G * G * G;
  double i_7 = 2 * sqrt(3) * (-4 * (a * a + b * b - b * c + c * c - a * (b + c)) * G + 3 * G * G * G);

  double x, y;
  if (r_1 >= r_2)
  {
    x = (i_1 + i_2 + 2 * b * (2 * c * c * c - 3 * c * G * G + i_3) - 2 * a * (i_4 + i_3)) / i_5;
    y = (6 * b * b * b * (a - c) + 2 * a * i_3 + 2 * b * i_3 - 4 * c * i_3 - 3 * (a - G) * (a + G) * (2 * (a - c) * c + G * G) - 3 * b * b * (2 * (a - c) * (2 * a + c) + G * G) + 6 * b * (a * a * a + a * a * c - 2 * a * c * c + c * G * G)) / i_7;
  }
  else
  {
    x = (i_1 + i_2 + 2 * a * i_3 - 2 * a * (i_4)-2 * b * (-2 * c * c * c + 3 * c * G * G + i_3)) / i_5;
    y = (6 * b * b * b * (a - c) - 2 * a * i_3 - 2 * b * i_3 + 4 * c * i_3 - 3 * (a - G) * (a + G) * (2 * (a - c) * c + G * G) - 3 * b * b * (2 * (a - c) * (2 * a + c) + G * G) + 6 * b * (a * a * a + a * a * c - 2 * a * c * c + c * G * G)) / i_7;
  }

  double angle = atan2(y - G / (2 * sqrt(3)), x - 0.5 * G) * 180 / PI;
  if (angle < 0)
  {
    angle += 360;
  }
  double distance = sqrt((x - 0.5 * G) * (x - 0.5 * G) + (y - G / (2 * sqrt(3))) * (y - G / (2 * sqrt(3))));

  Origin ori = {angle, distance};

  return ori;
}