#ifndef __00_DRONE_H__
#define __00_DRONE_H__

// 3. 변수형 정의
typedef struct { int i2c_addr, PWR_MGMT_1, GYRO_XOUT_H; } mpu6050_t;//1
typedef struct { int16_t x, y, z; } gyro_raw_t;//1
typedef struct { int16_t x, y, z; } gyro_offset_t;//2
typedef struct { int16_t x, y, z; } gyro_adj_t;//3 
typedef struct { double pitch, roll, yaw; } gyro_rate_t;//4
typedef struct { uint32_t t_now, t_prev; double t_period; } dt_t;//5
typedef struct { double pitch, roll, yaw; } gyro_angle_t;//6
typedef struct { double pitch, roll, yaw; } target_angle_t;//7
typedef struct { double pitch, roll, yaw; } balancing_force_t;//7
typedef struct { double value; } throttle_t;//8
typedef struct { double a, b, c, d; } motor_speed_t;//8
typedef struct { int dummy; } remote_t;
typedef struct { int a,b,c,d; } motor_t;

#endif // __00_DRONE_H__
