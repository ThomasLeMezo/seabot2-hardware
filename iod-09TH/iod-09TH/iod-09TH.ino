//
// NB! This is a file generated from the .4Dino file, changes will be lost
//     the next time the .4Dino file is built
//
#include "ESP8266WiFi.h"
#include "GFX4dIoD9.h"
#include <Wire.h>

GFX4dIoD9 gfx = GFX4dIoD9();

#include "iod-09THConst.h"    // Note. This file will not be created if there are no generated graphics

int reg_i2c = 0;
byte data_ip[4] = {0, 0, 0, 0};
short data_pressure = 0;
short data_temp = 0;
char data_name[9] = "XXX";
char data_mission[16] = "XXX";
byte data_wp = 0;
byte data_wp_max = 0;
byte data_time[2] = {0,0};
byte data_time_remain[2] = {0,0};

void setup()
{
  Wire.begin(0x04);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event

  gfx.begin();
  gfx.Cls();
  gfx.ScrollEnable(false);
  gfx.BacklightOn(true);
  gfx.Orientation(LANDSCAPE_R);
  gfx.SmoothScrollSpeed(5);
  gfx.TextColor(WHITE, BLACK); gfx.Font(2);  gfx.TextSize(1);
  gfx.Open4dGFX("IOD-09~1"); // Opens DAT and GCI files for read using filename without extension. Note! Workshop generates files with Short filenames
} // end Setup **do not alter, remove or duplicate this line**

void loop()
{
  print_screen();
  // put your main code here, to run repeatedly:
  yield(); // Required for ESP
}



void print_screen()
{
  gfx.Cls();

  // Robot Name
  gfx.TextWindow(0,44,82,14,WHITE,BLACK);
  gfx.TWprintln(data_name);

  // Time
  gfx.TextWindow(0,128,32,14,WHITE,BLACK);
  char s_time[6];
  sprintf(s_time, "%i:%i", data_time[0], data_time[1]);
  gfx.TWprintln(s_time);

  // IP
  gfx.TextWindow(14,17,97,14,WHITE,BLACK);
  char s_ip[16] = "XXX.XXX.XXX.XXX";
  if(data_ip[0]==0){
     gfx.TWcolor(RED);
  }
  else{
    sprintf(s_ip, "%i.%i.%i.%i", data_ip[0], data_ip[1], data_ip[2], data_ip[3]);
  }
  gfx.TWprintln(s_ip);

  // Pressure
  gfx.TextWindow(28,0,28,14,RED,BLACK);
  char s_pressure[5] = "XXXX";
  sprintf(s_pressure, "%i", data_pressure);
  gfx.TWprintln(s_pressure);

  // Temperature
  gfx.TextWindow(28,64,25,14,RED,BLACK);
  char s_temp1[4] = "000";
  sprintf(s_temp1, "%i", data_temp);
  gfx.TWprintln(s_temp1[0]+s_temp1[1]+"."+s_temp1[2]);

  // Mission name
  gfx.TextWindow(42,41,95,14,WHITE,BLACK);
  gfx.TWprintln(data_mission);

  // Waypoint
  gfx.TextWindow(56,25,47,14,WHITE,BLACK);
  char s_wp[8] = "256/256";
  sprintf(s_wp, "%i/%i", data_wp, data_wp_max);
  gfx.TWprintln(s_wp);

  // Remaining time
  gfx.TextWindow(56,80,36,14,WHITE,BLACK);
  char s_time_remain[7] = "-00:00";
  sprintf(s_time_remain, "-%i:%i", data_time_remain[0], data_time_remain[1]);
  gfx.TWprintln(s_time_remain);
}

void receiveEvent(int howMany)
{
  // Read
  if(howMany > 0){
   switch(reg_i2c){
    case 0:
      Wire.write("TEST", howMany);
      break;
    default:
      Wire.write(0x00);
      break;
   }
  }

  // Received
  if(Wire.available()){
    reg_i2c = Wire.read(); // receive byte as a character

    switch(reg_i2c){
      case 0:
          if(Wire.available()){
            byte val = Wire.read();
            if(val==0)
              gfx.BacklightOn(false); // Sets backlight on or off, Boolean true false
            else
              gfx.BacklightOn(true);
          }
        break;
      case 1:
        {
          data_name[0] = '\0';
          int i=0;
          while(Wire.available()){
            char val = Wire.read();
            if(i<sizeof(data_name)){
              data_name[i] = val;
              i++;
            }
          }
        }
        break;


      default:
        break;
    }
  }
}

