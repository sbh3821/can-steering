#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "writepwm.h"

struct timeval pwmStart, pwmEnd, freqStart, freqEnd;
double freq;


double time_diff(struct timeval x, struct timeval y){
    double x_ms, y_ms, diff;
	x_ms = (double)x.tv_sec +(double) x.tv_usec/1000000;
	y_ms = (double)y.tv_sec + (double)y.tv_usec/1000000;
	diff = (double)y_ms - (double)x_ms;
	return diff;
}

//initiallizing pwm setting
void setpwm(){
    wiringPiSetupGpio();    // BCM핀 번호를 사용하겠다고 선언
	pwmSetMode(PWM_MODE_MS);    // PWM을 마크-스페이스 모드로 사용 선언
	pinMode(BCM12_PWM0, PWM_OUTPUT);  //12번 핀을 하드웨어 PWM 모드로 설정
    pwmSetRange(2);
    pinMode(13,PWM_OUTPUT);
    pwmSetRange(2);
}

//Write PWM
void writepwm(double duty,double period,double duration){
    gettimeofday(&freqStart,NULL);
    while(1){
        if(time_diff(freqStart,freqEnd)<duration){
		    gettimeofday(&pwmStart,NULL); // get time when period starts
		
            while(time_diff(pwmStart,pwmEnd)<period){
                if(time_diff(pwmStart,pwmEnd)<period*duty)
                    pwmWrite(BCM12_PWM0,2);
			    else
                    pwmWrite(BCM12_PWM0,0);

                gettimeofday(&pwmEnd,NULL);
       	    	}	
        }
        else {
            break;
            }
        gettimeofday(&freqEnd,NULL);
        }
};

void testpwm(double startfreq, double Endfreq, double Startduty, double Endduty){
    double freq, period, periodE, periodS, duty;
	periodS = (double)1/startfreq;
	periodE = (double)1/Endfreq;
    int i,j;
    for (i=0;i<=NumOfSampleD;i++){
        duty = Startduty + (Endduty - Startduty)/NumOfSampleD*i;
        for(j=0;j<=NumOfSampleF;j++){
		    period = periodS - (periodS-periodE)/NumOfSampleF*j;
		    freq = (double)1/period;
		    printf("duty : %.1f, period : %1.fus, frequncy : %.1f\n",duty,1000000*period,(double)1/period);
		    writepwm(duty,period,0.05);
		}
        printf("\n\n");
    }
};
