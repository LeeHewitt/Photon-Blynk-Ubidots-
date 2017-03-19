//BERT as of 19/3/2017 https://community.particle.io/t/dailytimerspark-library-added/22911/11



// This #include statement was automatically added by the Particle IDE.
#include <DailyTimerSpark.h>


// WAVE 2 initial timing settings
//int tStartHour = 15;
//int tStartMin = 30;
//int tStopHour = 17;
//int tStopMin = 0;

// WAVE 3 initial timing settings
//int tStartHour2 = 18;
//int tStartMin2 = 0;
//int tStopHour2 = 21;
//int tStopMin2 = 0;


// This #include statement was automatically added by the Particle IDE.
#include <blynk.h>

WidgetLCD lcd(V0);


char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

//BLYNK_WRITE(V17) {                      //time input
//  TimeInputParam t(param);
//  tStartHour = t.getStartHour();
//  tStartMin = t.getStartMinute();
//  tStopHour = t.getStopHour();
//  tStopMin = t.getStopMinute();
//}

/*BLYNK_WRITE(V18) {                      //time input
  TimeInputParam t(param);
  tStartHour2 = t.getStartHour();
  tStartMin2 = t.getStartMinute();
  tStopHour2 = t.getStopHour();
  tStopMin2 = t.getStopMinute();
}*/

int constantOn = 0;
int heatOff = 0;
int shot = 0;
int heatVariable = 0;
int pulseTimeOn = 0;
int pulseTimeOff = 0;
int oneHour = 0;
unsigned int heatTime = 0;
unsigned int tTime = 0;
int pulseOn = 0;
int pulseOff = 0;
int pumpDelay = 0;
int pumpPump = 0;
int pumpDelayAdd = 300000;
int pumpPumpAdd = 300000;
int tSlide1 = 10000;


DailyTimer timer1(6, 0,  8, 0, EVERY_DAY);                                            // 06:00 to 08:00 optional callback function for random number generation, see below example
DailyTimer timer2(15, 30,  17, 0, EVERY_DAY);            // 15:30 to 17:00
DailyTimer timer3(18, 0,  21, 0, EVERY_DAY);        // 18:00 to 21:00
//DailyTimer timer2(12,  0, 13,  0, SATURDAY, FIXED);                                 // default is FIXED, this will randomize the start time only
//DailyTimer timer3( 8, 30, 8, 31, WEEKENDS);                                         // creates with a default fixed start time and end time
bool timer1_LastState = false;
bool timer2_LastState = false;
bool timer3_LastState = false;

uint32_t lastUpdateTime = 0;




void setup()
{
    Serial.begin(9600);
    delay(5000);                                                                      // Allow board to settle

    Blynk.begin(auth);
    
    //BEGIN ON SEQUENCE
    Blynk.virtualWrite(V11, 800); //LED: COLD
    delay(1000);
    Blynk.virtualWrite(V12, 800); //LED: heating is OFF DUE TO THERMOSTAT
    delay(1000);
    Blynk.virtualWrite(V7, 800);  //LED: heating is in warmish mode
    delay(1000);
    Blynk.virtualWrite(V9, 800);  //LED: Warmer
    delay(1000);
    Blynk.virtualWrite(V6, 800);  //LED: 1 HOUR of heat requested
    delay(1000);
    Blynk.virtualWrite(V4, 800);  //LED: heating is off
    delay(1000);
    Blynk.virtualWrite(V3, 800);  //LED: heating is on constant
    delay(1000);
    Blynk.virtualWrite(V5, 800);  //LED: TIMED HEATING ON
    delay(2000);
    
    Blynk.virtualWrite(V5, 0);    //LED: TIMED HEATING ON
    delay(1000);
    Blynk.virtualWrite(V3, 0);    //LED: heating is on constant
    delay(1000);
    Blynk.virtualWrite(V4, 0);    //LED: heating is off
    delay(1000);
    Blynk.virtualWrite(V6, 0);    //LED: 1 HOUR of heat requested
    delay(1000);
    Blynk.virtualWrite(V9, 0);    //LED: Warmer
    delay(1000);
    Blynk.virtualWrite(V7, 0);    //LED: heating is in warmish mode
    delay(1000);
    Blynk.virtualWrite(V12, 0);   //LED: heating is OFF DUE TO THERMOSTAT
    delay(1000);
    Blynk.virtualWrite(V11, 0);   //LED: COLD
    delay(1000);
    //END ON SEQUENCE
    
    
  pinMode(D0,OUTPUT);
  //pinMode(D1,OUTPUT);
  pulseOn = 0;
  Particle.subscribe("Heatingwey", myHandler); 
  lcd.clear();
  lcd.print(0, 0, "Hello World! ");
  lcd.print(0, 1, "Coffee on...");
  delay(3000);
  lcd.clear();
}

