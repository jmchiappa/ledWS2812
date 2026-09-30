#include <ledws2812.hpp>


Color OK(0,255,0);
Color KO(255,0,0);
Color CYAN(0,128,255);
Color color[] = { OK, KO };

const uint8_t INDEX_LED_HEALTH = 7;
const uint8_t INDEX_LED_STATE = 0;
const uint8_t INDEX_LED_SMOOTH = 4;
const uint8_t INDEX_LED_TOGGLE = 2;


uint8_t myStripLedArray[BUF_SIZE(8)];
stripLed myStripLed(myStripLedArray,sizeof(myStripLedArray),WS_GRB);

LEDWS2812 health(myStripLed,  INDEX_LED_HEALTH );
LEDWS2812 state( myStripLed, INDEX_LED_STATE );
LEDWS2812 smooth( myStripLed, INDEX_LED_SMOOTH );
LEDWS2812 toggle( myStripLed, INDEX_LED_TOGGLE );

uint8_t intensity = 1;
uint8_t cnt;

void setup() {
  myStripLed.begin();
  myStripLed.Fill( 0,0,0);
  health.begin();
  state.begin();
  smooth.begin();
  toggle.begin();
  toggle.set( CYAN );
  smooth.set( CYAN );
  smooth.smooth(2000);
  health.set( OK );
  health.setDuration( 15,700, true );
}

void loop() {
  toggle.toggle();
  state.set( color[cnt++] );
  cnt %= 2;
  state.blink( 200 ,500, 3 );
  state.waitUntilEndOfBlink();
}