#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

//#include "SignalComms.h"

volatile sig_atomic_t hbreceived = 0;
volatile sig_atomic_t payloadpid;

void heartbeat(int sig, siginfo_t *info, void *uc)
{
    (void)sig;
    (void)uc;
    hbreceived = 1;
    payloadpid = info->si_pid;
}
int checkregistration(int *registration)
{
	for(int i = 0; i < 5; i++)
	{
		if(registration[i] == payloadpid)
		{
			return 1;
		}
	}
	return 0;

}
void printregistration(int *registration)
{
	for(int i = 0; i < 5; i++)
	{
		printf("%d\n",registration[i]);
	}
}

int main()
{
	pid_t parent = getpid();
	printf("Process ID: %d\n",parent);	
	int childpids[5] = {0};
	struct sigaction A = {0}; // Initialize to 0
	A.sa_sigaction = &heartbeat; //Register signal handler example1 to A
	A.sa_flags = SA_SIGINFO; //Turn on SA siginfo
	if(sigaction(SIGUSR1, &A, NULL) == -1)
	{
		return 1;
		
	}
	while(1)
	{
		hbreceived += 1;
		if(hbreceived == 5)
		{
			printf("Termination process\n");
			printf("Effected PIDS:\n");
			printregistration(childpids);
			return 1;
		}
		else
		{
			if((checkregistration(childpids))==0)
			{
				for(int i = 0;i<5;i++)
				{
					if(childpids[i] == 0)
					{
						childpids[i] = payloadpid;
						break;
					}
				}
			}
		}
		sleep(1);
	}
}