void  loop(){  
          
  Blynk.run();
  
  
  
  if ((constantOn == 1) && (heatOff == 0) && (heatVariable == 0))                            //CONSTANT (K) is ON ADD THIS: && (oneHour == 3)
  {
      digitalWrite(D0,HIGH);
  }
  if (constantOn == 0)
  {
      digitalWrite(D0,LOW);
      constantOn = 2;
  }
  
  
  
  
  if (heatOff == 1)                                                                    //HEATING HAS BEEN TURNED OFF  
  {
     digitalWrite(D0,LOW);
     heatVariable = 0;
  }
  
  
  
  
  if ((oneHour == 1) && (constantOn == 2) && (heatOff == 0) && (heatVariable == 0))                           //Requested heat ON
  {
     digitalWrite(D0,HIGH);
  }

  if ((oneHour == 0) && (constantOn == 2) && (heatOff == 0) && (heatVariable == 0))
  {
      oneHour = 3;
      digitalWrite(D0,LOW);
  }
  
  if (heatTime < millis())
  {
      oneHour = 0;
  }

  
  
  
  
  
  if ((heatOff == 0) && (heatVariable == 1))        // timed heat, pulsed
  {
      if (shot == 0)
      {
      digitalWrite(D0,HIGH);
      }
       if (shot == 1)
      {
      digitalWrite(D0,LOW);
      }
} 
  if ((pulseOn == 0) && (shot == 2) && (heatOff == 0)  && (heatVariable == 1)) /////////////////////////////warmer
  {
      digitalWrite(D0,HIGH);
      pulseOff = 0;
      pumpDelay = (millis() + pumpDelayAdd);
      }

          
  if ((pulseOff == 1) && (shot == 2) && (heatOff == 0) && (heatVariable == 1)) /////////////////////////////warmer
  {
      digitalWrite(D0,LOW);
      pulseOn = 1;
      pumpPump = (millis() + pumpPumpAdd);
      }
      
      
  if ((pulseOn == 0) && (shot == 6) && (heatOff == 0)  && (heatVariable == 1)) /////////////////////////////warmish
  {
      digitalWrite(D0,HIGH);
      pulseOff = 0;
      pumpDelay = (millis() + (pumpDelayAdd - 60000));
      }

          
  if ((pulseOff == 1) && (shot == 6) && (heatOff == 0) && (heatVariable == 1)) /////////////////////////////warmish
  {
      digitalWrite(D0,LOW);
      pulseOn = 1;
      pumpPump = (millis() + (pumpPumpAdd + 60000));
      }
         
          
      
      
  if (pumpPump < millis())
  {
      pulseOn = 1;
      pulseOff = 1;
  }
  
  if (pumpDelay < millis())
  {
      pulseOn = 0;
      pulseOff = 0;
  }
  
  
  bool timer1State = timer1.isActive();  //State Change method this block
  if(timer1State != timer1_LastState)
  {
    if(timer1State)
    {
      digitalWrite(D0, HIGH);
      lcd.print(0, 1, "wave 1 is on");
      //lcd.clear();
      Particle.publish("DailyTimer", "wave 1 on", 60, PRIVATE);
      heatVariable = 1;
      Blynk.virtualWrite(V5, 800);  //LED: TIMED HEATING ON
    }
    else
    {
      digitalWrite(D0, LOW);
      lcd.print(0, 1, "wave 1 finito");
      //delay(3000);
      //lcd.clear();
      Particle.publish("DailyTimer", "wave 1 off", 60, PRIVATE);
      heatVariable = 0;
      Blynk.virtualWrite(V5, 0);  //LED: TIMED HEATING ON
    }
    timer1_LastState = timer1State;
  }
  
  bool timer2State = timer2.isActive();  //State Change method this block
  if(timer2State != timer2_LastState)
  {
    if(timer2State)
    {
      digitalWrite(D0, HIGH);
      lcd.print(0, 1, "wave 2 is on");
      Particle.publish("DailyTimer", "wave 2 on", 60, PRIVATE);
      heatVariable = 1;
      Blynk.virtualWrite(V5, 800);  //LED: TIMED HEATING ON
    }
    else
    {
      digitalWrite(D0, LOW);
      lcd.print(0, 1, "wave 2 finito");
      //delay(3000);
      //lcd.clear();
      Particle.publish("DailyTimer", "wave 2 off", 60, PRIVATE);
      heatVariable = 0;
      Blynk.virtualWrite(V5, 0);  //LED: TIMED HEATING ON
    }
    timer2_LastState = timer2State;
  }
  
  bool timer3State = timer3.isActive();  //State Change method this block
  if(timer3State != timer3_LastState)
  {
    if(timer3State)
    {
      digitalWrite(D0, HIGH);
      lcd.print(0, 1, "wave 3 is on");
      Particle.publish("DailyTimer", "wave 3 on", 60, PRIVATE);
      heatVariable = 1;
      Blynk.virtualWrite(V5, 800);  //LED: TIMED HEATING ON
    }
    else
    {
      digitalWrite(D0, LOW);
      lcd.print(0, 1, "wave 3 finito");
      delay(3000);
      lcd.clear();
      Particle.publish("DailyTimer", "wave 3 off", 60, PRIVATE);
      heatVariable = 0;
      Blynk.virtualWrite(V5, 0);  //LED: TIMED HEATING ON
    }
    timer3_LastState = timer3State;
  }
  
  if(millis() - lastUpdateTime > 1000UL)
  {
    char timeBuffer[32] = "";
    lcd.print(0, 0, Time.timeStr());
    lastUpdateTime += 1000UL;
  }
          
  }   
      
  




