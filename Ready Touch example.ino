/*
	Name:       Factory ready touch.ino
	Created:	11/28/2024 5:53:03 AM
	Author:     WeGoWireless.com

Thank you for supporting my products!

A simple example for WeGoWireless.com Ready Touch 2.8" tft

Adafruit:
"Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing products from Adafruit!"

Use Adafruit Feather ESP32-S3 2MB via board manager or other compatible board.

Download install the below libraries
*/
#include <Arduino.h>

// Voltage regulator power to real time clock DS3231, SD card slot, LCD backlight, DSB18B20, optional SCD41, One Wire DS241
const uint8_t PROGMEM LDO2_EN = 4;

// Reset pin to TFT
const uint8_t PROGMEM RESET = 9;

// TFT 2.8"
const int8_t VSPI_MISO = 37;
const int8_t VSPI_MOSI = 35;
const int8_t VSPI_SCLK = 36;
const int8_t VSPI_TFT_CS = 39;
const int8_t VSPI_TFT_DC = 38;
const int8_t VSPI_RST = -1;
const int8_t VSPI_TOUCH_CS = 40;
const uint8_t PROGMEM TFT_LED = 41;

// i2c
const uint8_t PROGMEM I2C_SDA_0 = 2;
const uint8_t PROGMEM I2C_SCL_0 = 1;
const uint32_t PROGMEM I2C_FREQUENCY = 100000;  //standard mode 100k, fast mode 400k

// SD
const uint8_t PROGMEM SD_CS = 21;  // SD CS
const uint8_t PROGMEM SD_CD = 44;  // card detect

// Piezo
const uint8_t PROGMEM PIEZO_PIN = 14;

// 5v input digital or analog reading resistor divider
const uint8_t PROGMEM SENSE_VBUS_PIN = 15;

// INPUTS resistor divider for analog EOL (End of line security system to detect short, open, closed)
// or simple digital reading
const uint8_t PROGMEM IN0 = 5;
const uint8_t PROGMEM IN1 = 6;

// Button inputs
const uint8_t PROGMEM BTN1 = 47;
const uint8_t PROGMEM BTN2 = 48;
const uint8_t PROGMEM BTN3 = 13;
const uint8_t PROGMEM BTN4 = 12;

// RELAY latching set for min 40 ms hold for on or off
const uint8_t PROGMEM REL1_SET_PIN = 10;
const uint8_t PROGMEM REL1_RESET_PIN = 11;
const uint8_t PROGMEM REL2_SET_PIN = 3;
const uint8_t PROGMEM REL2_RESET_PIN = 8;
const uint8_t PROGMEM REL3_SET_PIN = 16;
const uint8_t PROGMEM REL3_RESET_PIN = 46;

#include <Wire.h>  // ESP32 lib

#include "WiFi.h"  // ESP32 lib

// TFT
#include <Adafruit_ILI9341.h>  // https://github.com/adafruit/Adafruit_ILI9341
Adafruit_ILI9341 display_gfx = Adafruit_ILI9341(VSPI_TFT_CS, VSPI_TFT_DC);

#include <Adafruit_GFX.h>  // https://github.com/adafruit/Adafruit-GFX-Library
#include "SansSerif_plain_10.h"
#include "SansSerif_plain_16.h"
#include "SansSerif_plain_numbers_90.h"

// Touch screen
#include <XPT2046_Touchscreen.h>  // by Paul Stoffregenesp 

#include <SD.h>    // ESP32 lib
#include <SPI.h>  // ESP32 lib

// One wire DS248
#include <Adafruit_DS248x.h>
Adafruit_DS248x ds248x;
float tempF;
uint8_t rom[8];
#define DS18B20_FAMILY_CODE 0x28
#define DS18B20_CMD_CONVERT_T 0x44
#define DS18B20_CMD_MATCH_ROM 0x55
#define DS18B20_CMD_READ_SCRATCHPAD 0xBE

#include <RTClib.h> // https://github.com/adafruit/RTClib
RTC_DS3231 rtc;

#include <Adafruit_MAX1704X.h>  //download install. Lib manager doesn't report latest https://github.com/adafruit/Adafruit_MAX1704X/tree/main
Adafruit_MAX17048 maxlipo;

bool SD_inserted = false;
bool FoundMax = false;
char wifiScanBuff[33];
bool wifiScanPrint = false;

bool piezo_busy = false;
uint32_t piezo_timer;
const long PIEZO_DURATION = 100;

GFXcanvas1 canvasTime(100, 12);
GFXcanvas16 canvasButton(60, 20);
GFXcanvas16 canvasIn(60, 20);
GFXcanvas16 canvasAnalog(60, 20);
GFXcanvas16 canvasOutput(104, 20);
GFXcanvas16 canvasBattery(85, 22);
GFXcanvas1 canvasTemperature(112, 70);
GFXcanvas1 canvasSD(100, 12);
GFXcanvas1 canvasWiFi(100, 190);

