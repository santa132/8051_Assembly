#include <virtuabotixRTC.h>
#include <LowPower.h>
//thứ tự kết nối chân CLK -> 10 , DAT -> 11, Reset -> 12
virtuabotixRTC myRTC(10, 11, 12);  //khai báo các chân kết nối
int count = 0;
int percent;
int readss;
int term;
int second;
int minute;
bool flag_van = true ;

void setup() {
 Serial.begin(9600);
 // Power Save bang cach output tat ca GPIO
  for (int i=0;i<20;i++){
    if (i != 19 && i != 2 ) pinMode(i, OUTPUT);
 }
 // Chỉnh thông số theo thứ tự
 // seconds, minutes, hours, day of the week, day of the month, month, year
 myRTC.setDS1302Time(40, 59, 15, 3, 28, 6, 2022); 
 pinMode(A5, INPUT);  // A5 = pin 19
 pinMode(2, INPUT); 
}

void loop()
{
   // lệnh để kiểm tra và update đúng thời gian thực tế
   myRTC.updateTime();
   // In ra màn hình   
   Serial.print("Current / Time: ");
   Serial.print(myRTC.dayofmonth); 
   Serial.print("/");
   Serial.print(myRTC.month);
   Serial.print("/");
   Serial.print(myRTC.year);
   Serial.print(" ");
   Serial.print(myRTC.hours);
   Serial.print(":");
   Serial.print(myRTC.minutes);
   Serial.print(":");
   Serial.println(myRTC.seconds);
    // Sleep Mode
    attachInterrupt(0, wakeUp, RISING);
    count++;
    if (count == 5) {
    Serial.println("Sleep");
    delay(100);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    Serial.println("Wake-up");
    delay(100);
    }
    detachInterrupt(0);
    // sensor 
    readss = analogRead(A5);
    term = map(readss, 0, 1023, 0, 100);
    percent = 100 - term;
    Serial.print(" Sensor: ");
    Serial.println(percent);
    
    // Mo van 10s khi den gio tuoi nuoc.
    if ((myRTC.hours == 7 || myRTC.hours == 16 || myRTC.hours == 19) && myRTC.minutes == 0 && flag_van == true) {
      MoVan();
      MoVan();
      flag_van = false; 
      count = 0;
      }
      
     // Tat co bao da tuoi xong
      if (myRTC.minutes == 1) flag_van = true;
      
     // Tuoi khi thieu nuoc
    if (percent > 40) {
          digitalWrite(4, LOW); // tắt van
    }
    else {  MoVan();
            count = 0;
     }
    if (count == 5) count--; 
    delay(1000);
}
void MoVan(){
  digitalWrite(4, HIGH); //mở van
        second= myRTC.seconds ;
        minute = myRTC.minutes;
    if(second>54){
        if(minute){
          while(myRTC.minutes==0 && second-55!=myRTC.seconds){myRTC.updateTime();}
          }
        else{
          while(myRTC.minutes<=minute+1 && second-55!=myRTC.seconds){myRTC.updateTime();}
          }
      }
    else{ while (myRTC.seconds - second <= 5){
          myRTC.updateTime();
          //Serial.println(myRTC.seconds);
        }
    }
   digitalWrite(4, LOW);
 }
 void wakeUp(){
  Serial.println("Wake-up interrupt");
}
