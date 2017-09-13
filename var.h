//
// Created by Bruno on 30/06/2017.
//

//Software Version
#define APPNAME "Arduino Temperature"
#define APPVERSION "1.4"

//Arduino I/O
#define NB_IO_OUTPUT 8

//DHT22
#define PINDHT22 2

//LCD
#define LCD_ADDR 0x3F
#define LCD_LINE 2
#define LCD_ROW 16

//Acquisition frequency
#define ACQ_FREQUENCY 5000
#define LOG_FREQUENCY 10000

#define BLINK_TIME 50
#define BLINK_INIT 5
#define BLINK_INIT_TIME 250

//LED
#define LED_PIN 2

//SD Access Pin
#define SD_PIN 10

//File for log
#define DATAFILE "log.csv"

#define SERIAL_BAUD 115200

//DEBUG MODE
#define DEBUG
#define INFO
