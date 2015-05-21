#define GYRO_ADDRESS 0x6b
#define GYRO_WHOAMI_REG 0x0F 
#define GYRO_IDENTITY 0xD7
#define GYRO_CTRL_REG1 0x20  
#define GYRO_CTRL_REG2 0x21
#define GYRO_CTRL_REG3 0x22
#define GYRO_CTRL_REG4 0x23
#define GYRO_CTRL_REG5 0x24
#define GYRO_RATE_HZ 760;

int16_t gyro[2][3] = {{0,0,0},{0,0,0}};   
int16_t accel[2][3] = {{0,0,0},{0,0,0}};   

long prevtime = micros();
long nowtime, deltatime;

double deltatimeseconds = 0.001;

/****************/

int i2cspeed = 10;
boolean gyro1init = 0;
boolean gyro2init = 0;
boolean accel1init = 0;
boolean accel2init = 0;



void setup() {
   Serial.begin(115200);
   delay(1000);

   while ( gyro1init == 0) {
     gyro1init = initializeGyro(0x6a, 9,8);
     Serial.println("init 6a gyro");
   }
   
   while ( gyro2init == 0) {
     gyro2init = initializeGyro(0x6b, 9,8);
     Serial.println("init 6b gyro");
   } 
   
   while ( accel1init == 0) {
     accel1init = initializeAccel(0b0011101, 9,8);
     Serial.println("init 6a accel");
   }
   
   while ( accel2init == 0) {
     accel2init = initializeAccel(0b0011110, 9,8);
     Serial.println("init 6b accel");
   }    
}



void loop() {
  
 // getCtrl();
 // gyroStatus();
  readGyroRaw(0x6a, 9,8, 0);
  readGyroRaw(0x6b, 9,8, 1);
  readAccelRaw(0b0011101, 9,8, 0);
  readAccelRaw(0b0011110, 9,8, 1);
  
  Serial.print("{ \"g0x\" :\"");
  Serial.print((int)gyro[0][0]); 
  Serial.print("\", \"g0y\" :\"");
  Serial.print((int)gyro[0][1]); 
  Serial.print("\", \"g0z\" :\"");
  Serial.print((int)gyro[0][2]); 
  Serial.print("\", \"g1x\" :\"");
  Serial.print((int)gyro[1][0]); 
  Serial.print("\", \"g1y\" :\"");
  Serial.print((int)gyro[1][1]); 
  Serial.print("\", \"g1z\" :\"");
  Serial.print((int)gyro[1][2]); 
  Serial.println("\"}");  
  
  Serial.print("{ \"a0x\" :\"");
  Serial.print((int)accel[0][0]); 
  Serial.print("\", \"a0y\" :\"");
  Serial.print((int)accel[0][1]); 
  Serial.print("\", \"a0z\" :\"");
  Serial.print((int)accel[0][2]); 
  Serial.print("\", \"a1x\" :\"");
  Serial.print((int)accel[1][0]); 
  Serial.print("\", \"a1y\" :\"");
  Serial.print((int)accel[1][1]); 
  Serial.print("\", \"a1z\" :\"");
  Serial.print((int)accel[1][2]); 
  Serial.println("\"}");  
  
  delay(10);
}

/********** ACCEL *****************/

int initializeAccel(byte devaddress, int sda, int scl) { 
   // Accelerometer
    // 0x00 = 0b00000000
    // AFS = 0 (+/- 2 g full scale)
      beginTransmission(devaddress,sda, scl);
      i2c_write(0x21,sda, scl);    
      i2c_write(0x00,sda, scl);       
      endTransmission(sda, scl);    

    // 0x57 = 0b01010111
    // AODR = 0101 (50 Hz ODR); AZEN = AYEN = AXEN = 1 (all axes enabled)
      beginTransmission(devaddress,sda, scl);
      i2c_write(0x20,sda, scl);    
      i2c_write(0x57,sda, scl);       
      endTransmission(sda, scl);     
    


    // Magnetometer

    // 0x64 = 0b01100100
    // M_RES = 11 (high resolution mode); M_ODR = 001 (6.25 Hz ODR)
      beginTransmission(devaddress,sda, scl);
      i2c_write(0x24,sda, scl);    
      i2c_write(0x64,sda, scl);       
      endTransmission(sda, scl);   
      


    // 0x20 = 0b00100000
    // MFS = 01 (+/- 4 gauss full scale)
      beginTransmission(devaddress,sda, scl);
      i2c_write(0x25,sda, scl);    
      i2c_write(0x20,sda, scl);       
      endTransmission(sda, scl);       
    


    // 0x00 = 0b00000000
    // MLP = 0 (low power mode off); MD = 00 (continuous-conversion mode)
      beginTransmission(devaddress,sda, scl);
      i2c_write(0x26,sda, scl);    
      i2c_write(0x00,sda, scl);       
      endTransmission(sda, scl);       
};




