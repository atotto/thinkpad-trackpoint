
#include <HID.h>
#include <hidcomposite.h>
#include <usbhub.h>


#include <hidescriptorparser.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

leonardo.build.usb_product="My Awesome Controller Leonardo";

//#define SERIAL_DEBUG 1

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  // keyboard
  0x05 , 0x01 , 0x09 , 0x06 , 0xA1 , 0x01 ,
  0x85 , 0x01,                    //     REPORT_ID (1)
  0x05 , 0x07 , 0x19 , 0xE0 , 0x29 , 0xE7 , 0x15 , 0x00 , 0x25 , 0x01,
  0x95 , 0x08 , 0x75 , 0x01 , 0x81 , 0x02 , 0x95 , 0x08 , 0x75 , 0x01 , 0x81 , 0x01 , 0x05 , 0x08 , 0x19 , 0x01 ,
  0x29 , 0x03 , 0x95 , 0x03 , 0x75 , 0x01 , 0x91 , 0x02 , 0x95 , 0x01 , 0x75 , 0x05 , 0x91 , 0x01 , 0x05 , 0x07 ,
  0x19 , 0x00 , 0x2A , 0xFF , 0x00 , 0x15 , 0x00 , 0x26 , 0xFF , 0x00 , 0x95 , 0x06 , 0x75 , 0x08 , 0x81 , 0x00 ,
  0xC0 ,

  //  // trackpoint
  //  0x05 , 0x01 , 0x09 , 0x02 , 0xA1 , 0x01 ,
  //  0x85 , 0x02 ,
  //  0x09 , 0x01 , 0xA1 , 0x00 , 0x05 , 0x09 , 0x19 , 0x01 ,
  //  0x29 , 0x03 , 0x15 , 0x00 , 0x25 , 0x01 , 0x95 , 0x03 , 0x75 , 0x01 , 0x81 , 0x02 , 0x95 , 0x01 , 0x75 , 0x05 ,
  //  0x81 , 0x01 , 0x05 , 0x01 ,
  //  0x09 , 0x30 ,
  //  0x09 , 0x31 ,
  //  0x15 , 0x81 , 0x25 , 0x7F ,
  //  0x95 , 0x02 ,
  //  0x75 , 0x08 , 0x81 , 0x06 , 0xC0 , 0xC0 ,

  // trackpoint + scroll
  0x05 , 0x01 , 0x09 , 0x02 , 0xA1 , 0x01 ,
  0x85 , 0x02 ,
  0x09 , 0x01 , 0xA1 , 0x00 , 0x05 , 0x09 , 0x19 , 0x01 ,
  0x29 , 0x03 , 0x15 , 0x00 , 0x25 , 0x01 , 0x95 , 0x03 , 0x75 , 0x01 , 0x81 , 0x02 , 0x95 , 0x01 , 0x75 , 0x05 ,
  0x81 , 0x01 , 0x05 , 0x01 ,
  0x09 , 0x30 ,
  0x09 , 0x31 ,
  0x09 , 0x38 , // USAGE (Wheel)
  0x15 , 0x81 , 0x25 , 0x7F ,
  0x95 , 0x03 ,
  0x75 , 0x08 , 0x81 , 0x06 , 0xC0 , 0xC0 ,

  //  0x05 , 0x01 , 0x09 , 0x80 , 0xA1 , 0x01 ,
  //  0x85 , 0x03 ,
  //  0x05 , 0x01 , 0x15 , 0x00,
  //  0x25 , 0x01 , 0x95 , 0x08 , 0x75 , 0x01 , 0x19 , 0x81 , 0x29 , 0x88 , 0x81 , 0x02 , 0xC0 , 0x05 , 0x0C , 0x09,
  //  0x01 , 0xA1 , 0x01 ,
  //  0x85 , 0x04 ,
  //  0x95 , 0x08 , 0x75 , 0x01 , 0x15 , 0x00 , 0x25 , 0x01 , 0x09 , 0xE9 , 0x09,
  //  0xEA , 0x09 , 0xE2 , 0x09 , 0xB7 , 0x09 , 0xCD , 0x09 , 0xB5 , 0x09 , 0xB6 , 0x0A , 0x94 , 0x01 , 0x81 , 0x02,
  //  0x09 , 0x03 , 0xA1 , 0x02 , 0x05 , 0x09 , 0x19 , 0x10 , 0x29 , 0x17 , 0x81 , 0x02 , 0x05 , 0x09 , 0x19 , 0x18,
};


