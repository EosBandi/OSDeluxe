# OSDeluxe


OSDeluxe is a color OSD with four video input, Picture in Picture function for Mavlink flight controllers. It using a Renesas TW2837 Video Mux ASIC
and a Teensy 3.2.

# History

This project, just like many others has a long history. It started as “blue screen” eliminator, for FPV flights. Before custom made FPV monitors, it was a constant problem that normal monitors went dark or blue when video signal become noisy. A cheap video quad switch based on a TW2823 (earlier version of the chip currently used) was reprogrammed to digitize incoming video in a free running mode, and since it already had four inputs and PIP capabilities it was also added to the mix. (The idea come from a Russian webpage http://we.easyelectronics.ru/howwork/kvadrator-izobrazheniya-dlya-sistem-videonablyudeniya.html and “confidential” datasheet of TW2823 was found on Chinese page.) 
The TW2823 chip has color OSD capabilities and it seemed a good idea to make a PIP OSD for FPV use with it. After experimenting for a while it turned out that the speed of OSD functions are too slow to use  for FPV, so the project was stalled. Then a new version of the chip come out, the TW2837 with accelerated OSD functions. Using these functions with some tuned code, now it is possible to have a decent color OSD with around 10Hz refresh rate.

#TW2837
The TW2837 is a four channel video and audio controller designed for security applications by Renesas (Originally TechWell, then Intersil…). It has four PAL/NTSC video decoders (digitizers) and dual video encoders. Video encoders has scalers and video enhancer built in. Decoders and encoders are connected to a MUX engine which provides two display paths and an accelerated graphic overlay (OSD) for each paths. Unlike older TW28xx chips, datasheet for TW2837 is freely available.  (unfortunately OSD sections of the datasheet are quite rudimentary, so it needed a lot’s of experimenting to make it work.)
Communication with the chip can be done via I2C interface (up to 10Mhz!! clock speed) or via a parallel interface. 

