//==============================================================================
// main.c - Bluetooth mesh client/server app
//==============================================================================
//
//                   +--------------------+
//                   |        APP         |
//                   +--------------------+
//                   |      SWITCH:       |  SWITCH interface
//  (^)->      STS ->|      @id,val       |  receive switch @id status
//                   +--------------------+
//                   |      GOOCLI:       |  GOOCLI ifc. (generic on/off client)
//  (v)<-      SET <-|  @id,<goosrv>,val  |  publish generic on/off SET message
//                   +--------------------+
//                   |      GOOSRV:       |  GOOSRV ifc. (generic on/off server)
//  (^)->      SET ->|  @id,<goosrv>,val  |  recieve generic on/off SET message
//                   +--------------------+
//                   |        LED:        |  LED interface
//  (v)<-      SET <-|      @id,val       |  set LED @id on/off
//                   +--------------------+
//
//==============================================================================

  #include "bluccino.h"

  int bl_down(BL_ob *o, int val)            // public APP module interface
  {
    if (bl_is(o,_GOOCLI,LET_))              // switch status update
    {
      BL_ob srv = {_GOOSRV,STS_};
      bl_up(&srv,val);
    }
    return 0;                               // OK
  }

  int app(BL_ob *o, int val)                // public APP module interface
  {
    BL_ob cli = {_GOOCLI,SET_,o->id,NULL};  // generic on/off client object
    BL_ob led = {_LED,SET_,o->id,NULL};     // LED object

    if (bl_is(o,_SWITCH,STS_))              // switch status update
      bl_down(&cli,val);                    // send generic on/off SET message
    else if (bl_is(o,_GOOSRV,STS_))         // generic on/off srv status update
      bl_down(&led,val);                    // turn LED @id on/off
    return 0;                               // OK
  }

  void main(void)
  {
    bl_hello(4,"05-mockup");                // set verbose level, print hello message
    bl_init(bluccino,app);                  // init bluccino, output to app()

    for (int i=0;;i++)
    {
      BL_ob swi = {_SWITCH,STS_,1,NULL};
      bl_up(&swi,i%2);
      bl_sleep(5000);
    }
  }
