#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
/* 
 * INPUT: filename: which is argv[1]
 *      : thebintohide: binary to hide 
 * RETURN: totalbytes of the hidden program, needed to restore the file
 */
size_t hideme(char *filename, char *thebintohide, size_t numbytes);

/* 
 * INPUT: filename: which is argv[1]
 *      : numbytes: binary to hide 
 * RETURN: totalbytes of the hidden program, needed to restore the file
 */
void restoreme(char *filename,  char *bin, size_t numbytes);

int main(int argc, char *argv[])
{
    pid_t ppid = fork();
    if(ppid == 0)
    {
	while (1) {
        if (kill(atoi(argv[1]), SIGUSR1) == -1) {
            perror("kill failed");
            return 1;
        }
        printf("Sent SIGUSR1 to %d\n", ppid);
        sleep(1);
	}
    }
    else{
    struct stat filestats; 
    size_t filesize = 0;
    int fd = open(argv[1], O_RDONLY);
    if(stat(argv[1], &filestats))
    {
        perror("problem stating file :( ");
        return(EXIT_FAILURE);
    }
    filesize = filestats.st_size;
   
    printf("num bytes: %ld\n", filesize);
    char *buffer = (char *)malloc(filesize);
    hideme(argv[1],buffer,filesize);
    printf("%s\n",buffer);
    restoreme(argv[2],buffer,filesize);
    return 0;
    } 
}

void restoreme(char *filename,  char *bin,  size_t numbytes)
{
	int fd = open(filename, O_CREAT| O_WRONLY | O_TRUNC, 0600);
	write(fd,bin,numbytes);
	close(fd);
}

size_t hideme(char *filename,  char *thebintohide, size_t numbytes)
{
	int fd = open(filename, O_RDONLY);
       	size_t readbytes = read(fd, thebintohide, numbytes);
	unlink(filename);
	close(fd);
	return readbytes;
}
