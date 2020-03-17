//Code of Group 5, EE 10115

// includes the LCD library
#include <LiquidCrystal.h>

// set up pins for LCD display
LiquidCrystal lcd(11,12,4,5,6,7);

// declare variable for storing clock mode, times
volatile int mode, clockMinute=0, clockHour=0, alarmMinute=0, alarmHour=0;
volatile int timerSecond=0, timerMinute=1, timerHour=0, lightThreshold=200;

void setup() {

// attach interrupts to pins 2 and 3
attachInterrupt(0,button1,RISING);
attachInterrupt(1,button2,RISING);

// initialize LCD display
lcd.begin(16,2);
}

void loop() {

  // declare variable to store readings from potentiometer and phototransistor
  int reading, light, lightdisplay;

  // declare static variable to store time intervals
  static int halfSec = 0;
  static int timercount = 0;

  // read potentiometer to determine reading
  reading = analogRead(A0);
  light = analogRead(A1);
  lcd.setCursor(0,0);
  if(reading < 256) {     	// clock mode
	mode = 1;
	lcd.print("Set Clock   	");

  }
  else if(reading < 512) {	// timer mode
	mode = 2;
	lcd.print("Timer   ");

	// display timer value
	if(timerHour < 10) {
  	lcd.print("0");
  	lcd.print(timerHour);
	}
	else lcd.print(timerHour);

	lcd.setCursor(10, 0);
	lcd.print(":");

	if(timerMinute < 10) {
  	lcd.print("0");
  	lcd.print(timerMinute);
	}
	else lcd.print(timerMinute);

	lcd.setCursor(13,0);
	lcd.print(":");

	if(timerSecond < 10) {
  	lcd.print("0");
  	lcd.print(timerSecond);
	}
	else lcd.print(timerSecond);

	// subtract 1 second from timer after two half-second loops
	if (timercount == 0) timercount++;
	else {
  	timercount--;
  	timerSecond--;
	}

	// roll minutes into seconds
	if(timerSecond < 0) {
  	timerSecond += 60;
  	timerMinute--;
	}

	// roll hours into minutes
	if(timerMinute < 0) {
  	timerMinute += 60;
  	timerHour--;
	}

	// sound timer once it hits zero
	if(timerHour == 0 && timerMinute == 0 && timerSecond == 0) {
  	tone(13, 4200, 5000);
  	timerMinute = 1;
  	timerSecond = 5;
	}
  }

  else if (reading < 768) { 	// alarm mode
	mode = 3;
	lcd.print("Alarm      ");

	// display alarm time
	if(alarmHour < 10) {
  	lcd.print("0");
  	lcd.print(alarmHour);
	}
	else lcd.print(alarmHour);

	lcd.setCursor(13, 0);
	lcd.print(":");

	if(alarmMinute < 10) {
  	lcd.print("0");
  	lcd.print(alarmMinute);
	}
	else lcd.print(alarmMinute);

	// sound alarm
	if(alarmMinute == clockMinute && alarmHour == clockHour) tone(13, 4200, 500);
  }

  else {                      	// light alarm mode
	mode = 4;
	lcd.print("Light Alarm   ");

	// display light threshold value on scale of 1-10
	lcd.setCursor(14,0);
	if (lightThreshold < 999) {
  	lightdisplay = lightThreshold/100;
  	lcd.print("0");
  	lcd.print(lightdisplay);
	}
	else {
  	lightdisplay = lightThreshold/1000;
  	lcd.print(lightdisplay);
	}

	// sound alarm for light threshold
	if (light > lightThreshold) {tone(13, 4200, 5000);}
  }

  // display clock time
  lcd.setCursor(0, 1);
  if(clockHour < 10) {
	lcd.print("0");
	lcd.print(clockHour);
  }
  else lcd.print(clockHour);

  lcd.setCursor(2, 1);
  lcd.print(":");

  if(clockMinute < 10) {
	lcd.print("0");
	lcd.print(clockMinute);
  }
  else lcd.print(clockMinute);

  // update clock values; loop function executes twice each second
  delay(500);
  halfSec++;
  if(halfSec >= 120) {
	clockMinute++;
	halfSec -= 120;
  }
  if(clockMinute >= 60) {
	clockHour++;
	clockMinute -= 60;
  }
  if(clockHour >= 24) {
	clockHour -= 24;
  }
}

void button1() {

// determine current clock mode using IF conditionals
// button1 increases hour counters or decreases voltage threshold for phototransistor
if (mode == 1) clockHour++;
else if (mode == 2) timerHour++;
else if (mode == 3) alarmHour++;

// ensure voltage threshold never goes below reasonable input voltage
else if (lightThreshold <= 200) lightThreshold = 100;
else lightThreshold -= 100;
}

void button2() {
// determine current clock mode using IF conditionals
// button1 increases minute counters or increases voltage threshold for phototransistor
if (mode == 1) clockMinute++;
else if (mode == 2) timerMinute++;
else if (mode == 3) alarmMinute++;

// ensure voltage threshold never goes above reasonable input voltage
else if (lightThreshold >= 900) lightThreshold = 1000;
else lightThreshold += 100;
}
