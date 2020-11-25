/*
 *******************************************************************************
 * USB-MIDI to Legacy Serial MIDI converter with SysEx support
 * Copyright 2017 Yuuichi Akagawa
 *
 * based on USBH_MIDI for USB Host Shield 2.0.
 * https://github.com/YuuichiAkagawa/USBH_MIDI
 *
 * This is sample program. Do not expect perfect behavior.
 *
 * Note: This driver supports for MIDI Streaming class only.
 *       If your MIDI Controller does not work, probably you needs its vendor specific driver. *******************************************************************************
 */

// Patch printf so we can use it.
#define LOAD_UHS_PRINTF_HELPER
// Load the USB Host System core
#define LOAD_USB_HOST_SYSTEM
// Load USB Host Shield
#define LOAD_USB_HOST_SHIELD
// Load MIDI class driver
#define LOAD_UHS_MIDI

//#define ENABLE_UHS_DEBUGGING 1
//#define DEBUG_PRINTF_EXTRA_HUGE 1
//#define DEBUG_PRINTF_EXTRA_HUGE_UHS_HOST 1
//#define DEBUG_PRINTF_EXTRA_HUGE_USB_HOST_SHIELD 1
//#define DEBUG_PRINTF_EXTRA_HUGE_MIDI_HOST 1
//#define UHS_DEVICE_WINDOWS_USB_SPEC_VIOLATION_DESCRIPTOR_DEVICE 1


// Note: useless check!
// Should really be checking:
// SERIAL_PORT_MONITOR
// SERIAL_PORT_USBVIRTUAL
// SERIAL_PORT_HARDWARE_OPEN
// not USBCON (OR USBCON ONLY) PLEASE FIX THIS!

#ifdef USBCON
#define _MIDI_SERIAL_PORT Serial1
#define USB_HOST_SERIAL Serial
#else
#define _MIDI_SERIAL_PORT Serial
#define USB_HOST_SERIAL Serial1
#endif

#include <Arduino.h>
#ifdef true
#undef true
#endif
#ifdef false
#undef false
#endif

#include <UHS_host.h>

MAX3421E_HOST *UHS_Usb;
UHS_MIDI *Midi;
bool connected;

// Poll USB MIDI Controller and send to serial MIDI

void MIDI_poll() {
        uint8_t size;
        uint8_t outBuf[4];
        uint8_t sysexBuf[3];

        do {
                if((size = Midi->RecvRawData(outBuf)) > 0) {
                        //MIDI Output
                        uint8_t rc = Midi->extractSysExData(outBuf, sysexBuf);
                        if(rc != 0) { //SysEx
                                _MIDI_SERIAL_PORT.write(sysexBuf, rc);
                        } else { //SysEx
                                _MIDI_SERIAL_PORT.write(outBuf + 1, size);
                        }
                }
        } while(size > 0);
}


void setup() {
        connected = false;
        while(!USB_HOST_SERIAL) {
                yield();
        }
        _MIDI_SERIAL_PORT.begin(31250);
        USB_HOST_SERIAL.begin(115200);
        delay(100);
        printf_P(PSTR("\r\n\r\n\r\n\r\n\r\n\r\nUSB MIDI Converter example.\r\n\r\n"));
        UHS_Usb = new MAX3421E_HOST();
        Midi = new UHS_MIDI(UHS_Usb);
        while(UHS_Usb->Init(1000) != 0);
}

void loop() {
        if(Midi->isReady()) {
                if(!connected) {
                        connected = true;
                        printf_P(PSTR("Connected to MIDI\r\n"));
                }
                MIDI_poll();
        } else {
                if(connected) {
                        connected = false;
                        printf_P(PSTR("\r\nDisconnected from MIDI\r\n"));
                }
        }
}
