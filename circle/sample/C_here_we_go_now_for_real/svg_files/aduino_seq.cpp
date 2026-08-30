//----------------------------------------------------------------------------------------------------------------------------------------------------------
#include <WiFi.h>
#include <OneButton.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <U8g2lib.h>

#define u8g2_wave_width 32
#define u8g2_wave_height 128

#define WATCHDOG_TIMEOUT_SEC 10                           // Watchdog timer timeout in seconds

#define WDT_REG_BASE        0x3FF48000                    // Define register addresses for the watchdog timer

#define WDT_WDTCONFIG0_REG  (WDT_REG_BASE + 0x00)
#define WDT_WDTCONFIG1_REG  (WDT_REG_BASE + 0x04)
#define WDT_WDTCONFIG2_REG  (WDT_REG_BASE + 0x08)
#define WDT_WDTFEED_REG     (WDT_REG_BASE + 0x0C)
//--INPUT&OUTPUT-PINS---------------------------------------------------------------------------------
#define PULSE_OUT_PIN_1      13  // GPIO13 - Puls output
#define PULSE_OUT_PIN_2      12  // GPIO12 - Puls output
#define DAC_OUT_PIN_1        25  // GPIO25 - DAC output
#define DAC_OUT_PIN_2        26  // GPIO26 - DAC output
#define ANALOG_IN_PIN_1      34  // GPIO32 - Analog input scale
#define ANALOG_IN_PIN_2      35  // GPIO33 - Analog input metrum
#define ANALOG_IN_PIN_3      33  // GPIO34 - Analog input morph has to be changed
#define ANALOG_IN_PIN_4      32  // GPIO35 - Analog input random has to be changed
#define BUTTON_PIN           15  // GPIO15 - Button input
#define LED_DATA_PIN          2  // GPIO2 - LED data pin (for 4 LEDs)
#define NUM_LEDS              4
#define SWITCH_CONTROL        4
#define I2C_SDA_PIN          21  // GPIO21 - I2C SDA
#define I2C_SCL_PIN          22  // GPIO22 - I2C SCL
//--VARIABLES-----------------------------------------------------------------------------------------
SemaphoreHandle_t xMutex;
static const uint8_t maxInputs      =   4;     // number of analog inputs
const uint8_t  maxValue             = 140;     // Adjust this value based on your specific requirements             // threshold for the maximum analog value
static const int maxSteps           =  32;     // max number of steps ( may vary )    
static const uint8_t maxMenueLevel  =   3;     // number of menu levels > remarks on menu
static const uint8_t maxTimerMode   =   3;     // number of menu levels > remarks on menu
static const int maxSSIDs           =   8;      // max numbers of WiFi SSIDs to scan for         

hw_timer_t *My_timer                =   NULL; // for timer isr definition setTimer()

static bool lockedFlagSeq1          =   false; // is cv output locked to trigger output ?
static bool lockedFlagSeq2          =   false;

static uint8_t currentMenuLevel     =   0;     // current level of menu
static uint8_t bufferedMenuLevel    =   0;     // buffered menu level

uint8_t currentTimerMode            =   0;
uint8_t TimerModeBuffer             =   0;
static uint8_t analogNoiseTolerance =   1;

static int netWorksFound            =   0;      // callback of "scanforWiFi" in core 0, number of networks found
uint32_t currentMillis1             =   0;
uint32_t previousMillis             =   0;      // buffer for the last puls on the outputs for timing
static uint32_t IntCLKtimer         =   0;      // button press intervall timer start used in getCLK()
static uint32_t IntCLKtimerOld      =   0;      // button press intervall timer buffer used in getCLK()
static uint32_t IntCLKtimerDif      =   0;      // button press intervall difference result used in getCLK()
uint8_t currentStep1                =   0;      // Current step for Sequencer 1
uint8_t currentStep2                =   0;      // Current step for Sequencer 2
static uint16_t bpm                 =   0;
uint8_t intervalIndex               =   0;      // Index to keep track of the current interval in the array
uint8_t mappedValue1                =   0;
uint8_t mappedValue2                =   0;
// uint32_t interval;
static uint32_t interval1           =   0;
static uint32_t interval2           =   0;
uint32_t startTime1                 =   0;
uint32_t startTime2                 =   0;
uint32_t targetTime1                =   0;
uint32_t targetTime2                =   0;
uint8_t stepsBuffer1                =   0;
uint8_t stepsBuffer2                =   0;
uint8_t bpmBuffer                   =   0;

static uint8_t            hasPotMoved[maxInputs];
static uint8_t            analogInputs[maxInputs];
static uint8_t            oldAnalogInputs[maxInputs];
static uint8_t            analogBuffer[maxInputs][maxMenueLevel];
static bool               pickUpFlag[maxInputs][maxMenueLevel]; // = {{false,false,false},{false,false,false},{false,false,false},{false,false,false}};

static uint8_t            parameters[maxInputs][maxMenueLevel];

static uint8_t            patternBuffer[6][maxSteps]; // 1&2 for Trigger, 3&4 for rotation of 1&2, 5&6 for cv // trigger sequence 1

                                            
volatile uint8_t          SSIDsNames[maxSSIDs][maxSteps];  // Define 2D array to store SSID data as uint8_t values
volatile uint8_t          SSIDsLength[maxSSIDs];  // Define 1D array to store number of chars in SSID
volatile uint8_t          SSIDsStrength[maxSSIDs]; // Define 1D array to store strength and order

int                       intervals[4]; // Array to store the last four intervals for bpm

const static uint8_t      scalesLength[16] = {7,7,7,7, 7,7,7,7, 7,8,7,5, 5,8,7,12};  //{8,8,8,8, 8,8,8,8, 8,9,8,6, 6,8,7,13};
                                             
