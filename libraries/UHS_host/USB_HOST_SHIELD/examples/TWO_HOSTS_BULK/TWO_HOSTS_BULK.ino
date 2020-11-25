#define LOAD_USB_HOST_SYSTEM
#define LOAD_USB_HOST_SHIELD
#define LOAD_UHS_HUB
#define LOAD_UHS_BULK_STORAGE

#include <Arduino.h>
#ifdef true
#undef true
#endif
#ifdef false
#undef false
#endif

#include <UHS_host.h>

MAX3421E_HOST UHS_Usb_A;
UHS_USBHub hub_A(&UHS_Usb_A);
UHS_Bulk_Storage Storage_A(&UHS_Usb_A);

uint8_t laststate_A;
boolean tested_A;
boolean notified_A;
boolean lastEnable_A = false;

MAX3421E_HOST UHS_Usb_B(4, 2); // SS on pin 4, IRQ on pin 2
UHS_USBHub hub_B(&UHS_Usb_B);
UHS_Bulk_Storage Storage_B(&UHS_Usb_B);

uint8_t laststate_B;
boolean tested_B;
boolean notified_B;
boolean lastEnable_B = false;

uint8_t usbstate;

uint8_t buf[512]; // WARNING! Assumes a sector is 512bytes!

void test_bulk( UHS_Bulk_Storage* interface, uint8_t lun, boolean *v) {
        uint8_t rcode = 0;
        uint16_t loops = 0;
        *v = true;
        E_Notify(PSTR("\r\nTesting LUN "), 0);
        USB_HOST_SERIAL.print(lun);
        uint32_t xnow = millis();
        while(!rcode && loops < 2048) {
                loops++;
                rcode = interface->Read(lun, loops, 512, 1, buf);
        }
        uint32_t finish = millis();
        if(!rcode) {
                E_Notify(PSTR("\r\nRead passed, Read 2048 sectors (1024K) in "), 0);
                USB_HOST_SERIAL.print(finish - xnow);
                E_Notify(PSTR("ms "), 0);
        } else {
                E_Notify(PSTR("\r\nERROR: Read Failed"), 0);

        }
}

void setup() {
        while(!Serial);
        USB_HOST_SERIAL.begin(115200);

        while(UHS_Usb_A.Init(1000) !=0);
        while(UHS_Usb_B.Init(1000) !=0);
        E_Notify(PSTR("\r\n\r\ngo!\r\n"), 0);
        laststate_A = 0xff;
        tested_A = false;
        notified_A = false;
        E_Notify(PSTR("\r\nPlug in storage devices/hubs now..."), 0);
}

void loop() {
        usbstate = UHS_Usb_A.getUsbTaskState();
        if(usbstate != laststate_A) {
                USB_HOST_SERIAL.print("\r\nHOST_A FSM state: 0x");
                if(usbstate < 0x10) USB_HOST_SERIAL.print("0");
                USB_HOST_SERIAL.print(usbstate,HEX);
                laststate_A = usbstate;
                switch(usbstate) {
                        case UHS_USB_HOST_STATE_IDLE:
                                // E_Notify(PSTR("\r\nPlug in a storage device now..."), 0);
                                break;
                        case UHS_USB_HOST_STATE_ERROR:
                                E_Notify(PSTR("\r\nHOST_A USB state machine reached error state 0x"),0);
                                USB_HOST_SERIAL.print(UHS_Usb_A.usb_error, HEX);
                        break;
                        case UHS_USB_HOST_STATE_RUNNING:
                                if(hub_A.bPollEnable) {
                                        E_Notify(PSTR("\r\nHOST_A Hub Connected..."),0);
                                        E_Notify(PSTR("\r\nHOST_A Plug in a storage device now..."), 0);
                                }
                                break;
                        default:
                                //E_Notify(PSTR("."),0);
                                break;

                }
        }
        boolean pbe = Storage_A.bPollEnable;
        boolean en = (pbe == lastEnable_A);
        if(!en) {
                lastEnable_A = pbe;
                if(pbe) {
                        E_Notify(PSTR("\r\nHOST_A Storage is polling..."),0);
                } else {
                        E_Notify(PSTR("\r\nHOST_A Storage is not polling..."),0);
                }
        }
        if((usbstate == UHS_USB_HOST_STATE_RUNNING) && Storage_A.bPollEnable && !tested_A) {
                if(!notified_A) {
                        E_Notify(PSTR("\r\nHOST_A Waiting for device to become ready..."),0);
                        notified_A = true;
                }
                for(uint8_t i = 0; i < MASS_MAX_SUPPORTED_LUN; i++) {
                        if(Storage_A.LUNIsGood(i)) {
                                test_bulk(&Storage_A, i, &tested_A);
                        }
                }
        }

        if(!Storage_A.bPollEnable && tested_A) {
                           tested_A = false;
                           notified_A = false;
                           E_Notify(PSTR("\r\nHOST_A Plug in a storage device now..."), 0);
        }



        usbstate = UHS_Usb_B.getUsbTaskState();
        if(usbstate != laststate_B) {
                USB_HOST_SERIAL.print("\r\nHOST_B FSM state: 0x");
                if(usbstate < 0x10) USB_HOST_SERIAL.print("0");
                USB_HOST_SERIAL.print(usbstate,HEX);
                laststate_B = usbstate;
                switch(usbstate) {
                        case UHS_USB_HOST_STATE_IDLE:
                                // E_Notify(PSTR("\r\nPlug in a storage device now..."), 0);
                                break;
                        case UHS_USB_HOST_STATE_ERROR:
                                E_Notify(PSTR("\r\nHOST_B USB state machine reached error state 0x"),0);
                                USB_HOST_SERIAL.print(UHS_Usb_B.usb_error, HEX);
                        break;
                        case UHS_USB_HOST_STATE_RUNNING:
                                if(hub_B.bPollEnable) {
                                        E_Notify(PSTR("\r\nHOST_B Hub Connected..."),0);
                                        E_Notify(PSTR("\r\nHOST_B Plug in a storage device now..."), 0);
                                }
                                break;
                        default:
                                //E_Notify(PSTR("."),0);
                                break;

                }
        }

        pbe = Storage_B.bPollEnable;
        en = (pbe == lastEnable_B);
        if(!en) {
                lastEnable_B = pbe;
                if(pbe) {
                        E_Notify(PSTR("\r\nHOST_B Storage is polling..."),0);
                } else {
                        E_Notify(PSTR("\r\nHOST_B Storage is not polling..."),0);
                }
        }
        if((usbstate == UHS_USB_HOST_STATE_RUNNING) && Storage_B.bPollEnable && !tested_B) {
                if(!notified_B) {
                        E_Notify(PSTR("\r\nHOST_B Waiting for device to become ready..."),0);
                        notified_B = true;
                }
                for(uint8_t i = 0; i < MASS_MAX_SUPPORTED_LUN; i++) {
                        if(Storage_B.LUNIsGood(i)) {
                                test_bulk(&Storage_B, i, &tested_B);
                        }
                }
        }

        if(!Storage_B.bPollEnable && tested_B) {
                           tested_B = false;
                           notified_B = false;
                           E_Notify(PSTR("\r\nHOST_B Plug in a storage device now..."), 0);
        }
}