struct tft_struct
{
	uint8_t tm_sec;
	boolean drawTime = false;
	uint8_t input_last[8];
	float input_voltage_last[2];
	float lastBattV = 0;
	float Temperature = 0;
	int8_t drawSD = 0;					   // 0=not drawn, 1 mounted, 2 not mounted
};
struct tft_struct tftSt;

struct power_struct
{
	float CellVoltage = 0;
	float CellPercent = 0;
	float ChargeRate = 0;
};
struct power_struct power_st;

struct wifi_struct
{
	char ssid[33];
	char rssi[33];
};
struct wifi_struct wifi_st;

// INPUT
const uint8_t PROGMEM IN_DIGITAL = 1;
const uint8_t PROGMEM IN_ANALOG = 2;

// INPUT
const uint8_t PROGMEM IN_UNKNOWN = 0;
const uint8_t PROGMEM IN_CLOSED = 1;
const uint8_t PROGMEM IN_NORMAL = 3;
const uint8_t PROGMEM IN_OPEN = 6;
const uint8_t PROGMEM IN_VIOLATED = 11;
const uint8_t PROGMEM IN_FAULTY = 12;
const uint8_t PROGMEM IN_SHORT = 13;
const uint8_t PROGMEM IN_POWER_LOSS = 14;

// INPUT
const long IO_INPUT_UPDATE = 1 * 1000;
uint32_t TimerInput;

const long IO_BUTTON_UPDATE = 400;
uint32_t TimerButton;

const uint32_t WIFI_UPDATE = 5 * 1000;
uint32_t Timer_wifi;

const uint32_t TIME_UPDATE = 1000;
uint32_t Timer_time;

uint16_t BATTERY_UPDATE = 60 * 1000;
int32_t Timer_battery = millis() - BATTERY_UPDATE;

const uint32_t TEMPERATURE_UPDATE = 60 * 1000;
int32_t TimerTemperature;
const uint32_t START_CONVERSION_UPDATE = 50 * 1000;
uint32_t TimerStartConversion;

void setupWiFi();
void scanWiFi(uint16_t x, uint16_t y);
void setupRTC();
void setupMAX();
void setupDS248X();
void SearchDS2484();
float readTemperature(uint8_t* rom);
void Strat_Conversion(uint8_t* rom);
void setupTFT();
void drawWiFi(uint16_t x, uint16_t y);
void drawBattery(uint16_t x, uint16_t y, bool initDraw = false);
void drawTemperatureDetail(uint16_t x, uint16_t y, bool initDraw = false);
void drawSD(uint16_t x, uint16_t y);

void setup()
{
	Serial.begin(115200);

	// Turn on LDO2
	pinMode(LDO2_EN, OUTPUT);
	digitalWrite(LDO2_EN, HIGH);  // startup time 20us no load as per AP2112 spec

	delay(50); // Wait for power up

	// Set TFT reset pin
	pinMode(RESET, OUTPUT);
	digitalWrite(RESET, HIGH);

	// Set button inputs
	pinMode(BTN1, INPUT);
	pinMode(BTN2, INPUT);
	pinMode(BTN3, INPUT);
	pinMode(BTN4, INPUT);

	// configure buzzer pins
	pinMode(PIEZO_PIN, OUTPUT);  // set as outputs
	digitalWrite(PIEZO_PIN, 0);  // clamp to ground

	pinMode(SD_CD, INPUT);  // Card Detect
	pinMode(TFT_LED, OUTPUT);

	// Wire setup
	Wire.setPins(I2C_SDA_0, I2C_SCL_0);

	setupWiFi();
	setupRTC();
	setupMAX();
	setupDS248X();
	SearchDS2484();
	Strat_Conversion(rom);
	TimerTemperature = millis() - TEMPERATURE_UPDATE + 1000;  // read temperature 1 second giving time for conversion
	setupTFT();
	drawBattery(18, 0, true);
	drawTemperatureDetail(100, 80, true);
	drawSD(220, 0);
}

