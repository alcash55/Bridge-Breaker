#include <SPI.h>
#include <SD.h>

#define sd_chip_select 4
#define sd_detect 5
#define SD_ABSENT 0x00
#define SD_NOPART 0x0F
#define SD_INSERTED 0xF0
#define SD_CARDERR 0xDD
#define SD_READY 0xFF

char fileName[12] = {"BB_0000.txt"};
char fileSegment[5] = {"0000"};
File bridge_breaker_output;
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
        if (SD.begin(sd_chip_select))
        {
          sd_state = SD_READY;
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
  uint16_t result = 0;
  if (sd_state == SD_READY)
  {
    for (int i = 1; i < 9999; i++)
    {
      sprintf(fileSegment, "%04d", i);
      memcpy(fileName + 3, fileSegment, 4);
      if (!SD.exists(fileName))return i;
    }
  }
  return result; //It should never reach this
}

void file_open(uint16_t usable_number)
{
  if (sd_state == SD_READY) {
    sprintf(fileSegment, "%04d", usable_number);
    memcpy(fileName + 3, fileSegment, 4);
    bridge_breaker_output = SD.open(fileName, FILE_WRITE);
    bridge_breaker_output.println("Bridge Breaker Results");
    bridge_breaker_output.println("Time\tForce\tDisplacement");
    bridge_breaker_output.close();
  }
}

void file_print(uint16_t usable_number, double seconds, double force, double displacement)
{
  if (sd_state == SD_READY) {
    bridge_breaker_output = SD.open(fileName, FILE_WRITE);
    bridge_breaker_output.print(seconds, 1);
    bridge_breaker_output.print("\t");
    bridge_breaker_output.print(force, 4);
    bridge_breaker_output.print("\t");
    bridge_breaker_output.println(displacement, 4);
    bridge_breaker_output.close();
  }
}
