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

char appVersion[80];

#include <Wire.h>  // ESP32 lib
#include <WiFi.h>  // ESP32 lib
#include <WiFiClient.h>  // ESP32 lib
#include <WebServer.h>  // ESP32 lib
#include <ArduinoOTA.h>  // ESP32 lib
WebServer server(80);
const char* softAP_ssid = "Ready Touch 2.8";
const char* softAP_password = "";
IPAddress apIP(192, 168, 4, 1);
IPAddress apNetMsk(255, 255, 255, 0);
char sta_ssid[31];
char sta_pass[31];

// TFT
#include <Adafruit_ILI9341.h>  // https://github.com/adafruit/Adafruit_ILI9341
Adafruit_ILI9341 display_gfx = Adafruit_ILI9341(VSPI_TFT_CS, VSPI_TFT_DC);

#include <Adafruit_GFX.h>  // https://github.com/adafruit/Adafruit-GFX-Library
#include "SansSerif_plain_10.h"
#include "SansSerif_plain_16.h"
#include "SansSerif_plain_numbers_90.h"

#include "html.h"
#include "images/wifi_signal_1.svg.gz.h"
#include "images/wifi_signal_2.svg.gz.h"
#include "images/wifi_signal_3.svg.gz.h"
#include "images/wifi_signal_4.svg.gz.h"
#include "images/wifi_signal_5.svg.gz.h"

// Touch screen
#include <XPT2046_Touchscreen.h>  // by Paul Stoffregenesp 
XPT2046_Touchscreen ts(VSPI_TOUCH_CS);
uint32_t lastTouched = 0;

const uint16_t TS_MINX = 370;
const uint16_t TS_MINY = 470;
const uint16_t TS_MAXX = 3700;
const uint16_t TS_MAXY = 3600;

#include <SD.h>    // ESP32 lib
#include <SPI.h>  // ESP32 lib
#include <Preferences.h>  // ESP32 lib
Preferences Prefs;

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
char scanJson[1565] = "[";  // wifi scan for web page 1565 plus /0 = 20 networks if all had 32 char ssid, this is not going to happen.
bool mode_ETH = false;
bool mode_AP = false;
bool mode_STA = false;
float TxPower = 0;
uint8_t sta_authmode = 0;

bool piezo_busy = false;
uint32_t piezo_timer;
const long PIEZO_DURATION = 100;

GFXcanvas1 canvasTime(100, 12);
GFXcanvas16 canvasButton(60, 20);
GFXcanvas16 canvasIn(60, 20);
GFXcanvas16 canvasAnalog(60, 20);
GFXcanvas16 canvasOutput(104, 20);
GFXcanvas16 canvasBattery(85, 22);
GFXcanvas16 canvasRelayButton(70, 25);
GFXcanvas1 canvasTemperature(112, 70);
GFXcanvas1 canvasSD(100, 24);
GFXcanvas1 canvasWiFi(160, 170);
GFXcanvas1 canvasWiFiQuality(50, 12);

struct tft_struct
{
	uint8_t tm_sec;
	boolean drawTime = false;
	uint8_t input_last[8];
	float input_voltage_last[2];
	float lastBattV = 0;
	float Temperature = 0;
	int8_t drawSD = 0;					   // 0=not drawn, 1 mounted, 2 not mounted
	int8_t drawQuality = 255;

	uint16_t btnRelay1X = 10, btnRelay1Y = 210, btnRelay1W = 50, btnRelay1H = 50;
	uint16_t btnRelay2X = 100, btnRelay2Y = 210, btnRelay2W = 50, btnRelay2H = 50;
	uint16_t btnRelay3X = 190, btnRelay3Y = 210, btnRelay3W = 50, btnRelay3H = 50;
};
struct tft_struct tftSt;
uint8_t screenRotation = 3; // 3 = landscape sd up

struct power_struct
{
	float CellVoltage = 0;
	float CellPercent = 0;
	float ChargeRate = 0;
};
struct power_struct power_st;

// RELAY
const uint8_t PROGMEM ON = 1;
const uint8_t PROGMEM OFF = 0;
struct relay_struct
{
	uint8_t state = OFF;
	uint8_t lastState = OFF;
};
struct relay_struct io_relay_struct[3];
uint8_t out_relayGPIOs[6] = { REL1_SET_PIN, REL1_RESET_PIN, REL2_SET_PIN, REL2_RESET_PIN, REL3_SET_PIN, REL3_RESET_PIN };  // Assign each GPIO to a relay

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

