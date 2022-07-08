#include <stdio.h>
#include "pid.h"
#include "writepwm.h"
#include "steering.h"

double SamplingFrequency; //10hz

void setSamplingFrequency(double SF){
    SamplingFrequency = SF;
}

void initSteering(){
    setpwm();
    setKvalue(0.005,0.001,0.0001);
    setSamplingFrequency(10);
}

//steering for 1 samping period
double steering(double err){
    double steer_err=0;
    double pid_temp=0;
    int counterSteering = 0;
    double steeringduration = 0.001;
    double steeringduty = 0.5;
    steer_err=err;
    while(counterSteering<(1/SamplingFrequency/steeringduration)){
        pid_temp = pid_cal(0,steer_err);
        if(pid_temp>=0){
            pwmWrite(13,2);
            writepwm((double)pid_temp/100,0.0005,steeringduration);
            
        }else if(pid_temp<0){
            pwmWrite(13,0);
            writepwm((double)-pid_temp/100,0.0005,steeringduration);
        }
        counterSteering++;
        steer_err += pid_temp;
    }
    return pid_temp;
}
