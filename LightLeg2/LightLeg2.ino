
#include <Bounce.h>
#include <TimerOne.h>
#include <FastLED.h>
#include "map.h"


// Settings
#define BASE_RATE_HZ 600
#define REFRESH_RATE_HZ 30
#define START_ANIM_RATE_HZ 200

// Other Stuff
#define NUM_LEDS 192
#define COLUMNS 11
#define ROWS 20
#define DATA_PIN 17
CRGB leds[NUM_LEDS];
const int REFRESH_COUNTER_RESET = (BASE_RATE_HZ / REFRESH_RATE_HZ);
int anim_counter_reset = (BASE_RATE_HZ / START_ANIM_RATE_HZ);
IntervalTimer movement_ticker;
int ctr_refresh = REFRESH_COUNTER_RESET, ctr_anim = anim_counter_reset;
bool refresh_gate = false, anim_gate = false;

void tick() {
	if (!--ctr_refresh) {
		ctr_refresh = REFRESH_COUNTER_RESET;
		refresh_gate = true;
	}
	if (!--ctr_anim) {
		ctr_anim = anim_counter_reset;
		anim_gate = true;
	}
}

void setup() {
	Serial.begin(115200);
	movement_ticker.begin(tick, 1000000 / BASE_RATE_HZ);
	FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
	LEDS.setBrightness(30);
	leds[0] = CRGB::Blue;
	FastLED.show();
	delay(300);
	leds[0] = CRGB::Black;
	FastLED.show();
}

int hue = 0, idx =0;

struct Ring {
	Ring(int rate, int hue, int dHue, int saturation, int length, int center) {
		slide = rate > 0;
		_dHue = dHue;
		_hue = hue;
		_sat = saturation;
		_rate = rate;
		counter = rate;
		_len = length;
		idx = length;//min(max(abs(rate), length), NUM_LEDS - 1);
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

void column(int col, CHSV color) {
	for (int i = 0; i < ROWS; i++) {
		leds[morp[i][col]] = color;
	}
}


Ring rings[1] = {
	Ring(5, 0, 1, 255, 50, 10),
	//Ring(-40, 90, 0, 0, 10, 20),
	//Ring(-80, 90, 0, 0, 10, 20),
	//Ring(-120, 90, 0, 0, 10, 20),
	//Ring(-160, 90, 0, 0, 10, 20)
};

int boop, offset = 23;

void loop() {
	if (refresh_gate) {
		FastLED.show();
		refresh_gate = false;
	}
	if (anim_gate) {
		FastLED.clear();
		// Call Animations
		for (int i = 0; i < 1; i++) {
			rings[i].move();
		}
		
		/*
		for (int i = 0; i < COLUMNS; i++) {
			boop = hue + (offset * i);
			//if (i == 10) Serial.println(boop);
			column(i, CHSV(boop, 255, 255));
		}
		hue = (hue + 1) % 255;*/
		anim_gate = false;
	}
	// Check inputs
}