const uint32_t TOUCH_UPDATE = 200; // ms, dont set too high as 1000 will not trigger touchscreen
uint32_t Timer_touch;

// INPUT
const long IO_INPUT_UPDATE = 1 * 1000;
uint32_t TimerInput;

const long IO_BUTTON_UPDATE = 400;
uint32_t TimerButton;

const uint32_t WIFI_UPDATE = 5 * 1000;
uint32_t Timer_wifi;

const uint32_t TFT_UPDATE = 1000;
uint32_t Timer_tft;

uint16_t BATTERY_UPDATE = 60 * 1000;
int32_t Timer_battery = millis() - BATTERY_UPDATE;

const uint32_t TEMPERATURE_UPDATE = 60 * 1000;
int32_t TimerTemperature;
const uint32_t START_CONVERSION_UPDATE = 50 * 1000;
uint32_t TimerStartConversion;

void setupPreferences();
void handleRoot();
void handleScan();
void handleSaveNetwork();
void startAP();
void startClient();
void startServer();
void scanWiFi(uint16_t x, uint16_t y);
void setupRTC();
void setupMAX();
void setupDS248X();
void SearchDS2484();
float readTemperature(uint8_t* rom);
void Strat_Conversion(uint8_t* rom);
void setupTFT();
void setTouchRotation(uint8_t n);
void drawWiFi(uint16_t x, uint16_t y);
void drawWifiQuality(uint16_t x, uint16_t y);
void drawBattery(uint16_t x, uint16_t y, bool initDraw = false);
void drawTemperatureDetail(uint16_t x, uint16_t y, bool initDraw = false);
void drawSD(uint16_t x, uint16_t y);
void detectButtons(int16_t x, int16_t y);
void drawRedBtn(uint16_t x, uint16_t y);
void drawGreenBtn(uint16_t x, uint16_t y);
void relaySetup();

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

	get_build_date_time();
	setupPreferences();
	readPreferences();
	startAP();
	startClient();
	startServer();
	setupOTA();
	setupRTC();
	setupMAX();
	setupDS248X();
	SearchDS2484();
	Strat_Conversion(rom);
	TimerTemperature = millis() - TEMPERATURE_UPDATE + 1000;  // read temperature 1 second giving time for conversion
	setupTFT();
	drawBattery(18, 0, true);
	drawTemperatureDetail(75, 50, true);
	drawSD(220, 0);
	drawGreenBtn(tftSt.btnRelay1X, tftSt.btnRelay1Y);
	drawGreenBtn(tftSt.btnRelay2X, tftSt.btnRelay2Y);
	drawGreenBtn(tftSt.btnRelay3X, tftSt.btnRelay3Y);
	relaySetup();
}

void setupPreferences() {
	Prefs.begin("Ready_Touch", false); // Open our namespace (or create it if it doesn't exist) in RO mode.

	bool tpInit = Prefs.isKey("nvsInit"); // Test for the existence of the "already initialized" key.

	if (tpInit == false) {	// If tpInit is 'false', the key "nvsInit" does not yet exist therefore this
							//  must be our first-time run. We need to set up our Preferences namespace keys. So...
		Prefs.end();
		Prefs.begin("Ready_Touch", false); // reopen it in RW mode.

		Prefs.putBool("nvsInit", true);  // Create the "already initialized"  key and store a value.
		Prefs.end();                             // Close the namespace in RW mode and...
		Prefs.begin("Ready_Touch", true);
	}
}

void readPreferences() {
	Prefs.getString("sta_ssid", sta_ssid, 30);
	Prefs.getString("sta_pass", sta_pass, 30);
}

void writePreferences() {
	Prefs.putString("sta_ssid", sta_ssid);
	Prefs.putString("sta_pass", sta_pass);
	Prefs.end();
	readPreferences();
}

void get_build_date_time() {
	char s_month[5];
	const char date[] = __DATE__;
	const char time[] = __TIME__;

	int month, day, year, hour, min, sec;
	static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

	sscanf(date, "%s %d %d", s_month, &day, &year);
	sscanf(time, "%d:%d:%d", &hour, &min, &sec);

	// add one to month
	month = (strstr(month_names, s_month) - month_names) / 3 + 1;

	sprintf(appVersion, "%02d/%02d/%04d %02d:%02d:%02d", month, day, year, hour, min, sec);
}

