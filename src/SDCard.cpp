#include "SDCard.h"

#define TEST_FILE_SIZE 1

void testWriteFile(fs::FS &fs, const char *path, uint8_t *buf, int len)
{
  if(SDEBUG) Serial.printf("Test writing %d bytes to %s\n", len, path);
  unsigned long start_time = millis();

  File file = fs.open(path, "a");
  if (!file) {
    if(SDEBUG) Serial.println("Failed to open file for writing");
    return;
  }
  unsigned long time_used = millis() - start_time;
  if(SDEBUG) Serial.printf(" It took %lu ms to open the file\n", time_used);
  start_time = millis();
  int loop = TEST_FILE_SIZE;
  while (loop--)
  {
    if (!file.write(buf, len)) {
      if(SDEBUG) Serial.println("Write failed");
      return;
    }
  }
  file.flush();
  time_used = millis() - start_time;
  if(SDEBUG) Serial.printf(" Writing %dB, %d time(s) took %lums, each write averaged %fms @ %fKB/s\n", 
                    len, TEST_FILE_SIZE , time_used, (float)time_used/(float)TEST_FILE_SIZE, (float)TEST_FILE_SIZE * len/(float)time_used);
  file.close();
}

void testReadFile(fs::FS &fs, const char *path, uint8_t *buf, int len)
{
  Serial.printf("Test reading %d bytes from %s\n", len, path);
  unsigned long start_time = millis();

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }
  unsigned long time_used = millis() - start_time;
  Serial.printf(" It took %lu ms to open the file\n", time_used);
  start_time = millis();
  int loop = TEST_FILE_SIZE;
  while (loop--)  {
    if (!file.read(buf, len)) {
      Serial.println("Read failed");
      return;
    }
  }
  time_used = millis() - start_time;
  Serial.printf(" Reading %dB, %d time(s) took %lums, each read averaged %fms @ %fKB/s\n", 
                    len, TEST_FILE_SIZE , time_used, (float)time_used/(float)TEST_FILE_SIZE, (float)TEST_FILE_SIZE * len/(float)time_used);
  file.close();
}

void testIO(fs::FS &fs)
{
  uint8_t *buf = (uint8_t*)malloc(64 * 1024);
  testWriteFile(fs, "/test_128.bin", buf, 128);
  testWriteFile(fs, "/test_1k.bin", buf, 1024);
  testWriteFile(fs, "/test_2k.bin", buf, 2 * 1024);
  testWriteFile(fs, "/test_4k.bin", buf, 4 * 1024);
  testWriteFile(fs, "/test_8k.bin", buf, 8 * 1024);
  testWriteFile(fs, "/test_16k.bin", buf, 16 * 1024);
  testWriteFile(fs, "/test_32k.bin", buf, 32 * 1024);
  testWriteFile(fs, "/test_64k.bin", buf, 64 * 1024);

  testReadFile(fs, "/test_1k.bin", buf, 1024);
  testReadFile(fs, "/test_2k.bin", buf, 2 * 1024);
  testReadFile(fs, "/test_4k.bin", buf, 4 * 1024);
  testReadFile(fs, "/test_8k.bin", buf, 8 * 1024);
  testReadFile(fs, "/test_16k.bin", buf, 16 * 1024);
  testReadFile(fs, "/test_32k.bin", buf, 32 * 1024);
  testReadFile(fs, "/test_64k.bin", buf, 64 * 1024);
  Serial.println("\nTest Completed!");
  free(buf);
}
void initSDCard(){
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();
    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }
    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
}
