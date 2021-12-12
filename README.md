![Bluccino Banner](https://user-images.githubusercontent.com/39674928/145687742-748bacf8-e285-45de-ac3b-af922959222a.jpg)


## What the Hell is Bluccino?

#### Bluccino Sweet Spots

* Bluccino is a rapid prototyping platform for Bluetooth BLE and Mesh networking applications
* Bluccino is a simple and elegant language independent unit testing framework (mocking platform)
* Bluccino makes the essential flows of event driven applications transparent and blurs out all unessential details

#### But there's even more ...

* Bluccino is a programming philosophy for platform independent Bluetooth networking applications
* Bluccino is platform, hardware and language independent (C, C++, Python, ...)
* Bluccino is a universal paradigm for event driven system
* Bluccino saves efforts, cost and development time
* Bluccino saves efforts, cost and development time
* Bluccino is open source with open tutorials
* Bluccino is a slim code layer
* Bluccino is awesome and makes great fun

## Lot's of bold statements, but what exactly can I do with Bluccino?

Bluccino was developed in parallel to the firmware development of our Bluenetics connector.
This is a Bluetooth mesh application, running on one or two processors. When we learnt about Bluetooth mesh we started to study the simplest Zephyr mesh example which is called onoff_app (zephyr/samples/boards/nrf/mesh/onoff-app). The README.rst file of this sample mentions the following text

```
This is a simple application demonstrating a Bluetooth mesh multi-element node.
Each element has a mesh onoff client and server
model which controls one of the 4 sets of buttons and LEDs .
```

This sounded promising, especially the phrase 'simple'. When we examined the 'simple application' we found one source file main.c which has 647 lines. Whoppa - more than 600 lines of source code and such an application is termed simple? Let's go back to the year 1978 when Brian W. Kernighan and Dennis M. Ritchie published their book "The C Programming Language". In a very early section they present the world's most famous program:  

![image](https://user-images.githubusercontent.com/17394277/145695547-f0345886-8ad6-487f-973e-6e99c6c4ccbc.png)

This is what we would call a 'simple' program. But let's keep it as it is and call the 647-line source code of onoff_app a 'simple application', what would just mean that 'hello world' is super-super-super simple.

If we analyse the 'onoff_app' and ask what the code exactlay does when the node is provisioned (on-boarded onto the mesh network), then below diagram will tell us everything: The phrase 'for (i=1:4)' just means that we have 4 similar setups, each setup (refering to index i) comprising a button, a mesh client model, a mesh server model and an LED. The i-th button is connected with the i-th client, indicating that a button preess would send an ON message via Bluetooth mesh to the i-th server, while a double button press sends an OFF message to the i-th server, while the i-th server forwards the ON or OFF message to an LED module which will just cause the i-th LED going on and off. That's it.  

![onoff_app_essence](https://user-images.githubusercontent.com/17394277/145696057-b7fba735-ed74-4f4e-b8b9-9a1e0d1c1407.png)

Let's see how a C++ program running on a Bluccino framework would look in every detail!

![image](https://user-images.githubusercontent.com/17394277/145696286-a38c4422-be82-47da-837c-e6a5cef88d17.png)

It's no more super-super-super simple, but the reader might agree that wecan call it super-super simple. What we see is a button object of class Button, a client object of class GooCli (generic OnOff client), a server object





Blucino has been introduced to write super-super simple Bluetooth mesh programs. 

## Is Bluccino Free?

If you don't make money with Bluccino you don't have to give us money. If you are making money with Bluccino, give us a little back for the value we provide you! Does this seem fair? If you use Bluccino for commercial use you need a license for it, and you should be aware that such license is free for you in case of non-commercial use. And for commercial use the license is afforable, especially you are a start-up company.  

## Want to see Bluccino in Action?

Definitely a great idea! Let's go and see an intro!
