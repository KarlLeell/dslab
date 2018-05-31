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

std::string
kv_server::marshal_state()
{
	//Lab3: You fill this function to marshal the state of the kv_server
	//Hint: you may want to use ostringstream to marshal your state
	std::ostringstream ost;
        pthread_mutex_lock(&mu);
        ost << dict.size() << " ";
        std::map<std::string, kv_protocol::versioned_val>::iterator it;
        for(it = dict.begin(); it != dict.end(); ++it) {
            ost << it->first << " ";
            ost << it->second.buf << " ";
            ost << it->second.version << " ";
        }
        pthread_mutex_unlock(&mu);
	return ost.str();
}

void
kv_server::unmarshal_state(std::string state)
{
	//Lab3: You fill this function to unmarshal the transferred state into kv_server
	//Hint: use istringstream to extract stuff out of the state string
  	std::istringstream ist(state);
        pthread_mutex_lock(&mu);
        int len;
        ist >> len;
        for(int i = 0; i < len; i++) {
            std::string key, value;
            int version;
            ist >> key >> value >> version;
            kv_protocol::versioned_val v;
            v.buf = value;
            v.version = version;
            dict[key] = v;
        }
        pthread_mutex_unlock(&mu);
}