void readAccelRaw(byte devaddress, int sda, int scl, int id) {   
    uint8_t xlg,xhg, ylg, yhg, zlg, zhg;
    xlg = getReading(devaddress, 0x28, sda, scl);
    xhg = getReading(devaddress, 0x29, sda, scl);
    ylg = getReading(devaddress, 0x2A, sda, scl);
    yhg = getReading(devaddress, 0x2B, sda, scl);
    zlg = getReading(devaddress, 0x2C, sda, scl);
    zhg = getReading(devaddress, 0x2D, sda, scl);
    
    int16_t accelraw[3];
    accelraw[0] = (int16_t)(xhg << 8 | xlg);
    accelraw[1] = (int16_t)(yhg << 8 | ylg);
    accelraw[2] = (int16_t)(zhg << 8 | zlg);
    accel[id][0] = (int) accelraw[0];
    accel[id][1] = (int) accelraw[1];
    accel[id][2] = (int) accelraw[2];
}






/************* GYRO *****************/


int initializeGyro(byte devaddress, int sda, int scl) {

    beginTransmission(devaddress,sda, scl);
    i2c_write(GYRO_WHOAMI_REG,sda, scl);    
    endTransmission(sda, scl);  
    requestFrom(devaddress,sda, scl);  
    byte i2cid = i2c_read(sda, scl);
    endTransmission(sda, scl); 
    Serial.println(i2cid);
    if (i2cid == GYRO_IDENTITY)
    {

      //using node.js to convert binary to hex
      // parseInt("1010000", 2).toString(16);
      
      // SET SENSITIVITY
      beginTransmission(devaddress,sda, scl);
      i2c_write(GYRO_CTRL_REG4,sda, scl);    
      i2c_write(0xa0,sda, scl);       
      endTransmission(sda, scl); 
      
      // ENABLE
      beginTransmission(devaddress,sda, scl);
      i2c_write(GYRO_CTRL_REG1,sda, scl);   
      i2c_write(0x6F,sda, scl);    
      endTransmission(sda, scl);    
      
      return 1;  //success
    } else { 
      return 0; //fail
    }

}



void readGyroRaw(byte devaddress, int sda, int scl, int id) {   
    uint8_t xlg,xhg, ylg, yhg, zlg, zhg;
    xlg = getReading(devaddress, 0x28, sda, scl);
    xhg = getReading(devaddress, 0x29, sda, scl);
    ylg = getReading(devaddress, 0x2A, sda, scl);
    yhg = getReading(devaddress, 0x2B, sda, scl);
    zlg = getReading(devaddress, 0x2C, sda, scl);
    zhg = getReading(devaddress, 0x2D, sda, scl);
    
    int16_t gyroraw[3];
    gyroraw[0] = (int16_t)(xhg << 8 | xlg);
    gyroraw[1] = (int16_t)(yhg << 8 | ylg);
    gyroraw[2] = (int16_t)(zhg << 8 | zlg);
    gyro[id][0] = (int) gyroraw[0];
    gyro[id][1] = (int) gyroraw[1];
    gyro[id][2] = (int) gyroraw[2];
}


void getAddr(int sda, int scl) {
  beginTransmission(GYRO_ADDRESS,sda, scl);
  i2c_write(GYRO_WHOAMI_REG,sda, scl);    
  endTransmission(sda, scl);    
  requestFrom(GYRO_ADDRESS,sda, scl);  
  byte i2cid = i2c_read(sda, scl);
  Serial.println(i2cid);
  endTransmission(sda, scl);   
}

void getCtrl(int sda, int scl) {
  beginTransmission(GYRO_ADDRESS,sda, scl);
  i2c_write(0x20 ,sda, scl);    
  endTransmission(sda, scl);    
  requestFrom(GYRO_ADDRESS,sda, scl);  
  byte reply = i2c_read(sda, scl);
  endTransmission(sda, scl);   
  Serial.println(reply);  
}

void gyroStatus(int sda, int scl) {
  beginTransmission(GYRO_ADDRESS,sda, scl);
  i2c_write(0x27,sda, scl);    
  endTransmission(sda, scl);    
  requestFrom(GYRO_ADDRESS,sda, scl);  
  byte reply = i2c_read(sda, scl);
  endTransmission(sda, scl);   
  
  if (reply != 255) {
  Serial.println("status");
  Serial.println(reply);
  }  
  
}



