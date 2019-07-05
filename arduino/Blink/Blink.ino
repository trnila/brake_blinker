#include <Wire.h>

const int DEMO_BLINK_MS = 5000;

const int MPU_addr=0x68;  // I2C address of the MPU-6050
int AcX, AcY, AcZ, GyX, GyY, GyZ, Tmp;
const int LED_PIN = 5;

unsigned int iteration = 0;


void mpu_set(int reg, int val) {
  Wire.beginTransmission(MPU_addr);
  Wire.write(reg);
  Wire.write(val);
  Wire.endTransmission(true);
}

void init_accelerometer() {
	mpu_set(0x6B, 0); // PWR_MGMT_1 register
//	mpu_set(0x1A, 0x06);


}

void setup(){
  Serial.begin(115200);
  Serial.println("HELLO");
  
  pinMode(LED_PIN, OUTPUT);

  Wire.begin();
  Wire.setClock(400000);

  // wait until accelerometer ready
  delay(100);

  init_accelerometer();

/*  for(int i = 0; i < DEMO_BLINK_MS / 200; i++) {
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
  }
  digitalWrite(LED_PIN, LOW);
  */
}
void loop() {  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  /*
  AcX = AcX/16384.0*10;
  AcY = AcY/16384.0*10;
  AcZ = AcZ/16384.0*10;
  */

  // try to reinitialize accelerometer
  if(Tmp == 0 && AcX == 0 && AcZ == 0) {
    init_accelerometer();
    Serial.print("crashed at");
    Serial.println(micros());
  }

  Serial.print(micros()); Serial.print(',');
  Serial.print(AcX); Serial.print(',');
  Serial.print(AcY); Serial.print(',');
  Serial.print(AcZ); Serial.print(',');
  Serial.print(Tmp); Serial.print(',');
  Serial.print(GyX); Serial.print(',');
  Serial.print(GyY); Serial.print(',');
  Serial.print(GyZ);
  Serial.println();


  if(Serial.available() && Serial.read() == '1') {
      digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }



  iteration++;
  delay(1);

}
