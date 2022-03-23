Zephyr Demo Changelog
=====================

Contents
- original Zephyr demo examples
- traffic test examples

================================================================================
                                Version 1.2
================================================================================

Targets
=======
- re-make demos 01-onoff, 02-multi, 03-alive, 04-togglerun with gear based
  blucino library

Changelog
=========
- update .edit and .backup file
- [###] 'first commit - Zdemo.V1.2.Bak00'
- blucino/03-hello demo (hello Blucino!)
- blucino/04-sos demo (using Blucino library)



################################################################################
                               Older Versions
################################################################################

================================================================================
                                Version 1.1
================================================================================

Targets
=======
+ start of blucino V0.0
+ blucino demos 01-onoff, 02-multi, 03-alive, 04-toggle
- analyse demo 07-speed preventing from repeated sending
- analyse demo 06-onoff-org freeing up from zlib
- analyse demo 08-onoff-work with worker decoupling for notifications
- blucino demo 05-prov with bcdebug.c/.h module
- blucino demo 06-gpio with bcgpio.c/.h module
- blucino based analyse/05-onooff-flood

Changelog
=========
- update .edit and .backup file
- build & run analyse/05-onooff-flood
- build & run analyse/06-onooff-org
- [###] 'first commit - Zdemo.V1.1.Bak00'
- copy onoff-app and multi-app to blucino/01-onoff, blucino/02-multi
- create blucino/03-alive
- create blucino/04-toggle
+ blucino demos 01-onoff, 02-multi, 03-alive, 04-toggle
- [###] 'blucino demos 01-onoff, o2-multi, 03-alive, 04-toggle - Zdemo.V1.1.Bak01'
- organizing code for analyse/06-onoff-org into src and zlib dir
- add scan hook to advertiser (adv.c) in Zephyr branch zephyr-v2.7.0-hooks
- adc.c with hooks copied to 06-tweak
- in iBeacon set advertising interval to [40 50] => works!
- in iBeacon set advertising interval to [40 50] => works!
- 06-tweak now receiving 100 #/s :-)
- blucino/04-toggle
- blucino/05-log - add logging module bllog.c/.h
- bug fix 05-log: increase stack from 512B to 1024B to avoid stack crash
- new object style 05-log
- 06-log - blucino.cpp first time (global instance constructors not called)
- got C++ with global instance initialising running :-)))))
- super first C++ Blucino kernel
- super, super, super Blucino C++ implementation of 06-log
- demo 08-quad++ started with improved ocore4
- demo 08-quad++ on a good level :-)
- demo 07-tiny++ startet (derive from 08-quad++)
- add an Led class to Blucino and do basic tests
- traffic/02-quad fixed
- led bug fixed in blucino/07-tiny++
- blucino/07-tiny++ running in mesh - marvelous :-)))))
- beginning 08-goosrv++ and 09-goocli++
- fixed 07-clisrv++, 08-goosrv++, 09-goocli++
- 07-clisrv++, 08-goosrv++, 09-goocli++ completed
- [###] 'tag Zstuff-V1.1 - Zdemo.V1.1.Bak02'

================================================================================
                                Version 1.0
================================================================================

Changelog
=========
- make first traffic demo 01-onoff
- documentation about provisioning and testing
- make .edit and .backup file
- traffic demo 01-onoff working :-)))
- begin traffic demo 02-post
- heart beat traces
- receive goo message
- button press notification in kernel
- separated publish function in main
- kernel function for LED on/off
- introduction of color traces and debug levels
- change button logic from double-press to long-press did not work
- sequencer which sends 50 packets per second
- some prettyfication
- [###] 'Nice 02-quad demo - Zdemo.V1.0.Bak00'
- start 03-flood demo
- improving kern.c with smart trace function, including time stamps
- bug: MPU fault (when activating enable)
- bug: crashes with hi frequency -> revert back to 01-onoff stage
- overwrite 03-flood/src/mesh.c with 01-onoff/src/main.c
- rename main() as mesh_init()
- insert #include statements for mesh.h and kern.h
- insert mesh_prov(), mesh_get(), mesh_set(), and a dummy for mesh_pub()
- decrease button debouncing delay
- insert many header comments between functions for better structuring
- no more set gpio in goo_set_unack(), but call km_receive() instead
- change local name 'onoff_state' in gen_onoff_set_unack() to 'p'
- trigger on rising and falling button edges
- change button press logic
- more traces between #ifdefs
- everything re-assembled except publishing
- publication re-implemented
- first mesh traffic
- first "flood" (9#/s), sometimes out of buffers
- increase L2CAP buffer size from 8 to 50
- increase mesh ADV buffer count from 20 to 50
- first nice test with 18#/s :-))), even buffer overflow
- [###] 'Nice 03-flood demo - Zdemo.V1.0.Bak01'
- starting zlib V16.1
- forget zlib V16.1, we will continue with zlib V2.0
- made all demos in sample/v1.14.1 for Zephyr v1.14.0 running :-)))))
- [###] 'sample/v1.14.1 demos for Zephyr v1.14.0 all running - Zdemo.V1.0.Bak02'
- rename Zdemo folder to Zstuff
- copy original onoff_app from v1.14.0 Zephyr repository
- v1.14.0/01-onoff_app successfully built and tested
- 02-quad and 03-flood implemented, but floof crash :-(
- change kn_verbose() to implicitely support colors
- simplification of types in kern.h
- change .backup to make Zstuff.Vx.y.Bakxx file (instead of Zdemo...bak)
- [###] 'type simplification in kern.h - Zstuff.V1.0.Bak03'
- tweak adv.c to call kn_tweak()
- super session - iBeacon and 06-tweak running :-)))
- 04-pub sample running in a first stage
- rename 04-pub to 04-gpio
- 04-gpio super basis, and super working :-)
- lots of demo renaming ...
- got 04-pub running, but with error messages from stack
- message drops are based on lacking kernel sleep time
- significant progresses with 04-pub
- reduced sleep in mf_loop() call
- CONFIG_LOG_BUFFER_SIZE=2048 set to prevent segger RTT from message drop
- when switching back to mf_sleep(1) in main loop, we had no segger RTT
  message drops after provisioning, and after CLI config. But when we config'ed
  the GOOSRV then there were frequent message drops
- after increasing mf_sleep(10) to 10ms, we still had 6-9 messages dropped
  on regulary basis by segger RTT
- now increase CONFIG_LOG_BUFFER_SIZE=4096 to prevent seggerRTT from msg drop
- switch mf_sleep(1) back to 1ms
- some color printk fixes in mfmesh.c
- increasing CONFIG_LOG_BUFFER_SIZE=4096 solved segger RTT message drops :-)))
- rearranging samples and traffic demos by introducing legacy
- [###] 'traffic/04-pub, a big milestone :-))))) - Zstuff.V1.0.Bak04'
- introduction of 04-onoff-analyse
- introduction of 05-onoff-flood (first steps)
- 05-onoff-flood looks good, but stack crashes
- CONFIG_MAIN_STACK_SIZE=1024 (increased from original 512 bytes)
- [###] 'analyse/05-onoff-flood stable running woth 5#/s :-) - Zstuff.V1.0.Bak05'
- zlorg.c integrated :-)))
- [###] 'tag Zstuff-V1.0 05-onoff-flood with org & crashes - Zstuff.V1.0.Bak06'
