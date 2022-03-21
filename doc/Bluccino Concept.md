# Bluccino

### Session 1 - What the Hell is Bluccino?
* What is Bluccino?
* Why Bluccino ?
* A sample - 15 Lines of C-code
* Sweet Spots
* Ready to go Libraries for BT-mesh apps
* The work bench model
* SW Schematics / Signal Flow Docu
* Rapid Prototyping Toolset
- init/tick/tock framework
* Event sniffing

### Session 2 - A Simple Bluetooth Mesh Example
* App description
* The Client Application Code
* Running
* Provisioning
* Configuration
* Test

#### App Description

Let us study a simple Bluetooth mesh application with two Bluetooth mesh nodes being involved. The first node is equipped with n buttons (labeled @1,@2,...,@n), while the second node supports n LEDs (labeled @1,@2,...,@n), and our task is to write an app code that toggles LED @id on node 2 whenever button @id is pressed on node 1.

The first important thing to understand is that any Bluetooth mesh communication happens between mesh models which can be treated as Bluetooth mesh interfaces. There are server models which control a state, and there are state-less client models. There is a standard way defined for wireless on/off control via Bluetooth mesh which utilizes a 'generic on/off client model' (GOOCLI) for sending an on/off command, as well as a 'generic on/off server model' (GOOSRV) to receive the wireless on/off command emitted by the client.

Let us assume that each of our buttons @id toggles a switch state @id, where all this toggle control happens inside of a black box called SWITCH @id. Let us further assume that all of our LEDS are controlled by LED @id control blocks which physically turn an LED on or off according to the LED @id block input signal.

In total we have to deal with four types of control blocks (SWITCH, LED, GOOCLI, GOOSRV) where node 1 has to run a client application listening to status change events of SWITCH @id blocks, which have to be forwarded as on/off commands via the Bluetooth mesh network utilizing GOOCLI @id blocks.

Conversely node 2 needs to run a server application listening on GOOSRV @id blocks which receive Bluetooth mesh on/off commands from GOOCLI @id blocks of node 1, and being forwarded to LED @id control blocks, which finally turns an LED @id on or off. The whole scenario is shown in figure 1, where the left part describes the client application part, and the right part the server application part.

#>>>>>>>>>>>>>>>
Figure 1: LED on/off control via Bluetooth mesh
#>>>>>>>>>>>>>>>

#### The Client Application Code

Let us look at C-code utilizing the Bluccino framework implementing the full app functionality described in previous section. Here is the client application code:

  #include "bluccino.h"                     // 02-swicli

  int app(BL_ob *o, int val)                // public APP module interface
  {
    BL_ob cli = {_GOOCLI,SET_,o->id,NULL};  // generic on/off client object

    if (bl_is(o,_SWITCH,STS_))              // switch status update
      bl_down(&cli,val);                    // send generic on/off SET message
    return 0;                               // OK
  }

  void main(void)
  {
    bl_hello(4,"02-clisrv");                // set verbose level, print hello message
    bl_init(bluccino,app);                  // init bluccino, output to app()
  }

The whole application s quite compact, there are only 13 lines of source code (empty lines not counted) and comprises two functions - main() and app(). Even without detailed understanding of the Bluccino framework we get an intuitive idea of what is happening:

* the code begins with an #include "bluccino.h" statement, which includes the most important definitions like BL_ob, _GOOCLI, _SWITCH, SET_, STS_, and function prototypes like bl_is(), bl_down(), bl_hello(), bl_init() for working with Bluccino framework.
* the first line in the main function sets the verbose level for the debug logging and prints an initial (hello) message, while the following statement initializes the Bluccino framework, telling that any events being issued by the framework have to be handled by the app() function.
* the app() function, implementing the actual application, acts as a kind of universal callback.
* The first statement of app() declares a messaging object cli of type BL_ob which is prepared for sending [GOOCLI:SET] messages.
* The second statement dispatches the incoming object information accessed by *o.
* in case  a [SWITCH:STS] message is detected a [GOOCLI:SET] message is sent to the GOOCLI @id mesh model of the mesh driver layer, from where a generic on/off message is posted into the Bluetooth mesh network.  

Surely this short description skipped some details which will be covered in detail somewhere later.

#### The Server Application Code

Before we dive into the details let's investigate the server application.

  #include "bluccino.h"                     // 03-ledsrv app

  int app(BL_ob *o, int val)                // public APP module interface
  {
    BL_ob led = {_LED,SET_,o->id,NULL};     // LED object

    if (bl_is(o,_GOOSRV,STS_))              // generic on/off srv status update
      bl_down(&led,val);                    // turn LED @id on/off
    return 0;                               // OK
  }

  void main(void)
  {
    bl_hello(4,"03-ledsrv");                // set verbose level, print hello message
    bl_init(bluccino,app);                  // init bluccino, output to app()
  }

The structure of the server application is quite similar.
* again the code begins with an #include "bluccino.h" statement, which includes important definitions like BL_ob, _GOOSRV, _LED, SET_, STS_, and function prototypes like bl_is(), bl_down(), bl_hello(), bl_init() for working with Bluccino framework.
* the first line in the main function sets the verbose level for the debug logging and prints an initial (hello) message, while the following statement initializes the Bluccino framework, telling that any events being issued by the framework have to be handled by the app() function.
* the app() function, implementing the actual application, acts again as a kind of universal callback.
* the first statement of app() declares a messaging object led of type BL_ob which is prepared for sending [LED:SET] messages.
* The second statement dispatches the incoming object information accessed by *o.
* in case a [GOOSRV:STS] (generic on/off server status) message is detected a [LED:SET] message is sent down to the LED @id block of the hardware driver layer to on/off control the LED @id.


#### A Client/Server Application Code

Let us conclude that the code for the client and server application can be combined
to a single client/server application, and we can use the Bluetooth mesh configuration features to run this combined application either as a client, or a server, or a combined client/server.

  #include "bluccino.h"                     // 04-clisrv (client/server app)

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
    bl_hello(4,"04-clisrv");                // set verbose level, print hello message
    bl_init(bluccino,app);                  // init bluccino, output to app()
  }

Note that the combined client/server code comprises only 16 code lines (blank lines not counted).
