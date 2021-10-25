//
// NB! This is a file generated from the .4Dino file, changes will be lost
//     the next time the .4Dino file is built
//
#include "ESP8266WiFi.h"
#include <GFX4dIoD9.h>
#include "Wire.h"

GFX4dIoD9 gfx = GFX4dIoD9();

//#include "iod-09THConst.h"    // Note. This file will not be created if there are no generated graphics

int reg_i2c = 0;
byte data_ip[4] = {0, 0, 0, 0};
short data_pressure = 9999;
short data_temperature = 999;
short data_tension = 999;
char data_name[9] = "TestNom";
char data_mission[16] = "TestMission";
byte data_wp = 0;
byte data_wp_max = 0;
byte data_time[2] = {0,0};
byte data_time_remain[2] = {0,0};


short pressure_threshold = 700;
short tension_threshold = 120;
short temperature_threshold = 400;

void setup()
  {

  Wire.begin(0x04);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event

  gfx.begin();
  gfx.Cls();
  gfx.ScrollEnable(false);
  gfx.BacklightOn(false);
  gfx.Orientation(LANDSCAPE);
  gfx.SmoothScrollSpeed(5);
  gfx.TextColor(WHITE, BLACK); gfx.Font(1); gfx.TextSize(1);
  gfx.TextWrap(false);
  Serial.begin(115200);

  print_screen();

} // end Setup **do not alter, remove or duplicate this line**

void loop()
{
  // put your main code here, to run repeatedly:
  yield(); // Required for ESP
}

void print_screen()
{
Serial.println("Test");
  gfx.Cls();

  // Robot Name
  gfx.MoveTo(0, 0);
  gfx.TextColor(WHITE);
  gfx.print(String("Seabot ") + data_name);

  // Time
  gfx.MoveTo(128, 0);
  gfx.TextColor(WHITE);
  char s_time[6];
  sprintf(s_time, "%02i:%02i", data_time[0], data_time[1]);
  gfx.print(s_time);

  // IP
  gfx.MoveTo(0, 14);
  char s_ip[19] = "IP XXX.XXX.XXX.XXX";
  if(data_ip[0]==0)
     gfx.TextColor(RED);
  else
      gfx.TextColor(WHITE);
  sprintf(s_ip, "IP %i.%i.%i.%i", data_ip[0], data_ip[1], data_ip[2], data_ip[3]);
  gfx.print(s_ip);

  // Pressure
  gfx.MoveTo(0, 28);
  if(data_pressure < pressure_threshold)
    gfx.TextColor(GREEN);
  else
    gfx.TextColor(RED);
  char s_pressure[10] = "XXXX mbar";
  sprintf(s_pressure, "%4i mbar", data_pressure);
  gfx.print(s_pressure);

  // Temperature
  gfx.MoveTo(64, 28);
  if(data_temperature < temperature_threshold)
    gfx.TextColor(GREEN);
  else
    gfx.TextColor(RED);
  char s_temp[8] = "00.0 �C";
  float temp = data_temperature/10.;
  sprintf(s_temp, "%3.1f \370C", temp);
  gfx.print(s_temp);

  // Tension
  gfx.MoveTo(112, 28);
  if(data_tension < tension_threshold)
    gfx.TextColor(GREEN);
  else
    gfx.TextColor(RED);
  char s_tens[7] = "00.0 V";
  float tension = data_tension/10.;
  sprintf(s_tens, "%2.1f V", tension);
  gfx.print(s_tens);

  // Mission name
  gfx.MoveTo(0, 42);
  gfx.TextColor(WHITE);
  gfx.print(String("Mission ") + data_mission);

  // Waypoint
  gfx.MoveTo(0, 56);
  gfx.TextColor(WHITE);
  char s_wp[11] = "Wp 256/256";
  sprintf(s_wp, "Wp %i/%i", data_wp, data_wp_max);
  gfx.print(s_wp);

  // Remaining time
  gfx.MoveTo(80, 56);
  gfx.TextColor(WHITE);
  char s_time_remain[7] = "-00:00";
  sprintf(s_time_remain, "-%02i:%02i", data_time_remain[0], data_time_remain[1]);
  gfx.print(s_time_remain);

  // ENSTA B logo
  gfx.MoveTo(70, 67);
  gfx.TextColor(ORANGE);
  gfx.print("ENSTA Bretagne");

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
      for(int i=0; i<howMany; ++i)
        Wire.write(0x00);
      break;
   }
  }

  // Received
  if(Wire.available()){
    reg_i2c = Wire.read(); // receive byte as a character

    if(Wire.available()){
      switch(reg_i2c){
        case 0: // Screen light
            if(Wire.available()){
              byte val = Wire.read();
              if(val==0)
                gfx.BacklightOn(false); // Sets backlight on or off, Boolean true false
              else
                gfx.BacklightOn(true);
            }
          break;
        case 1: // Name of the robot
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
        case 2: // IP
          {
            if(Wire.available()>3){
              data_ip[0] = Wire.read();
              data_ip[1] = Wire.read();
              data_ip[2] = Wire.read();
              data_ip[3] = Wire.read();
            }
            // byte data_ip[4] = {0, 0, 0, 0};
          }
          break;

        case 3: // Pressure
          {
            if(Wire.available()>1){
              byte high = Wire.read();
              byte low = Wire.read();
              data_pressure = high << 8 + low;
            }
            // short data_pressure = 0;
          }
          break;

        case 4: // Temperature
          {
            if(Wire.available()>1){
              byte high = Wire.read();
              byte low = Wire.read();
              data_temperature = high << 8 + low;
            }
              // short data_temp = 0;
          }
          break;

        case 5: // Tension
          {
            if(Wire.available()>1){
              byte high = Wire.read();
              byte low = Wire.read();
              data_tension = high << 8 + low;
            }
              // short data_tension = 0;
          }
          break;

        case 6: // Mission name
          {
              // data_mission
            data_mission[0] = '\0';
            int i=0;
            while(Wire.available()){
              char val = Wire.read();
              if(i<sizeof(data_mission)){
                data_mission[i] = val;
                i++;
              }
            }
          }
          break;

        case 7: // Waypoint id
          {
            if(Wire.available()){
              data_wp = Wire.read();
            }
            // byte data_wp = 0;
          }
          break;

        case 8: // Nb waypoints
          {
            if(Wire.available()){
              data_wp_max = Wire.read();
            }
            // byte data_wp_max = 0;
          }
          break;

        case 9: // Time
          {
            if(Wire.available()>1){
              data_time[0] = Wire.read();
              data_time[1] = Wire.read();
            }
            // byte data_time[2] = {0,0};
          }
          break;

        case 10: // Time remaining
          {
            if(Wire.available()>1){
              data_time_remain[0] = Wire.read();
              data_time_remain[1] = Wire.read();
            }
            // byte data_time_remain[2] = {0,0};*/
          }
          break;

        default:

          break;
      }

      while(Wire.available()){
        Wire.read();
      }

      print_screen();
    }
  }
}

