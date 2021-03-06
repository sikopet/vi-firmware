#include "interface/usb.h"
#include "util/bytebuffer.h"
#include "util/log.h"

bool USB_PROCESSED = false;

void openxc::interface::usb::processSendQueue(UsbDevice* usbDevice) {
    USB_PROCESSED = true;
}

void openxc::interface::usb::initialize(UsbDevice* usbDevice) {
    usb::initializeCommon(usbDevice);
}

void openxc::interface::usb::read(UsbDevice* usbDevice, bool (*callback)(uint8_t*)) { }
