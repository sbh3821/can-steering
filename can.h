#ifndef NEW_CAN_H
#define NEW_CAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include "struct.h"
#include "steering.h"
#include "writepwm.h"
#include "pid.h"

#define CAN_FRAME_MAX_LEN 8
#define CHECK_TIME 7

#define GET_SUCCESS 9
#define GET_FAIL 10

#define ID_SLOPE 0x8CF02980
#define ID_ACCELERATION 0x88F02D80
#define ID_ANGULARRATE 0x8CF02A80
#define ID_ACCELERATION_HR 0x88FF6D80
#define ID_ANGULARRATE_HR 0x8CFF6B80 
#define ID_LATERAL_DEVIATION 0x80000001
#define ID_HEADING_ERR 0x80000002
#define ID_FULL_ERR 0x111

// Error
#define ERROR_SOCKET (-1)
#define ERROR_INTERFACE (-2)
#define ERROR_BIND (-3)
#define ERROR_WRITE (-4)
#define ERROR_READ (-5)
#define ERROR_CLOSE (-6)



// fundamental functions
int CAN_open(const char* ifname);
int CAN_transmit(int fd, u_int32_t id, u_int8_t* data);
int CAN_receive_IMU(int fd, struct IMU_data* IMU_data);
int CAN_receive_ERROR(int fd,struct ERR* ERR);
int CAN_RECEIVE_AND_STEERING(int can_receive, ERR* err);
int CAN_close(int fd);

// data parsing
void parse_slope(const u_int8_t* data, struct IMU_data* IMU_data);
void parse_angularRate_HR(const u_int8_t* data, struct IMU_data* IMU_data);
void parse_accel_HR(const u_int8_t* data, struct IMU_data* IMU_data);
void parse_angularRate(const u_int8_t* data, struct IMU_data* IMU_data);
void parse_accel(const u_int8_t* data, struct IMU_data* IMU_data);
void parse_ERR(const u_int8_t* data, struct ERR* err);
void setCanReceiveFrequency(int sf);
void setwvalue(int W0,int W1);
void setERRMax(double LtD_max, double HdE_max);

int id_counter_IMU[3];
int id_counter_ERR[2];
#endif
