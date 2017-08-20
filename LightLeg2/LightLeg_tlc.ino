
//#include <OctoWS2811.h>
#include "FastLED.h"
#include "map.h"

#define NUM_LEDS 192
#define DATA_PIN 7
CRGB leds[NUM_LEDS];

#define COLUMNS 11
#define ROWS 20

int col = 0, hue = 1;

void column(int col, CHSV color) {
	for (int i = 0; i < ROWS; i++) {
		leds[morp[i][col]] = color;
	}
}

struct Ring {
	Ring(int rate, int hue, int dHue, int saturation, int length, int center) {
		slide = rate > 0;
		_dHue = dHue;
		_hue = hue;
		_sat = saturation;
		_rate = rate;
		counter = rate;
		_len = length;
		idx = min(max(abs(rate), length), NUM_LEDS - 1);
		centerWidth = center;
		tail = (length - centerWidth) / 2;
		tailD = 255 / tail;
	}

	int _hue, _dHue, idx = 80, _rate, _len, counter, tail, tailD, centerWidth, _sat;
	bool down = true, slide;
	void move() {
		int tIdx, brightness;
		for (int i = 0; i < _len; i++) {
			if (i <= tail) {
				brightness = i*tailD;
			}
			else if (i <= tail + centerWidth) {
				brightness = 255;
			}
			else {
				brightness = 255 - ((i - (tail + centerWidth)) * tailD);
			}
			leds[idx + (down ? -i : i)] = CHSV(_hue, _sat, brightness);
		}

		if (slide) {
			if (--counter <= 0) {
				counter = _rate;
				_hue = (_hue + _dHue) % 255;
				if ((down && (idx == NUM_LEDS - 1)) || (!down && (idx == 0))) {
					idx = (down ? NUM_LEDS - _len : _len);
					down = !down;
				}
				idx += (down ? 1 : -1);
			}
		}
	}
};

Ring rings[5] = {
	Ring(3, 0, 1, 255, 50, 50),
	Ring(-40, 90, 0, 0, 10, 20),
	Ring(-80, 90, 0, 0, 10, 20),
	Ring(-120, 90, 0, 0, 10, 20),
	Ring(-160, 90, 0, 0, 10, 20)
};

void setup() {
	Serial.begin(115200);
	FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
	LEDS.setBrightness(30);

	FastLED.clear();
	FastLED.show();
	delay(100);


}

int offset = 23, boop = 0, curIdx = 0;
bool toggle = true;
void loop() {

	for (int i = 0; i < 256; i++) {
		fill_solid(leds, NUM_LEDS, CHSV(0, 0, sine[i]));
		rings[0].move();
		column(3, CHSV(85, 255, 255));
		FastLED.show();



		delay(10);
	}
	//boop = (boop + 1) % 256;

	/*
	column(6, CHSV(85, 255, 255));
	column(9, CHSV(85, 255, 255));

	for (int i = 0; i < 5; i++) {
	rings[i].move();
	}


	FastLED.clear();
	//delay(1);

	/*
	for (int i = 0; i < COLUMNS; i++) {
	boop = hue + (offset * i);
	if (i == 10) Serial.println(boop);
	column(i, CHSV(boop, 255, 255));
	}


	delay(30);
	//FastLED.clear();
	hue = (hue + 1) % 255;
	*/
}