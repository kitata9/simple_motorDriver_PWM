/*
 * 2019/01/05 kitata9
 * simple_motorDriver_PWM
 */

 
/* PIN settings */
#define M1_IN1 11
#define M1_IN2 2
#define M1_PWM 6
#define M2_IN1 9
#define M2_IN2 4
#define M2_PWM 10
#define CH1_PWM 3
#define CH2_PWM 5

/* PWM settings */
/* CH1 is forward-backward PWM channel.
 * CH2 is left-right PWM channel.
 */
#define CH1_PWM_N 1520
#define CH1_PWM_RANGE 400
#define CH2_PWM_N 1520
#define CH2_PWM_RANGE 400
int ch1_val = CH1_PWM_N;
int ch2_val = CH2_PWM_N;
float ch1_norm_val = 0;
float ch2_norm_val = 0;

float ZERO_THRESHOULD = 0.03;

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max){
 return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

/* Check if num is under threshold */
bool checkZero(float num){
  if(num == 0.0){
    return true;
  }else if(num > 0.0){
    if(num < ZERO_THRESHOULD){
      return true;
    }else{
      return false;
    }
  }else{
    if(num > -ZERO_THRESHOULD){
      return true;
    }else{
      return false;
    }
  }
}

/*Control motors*/
void runMotors(float in_ch1, float in_ch2){
  if(!checkZero(in_ch1) || !checkZero(in_ch2)){
    if (in_ch1 > 0 && checkZero(in_ch2)) {
      digitalWrite(M2_IN2, HIGH);
      digitalWrite(M2_IN1, LOW);
      digitalWrite(M1_IN2, HIGH);
      digitalWrite(M1_IN1, LOW);
      int velocity = in_ch1*255;
      if(velocity > 255){
        velocity = 255;
      }
      Serial.print(velocity);
      analogWrite(M1_PWM, velocity);
      analogWrite(M2_PWM, velocity);
      Serial.println(":-----Forward-----");
    }
    else if (in_ch1 < 0 && checkZero(in_ch2)) {
      digitalWrite(M2_IN2, LOW);
      digitalWrite(M2_IN1, HIGH);
      digitalWrite(M1_IN2, LOW);
      digitalWrite(M1_IN1, HIGH);
      int velocity = in_ch1*(-255);
      if(velocity > 255){
        velocity = 255;
      }
      Serial.print(velocity);
      analogWrite(M1_PWM, velocity);
      analogWrite(M2_PWM, velocity);
      Serial.println(":-----Back-----");
    }

    if (in_ch2 > 0 && checkZero(in_ch1)) {
      digitalWrite(M2_IN2, LOW);
      digitalWrite(M2_IN1, HIGH);
      digitalWrite(M1_IN2, HIGH);
      digitalWrite(M1_IN1, LOW);
      int velocity = in_ch2*255;
      if(velocity > 255){
        velocity = 255;
      }
      Serial.print(velocity);
      analogWrite(M1_PWM, velocity);
      analogWrite(M2_PWM, velocity);
      Serial.println(":-----Left turn-----");
    }
    else if (in_ch2 < 0 && checkZero(in_ch1)) {
      digitalWrite(M2_IN2, HIGH);
      digitalWrite(M2_IN1, LOW);
      digitalWrite(M1_IN2, LOW);
      digitalWrite(M1_IN1, HIGH);
      int velocity = in_ch2*(-255);
      if(velocity > 255){
        velocity = 255;
      }
      Serial.print(velocity);
      analogWrite(M1_PWM, velocity);
      analogWrite(M2_PWM, velocity);
      Serial.println(":-----Right turn-----");
    }
    
    if (in_ch1 > 0 && !checkZero(in_ch2)) {
      digitalWrite(M2_IN2, HIGH);
      digitalWrite(M2_IN1, LOW);
      digitalWrite(M1_IN2, HIGH);
      digitalWrite(M1_IN1, LOW);
      int velocity = in_ch1*255;
      int velocity_z = in_ch2*255;

      int right_velocity = velocity - velocity_z;
      int left_velocity = velocity + velocity_z;
      
      if(right_velocity > 255){
        right_velocity = 255;
      }else if(right_velocity < 0){
        right_velocity = 0;
      }
      if(left_velocity > 255){
        left_velocity = 255;
      }else if(left_velocity < 0){
        left_velocity = 0;
      }
      analogWrite(M1_PWM, right_velocity);
      analogWrite(M2_PWM, left_velocity);
    }
    else if (in_ch1 < 0 && !checkZero(in_ch2)) {
      digitalWrite(M2_IN2, LOW);
      digitalWrite(M2_IN1, HIGH);
      digitalWrite(M1_IN2, LOW);
      digitalWrite(M1_IN1, HIGH);
      int velocity = in_ch1*(-255);
      int velocity_z = in_ch2*255;

      int right_velocity = velocity + velocity_z;
      int left_velocity = velocity - velocity_z;
      
      if(right_velocity > 255){
        right_velocity = 255;
      }else if(right_velocity < 0){
        right_velocity = 0;
      }
      if(left_velocity > 255){
        left_velocity = 255;
      }else if(left_velocity < 0){
        left_velocity = 0;
      }
      analogWrite(M1_PWM, right_velocity);
      analogWrite(M2_PWM, left_velocity);
    }
    
  } 
  else {
    digitalWrite(M2_IN2, LOW);
    digitalWrite(M2_IN1, LOW);
    digitalWrite(M1_IN2, LOW);
    digitalWrite(M1_IN1, LOW);
    analogWrite(M1_PWM,0);
    analogWrite(M2_PWM,0);
  }
  if(checkZero(in_ch1) && checkZero(in_ch2)){
    digitalWrite(M2_IN2, LOW);
    digitalWrite(M2_IN1, LOW);
    digitalWrite(M1_IN2, LOW);
    digitalWrite(M1_IN1, LOW);
    analogWrite(M1_PWM,0);
    analogWrite(M2_PWM,0);
  }
}

/*Normalize PWM input between -1.0 and 1.0*/
float normPWM(float in_val, float in_c, float in_range){
  if(in_val > in_c+in_range){
    return 1.0;
  }else if(in_val < in_c-in_range){
    return -1.0;
  }
  return mapfloat(in_val, in_c-in_range, in_c+in_range, -1.0, 1.0);
}

void setup()
{
  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT);
  pinMode(M2_IN2, OUTPUT);
  pinMode(CH1_PWM, INPUT);
  pinMode(CH2_PWM, INPUT);
  Serial.begin(57600);
  Serial.println("Start!!!");
}

void loop()
{
  ch1_val = pulseIn(CH1_PWM, HIGH);
  ch2_val = pulseIn(CH2_PWM, HIGH);
  ch1_norm_val = normPWM((float)ch1_val, CH1_PWM_N, CH1_PWM_RANGE);
  ch2_norm_val = normPWM((float)ch2_val, CH1_PWM_N, CH2_PWM_RANGE);
  Serial.print(ch1_val);
  Serial.print(":");
  Serial.print(ch1_norm_val,4);
  Serial.print(" ");
  Serial.print(ch2_val);
  Serial.print(":");
  Serial.println(ch2_norm_val,4);
  runMotors(ch1_norm_val, ch2_norm_val);
}


