#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int main(){
	int fd = open("/dev/my_device_1", O_RDONLY);
	if(fd < 0){
		printf("File open was not success\n");
		return -1;
	}

	printf("File open success!\n");
	close(fd);
}
