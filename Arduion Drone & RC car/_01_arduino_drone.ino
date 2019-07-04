#include "_00_drone.h"

mpu6050_t mpu6050 = {
  .i2c_addr = 0x68,
  .PWR_MGMT_1 = 0x6b,
  .GYRO_XOUT_H = 0x43,
};//1

// 2.변수 선언
gyro_raw_t gyro_raw;//1
gyro_offset_t gyro_offset;//2
gyro_adj_t gyro_adj;//3
gyro_rate_t gyro_rate;//4
dt_t dt;//5
gyro_angle_t gyro_angle;//6
target_angle_t target_angle = { .pitch = 0.0, .roll = 0.0, .yaw = 0.0, };//7
balancing_force_t balancing_force;//7
throttle_t throttle = { .value = 0.0, };//8,9 => balancing.distribute
motor_speed_t motor_speed;//8
remote_t remote;//9
motor_t motor = { .a = 6, .b = 10, .c = 9, .d = 5, };//10

// 1. 전체 문맥 구성
void setup() {
  Serial.begin(115200);//1    
  init(mpu6050);//1    
  get(mpu6050, gyro_offset);//2        
  init(dt);//5 
  init(remote);//9
  init(motor);//10
}
char trans = 'd'; //d 면 드론모드 r 이면 rc카 모드
int rcA = 0, rcB = 0, rcC = 0, rcD = 0;
char rcTrans = 'f';
void loop() {  
  read(mpu6050, gyro_raw);//1        
  calc(gyro_adj, gyro_raw, gyro_offset);//3        
  calc(gyro_rate, gyro_adj);//4                
  calc(dt);//5       
  calc(gyro_angle, gyro_rate, dt);//6
  calc(balancing_force, target_angle, gyro_angle);//7
  add(balancing_force, gyro_rate);//11
  add(balancing_force, target_angle, gyro_angle, dt);//12
  distribute(motor_speed, throttle, balancing_force);//8 

  
  //check(remote, throttle, target_angle);//9
  if(trans == 'd'){
    static uint8_t cnt_msg;
    if(Serial1.available() > 0) {
      while(Serial1.available() > 0) {
        uint8_t msp_data = Serial1.read(); 
        if(msp_data == '$') cnt_msg = 0;
        else cnt_msg++;   

        if(cnt_msg == 4 && msp_data == 151){
          trans = 'r';
          break;  
        }
        else if(cnt_msg == 8) throttle.value = msp_data;
        else if(cnt_msg == 7) target_angle.yaw = -(msp_data-125);//12
        else if(cnt_msg == 6) target_angle.pitch = -(msp_data-125);//12
        else if(cnt_msg == 5) target_angle.roll = (msp_data-125);//12
  
        #define RANGE   30 //각도 최대
        if(target_angle.roll < -RANGE) target_angle.roll = -RANGE;
        if(target_angle.roll > RANGE) target_angle.roll = RANGE;
        if(target_angle.pitch < -RANGE) target_angle.pitch = -RANGE;
        if(target_angle.pitch > RANGE) target_angle.pitch = RANGE;
        if(target_angle.yaw < -RANGE) target_angle.yaw = -RANGE;
        if(target_angle.yaw > RANGE) target_angle.yaw = RANGE; 
      }
    }
    analogWrite(motor.a, motor_speed.a); //a가 왼쪽 위에
    analogWrite(motor.c, motor_speed.c); //c가 오른쪽 아래
    analogWrite(motor.b, motor_speed.b); //b가 오른쪽 위에
    analogWrite(motor.d, motor_speed.d); //d가 왼쪽 아래
  }
  else if(trans == 'r'){
      static uint8_t cnt_msg;
      if(Serial1.available() > 0){
        while(Serial1.available() > 0){
          uint8_t msp_data = Serial1.read();
          if(msp_data == '$') cnt_msg = 0;
          else cnt_msg++;

          if(rcTrans=='f'){
              if(cnt_msg == 4 && msp_data == 152){
              trans = 'd';
              analogWrite(motor.a,  0);
              analogWrite(motor.b,  0);
              analogWrite(motor.c,  0);
              analogWrite(motor.d,  0);
              break;  
              }
              else if(cnt_msg == 6){
                if(msp_data > 130){
                    rcB = 250;
                    rcD = 250;
                    rcA = rcC = 0;
              //앞으로가기
                }
                else if(msp_data == 125){
                    rcA = rcC = rcB = rcD = 0;
                }
                else if(msp_data < 100){
                  rcTrans = 'L';  
                }
              }
          }
          else if(rcTrans=='L'){
            if(cnt_msg == 4 && msp_data == 152){
            trans = 'd';
            analogWrite(motor.a,  0);
            analogWrite(motor.b,  0);
            analogWrite(motor.c,  0);
            analogWrite(motor.d,  0);
            break;  
            }
            else if(cnt_msg == 5){
                if(msp_data > 125){ //오른쪽바퀴
                rcA = rcC = rcD = 0;
                rcB = (msp_data-125)*2; //roll이 7맞는지 확인 하고 오른쪽 왼쪽 바퀴 확인
                //analogWrite(motor.a,  rcA); //a랑 c랑 앞으로가는거     //b랑 d랑 뒤로가는거
                //analogWrite(motor.b,  rcB);
                //analogWrite(motor.c,  rcC);
                //analogWrite(motor.d,  rcD);
            }
            else if(msp_data < 125){ //왼쪽바퀴
              rcA = rcC = rcB = 0;
              rcD = (125 - msp_data)*2;
              //analogWrite(motor.a,  rcA); //a랑 c랑 앞으로가는거     //b랑 d랑 뒤로가는거
              //analogWrite(motor.b,  rcB);
              //analogWrite(motor.c,  rcC);
              //analogWrite(motor.d,  rcD);
            }
            else {
              rcA = rcC = rcB = rcD = 0;
              //analogWrite(motor.a,  rcA); //a랑 c랑 앞으로가는거     //b랑 d랑 뒤로가는거
              //analogWrite(motor.b,  rcB);
              //analogWrite(motor.c,  rcC);
              //analogWrite(motor.d,  rcD);
            }
            }
            else if(cnt_msg == 6){
                if(msp_data < 100){
                  rcTrans = 'f';  
                }
            }
          }
           analogWrite(motor.a,  rcA); //a랑 c랑 앞으로가는거     //b랑 d랑 뒤로가는거
           analogWrite(motor.b,  rcB);
           analogWrite(motor.c,  rcC);
           analogWrite(motor.d,  rcD);
        }  
      }

      
  }
}
