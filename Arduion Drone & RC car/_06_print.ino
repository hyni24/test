#include "_00_drone.h"

// 4. 함수 정의
void print(gyro_raw_t& gyro_raw) {

  Serial.print("#RAW:GyX = "); 
  Serial.print(gyro_raw.x);
  Serial.print(" | GyY = "); 
  Serial.print(gyro_raw.y);
  Serial.print(" | GyZ = "); 
  Serial.print(gyro_raw.z);
}//1

void println(void) {
  
  Serial.println();
}//1

void print(gyro_offset_t& gyro_offset) {

  Serial.print("#OFF:GyX = "); 
  Serial.print(gyro_offset.x);
  Serial.print(" | GyY = "); 
  Serial.print(gyro_offset.y);
  Serial.print(" | GyZ = "); 
  Serial.print(gyro_offset.z);  
}//2

void print(gyro_adj_t& gyro_adj) {
  
  Serial.print("#ADJ:GyX = "); 
  Serial.print(gyro_adj.x);
  Serial.print(" | GyY = "); 
  Serial.print(gyro_adj.y);
  Serial.print(" | GyZ = "); 
  Serial.print(gyro_adj.z);  
}//3

void print(gyro_rate_t& gyro_rate) {
  
  Serial.print("#RATE:"); 
  Serial.print(gyro_rate.roll);
  Serial.print(","); 
  Serial.print(gyro_rate.pitch);
  Serial.print(","); 
  Serial.print(gyro_rate.yaw);
}//4

void print(dt_t& dt) {
    
  Serial.print("DEL:");
  Serial.print(dt.t_period, DEC);
}//5

void print(gyro_angle_t& gyro_angle) {
  
  Serial.print("#ANGLE:"); 
  Serial.print(gyro_angle.roll);
  Serial.print(","); 
  Serial.print(gyro_angle.pitch);
  Serial.print(","); 
  Serial.print(gyro_angle.yaw);
}//6

void print(balancing_force_t& balancing_force) {
  
  Serial.print("#FORCE:"); 
  Serial.print(balancing_force.roll);
  Serial.print(","); 
  Serial.print(balancing_force.pitch);
  Serial.print(","); 
  Serial.print(balancing_force.yaw);
}//7

void print(motor_speed_t& motor_speed) {
  
  Serial.print("#A:");
  Serial.print(motor_speed.a, 2);
  Serial.print("#B:");
  Serial.print(motor_speed.b, 2);
  Serial.print("#C:");
  Serial.print(motor_speed.c, 2);
  Serial.print("#D:");
  Serial.print(motor_speed.d, 2);
}//8
