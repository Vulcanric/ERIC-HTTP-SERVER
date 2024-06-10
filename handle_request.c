#include "server.h"

finald_t do_nothing(char *http_request);
/**
 * handle_request - Helper function used to determine the right handler
 * for the http method used
 * @http_method: the method to handle
 * @int (*) (char *http_request): the request message to be processed by the
 * http_method handler
 *
 * Return: A function pointer to the @http_method handler function
 * Description: It matches the right method handler for an http method and
 * returns a pointer to it
 */
finald_t /* - */(*handle_request(char *http_method))(char *http_request)
{
	int i = 0;
	http_request_t match[10] = {
		{"GET",         get},
		{"HEAD",       head},
//		{"POST",       post},
//		{"PUT",         put},
//		{"DELETE",  deleter},
//		{"OPTIONS", options},
//		{"TRACE",     trace},
		{NULL,         NULL}
	};

	while (match[i].http_method && (strcmp_(match[i].http_method, http_method) != 0))
		i++;

	/* If the method doesn't match the above */
	if (!match[i].http_method)
	{
		finald_t (*handleMismatch)(char *);
		/* If the method is not composed of uppercase chars */
		if (isupper_(http_method) != 1)
			handleMismatch = &bad_request;
		else
			handleMismatch = &method_not_allowed;

		return (handleMismatch);  /* Calls function bad_request or method_not_allowed depending on outcome */
	}

	return (match[i].handler);
}


/**
 * do_nothing - Actually does nothing. Just used to avoid SIGSEGV
 * @http_request: Unused
 *
 * Return: A phony finald_t struct
 */
finald_t do_nothing(char *http_request __attribute__((unused)))
{
	finald_t not;

	return (not);
}
