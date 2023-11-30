//Created by Pasquale Pepe pasqualepepe887@gmail.com

#include <Arduino.h>
#include <Servo.h>
#include <SPI.h>
#include <RTCLib.h>
#include<SoftwareSerial.h>
#include <EEPROM.h>


RTC_DS3231 rtc;
Servo m1;  //Use a 180 Servo for 2 SLOT or a 360 Servo for 3 Slot


// HC-05 BLUES  TX-> RX RX-> TX  VCC -> 3.3V


int i=0;  
bool edit_mode;                                                        
char inputString[20];
char back_date;
const char targetString_A[]= "get/date/A";
const char targetString_B[]= "get/date/B";
const char targetString_C[]= "get/date/C";
String a_search= "S/A/";
String b_search= "S/B/";
String c_search= "S/C/";
int stringIndex=0;
String inputString_set="";
String name_ext = "";
String time_ext = "";
bool one_time;

void sound(){
  for (int k=0; k<=10;k++){
  tone(D5,1000,200);
  delay(500);
  noTone(D5);
  delay(500);
  }
}

void save_memory(String data,int address){
 for (int j=0; j<data.length();j++){
    EEPROM.write(address+j,data[j]);

   }
   EEPROM.write(address+data.length(),'\0');
   
}

String read_memory(int address,int lenght_str){
  char dest[lenght_str+1];
  for (int j=0; j<lenght_str;j++){
    dest[j]=EEPROM.read(address+j);
if (dest[j] =='\0'){
break;
}
   }
      dest[lenght_str]= '\0';

   return String(dest);
}

String get_date(char slot, char date_type){  //date_type or T=time or N=name
  int address_buffer;
  String date_buffer= "";
  String return_date= "";
  if (slot == 'A'){
     address_buffer= 0;
     date_buffer= read_memory(address_buffer,30);

     if (date_type == 'N'){
        return_date= date_buffer.substring(1,date_buffer.indexOf('/')-1);  

     }else if (date_type == 'T'){
        return_date= date_buffer.substring(date_buffer.indexOf('/')+1);  
    }
  }else if (slot == 'B'){
address_buffer=50;
     date_buffer= read_memory(address_buffer,30);

      if (date_type == 'N'){
        return_date= date_buffer.substring(1,date_buffer.indexOf('/')-1);  

     }else if (date_type == 'T'){
        return_date= date_buffer.substring(date_buffer.indexOf('/')+1);  
    }
  }else if (slot == 'C'){
address_buffer=100;
    date_buffer= read_memory(address_buffer,30);

       if (date_type == 'N'){
        return_date= date_buffer.substring(1,date_buffer.indexOf('/')-1);  

     }else if (date_type == 'T'){
        return_date= date_buffer.substring(date_buffer.indexOf('/')+1);  
    }
  }

return return_date;
}


void setup() 
{
   pinMode(LED_BUILTIN, OUTPUT);
   pinMode(PA10,INPUT);
   pinMode( PA9,OUTPUT);
   pinMode(D5,OUTPUT);  //Set Pin of Buzzer
   m1.attach(D6);
   Serial.begin(9600);
   digitalWrite(LED_BUILTIN, HIGH); 
   Serial1.begin(9600);

   rtc.begin();
   if (rtc.lostPower()){
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
   }

   //Do animation with Servo and Slot 0=NO SLOT 90=SLOTA  180=SLOTB  270=SLOTC
   m1.write(0);
   delay(1000);
   m1.write(90);
   delay(1000);
   m1.write(180);
   delay(1000);
   m1.write(270);
   delay(1000);
   sound();
   edit_mode= false;

}


  


