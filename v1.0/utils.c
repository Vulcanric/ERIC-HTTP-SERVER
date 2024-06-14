#include "server.h"


/*** UTILS.C ***/
/** This file contains all necessary functions that enable this server to work effectively **/
/** They aren't the main function or handlers, but helper functions that make the process easier **/

/**
 * set_resp_struct - Sets/Resets http response headers structure
 * @resp:    Pointer to the response headers structure
 * @status: Operation status
 * @reason:      Reason for the status
 * @date:        Date and time of operation
 * @connection:  The connection state after operation
 *
 * Return:       Nothing
 */
void set_resp_struct(http_response_t *resp, int status, char *reason, char *connection)
{
	FILE *verfp = fopen("eric_/eric.ver", "r");
	char *version = NULL;
	size_t n = 0;

	if (!verfp)
	{
		error("Fopen() failed to open eric.ver");
	}
	else
		printf("Successfully opened eric_/eric.ver");

	resp->status = status;
	resp->http_ver = 1.1;
	resp->reason = reason;

	/* read n store the version of server from verfp to var respectively */
	getline(&version, &n, verfp);
	printf("[I malloc'd version: %p]\n", &(*version));
	version[strlen(version) - 1] = 0;  /* Remove trailing newline */
	printf("(Eric's Version is '%s')\n", version);
	fclose(verfp);

	/* Format the server header */
	sprintf(resp->server, "eric/%s", version);
	free(version);
	printf("[I free'd version: %p]\n", &(*version));

	/* Set current time of operation */
	time_t current_time = time(NULL);

	char *t = ctime(&current_time);
	t[strlen(t) - 1] = 0;  /* Remove trailing newline */
	sprintf(resp->date, "%s GMT+1", t);
	printf("(Date is %s)\n", resp->date);

	/* Set connection status */
	resp->connection = connection;
	printf("(Connection is %s)\n", resp->connection);
}


/**
 * get_config - Processes configuration file
 *
 * Return: a structure containing the configuration
 */
conf_t get_config(void)
{
	char *confile = "eric_/eric.conf", *line = NULL, *value, *key;
	FILE *fp = fopen(confile, "r");
	conf_t conf;
	size_t size = 0;
	int i;

	if (!fp)
		error("Fopen() failed to open eric_/eric.conf");

	while (getline(&line, &size, fp) != -1)
	{
		printf("[I malloc'd line: %p]\n", &(*line));
		size = 0;  /* Reset size */

		/* Working with address(in memory) here. Reason: @line */
		printf("%s", line);
		printf("(1. line: %p)\n", &(*line));
		int maddrcount = 0;  /* Address in memory counter */
		while (*line == '\t')
		{
			line++;
			maddrcount++;
		}

		printf("(2. line++: %p)\n", &(*line));
		printf("%s", line);

		if (*line == '#')  /* Skip comments */
		{
			free_line(line, maddrcount);
			continue;
		}

		key = strtok(line, " :");
		for (i = 0; (value = strtok(NULL, " :")); i++)
		{
			if (strcmp(key, "listen") == 0)
			{
				conf.listen = atoi(value);
				printf("Listen is %ld\n", conf.listen);
				break;
			}
			else if (strcmp(key, "root") == 0)
			{
				sprintf(conf.root, "%s", value);
				break;
			}
			else if (strcmp(key, "index") == 0)
			{
				strcpy(conf.index[i].file, value);
				printf("index[%d] is %s\n", i, conf.index[i].file);
			}
			else
				break;
		}
		free_line(line, maddrcount);  /* See below for definition */
	}
	fclose(fp);

	return (conf);
}


/**
 * free_line - See @Description !
 * @line: Pointer to a char
 * @memcount: Number of memory addresses moved forward to
 *
 * Returns: Nothing
 *
 * Description: When @line pointer is moved forward(line++) to point to the next address in memory,
 * it results in freeing an invalid pointer when summoning free() on @line, therefore this function
 * takes the pointer back to the first memory address allocated and free()'s it
 */
void free_line(void *line, int memcount)
{
	while (memcount > 0)
	{
		line--;
		memcount--;
	}
	printf("(3. line when freeing: %p)\n", &(*line));
	free(line);
	printf("[I free'd line: %p]\n", &(*line));
}


/**
 * mime_type - Determine a mime type of a file by its extension
 * @ext: Pointer to the extension of the file
 *
 * Return: pointer to the equivalent mime type
 */
