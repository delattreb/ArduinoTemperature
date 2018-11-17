//
// Created by Bruno on 30/06/2017.
//

//DEBUG MODE
//#define DEBUG
//#define INFO

//Serial speed
#define SERIAL_BAUD 9600

//Device 1:cave 2:séjour 3:chambre 4:nomade
#define DEVICE_NUMBER 4

#define MAXDEVICE 5
//ESP8266 Wifi
#define RX_PIN 6
#define TX_PIN 5

//LCD
#define LCD_ADDR 0x3F
//#define LCD_ADDR 0x27
#define LCD_LINE 2
#define LCD_ROW 16

//Acquisition frequency
#define ACQ_FREQUENCY 1000
#define LOG_FREQUENCY 240000  // 4 minutes
#define SERIAL_DELAY 1000

#define BLINK_TIME 50
#define BLINK_INIT 5
#define BLINK_INIT_TIME 250

//LED
#define LED_PIN 2

//SD Access Pin
#define SD_CS_PIN 10