void loop(){
one_time=true;
  while (Serial1.available()){

  char date2= Serial1.read();

  inputString_set+=date2;
  Serial.print(date2);
  edit_mode = true;
 
  
  if (date2== targetString_A[stringIndex]){  //if the App wants the date of slot A
     //Go to SLOTA
    inputString[stringIndex]= date2;
    stringIndex++;
  
    if(stringIndex== sizeof(targetString_A)-1){
      inputString[stringIndex]= '\0';
    
      Serial.println(read_memory(0,30));  //Extract Dates from EEMPROM SLOTA ADDRESS 0
      Serial1.println(read_memory(0,30));
       m1.write(90);
       delay(1000);

      stringIndex= 0;
    }


  }else if(date2== targetString_B[stringIndex]){  //if the App wants the date of slot B
    //Go to SLOTB
    inputString[stringIndex]= date2;
    stringIndex++;
  
    if(stringIndex== sizeof(targetString_B)-1){
      inputString[stringIndex]= '\0';
          Serial.println(read_memory(50,30));  //Extract Dates from EEMPROM SLOTA ADDRESS 50
      Serial1.println(read_memory(50,30));
     m1.write(180);
       delay(1000);
      stringIndex= 0;
    }


  }else if (date2== targetString_C[stringIndex]){  //if the App wants the date of slot C
     //Go to SLOTA
    inputString[stringIndex]= date2;
    stringIndex++;
  
    if(stringIndex== sizeof(targetString_C)-1){
      inputString[stringIndex]= '\0';
      Serial.println(read_memory(100,30));  //Extract Dates from EEMPROM SLOTA ADDRESS 100
      Serial1.println(read_memory(100,30));
     m1.write(270);
       delay(1000);
      stringIndex= 0;
    }


  }else{

    stringIndex= 0;
  }

 }

 int s_a= inputString_set.indexOf(a_search);   //If there is the S/A/ so the device wants to set the date of slot A

  while(s_a !=-1 and one_time == true){

   m1.write(90); //Go to SLOTA
   delay(800);
   int second= inputString_set.indexOf('/',5);
   name_ext= inputString_set.substring(4,second);  
   int third= inputString_set.indexOf('/',second+1);
   time_ext = inputString_set.substring(second+1,third);
   Serial.println(name_ext);
   Serial.println(time_ext);
   String save_date_string =  name_ext+"/"+time_ext;
   save_memory(save_date_string,0);



   inputString_set = "";
   name_ext="";
   time_ext = "";
   one_time = false;
}

 int s_b= inputString_set.indexOf(b_search);   //If there is the S/B/ so the device wants to set the date of slot B

  while(s_b !=-1 and one_time == true){

   m1.write(180); //Go to SLOTB
   delay(800);
   int second= inputString_set.indexOf('/',5);
   name_ext= inputString_set.substring(4,second);  
   int third= inputString_set.indexOf('/',second+1);
   time_ext = inputString_set.substring(second+1,third);
   Serial.println(name_ext);
   Serial.println(time_ext);
   String save_date_string =  name_ext+"/"+time_ext;
   save_memory(save_date_string,50);



   inputString_set = "";
   name_ext="";
   time_ext = "";
   one_time = false;
}

int s_c= inputString_set.indexOf(c_search);   //If there is the S/C/ so the device wants to set the date of slot C

  while(s_c !=-1 and one_time == true){

   m1.write(270); //Go to SLOTC
   delay(800);
   int second= inputString_set.indexOf('/',5);
   name_ext= inputString_set.substring(4,second);  
   int third= inputString_set.indexOf('/',second+1);
   time_ext = inputString_set.substring(second+1,third);
   Serial.println(name_ext);
   Serial.println(time_ext);
   String save_date_string =  name_ext+"/"+time_ext;
   save_memory(save_date_string,100);



   inputString_set = "";
   name_ext="";
   time_ext = "";
   one_time = false;
}




  inputString_set = "";
  name_ext="";
  time_ext = "";
//delay(800);
//Start the time control and extract the data from the memory. After compare the date(time) with the RTC's time

DateTime now = rtc.now();

String now_time= String(now.hour())+ ":"+String(now.minute());

String a_time= get_date('A','T');
String b_time= get_date('B','T');
String c_time= get_date('C','T');

Serial.println(now_time);


if (now_time.equals(a_time)){
//SERVO GOES A
m1.write(90);
sound();
}else if (now_time.equals(b_time)){
//SERVO GOES B
m1.write(180);
sound();
}else if (now_time.equals(c_time)){
//SERVO GOES C
m1.write(270);
sound();

}else if (edit_mode == false){
  m1.write(0);
}
delay(800);
edit_mode= false;
}

