#ifndef ultrasonic_h
#define ultrasonic_h
void setupUltrasonicPins();
int getCm(int trigPin, int echoPin);
int scanForward();
int scanLeft();
int scanRight();
#endif