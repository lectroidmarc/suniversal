/*
    mouse handler
    Copyright (c) 2017, Alexander Vollschwitz

    This file is part of suniversal.

    suniversal is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    suniversal is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with suniversal. If not, see <http://www.gnu.org/licenses/>.
*/

#include "config.h"
#include "mouse.h"

/*
    The mouse I tested this with is a model Compact 1, SUN no. 370-1586-03.
    This one uses the 5-byte Mousesystems protocol, not the 3-byte SUN
    protocol. Here it is, collected from Linux mouse man page:

    The Mousesystems protocol uses 1 start bit, 8 data bits, no parity
    and two stop bits at the speed of 1200 bits/sec. Data is sent to RxD
    in 5-byte packets. The relative mouse movement is sent as dx (positive
    means right) and dy (positive means down). dx is sent as the sum of
    the two two's-complement values, dy is sent as negated sum of the two
    two's-complement values. lb (mb, rb) are cleared when the left (middle,
    right) button is pressed:

	 byte  d7    d6     d5     d4     d3     d2     d1     d0
	  1    1     0      0      0      0      lb     mb     rb
	  2    0    dxa6   dxa5   dxa4   dxa3   dxa2   dxa1   dxa0
	  3    0    dya6   dya5   dya4   dya3   dya2   dya1   dya0
    4    0    dxb6   dxb5   dxb4   dxb3   dxb2   dxb1   dxb0
    5    0    dyb6   dyb5   dyb4   dyb3   dyb2   dyb1   dyb0

	The implementation below will however also automatically detect when the
	3-byte SUN protocol is used.

	http://www.rosenau-ka.de/ps2sun/

*/

#define BUTTON_RIGHT_MASK  0x01
#define BUTTON_MIDDLE_MASK 0x02
#define BUTTON_LEFT_MASK   0x04

#define DATA_FRAME_START   0x80
#define FRAME_START_MASK   0xf8

#define IX_BUTTONS 0
#define IX_DX_A    1
#define IX_DY_A    2
#define IX_DX_B    3
#define IX_DY_B    4

/*

 */
MouseConverter::MouseConverter() {
	bufferIx = 0;
	frameLength = 0;
	fiveBytes = false;
}

/*

 */
MouseConverter::update(uint8_t data) {
	frameLength++;
	// we need to sync on data frame start
	if ((data & FRAME_START_MASK) == DATA_FRAME_START) {
		flushBuffer();
		buffer[0] = data;
		bufferIx = 1;
		fiveBytes = frameLength == 5; // determine protocol
		frameLength = 0;
	} else if (bufferIx > 0) {
		buffer[bufferIx++] = data;
		flushBuffer();
	}
}

/*

 */
MouseConverter::flushBuffer() {

	if ((bufferIx == 3 && !fiveBytes) || bufferIx == 5) {

		DPRINT("MouseConverter.flushBuffer: [");
		if (DEBUG) {
			for (uint8_t i = 0; i < bufferIx; i++) {
				DPRINT(" " + String(buffer[i], HEX));
			}
		}
		DPRINTLN(" ]");

		uint8_t b = buffer[IX_BUTTONS];
		handleButtons(b);

		if (EMULATE_SCROLL_WHEEL && (b & BUTTON_MIDDLE_MASK) == 0) {
			handleScroll(buffer[IX_DY_A], buffer[IX_DX_A]);
			handleScroll(buffer[IX_DY_B], buffer[IX_DX_B]);
		} else {
			handleMove(buffer[IX_DX_A], buffer[IX_DY_A]);
			if (bufferIx == 5) {
				handleMove(buffer[IX_DX_B], buffer[IX_DY_B]);
			}
		}

		bufferIx = 0;
	}
}

/*

 */
MouseConverter::handleScroll(uint8_t v, uint8_t h) {
	if (v != 0 || h != 0) {
		DPRINTLN("MouseConverter.handleScroll: [ v=" +
			String(v) + ", h=" + String(h) + " ]");
		if (INVERTED_SCROLLING) {
			usbMouse.scroll((0x100 - v) & 0xff, (0x100 - h) & 0xff);
		} else {
			usbMouse.scroll(v, h);
		}
	}
}

/*

 */
MouseConverter::handleMove(uint8_t dx, uint8_t dy) {
	if (dx != 0 || dy != 0) {
		dy = (0x100 - dy) & 0xff; // dy is negated two's complement
		DPRINTLN("MouseConverter.handleMove: [ dx="
			+ String(dx) + ", dy=" + String(dy) + "]");
		usbMouse.move(dx, dy);
	}
}

/*

 */
MouseConverter::handleButtons(uint8_t states) {
	if ((states ^ buttonStates) != 0) { // any changes at all?
		DPRINTLN("MouseConverter.handleButtons: " + String(states, HEX));
		handleButton(states, BUTTON_LEFT_MASK, MOUSE_LEFT);
		handleButton(states, BUTTON_MIDDLE_MASK, MOUSE_MIDDLE);
		handleButton(states, BUTTON_RIGHT_MASK, MOUSE_RIGHT);
		buttonStates = states;
	}
}

/*

 */
MouseConverter::handleButton(uint8_t states, uint8_t buttonMask, char buttonId) {
	if (((states ^ buttonStates) & buttonMask) != 0) { // any change for button?
		if ((states & buttonMask) == 0) {
			usbMouse.press(buttonId);
		} else {
			usbMouse.release(buttonId);
		}
	}
}

MouseConverter mouseConverter;