// Make size of files human readable
// source: https://github.com/CelliesProjects/minimalUploadAuthESP32
String file_humanReadableSize(const size_t bytes) {
	if (bytes < 1024) return String(bytes) + " B";
	else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
	else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
	else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

void setupWiFi() {
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
}

// run WiFi scan in non blocking async mode. Check for complete and return results.
void scanWiFi(uint16_t x, uint16_t y) {
	char buff[33];
	uint8_t canvasLine;
	int16_t n;

	n = WiFi.scanComplete();

	if (n == 0) {
		Serial.println("WiFi no networks found");
	}
	else if (n == WIFI_SCAN_RUNNING) {
		if (wifiScanPrint) {
			Serial.println("WiFi scan running");
		}
	}
	else if (n == WIFI_SCAN_FAILED) {
		Serial.println("WiFi scan failed");

		// Delete the scan result to free memory for code below.
		WiFi.scanDelete();

		// WiFi.scanNetworks start new scan
		n = WiFi.scanNetworks(true);
		if (n == WIFI_SCAN_RUNNING) {
			if (wifiScanPrint) {
				Serial.println("WiFi start scan running");
			}
		}
		else if (n == WIFI_SCAN_FAILED) {
			Serial.println("WiFi scan start failed");
		}
	}
	else if (n > 0) {
		canvasWiFi.fillScreen(ILI9341_BLACK);

		if (wifiScanPrint) {
			Serial.println("Scan done");
			Serial.print(n);
			Serial.println(" networks found");
			Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
		}

		canvasLine = 0;
		for (int i = 0; i < n; ++i) {
			// Print SSID and RSSI for each network found
			if (wifiScanPrint) {
				Serial.printf("%2d", i + 1);
				Serial.print(" | ");
				Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
				Serial.print(" | ");
				Serial.printf("%4ld", WiFi.RSSI(i));
				Serial.print(" | ");
				Serial.printf("%2ld", WiFi.channel(i));
				Serial.print(" | ");
				switch (WiFi.encryptionType(i)) {
				case WIFI_AUTH_OPEN:            Serial.print("open"); break;
				case WIFI_AUTH_WEP:             Serial.print("WEP"); break;
				case WIFI_AUTH_WPA_PSK:         Serial.print("WPA"); break;
				case WIFI_AUTH_WPA2_PSK:        Serial.print("WPA2"); break;
				case WIFI_AUTH_WPA_WPA2_PSK:    Serial.print("WPA+WPA2"); break;
				case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
				case WIFI_AUTH_WPA3_PSK:        Serial.print("WPA3"); break;
				case WIFI_AUTH_WPA2_WPA3_PSK:   Serial.print("WPA2+WPA3"); break;
				case WIFI_AUTH_WAPI_PSK:        Serial.print("WAPI"); break;
				default:                        Serial.print("unknown");
				}
				Serial.println();
			}
			canvasWiFi.setFont(&SansSerif_plain_10);
			sprintf(buff, "%d", WiFi.RSSI(i));
			canvasWiFi.setCursor(0, canvasLine * 12);
			canvasWiFi.print(buff);

			canvasWiFi.setFont(&SansSerif_plain_10);
			WiFi.SSID(i).toCharArray(buff, WiFi.SSID(i).length() + 1);
			canvasWiFi.setCursor(30, canvasLine * 12);
			canvasWiFi.print(buff);

			if (WiFi.SSID(i).length() >= 9) {
				canvasLine += 1;
			}
			canvasLine += 1;
			//delay(10);
		}
		display_gfx.drawBitmap(x, y, canvasWiFi.getBuffer(), 100, 190, ILI9341_WHITE, ILI9341_BLACK);
		if (wifiScanPrint) {
			Serial.println("");
		}

		// Delete the scan result to free memory for code below.
		WiFi.scanDelete();

		// WiFi.scanNetworks start new scan
		n = WiFi.scanNetworks(true);
		if (n == WIFI_SCAN_RUNNING) {
			if (wifiScanPrint) {
				Serial.println("WiFi start scan");
			}
		}
		else if (n == WIFI_SCAN_FAILED) {
			Serial.println("WiFi scan start failed");
		}
	}
}


void drawWiFi(uint16_t x, uint16_t y) {
	//canvasWiFi.fillScreen(ILI9341_BLACK);

	//display_gfx.drawBitmap(x, y, canvasWiFi.getBuffer(), 100, 100, ILI9341_WHITE, ILI9341_BLACK);
}

void drawSD(uint16_t x, uint16_t y) {
	if (SD_inserted && tftSt.drawSD == 0 || SD_inserted && tftSt.drawSD == 2) {
		canvasSD.fillScreen(ILI9341_BLACK);
		canvasSD.drawFastHLine(3, 0, 10, ILI9341_WHITE);  // top
		canvasSD.drawLine(3, 0, 0, 3, ILI9341_WHITE);     // left top short angle
		canvasSD.drawFastVLine(0, 3, 7, ILI9341_WHITE); // left short
		canvasSD.drawFastVLine(10, 0, 10, ILI9341_WHITE); // right
		canvasSD.drawFastHLine(0, 10, 10, ILI9341_WHITE);  // bottom
		canvasSD.drawFastVLine(4, 1, 3, ILI9341_WHITE); // 3 pins top
		canvasSD.drawFastVLine(6, 2, 3, ILI9341_WHITE); // 3 pins top
		canvasSD.drawFastVLine(8, 2, 3, ILI9341_WHITE); // 3 pins top

		canvasSD.setFont(&SansSerif_plain_10);
		canvasSD.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
		canvasSD.setCursor(22, 11);
		canvasSD.print(file_humanReadableSize(SD.cardSize()));


		display_gfx.drawBitmap(x, y, canvasSD.getBuffer(), 100, 12, ILI9341_WHITE, ILI9341_BLACK);
		tftSt.drawSD = 1;
	}
	else if (SD_inserted == false && tftSt.drawSD == 1 || SD_inserted == false && tftSt.drawSD == 0) {
		canvasSD.fillScreen(ILI9341_BLACK);
		canvasSD.drawFastHLine(3, 0, 10, ILI9341_RED);  // top
		canvasSD.drawLine(3, 0, 0, 3, ILI9341_RED);     // left top short angle
		canvasSD.drawFastVLine(0, 3, 7, ILI9341_RED); // left short
		canvasSD.drawFastVLine(10, 0, 10, ILI9341_RED); // right
		canvasSD.drawFastHLine(0, 10, 10, ILI9341_RED);  // bottom
		canvasSD.drawFastVLine(4, 1, 3, ILI9341_RED); // 3 pins top
		canvasSD.drawFastVLine(6, 2, 3, ILI9341_RED); // 3 pins top
		canvasSD.drawFastVLine(8, 2, 3, ILI9341_RED); // 3 pins top
		display_gfx.drawBitmap(x, y, canvasSD.getBuffer(), 100, 12, ILI9341_RED, ILI9341_BLACK);
		tftSt.drawSD = 2;
	}
}

void setupDS248X() {
	uint8_t i = 0;

	if (!ds248x.begin(&Wire, DS248X_ADDRESS)) {
		Serial.println(F("DS248x initialization failed."));
	}
	else {
		while (!ds248x.OneWireReset()) {
			if (i > 10) {
				Serial.println("Failed to do a 1W reset");
				break;
			}
			if (ds248x.shortDetected()) {
				Serial.println("Short detected");
			}
			if (!ds248x.presencePulseDetected()) {
				Serial.println("No presense pulse");
			}
			Serial.print(F("."));
			i++;
			delay(500);
		}
		Serial.println("One Wire bus reset OK");
	}
}

void SearchDS2484() {
	if (!ds248x.OneWireSearch(rom)) {
		Serial.println("No more devices found\n\n");
		return;
	}

	Serial.print("Found device ROM: ");
	for (int i = 0; i < 8; i++) {
		if (rom[i] < 16) {
			Serial.print("0");
		}
		Serial.print(rom[i], HEX);
		Serial.print(" ");
	}
	Serial.println("");
}

/********************************************************************
 * @brief				draws the temperature readings
 * @param  thermostat	thermostat number for temperature sensor reading.
 * @param  x			Top left corner horizontal coordinate.
 * @param  y			Top left corner vertical coordinate.
 * @return				None
 *********************************************************************/
void drawTemperatureDetail(uint16_t x, uint16_t y, bool initDraw) {
	// only draw if temperature changed by 1 up or down
	if (tempF - tftSt.Temperature < 1 || tempF - tftSt.Temperature > 1 || initDraw) {
		tftSt.Temperature = tempF;
		char charBuf[5];
		dtostrf(tempF, 2, 0, charBuf);
		canvasTemperature.fillScreen(ILI9341_BLACK);
		canvasTemperature.setFont(&SansSerif_plain_90);
		canvasTemperature.setCursor(0, 68); // cutting off the bottom
		canvasTemperature.print(charBuf);
		display_gfx.drawBitmap(x, y, canvasTemperature.getBuffer(), 112, 70, ILI9341_WHITE, ILI9341_BLACK);

	}
}

float readTemperature(uint8_t* rom) {
	uint8_t i;

	// Select the DS18B20 device
	ds248x.OneWireReset();
	ds248x.OneWireWriteByte(DS18B20_CMD_MATCH_ROM); // Match ROM command
	for (i = 0; i < 8; i++) {
		ds248x.OneWireWriteByte(rom[i]);
	}
	ds248x.OneWireWriteByte(DS18B20_CMD_READ_SCRATCHPAD); // Read Scratchpad command

	// Read scratchpad
	uint8_t data[9];
	for (i = 0; i < 9; i++) {
		ds248x.OneWireReadByte(&data[i]);
	}

	// Calculate temperature
	//int16_t raw = (data[1] << 8) | data[0];
	//float celsius = (float)raw / 16.0;

	int16_t fpTemperature = (((int16_t)data[1]) << 11)
		| (((int16_t)data[0]) << 3);

	return (((float)fpTemperature * 0.0140625f) + 32.0f);

	//return celsius;
}

void Strat_Conversion(uint8_t* rom) {
	// Select the DS18B20 device
	ds248x.OneWireReset();
	ds248x.OneWireWriteByte(DS18B20_CMD_MATCH_ROM); // Match ROM command
	for (uint8_t i = 0; i < 8; i++) {
		ds248x.OneWireWriteByte(rom[i]);
	}

	// Start temperature conversion
	ds248x.OneWireWriteByte(DS18B20_CMD_CONVERT_T); // Convert T command
}

void setupRTC() {
	// I2C DS3231 clock
	if (!rtc.begin()) {
		Serial.println("Couldn't find RTC");
	}
	else {
		// following line sets the RTC to the date & time this sketch was compiled
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		Serial.println("Found RTC");
	}
}

void setupTFT() {
	// TFT setup
	display_gfx.begin();
	display_gfx.setRotation(3);
	display_gfx.fillScreen(ILI9341_BLACK);

	// Turn on TFT LEDs
	digitalWrite(TFT_LED, LOW);    // LOW to Turn on;
}

void setupMAX() {
	// I2C MAX1704X
	uint8_t i = 0;
	FoundMax = true;

	while (!maxlipo.begin()) {
		Serial.print(F("."));
		delay(10);
		i++;
		if (i > 10) {
			Serial.println("Couldnt find MAX17048!");
			FoundMax = false;
			break;
		}
	}

	if (FoundMax) {
		Serial.printf("Found MAX17048 Chip ID: 0x%X", maxlipo.getChipID());
		Serial.println("");
		maxlipo.wake();
		power_st.CellVoltage = maxlipo.cellVoltage();
		power_st.CellPercent = maxlipo.cellPercent();
		power_st.ChargeRate = maxlipo.chargeRate();

		Timer_battery = millis();
	}
}

void mountSD() {
	uint8_t i = 0;
	// Mount SD
	while (!SD.begin(SD_CS, SPI, 4000000, "/sd", 5U, false)) {
		Serial.println(F('.'));
		delay(100);
		i++;
		if (i > 10) {
			Serial.println(F("Gave up waiting for SD card to mount."));
		}
	}
}

// Input ESP Analog read
/***********************************************************************************
	2) Get analog of the inputs of the board.

***********************************************************************************/
float get_input_Analog(int sensor_pin) {
	// Feather HUZZAH32 You can read half of the battery voltage off of A13.Don't forget to double the voltage
	//  you read, since there is a divider.
	//  LiPoly batteries are 'maxed out' at 4.2V and stick around 3.7V for much of the battery life, then slowly sink down
	//  to 3.2V or so before the protection circuitry cuts it off.By measuring the voltage you
	//  can quickly tell when you're heading below 3.7V.
	// returns uint32_t

	float volt = analogReadMilliVolts(sensor_pin);
	volt *= 2;    // we divided by 2 with resistors, so multiply back
	volt /= 1000; // convert to volts!

	return volt;
}


// Input ESP EOL read
/***********************************************************************************
	1) Get states of the inputs of the board. Read analog EOL (2.2k resistor End
	Of Line resistor) and return.

***********************************************************************************/
uint8_t
get_input_EOL(int sensor_pin) {
	int sensorValue;
	float volt = 0;

	sensorValue = analogRead(sensor_pin);
	volt = (sensorValue * 3.3) / 4095;

	if (volt > 1.9 && volt < 2.5) {
		return IN_CLOSED;
	}
	else if (volt > 2.5 && volt < 3) {
		return IN_OPEN;
	}
	else if (volt < 1) {
		return IN_SHORT;
	}
	else if (volt > 3) {
		return IN_FAULTY;
	}
	else {
		return IN_UNKNOWN;
	}
}

/*********************************************************************
 * @brief     draw the time
 * @param  x  Top left corner horizontal coordinate.
 * @param  y  Top left corner vertical coordinate.
 * @return    None
 * @note      Normaly you read the I2C clock every x hours and set the ESP32 clock.
 *			  Reading the I2C clock may not be the best way to read time vrs reading ESP32 clock.
 *			  This is just for testing.
 *********************************************************************/
void drawTime(uint16_t x, uint16_t y) {
	DateTime now = rtc.now();

	// only update screen if time changed
	if (now.second() != tftSt.tm_sec) {
		tftSt.tm_sec = now.second();

		canvasTime.fillScreen(ILI9341_BLACK);
		canvasTime.setFont(&SansSerif_plain_16);
		canvasTime.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

		canvasTime.setCursor(0, 12);
		canvasTime.print(now.hour(), DEC);
		canvasTime.print(':');
		canvasTime.print(now.minute(), DEC);
		canvasTime.print(':');
		canvasTime.print(now.second(), DEC);

		display_gfx.drawBitmap(x, y, canvasTime.getBuffer(), 100, 12, ILI9341_WHITE, ILI9341_BLACK);
	}
}

void drawButton(uint8_t input, uint8_t type, uint8_t value, float a_value, uint16_t x, uint16_t y) {
	if (type == IN_DIGITAL) {
		canvasButton.fillScreen(ILI9341_BLACK);

		canvasButton.drawRoundRect(0, 0, 36, 19, 5, ILI9341_WHITE);
		canvasButton.setFont(&SansSerif_plain_16);
		canvasButton.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

		canvasButton.setCursor(2, 16);
		canvasButton.print(F("B"));
		canvasButton.print(input);

		if (value == IN_CLOSED) {
			canvasButton.setTextColor(ILI9341_BLACK, ILI9341_GREEN);
			canvasButton.fillCircle(36, 10, 9, ILI9341_GREEN);
			canvasButton.setCursor(30, 17);
			canvasButton.print(F("C"));
		}
		else if (value == IN_OPEN) {
			canvasButton.setTextColor(ILI9341_WHITE, ILI9341_RED);
			canvasButton.fillCircle(36, 10, 9, ILI9341_RED);
			canvasButton.setCursor(30, 17);
			canvasButton.print(F("O"));
		}
		display_gfx.drawRGBBitmap(x, y, canvasButton.getBuffer(), 60, 20);
	}
}
void drawInput(uint8_t input, uint8_t type, uint8_t value, float a_value, uint16_t x, uint16_t y) {
	if (type == IN_DIGITAL) {
		canvasIn.fillScreen(ILI9341_BLACK);

		canvasIn.drawRoundRect(0, 0, 36, 19, 5, ILI9341_WHITE);
		canvasIn.setFont(&SansSerif_plain_16);
		canvasIn.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

		canvasIn.setCursor(2, 16);
		canvasIn.print(input);

		if (value == IN_CLOSED) {
			canvasIn.setTextColor(ILI9341_BLACK, ILI9341_GREEN);
			canvasIn.fillCircle(36, 10, 9, ILI9341_GREEN);
			canvasIn.setCursor(30, 17);
			canvasIn.print(F("C"));
		}
		else if (value == IN_OPEN) {
			canvasIn.setTextColor(ILI9341_WHITE, ILI9341_RED);
			canvasIn.fillCircle(36, 10, 9, ILI9341_RED);
			canvasIn.setCursor(30, 17);
			canvasIn.print(F("O"));
		}
		else if (value == IN_NORMAL) {
			canvasIn.setTextColor(ILI9341_WHITE, ILI9341_RED);
			canvasIn.fillCircle(36, 10, 9, ILI9341_RED);
			canvasIn.setCursor(30, 17);
			canvasIn.print(F("N"));
		}
		else if (value == IN_POWER_LOSS) {
			canvasIn.setTextColor(ILI9341_WHITE, ILI9341_RED);
			canvasIn.fillCircle(36, 10, 9, ILI9341_RED);
			canvasIn.setCursor(30, 17);
			canvasIn.print(F("P"));
		}
		else if (value == IN_VIOLATED) {
			canvasIn.setTextColor(ILI9341_WHITE, ILI9341_RED);
			canvasIn.fillCircle(36, 10, 9, ILI9341_RED);
			canvasIn.setCursor(30, 17);
			canvasIn.print(F("V"));
		}
		else if (value == IN_FAULTY) {
			canvasIn.setTextColor(ILI9341_WHITE, ILI9341_RED);
			canvasIn.fillCircle(36, 10, 9, ILI9341_RED);
			canvasIn.setCursor(30, 17);
			canvasIn.print(F("F"));
		}
		else if (value == IN_SHORT) {
			canvasIn.setTextColor(ILI9341_WHITE, ILI9341_RED);
			canvasIn.fillCircle(36, 10, 9, ILI9341_RED);
			canvasIn.setCursor(30, 17);
			canvasIn.print(F("S"));
		}
		display_gfx.drawRGBBitmap(x, y, canvasIn.getBuffer(), 60, 20);
	}
	else if (type == IN_ANALOG) {
		canvasAnalog.fillScreen(ILI9341_BLACK);

		canvasAnalog.drawRoundRect(0, 0, 60, 19, 5, ILI9341_WHITE);
		canvasAnalog.setFont(&SansSerif_plain_16);
		canvasAnalog.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

		canvasAnalog.setCursor(2, 16);
		canvasAnalog.print(input);
		canvasAnalog.setCursor(20, 16);
		canvasAnalog.printf("%.1f", a_value);
		canvasAnalog.setFont(&SansSerif_plain_10);
		canvasAnalog.setCursor(50, 16);
		canvasAnalog.print(F("v"));

		display_gfx.drawRGBBitmap(x, y, canvasAnalog.getBuffer(), 60, 20);
	}
}

/********************************************************************
 * @brief     Show the current battery charge level on the screen.
 * @param  x  Top left corner horizontal coordinate.
 * @param  y  Top left corner vertical coordinate.
 * @return    None
 *********************************************************************/
void drawBattery(uint16_t x, uint16_t y, bool initDraw) {
	if (power_st.CellVoltage - tftSt.lastBattV > .5 || power_st.CellVoltage - tftSt.lastBattV < .5 || initDraw) {
		tftSt.lastBattV = power_st.CellVoltage;

		if (power_st.CellVoltage > 1 || initDraw) { // Only display if there is a valid reading
			canvasBattery.fillScreen(ILI9341_BLACK);
			canvasBattery.drawRect(0, 0, 25, 10, ILI9341_WHITE);
			canvasBattery.drawRect(25, 2, 2, 6, ILI9341_WHITE);

			canvasBattery.setFont(&SansSerif_plain_10);
			canvasBattery.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
			canvasBattery.setCursor(33, 10);
			canvasBattery.printf("%.0f%s", power_st.CellPercent, "%");

			canvasBattery.setCursor(0, 22);
			canvasBattery.printf("%.1fv", power_st.CellVoltage);

			canvasBattery.setCursor(35, 22);
			canvasBattery.printf("%.1f%%", power_st.ChargeRate);

			if (power_st.CellPercent >= 40) {
				canvasBattery.fillRect(1, 2, 21 * power_st.CellPercent / 100.0, 6, ILI9341_GREEN);
			}
			else if (power_st.CellPercent < 40 && power_st.CellPercent >= 20) {
				canvasBattery.fillRect(1, 2, 21 * power_st.CellPercent / 100.0, 6, ILI9341_ORANGE);
			}
			else if (power_st.CellPercent < 20) {
				canvasBattery.fillRect(1, 2, 21 * power_st.CellPercent / 100.0, 6, ILI9341_RED);
			}

			display_gfx.drawRGBBitmap(x, y, canvasBattery.getBuffer(), 85, 22);
		}

	}
}

void loop()
{
	// check to see if user inserted or removed SD card
	if (!SD_inserted) {
		if (digitalRead(SD_CD)) {
			delay(200); // delay as card was just inserted debounce connection
			mountSD();
			SD_inserted = true;
			Serial.printf("SD Card Size: %i Total space: %i Used space: %i Free space: %i", file_humanReadableSize(SD.cardSize()), file_humanReadableSize(SD.totalBytes()), file_humanReadableSize(SD.usedBytes()), file_humanReadableSize(SD.totalBytes() - SD.usedBytes()));
			drawSD(220, 0);
		}
	}
	else if (SD_inserted && digitalRead(SD_CD) == false) {
		SD_inserted = false;
		Serial.println(F("SD removed"));
		drawSD(220, 0);
	}

	if ((millis() - Timer_time) >= TIME_UPDATE) {
		drawTime(115, 0);
		Timer_time = millis();
	}

	if ((millis() - TimerButton) >= IO_BUTTON_UPDATE) {
		uint8_t in_value;

		if (digitalRead(BTN1) == 0) {
			in_value = IN_CLOSED;
		}
		else {
			in_value = IN_OPEN;
		}
		if (in_value != tftSt.input_last[0]) {
			if (in_value == IN_CLOSED) {
				if (!piezo_busy) {
					piezo_busy = true;
					tone(PIEZO_PIN, 4000, PIEZO_DURATION);
					piezo_timer = millis();
				}
			}
			drawButton(1, IN_DIGITAL, in_value, 0, 10, 80);
			tftSt.input_last[0] = in_value;
		}

		if (digitalRead(BTN2) == 0) {
			in_value = IN_CLOSED;
		}
		else {
			in_value = IN_OPEN;
		}
		if (in_value != tftSt.input_last[1]) {
			if (in_value == IN_CLOSED) {
				if (!piezo_busy) {
					piezo_busy = true;
					tone(PIEZO_PIN, 4000, PIEZO_DURATION);
					piezo_timer = millis();
				}
			}
			drawButton(2, IN_DIGITAL, in_value, 0, 10, 100);
			tftSt.input_last[1] = in_value;
		}

		if (digitalRead(BTN3) == 0) {
			in_value = IN_CLOSED;
		}
		else {
			in_value = IN_OPEN;
		}
		if (in_value != tftSt.input_last[2]) {
			if (in_value == IN_CLOSED) {
				if (!piezo_busy) {
					piezo_busy = true;
					tone(PIEZO_PIN, 4000, PIEZO_DURATION);
					piezo_timer = millis();
				}
			}
			drawButton(3, IN_DIGITAL, in_value, 0, 10, 120);
			tftSt.input_last[2] = in_value;
		}

		if (digitalRead(BTN4) == 0) {
			in_value = IN_CLOSED;
		}
		else {
			in_value = IN_OPEN;
		}
		if (in_value != tftSt.input_last[3]) {
			if (in_value == IN_CLOSED) {
				if (!piezo_busy) {
					piezo_busy = true;
					tone(PIEZO_PIN, 4000, PIEZO_DURATION);
					piezo_timer = millis();
				}
			}
			drawButton(4, IN_DIGITAL, in_value, 0, 10, 140);
			tftSt.input_last[3] = in_value;
		}
		TimerButton = millis();
	}


	// -------------------------------------------------------------------
	// Update inputs every time
	// -------------------------------------------------------------------
	if ((millis() - TimerInput) >= IO_INPUT_UPDATE) {
		uint8_t in_value;
		float in_analog;

		// Input Analog read resistor divider
		in_analog = get_input_Analog(IN0);
		if (in_analog != tftSt.input_voltage_last[0]) {
			drawInput(1, IN_ANALOG, 0, in_analog, 10, 160);
			tftSt.input_voltage_last[0] = in_analog;
		}

		// Input EOL read resistor divider
		in_value = get_input_EOL(IN0);
		if (in_value != tftSt.input_last[4]) {
			drawInput(1, IN_DIGITAL, in_value, 0, 70, 160);
			tftSt.input_last[4] = in_value;
		}

		// Analog read resistor divider
		in_analog = get_input_Analog(IN1);
		if (in_analog != tftSt.input_voltage_last[1]) {
			drawInput(2, IN_ANALOG, 0, in_analog, 10, 180);
			tftSt.input_voltage_last[1] = in_analog;
		}

		// Input EOL read resistor divider
		in_value = get_input_EOL(IN1);
		if (in_value != tftSt.input_last[5]) {
			drawInput(2, IN_DIGITAL, in_value, 0, 70, 180);
			tftSt.input_last[5] = in_value;
		}

		// 5v power analog read
		in_analog = get_input_Analog(SENSE_VBUS_PIN);
		if (in_analog != tftSt.input_voltage_last[2]) {
			drawInput(3, IN_ANALOG, 0, in_analog, 10, 200);
			tftSt.input_voltage_last[2] = in_analog;
		}

		// 5v power digital read
		// we cant read digital since we are set to analog input in this example
		// just an example of reading digital for sensing 5v power loss on battery.
		// Set input as digital in setup for this to work.
		//if (digitalRead(SENSE_VBUS_PIN) == 1) {
		//	in_value = IN_NORMAL;
		//}
		//else {
		//	in_value = IN_POWER_LOSS;
		//}
		//if (in_value != tftSt.input_last[7]) {
		//	drawInput(3, IN_DIGITAL, in_value, 0, 70, 200);
		//	tftSt.input_last[7] = in_value;
		//}

		TimerInput = millis();
	}

	// simple reset piezo timer busy
	if (piezo_busy) {
		if ((millis() - piezo_timer) >= PIEZO_DURATION) {
			piezo_busy = false;
			piezo_timer = millis();
		}
	}

	if ((millis() - Timer_battery) >= BATTERY_UPDATE) {
		if (FoundMax) {
			power_st.CellVoltage = maxlipo.cellVoltage();
			power_st.CellPercent = maxlipo.cellPercent();
			power_st.ChargeRate = maxlipo.chargeRate();
			drawBattery(18, 0);
		}

		Timer_battery = millis();
	}

	// get temperature reading
	if ((millis() - TimerTemperature) >= TEMPERATURE_UPDATE) {
		tempF = readTemperature(rom);

		drawTemperatureDetail(100, 80);

		TimerTemperature = millis();
		TimerStartConversion = TimerTemperature;
	}

	// Start non blocking temperature conversion
	if ((millis() - TimerStartConversion) >= START_CONVERSION_UPDATE) {
		Strat_Conversion(rom);

		TimerStartConversion = millis(); // set here so we dont hit again but gets updated again after TimerTemperature
	}

	// WiFi scan non blocking async mode
	if ((millis() - Timer_wifi) >= WIFI_UPDATE) {
		scanWiFi(220, 50);

		Timer_wifi = millis();
	}
}
