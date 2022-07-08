#include <stdlib.h>


int main(){
    system("sudo ip link set can0 up type can bitrate 250000");
    system("sudo ip link set can1 up type can bitrate 250000");
    system("sudo ifconfig can0 txqueuelen 65536");
    system("sudo ifconfig can1 txqueuelen 65536");
    return 0;
}