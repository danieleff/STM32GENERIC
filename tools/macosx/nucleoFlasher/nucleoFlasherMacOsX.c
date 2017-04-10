
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mount.h>

#define MAX_FS 128

int main(int argc, char *argv[])
{
  int i;
  int ret = 0;
  int device_found = 0;
  char input_path[256];
  char output_dev[256];
  char output_path[256];
  char cmd[512];
  struct statfs buf[MAX_FS];
  int fs_count;

  if(argc < 4) {
    printf("error: missing parameters\n");
    ret = -1;
  }

  for(i = 1; i < argc; i++) {

    if((strcmp(argv[i], "-I") == 0)&&(i+1 < argc)) {
      strcpy(input_path, argv[i+1]);
      i++;
    } else if((strcmp(argv[i], "-O") == 0)&&(i+1 < argc)) {
      strcpy(output_dev, argv[i+1]);
      i++;
    } else {
      printf("error: unknown option %s\n", argv[i]);
      ret = -1;
    }
  }

  if(ret == 0) {

    fs_count = getfsstat(NULL,0,MNT_WAIT);
    if(fs_count < 0) {
      perror("getfsstat");
      exit(1);
    }

    getfsstat(buf,fs_count*sizeof(buf[0]),MNT_WAIT);

    for(i = 0; i < fs_count; i++) {
      if(strstr(buf[i].f_mntonname,output_dev)) {
        sprintf(output_path, "%s", buf[i].f_mntonname);
        device_found = 1;
      }
    }

    if(device_found) {
      printf("copying %s to %s\n", input_path, output_path);
      sprintf(cmd, "scp %s %s", input_path, output_path);
      system(cmd);
    } else {
      printf("%s not found. please ensure the device is correctly connected\n",
                output_dev);
      ret = -1;
    }
  }

  return ret;
}
