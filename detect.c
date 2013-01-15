#include <linux/input.h>
#include <stdio.h>
#include <string.h> /* memset */
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

int read_volume() {
    printf("read_vol_start");
    FILE *fp;
    int status;
    char path[1035];
    int volume=100;

    fp = popen("/usr/bin/amixer sget Master | grep % | grep Left | cut -f 2 -d '[' | tr -d 'g/%]//'", "r");
      if (fp == NULL) {
        printf("Failed to run command\n" );
        exit;
      }

      /* Read the output a line at a time - output it. */
      while (fgets(path, sizeof(path)-1, fp) != NULL) {
        volume = atoi(path);
      }

      /* close */
      pclose(fp);
      printf("read_vol_return");
      return volume;
}

int main(int argc, char* argv[])
{
    int fd = -1;
    char name[256]= "Unknown";
    struct input_event event;
    int volume=100;

    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("evdev open");
        exit(1);
    }

    if(ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0) {
        perror("evdev ioctl");
    }

    //printf("The device on %s says its name is %s\n",
//            argv[1], name);
    while (1) {
        read(fd, &event, sizeof(struct input_event));
        //printf("Event type is %d\n", event.type);
        //printf("Event code is %d\n", event.code);
        //printf("Event value is %d\n", event.value);
        if (event.value == 1) {
            printf("Connected");

            //printf("volume: %d", volume);

            char str[512];
            memset(str, '\0', sizeof(512));
            sprintf(str, "amixer set Master %d", volume);
            printf("====== string:");
            //printf(str);
            system(str);
        } else if (event.value == 0) {
            printf("Disconnected");
            volume=read_volume();
            system("amixer set Master 0");
        }
    }
    close(fd);
    return 0;
}