static const uint8_t _us2jisTable[] PROGMEM = {
  // key,shift,  key,shift
  //  0x2c,0x00,  0x2c,0x00,  // (space)
  //  0x1e,0x01,  0x1e,0x01,  // !
  0x34, 0x01,  0x1f, 0x01, // "
  //  0x20,0x01,  0x20,0x01,  // #
  //  0x21,0x01,  0x21,0x01,  // $
  //  0x22,0x01,  0x22,0x01,  // %
  0x24, 0x01,  0x23, 0x01, // &
  0x34, 0x00,  0x24, 0x01, // '
  0x26, 0x01,  0x25, 0x01, // (
  0x27, 0x01,  0x26, 0x01, // )
  0x25, 0x01,  0x34, 0x01, // *
  0x2e, 0x01,  0x33, 0x01, // +
  //  0x36,0x00,  0x36,0x00,  // ,
  //  0x2d,0x00,  0x2d,0x00,  // -
  //  0x37,0x00,  0x37,0x00,  // .
  //  0x38,0x00,  0x38,0x00,  // /
  0x33, 0x01,  0x34, 0x00, // :
  //  0x33,0x00,  0x33,0x00,  // ;
  //  0x36,0x01,  0x36,0x01,  // <
  0x2e, 0x00,  0x2d, 0x01, // =
  //  0x37,0x01,  0x37,0x01,  // >
  //  0x38,0x01,  0x38,0x01,  // ?
  0x1f, 0x01,  0x2f, 0x00, // @
  0x2f, 0x00,  0x30, 0x00, // [
  0x31, 0x00,  0x89, 0x00, // BackSlash
  0x30, 0x00,  0x32, 0x00, // ]
  0x23, 0x01,  0x2e, 0x00, // ^
  0x2d, 0x01,  0x87, 0x01, // _
  0x35, 0x00,  0x2f, 0x01, // `
  0x2f, 0x01,  0x30, 0x01, // {
  0x31, 0x01,  0x89, 0x01, // |
  0x30, 0x01,  0x32, 0x01, // }
  0x35, 0x01,  0x2e, 0x01, // ~
  0
};


#define REPORT_KEYS 6
typedef struct {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[REPORT_KEYS];
} KeyReport;

class HIDSelector : public HIDComposite
{
  private:
    bool enable_us2jis = false;
    int8_t mouse_speed = 1;

  public:
    HIDSelector(USB *p) : HIDComposite(p) {};

  protected:
    void ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf); // Called by the HIDComposite library
    bool SelectInterface(uint8_t iface, uint8_t proto);
};

// Return true for the interface we want to hook into
bool HIDSelector::SelectInterface(uint8_t iface, uint8_t proto)
{
  return true;
}

