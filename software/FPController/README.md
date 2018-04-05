# Introduction

This contains the source code for the front panel controller, an ATMEGA 328 running with 3.3V at 8MHz with Arduino bootloader.

The front panel controller is configured as an SPI slave controlled over the SPI bus from the main (esp8266) processor.

The behaviour depends on the instructions sent using SPI.

# Instructions

*Will attempt to kept this current with the code, but the code is the point of truth.*

The behaviour depends on the instruction:

```
  Instruction Name          Code     Description
  ====================      ======   ================================================================
  INST_NULL                 0x00     Null instruction
  INST_GET_STATION          0x01     Delivers one byte representing up 10 stations.
  INST_GET_VOL              0x02     Delivers one byte representing the volume. This has a value from
                                     1 to 31.
  INST_STATUS_OK            0x03     Used to indicate that the status is OK. No data is received or
                                     transmitted
  INST_STATUS_ERROR         0x04     Used to indicate an error status. One byte with the error code is
                                     transmitted. Value is from 0 (= OK) to 8.
  INST_GET_CHANGES          0x05     Gets a byte from the  FPController whose bits indicated
                                     what has changed.
                                     Once read the FPController sets ALL bits back to zero.
  INST_RESET_CHANGES        0x06     Resets the change bits. This should be used after ALL changes have
                                     been processed.
```
  Change Status Bits (from `INST_GET_CHANGES`)
```
  Bit Name                 Code      Description
  ==================       =======   ===============================================================
  CHANGED_VOL              0x01      The volume has changed
  CHANGED_STATION          0x02      A new station has been selected
```
