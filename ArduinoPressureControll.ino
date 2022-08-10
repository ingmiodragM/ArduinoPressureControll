#include "gdb.h"
#include "Arduino.h"
#include "EEPROM.h"
#include <Keypad.h>
#include <UTFT.h>
#include <URTouch.h>

UTFT myGLCD(ILI9341_16, 38, 39, 40, 41);
URTouch myTouch(6, 5, 4, 3, 2);
extern uint8_t BigFont[];

int x, y, Ekran = 0;
char stCurrent[20] = "";
int stCurrentLen = 0;
char stLast[20] = "";
float stringu;
char customKey;
int upisbr;
int stanje;

float opsegf;
float pressurezf;
double pressure;
double opseg;

const int relej = 8;

const float x1 = 0.343;
const float y1 = 0.52;
const float x2 = 0.407;
const float y2 = 0.70;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = { { '*', '7', '4', '1' }, { '0', '8', '5', '2' }, {
		'#', '9', '6', '3' }, { 'D', 'C', 'B', 'A' } };
byte rowPins[ROWS] = { 18, 19, 20, 21 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 17, 16, 15, 14 }; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS,
		COLS);

void updateStr(int val) {

	if (stCurrentLen < 20) {
		stCurrent[stCurrentLen] = val;
		stCurrent[stCurrentLen + 1] = '\0';
		stCurrentLen++;

		switch (upisbr) {
		case 1: {
			opsegf = atof(stCurrent);
			// unosim prva tri u eprom
			// writtepressure();
			EEPROM.write(0, stCurrent[0]);
			EEPROM.write(1, stCurrent[1]);
			EEPROM.write(2, stCurrent[2]);
			break;
		}
		case 2: {
			pressurezf = atof(stCurrent);
			//unosim druga tri u eprom
			//  writtehisteresis();
			EEPROM.write(3, stCurrent[0]);
			EEPROM.write(4, stCurrent[1]);
			EEPROM.write(5, stCurrent[2]);
			break;
		}
		}
		//stCurrent[0]='0.'
//    myGLCD.setColor(0, 255, 0);
		//  myGLCD.print(stCurrent, LEFT, 224);
		//  myGLCD.print(stCurrent, 105, 110);
		//   zadatipritisak=atof(stCurrent);
	}
}

void Resetujstring() {
	stCurrent[0] = '\0';
	stCurrentLen = 0;
}

void pressurecheck(int stanje, int upisbr) {

	if (stanje == 1) {
		double zadatipritisak;
//	double opseg;

		//opseg=opsegf;
		zadatipritisak = (pressurezf / 1000);
		//opseg=zadatipritisak+(opsegf/1000);

		float senzor = analogRead(A2);
		// Serial.println(senzor);
		float voltage = senzor * 5 / 1024;
		//Serial.println(voltage);
		float pritisak = ((y2 - y1) / (x2 - x1) * (voltage - x1)) + y1 - 0.05;
		//Serial.println(pritisak);
		if (abs(pritisak) <= zadatipritisak) {
			digitalWrite(relej, LOW);
		} else {

			if (abs(pritisak) >= (zadatipritisak + (opsegf / 1000)))

			{
				digitalWrite(relej, HIGH);
			}
		}

		myGLCD.printNumF(pritisak, 2, 105, 200);    ///upisuje pritisak
		// Serial.println(pritisak);
		//  Serial.println(opseg);
		//opsegf=opsegf;
		pressure = pritisak;
	}

}

