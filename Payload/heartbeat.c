#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#include "SignalComms.h"

volatile sig_atomic_t hbreceived = 0;
volatile sig_atomic_t payloadpid;

void heartbeat(int sig, siginfo_t *info, void *uc)
{
    (void)sig;
    (void)uc;
    hbreceived = 1;
    payloadpid = info->si_pid;
}
int checkregistration(int *registration, int payloadpid)
{
	for(int i = 0; i < 5; i++)
	{
		if(registration[i] == payloadpid)
		{
			printf("Child already in\n");
			return 1;
		}
	}
	return 0;

}
void printregistration(int *registration)
{
	for(int i = 0; i < 5; i++)
	{
		printf("%d\n",registartion[i]);
	}
}

int main()
{
	int childpids[5] = {0};
	struct sigaction A = {0}; // Initialize to 0
	A.sa_sigaction = &example1; //Register signal handler example1 to A
	A.sa_flags = SA_SIGINFO; //Turn on SA siginfo
	if(sigaction(SIGUSR1, &A, NULL) == -1)
	{
		if((checkregistration(childpids,payloadpid)) == 0)
		{
			for(int j = 0;j < 5; j++)
			{
				if(childpids[j] == 0)
				{
					childpids[j] = payloadpid;
				}
			}
		}
		printregistration(childpids);
		
	}
    time_t current_time;
    time_t last_heartbeat_time;

    // Get the initial time
    time(&current_time);
    last_heartbeat_time = current_time;
}