char *mime_type(const char *ext)
{
	int i = 0;
	type_t match[] = {
		{".html|.htm", "text/html"},
		{".txt",      "text/plain"},
		{".css",        "text/css"},
		{".js",          "text/js"},
		{".jpg|jpeg", "image/jpeg"},
		{".png",       "image/png"},
		{".mp4",       "video/mp4"},
		{".mp3",      "audio/mpeg"},
		{NULL,                NULL}
	};

	/* Search until there's match in extension */
	while (match[i].extension && !strstr(match[i].extension, ext))
		i++;

	if (!match[i].extension)  /* If there isn't any match */
		i = 1;  /* Set mime type to text/plain */

	return (match[i].mime_type);
}


/**
 * get_file_generate_response - Helper function that will get file info
 * set it into the response structure and generate response buffer(message).
 * It assist all handlers for independency.
 * @path:       Path to file
 * @resp:      Response structure from handlers
 * @enablefile: Flag that determines if the file's content should be included
 * in the response. 1 if "yes" and 0 if "no"
 * Return:     A finald_t structure containing the generated buffer
 * and a keep-connection status
 */
finald_t get_file_generate_response(const char *path, http_response_t *resp, int enablefile)
{
	struct stat sb;
	int fd;

	/* Set file info */
	stat(path, &sb);

	resp->file.size = sb.st_size;
	resp->file.type = mime_type(strrchr(path, '.')); /* Determine mime type by file ext. */

	printf("Path is %s\n", path);
	if (enablefile == 1)
	{
		if ((fd = open(path, O_RDONLY)) == -1)
			error("Failed to open file");
		if (!(resp->file.content = calloc(sb.st_size, sizeof(char))))
			error("Failed to calloc memory for requested file content");
		if (read(fd, resp->file.content, sb.st_size) == -1)
			error("Failed to read file");
	}
	else
		resp->file.content = calloc(1, 1);
	printf("[I malloc'd resp->file.content: %p]\n", &(*resp->file.content));

	/* Generating HTTP response */
	char *buff = (char *)calloc(sb.st_size, sizeof(char));

	if (!buff)
		error("Failed to calloc memory for response buffer");
	printf("[I malloc'd buff: %p]\n", &(*buff));

	sprintf(buff, "HTTP/%.1f %d %s\r\nDate: %s\r\nServer: %s (Ubuntu)\r\nContent-Length: %ld\r\nContent-Type: %s\r\nConnection: %s\r\n\r\n%s", resp->http_ver, resp->status, resp->reason, resp->date, resp->server, resp->file.size, resp->file.type, resp->connection, resp->file.content);

	close(fd);
	free(resp->file.content);
	printf("[I free'd resp->file.content: %p]\n", &(*resp->file.content));

	finald_t report = {buff, strlen(buff), (strcmpbool(resp->connection, "keep-alive"))};
	return (report);
}


/**
 * index_path - Helps form indexfile path of a directory specified as URI resource
 * @dir: The directory
 * @index: The index file name
 *
 * Return: "<dir>/<index>"
 *
 * Description: If the request path is a directory, it helps to create the path
 * to the html file in that directory.
 * For example, if request is "GET /resource/ HTTP/1.1", it returns "/resource/index.html"
 * And if request is "GET /resource HTTP/1.1", it adds a trailing '/' inbetween "/resource/index.html"
 */
char *index_path(const char *dir, const char *index)
{
	char *new_path = alloca(200);

	if (dir[strlen(dir) - 1] != '/')
		sprintf(new_path, "%s/%s", dir, index);
	else
		sprintf(new_path, "%s%s", dir, index);

	printf("%s\n", new_path);
	return (new_path);
}


/**
 * isupper_ - Checks if all the characters of a string are uppercase letters
 * @str: Pointer to first char of the string
 *
 * Returns: 1 if all chars are uppercase, 0 otherwise
 */
int isupper_(const char *str)
{
	int i;

	for (i = 0; str[i]; i++)
	{
		if (isupper(str[i]) == 0)
			return (0);  /* Not an uppercase */
	}
	return (1);  /* is uppercase */
}

/**
 * strcmp_ - Compares the equality between two strings
 * @s1: First string
 * @s2: Second string
 *
 * Return: A non-zero integer if they aren't equal, and 0 if they are
 */
int strcmp_(const char *s1, const char *s2)
{
	int i, j, cmp = 0;

	if (!s1 || !s2)
		return (-1);

	i = j = 0;
	while ((s1[i] && s2[j]) && (s1[i] == s2[j]))
		i = j = i + 1;

	return (s1[i] - s2[j]);
}