void drawButtons() {
	myGLCD.setColor(0, 0, 255);
	myGLCD.setBackColor(0, 0, 255);
	myGLCD.fillRoundRect(100, 5, 200, 50);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(100, 5, 200, 50);
	myGLCD.drawCircle(50, 22, 20);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("Podesen prit. mBar", 20, 60);
	myGLCD.setBackColor(0, 0, 255);

	myGLCD.setColor(0, 0, 255);
	myGLCD.setBackColor(0, 0, 255);
	myGLCD.fillRoundRect(100, 90, 200, 135);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(100, 90, 200, 135);
	myGLCD.drawCircle(50, 115, 20);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("Histerezis mBar", 18, 140);
	myGLCD.setBackColor(0, 0, 255);
	myGLCD.setColor(0, 0, 0);
	// myGLCD.fillCircle(50,115,20);
	myGLCD.setColor(0, 0, 255);

	myGLCD.setColor(0, 0, 255);
	myGLCD.setBackColor(0, 0, 255);
	myGLCD.fillRoundRect(100, 175, 200, 220);
	myGLCD.setColor(255, 255, 255);
	myGLCD.drawRoundRect(100, 175, 200, 220);
	myGLCD.drawCircle(50, 197, 20);
	myGLCD.setBackColor(0, 0, 0);
	myGLCD.print("Trenutni prit. Bar", 20, 223);
	myGLCD.setBackColor(0, 0, 255);

}
void writecircle() {
	switch (upisbr) {
	case 1:

	{
		myGLCD.drawCircle(50, 22, 20);
		myGLCD.setColor(0, 255, 0);
		myGLCD.fillCircle(50, 22, 20);
		myGLCD.setColor(0, 0, 0);
		myGLCD.fillCircle(50, 115, 20);
		upisbr = 2;
		break;
	}

	case 2: {
		myGLCD.drawCircle(50, 115, 20);
		myGLCD.setColor(0, 255, 0);
		myGLCD.fillCircle(50, 115, 20);
		myGLCD.setColor(0, 0, 0);
		myGLCD.drawCircle(50, 22, 20);
		myGLCD.fillCircle(50, 22, 20);
		upisbr = 1;
		break;
	}
	}
}

void obrisipolja(int polje) {
	if (polje == 1) {
		myGLCD.setColor(0, 0, 255);
		myGLCD.setBackColor(0, 0, 255);
		myGLCD.fillRoundRect(100, 5, 200, 50);
		myGLCD.setColor(255, 255, 255);
		myGLCD.drawRoundRect(100, 5, 200, 50);

	}

	if (polje == 2) {

		myGLCD.setColor(0, 0, 255);
		myGLCD.setBackColor(0, 0, 255);
		myGLCD.fillRoundRect(100, 90, 200, 135);
		myGLCD.setColor(255, 255, 255);
		myGLCD.drawRoundRect(100, 90, 200, 135);

	}

	// myGLCD.fillCircle(50,115,20);

}
void setup() {
	myGLCD.InitLCD();
	myGLCD.clrScr();
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);
	myGLCD.setFont(BigFont);
	myGLCD.setBackColor(0, 0, 255);
	drawButtons();

	pinMode(relej, OUTPUT);
	Serial.begin(9600);
	upisbr = 2;
	stanje = 2;
	setupbutton();
	readpressure();
	opsegf = atof(stCurrent);
	myGLCD.print(stCurrent, 105, 110);
//Resetujstring();
	upisbr = 1;
	stanje = 2;
	readhisteresis();
	pressurezf = atof(stCurrent);
	myGLCD.print(stCurrent, 105, 20);
//Resetujstring();
	upisbr = 2;

}

void setupbutton() {
	myGLCD.drawCircle(50, 22, 20);
	myGLCD.setColor(0, 255, 0);
	myGLCD.fillCircle(50, 22, 20);

	myGLCD.setColor(0, 0, 0);
	myGLCD.fillCircle(50, 115, 20);

	myGLCD.setColor(255, 255, 255);
	myGLCD.drawCircle(50, 115, 20);

	// opsegf=atof(stCurrent);

}
void writtepressure() { //Copy function
	for (int i = 0; i < 3; i++) {
//	  arrayOriginal[]
		EEPROM.write(i, stCurrent[i]);

		//tableauCopie[i]=tableauOriginal[tableau_2[i]];
		// Serial.print(arrayOriginal[i]);
		delay(50);
	}
}

void writtehisteresis() { //Copy function

	for (int i = 0; i < 3; i++) {

		EEPROM.write(i + 3, stCurrent[0]);
		EEPROM.write(i + 3, stCurrent[1]);
		EEPROM.write(i + 3, stCurrent[2]);

		//tableauCopie[i]=tableauOriginal[tableau_2[i]];
		//Serial.print(arrayOriginal[i]);
		delay(50);
	}
}

