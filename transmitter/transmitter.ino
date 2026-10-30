// Transmitter - serial text in, laser pulses out.
// Type into the Serial Monitor at 9600 baud; each printable character is looked up in
// asci[] and flashed as dots and dashes. One dot = dotDelay ms, one dash = 3 dots.

int ledPin = 12;

char* asci[] = {"..-..",".-.-","*","*","*","*",".-----.","-.--.","-.--.-","*",
                    ".-.-.","--..--","-....-",".-.-.-","-..-.","-----",".----","..---",
                    "...--","....-",".....","-....","--...","---..","----.","---...","-.-.-.",
                    "*","-...-","*","..--..","*",".-", "-...", "-.-.", "-..", ".", "..-.", "--.", 
                    "....", "..",".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.","...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..","*","*","*","*","..--.-","*",".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.","...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..","*","*","*","*","*"
                  };

int dotDelay = 40;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  Serial.begin(9600);
  Serial.println("mostranslater");
}

void loop() {
  char ch;
  if (Serial.available()) {
    ch = Serial.read();
    Serial.print(ch);
    Serial.println(" -> Convert morse code");
    if (ch >= '!' && ch <= '~') {
      flashSequence(asci[ch - '!']);
      Serial.println(asci[ch - '!']);
    }
    else if (ch <= ' ') {   // whitespace
      delay(dotDelay * 1);
    }
  }
}

void flashSequence(char* sequence) {
  int i = 0;
  while (sequence[i] != NULL) {
    flashDotOrDash(sequence[i]);
    i++;
  }
  delay(dotDelay * 3);
}

void flashDotOrDash(char dotOrDash) {
  digitalWrite(ledPin, HIGH);
  if (dotOrDash == '.')
    delay(dotDelay);
  else
    delay(dotDelay * 3);
  digitalWrite(ledPin, LOW);
  delay(dotDelay);
}
