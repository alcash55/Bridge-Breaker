#include <SPI.h>
#include <SD.h>

#define sd_chip_select 4
#define sd_detect 5
#define SD_ABSENT 0x00
#define SD_NOPART 0x0F
#define SD_INSERTED 0xF0
#define SD_CARDERR 0xDD
#define SD_READY 0xFF

char fileName[17] {'B','r','d','g','B','r','k','r','_','0','0','0','0','.','t','x','t'}

Sd2Card card;
SdVolume volume;
SdFile root;
byte sd_state = 0x00;
byte sd_last_state = sd_state;

void file_start()
{
  pinMode(sd_detect, INPUT_PULLUP);
}

void file_task()
{
  if (digitalRead(sd_detect)) {
    sd_state = SD_ABSENT;
  }
  else
  {
    switch (sd_state)
    {
      case SD_ABSENT:
        sd_state = SD_INSERTED;
        break;
      case SD_INSERTED:
        if (card.init(SPI_HALF_SPEED, sd_chip_select))
        {
          if (volume.init(card))
          {
            sd_state = SD_READY;
          }
          else
          {
            sd_state = SD_NOPART;
          }
        }
        else
        {
          sd_state = SD_CARDERR;
        }
      default:;
    }
  }
  if (sd_state != sd_last_state)
  {
    if (sd_state == 0xFF) {
      graphics_set_sd_state(true);
    }
    else {
      graphics_set_sd_state(false);
    }
  }
  sd_last_state = sd_state;
}

uint16_t file_find_usable_number()
{
uint16_t = 1;
if(sd_state==SD_READY)
{

}
}