//THE BLYNK SECTION

// constant heat on
BLYNK_WRITE(V1) {
    if (param.asInt() == 1 && (heatOff == 0) && (heatVariable == 0) && (oneHour == 3))
    {  
    constantOn = 1;
    Blynk.virtualWrite(V11, 900);   //LED: Cold
    Blynk.virtualWrite(V12, 0);     //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);      //LED: Warmish
    Blynk.virtualWrite(V6, 0);      //LED: 1 HOUR of heat requested
    Blynk.virtualWrite(V4, 0);      //LED: heating is off
    Blynk.virtualWrite(V3, 900);    //LED: heating is on constant
    }
    //}
    else if (param.asInt() == 0)
    {  
    constantOn = 0;
    Blynk.virtualWrite(V6, 0);      //LED: 1 HOUR of heat requested
    Blynk.virtualWrite(V3, 0);    //LED: heating is on constant
    }
}

// heating off
BLYNK_WRITE(V2) {
    if (param.asInt() == 1) 
    {       
    heatOff = 1;
    //Blynk.virtualWrite(V11, 0);     //LED: Cold
    //Blynk.virtualWrite(V12, 0);     //LED: heating is OFF DUE TO THERMOSTAT
    //Blynk.virtualWrite(V7, 0);      //LED: Warmish
    Blynk.virtualWrite(V6, 0);      //LED: 1 HOUR of heat requested
    Blynk.virtualWrite(V4, 500);    //LED: heating is off
    Blynk.virtualWrite(V3, 0);      //LED: heating is on constant
    Blynk.virtualWrite(V3, 0);      //LED: TIMED ON
    oneHour = 0;
    constantOn = 0;
    }
    else if (param.asInt() == 0)
    {  
    heatOff = 0;
    //digitalWrite(D1,LOW);
    Blynk.virtualWrite(V4, 0);       //LED: heating is off
    }
}

// 1 hour heat
BLYNK_WRITE(V8) {
    if (param.asInt() == 1 && (heatOff == 0) && (heatVariable == 0))
    {      
    oneHour = 1;
    //tTime = millis();
    heatTime = (millis() + tSlide1);     //1 hour added to current time ==========================================================================
    Blynk.virtualWrite(V6, 900);     //LED: 1 HOUR of heat requested
    }
    }

BLYNK_WRITE(V16) {
    if (param.asInt() == 1)
    {      
    oneHour = 0;
    Blynk.virtualWrite(V6, 0);     //LED: 1 HOUR of heat requested
    }
    }

BLYNK_WRITE(V15) {
  tSlide1 = (1000 * param.asInt());
  lcd.print(0, 1, (tSlide1/1000)/60);
  delay(1000);
  lcd.clear();
  //Particle.publish("yqwegbbbHG", String(tSlide1), 60, PRIVATE);
}

