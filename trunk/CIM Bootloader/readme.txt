 This application is meant to demonstrate a framework for an application running on the HCS908GB/GT MCU.
 It demonstrates interrupts from the timer and the ADC, uses general purpose timeouts, and transmits 
 information out the serial port.

 Application Information:

 (1) An output compare channel of timer A is used to create a time
     base for the application. Output compare intervals happen every
     10ms, and is continually set up in the timer interrupt service
     routine. The internal Bus clock is 4.0000 MHZ.
 (2) Provides three general purpose timeouts, each with a resolution of
     10ms and a maximum timeout of 2.55 Seconds (1-255). When a timeout value
     is zero, it is ignored. When a timeout value is non-zero, it is
     decremented by the timer interrupt approximately every four milli-
     seconds. When a timeout value goes to zero, the timeout event
     handler is called. The "timeout 1" value is initially set to 3,
     so the "timeout 1" event handler will be run after 30ms. The event
     handler sets up the ADC converter to do a single conversion on the
     voltage from the internal temperature sensor, which when complete, will
     cause an interrupt. After starting the conversion, the "timeout 1"
     event schedules another "timeout 1" event in 30ms (timeout=3).
 (3) When the ADC conversion is complete, an interrupt is generated.
     In the interrupt routine, the ADC input value is read and transmitted
     out the TXD line of the serial port at 9600 baud.
     Note that if you are running in "Simulation Only" mode,
     you have to use the ADDI command prior to the conversion becoming
     complete. Otherwise the ADR value will not be set and the simulator
     will give an error when the simulator tries to load the uninitialized
     value in ADR. Note: VDDAREF and VREFH must have a positive voltage,
     and VREFL must be connected to ground for this part of the demo to work
     properly.

 * This source is (C)opyright 2003, P&E Microcomputer Systems, Inc.
 * Visit us at http://www.pemicro.com
 
 Here is the sample application...
 