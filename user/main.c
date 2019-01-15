#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/poll.h>

#define DEVICE_NAME	"/dev/rpmsg_pru30"
#define BUF_SIZE 512

int main(void)
{
	struct pollfd pollfd;
    int read_status;
    char *write_buf = "switch1";
    char read_buf[BUF_SIZE];
    int write_buf_len = strlen(write_buf);
	pollfd.fd = open(DEVICE_NAME, O_RDWR);
	if (pollfd.fd < 0) {
		printf("Failed to open %s\n", DEVICE_NAME);
		return -1;
	}
	printf("Opened %s\n", DEVICE_NAME);
    if (write(pollfd.fd, "switch1", 7) != 7)
    {
        printf("Write error");
    }
    printf("Sent switch1");
    while (read_status = read(pollfd.fd, read_buf, BUF_SIZE) > 0)
    {
        printf("Received %d, read_buf: %s", read_status, read_buf);
    }
    printf("Finished receiving: %s", read_buf);
    if (read_status < 0)
    {
        printf("Error: %d", read_status);
    }

	close(pollfd.fd);
	return 0;
}
