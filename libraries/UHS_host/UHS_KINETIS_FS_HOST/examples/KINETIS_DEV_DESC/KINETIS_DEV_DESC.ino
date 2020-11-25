#include <Arduino.h>
#ifdef true
#undef true
#endif
#ifdef false
#undef false
#endif

#define LOAD_USB_HOST_SYSTEM
#define LOAD_UHS_KINETIS_FS_HOST

#define USB_HOST_SERIAL SERIAL_PORT_HARDWARE
#define LOAD_UHS_PRINTF_HELPER

#define ENABLE_UHS_DEBUGGING 1
#define DEBUG_PRINTF_EXTRA_HUGE 1
#define DEBUG_PRINTF_EXTRA_HUGE_UHS_HOST 1
#define DEBUG_PRINTF_EXTRA_HUGE_USB_HOST_KINETIS 1
//#define UHS_DEVICE_WINDOWS_USB_SPEC_VIOLATION_DESCRIPTOR_DEVICE 1


#include <UHS_host.h>

uint8_t rcode;
uint8_t usbstate;
uint8_t laststate;
USB_DEVICE_DESCRIPTOR buf;

UHS_KINETIS_FS_HOST UHS_Usb;

uint8_t retries;
UHS_Device *p;
uint8_t dtp;

void setup() {
        // USB data switcher, PC -> device.
        pinMode(5,OUTPUT),
        digitalWriteFast(5, HIGH);

        laststate = 0;
        USB_HOST_SERIAL.begin(115200);

        Init_dyn_SWI();
        while(UHS_Usb.Init(1000) == -1);
        printf_P(PSTR("\r\n\r\n\r\n\r\nUSB HOST READY.\r\n"));

}

void loop() {
        usbstate = UHS_Usb.getUsbTaskState();
        if(usbstate != laststate) {
                p = NULL;
                laststate=usbstate;
                switch( usbstate ) {
                        case( UHS_USB_HOST_STATE_ERROR ):
                                UHS_Usb.doSoftReset(0, 0, 0);
                                p = UHS_Usb.addrPool.GetUsbDevicePtr(0);
                                if(p) {
                                        dtp = 0;
                                }
                        // fall thru
                        case( UHS_USB_HOST_STATE_RUNNING ):
                                if(!p) {
                                        UHS_Usb.doSoftReset(0, 0, 1);
                                        p = UHS_Usb.addrPool.GetUsbDevicePtr(1);
                                        if(!p) {
                                                printf_P(PSTR("\r\nError Getting device pointer"));
                                        } else {
                                                printf_P(PSTR("\r\nGot device pointer at 1"));
                                                dtp = 1;
                                        }
                                }

                                retries = 0;
                                again:
                                printf_P(PSTR("\r\nGetting device descriptor"));
                                rcode = UHS_Usb.getDevDescr( dtp, sizeof(USB_DEVICE_DESCRIPTOR), (uint8_t*)&buf );

                                if( rcode ) {
                                        printf_P(PSTR("\r\nError reading device descriptor. Error code 0x%2.2x\r\n"),rcode);
                                        if(rcode == UHS_HOST_ERROR_JERR && retries < 3) {
                                                delay(100);
                                                retries++;
                                                goto again;
                                        }
                                } else {
                                        /**/
                                        printf_P(PSTR("\r\nDescriptor Length:\t0x%2.2x"),buf.bLength);
                                        printf_P(PSTR("\r\nDescriptor type:\t0x%2.2x"),buf.bDescriptorType);
                                        printf_P(PSTR("\r\nUSB version:\t\t0x%4.4x"),buf.bcdUSB);
                                        printf_P(PSTR("\r\nDevice class:\t\t0x%2.2x"),buf.bDeviceClass);
                                        printf_P(PSTR("\r\nDevice Subclass:\t0x%2.2x"),buf.bDeviceSubClass);
                                        printf_P(PSTR("\r\nDevice Protocol:\t0x%2.2x"),buf.bDeviceProtocol);
                                        printf_P(PSTR("\r\nMax.packet size:\t0x%2.2x"),buf.bMaxPacketSize0);
                                        printf_P(PSTR("\r\nVendor  ID:\t\t0x%4.4x"),buf.idVendor);
                                        printf_P(PSTR("\r\nProduct ID:\t\t0x%4.4x"),buf.idProduct);
                                        printf_P(PSTR("\r\nRevision ID:\t\t0x%4.4x"),buf.bcdDevice);
                                        printf_P(PSTR("\r\nMfg.string index:\t0x%2.2x"),buf.iManufacturer);
                                        printf_P(PSTR("\r\nProd.string index:\t0x%2.2x"),buf.iProduct);
                                        printf_P(PSTR("\r\nSerial number index:\t0x%2.2x"),buf.iSerialNumber);
                                        printf_P(PSTR("\r\nNumber of conf.:\t0x%2.2x"),buf.bNumConfigurations);
                                        /**/
                                        printf_P(PSTR("\r\n\nInsert Another device."));
                                }
                                break;

                        default:
                                break;
                }
                fflush(stdout);
        }
}