BLYNK_WRITE(V13) {                  // pump on
  pumpPumpAdd = (1000 * param.asInt());
  lcd.print(0, 1, (pumpPumpAdd/1000)/60);
  delay(1000);
  lcd.clear();
}

BLYNK_WRITE(V14) {                  // pump off
  pumpDelayAdd = (1000 * param.asInt());
  lcd.print(0, 1, (pumpDelayAdd/1000)/60);
  delay(1000);
  lcd.clear();
}

// reset
BLYNK_WRITE(V10) {
    if (param.asInt() == 1) 
    { 
    System.reset();
    }  
}




void myHandler(const char *event, const char *data)
{
  /* Particle.subscribe handlers are void functions, which means they don't return anything.
  They take two variables-- the name of your event, and any data that goes along with your event.

  chars just don't play that way. Instead we're going to strcmp(), which compares two chars.
  If they are the same, strcmp will return 0.
  */

   if (strcmp(data,"23")==0)    //It's warm, turn the heating off                       23C
  {
    shot = 1; //1
    Blynk.virtualWrite(V12, 800);       //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 0);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "23C");
  } 
   else
   if (strcmp(data,"22")==0)    //It's warm, turn the heating off                       22C
  {
    shot = 1; //1
    Blynk.virtualWrite(V12, 800);       //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 0);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "22C");
  } 
   else
   if (strcmp(data,"21")==0)    //It's warm, turn the heating off                       21C
  {
    shot = 1; //1
    Blynk.virtualWrite(V12, 800);       //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 0);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "21C");
  } 
   else 
   if (strcmp(data,"20")==0)    //It's warm, turn the heating off                       20C
  {
    shot = 1; //1
    Blynk.virtualWrite(V12, 800);       //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 0);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "20C");
  } 
   else 
   if (strcmp(data,"itshot")==0)    //It's warm, turn the heating off                       19C and above
  {
    shot = 1; //1
    Blynk.virtualWrite(V12, 800);       //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 0);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "19C");
  } 
   else 
   if (strcmp(data,"pulse2")==0)    //It's warmer
  {
    shot = 2;                       //2                                                     18.5C to 19C
    Blynk.virtualWrite(V7, 500);        //LED: Warmish
    Blynk.virtualWrite(V12, 0);         //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V11, 0);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "18.5C");
  }
   else 
   if (strcmp(data,"pulse1")==0)    //It's nearly warm
  {
    shot = 6;                       //2                                                     18C to 18.5C
    Blynk.virtualWrite(V9, 500);        //LED: Warmer
    Blynk.virtualWrite(V7, 0);        //LED: Warmish
    Blynk.virtualWrite(V12, 0);         //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V11, 0);       //LED: Cold
    lcd.print(0, 1, "18C");
  }
   else 
   if (strcmp(data,"itscold")==0)        //It's cold
  {
    shot = 0;                       //0                                                     17C and below
    Blynk.virtualWrite(V12, 0);         //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 900);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "17C");
  }
  else 
  if (strcmp(data,"16")==0)        //It's cold
  {
    shot = 0;                       //0                                                     16C
    Blynk.virtualWrite(V12, 0);         //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 900);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "16C");
  }
  else 
  if (strcmp(data,"15")==0)        //It's cold
  {
    shot = 0;                       //0                                                     15C
    Blynk.virtualWrite(V12, 0);         //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 900);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "15C");
  }
  else 
  if (strcmp(data,"14")==0)        //It's cold
  {
    shot = 0;                       //0                                                     14C
    Blynk.virtualWrite(V12, 0);         //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 900);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "14C");
  }
  else 
  if (strcmp(data,"13")==0)        //It's cold
  {
    shot = 0;                       //0                                                     13C
    Blynk.virtualWrite(V12, 0);         //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 900);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "13C");
  }
  else 
  if (strcmp(data,"12")==0)        //It's cold
  {
    shot = 0;                       //0                                                     12C
    Blynk.virtualWrite(V12, 0);         //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 900);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "12C");
  }
  else 
  if (strcmp(data,"11")==0)        //It's cold
  {
    shot = 0;                       //0                                                     11C
    Blynk.virtualWrite(V12, 0);         //LED: heating is OFF DUE TO THERMOSTAT
    Blynk.virtualWrite(V7, 0);          //LED: Warmish
    Blynk.virtualWrite(V11, 900);       //LED: Cold
    Blynk.virtualWrite(V9, 0);        //LED: Warmer
    lcd.print(0, 1, "11C");
  }
   

}


