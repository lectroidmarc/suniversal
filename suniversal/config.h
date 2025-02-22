#ifndef SUNIVERSAL_h
#define SUNIVERSAL_h

// Set whether to activate debug mode. In debug mode, diagnostic messages are
// written to the serial port. Additionally, power button will turn into reset
// button for the keyboard, so it's easier to observe start up messages.
//
#define DEBUG false


// Set whether to use the mouse (if one is plugged into the keyboard).
//
#define USE_MOUSE true


// Set whether to emulate a mouse scroll wheel, i.e. scroll up/down and
// left/right, when the mouse is moved up/down and left/right with the
// middle mouse button being pressed.
//
#define EMULATE_SCROLL_WHEEL true


// Set whether to invert scrolling. When set to true, the scrolling direction
// is applied to the page instead of the view port. That is, moving the mouse
// up/down with middle button pressed scrolls down/up, and moving left/right
// scrolls right/left.
//
#define INVERTED_SCROLLING false


// Set whether to use macros instead of single codes for the special keys in the
// fun cluster.
//
#define USE_MACROS true


// When compose mode is true, the LED will turn on when the key is pressed, and
// go off after the next two key strokes, or when Compose is pressed again. This
// is meant for when you assign the key to actual compose on the host. When
// false, the Compose key LED will never be on.
//
#define COMPOSE_MODE false

// Set whether the state of the compose LED should be controlled by the host
// (true). If this does not work (depending on the host OS, the compose LED may
// be kept off all the time), set this to false to let suniversal control the
// LED.
#define COMPOSE_LED_HOST_CONTROLLED true

// Set whether to have the keyboard blink & beep at start up.
//
#define STARTUP_GREETING false


// SUN keyboard layouts
//
#define UNITED_STATES       0b00000
#define FRENCH_BELGIUM      0b00010
#define CANADA_FRENCH       0b00011
#define DENMARK             0b00100
#define GERMANY             0b00101
#define ITALY               0b00110
#define NETHERLANDS         0b00111
#define NORWAY              0b01000
#define PORTUGAL            0b01001
#define SPAIN_LATIN_AMERICA 0b01010
#define SWEDEN_FINLAND      0b01011
#define SWISS_FRENCH        0b01100
#define SWISS_GERMAN        0b01101
#define UNITED_KINGDOM      0b01110
#define GET_FROM_KEYBOARD   0b11111


// Use this to force a particular layout, instead of using the layout reported
// by the keyboard according to its DIP switches. Specify the desired layout
// with one of the layout defines from the list above. Otherwise, leave as
// GET_FROM_KEYBOARD.
//
#define FORCE_LAYOUT GET_FROM_KEYBOARD


// --- debug helpers ---------------------------------------------------------

#if DEBUG == true

#include <Arduino.h>

#define DPRINT(...)    Serial.print(__VA_ARGS__)
#define DPRINTLN(...)  Serial.println(__VA_ARGS__)

#else

#define DPRINT(...)
#define DPRINTLN(...)

#endif


// --- other helpers ---------------------------------------------------------

#define array_len( x )  ( sizeof( x ) / sizeof( *x ) )


// ---------------------------------------------------------------------------

#endif