const static uint8_t      scales[16][40] =
//   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39 
 {{  0,  2,  4,  5,  7,  9, 11, 12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67},
  {  0,  2,  3,  5,  7,  8, 10, 12, 14, 15, 17, 19, 20, 22, 24, 26, 27, 29, 31, 32, 34, 36, 38, 39, 41, 43, 44, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65, 67},
  {  0,  2,  4,  5,  7,  9, 11, 12, 14, 16, 17, 19, 21, 23, 24, 26, 28, 29, 31, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67},
  {  0,  2,  3,  5,  7,  9, 10, 12, 14, 15, 17, 19, 21, 22, 24, 26, 27, 29, 31, 33, 34, 36, 38, 39, 41, 43, 45, 46, 48, 50, 51, 53, 55, 57, 58, 60, 62, 63, 65, 67},

  {  0,  1,  3,  5,  7,  8, 10, 12, 13, 15, 17, 19, 20, 22, 24, 25, 27, 29, 31, 32, 34, 36, 37, 39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 61, 63, 65, 67},
  {  0,  2,  4,  6,  7,  9, 11, 12, 14, 16, 18, 19, 21, 23, 24, 26, 28, 30, 31, 33, 35, 36, 38, 40, 42, 43, 45, 47, 48, 50, 52, 54, 55, 57, 59, 60, 62, 64, 66, 67},
  {  0,  2,  4,  5,  7,  9, 10, 12, 14, 16, 17, 19, 21, 22, 24, 26, 28, 29, 31, 33, 34, 36, 38, 40, 41, 43, 45, 46, 48, 50, 52, 53, 55, 57, 58, 60, 62, 64, 65, 67},
  {  0,  1,  3,  5,  7,  8, 10, 12, 13, 15, 17, 19, 20, 22, 24, 25, 27, 29, 31, 32, 34, 36, 37, 39, 41, 43, 44, 46, 48, 49, 51, 53, 55, 56, 58, 60, 61, 63, 65, 67},

  {  0,  2,  3,  5,  7,  8, 10, 12, 14, 15, 17, 19, 20, 22, 24, 26, 27, 29, 31, 32, 34, 36, 38, 39, 41, 43, 44, 46, 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65, 67},
  {  0,  1,  3,  4,  6,  7,  9, 10, 12, 13, 15, 16, 18, 19, 21, 22, 24, 25, 27, 28, 30, 31, 33, 34, 36, 37, 39, 40, 42, 43, 45, 46, 48, 49, 51, 52, 54, 55, 57, 58},
  {  0,  3,  5,  6,  7,  8, 10, 12, 15, 17, 18, 19, 20, 22, 24, 27, 29, 30, 31, 32, 34, 36, 39, 41, 42, 43, 44, 46, 48, 51, 53, 54, 55, 56, 58, 60, 63, 65, 66, 67},
  {  0,  2,  4,  7,  9, 12, 14, 16, 19, 21, 24, 26, 28, 31, 33, 36, 38, 40, 43, 45, 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74, 76, 79, 81, 84, 86, 88, 91, 93},
  {  0,  3,  5,  7, 10, 12, 15, 17, 19, 22, 24, 27, 29, 31, 34, 36, 39, 41, 43, 46, 48, 51, 53, 55, 58, 60, 63, 65, 67, 70, 72, 75, 77, 79, 82, 84, 87, 89, 91, 94},
  {  0,  1,  4,  5,  7,  8, 11, 12, 13, 16, 17, 19, 20, 23, 24, 25, 28, 29, 31, 32, 35, 36, 37, 40, 41, 43, 44, 47, 48, 49, 52, 53, 55, 56, 59, 60, 61, 64, 65, 67},
  {  0,  2,  4,  6,  8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78},
  {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0}};

const static uint8_t      oneVperOctaveTable[40] =
//   c   c+    d   d+    e    f  f+    g   g+    a   a+    b
  {  0,   6,  13,  19,  26,  32,  39,  45,  52,  58,  65,  71,   // octave 1
    78,  84,  91,  97, 103, 110, 116, 123, 129, 136, 142, 149,   // octave 2
   155, 162, 168, 175, 181, 187, 194, 200, 207, 213, 220, 226,   // octave 3
   233, 239, 246, 252};                                          // 4 halftone steps of octave 4

const char* noteNames[40] = 
//    0       1     2       3     4     5       6     7       8     9      10    11
  {
   "c1", "cis1", "d1", "dis1", "e1", "f1", "fis1", "g1", "gis1", "a1", "ais1", "b1"
   "c2", "cis2", "d2", "dis2", "e2", "f2", "fis2", "g2", "gis2", "a2", "ais2", "b2"
   "c3", "cis3", "d3", "dis3", "e3", "f3", "fis3", "g3", "gis3", "a3", "ais3", "b3"
   "c4", "cis4", "d4", "dis4"
   };

static unsigned char u8g2_wave_bits[] PROGMEM= 
  {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x38, 0x00,
   0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x20, 0x00, 0xfc,
   0x00, 0x30, 0xc0, 0xff, 0x00, 0x18, 0xf0, 0x00, 0x00, 0x08, 0x38, 0x00, 0x00, 0x0c, 0x0c, 0x00, 0x00, 0x0c, 0x06, 0x3e, 0x00, 0x04, 0x03, 0xff,
   0x00, 0x02, 0x23, 0x80, 0x00, 0x82, 0x31, 0x00, 0x00, 0x83, 0x10, 0x7c, 0x00, 0x80, 0x08, 0xff, 0x00, 0x81, 0x88, 0x01, 0x00, 0x41, 0x84, 0x00,
   0x00, 0x41, 0xc4, 0x00, 0x00, 0x41, 0x44, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x80, 0x48, 0x60, 0x00, 0x80, 0x49, 0x60, 0x00, 0x80, 0x19, 0x20,
   0x00, 0x01, 0xd3, 0x20, 0x00, 0x01, 0xb2, 0x61, 0x00, 0x01, 0xc6, 0xc7, 0x00, 0x03, 0x88, 0xff, 0x00, 0x02, 0x18, 0xfe, 0x00, 0x02, 0x30, 0x00,
   0x00, 0x04, 0xe0, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x08, 0x00, 0xfc, 0x00, 0x18, 0x00, 0xc0, 0x00, 0x10, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00,
   0x00, 0x60, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x9d, 0x80, 0x03, 0x00, 0xf8, 0x05, 0x07, 0x00, 0x00, 0xfd, 0x1c, 0x00, 0x00, 0x80, 0x7f, 0x00,
   0x00, 0x80, 0xff, 0x00, 0x00, 0xf0, 0x1f, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00,
   0x3c, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x7f, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0xc0, 0x01, 0x00,
   0x00, 0xf0, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x02, 0x00, 0x07, 0xf8, 0x3f, 0x80, 0x03, 0x1c, 0xfc, 0x80, 0x01, 0x07, 0x80,
   0xc0, 0x80, 0x03, 0x00, 0x60, 0xc0, 0x01, 0x00, 0x70, 0xe0, 0x00, 0x00, 0x30, 0x60, 0xe0, 0x1f, 0x10, 0x30, 0xf8, 0xf0, 0x18, 0x10, 0x1c, 0xfc,
   0x18, 0x18, 0x06, 0xff, 0x08, 0x18, 0x03, 0x8b, 0x0c, 0x8c, 0x81, 0x0d, 0x0c, 0x8c, 0x81, 0x0d, 0x0c, 0x8c, 0xc0, 0x38, 0x0c, 0x8c, 0xc0, 0xf8,
   0x04, 0x88, 0x80, 0x40, 0x0c, 0x88, 0x80, 0x01, 0x00, 0x88, 0x80, 0x01, 0x08, 0x98, 0x01, 0x83, 0x00, 0x10, 0x01, 0xff, 0x10, 0x30, 0x03, 0xfc,
   0x00, 0x60, 0x06, 0xc0, 0x20, 0xe0, 0x0c, 0x70, 0x20, 0x80, 0x79, 0x3e, 0x00, 0x00, 0xe3, 0x8f, 0x80, 0x00, 0x0c, 0xb0, 0x80, 0x01, 0xf8, 0x3f,
   0x00, 0x03, 0x80, 0x9f, 0x00, 0x0e, 0x00, 0xc0, 0x00, 0x1c, 0x00, 0x70, 0x00, 0x78, 0x00, 0x0e, 0x00, 0xc0, 0xfe, 0x01, 0x00, 0x00, 0xfe, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00,
   0x80, 0x0f, 0x00, 0xe0, 0x00, 0xfc, 0x01, 0xf0, 0x00, 0xe0, 0xf7, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00 
   };
