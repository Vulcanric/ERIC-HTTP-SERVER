#include "server.h"

/*
 * This file contains all the function used to process HTTP Methods
 * They process the request, do what is meant to be done and generates response
 * according to the request
 */


extern conf_t config;  /* From main() */

/**
 * get - Processes HTTP GET request and generates a response
 * @http_request: The request
 *
 * Return: the generated response and a flag to determine keeping the connection
 */
finald_t get(char *http_request)
{
	char *uri = strtok(http_request, " ");
	struct stat sb;

	http_response_t resp;  /* Used to set the response headers */

	char resource_path[200] = {0};
	strcpy(resource_path, config.root); /* Substitute root for value of '/' in uri */
	resource_path[strlen(resource_path) - 1] = 0; /* Remove trailing newline char */
	strcat(resource_path, uri);  /* Then append the uri to the root */

	/* Checking if the file exist */
	if (stat(resource_path, &sb) != 0)
		return (not_found(NULL));
	else
	{
		/* If it's a directory */
		if (S_ISDIR(sb.st_mode))
		{
			/* Check if index files are found in it */
			int i = 0, isfound;
			struct stat newsb;

			/* Search for index file */
			while ((isfound = stat(index_path(resource_path, config.index[i].file), &newsb)) == -1)
				i++;

			if (isfound != 0)
				return (not_found(NULL));

			/* If found */
			strcpy(resource_path, index_path(resource_path, config.index[i].file));  /* Set to new path */
			printf("SEE MY FILE2: %s\n", resource_path);
			sb = newsb;
		}
		set_resp_struct(&resp, OK, "OK", "keep-alive");
	}


	printf("(Resource path is %s)\n", resource_path);
	return (get_file_generate_response(resource_path, &resp, 1));  /* 1 rep. enable file see server.h */
}


/**
 * head - Processes HTTP HEAD request and generates a response
 * @http_request: The request
 *
 * Return: The generated response and a flag to determine keeping the connection
 */
finald_t head(char *http_request)
{
	char *uri = strtok(http_request, " ");
	struct stat sb;

	http_response_t resp;  /* Used to set the response headers */

	char resource_path[200] = {0};
	strcpy(resource_path, config.root); /* Substitute root for value of '/' in uri */
	resource_path[strlen(resource_path) - 1] = 0; /* Remove trailing newline char */
	strcat(resource_path, uri);  /* Then append the uri to the root */

	/* Checking if the file exist */
	if (stat(resource_path, &sb) != 0)
	{
		return (not_found(NULL));
	}
	else
	{
		/* If it's a directory */
		if (S_ISDIR(sb.st_mode))
		{
			/* Check if index files are found in it */
			int i = 0, isfound;
			struct stat newsb;

			while ((isfound = stat(index_path(resource_path, config.index[i].file), &newsb)) == -1)
				i++;

			if (isfound != 0)
				return (not_found(NULL));

			/* If found */
			strcpy(resource_path, index_path(resource_path, config.index[i].file));
			sb = newsb;
		}
		set_resp_struct(&resp, OK, "OK", "keep-alive");
	}

	return (get_file_generate_response(resource_path, &resp, 0)); /* Don't enable file's content */
}

/*
finald_t post(char *http_request)
{
};


finald_t put(char *http_request)
{
};



**
 * deleter - Processes the HTTP DELETE request, by deleting the requested file or directory
 * @http_request: The request
 *
 * Return: The generated response and a flag to determine keeping the connection
 *
finald_t deleter(char *http_request)
{
};
*/