// Will be called for all HID data received from the USB interface
void HIDSelector::ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len, uint8_t *buf) {
  if (len && buf)  {

#if SERIAL_DEBUG
    Serial.print(" ep=");
    Serial.print(ep);
    Serial.print(" rpt=");
    Serial.print(is_rpt_id);
    Serial.print(" len=");
    Serial.print(len);
    Serial.print(" buf=");
    for (uint8_t i = 0; i < len; i++) {
      Serial.print(buf[i], HEX);
      Serial.print(", ");
    }
#endif

    if (ep == 1) { // keyboard
      KeyReport *keyReport = (KeyReport*)(buf);
      uint8_t mod = keyReport->modifiers;

      // swap command key and alt key
      uint8_t lalt = 0x00;
      if (mod & 0x08) {
        lalt = 0x04;
        mod &= ~0x08;
      }
      uint8_t lcmd = 0x00;
      if (mod & 0x04) {
        lcmd = 0x08;
        mod &= ~0x04;
      }
      mod |= lalt;
      mod |= lcmd;

      uint8_t shifted = 0x00;
      if (mod & 0x22) {
        shifted = 0x01;
      }

      for (int i = 0; i < 6; i++) {
        uint8_t key = keyReport->keys[i];

        // caps -> l-ctrl
        if (key == 0x39) {
          keyReport->keys[i] = 0xE0;
          continue;
        }

        if (enable_us2jis) {
          uint8_t *pUsToJis = _us2jisTable;
          while (pgm_read_byte(pUsToJis)) {
            if ((pgm_read_byte(pUsToJis) == key) && (pgm_read_byte(pUsToJis + 1) == shifted)) {
              if (pgm_read_byte(pUsToJis + 3)) {
                mod |= 0x02;
              } else {
                mod &= ~0x22;
              }
              key = pgm_read_byte(pUsToJis + 2);
              break;
            }
            pUsToJis += 4;
          }
        }

        keyReport->keys[i] = key;
      }
      keyReport->modifiers = mod;

      HID().SendReport(1, keyReport , len);
    } else if  (ep == 2) { // trackpoint and function keys
      uint8_t report_id = buf[0];
      uint8_t *data = buf + 1;
      uint8_t data_len = len - 1;

      if (report_id == 1) { // trackpoint
        if (data[0] & 0x04) { // middle click
          data[1] = 0x00;
          int8_t diff = data[2];
          if (diff > 0) {
            diff += 8;
          } else if  (diff < 0) {
            diff -= 8;
          }
          data[3] = diff / 8; // scroll
          data[2] = 0x00;
        }

        // mouse speed
        int8_t speed = this->mouse_speed;
        if (speed >= 0) {
          data[1] = ((int8_t)(data[1])) * speed;
          data[2] = ((int8_t)(data[2])) * speed;
        } else {
          speed = (-speed)+1;
          data[1] = ((int8_t)(data[1])) / speed;
          data[2] = ((int8_t)(data[2])) / speed;
        }

        HID().SendReport(2, data , 4);
      } else if (report_id == 3) { // function keys

        if (data[1] & 0x02) { // think vantage
          this->enable_us2jis = !enable_us2jis;
        }
        if (data[0] & 0x04) { // speaker off
          KeyReport keyReport;
          keyReport.modifiers = 0x00;
          keyReport.reserved = 0x00;
          keyReport.keys[0] = 0x35; // `=全半
          keyReport.keys[1] = 0x00;
          keyReport.keys[2] = 0x00;
          keyReport.keys[3] = 0x00;
          keyReport.keys[4] = 0x00;
          keyReport.keys[5] = 0x00;

          HID().SendReport(1, &keyReport , 8);

          keyReport.keys[0] = 0x00;
          HID().SendReport(1, &keyReport , 8);
        }

        if (data[0] & 0x01) { // volume +
          this->mouse_speed++;
          if (this->mouse_speed == 0) {
            this->mouse_speed++;
          }
        } else if (data[0] & 0x02) { // volume -
          this->mouse_speed--;
          if (this->mouse_speed == 0) {
            this->mouse_speed--;
          }
        }
      }

    } else {

    }

#if SERIAL_DEBUG
    Serial.println();
#endif
  }
}


USB          Usb;
HIDSelector  hidSelector(&Usb);

void error_blink(int period) {
  for (;;) {
    TXLED1; delay(period); TXLED0; delay(period);
    RXLED1; delay(period); RXLED0; delay(period);
  }
}

void setup()
{
#if SERIAL_DEBUG
  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");
#endif

  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);
  delay(200);
  if (Usb.Init() == -1) {
    error_blink(400);
  }

  delay( 200 );
}

void loop()
{
  Usb.Task();
}
