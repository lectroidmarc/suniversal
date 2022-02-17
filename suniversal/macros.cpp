/*
    macros - macro definitions for special keys
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
#include "macros.h"

// individual macros
static const uint16_t macro_again[]   = {COMMAND, USB_G, 0};
static const uint16_t macro_undo[]    = {COMMAND, USB_Z, 0};
static const uint16_t macro_undo_fr[] = {COMMAND, USB_W, 0};
static const uint16_t macro_copy[]    = {COMMAND, USB_C, 0};
static const uint16_t macro_paste[]   = {COMMAND, USB_V, 0};
static const uint16_t macro_cut[]     = {COMMAND, USB_X, 0};

static const uint16_t macro_stop[]    = {CONTROL, USB_C, 0};
static const uint16_t macro_props[]   = {COMMAND, USB_COMMA, 0};
static const uint16_t macro_front[]   = {CONTROL, USB_DOWN, 0};
static const uint16_t macro_open[]    = {COMMAND, USB_O, 0};
static const uint16_t macro_find[]    = {COMMAND, USB_F, 0};

static const uint16_t macro_help[]    = {SHIFT, COMMAND, USB_SLASH, 0};
//

MacroTable::MacroTable() {
    DPRINTLN("populating macro table");
    table[MACRO_AGAIN] = macro_again;
    table[MACRO_UNDO]  = macro_undo;
    table[MACRO_COPY]  = macro_copy;
    table[MACRO_PASTE] = macro_paste;
    table[MACRO_CUT]   = macro_cut;
    table[MACRO_STOP]  = macro_stop;
    table[MACRO_PROPS] = macro_props;
    table[MACRO_FRONT] = macro_front;
    table[MACRO_OPEN]  = macro_open;
    table[MACRO_FIND]  = macro_find;
    table[MACRO_HELP]  = macro_help;
}

uint16_t* MacroTable::get(uint8_t ix) {
    return table[ix];
}

/*
    For the different keyboard layouts see:
        https://docs.oracle.com/cd/E19683-01/806-6642/new-357/index.html
 */
MacroTable::adjustToLayout(uint8_t layout) {
    switch (layout) {
        case FRENCH_BELGIUM:
            DPRINTLN("adjusting macro for French/Belgium layout");
            table[MACRO_UNDO] = macro_undo_fr;
            break;

        case GERMANY:
        case SWISS_FRENCH:
        case SWISS_GERMAN:
            DPRINTLN("adjusting macro for German/Swiss layout");
            table[MACRO_AGAIN] = macro_undo;
            table[MACRO_UNDO] = macro_again;
            break;
/*
    currently, nothing to do for these layouts

        case CANADA_FRENCH:
        case DENMARK:
        case ITALY:
        case NETHERLANDS:
        case NORWAY:
        case PORTUGAL:
        case SPAIN_LATIN_AMERICA:
        case SWEDEN_FINLAND:
        case UNITED_KINGDOM:
            break;
 */
        default:
            break;
    }
}
