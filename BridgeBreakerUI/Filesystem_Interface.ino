///////////////////////// SD Card /////////////////////////
#include <SPI.h>
byte sdState = 0x03;
byte sdLastState = 255;
boolean sdSafe = true;
#define SD_CARD_PRESENT 0x00
#define SD_CARD_INSERTED 0x01
#define SD_CARD_REMOVED 0x02
#define SD_CARD_MISSING 0x03
#define SD_CARD_ERROR 0xFF

//#include <SD.h>
//File sdFile;


///////////////////////// USB Host /////////////////////////
byte usbState = 0x03;
byte usbLastState = 255;
uint8_t current_state = 128;
uint8_t last_state = 255;
                    boolean usbSafe = true;
#define USB_PRESENT 0x00
#define USB_INSERTED 0x01
#define USB_REMOVED 0x02
#define USB_MISSING 0x03
#define USB_ERROR 0xFF

#define USB_LABEL "/"
#define USB_PATH "/"

#define LOAD_UHS_PRINTF_HELPER
// Load the USB Host System core
#define LOAD_USB_HOST_SYSTEM
// Load USB Host Shield
#define LOAD_USB_HOST_SHIELD
// Bulk Storage
#define LOAD_UHS_BULK_STORAGE
// RTC/clock
#define LOAD_RTCLIB
// Filesystem
#define LOAD_GENERIC_STORAGE

#include <RTClib.h>
#include <UHS_host.h>

MAX3421E_HOST UHS_Usb;

///////////////////////// FS Setup /////////////////////////

void filesystemStart()
{
  Init_Generic_Storage(&UHS_Usb);
  while (UHS_Usb.Init(1000) != 0);
  usbState = USB_REMOVED;
  sdState = SD_CARD_REMOVED;
}

///////////////////////// FS Usage /////////////////////////



///////////////////////// FS Utilities /////////////////////////

void fsChkStorage()
{
current_state = UHS_Usb.getUsbTaskState();
        if(current_state != last_state) {
                last_state = current_state;
        switch(current_state)
        {
        case 0x60:
        if(usbState!=USB_PRESENT||usbState!=USB_INSERTED)
        usbState=USB_PRESENT;
        break;
        case 0x1d:
        if(usbState!=USB_MISSING||usbState!=USB_REMOVED)
        usbState=USB_MISSING;
        break;  
        default:;
        }
        }
}

boolean sdPresent()
{
  if (sdState == SD_CARD_PRESENT)return true;
  return false;
}

boolean usbPresent()
{
  if (usbState == USB_PRESENT)return true;
  return false;
}

boolean fsDeviceChange()
{
  if (sdState != sdLastState)
  {
    sdLastState = sdState;
    return true;
  }
  if (usbState != usbLastState)
  {
    usbLastState = usbState;
    return true;
  }
  return false;
}
