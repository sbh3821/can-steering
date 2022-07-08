#include <stdio.h>
#include <stdlib.h>

double error;
double output;


//this function initiate Integral of error
void initPid();

//Literally, set K values for PID
void setKvalue(double kp,double ki,double kd);

//this function return feedback
double pid_cal(double reference, double error_in);

//should be reform, because moter change
double pidValueToPWMValue(double pidValue);

