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

//No parameters
//no return
void file_start() // Configure pinmode for SD card detection
{
  pinMode(sd_detect, INPUT_PULLUP);
}

//No parameters
//no return
void file_task() // Monitors storage devices
{
  if (digitalRead(sd_detect)) { // If no SD card, then SD is absent
    sd_state = SD_ABSENT;
  }
  else
  {
    switch (sd_state)
    {
      case SD_ABSENT: // If card is detected, then card is inserted
        sd_state = SD_INSERTED;
        break;
      case SD_INSERTED:
        if (SD.begin(sd_chip_select)) // If card can be initialized, it is ready to use and can be accessed by the MCU
        {
          sd_state = SD_READY;
        }
        else
        {
          sd_state = SD_CARDERR; // If card cannot be initialized, it is not ready and is in a state of error
        }
      default:;
    }
  }
}

//No parameters
//returns usable file number
uint16_t file_find_usable_number() //Return usable file number. Ex. If files exist from BB_0001 to BB_0012, then it will return 13 as BB_0013 is the next usable number.
{
  uint16_t result = 0;
  if (sd_state == SD_READY)
  {
    for (int i = 1; i < 9999; i++)
    {
      sprintf(fileSegment, "%04d", i); //Convert integer to filename
      memcpy(fileName + 3, fileSegment, 4);
      if (!SD.exists(fileName))return i; //Check if filename exists
    }
  }
  return result; //It should never reach this
}

// 1 Parameter - usable file number 
// no return
void file_open(uint16_t usable_number) //Opens file with usable number input, initializes file by printing the header information
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

// 4 Parameters - file number, seconds, force, displacement
// No return

void file_print(uint16_t usable_number, float seconds, float force, float displacement) //Prints test data to file, seconds, force, and displacement
{
  if (sd_state == SD_READY) {
    bridge_breaker_output = SD.open(fileName, FILE_WRITE);
    bridge_breaker_output.print(seconds, 2);
    bridge_breaker_output.print("\t");
    bridge_breaker_output.print(force, 4);
    bridge_breaker_output.print("\t");
    bridge_breaker_output.println(displacement, 4);
    bridge_breaker_output.close();
  }
}

//No parameters
//Returns status byte
byte file_get_sd_state() //Returns SD status byte
{
return sd_state;
}

//No parameters
//Returns bool
bool file_sd_present() //Returns true if SD is ready
{
if(sd_state==0xFF)return true;
return false;  
}
