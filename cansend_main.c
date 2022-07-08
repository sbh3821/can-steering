#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "can.h"

int main(){
    /*
    #define ID_SLOPE 0x8CF02980
    #define ID_ACCELERATION 0x88F02D80
    #define ID_ANGULARRATE 0x8CF02A80
    #define ID_ACCELERATION_HR 0x88FF6D80
    #define ID_ANGULARRATE_HR 0x8CFF6B80 
    #define ID_LATERAL_DEVIATION 0x80000001
    #define ID_HEADING_ERR 0x80000002

    */
    int sock = CAN_open("can0");

    if (sock<0){
        printf("init failed");
    }

    u_int8_t can_data_1[CAN_FRAME_MAX_LEN] = {0xFF,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0x07};
    u_int8_t can_data_2[CAN_FRAME_MAX_LEN] = {0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0xFF};
    while(1){
        for(int i=0;i<10;i++){
            CAN_transmit(sock,0x111,can_data_1);
            delay(100);
        }
        for(int j=0;j<10;j++){
            CAN_transmit(sock,0x111,can_data_2);
            delay(100);
        }
    
    }
return 0;
}