#include <Wire.h>
#include <I2Cdev.h> //I2C kütüphanesini ekledik
#include <MPU6050.h> //Mpu6050 kütüphanesi ekledik
#define  BLYNK_PRINT Serial
#include  <ESP8266WiFi.h>
#include  <BlynkSimpleEsp8266.h>

char auth[] = "tPdgASfDXLuL7clKF1NsJP9wUMlqwsFY";
char ssid[] = "Mi A2";
char pass[] = "00123456789";

MPU6050 ivme_sensor; // sensörümüze ivme_sensor adını verdik
int16_t ax, ay, az; //ivme tanımlama
int16_t gx, gy, gz; //gyro tanımlama
int off_x = 6;
int off_y = 6;


//-------------------------------------------------
void checkDataAndNotify(){
  ivme_sensor.getMotion6(&ax, &ay, &az, &gx, &gy, &gz); // ivme ve gyro değerlerini okuma
 
//açısal ivmeleri ve gyro değerlerini serial ekrana yazdıralım
  Serial.print("ax: "); 
  Serial.print(ax/100); 
  
  Serial.print("\tay: "); 
  Serial.println(ay/100); 
  
  if(off_x-(ax/100)>0 || off_x+(ax/100)>17 || off_y+(ay/100)<=0 || off_y+(ay/100)>6){
    Blynk.notify("!!! WARNING EARTHQUAKE DETECTED !!!");  
    for(int i=0; i<10; i++){
        Serial.print("X > 1: ");
        Serial.println(off_x-(ax/100));
        Serial.print("X > 17: ");
        Serial.println(off_x+(ax/100));
        
        Serial.print("Y <= 0: ");
        Serial.println(off_y+(ay/100));
        Serial.print("Y > 6: ");
        Serial.println(off_y+(ay/100));
        Serial.println();
  
        digitalWrite(D7,HIGH);
        digitalWrite(D6,HIGH);
        delay(50);
        digitalWrite(D7,LOW);
        digitalWrite(D6,LOW);
        delay(50);
    }
  }
}
//--------------------------------------------------------

void setup() {
  Wire.begin();
  Serial.begin(9600);
  pinMode(D7,OUTPUT);
  pinMode(D6, OUTPUT);
  digitalWrite(D7, LOW);
  digitalWrite(D6, LOW);

  Serial.println("I2C cihazlar baslatiliyor...");
  ivme_sensor.initialize();
  Serial.println("Test cihazi baglantilari...");
  Serial.println(ivme_sensor.testConnection() ? "MPU6050 baglanti basarili" : "MPU6050 baglanti basarisiz");

  Blynk.begin(auth, ssid, pass);
}

 
void loop() {
  checkDataAndNotify();
  delay(500);
  Blynk.run();
}
