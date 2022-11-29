#include "ultrasonicObject.h"
extern "C" {
#include "ultrasonic.h"
}

Ultrasonic::Ultrasonic(int trigPinToSet, int echoPinToSet)
{
    trigPin = trigPinToSet;
    echoPin = echoPinToSet;
    setupUltrasonicPins();
}

int Ultrasonic::getCM()
{
    return getCm(trigPin, echoPin);
}