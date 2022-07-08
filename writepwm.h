#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BCM12_PWM0 12 
#define NumOfSampleF 50
#define NumOfSampleD 3

double period;
struct timeval pwmStart, pwmEnd, freqStart, freqEnd;

double time_diff(struct timeval x, struct timeval y);

//initiallizing pwm setting
void setpwm();

//Write PWM
void writepwm(double duty,double period,double duration);

//test PWM
//you can adjust Number of Sample by adjusting NumOfSam above
void testpwm(double startfreq, double Endfreq, double Startduty, double Endduty);