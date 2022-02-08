//==============================================================================
// mcore.c
// multi model mesh demo based mesh core (version 5)
//
// Created by Hugo Pristauz on 2022-Jan-02
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================

  #include "app_gpio.c"
  #undef LOG
  #undef LOGO

  #include "ble_mesh.c"
  #undef LOG
  #undef LOGO

  #include "device_composition.c"
  #undef LOG
  #undef LOGO

  #include "blcore.c"
  #include "no_transition_work_handler.c"
  #include "publisher.c"
  #undef ONOFF

  #include "state_binding.c"
  #include "storage.c"
  #include "transition.c"
