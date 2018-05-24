// the extent server implementation

#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#include "lang/verify.h"
#include "kv_server.h"

#include <map>
std::map<std::string, kv_protocol::versioned_val> dict;
std::map<std::string, int> flag;

kv_server::kv_server() 
{
    pthread_mutex_init(&mu, NULL);
}

/* The RPC reply argument "val" should contain 
 * the retrieved val together with its current version 
 */
int 
kv_server::get(std::string key, kv_protocol::versioned_val &val)
{
	// You fill this in for Lab 1.
        pthread_mutex_lock(&mu);
        if(dict.count(key) == 0 || flag[key] == 0) {
            pthread_mutex_unlock(&mu);
	    return kv_protocol::NOEXIST;
        } else {
            val.version = dict[key].version;
            val.buf = dict[key].buf;
            pthread_mutex_unlock(&mu);
            return kv_protocol::OK;
        }
}


/* the server should store the key-value entry, increment its version if appropriate.
   and put the new version of the stored entry in the RPC reply (i.e. the last argument)
*/
int 
kv_server::put(std::string key, std::string buf, int &new_version)
{
	// You fill this in for Lab 1.
        pthread_mutex_lock(&mu);
        if(dict.count(key) == 0) {
            kv_protocol::versioned_val n_val;
            n_val.version = 1;
            n_val.buf = buf;
            dict[key] = n_val;
        } else {
            dict[key].buf = buf;
            dict[key].version += 1;
        }
        flag[key] = 1;
        new_version = dict[key].version;
        pthread_mutex_unlock(&mu);
	return kv_protocol::OK;
}

/* "remove" the existing key-value entry
 * do not actually delete it from your storage
 * treat it as a special type of put and 
 * increments the key-value pair's version number
 * like regular puts.
 * Set a delete flag to the "removed" key-value 
 * entry so a later get does not see it.
*/
int 
kv_server::remove(std::string key, int &new_version)
{
	// You fill this in for Lab 1.
        pthread_mutex_lock(&mu);
        if(flag[key] == 0) {
            if(dict.count(key) == 0) {
                kv_protocol::versioned_val n_val;
                n_val.version = 1;
                n_val.buf = "";
                dict[key] = n_val;
            } else {
                dict[key].version += 1;
            }
            flag[key] = 0;
            pthread_mutex_unlock(&mu);
	    return kv_protocol::NOEXIST;
        } else {
            dict[key].version += 1;
            flag[key] = 0;
            new_version = dict[key].version;
            pthread_mutex_unlock(&mu);
            return kv_protocol::OK;
        }
}

int 
kv_server::stat(int x, std::string &msg)
{
	msg = "Server says: I am kicking";	
	return kv_protocol::OK;
}
