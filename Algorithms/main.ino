#include <CurieTimerOne.h>

#include <BMI160.h>

#include <CurieIMU.h>

#include <CurieBLE.h>

 #include <MadgwickAHRS.h>

//defines

#define BLE_SIZE 4

#define TASK_RATE 25u

#define GYRO_RANGE 500u

#define ACC_RANGE 8u

#define TASK_PERIOD_US ((int)(1000000/TASK_RATE))
#define DELTA_T (1.0f/(float)TASK_RATE)

#define LED_ON() (digitalWrite(13,HIGH))

#define LED_OFF() (digitalWrite(13,LOW))
#define FILTER_COEF 0.3f
#define IMU_RAW_MAX 32768.0f
#define START_TH 10
#define STOP_TH 10
#define STOP_CNT_TH TASK_RATE
//types definitions

float posx,posy,posz,vx,vy,vz,alfa,beta,gama,pos2,dpos,pos2_old;
int rfilx, rfily, rfilz;
//globals

int state;
Madgwick filter;

volatile int read_flag;

int x;

//functions definitions

void CheckSignal(int16_t signal);

void IMU_SetUp(void);

void timer_callback(void);

int filterhp(int input, int old);

//function 

void timer_callback(void){

  read_flag = 1;

}

void IMU_SetUp(void)

{

  LED_ON();

  CurieIMU.begin();

  CurieIMU.setAccelerometerRange(ACC_RANGE);

  CurieIMU.setGyroRange(GYRO_RANGE);

  CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS, 0);

  CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS, 0);

  CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, 1); 

  CurieIMU.autoCalibrateGyroOffset();

   CurieIMU.setGyroRate(TASK_RATE);

  CurieIMU.setAccelerometerRate(TASK_RATE);

  filter.begin(TASK_RATE);

  CurieIMU.setAccelOffsetEnabled(true);

  LED_OFF();

}

void Tim_SetUp(void){

  CurieTimerOne.start(TASK_PERIOD_US,timer_callback);

}

  

void setup() {

  // put your setup code here, to run once:

  pinMode(13, OUTPUT);

  Serial.begin(115200);

  IMU_SetUp();

  Tim_SetUp();

}

void loop() {

  if(read_flag ==1)

  {

        int aix, aiy, aiz;

        int gix, giy, giz;
        int cnt;
        float ax, ay, az;

        float gx, gy, gz;
    LED_ON();
        

        CurieIMU.readMotionSensor(aix, aiy, aiz, gix, giy, giz);

  

        // convert from raw data to gravity and degrees/second units

        //should be changed to SG filter or Kalman filter
        rfilx = filterhp(aix,rfilx);
        rfily = filterhp(aiy,rfily);
        rfilz = filterhp(aiz,rfilz);                
        ax = convertRawAcceleration(aix - rfilx);

        ay = convertRawAcceleration(aiy-rfily);

        az = convertRawAcceleration(aiz-rfilz);

        gx = convertRawGyro(gix);

        gy = convertRawGyro(giy);

        gz = convertRawGyro(giz); 

        filter.updateIMU(gx, gy, gz, ax, ay, az);
        alfa = filter.getRoll();
        beta = filter.getPitch();
        gama = filter.getYaw()-180;
        //ax = ax - sin(alfa);
        //ay = ay - cos(beta)*sin(gama);
        //az = az - cos(beta)*cos(gama);
        posx = ax*DELTA_T*DELTA_T/2 + vx*DELTA_T;
        posy = ay*DELTA_T*DELTA_T/2 + vy*DELTA_T;
        posz = az*DELTA_T*DELTA_T/2 + vz*DELTA_T;
        vx += ax*DELTA_T;
        vy += ay*DELTA_T;
        vz += az*DELTA_T;
        pos2_old = pos2;
        pos2 = posx*posx+posy*posy+posz*posz;
        dpos = pos2-pos2_old;
        if((dpos > START_TH) & (state == 0))
        {
           state = 1;            
        }

        if(dpos < STOP_TH & (state == 1))
          cnt++;
          if(cnt> STOP_CNT_TH)
          {
            cnt = 0;
            state = 0;
            vx = 0;
            vy= 0;
            vz = 0;
          }
 //       if(state == 1)
        {
          Serial.print(posx);

          Serial.print((" "));

          Serial.print(posy);      

          Serial.print((" "));
                    
          Serial.print(posz);

          Serial.print((" "));

          Serial.print(alfa);      

          Serial.print((" "));

          Serial.print(beta);

          Serial.print((" "));

          Serial.print(gama);
          Serial.print("\n \r");
        }

       LED_OFF(); 
         
    }

    read_flag = 0;

   

  }

  // put your main code here, to run repeatedly:


float convertRawAcceleration(int aRaw) {

  float a = (aRaw * (float)ACC_RANGE) / IMU_RAW_MAX;

  return a;

}

float convertRawGyro(int gRaw) {

  float g = (gRaw * (float)GYRO_RANGE) / IMU_RAW_MAX;

  return g;

}

int filterhp(int input, int old){
return (int)(FILTER_COEF*input) + (int)(((1-FILTER_COEF)*old));
}
