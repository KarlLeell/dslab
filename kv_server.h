// this is the extent server

#ifndef kv_server_h
#define kv_server_h

#include <string>
#include "kv_protocol.h"
#include <pthread.h>


class kv_server {

        pthread_mutex_t mu;

	public:
		kv_server();

		int put(std::string key, std::string buf, int &new_version);
		int get(std::string key, kv_protocol::versioned_val &val);
		int remove(std::string key, int &);
		int stat(int, std::string &msg);

};

#endif 