byte i2cGet(byte devaddress, byte regaddress, int sda, int scl) {
  beginTransmission(devaddress, sda, scl);
  i2c_write(regaddress, sda, scl);  
  endTransmission(sda, scl);  
  requestFrom(devaddress, sda, scl);  
  byte data = i2c_read(sda, scl);
  endTransmission(sda, scl); 
  return data;
}


uint8_t getReading(uint8_t devaddress, uint8_t regaddress, int sda, int scl) {
    beginTransmission(devaddress, sda, scl);    
    i2c_write(regaddress, sda, scl);
    endTransmission(sda, scl);
    requestFrom(devaddress, sda, scl);
    endTransmission(sda, scl);    
    uint8_t reply = i2c_read(sda, scl); nack(sda, scl);
    endTransmission(sda, scl);
    return reply;
}

void ack(int sda, int scl) {
  pinMode(sda, OUTPUT);
  digitalWrite(sda, LOW);
  delayMicroseconds(i2cspeed);
  digitalWrite(scl, HIGH);
  delayMicroseconds(i2cspeed);
  digitalWrite(scl, LOW);
  digitalWrite(sda, HIGH);
}

void nack(int sda, int scl) {
  pinMode(sda, OUTPUT);  
  digitalWrite(sda, HIGH);
  delayMicroseconds(i2cspeed);
  digitalWrite(scl, HIGH);
  delayMicroseconds(i2cspeed);
  digitalWrite(scl, LOW);
  digitalWrite(sda, HIGH);
}

void i2c_start(int sda, int scl) {
  pinMode(sda, OUTPUT);
  digitalWrite(sda, LOW);
  delayMicroseconds(i2cspeed);
  pinMode(scl, OUTPUT);
  digitalWrite(scl, LOW);
}

void i2c_stop(int sda, int scl) {
  pinMode(scl, OUTPUT);
  digitalWrite(scl, HIGH);
  delayMicroseconds(i2cspeed);
  pinMode(sda, OUTPUT);
  digitalWrite(sda, HIGH);  
}


uint8_t beginTransmission(uint8_t address, int sda, int scl) {
  i2c_start(sda, scl);
  uint8_t rc = i2c_write((address<<1) | 0, sda, scl);
  return rc;
}

uint8_t beginTransmission(int address, int sda, int scl) {
  return beginTransmission( (uint8_t) address, sda, scl);
}

uint8_t requestFrom(uint8_t address, int sda, int scl)
{
    i2c_start(sda, scl);
    uint8_t rc = i2c_write((address<<1) | 1, sda, scl); // set read bit
    return rc;
}

uint8_t requestFrom(int address, int sda, int scl)
{
    return requestFrom( (uint8_t) address, sda, scl);
}

uint8_t endTransmission(int sda, int scl)
{
    i2c_stop(sda, scl);
    return 0;
}

uint8_t i2c_write( uint8_t c , int sda, int scl)
{
    for ( uint8_t i=0;i<8;i++) {
        i2c_writebit( c & 128 , sda, scl);
        c<<=1;
    }
    
    return i2c_readbit(sda, scl); //ACK
}

uint8_t i2c_read(int sda, int scl)
{
    uint8_t res = 0;

    for ( uint8_t i=0;i<8;i++) {
        res <<= 1;
        res |= i2c_readbit(sda, scl);  
        //Serial.println(res);
    }

    //i2c_writebit(0); //ACK
    return res;
}

void i2c_writebit( uint8_t c, int sda, int scl )
{
    delayMicroseconds(i2cspeed);
    
    if ( c > 0 ) {
      pinMode(sda, OUTPUT);
      digitalWrite(sda, HIGH);
    } else {
      pinMode(sda, OUTPUT);
      digitalWrite(sda, LOW);
    }
    delayMicroseconds(i2cspeed);
    
    pinMode(scl, OUTPUT);
    digitalWrite(scl, HIGH);
    delayMicroseconds(i2cspeed);

    pinMode(scl, OUTPUT);
    digitalWrite(scl, LOW);
    delayMicroseconds(i2cspeed);
   
}

//
uint8_t i2c_readbit(int sda, int scl)
{
    
    pinMode(sda, INPUT);
    digitalWrite(scl, LOW);
    delayMicroseconds(i2cspeed);
    digitalWrite(scl, HIGH);
      delayMicroseconds(i2cspeed);    
      uint8_t i2creadbit = digitalRead(sda);   
    digitalWrite(scl, LOW);
    
    delayMicroseconds(i2cspeed);

    return i2creadbit;
}