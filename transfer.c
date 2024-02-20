#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "client_hello.h"

int main(int argc, char * argv[]) {
	struct client_hello * client_hello = make_client_hello();
	struct data * request = encode_client_hello(client_hello);
	destroy_client_hello(client_hello);

	FILE * client_output = fopen("client_output.bin", "wb");
	fwrite(request->payload, request->size, 1, client_output);
	
	fclose(client_output);
	//destroy_data(request);
	//return 0;

	// ------------------------------------------------------------------------------------

	int socket_id;
	struct addrinfo dns_hints, * dns_result;
	struct sockaddr_in server_address;

	memset(&dns_hints, 0, sizeof(struct addrinfo));
	dns_hints.ai_family = AF_INET; // Use IPv4
  dns_hints.ai_socktype = SOCK_STREAM;

	int dns_status = getaddrinfo(argv[1], NULL, &dns_hints, &dns_result);
	if (dns_status != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(dns_status));
		return -1;
	}
	struct sockaddr_in * ip_result = (struct sockaddr_in *)dns_result->ai_addr;

	if ((socket_id = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "socket: %s", strerror(errno));
		return -1;
	}

	memset(&server_address, 0, sizeof(struct sockaddr_in));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(80);
	server_address.sin_addr = ip_result->sin_addr;

	if (connect(socket_id, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
			fprintf(stderr, "connect: %s\n", strerror(errno));
			return -1;
	}

	// ------------------------------------------------------------------------------------
	void * response = malloc(4096);
	FILE * server_input = fopen("server_input.bin", "wb");

	write(socket_id, request->payload, request->size);
	size_t received = recv(socket_id, response, 4096, 0);
	fwrite(response, 1, received, server_input);
	
	fclose(server_input);
	free(response);
	destroy_data(request);

	// ------------------------------------------------------------------------------------

	close(socket_id);
  return 0;
}
