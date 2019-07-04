#include "_00_drone.h"

// 4. 함수 정의
#include <Wire.h>
void init(mpu6050_t& mpu6050) {
  
  Wire.begin();
  Wire.beginTransmission(mpu6050.i2c_addr);
  Wire.write(mpu6050.PWR_MGMT_1);
  Wire.write(0);
  Wire.endTransmission(true);
}//1

void read(mpu6050_t& mpu6050, gyro_raw_t& gyro_raw) {
  
  Wire.beginTransmission(mpu6050.i2c_addr);
  Wire.write(mpu6050.GYRO_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(mpu6050.i2c_addr, 6, true);
  gyro_raw.x=Wire.read()<<8|Wire.read();
  gyro_raw.y=Wire.read()<<8|Wire.read();  
  gyro_raw.z=Wire.read()<<8|Wire.read();
}//1

#define NSAMPLES 1000//2  
void get(mpu6050_t& mpu6050, gyro_offset_t& gyro_offset) {
  
  gyro_raw_t gyro_raw = {0,};
  int32_t sumGyX = 0, sumGyY = 0, sumGyZ = 0;
  
  for(int i=0;i<NSAMPLES;i++) {
    read(mpu6050, gyro_raw);
    
    sumGyX += gyro_raw.x;  
    sumGyY += gyro_raw.y;  
    sumGyZ += gyro_raw.z;

    delay(1);
  }

  gyro_offset.x = (double)sumGyX/NSAMPLES;  
  gyro_offset.y = (double)sumGyY/NSAMPLES;  
  gyro_offset.z = (double)sumGyZ/NSAMPLES;  
}//2

void calc(
  gyro_adj_t& gyro_adj, 
  gyro_raw_t& gyro_raw, 
  gyro_offset_t& gyro_offset) {
    
  gyro_adj.x = gyro_raw.x - gyro_offset.x;
  gyro_adj.y = gyro_raw.y - gyro_offset.y;
  gyro_adj.z = gyro_raw.z - gyro_offset.z;  
}//3

void calc(gyro_rate_t& gyro_rate, gyro_adj_t& gyro_adj) {

  gyro_rate.pitch = gyro_adj.x/131.0;
  gyro_rate.roll  = gyro_adj.y/131.0;  
  gyro_rate.yaw   = gyro_adj.z/131.0;
}//4

void init(dt_t& dt) {
  
 dt.t_prev = micros();
}//5  
void calc(dt_t& dt) {
  
  dt.t_now = micros();
  dt.t_period = (dt.t_now - dt.t_prev)/1000000.0;
  dt.t_prev = dt.t_now;
}//5

void calc(gyro_angle_t& gyro_angle, gyro_rate_t& gyro_rate, dt_t& dt) { 

  gyro_angle.pitch += gyro_rate.pitch * dt.t_period;
  gyro_angle.roll  += gyro_rate.roll  * dt.t_period;
  gyro_angle.yaw   += gyro_rate.yaw   * dt.t_period;

    extern throttle_t throttle;//9
  if(throttle.value==0) {
      gyro_angle.pitch = 0;
      gyro_angle.roll = 0;
      gyro_angle.yaw = 0;
  }//9
}//6
