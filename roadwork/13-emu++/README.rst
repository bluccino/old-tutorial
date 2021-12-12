//==============================================================================
// 09-emu++ Tiny Onoff Control Demo with Emulation Core (C++ version)
//==============================================================================

  A single LED is controlled via a button, where single button press turns LED
  on, and double press turns LED off. The demo version is based on the emu-
  lation core ecore1.c

  Emulation of a provisioned mode:
    - button presses are simulated by the application
    - single button press turns 'configured' LED on
    - double button press turns 'configured' LED off
    - and LED(i) is 'configured' if the according generic on-off server
      has been assigned with app key and subscribed to a group
    - operation is network wide


Test:
=====
