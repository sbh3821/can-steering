#include "can.h"

int SF;
int w0,w1;
double HdEMax, LtDMax;
int CAN_open(const char* ifname) {
    int fd;
    struct sockaddr_can addr;
    struct ifreq ifr;
    
    memset(&addr, 0, sizeof(addr));
    memset(&ifr, 0, sizeof(ifr));

    if ((fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        return ERROR_SOCKET;
    }

    strcpy(ifr.ifr_name, ifname);

    if (ioctl(fd, SIOCGIFINDEX, &ifr) < 0) {
        return ERROR_INTERFACE;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    fcntl(fd, F_SETFL, O_NONBLOCK);

    if (bind(fd, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
        return ERROR_BIND;
    }
    
    return fd;
}

int CAN_transmit(int fd, u_int32_t id, u_int8_t* data) {
    struct can_frame frame;
    size_t data_len;
    data_len = sizeof(data);
    frame.can_id = id;
    frame.can_dlc = data_len;

    for (int i = 0; i < data_len; i++) {
        frame.data[i] = data[i];
    }

    if (write(fd, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
        return ERROR_WRITE;
    }
    
    return 0;
}

int CAN_receive_IMU(int fd, struct IMU_data* IMU_data) {
    struct can_frame frame;
    ssize_t nBytes = read(fd, &frame, sizeof(frame));

    if (nBytes < 0  || nBytes < (ssize_t) sizeof(struct can_frame)){//|| frame.can_dlc > CAN_FRAME_MAX_LEN) {
        return ERROR_READ;
    }


    switch (frame.can_id) {
        case ID_SLOPE : // Slope
            parse_slope(frame.data, IMU_data);
            id_counter_IMU[0] = GET_SUCCESS;
            break;
        case ID_ACCELERATION : // Acceleration
            parse_accel(frame.data, IMU_data);
            id_counter_IMU[1] = GET_SUCCESS;
            break;
        case ID_ANGULARRATE : // Angular Rate
            parse_angularRate(frame.data, IMU_data);
            id_counter_IMU[2] = GET_SUCCESS;
            break;
        case ID_ACCELERATION_HR : // High resolution Acceleration
            parse_accel_HR(frame.data, IMU_data);
            id_counter_IMU[1] = GET_SUCCESS;
            break;
        case ID_ANGULARRATE_HR : // HIGH Resolution Angular Rate
            parse_angularRate_HR(frame.data, IMU_data);
            id_counter_IMU[2] = GET_SUCCESS;
            break;
    }
        if ((id_counter_IMU[0] == GET_SUCCESS) && (id_counter_IMU[1] == GET_SUCCESS) && (id_counter_IMU[2] == GET_SUCCESS)) {
        id_counter_IMU[0] = GET_FAIL;
        id_counter_IMU[1] = GET_FAIL;
        id_counter_IMU[2] = GET_FAIL;
        return CHECK_TIME;
        }

    return 0;
    
}


int CAN_receive_ERROR(int fd, struct ERR* ERR){
    struct can_frame frame;
    ssize_t nBytes = read(fd, &frame, sizeof(frame));
    /*if (nBytes < 0 || nBytes < (ssize_t) sizeof(struct can_frame) || frame.can_dlc > CAN_FRAME_MAX_LEN) {
        return ERROR_READ;
    }*/
    if(frame.can_id==ID_FULL_ERR){
        parse_ERR(frame.data,ERR);
        id_counter_ERR[0] = GET_SUCCESS;
        id_counter_ERR[1] = GET_SUCCESS;printf("%f\n",ERR->LtD);
        }
    if ((id_counter_ERR[0] == GET_SUCCESS) && (id_counter_ERR[1] == GET_SUCCESS)) {
        id_counter_ERR[0] = GET_FAIL;
        id_counter_ERR[1] = GET_FAIL;
        (*ERR).ERR_receive_timer=1;
        return CHECK_TIME;
        }
    
}

int CAN_close(int fd){
}

int CAN_RECEIVE_AND_STEERING(int fd, ERR* err){
    struct timeval receiveStart, receiveEnd;  
    double steeringValue;  

    gettimeofday(&receiveEnd,NULL);
    while(1){
            CAN_receive_ERROR(fd,err);
        while(time_diff(receiveStart,receiveEnd)>(float)(1/SF)){
            if ((*err).ERR_receive_timer==1){
                steeringValue=w0*err->LtD+w1*err->HdE;
                printf("ltd : %f,HdE : %f, ERR : %f\n",err->LtD,err->HdE,steeringValue);
                steering(-steeringValue);         
                (*err).ERR_receive_timer=0;
            }
        gettimeofday(&receiveStart,NULL);
        }
        gettimeofday(&receiveEnd,NULL);
    }
}
void setCanReceiveFrequency(int sf){
    SF=sf;
}
void setwvalue(int W0,int W1){
    w0=W0;
    w1=W1;
}
void parse_slope(const u_int8_t* data, struct IMU_data* IMU_data){
    //printf("get slope information\n");
}
void parse_angularRate_HR(const u_int8_t* data, struct IMU_data* IMU_data){
    //printf("get angular information_HR\n");
}
void parse_accel_HR(const u_int8_t* data, struct IMU_data* IMU_data){
    //printf("get acceleration data\n");
}
void parse_angularRate(const u_int8_t* data, struct IMU_data* IMU_data){
    //printf("get angular information\n");
}
void parse_accel(const u_int8_t* data, struct IMU_data* IMU_data){
    //printf("get acceleration data\n");
}


void parse_ERR(const u_int8_t* data, struct ERR* err){
    float LtD_temp, HdE_temp;
    LtD_temp = (float)(((data[0]<<24)+(data[1]<<16)+(data[2]<<8)+(data[3])))/1000000;
    HdE_temp = (float)(((data[4]<<24)+(data[5]<<16)+(data[6]<<8)+(data[7])))/1000000;

    if(LtD_temp>LtDMax){
        LtD_temp=LtDMax;
    }else if(LtD_temp<-LtDMax){
        LtD_temp=(-LtDMax);
    }

    if(HdE_temp>HdEMax){
        HdE_temp = HdEMax;
    }else if(HdE_temp<-HdEMax){
        HdE_temp = (-HdEMax);
    }

    (*err).LtD = LtD_temp;
    (*err).HdE = HdE_temp;
}

void setERRMax(double LtD_max, double HdE_max){
    LtDMax = LtD_max;
    HdEMax = HdE_max;
}
