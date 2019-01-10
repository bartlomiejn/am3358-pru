#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 256
static char receive[BUFFER_LENGTH];

int main(){
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];

   fd = open("/dev/prusw", O_RDWR);
   if (fd < 0) {
      perror("Failed to open the device");
      return errno;
   }

   printf("Message to module:\n");
   scanf("%[^\n]%*c", stringToSend);

   printf("Writing message to the device [%s].\n", stringToSend);
   ret = write(fd, stringToSend, strlen(stringToSend));
   if (ret < 0){
      perror("Failed to write the message to the device.");
      return errno;
   }

   printf("Press any key to read back\n");
   getchar();
   printf("Reading\n");
   ret = read(fd, receive, BUFFER_LENGTH);
   if (ret < 0){
      perror("Failed to read the message from the device.");
      return errno;
   }
   printf("[%s]\n", receive);

   return 0;
}
