#include "_00_drone.h"

// 4. 함수 정의
void init(motor_t& motor) {

  analogWrite(motor.a, 0);
  analogWrite(motor.b, 0);
  analogWrite(motor.c, 0);
  analogWrite(motor.d, 0);
}//10
