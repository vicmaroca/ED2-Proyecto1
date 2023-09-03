/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME  "Vicmaroca"
#define IO_KEY       "aio_jDrc58AGEnaoMge6ZLb5WnRC7cX6"

/******************************* WIFI **************************************/

#define WIFI_SSID "iPhone"
#define WIFI_PASS "internet"

// comment out the following lines if you are using fona or ethernet
#include "AdafruitIO_WiFi.h"

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);