typedef struct IMU_data{

    float IMU_accx;
    float IMU_accy;
    float IMU_accz;
    float IMU_gyrox;
    float IMU_gyroy;
    float IMU_gyroz;
    float IMU_roll;
    float IMU_pitch;
    float dt;

    float accx_init;
    float accy_init;
    float accz_init;
    float gyrox_init;
    float gyroy_init;
    float gyroz_init;
    float roll_init;
    float pitch_init;

} IMU_data;


typedef struct ERR{
    float LtD;
    float HdE;
    int ERR_receive_timer;
} ERR;
