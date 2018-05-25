#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include "paxos.h"
#include "rsm.h"

#include "rpc/jsl_log.h"

#include "kv_server.h"

int
main(int argc, char *argv[])
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);

	srandom(getpid());

	//uncomment the following line to print a lot of debug info
	//jsl_set_debug(JSL_DBG_4);

	if(argc != 3){
		fprintf(stderr, "Usage: %s [master:]port [me:]port\n", argv[0]);
		exit(1);
	}

	//In Lab2, we disable your kv server and focus on Paxos implementation.
	rsm rsm(argv[1], argv[2]);

	while(1)
		sleep(1000);
}
