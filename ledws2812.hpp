#ifndef __LEDWS2812_HPP__
#define __LEDWS2812_HPP__

#include <Pin.hpp>
#include <ws2812.h>

class Color {
  public:
    Color() {
      Color(  0,0,0 );
    }

    Color( uint8_t const red, uint8_t const green, uint8_t const blue ) {
      _r = red;
      _g = green;
      _b = blue;
      _intensity = 255 ;
    }

    void set(uint8_t const red, uint8_t const green, uint8_t const blue) {
      _r = red;
      _g = green;
      _b = blue;
    }

    void setRed( uint8_t const red ) {
      _r = red;
    }

    void setGreen( uint8_t const green ) {
      _g = green;
    }

    void setBlue( uint8_t const blue ) {
      _b = blue;
    }

    uint8_t red() {
      return ( (uint16_t)_r * (uint16_t)_intensity ) >> 8;
    }

    uint8_t green() {
      return ( (uint16_t)_g * (uint16_t)_intensity ) >> 8;
    }

    uint8_t blue() {
      return ( (uint16_t)_b * (uint16_t)_intensity ) >> 8;
    }

    uint8_t intensity() {
      return _intensity;
    }

    void setIntensity(uint8_t const intensity) {
      _intensity = intensity;
    }

    void clear() {
      _r = 0;
      _g = 0;
      _b = 0;
    }

    void add( Color &color ) {
      _r = constrain( (uint16_t)_r + (uint16_t)color.red(), 0 ,255 );
      _g = constrain( (uint16_t)_g + (uint16_t)color.green(), 0 ,255 );
      _b = constrain( (uint16_t)_b + (uint16_t)color.blue(), 0 ,255 );
    }

  private:
    uint8_t _r = 0;
    uint8_t _g = 0;
    uint8_t _b = 0;
    uint8_t _intensity = 0;
};

class LEDWS2812 : public Pin {

  public:

    LEDWS2812( stripLed &strip, uint32_t const index ) : Pin(NC) {
      _strip = strip;
      _index = index;
      _color.set( 0,0,0);
    }

    void begin() {
      color();
    }

    void set(uint8_t const red, uint8_t const green, uint8_t const blue) {
      _color.set( red, green, blue );
      color();
    }

    void set(Color &color_) {
      _color = color_;
      color();
    }

    void set(const Color &color_) {
      set ( (Color)color_ );
    }

    void color() {
      state = HIGH;
      _strip.setLEDcolor( _index , _color.red(), _color.green() , _color.blue() );
    }

    void intensity( uint8_t value) {
      _color.setIntensity( value );
      color();
    }

    void clear() {
      _strip.setLEDcolor( _index , 0,0,0 );
      state = LOW;
    }

    void smooth( uint32_t const period ) {
      _high = period>>9;
      if( _high < 2 ) _high = 2;
      _smoothing();
      this->Chronos::start( true );
    }

  protected:

    void output( uint8_t const state) {
      if( state == HIGH )
        color();
      else
        this->clear();
    }

    uint8_t read() {
      return state;
    }
  private:
  
    uint8_t read(uint8_t const mode) {
      return read();
    }
    void mode( uint8_t const mode_ ) { }

    void _smoothing(void) {
      this->reset();
      this->attachInterrupt(this->_high, [this](){ this->_smoothing(); } );
      _color.setIntensity( _color.intensity() + this->smoothDirection );
      if(_color.intensity() == 0xFF ) {
        smoothDirection = -1;
      }
      if(_color.intensity() == 0x00 ) {
        smoothDirection = 1;
      }
      color();
    }

    stripLed _strip;
    Color _color;
    uint32_t _index=0;
    uint32_t _high=0;
    uint32_t _low=0;
    uint32_t cycles=0;
    uint8_t smoothDirection = 1;
    uint8_t state;
};

#endif