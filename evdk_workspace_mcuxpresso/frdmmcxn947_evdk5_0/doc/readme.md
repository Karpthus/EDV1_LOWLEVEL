Overview
========
MCUXpresso-IDE source code repository for the EVDK5.

Getting started
===============
Hardware changes:
- Make sure the jumpers SJ16, SJ26 and SJ27 are re-soldered and connected to the
  camera data lines.
- Connect the camera module to J9.

MCUXpresso-IDE:
- Download and install the SDK for the FRDM-MCXN947 development board.
- Use the option *Open project from File System...* to open the project
  *frdmmcxn947_evdk5_0*.
- Build the project.
- Connect the FRDM-MCXN947 development board to your computer using J17 (MCU-Link).
- Upload the program (start the debugger or use the program flash action).
- Connect the FRDM-MCXN947 development board to your laptop using connector J11 
  MCX HS USB
- Open the camera app on your computer. On Windows: Start > Camera. Make sure that
  the external camera is selected. The app shows images from the FRDM-MCXN947 
  development board.

For more details, refer to the EVD1 Study guide located in this repository.