//static void WiFiStationConnected(WiFiEvent_t event, wifi_sta_connected info) {
//	sta_authmode = info.authmode;
//}

void startClient() {
	if (sizeof(sta_ssid[0]) != 0) {
		Serial.println("start client");
		WiFi.disconnect(true);
		WiFi.hostname("Ready Touch 2.8");
		WiFi.enableSTA(true);
		WiFi.begin(sta_ssid, sta_pass);
		mode_STA = true;

		//WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
	}
}

// -------------------------------------------------------------------
// Start Access Point
// Access point is used for wifi network selection
// -------------------------------------------------------------------
void startAP() {
	//WiFi.mode(WIFI_AP);
	WiFi.enableAP(true);
	WiFi.softAPConfig(apIP, apIP, apNetMsk);
	WiFi.softAP(softAP_ssid, softAP_password, 1, 0, 4);
	mode_AP = true;

	// display on webpage
	switch (WiFi.getTxPower()) {
	case WIFI_POWER_19_5dBm:
		TxPower = 19.5;
		break;
	case WIFI_POWER_19dBm:
		TxPower = 19;
		break;
	case WIFI_POWER_18_5dBm:
		TxPower = 18.5;
		break;
	case WIFI_POWER_17dBm:
		TxPower = 17;
		break;
	case WIFI_POWER_15dBm:
		TxPower = 15;
		break;
	case WIFI_POWER_13dBm:
		TxPower = 13;
		break;
	case WIFI_POWER_11dBm:
		TxPower = 11;
		break;
	case WIFI_POWER_8_5dBm:
		TxPower = 8.5;
		break;
	case WIFI_POWER_7dBm:
		TxPower = 7;
		break;
	case WIFI_POWER_5dBm:
		TxPower = 5;
		break;
	case WIFI_POWER_2dBm:
		TxPower = 2;
		break;
	case WIFI_POWER_MINUS_1dBm:
		TxPower = -4;
		break;
	}
}

void startServer() {
	server.enableCORS(true);
	server.enableCrossOrigin(true);

	server.on("/", HTTP_GET, handleRoot);
	server.on("/status", HTTP_GET, handleStatus);
	server.on("/scan", HTTP_GET, handleScan);
	server.on("/savenetwork", handleSaveNetwork);
	server.on("/wifi_signal_1.svg", HTTP_GET, []() {
		server.send(200, "image/png", CONTENT_WIFI_SIGNAL_1_SVG_GZ);
		});
	server.on("/wifi_signal_2.svg", HTTP_GET, []() {
		server.send(200, "image/png", CONTENT_WIFI_SIGNAL_2_SVG_GZ);
		});
	server.on("/wifi_signal_3.svg", HTTP_GET, []() {
		server.send(200, "image/png", CONTENT_WIFI_SIGNAL_3_SVG_GZ);
		});
	server.on("/wifi_signal_4.svg", HTTP_GET, []() {
		server.send(200, "image/png", CONTENT_WIFI_SIGNAL_4_SVG_GZ);
		});
	server.on("/wifi_signal_5.svg", HTTP_GET, []() {
		server.send(200, "image/png", CONTENT_WIFI_SIGNAL_5_SVG_GZ);
		});

	server.onNotFound(handleNotFound);
	server.begin();
}

// -------------------------------------------------------------------
// Returns web root json
// url: /
// -------------------------------------------------------------------
void handleRoot() {
	server.send(200, "text/html", home);
}

// -------------------------------------------------------------------
// Returns scan json
// url: /scan
// -------------------------------------------------------------------
void handleScan() {
	server.send(200, "text/json", scanJson);
}

void returnFail(String msg) {
	server.send(500, "text/plain", msg + "\r\n");
}

void returnOK() {
	server.send(200, "text/plain", "");
}

void handleSaveNetwork() {
	char buff[31];

	if (server.args() == 0) {
		return returnFail("BAD ARGS");
	}

	if (server.hasArg("ssid")) {
		server.arg("ssid").toCharArray(buff, sizeof(server.arg("ssid"))+1);
		strcpy(sta_ssid, buff);
	}

	if (server.hasArg("pass")) {
		server.arg("pass").toCharArray(buff, sizeof(server.arg("pass"))+1);
		strcpy(sta_pass, buff);
	}
	returnOK();
	writePreferences();

	startClient();
}