void readpressure() { //Copy function

	stCurrent[0] = EEPROM.read(0);
	stCurrent[1] = EEPROM.read(1);
	stCurrent[2] = EEPROM.read(2);
//	  if (stCurrent[i]=="\0")
//	  {
//		  stCurrent	[i]=0;
//	  };

	//tableauCopie[i]=tableauOriginal[tableau_2[i]];
	// Serial.print(arrayOriginal[i]);
	delay(50);

}

void readhisteresis() { //Copy function

	stCurrent[0] = EEPROM.read(3);
	stCurrent[1] = EEPROM.read(4);
	stCurrent[2] = EEPROM.read(5);

	//tableauCopie[i]=tableauOriginal[tableau_2[i]];
	//Serial.print(arrayOriginal[i]);
	delay(50);

}

void loop() {

	while (true) {
		customKey = customKeypad.getKey();
// updateStr(customKey);
		if (customKey) {
			switch (customKey) {
			case 'A': {
				stanje = 1;
				pressurecheck(1, upisbr);
				//Serial.println(opsegf);
				// Serial.print("////////////////");
				// Serial.println(pressurezf);
				break;
			}

			case 'B': {

				switch (upisbr) {
				case 1:

				{
					myGLCD.drawCircle(50, 22, 20);
					myGLCD.setColor(0, 255, 0);
					myGLCD.fillCircle(50, 22, 20);

					myGLCD.setColor(0, 0, 0);
					myGLCD.fillCircle(50, 115, 20);

					myGLCD.setColor(255, 255, 255);
					myGLCD.drawCircle(50, 115, 20);

					// opsegf=atof(stCurrent);
					upisbr = 2;
					obrisipolja(1);
					Resetujstring();

					break;
				}

				case 2: {
					myGLCD.setColor(255, 255, 255);
					myGLCD.drawCircle(50, 115, 20);

					myGLCD.setColor(0, 255, 0);
					myGLCD.fillCircle(50, 115, 20);

					myGLCD.setColor(0, 0, 0);
					myGLCD.fillCircle(50, 22, 20);

					myGLCD.setColor(255, 255, 255);
					myGLCD.drawCircle(50, 22, 20);

					//	    pressurezf=atof(stCurrent);
					upisbr = 1;
					obrisipolja(2);
					Resetujstring();

					break;
				}
				}

				break;
			}

			case 'C': {
				//opsegf=0;
				stanje = 2;
				digitalWrite(relej, LOW);
				//pressurecheck(2,upisbr);
				break;
			}

			case '*': {
				Resetujstring();
				obrisipolja(1);
				obrisipolja(2);
				break;
			}

			case '#': {

				break;
			}

			case 'D': {
				readpressure();
				pressurezf = atof(stCurrent);
				readhisteresis();
				opsegf = atof(stCurrent);
				break;
			}

			default:

				// Serial.println(customKey);

				if (stCurrentLen > 2)   ////////proveri duzinu stringa
						{

					Resetujstring();
					obrisipolja(1);
					obrisipolja(2);
				} else {
					updateStr(customKey);
				}
				break;

			}

		} else {
			//pressurecheck();
			// break;
		}
		if (stanje == 1) {
			pressurecheck(1, upisbr);
		}
		myGLCD.printNumF(pressure, 2, 105, 200);  ///loop upisuje pritisak
		if (upisbr == 1) {
			myGLCD.print(stCurrent, 105, 110);
			//	myGLCD.printNumI(upisbr, 230, 110);
			myGLCD.printNumF(opsegf, 3, 220, 110);
			myGLCD.printNumF(pressurezf, 3, 220, 20);
			myGLCD.printNumI(stanje, 220, 160);

			//Serial.println(stCurrent);
			// delay(1000);
			//Serial.println("====================");
		}
		if (upisbr == 2) {
			myGLCD.print(stCurrent, 105, 20);
			//	   myGLCD.printNumI(upisbr, 230, 20);

			myGLCD.printNumF(opsegf, 3, 220, 110);
			myGLCD.printNumF(pressurezf, 3, 220, 20);
			myGLCD.printNumI(stanje, 220, 160);
		}

	}
}

