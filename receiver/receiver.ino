// Receiver - laser pulses in, text out on the Serial Monitor and a 16x2 LCD.
// A0 is sampled every 20 ms (half a dot) and each run of samples above or below
// referenceValue is classified by the nearest expected duration.

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int dotDelay = 40;
int sensorPin = A0;
int sensorValue = 0;
int referenceValue = 410;
int countElectric = 1;
int countNaught = 1;
int tmp = 0;
String string = "";
String stringSum = "";

char* asci[] = {"..-..",".-.-","*","*","*","*",".-----.","-.--.","-.--.-","*",
                    ".-.-.","--..--","-....-",".-.-.-","-..-.","-----",".----","..---",
                    "...--","....-",".....","-....","--...","---..","----.","---...","-.-.-.",
                    "*","-...-","*","..--..","*",".-", "-...", "-.-.", "-..", ".", "..-.", "--.", 
                    "....", "..",".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.","...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..","*","*","*","*","..--.-","*",".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.","...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..","*","*","*","*","*"
                  };

int approximateValue(int a, int b, int value) {   // whichever of a, b is nearer to value
  if (abs(a - value) < abs(b - value)) {
    return a;
  } else if (abs(a - value) > abs(b - value)) {
    return b;
  }
}

int approximateValueQuad(int a, int b, int c, int d, int value) {
  return approximateValue(approximateValue(a, b, value), approximateValue(c, d, value), value);
}

void setup() {
  Serial.begin(9600);
  Serial.println("mos_reverse_translator");
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  // Calibrate against the ambient light: one reading with the beam blocked, one with the
  // beam on the sensor, and the decision boundary halfway between the two.
  int sensorValueBeforeRebooting = analogRead(sensorPin);
  Serial.print("sensorValueBeforeRebooting:");    Serial.println(sensorValueBeforeRebooting);
  while (1) {
    if (Serial.find("rebooting")) {
      Serial.println("rebooting");
      break;
    }
  }
  int sensorValueAfterRebooting = analogRead(sensorPin);
  Serial.print("sensorValueAfterRebooting: ");  Serial.println(sensorValueAfterRebooting);
  referenceValue = (sensorValueBeforeRebooting + sensorValueAfterRebooting) / 2;
  Serial.print("referenceValue: ");  Serial.println(referenceValue);
}

void loop() {
  sensorValue = analogRead(sensorPin);

  if (countElectric - countNaught == 1) {
    if (approximateValueQuad(dotDelay, dotDelay * 4, dotDelay * 5, dotDelay * 6, tmp) == dotDelay) {
    } else if (approximateValueQuad(dotDelay, dotDelay * 4, dotDelay * 5, dotDelay * 6, tmp) == dotDelay * 4) {
      for (int j = 0; j < 95; j++) {
        String str(asci[j]);
        if (string.equals(str) == 1) {
          char c = '!' + j;
          Serial.print("converted morse code -> ");  Serial.println(c); lcd.print(c);
          stringSum.concat(c);
          break;
        }
      }
      string.remove(0);
    } else if (approximateValueQuad(dotDelay, dotDelay * 4, dotDelay * 5, dotDelay * 6, tmp) == dotDelay * 5) {
      for (int j = 0; j < 95; j++) {
        String str(asci[j]);
        if (string.equals(str) == 1) {
          char c = '!' + j;
          Serial.print("converted morse code -> ");  Serial.println(c); lcd.print(c);
          stringSum.concat(c);
          break;
        }
      }
      string.remove(0);
      Serial.print("converted morse code -> ");   Serial.println("' '"); lcd.print("' '");
      stringSum.concat(' ');
    }
  } else if (countNaught - countElectric == 1) {
    if (approximateValue(dotDelay, dotDelay * 3, tmp) == dotDelay) {
      Serial.print(".");
      string.concat('.');
    } else if (approximateValue(dotDelay, dotDelay * 3, tmp) == dotDelay * 3) {
      Serial.print("-");
      string.concat('-');
    }
  }

  if (approximateValueQuad(dotDelay, dotDelay * 4, dotDelay * 5, dotDelay * 6, 20 * countNaught) == dotDelay * 6 && string.length() > 0) {
    for (int j = 0; j < 95; j++) {
      String str(asci[j]);
      if (string.equals(str) == 1) {
        char c = '!' + j;
        Serial.print("converted morse code -> ");  Serial.println(c); lcd.print(c);
        stringSum.concat(c);
        break;
      }
    }
    string.remove(0);
    Serial.println("converted sentence ->");  Serial.print(stringSum);
    delay(5000);
    lcd.clear();
    stringSum.remove(0);
  }

  if (sensorValue < referenceValue) {
    tmp = 20 * countNaught;
    countNaught = 0;
    countElectric++;
  } else if (sensorValue > referenceValue) {
    tmp = 20 * countElectric;
    countElectric = 0;
    countNaught++;
  }
  delay(20);
}
