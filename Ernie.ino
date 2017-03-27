// This #include statement was automatically added by the Particle IDE.
#include <Ubidots.h>

//ERNIE



// This #include statement was automatically added by the Particle IDE.
//#include <Ubidots.h>



// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// MPL3115A2
// This code is designed to work with the MPL3115A2_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Analog-Digital-Converters?sku=MPL3115A2_I2CS#tabs-0-product_tabset-2

#include <application.h>
#include <spark_wiring_i2c.h>

// MPL3115A2 I2C address is 0x60(96)
#define Addr 0x60

#define TOKEN "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"  // Put here your Ubidots TOKEN




Ubidots ubidots(TOKEN);

double cTemp = 0.0, fTemp = 0.0, pressure = 0.0, altitude = 0.0;
int temp = 0, tHeight = 0;
long pres = 0;
int x = 0;

void setup() 
{
    // Set variable
    Particle.variable("i2cdevice", "MPL3115A2");
    Particle.variable("cTemp", cTemp);
    Particle.variable("Pressure", pressure);
    //Particle.variable("altitude", altitude);
    //Particle.publish("Heatingwey", "null");

    // Initialise I2C communication
    Wire.begin();
    // Initialise Serial Communication, set baud rate = 9600
    Serial.begin(9600);

    // Start I2C transmission
    Wire.beginTransmission(Addr);
    // Select control register
    Wire.write(0x26);
    // Active mode, OSR = 128, altimeter mode
    Wire.write(0xB9);
    // Stop I2C transmission
    Wire.endTransmission();

    // Start I2C transmission
    Wire.beginTransmission(Addr);
    // Select data configuration register
    Wire.write(0x13);
    // Data ready event enabled for altitude, pressure, temperature
    Wire.write(0x07);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(300);
}

void loop()
{
    unsigned int data[6];

    // Start I2C transmission
    Wire.beginTransmission(Addr);
    // Select control register
    Wire.write(0x26);
    // Active mode, OSR = 128, altimeter mode
    Wire.write(0xB9);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(1000);

    // Start I2C transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write(0x00);
    // Stop I2C transmission
    Wire.endTransmission();

    // Request 6 bytes of data
    Wire.requestFrom(Addr, 6);
    
    // Read 6 bytes of data from address 0x00(00)
    // status, tHeight msb1, tHeight msb, tHeight lsb, temp msb, temp lsb
    if(Wire.available() == 6)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
        data[2] = Wire.read();
        data[3] = Wire.read();
        data[4] = Wire.read();
        data[5] = Wire.read();
    }
   

    // Convert the data to 20-bits
    tHeight = ((((long)data[1] * (long)65536) + (data[2] * 256) + (data[3] & 0xF0)) / 16);
    temp = ((data[4] * 256) + (data[5] & 0xF0)) / 16;
    altitude = tHeight / 16.0;
    cTemp = (temp / 16.0);
    fTemp = cTemp * 1.8 + 32;
    
    
    /*if (temp > 368) {                                                          // >23C
        Particle.publish("Heatingwey", "23");
    }
    else
    if ((temp > 352) && (temp <= 368)) {                                       // 22C
        Particle.publish("Heatingwey", "22");
    }
    else
    if ((temp > 336) && (temp <= 352)) {                                       // 21C
        Particle.publish("Heatingwey", "21");
    }
    else
    if (temp > 320) && (temp <= 336)) {                                       // 20C
        Particle.publish("Heatingwey", "twenty");
    }
    else*/
    if (temp > 304) {                                                         // 19C
        Particle.publish("Heatingwey", "itshot");
    }
    else
    if ((temp > 296) && (temp <= 304)) {                                       // >=18.5C <= 19C
        Particle.publish("Heatingwey", "pulse2");
    }
    else
    if ((temp > 288) && (temp <= 296)) {                                      // >=18C <= 18.5C
        Particle.publish("Heatingwey", "pulse1");
    }
    else
    if (temp <= 288) {                                       // <17C ((temp > 272) && (temp <= 288))
        Particle.publish("Heatingwey", "itscold");
    }/*
    else
    if ((temp > 256) && (temp <= 272)) {                                       // 16C
        Particle.publish("Heatingwey", "sixteen");
    }
    else
    if ((temp > 240) && (temp <= 256)) {                                       // 15C
        Particle.publish("Heatingwey", "15");
    }
    else
    if ((temp > 224) && (temp <= 240)) {                                       // 14C
        Particle.publish("Heatingwey", "14");
    }
    else
    if ((temp > 208) && (temp <= 224)) {                                       // 13C
        Particle.publish("Heatingwey", "13");
    }
    else
    if ((temp > 192) && (temp <= 208)) {                                       // 12C
        Particle.publish("Heatingwey", "12");
    }
    else
    if ((temp > 176) && (temp <= 192)) {                                       // 11C
        Particle.publish("Heatingwey", "11");
    }*/
    

    // Start I2C transmission
    Wire.beginTransmission(Addr);
    // Select control register
    Wire.write(0x26);
    // Active mode, OSR = 128, barometer mode
    Wire.write(0x39);
    // Stop I2C transmission
    Wire.endTransmission();
    
    // Start I2C transmission
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write(0x00);
    // Stop I2C transmission
    Wire.endTransmission();
    delay(1000);
    
    // Request 4 bytes of data
    Wire.requestFrom(Addr, 4);
    
    // Read 4 bytes of data
    // status, pres msb1, pres msb, pres lsb
    if(Wire.available() == 4)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
        data[2] = Wire.read();
        data[3] = Wire.read();
    }
   
    // Convert the data to 20-bits
    pres = (((long)data[1] * (long)65536) + (data[2] * 256) + (data[3] & 0xF0)) / 16;
    pressure = (pres / 4.0) / 100.0;

    // Output data to dashboard
    
    Particle.publish("pressure", String(pressure));
    delay(10000);
    Particle.publish("cTemp", String(cTemp));
    delay(10000);
    ubidots.add("Atmo pressure", pressure); 
    ubidots.add("hall way analog", cTemp); 
    ubidots.sendAll();
    
    
    delay(38000);
}





