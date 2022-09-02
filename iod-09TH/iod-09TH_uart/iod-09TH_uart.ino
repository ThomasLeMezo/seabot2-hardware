
/*
 * 
 * Compiler & board : https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
 * File>Preference>Additional Boards Manager add link above
 * In Tools>Board select "4D Systems gen4 IoD Range"
 * 
 * Add library to folder Documents\Arduino\libraries
 * GFX4DIoD9-master from https://github.com/4dsystems/GFX4DIoD9
 * 
 */

#include <GFX4dIoD9.h>
#define DEBUG false

GFX4dIoD9 gfx = GFX4dIoD9();

int reg_uart = 0;
byte data_ip[4] = {0, 0, 0, 0};
short data_pressure = 9999;
short data_temperature = 999;
short data_hygro = 99;
byte data_tension = 0;
char data_name[16] = "TestNom";
char data_mission[16] = "TestMission";
byte data_wp = 0;
byte data_wp_max = 0;
byte data_time[2] = {0,0};
byte data_time_remain[2] = {0,0};
byte data_status = 0;


short pressure_threshold = 700;
short tension_threshold = 120;
short temperature_threshold = 400;
short hygro_threshold = 70;

bool screen_errase = false;

void setup()
{
  gfx.begin();
  gfx.Cls();
  gfx.ScrollEnable(false);
  gfx.BacklightOn(false);
  gfx.Orientation(LANDSCAPE);
  gfx.SmoothScrollSpeed(5);
  gfx.TextColor(WHITE, BLACK); gfx.Font(1); gfx.TextSize(1);
  gfx.TextWrap(false);
  Serial.begin(115200);
  //Serial.setTimeout(200);

  print_screen();

} // end Setup **do not alter, remove or duplicate this line**

void loop()
{
   parseUART();

  yield(); // Required for ESP
}

void print_screen()
{
  gfx.Cls();

  // Robot Name
  gfx.MoveTo(0, 0);
  gfx.TextColor(WHITE);
  gfx.print(data_name);

  // Time
  gfx.MoveTo(128, 0);
  gfx.TextColor(WHITE);
  char s_time[6];
  sprintf(s_time, "%02u:%02u", data_time[0], data_time[1]);
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

  // Hygro
  gfx.MoveTo(112, 14);
  if(data_hygro < hygro_threshold)
    gfx.TextColor(WHITE);
  else
    gfx.TextColor(RED);
  char s_hygro[4] = "00%";
  sprintf(s_hygro, "%2u %%", data_hygro);
  gfx.print(s_hygro);

  // Pressure
  gfx.MoveTo(0, 28);
  if(data_pressure < pressure_threshold)
    gfx.TextColor(WHITE);
  else
    gfx.TextColor(RED);
  char s_pressure[10] = "XXXX mbar";
  sprintf(s_pressure, "%4u mbar", data_pressure);
  gfx.print(s_pressure);

  // Temperature
  gfx.MoveTo(64, 28);
  if(data_temperature < temperature_threshold)
    gfx.TextColor(GREEN);
  else
    gfx.TextColor(RED);
  char s_temp[8] = "00.0C";
  float temp = data_temperature/10.;
  sprintf(s_temp, "%3.1f \370C", temp);
  gfx.print(s_temp);

  // Tension
  gfx.MoveTo(112, 28);
  if(data_tension > tension_threshold)
    gfx.TextColor(WHITE);
  else
    gfx.TextColor(RED);
  char s_tens[7] = "00.0 V";
  float tension = data_tension/10.;
  sprintf(s_tens, "%2.1f V", tension);
  gfx.print(s_tens);

  // Mission name
  gfx.MoveTo(0, 42);
  gfx.TextColor(WHITE);
  gfx.print(data_mission);

  // Waypoint
  gfx.MoveTo(0, 56);
  gfx.TextColor(WHITE);
  char s_wp[11] = "Wp 256/256";
  sprintf(s_wp, "Wp %u/%u", data_wp, data_wp_max);
  gfx.print(s_wp);

  // Remaining time
  gfx.MoveTo(80, 56);
  gfx.TextColor(WHITE);
  char s_time_remain[7] = "-00:00";
  sprintf(s_time_remain, "-%02u:%02u", data_time_remain[0], data_time_remain[1]);
  gfx.print(s_time_remain);

  // Status
  gfx.MoveTo(0, 67);
  switch(data_status){
    case 0:
      gfx.RectangleFilled(0, 67, 60, 80, RED);
      break;
    case 1:
      gfx.RectangleFilled(0, 67, 60, 80, YELLOW);
      break;
    case 2:
      gfx.RectangleFilled(0, 67, 60, 80, GREEN);
      break;
    default:
      break;  
  }

  // ENSTA B logo
  gfx.MoveTo(70, 67);
  gfx.TextColor(ORANGE);
  gfx.print("ENSTA Bretagne");

  // Debug
  if(DEBUG)
  {
    gfx.MoveTo(0, 67);
    gfx.TextColor(ORANGE);
    gfx.print(reg_uart);
  }

}

void parseUART()
{
  if(Serial.available()){
    reg_uart = (int)Serial.read(); // receive byte as a character
    while (Serial.available() == 0);

    //if(Serial.available()){
      switch(reg_uart){
        case 0: // write to screen
            if(Serial.read()==1){
              print_screen();
            }
          break;
        case 1: // Name of the robot
          {
            size_t nb_bytes = Serial.readBytesUntil('\n', data_name, sizeof(data_name));
          }
          break;
        case 2: // IP
          {
            Serial.readBytes(data_ip, 4);
          }
          break;

        case 3: // Pressure
          {
            byte val[2];
            size_t nb_bytes = Serial.readBytes(val, 2);
            if(nb_bytes == 2)
              data_pressure = (val[1] << 8) + val[0];
          }
          break;

        case 4: // Temperature
          {
            byte val[2];
            size_t nb_bytes = Serial.readBytes(val, 2);
            if(nb_bytes == 2)
              data_temperature = (val[1] << 8) + val[0];
          }
          break;

        case 5: // Tension
          {
            data_tension = Serial.read();
          }
          break;

        case 6: // Hygro
          {
            data_hygro = Serial.read();
          }

        case 7: // Mission name
          {
              // data_mission
            size_t nb_bytes = Serial.readBytesUntil('\n', data_mission, sizeof(data_mission));
          }
          break;

        case 9: // Waypoint id
          {            
            data_wp = Serial.read();
          }
          break;

        case 10: // Nb waypoints
          {
            data_wp_max = Serial.read();
          }
          break;

        case 11: // Time
          {
            Serial.readBytes(data_time, 2);
          }
          break;

        case 12: // Time remaining
          {
            Serial.readBytes(data_time_remain, 2);
          }
          break;

        case 13:
        {
          data_status = Serial.read();
        }

        default:

          break;
      }
  }
}
