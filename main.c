#include <stdio.h>
#include <phidget22.h>

#include "can.h"
#include "steering.h" 
#include "writepwm.h"
#include "pid.h"

int main(void) {
    // CAN data variable
    ERR* err = (struct ERR *) malloc(sizeof(struct ERR));
    int can_receive;

    initSteering();     //setSamplingFrequency(SF); 
                        //setKvalue(0.005,0.001,0.0001);
    setwvalue(200,50);
    setCanReceiveFrequency(10);
    setERRMax(5,3);

    //CAN setup
    can_receive = CAN_open("can1");
    printf("i am ready\n");

    CAN_RECEIVE_AND_STEERING(can_receive,err);
    
    return 0;
}
