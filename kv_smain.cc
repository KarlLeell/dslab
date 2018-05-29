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

	//In Lab 3, we will make the kv server use your RSM layer.
	rsm rsm(argv[1], argv[2]);
	kv_server kvs;
	rsm.set_state_transfer((rsm_state_transfer *)&kvs);
	rsm.reg(kv_protocol::put, &kvs, &kv_server::put);
	rsm.reg(kv_protocol::get, &kvs, &kv_server::get);
	rsm.reg(kv_protocol::remove, &kvs, &kv_server::remove);
	rsm.reg(kv_protocol::stat, &kvs, &kv_server::stat);

	while(1)
		sleep(1000);
}
