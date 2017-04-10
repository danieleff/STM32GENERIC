
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mntent.h>

int main(int argc, char *argv[])
{
  int i;
  int ret = 0;
  int device_found = 0;
  char input_path[256];
  char output_dev[256];
  char output_path[256];
  char cmd[512];
  struct mntent *ent;
  FILE *aFile;


  for(i = 1; i < argc; i++) {

    if((strcmp(argv[i], "-I") == 0)&&(i+1 < argc)) {
      strcpy(input_path, argv[i+1]);
      i++;
    } else if((strcmp(argv[i], "-O") == 0)&&(i+1 < argc)) {
      strcpy(output_dev, argv[i+1]);
      i++;
    } else {
      printf("unknown option %s\n", argv[i]);
      ret = -1;
    }
  }

  if(ret == 0) {


    //get thee mounted devives list
    aFile = setmntent("/proc/mounts", "r");
    if (aFile == NULL) {
      perror("setmntent");
      exit(1);
    }

    //now lets read the path of the device
    while (NULL != (ent = getmntent(aFile))) {
      if (strstr(ent->mnt_dir, output_dev)) {
        sprintf(output_path, "%s", ent->mnt_dir);
        device_found = 1;
      }
    }

    endmntent(aFile);

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
