#include <linux/input.h>
#include <stdio.h>
#include <string.h> /* memset */
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

#define JACK_EVENT_TYPE 5
#define JACK_EVENT_CODE 2

// Exit codes
#define SUCCESS 0
#define AMIXER_FAIL 1
#define EVDEV_FAIL 1

#define DEBUG 0

int read_volume() {
  FILE *fp;
  int status;
  char path[1035];
  int volume=100;

  fp = popen("/usr/bin/amixer sget Master | grep % | grep Left | cut -f 2 -d '[' | tr -d 'g/%]//'", "r");
    if (fp == NULL) {
      perror("Failed to run command\n" );
      exit(AMIXER_FAIL);
  }

  /* Read the output a line at a time - output it. */
  while (fgets(path, sizeof(path)-1, fp) != NULL) {
    volume = atoi(path);
  }

  /* close */
  pclose(fp);
  return volume;
}

int main(int argc, char* argv[]) {
  int fd = -1;
  char name[256] = "Unknown";
  struct input_event event;
  int volume = read_volume();
  char *dev = "/dev/input/event10";


  if ((fd = open(dev, O_RDONLY)) < 0) {
      perror("evdev open");
      exit(EVDEV_FAIL);
  }

  if(ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0) {
      perror("evdev ioctl");
  }

  while (1) {
    read(fd, &event, sizeof(struct input_event));

    if (DEBUG) {
      printf("Event type is %d\n", event.type);
      printf("Event code is %d\n", event.code);
      printf("Event value is %d\n", event.value);
    }

    // Event type and code is defined above (differs for different hardware)
    if (event.type == JACK_EVENT_TYPE && event.code == JACK_EVENT_CODE) {
      if (event.value == 1) {
        char str[512];
        memset(str, '\0', sizeof(512));
        sprintf(str, "./setvol.sh %d", volume);

        // Restore volume
        system(str);
      } else if (event.value == 0) {
        if (DEBUG) {
            printf("Disconnected\n");
        }

        // Save current volume
        volume = read_volume();
        // Mute
        system("./setvol.sh 0");
      }
    }
  }

  close(fd);
  return SUCCESS;
}