//----------------------------------------------------------------------------------------------------------------------------------------------------------
OneButton Button(BUTTON_PIN, true, true);  // true=Button is active LOW,true=Enable int. pull-up resistor

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R1, /* reset=*/U8X8_PIN_NONE, SCL, SDA);
//----------------------------------------------------------------------------------------------------------------------------------------------------------
// Function to reset the watchdog timer
//----------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------------
// loop core 0 aka scan for ssids
//----------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------------
// calculate the bpm from 4 intervals version 2 with threshold and reset of the index if the input isnt accurate ( wonkyness )
//----------------------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------------------
// code handle button functions
//----------------------------------------------------------------------------------------------------------------------------------------------------------


void reset_watchdog_timer() 
{
    REG_SET_BIT(WDT_WDTCONFIG0_REG, 0);    // Disable the watchdog timer by clearing the WDTEN bit in WDTCONFIG0
    REG_WRITE(WDT_WDTCONFIG1_REG, (WATCHDOG_TIMEOUT_SEC << 12));    // Configure the timeout value in WDTCONFIG1
    REG_SET_BIT(WDT_WDTCONFIG0_REG, 0);    // Enable the watchdog timer by setting the WDTEN bit in WDTCONFIG0
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
// initialize WIFI
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void manageWiFi(bool activate) // manageWiFi(true) = Activate Wi-Fi (false) = Deactivate Wi-Fi
{
  if (activate) 
    {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    } 
  else 
    {
    WiFi.mode(WIFI_OFF);
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void storeSSIDsNameAndLength(int i) 
{
  String ssid = WiFi.SSID(i);
  SSIDsLength[i] = 0; // Initialize the character count to 0 

  for (int j = 0; j < 32; ++j) 
  {
    if (j < ssid.length()) 
      {
      SSIDsNames[i][j] = static_cast<uint8_t>(ssid[j]);
      SSIDsLength[i]++;
      } 
    else 
      {
      SSIDsNames[i][j] = 0;  // Set to 0 when no more characters are present
      }
  }
  SSIDsStrength[i] = abs(static_cast<int>(WiFi.RSSI(i)));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void core0TaskToScanForSSIDs(void *param) 
{
  while (1) 
  {
    netWorksFound = WiFi.scanNetworks(false, true, true, 80);    // Perform WiFi scan

    if (netWorksFound > 0) 
    {
      xSemaphoreTake(xMutex, portMAX_DELAY);      // Take the mutex before modifying shared resources in the loop

      for (int i = 0; i < min(netWorksFound, maxSSIDs); ++i) 
      {

        storeSSIDsNameAndLength(i);
      }
      xSemaphoreGive(xMutex); // Give the mutex after modifying shared resources in the loop
    }
    reset_watchdog_timer();
  }
}

static void handleClick()  // action for pressing the button ones
{
  switch (currentMenuLevel)
  {
  case 0:
  Button.setClickMs(100);
  getTabInBPM();
  break;

  case 1:
  Button.setClickMs(200);
  lockedFlagSeq1 = !lockedFlagSeq1;
  break;

  case 2:
  Button.setClickMs(200);
  lockedFlagSeq2 = !lockedFlagSeq2;
  break;
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
static void handleLongClick()  // action for pressing the button long
{
  bufferedMenuLevel = currentMenuLevel;
  currentMenuLevel++;
  currentMenuLevel=currentMenuLevel%maxMenueLevel;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
static void handleDoubleClick()  // action for pressing the button twice
{
  TimerModeBuffer = currentTimerMode;
  currentTimerMode++;

  currentTimerMode = currentTimerMode%maxTimerMode;
 // currentStep1 = 0;
 // currentStep2 = 0;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void readAndBufferAnalogInputs() // Define a constant for the maximum expected value
{
  // Read analog inputs and store to input array
  analogInputs[0] = constrain((analogRead(ANALOG_IN_PIN_1) + oldAnalogInputs[0]) / 2, 0, maxValue);
  analogInputs[1] = constrain((analogRead(ANALOG_IN_PIN_2) + oldAnalogInputs[1]) / 2, 0, maxValue);
  analogInputs[2] = constrain((analogRead(ANALOG_IN_PIN_3) + oldAnalogInputs[2]) / 2, 0, maxValue);
  analogInputs[3] = constrain((analogRead(ANALOG_IN_PIN_4) + oldAnalogInputs[3]) / 2, 0, maxValue);

  oldAnalogInputs[0] = analogInputs[0];
  oldAnalogInputs[1] = analogInputs[1];
  oldAnalogInputs[2] = analogInputs[2];
  oldAnalogInputs[3] = analogInputs[3];

  if (currentMenuLevel != bufferedMenuLevel)            // has menu changed?// yes, store analog input to buffer
    {
    if ( pickUpFlag[0][bufferedMenuLevel] == true )
      {    
      analogBuffer[0][bufferedMenuLevel] = analogInputs[0];
      }
    if ( pickUpFlag[1][bufferedMenuLevel] == true )
      {    
      analogBuffer[1][bufferedMenuLevel] = analogInputs[1];
      }
    if ( pickUpFlag[2][bufferedMenuLevel] == true )
      {    
      analogBuffer[2][bufferedMenuLevel] = analogInputs[2];
      }
    if ( pickUpFlag[3][bufferedMenuLevel] == true )
      {    
      analogBuffer[3][bufferedMenuLevel] = analogInputs[3];
      }
      pickUpFlag[0][bufferedMenuLevel] =  false; // set "pickupflag" to false:
      pickUpFlag[1][bufferedMenuLevel] =  false; // false means the input will not be used till
      pickUpFlag[2][bufferedMenuLevel] =  false; // true (current input is equal to buffered input)
      pickUpFlag[3][bufferedMenuLevel] =  false;
      }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void mapInputToParameter() 
{
  switch (currentMenuLevel)
  {
  case 0:
  if (abs(analogInputs[0] - analogBuffer[0][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[0][currentMenuLevel] == true) 
    {
    pickUpFlag[0][currentMenuLevel] = true;
    parameters[0][currentMenuLevel] = map(analogInputs[0],0,maxValue,0, parameters[0][1]);                                          // puls1 parameters[0][0]
    }
  if (abs(analogInputs[1] - analogBuffer[1][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[1][currentMenuLevel] == true) 
    {
    pickUpFlag[1][currentMenuLevel] = true;
    parameters[1][currentMenuLevel] = map(analogInputs[1],0,maxValue,0, parameters[0][2]);                                          // puls2 parameters[1][0]
    }
  if (abs(analogInputs[2] - analogBuffer[2][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[2][currentMenuLevel] == true) 
    {
    pickUpFlag[2][currentMenuLevel] = true;
    parameters[2][currentMenuLevel] = map(analogInputs[2],0,maxValue,0, parameters[0][1]);                                          // rot 1 parameters[2][0]
    }
  if (abs(analogInputs[3] - analogBuffer[3][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[3][currentMenuLevel] == true) 
    {
    pickUpFlag[3][currentMenuLevel] = true;
    parameters[3][currentMenuLevel] = map(analogInputs[3],0,maxValue,0, parameters[0][2]);                                          // rot 2 parameters[3][0]
    } 
  break;   
  case 1: // menu for sequencer one
  if (abs(analogInputs[0] - analogBuffer[0][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[0][currentMenuLevel] == true) 
    {
    pickUpFlag[0][currentMenuLevel] = true;
    parameters[0][currentMenuLevel] = map(analogInputs[0],0,maxValue,2, maxSteps);                                                  // steps 1 parameters[0][1]
    //stepsBuffer1 = parameters[0][currentMenuLevel];
    }
  if (abs(analogInputs[1] - analogBuffer[1][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[1][currentMenuLevel] == true) 
    {
    pickUpFlag[1][currentMenuLevel] = true;
    //mappedValue1 = map(analogInputs[1], 0, maxValue, 0, 3);
    parameters[1][currentMenuLevel] =  map(analogInputs[1], 0, maxValue, 0, 3);                                                     // range 1 parameters[1][1]
    }
  if (abs(analogInputs[2] - analogBuffer[2][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[2][currentMenuLevel] == true) 
    {
    pickUpFlag[2][currentMenuLevel] = true;
    parameters[2][currentMenuLevel] = map(analogInputs[2],0,maxValue,0, 15);                                                        // scale 1 parameters[2][1]
    }
  if (abs(analogInputs[3] - analogBuffer[3][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[3][currentMenuLevel] == true)
    {
    pickUpFlag[3][currentMenuLevel] = true;
    parameters[3][currentMenuLevel] = map(analogInputs[3],0,maxValue,1, 240);                                                       // speed parameters[3][1]

    }
  break;   
  case 2: // menu for sequencer one
  if (abs(analogInputs[0] - analogBuffer[0][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[0][currentMenuLevel] == true) 
    {
    pickUpFlag[0][currentMenuLevel] = true;
    parameters[0][currentMenuLevel] = map(analogInputs[0],0,maxValue,2, maxSteps);                                                  // steps 2 parameters[0][2]
    //stepsBuffer2 = parameters[0][currentMenuLevel];
    }
  if (abs(analogInputs[1] - analogBuffer[1][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[1][currentMenuLevel] == true) 
    {
    pickUpFlag[1][currentMenuLevel] = true;
    //mappedValue2 = map(analogInputs[1], 0, maxValue, 0, 3);
    parameters[1][currentMenuLevel] =  map(analogInputs[1], 0, maxValue, 0, 3);                                                     // range 2 parameters[1][2]
    }
  if (abs(analogInputs[2] - analogBuffer[2][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[2][currentMenuLevel] == true) 
    {
    pickUpFlag[2][currentMenuLevel] = true;
    parameters[2][currentMenuLevel] = map(analogInputs[2],0,maxValue,0, 15);                                                      // scale 1 parameters[2][2]
    }
  if (abs(analogInputs[3] - analogBuffer[3][currentMenuLevel]) <= analogNoiseTolerance || pickUpFlag[3][currentMenuLevel] == true)
    {
    pickUpFlag[3][currentMenuLevel] = true;
    //parameters[3][currentMenuLevel] = map(analogInputs[3],0,maxValue,3, 255);                                                   // speed parameters[3][2]
    }
  break;    
  default:
  break;
  }
}  

void showPattern(uint8_t numLines, uint8_t bufferDim, uint8_t pointer, uint8_t lockflag, uint8_t Xoffset, uint8_t Yoffset, uint8_t mirror) 
{  
    numLines = constrain(numLines, 2, 32); // Ensure numLines is within the valid range
 
    uint8_t startY1 = Yoffset; // Calculate the y-coordinates of the boundaries relative to the given (Xoffset, Yoffset)
    uint8_t endY1 = Yoffset + 95;

    uint8_t Xstart1 = 4;
    uint8_t width1 = 4;
    uint8_t Xstart2 = 10;
    uint8_t width2 = 3;
    uint8_t Xcircle = Xstart1;

    if (mirror == 1) 
    {
        Xstart1 = 15 - (Xstart1 + width1);
        width1 = 4;
        Xstart2 = 15 - (Xstart2 + width2);
        width2 = 3;
        Xcircle = 15 - Xcircle;
    }
    for (uint8_t i = 0; i < numLines; ++i)  
    {
        float position = static_cast<float>(i) / (numLines - 1);  
        uint8_t y1 = startY1 + position * (endY1 - startY1);  
    
        u8g2.drawHLine(Xoffset + Xstart1, y1, width1);        // Draw the horizontal line for the first column relative to (Xoffset, Yoffset)    
    }
    for (uint8_t i = 0; i < numLines; ++i)    // Separate for-loop for checking conditions and drawing the circle
      {
      float position = static_cast<float>(i) / (numLines - 1);  
      uint8_t y1 = startY1 + position * (endY1 - startY1);  

      if (patternBuffer[bufferDim][i] == 1)// Check if the line in the second column should be drawn based on the global patternBuffer
        {
        u8g2.drawHLine(Xoffset + Xstart2, y1, width2);
        }
      if (pointer == i && lockflag == 0)        // Check if a disc should be drawn at the start position of the actual line in the first column
        {
        u8g2.drawCircle(Xoffset + Xcircle, y1, 2);  // Adjust the radius as needed
        }
      else if (pointer == i && patternBuffer[bufferDim][i] == 1 && lockflag == 1)
        {
        u8g2.drawCircle(Xoffset + Xcircle, y1, 2);  // Adjust the radius as needed
        }
      }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void outputLED()  // give out the analog input via LED
{
  if (analogInputs[0] > 139) 
    {
    strip.setPixelColor(0, 0, 0, 0);
    } 
  else
    {
    strip.setPixelColor(0, analogInputs[0] , 0, analogInputs[0] );
    }
  if (analogInputs[1] > 139) 
    {
    strip.setPixelColor(1, 0, 0, 0);
    } 
  else
    {
    strip.setPixelColor(1, analogInputs[1] , 0, 0 );
    }
  if (analogInputs[2] > 139) 
    {
    strip.setPixelColor(2, 0, 0, 0);
    } 
  else
    {
    strip.setPixelColor(2, 0, analogInputs[2] , 0 );
    }
  if (analogInputs[3] > 139) 
    {
  strip.setPixelColor(3, 0, 0, 0);
    } 
  else
    {
    strip.setPixelColor(3, 0 , 0, analogInputs[3] );
    }
  strip.show();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void showScale(uint8_t Xoffset, uint8_t Yoffset, uint8_t scale, uint8_t mirror) 
{
  uint8_t XposScales[12] = {5, 11, 5, 11, 5, 5, 11, 5, 11, 5, 11, 5};
  uint8_t YposScales[12] = {0, 4,  8, 12, 16, 24, 28, 32, 36, 40, 44, 48};
  for (uint8_t i = 0; i < 12; i++) 
    {
    uint8_t xPosition = (mirror == 1) ? (15 - XposScales[i]) : XposScales[i];
    uint8_t yPosition = YposScales[i] + Yoffset;

    for (uint8_t j = 0; j < scalesLength[scale]; j++)
      {
      if (i == scales[scale][j]) 
      {
      u8g2.drawDisc(xPosition + Xoffset, yPosition, 3);
      } 
      else 
      {
      u8g2.drawCircle(xPosition + Xoffset, yPosition, 3);
      }
    }
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void showParameter(uint8_t Xoffset, uint8_t Yoffset, uint8_t Value1, uint8_t Value2) 
{
  u8g2.setFont(u8g2_font_micro_tr);
  u8g2.setFontDirection(0);
  u8g2.drawRFrame(Xoffset, Yoffset, 14, 16, 3);
  u8g2.setCursor(Xoffset + 3, Yoffset + 7);
  u8g2.print(String(Value1, DEC));
  u8g2.setCursor(Xoffset + 3, Yoffset + 13);
  u8g2.print(String(Value2, DEC));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void showBPM(uint8_t Xoffset,uint8_t Yoffset,uint8_t Value)
{
  u8g2.setFont(u8g2_font_4x6_tr);
  u8g2.drawRFrame(Xoffset+3, Yoffset,9,31,3);
  u8g2.setFontDirection(1);
  u8g2.setCursor(Xoffset + 4, Yoffset + 3);
  u8g2.print(String(Value, DEC));
  u8g2.setCursor(Xoffset + 4, Yoffset + 16);
  u8g2.print("BPM");
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void showRange(uint8_t Xoffset,uint8_t Yoffset,uint8_t parameter)
{
  u8g2.setFont(u8g2_font_micro_tr);
  u8g2.setFontDirection(0);
  u8g2.setCursor(Xoffset+ 4, Yoffset +7);
  u8g2.print("x");
  u8g2.setCursor(Xoffset + 8, Yoffset + 7);
  u8g2.print(String(parameter, DEC));
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void showMode(uint8_t Xoffset, uint8_t Yoffset,uint8_t mode)
{
  u8g2.setFont(u8g2_font_micro_tr);
  u8g2.setFontDirection(1);
  if(lockedFlagSeq1==true)
    {
    u8g2.setCursor(Xoffset+ 4, Yoffset +0);
    u8g2.print("[");
    }
  if(lockedFlagSeq2==true)
    {
    u8g2.setCursor(Xoffset+ 4, Yoffset +15);
    u8g2.print("]");
    }
  switch(currentTimerMode)
    {
    case 0:
      u8g2.drawCircle(Xoffset+5,Yoffset+8,3);
      u8g2.drawCircle(Xoffset+9,Yoffset+8,3);
    break;
    case 1:
      u8g2.drawDisc(Xoffset+5,Yoffset+8,3);
      u8g2.drawCircle(Xoffset+9,Yoffset+8,3);
    break;
    case 2:
      u8g2.drawCircle(Xoffset+5,Yoffset+8,3);
      u8g2.drawDisc(Xoffset+9,Yoffset+8,3);
    break;
    default:
    break;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void outputOLEDdebug()  //debug the OLED and show the Input Data
{
  u8g2.clearBuffer();
  u8g2.drawRFrame(0, 0,32,128,8);

  switch (currentMenuLevel) 
  {
    case 0:
      showPattern(parameters[0][1], 2, currentStep1, lockedFlagSeq1,  1,  7,  0);
      showPattern(parameters[0][2], 3, currentStep2, lockedFlagSeq2, 16, 23,  1);
      showParameter(  2,110, parameters[0][1],parameters[0][0]);  //steps 1
      showParameter( 16,  2, parameters[0][2],parameters[1][0]);  //steps 2
    break;

    case 1:  //seq1
      showPattern(parameters[0][1], 2, currentStep1, lockedFlagSeq1,  1,  7,  0);
      showParameter(  2,110, parameters[0][1],parameters[0][0]);  //steps 1
      showScale(15, 7,  parameters[2][1], 0); 
      showMode(16,71,currentTimerMode);
      showRange(16,57,parameters[1][1]);//showRange(16,57,mappedValue1);
      showBPM(16,95,parameters[3][1]);
    break;

    case 2:  //seq2
      showPattern(parameters[0][2], 3, currentStep2, lockedFlagSeq2, 16, 23,  1);
      showParameter( 16,  2, parameters[0][2],parameters[1][0]);  //steps 2
      showScale(1,69,parameters[2][2],1); 
      showMode(1,41,currentTimerMode);
      showRange(2,57,parameters[1][2]);//showRange(2,57,mappedValue2);
      showBPM(1,2,parameters[3][1]);
    break;
    default:
    break;
  }
  u8g2.sendBuffer();
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void startScreen()
{
  u8g2.clearBuffer();
  u8g2.drawXBM( 0, 1, u8g2_wave_width, u8g2_wave_height, u8g2_wave_bits);
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setFontDirection(1);
  u8g2.setCursor(12, 7);
  u8g2.drawBox(12,7,128,10);
  u8g2.print("DieAnthropologische");
  u8g2.setCursor(1, 64);
  u8g2.drawBox(1,64,64,10);
  u8g2.print("Konstante");
  u8g2.sendBuffer();
  delay(8000);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------


void debugNew(){
  Serial.print(bpmBuffer);
  Serial.print(":");
  Serial.print(parameters[3][1]);
  Serial.print("/");
  Serial.print(stepsBuffer1);
  Serial.print(":");
  Serial.print(parameters[0][1]);
  Serial.print(":");
  Serial.print(currentStep1);
  Serial.print(":");
  Serial.print(interval1);
  Serial.print("/");
  Serial.print(stepsBuffer2);
  Serial.print(":");
  Serial.print(parameters[0][2]);
  Serial.print(":");
  Serial.print(currentStep2);
  Serial.print(":");
  Serial.print(interval2);
  Serial.print("/");
  Serial.print(TimerModeBuffer);
  Serial.print(":");
  Serial.println(currentTimerMode);
}
void debugOutput()
{  
  Serial.print(currentMenuLevel);
    Serial.print("/");
  Serial.println(bufferedMenuLevel);
  Serial.print(analogInputs[0]);
    Serial.print("/");
  Serial.print(analogInputs[1]);
    Serial.print("/");
  Serial.print(analogInputs[2]);
    Serial.print("/");
  Serial.println(analogInputs[3]);

  Serial.print(analogBuffer[0][currentMenuLevel]);
    Serial.print("/");
  Serial.print(analogBuffer[1][currentMenuLevel]);
    Serial.print("/");
  Serial.print(analogBuffer[2][currentMenuLevel]);
    Serial.print("/");
  Serial.print(analogBuffer[3][currentMenuLevel]);
    Serial.print("  /  ");
  Serial.print(pickUpFlag[0][currentMenuLevel]);
    Serial.print("/");
  Serial.print(pickUpFlag[1][currentMenuLevel]);
    Serial.print("/");
  Serial.print(pickUpFlag[2][currentMenuLevel]);
    Serial.print("/");
  Serial.println(pickUpFlag[3][currentMenuLevel]);

  Serial.print(analogBuffer[0][bufferedMenuLevel]);
    Serial.print("/");
  Serial.print(analogBuffer[1][bufferedMenuLevel]);
    Serial.print("/");
  Serial.print(analogBuffer[2][bufferedMenuLevel]);
    Serial.print("/");
  Serial.print(analogBuffer[3][bufferedMenuLevel]);
    Serial.print("  /  ");
  Serial.print(pickUpFlag[0][bufferedMenuLevel]);
    Serial.print("/");
  Serial.print(pickUpFlag[1][bufferedMenuLevel]);
    Serial.print("/");
  Serial.print(pickUpFlag[2][bufferedMenuLevel]);
    Serial.print("/");
  Serial.println(pickUpFlag[3][bufferedMenuLevel]);

  Serial.print(parameters[0][currentMenuLevel]);
    Serial.print("/"); 
  Serial.print(parameters[1][currentMenuLevel]);
    Serial.print("/");
  Serial.print(parameters[2][currentMenuLevel]);
    Serial.print("/");
  Serial.println(parameters[3][currentMenuLevel]);
 
  Serial.print(parameters[0][bufferedMenuLevel]);
    Serial.print("/"); 
  Serial.print(parameters[1][bufferedMenuLevel]);
    Serial.print("/");
  Serial.print(parameters[2][bufferedMenuLevel]);
    Serial.print("/");
  Serial.println(parameters[3][bufferedMenuLevel]);
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void debugStoredSSIDs() 
{
  for (int i = 0; i < maxSSIDs; ++i) 
  {
    Serial.print(" : ");
    for (int j = 0; j < 32; ++j) 
      {
      Serial.print(SSIDsNames[i][j], HEX);
      Serial.print(","); // Separate the values with a space
      }
    Serial.println();

  Serial.print(SSIDsLength[i]);
  Serial.print("/");
  Serial.println(SSIDsStrength[i]);
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void debugDisplaySEQ() 
{
  for (int i = 0; i < 2; ++i) 
    {
    Serial.print("/seq ");
    Serial.print(i);
    Serial.print(": ");
    for (int j = 0; j < 32; ++j) 
      {
      Serial.print(patternBuffer[i+4][j], HEX);
      Serial.print(","); // Separate the values with a space
      }
    Serial.println();
  }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void debugTimer()
{
  Serial.print(parameters[0][1]);
  Serial.print("/");
  Serial.print(parameters[0][2]);
  Serial.print(":");
  Serial.print(currentStep1);
  Serial.print("/");
  Serial.print(currentStep2);
  Serial.print(":");
  Serial.print(startTime1);
  Serial.print("/");
  Serial.print(startTime2);
  Serial.print(":");
  Serial.print(targetTime1);
  Serial.print("/");
  Serial.print(targetTime2);
  Serial.print(":");
  Serial.print(interval1);
  Serial.print("/");
  Serial.print(interval2);
  Serial.println("");
}

void getTabInBPM() 
{
  IntCLKtimer = millis();
  IntCLKtimerDif = IntCLKtimer - IntCLKtimerOld;

  intervals[intervalIndex] = IntCLKtimerDif;    // Store the current interval in the array

  intervalIndex = (intervalIndex + 1) % 4;    // Move to the next index in the array

  IntCLKtimerOld = IntCLKtimer;
  
  if (intervalIndex == 0) // If we have collected four intervals, calculate and use BPM 
    {
    const uint16_t threshold = 500; // Adjust the threshold value as needed

    if (
        abs(intervals[1] - intervals[2]) <= threshold &&
        abs(intervals[2] - intervals[3]) <= threshold && 
       (intervals[1] > 0 || intervals[2] > 0 || intervals[3] > 0))
      {
      float averageInterval = (intervals[1] + intervals[2] + intervals[3]) / 3.0;
      parameters[3][1] = uint8_t(60000.0 / averageInterval);

    //  currentStep1=0;
    //  currentStep2=0;
      intervalIndex = 0;
      analogBuffer[3][1] = bpm;
      Serial.println(parameters[3][1]);
      } 
    else 
      {
      intervals[0] = 0;
      intervals[1] = 0;
      intervals[2] = 0;
      intervals[3] = 0;

      intervalIndex = 0;

      Serial.println("Button taps deviate too much. Resetting intervalse.");
      }
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
//void calculateInterval( uint8_t steps1, uint8_t steps2, uint32_t interval1, uint32_t interval2, uint8_t mode)
void calculateInterval( uint8_t steps1, uint8_t steps2, uint8_t mode)
{
  if( TimerModeBuffer != currentTimerMode  || stepsBuffer1 != parameters[0][1] || stepsBuffer2 != parameters[0][2] ) // || bpmBuffer!= parameters[3][1]
    {
    uint32_t BPMinMs = round(60000.0 / parameters[3][1]); // 60000 milliseconds in a minute
    double modifiedInterval =  0;
    switch(mode)
      {
      case 0:
        interval1 = BPMinMs;
        interval2 = BPMinMs;
      break;
      case 1:
        interval1 = BPMinMs;
        modifiedInterval = static_cast<double>(BPMinMs*steps1)/steps2;
        interval2 = round(modifiedInterval);
      break;
      case 2:
        modifiedInterval = static_cast<double>(BPMinMs*steps2)/steps1;
        interval1 = round(modifiedInterval);
        interval2 = BPMinMs;
      break;
      }
      Serial.println("has changed");
      currentStep1 = 0;
      currentStep2 = 0;
      targetTime1 = millis() + interval1;
      targetTime2 = millis() + interval2;
      }
    bpmBuffer = parameters[3][1];
    stepsBuffer1 = parameters[0][1];
    stepsBuffer2 = parameters[0][2];
    TimerModeBuffer = currentTimerMode;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
static void TimerCheck() 
{
  static uint32_t buttonPressedStartTime = 0;
  static uint32_t buttonReleasedStartTime = 0;

  uint32_t currentTime = millis();

  buttonPressedStartTime = Button.state() != 0 ? buttonPressedStartTime : currentTime;
  buttonReleasedStartTime = Button.state() == 0 ? buttonReleasedStartTime : currentTime;

  if  ( abs(analogInputs[0] - hasPotMoved[0]) >= analogNoiseTolerance ||
        abs(analogInputs[1] - hasPotMoved[1]) >= analogNoiseTolerance ||
        abs(analogInputs[2] - hasPotMoved[2]) >= analogNoiseTolerance ||
        abs(analogInputs[3] - hasPotMoved[3]) >= analogNoiseTolerance ) 
    {
    hasPotMoved[0] = analogInputs[0];
    hasPotMoved[1] = analogInputs[1];
    hasPotMoved[2] = analogInputs[2];
    hasPotMoved[3] = analogInputs[3];

    buttonReleasedStartTime = currentTime;
    }
  if (currentTime - buttonReleasedStartTime >= 4000) 
    {
    currentMenuLevel = 0;
    }
  if (currentTime - buttonPressedStartTime >= 4000) 
    {
    currentMenuLevel = 0;
    }
}

void generateSequence(uint8_t& pls,uint8_t& stp, uint8_t& rot, uint8_t& rng, uint8_t& pnt, uint8_t dSeq, 
                      uint8_t dRot, uint8_t dCV, uint8_t scl, uint8_t flg,uint8_t TPIN, uint8_t APIN)
{
  if  ( pls > stp )
    { 
    pls = stp;
    }
  for (uint8_t i = 0; i < stp; i++)                                       // Calculate and store the original pattern for Sequencer 1
    {
    patternBuffer[dSeq][i] = 0;                                           // patternBuffer[0] is the array position of sequence 1
    }
  for (uint8_t i = 0; i < pls; i++) 
    {
    patternBuffer[dSeq][(i * stp) / pls] = 1;
    } 
  for (uint8_t i = 0; i < stp; i++)                                       // Rotate the pattern based on the rot value for Sequencer 1
    {
    uint8_t rotatedIndex = (i + rot) % stp;
    patternBuffer[dRot][rotatedIndex] = patternBuffer[dSeq][i];           // patternBuffer[2] is the array position of rotated sequence 1
    }
  digitalWrite(TPIN, patternBuffer[dRot][(pnt + 1) % stp]);               // Output for Sequencer 1

  if (flg == true && patternBuffer[dRot][(pnt + 1) % stp] == 1)
    {
    uint8_t bufferIndex = patternBuffer[dCV][(pnt + 1) % stp];
    uint8_t scaleValue = scales[scl][bufferIndex];
  //uint8_t rangeValue =  rng + 1; // important to change the mapping function!!
    uint8_t valueToWrite = constrain((random(rng) * 12)+scaleValue,0,39);

    dacWrite(APIN,oneVperOctaveTable[valueToWrite]);  // Writing to the DAC
    /*
    Serial.print(dCV);
    Serial.print("/");
    Serial.print(pnt);
    Serial.print("/");
    Serial.print(noteNames[valueToWrite]);
    Serial.println("/"); 
    */
    }
  else if ( flg == false )
    {
    uint8_t bufferIndex = patternBuffer[dCV][(pnt + 1) % stp];
    uint8_t scaleValue = scales[scl][bufferIndex];
  //uint8_t rangeValue =  rng + 1; // important to change the mapping function!!
    uint8_t valueToWrite = constrain((random(rng)* 12)+scaleValue,0,39);

    dacWrite(APIN,oneVperOctaveTable[valueToWrite]);  // Writing to the DAC
    /*
    Serial.print(dCV);
    Serial.print("/");
    Serial.print(pnt);
    Serial.print("/");
    Serial.print(noteNames[valueToWrite]);
    Serial.println("/");
    */ 
    }
  pnt = (pnt + 1) % stp;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------
// uint8_terpolate the WIFI SSIDs in an even and odd manner
//----------------------------------------------------------------------------------------------------------------------------------------------------------
void weightedInterpolation()
{
  if ( netWorksFound >=1 && (currentStep1 == 0 && currentStep2 == 0))
  {
    for ( uint8_t y = 0; y < parameters[0][1]; y++ )
      {
      uint8_t vE1 = SSIDsNames[0][y];
      uint8_t wE1 = SSIDsStrength[0];
      uint8_t vE2 = SSIDsNames[2][y];
      uint8_t wE2 = SSIDsStrength[2];
      uint8_t vE3 = SSIDsNames[4][y];
      uint8_t wE3 = SSIDsStrength[4];
      uint8_t vE4 = SSIDsNames[6][y];
      uint8_t wE4 = SSIDsStrength[6];
      patternBuffer[4][y]=((vE1*wE1+vE2*wE2+vE3*wE3+vE4*wE4)/(wE1+wE2+wE3+wE4))%scalesLength[parameters[2][1]];
      }
    for ( uint8_t y = 0; y < parameters[0][2]; y++ )
      {
      uint8_t vO1 = SSIDsNames[1][y];
      uint8_t wO1 = SSIDsStrength[1];
      uint8_t vO2 = SSIDsNames[3][y];
      uint8_t wO2 = SSIDsStrength[3];
      uint8_t vO3 = SSIDsNames[5][y];
      uint8_t wO3 = SSIDsStrength[5];
      uint8_t vO4 = SSIDsNames[7][y];
      uint8_t wO4 = SSIDsStrength[7];
      patternBuffer[5][y]=((vO1*wO1+vO2*wO2+vO3*wO3+vO4*wO4)/(wO1+wO2+wO3+wO4))%scalesLength[parameters[2][2]];
      }
 }
}

void setup() 
{
  reset_watchdog_timer();
  Serial.begin(115200);

  xTaskCreatePinnedToCore(  core0TaskToScanForSSIDs,    // Function to run in the task
                          "core0TaskToScanForSSIDs",    // Task name
                                              10000,    // Stack size for the task (adjust as needed)
                                               NULL,    // Parameter to pass to the task function (NULL if none)
                                                  1,    // Priority of the task (higher values have higher priority)
                                               NULL,    // Task handle (NULL if not needed)
                                                  0);   // Core to run the task on (0 or 1 for core 0 or core 1)

  xMutex = xSemaphoreCreateMutex();  

  pinMode(PULSE_OUT_PIN_1, OUTPUT);
  pinMode(PULSE_OUT_PIN_2, OUTPUT);
  pinMode(SWITCH_CONTROL, OUTPUT);

  analogReadResolution(8);  // set to 8bit for conviniance !!// set the resolution to 9 bits (0-512)

  manageWiFi(true);

  strip.begin();  // Initialize the LEDs
  strip.show();   // Initialize all LEDs to 'off'

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);  // Initialize I2C

  u8g2.begin();                        // Initialize the U8g2 display
  //u8g2.setFont(u8g2_font_5x7_tr);  // set charset
  u8g2.setFont(u8g2_font_micro_tr);  // set charset

  Button.attachClick(handleClick);               // attach click to void handleClick()
  Button.attachDoubleClick(handleDoubleClick);   // attach doubleclick to void handleDoubleClick()
  Button.attachLongPressStart(handleLongClick);  // attach longpress to void handleLongClick()
  Button.setClickMs(200);                        // set YmenuePosMax doubleclick intervall = 1/5 second
  Button.setPressMs(750);                        // set longpress duration 3/4 second
  Button.setDebounceMs(10);                      // set debounce to 10 ms

  readAndBufferAnalogInputs();
  mapInputToParameter();


  parameters[0][1] = 8; // steps 1
  stepsBuffer1 = parameters[0][1] = 8;
  parameters[0][0] = 4; // puls 1
  parameters[2][0] = 0; // rot 1
  parameters[1][1] = 2; // range 1

  parameters[0][2] = 8; // steps 2
  stepsBuffer2 = parameters[0][2];
  parameters[1][0] = 4; // puls 2
  parameters[3][0] = 0; // rot 2
  parameters[1][2] = 2; // range 2

  parameters[3][1] = 60; // TIME
  bpmBuffer = parameters[3][1];

  startTime1 = millis();
  startTime2 = millis();
}

void loop() {

  if (xSemaphoreTake(xMutex, 0)) 
  {
    weightedInterpolation();
    xSemaphoreGive(xMutex);
  }

  Button.tick();

if (millis() >= targetTime1)
{  
  if(currentTimerMode == 1 && currentStep1 ==(parameters[0][1])-1)
  {
 //   Serial.println("BEEBOOB");   
    targetTime2 = targetTime1;
  }
  //  Serial.println("BEEB");
  targetTime1 = millis() + interval1;

    generateSequence( parameters[0][0],   //Puls 1
                      parameters[0][1],   //Steps 1
                      parameters[2][0],   //Rotation 1
                      parameters[1][1],   //Range 1
                          currentStep1,   //Current Step in Sequence
                                     0,   //Buffer Dimension for Calculated Euclidean Sequence 
                                     2,   //Buffer Dimension for Rotated Euclidean Sequence
                                     4,   //Buffer Dimension for generated CV Values
                      parameters[2][1],   //Scale 1
                        lockedFlagSeq1,   //Flag to link Euclidean and CV Sequence
                       PULSE_OUT_PIN_1,   //Pin for Euclidean Sequence Output
                         DAC_OUT_PIN_1 ); //Pin for CV Sequence Output
}

if (millis() >= targetTime2)
{
  if(currentTimerMode == 2 && currentStep2 ==(parameters[0][2])-1)
  {
 //     Serial.println("Boobeeb");
    targetTime1 = targetTime2;
  }

 // Serial.println("BooB");
  targetTime2 = millis() + interval2;

    generateSequence( parameters[1][0],   //Puls 1
                      parameters[0][2],   //Steps 1
                      parameters[3][0],   //Rotation 1
                      parameters[1][2],   //Range 1
                          currentStep2,   //Current Step in Sequence
                                     1,   //Buffer Dimension for Calculated Euclidean Sequence 
                                     3,   //Buffer Dimension for Rotated Euclidean Sequence
                                     5,   //Buffer Dimension for generated CV Values
                      parameters[2][2],   //Scale 1
                        lockedFlagSeq2,   //Flag to link Euclidean and CV Sequence
                       PULSE_OUT_PIN_2,   //Pin for Euclidean Sequence Output
                         DAC_OUT_PIN_2 ); //Pin for CV Sequence Output
}
  readAndBufferAnalogInputs();
  mapInputToParameter();
  
  calculateInterval(parameters[0][1],parameters[0][2], currentTimerMode);

  outputLED();
  outputOLEDdebug();
  TimerCheck();
    // reset puls out
  digitalWrite(PULSE_OUT_PIN_1, LOW);
  digitalWrite(PULSE_OUT_PIN_2, LOW);

debugNew();
}

