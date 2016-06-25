//refs : http://arduino.stackexchange.com/questions/176/how-do-i-print-multiple-variables-in-a-string
//refs : http://arduino.cc/en/Serial/Print

void setup() {
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  int  I  = 2;
  long L  = 123456789;
  float F  = 2.3;
  char  C  = 'b';
  char *S  = "test str";
  int N = 100;

  //ardprt function example
//  Serial.ardprt("%d,\t %l,\t %f,\t %c,\t %s", I, L, F, C, S);
  Serial.ardprt("Integer: %d", I);
  Serial.ardprt("Long Integer: %l", L);
  Serial.ardprt("Float: %f", F);
  Serial.ardprt("Character: %c", C);
  Serial.ardprt("Character string: %s", S);

  Serial.ardprt("Binary: %bb", N);
  Serial.ardprt("Octal: 0%o", N);
  Serial.ardprt("Decimal: %d", N);
  Serial.ardprt("Hex: 0x%x", N);
}

void loop() {
  // put your main code here, to run repeatedly:
}