void setupOTA() {
	ArduinoOTA
		.onStart([]() {
		String type;
		if (ArduinoOTA.getCommand() == U_FLASH) {
			type = "sketch";
		}

		Serial.println("Start updating " + type);
			})
		.onEnd([]() {
		Serial.println("\nEnd");
			})
		.onProgress([](unsigned int progress, unsigned int total) {
		Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
			})
		.onError([](ota_error_t error) {
		Serial.printf("Error[%u]: ", error);
		if (error == OTA_AUTH_ERROR) {
			Serial.println("Auth Failed");
		}
		else if (error == OTA_BEGIN_ERROR) {
			Serial.println("Begin Failed");
		}
		else if (error == OTA_CONNECT_ERROR) {
			Serial.println("Connect Failed");
		}
		else if (error == OTA_RECEIVE_ERROR) {
			Serial.println("Receive Failed");
		}
		else if (error == OTA_END_ERROR) {
			Serial.println("End Failed");
		}
			});

	ArduinoOTA.begin();
}

// -------------------------------------------------------------------
// Returns status json
// url: /status
// -------------------------------------------------------------------
void handleStatus() {
	// create our own json documnet
	char buff[33];  // max size to hold ssid 32 + /0
	char json[500] = "{";

	
	strcat(json, "\"appVersion\":\"");
	strcat(json, appVersion);
	strcat(json, "\"");

	strcat(json, ",\"sta_ssid\":\"");
	strcat(json, sta_ssid);
	strcat(json, "\"");
	strcat(json, ",\"sta_pass\":\"");
	strcat(json, sta_pass);
	strcat(json, "\"");

	strcat(json, ",\"in_0\":");
	sprintf(buff, "%.1f", get_input_Analog(IN0));
	strcat(json, buff);
	strcat(json, ",\"in_1\":");
	sprintf(buff, "%.1f", get_input_Analog(IN1));
	strcat(json, buff);
	strcat(json, ",\"vbus\":");
	sprintf(buff, "%.1f", get_input_Analog(SENSE_VBUS_PIN));
	strcat(json, buff);

	if (mode_ETH) {
		strcat(json, ",\"mode\":\"ETH\"");
	}
	else if (mode_STA && !mode_AP) {
		strcat(json, ",\"mode\":\"STA\"");
	}
	else if (mode_AP && !mode_STA) {
		strcat(json, ",\"mode\":\"AP\"");
	}
	else if (mode_AP && mode_STA) {
		strcat(json, ",\"mode\":\"STA+AP\"");
	}
	else {
		strcat(json, ",\"mode\":\"\"");
	}

	strcat(json, ",\"srssi\":");
	sprintf(buff, "%d", WiFi.RSSI());
	strcat(json, buff);
	strcat(json, ",\"ssid\":\"");
	WiFi.SSID().toCharArray(buff, WiFi.SSID().length() + 1);
	strcat(json, buff);
	strcat(json, "\",\"chan\":");
	sprintf(buff, "%d", WiFi.channel());
	strcat(json, buff);
	strcat(json, ",\"bssid\":\"");
	sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X", WiFi.BSSID()[0], WiFi.BSSID()[1], WiFi.BSSID()[2], WiFi.BSSID()[3], WiFi.BSSID()[4], WiFi.BSSID()[5]);
	strcat(json, buff);
	strcat(json, "\",\"txpwr\":");
	sprintf(buff, "%d", TxPower);
	strcat(json, buff);
	//strcat(json, ",\"enc\":");
	//sprintf(buff, "%s", sta_authmode);
	//strcat(json, buff);
	strcat(json, ",\"ip\":\"");
	sprintf(buff, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
	strcat(json, buff);
	strcat(json, "\",\"gw\":\"");
	sprintf(buff, "%d.%d.%d.%d", WiFi.gatewayIP()[0], WiFi.gatewayIP()[1], WiFi.gatewayIP()[2], WiFi.gatewayIP()[3]);
	strcat(json, buff);
	strcat(json, "\",\"dns\":\"");
	sprintf(buff, "%d.%d.%d.%d", WiFi.dnsIP()[0], WiFi.dnsIP()[1], WiFi.dnsIP()[2], WiFi.dnsIP()[3]);
	strcat(json, buff);
	strcat(json, "\"");

	strcat(json, "}");

	server.send(200, "text/json", json);
}

void handleNotFound() {
	char buff[50];

	char message[200] = "Not found\n\nURI: ";
	server.uri().toCharArray(buff, server.uri().length() + 1);
	strcat(message, buff);
	strcat(message, "\nMethod: ");
	strcat(message, (server.method() == HTTP_GET) ? "GET" : "POST");
	strcat(message, "\nArguments: ");
	sprintf(buff, "%d", server.args());
	strcat(message, buff);
	strcat(message, "\n");
	for (uint8_t i = 0; i < server.args(); i++) {
		server.argName(i).toCharArray(buff, server.argName(i).length() + 1);
		strcat(message, buff);
		strcat(message, ": ");
		server.arg(i).toCharArray(buff, server.arg(i).length() + 1);
		strcat(message, buff);
		strcat(message, "\n");
	}
	server.send(404, "text/plain", message);
}

void setTouchRotation(uint8_t n) {
	ts.setRotation(n);  // set ts rotation
}

// Make size of files human readable
// source: https://github.com/CelliesProjects/minimalUploadAuthESP32
String file_humanReadableSize(const size_t bytes) {
	if (bytes < 1024) return String(bytes) + " B";
	else if (bytes < (1024 * 1024)) return String(bytes / 1024.0) + " KB";
	else if (bytes < (1024 * 1024 * 1024)) return String(bytes / 1024.0 / 1024.0) + " MB";
	else return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

void piezo_click() {
	if (!piezo_busy) {
		piezo_busy = true;
		tone(PIEZO_PIN, 4000, PIEZO_DURATION);
		piezo_timer = millis();
	}
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

		// reinet array
		//char scanJson[1565] = "[";
		scanJson[0] = '[';
		scanJson[1] = (char)0;
		canvasLine = 0;
		for (int i = 0; i < n; ++i) {
			// create scanJson doc for web page wifi scan
			//char scanJson[1565] = "[";

			if (WiFi.RSSI(i) >= -80) {  // only add if dbm strong enough
				if (sizeof(scanJson) - strlen(scanJson) >= 52) { // make sure we have room to add to scanJson array
					if (i) strcat(scanJson, ",");
					strcat(scanJson, "{\"rssi\":");
					sprintf(buff, "%d", WiFi.RSSI(i));
					strcat(scanJson, buff);
					strcat(scanJson, ",\"ssid\":\"");
					WiFi.SSID(i).toCharArray(buff, WiFi.SSID(i).length() + 1);
					strcat(scanJson, buff);
					strcat(scanJson, "\"}");
				}
			}

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

			canvasLine += 1;
		}
		strcat(scanJson, "]");

		display_gfx.drawBitmap(x, y, canvasWiFi.getBuffer(), 160, 170, ILI9341_WHITE, ILI9341_BLACK);
		if (wifiScanPrint) {
			Serial.println("");
		}

		// Delete the scan result to free memory.
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

// converts the dBm to a range between 0 and 100%
int8_t getWifiQuality() {
	int32_t dbm = WiFi.RSSI();
	if (dbm <= -100) {
		return 0;
	}
	else if (dbm >= -50) {
		return 100;
	}
	else {
		return 2 * (dbm + 100);
	}
}

void drawWifiQuality(uint16_t x, uint16_t y) {
	int8_t quality = getWifiQuality();
	if (tftSt.drawQuality != quality) {
		tftSt.drawQuality = quality;
		canvasWiFiQuality.fillScreen(ILI9341_BLACK);
		//canvasWiFiQuality.drawRect(0, 0, 48, 10, ILI9341_WHITE); // just for testing the size of our area
		for (int8_t i = 0; i < 4; i++) {
			for (int8_t j = 0; j < 2 * (i + 1); j++) {
				if (quality > i * 25 || j == 0) {
					canvasWiFiQuality.drawPixel(39 + 2 * i, 10 - j, ILI9341_WHITE);
				}
			}
		}
		canvasWiFiQuality.setFont(&SansSerif_plain_10);
		canvasWiFiQuality.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
		canvasWiFiQuality.setCursor(0, 12);
		canvasWiFiQuality.print(String(quality) + "%");
		// 48 x 11 orginal exact fit
		// 1x 10font 7h
		display_gfx.drawBitmap(x, y, canvasWiFiQuality.getBuffer(), 50, 12, ILI9341_WHITE, ILI9341_BLACK);
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
		canvasSD.setCursor(0, 22);
		canvasSD.print(file_humanReadableSize(SD.cardSize()));


		display_gfx.drawBitmap(x, y, canvasSD.getBuffer(), 100, 24, ILI9341_WHITE, ILI9341_BLACK);
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
		display_gfx.drawBitmap(x, y, canvasSD.getBuffer(), 100, 24, ILI9341_RED, ILI9341_BLACK);
		tftSt.drawSD = 2;
	}
}

void relaySetup() {
	for (uint8_t i = 0; i < sizeof(out_relayGPIOs); i++) {
		pinMode(out_relayGPIOs[i], OUTPUT);
		digitalWrite(out_relayGPIOs[i], OFF);
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
	display_gfx.setRotation(screenRotation);
	display_gfx.fillScreen(ILI9341_BLACK);

	// Turn on TFT LEDs
	digitalWrite(TFT_LED, LOW);    // LOW to Turn on;

	// setup touch
	ts.begin();
	setTouchRotation(screenRotation);
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

void io_set_relay(uint8_t relay, uint8_t state)
{
	// latching relay requires two outputs
	// ON to latch then save power and turn off coil
	// Latch the ON or OFF relay position
	// manufacture specifies min 40ms on then off for latching to occure
	if (state == OFF) {
		digitalWrite(out_relayGPIOs[relay + 1], ON);
		delay(15);
		digitalWrite(out_relayGPIOs[relay + 1], OFF);
	}
	else {
		digitalWrite(out_relayGPIOs[relay], ON);
		delay(15);
		digitalWrite(out_relayGPIOs[relay], OFF);

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

void drawRedBtn(uint16_t x, uint16_t y)
{
	canvasRelayButton.fillScreen(ILI9341_BLACK);

	canvasRelayButton.drawRoundRect(0, 0, 70, 25, 12, ILI9341_RED);
	canvasRelayButton.drawRoundRect(1, 1, 68, 23, 12, ILI9341_RED);
	canvasRelayButton.fillCircle(58, 12, 7, ILI9341_RED);
	canvasRelayButton.setFont(&SansSerif_plain_16);
	canvasRelayButton.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	canvasRelayButton.setCursor(6, 19);
	canvasRelayButton.print("ON");
	canvasRelayButton.setTextColor(ILI9341_BLACK, ILI9341_BLACK);
	canvasRelayButton.setCursor(53, 17);
	canvasRelayButton.print(">");

	display_gfx.drawRGBBitmap(x, y, canvasRelayButton.getBuffer(), 70, 25);
}

void drawGreenBtn(uint16_t x, uint16_t y)
{
	canvasRelayButton.fillScreen(ILI9341_BLACK);

	canvasRelayButton.drawRoundRect(0, 0, 70, 25, 12, ILI9341_GREEN);
	canvasRelayButton.drawRoundRect(1, 1, 68, 23, 12, ILI9341_GREEN);
	canvasRelayButton.fillCircle(58, 12, 7, ILI9341_GREEN);
	canvasRelayButton.setFont(&SansSerif_plain_16);
	canvasRelayButton.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
	canvasRelayButton.setCursor(6, 19);
	canvasRelayButton.print("OFF");
	canvasRelayButton.setTextColor(ILI9341_BLACK, ILI9341_BLACK);
	canvasRelayButton.setCursor(53, 17);
	canvasRelayButton.print(">");

	display_gfx.drawRGBBitmap(x, y, canvasRelayButton.getBuffer(), 70, 25);
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

/********************************************************************
* @brief     detecting pressed buttons with the given touchscreen values
* @param  x  Top left corner horizontal coordinate.
* @param  y  Top left corner vertical coordinate.
* @return    None
*********************************************************************/
void detectButtons(int16_t x, int16_t y) {

	if (x > tftSt.btnRelay1X && x < (tftSt.btnRelay1X + tftSt.btnRelay1W) && y > tftSt.btnRelay1Y && y < (tftSt.btnRelay1Y + tftSt.btnRelay1H)) {
		// turned off to hear the relay click
		//piezo_click();
		if (io_relay_struct[0].state == ON) {
			io_set_relay(0, OFF);
			drawGreenBtn(tftSt.btnRelay1X, tftSt.btnRelay1Y);
			io_relay_struct[0].state = OFF;
		}
		else if (io_relay_struct[0].state == OFF) {
			io_set_relay(0, ON);
			drawRedBtn(tftSt.btnRelay1X, tftSt.btnRelay1Y);
			io_relay_struct[0].state = ON;
		}
	}
	else if (x > tftSt.btnRelay2X && x < (tftSt.btnRelay2X + tftSt.btnRelay2W) && y > tftSt.btnRelay2Y && y < (tftSt.btnRelay2Y + tftSt.btnRelay2H)) {
		// turned off to hear the relay click
		//piezo_click();
		if (io_relay_struct[1].state == ON) {
			io_set_relay(1, OFF);
			drawGreenBtn(tftSt.btnRelay2X, tftSt.btnRelay2Y);
			io_relay_struct[1].state = OFF;
		}
		else if (io_relay_struct[1].state == OFF) {
			io_set_relay(1, ON);
			drawRedBtn(tftSt.btnRelay2X, tftSt.btnRelay2Y);
			io_relay_struct[1].state = ON;
		}
	}
	else if (x > tftSt.btnRelay3X && x < (tftSt.btnRelay3X + tftSt.btnRelay3W) && y > tftSt.btnRelay3Y && y < (tftSt.btnRelay3Y + tftSt.btnRelay3H)) {
		// turned off to hear the relay click
		//piezo_click();
		if (io_relay_struct[2].state == ON) {
			io_set_relay(2, OFF);
			drawGreenBtn(tftSt.btnRelay3X, tftSt.btnRelay3Y);
			io_relay_struct[2].state = OFF;
		}
		else if (io_relay_struct[2].state == OFF) {
			io_set_relay(2, ON);
			drawRedBtn(tftSt.btnRelay3X, tftSt.btnRelay3Y);
			io_relay_struct[2].state = ON;
		}
	}
}

bool isTouched(int16_t debounceMillis) {
	if (ts.touched() && millis() - lastTouched > debounceMillis) {
		lastTouched = millis();
		return true;
	}
	return false;
}

void loop()
{
	if ((millis() - Timer_touch) >= TOUCH_UPDATE) {
		if (isTouched(500)) {
			TS_Point p = ts.getPoint();
			p.x = map(p.x, TS_MINX, TS_MAXX, 320, 0);
			p.y = map(p.y, TS_MINY, TS_MAXY, 240, 0);
			//Serial.println("");
			//Serial.print("map:");
			//Serial.print(p.x);
			//Serial.print(" ");
			//Serial.println(p.y);
			detectButtons(p.x, p.y);
		}
		Timer_touch = millis();
	}

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

	if ((millis() - Timer_tft) >= TFT_UPDATE) {
		drawWifiQuality(270, 0);
		drawTime(115, 0);

		Timer_tft = millis();
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
				piezo_click();
			}
			drawButton(1, IN_DIGITAL, in_value, 0, 10, 50);
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
				piezo_click();
			}
			drawButton(2, IN_DIGITAL, in_value, 0, 10, 70);
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
				piezo_click();
			}
			drawButton(3, IN_DIGITAL, in_value, 0, 10, 90);
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
				piezo_click();
			}
			drawButton(4, IN_DIGITAL, in_value, 0, 10, 110);
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
			drawInput(1, IN_ANALOG, 0, in_analog, 10, 130);
			tftSt.input_voltage_last[0] = in_analog;
		}

		// Input EOL read resistor divider
		in_value = get_input_EOL(IN0);
		if (in_value != tftSt.input_last[4]) {
			drawInput(1, IN_DIGITAL, in_value, 0, 70, 130);
			tftSt.input_last[4] = in_value;
		}

		// Analog read resistor divider
		in_analog = get_input_Analog(IN1);
		if (in_analog != tftSt.input_voltage_last[1]) {
			drawInput(2, IN_ANALOG, 0, in_analog, 10, 150);
			tftSt.input_voltage_last[1] = in_analog;
		}

		// Input EOL read resistor divider
		in_value = get_input_EOL(IN1);
		if (in_value != tftSt.input_last[5]) {
			drawInput(2, IN_DIGITAL, in_value, 0, 70, 150);
			tftSt.input_last[5] = in_value;
		}

		// 5v power analog read
		in_analog = get_input_Analog(SENSE_VBUS_PIN);
		if (in_analog != tftSt.input_voltage_last[2]) {
			drawInput(3, IN_ANALOG, 0, in_analog, 10, 170);
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

		drawTemperatureDetail(75, 50);

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
		scanWiFi(190, 40);

		Timer_wifi = millis();
	}

	server.handleClient();
	ArduinoOTA.handle();
}
