#include "server.h"

conf_t config;

#undef PORT /* - */
#define PORT ({ (config = get_config()).listen; })

/**
 * main - Just a basic web server for now, I will continuously upgrade it
 * 
 * Return: 0
 */
int main(void)
{
	/* Server */
	int eric_sock;
	struct sockaddr_in eric_addr;
	socklen_t eric_size = sizeof(eric_addr);

	/* Communication */
	char request[2048] = {0};
	ssize_t bytesrecv;

	/* Create a socket (end-point) first */
	if ((eric_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error("socket"); // Check this function-like-macro in server.h

	/* Set the address of my WebServer */
	eric_addr.sin_family = AF_INET;  // IPv4
	eric_addr.sin_port = htons(PORT);
	eric_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces (IP address)
	memset(eric_addr.sin_zero, 0, 8);  // Make it compatible with "struct sockaddr"

	/* Bind the socket to the address for communication */
	if (bind(eric_sock, &eric_addr, eric_size) != -1)
		printf("Socket is bound successfully!\n");
	else
		error("bind");

	/* Listen for incoming connection */
	if (listen(eric_sock, 20) != -1)  // Maximum of 20 pending request
		printf("Listening...\n");
	else
		error("listen");

	while (1)
	{
		/* Client connection */
		int client_sock;
		struct sockaddr_in client_addr;
		socklen_t client_size = sizeof(client_addr);

		/* Accepts an incoming connection and communicate with it */
		if ((client_sock = accept(eric_sock, &client_addr, &client_size)) == -1)
			error("accept");

		int pid = fork();

		if (pid == 0)  /* Inside Child process */
		{
			memset(request, 0, 2048);
			bytesrecv = recv(client_sock, request, 2048, 0);
			printf("%ld\n bytes received\nREQUEST HEADERS:\n%s\n", bytesrecv, request);

			char *save_request = alloca(1024);
			memset(save_request, 0, 1024);
			char *http_method = strtok_r(request, " ", &save_request);

			finald_t report = handle_request(http_method)(save_request);
			send(client_sock, report.response, report.size, 0);

			free(report.response);
			exit(0);
		}
		else /* Parent */
			close(client_sock);
	}

	close(eric_sock);
	shutdown(eric_sock, SHUT_RDWR);

	return (0);
}
