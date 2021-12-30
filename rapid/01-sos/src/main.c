//==============================================================================
// main.c (SOS app - rapid prototyping demo)
//==============================================================================
//
// SOS app controls the blinking pattern of one selected LED of a multi LED
// board (like Nordic NRF52DK_NRF52832 development kit) according to the
// following blinking pattern:
//
// LEVEL
//  ^
//  |== +== +==   +======     +======     +======   +== +== +==        +==
//  | | | | | |   |     |     |     |     |     |   | | | | | |        | |
// -+-+==-+==-+====-----+======-----+======-----+====-+==-+==-+===...===-+=...>
//
// The tasks of the application are assigned to two modules:
//
// - module SOS: deals with timing (derived from a timed TICK input) and gene-
//   rates a sequence of binary levels according to the SOS pattern. Input INIT
//   is used to select one of the multiple LEDs at initial time for performing
//   the blinbk task
//
// - module LED: receives LEVEL events and executes it as on/off switch actions
//   of the configured LED.
//
//                    +-------------+         +-------------+
//             INIT -->             |  LEVEL  |             |
//                    |     SOS     |--------->     LED     |
//    (500 ms) TICK -->             |         |             |
//                    +-------------+         +-------------+
//
// main taks:
// - 1) setup a main engine with 500 ms period tick generatiom
// - 2) main engine to call Bluccino and app initializers
// - 3) main engine to call Bluccino ticker and app tick() function
// - 4) app init() to init SOS and LED module
// - 5) app init() to setup message forwarding from SOS module to LED module
// - 6) app tick() to tick SOS module
//
//==============================================================================

  #include "bluccino.h"

//==============================================================================
// main engine
// - calling Bluccino init and app init() function
// - periodic (500ms) calls of app tick() function
//==============================================================================

  void main(void)
  {
    bl_prt("SOS rapid prototyping\n");
  }
