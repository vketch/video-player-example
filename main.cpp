/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "stdio.h"
#include "string.h"
#include <string>
#include <vector>

#include "mbed.h"
#include "Arial12x12.h"
#include "Arial24x23.h"

#include "ILI9341V.h"
#include "VideoPlayer.h"

#include "FATFileSystem.h"
#include "SDIOBlockDevice.h"


using namespace std::chrono;

// SDBlockDevice   blockDevice(PA_7, PA_6, PA_5, PD_14);  // mosi, miso, sck, cs
static SDIOBlockDevice bd;
BlockDevice *BlockDevice::get_default_instance() { return &bd; }
BlockDevice *get_other_blockdevice() { return &bd; }
BlockDevice *blockDevice = BlockDevice::get_default_instance();

FATFileSystem fileSystem("fs");

DigitalOut selectSPI(D4, true); // Select SPI iterface on LCD
ILI9341V myLCD(SPI_DMA_, 10000000, D11, D12, D13, D10, D8, D7, "myLCD");
Timer t;


unsigned short backgroundcolor = Green;
unsigned short foregroundcolor = Red;

void mount_file_system() {
  // Try to mount the filesystem
  printf("Mounting the filesystem... ");
  fflush(stdout);

  int err = fileSystem.mount(blockDevice);
  printf("%s\n", (err ? "Fail :(" : "OK"));
  if (err) {
    // Reformat if we can't mount the filesystem
    // this should only happen on the first boot
    printf("No filesystem found %s, (%d), formatting... ", strerror(-err), err);
    fflush(stdout);
    err = fileSystem.reformat(blockDevice);
    printf("%s\n", (err ? "Fail :(" : "OK"));
    if (err) {
      error("error: %s (%d)\n", strerror(-err), err);
    }
  }

  // Display the root directory
  printf("Opening the root directory... ");
  fflush(stdout);

  DIR *d = opendir("/fs/");
  printf("%s\n", (!d ? "Fail :(" : "OK"));
  if (!d) {
    error("error: %s (%d)\n", strerror(errno), -errno);
  }
}

void unmount_fs() {
  printf("Unmounting... ");
  fflush(stdout);
  int err = fileSystem.unmount();
  printf("%s\n", (err < 0 ? "Fail :(" : "OK"));
  if (err < 0) {
    error("error: %s (%d)\n", strerror(-err), err);
  }
}


// Get all BMP on the root dir of "fs"
std::vector<std::string> get_files_by_ext(const char *ext){  
  std::vector<std::string> files;   
  DIR *d = opendir("/fs/");
  if (!d) {
    error("error: %s (%d)\n", strerror(errno), -errno);
  }
  else{
    while (true) {
        struct dirent *e = readdir(d);
        if (!e) {
        break;
        }
        if( strstr(e->d_name, ext) != NULL )
            files.push_back("/fs/"+std::string(e->d_name));
    }
    int err = closedir(d);
    if (err < 0) {
        error("error: %s (%d)\n", strerror(errno), -errno);
  }

  }
  return files;
}


int main() {
  VideoPlayer video_player(&myLCD);

  mount_file_system();
  vector<string> bmp_files = get_files_by_ext(".bmp");
  vector<string> video_files = get_files_by_ext(".bin");

  myLCD.set_orientation(0);
  printf("\n\nSystem Core Clock = %.3f MHZ\r\n",
         (float)SystemCoreClock / 1000000);

  myLCD.background(backgroundcolor); // set background to black
  myLCD.foreground(foregroundcolor); // set chars to white
  myLCD.invert(1);

  t.start();

  while (1) {

    long long time;
    unsigned int frames = 1;
    for (const string& bmp_file : bmp_files )
    {
        t.reset();
        int res = myLCD.BMP_16(0, 0, bmp_file.c_str() );
        time = duration_cast<milliseconds>(t.elapsed_time()).count();
        myLCD.locate(2, 70);
        myLCD.set_font((unsigned char *)Terminal6x8);
        myLCD.printf("%llu FPS\n", frames * 1000 / time);
        printf("%llu FPS\n", frames * 1000 / time);
        ThisThread::sleep_for(5s);
    }
//    for (const string& video_file : video_files )
//    {
        uint32_t fps=16;
        t.reset();
        File file(&fileSystem, "NP_4100-6200_16fps.craw");
        int res = video_player.play(file, fps);
        time = duration_cast<seconds>(t.elapsed_time()).count();
        myLCD.locate(2, 70);
        myLCD.set_font((unsigned char *)Terminal6x8);
        myLCD.printf("%u FPS video takes %llu seconds \n", fps, time);
        printf("%u FPS video takes %llu seconds \n", fps, time);
        ThisThread::sleep_for(10s);
//    }

  }
  unmount_fs();
}