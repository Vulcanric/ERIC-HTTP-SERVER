#include "server.h"

/*** HTTP_ERROR_HANDLERS ***/
/** This file contains all the functions that serve an error response handlers **/

/**
 * bad_request - Sends a 400 status to the client, indicating
 * that the request(method) is not valid
 * @http_request: Not used, as this function is being called
 * within other handlers
 * Return: A finald_t structure containing the generated buffer
 * and a keep-connection status
 */
finald_t bad_request(char *http_request __attribute__((unused)))
{
	char *path = "http_error_codes/400.html";
	http_response_t resp; /* Response structure */

	set_resp_struct(&resp, BAD_REQUEST, "Bad Request", "closed");
	return (get_file_generate_response(path, &resp, 1));
}

/**
 * not_found - Sends a 404 status to the client, indicating
 * that the requested resource is not found
 * @http_request: Not used, as this function is being called
 * within other handlers
 *
 * Return: A finald_t structure containing the generated buffer and a keep-connection status.
 */
finald_t not_found(char *http_request __attribute__((unused)))
{
	char *path = "http_error_codes/404.html";
	http_response_t resp;

	set_resp_struct(&resp, NOT_FOUND, "Not Found", "keep-alive");
	return (get_file_generate_response(path, &resp, 1));
}


/**
 * method_not_allowed - Sends a 405 status to the client, indicating that the request(method) is not allowed
 * @http_request: Not used, as this function is being called within other handlers
 *
 * * Return: A finald_t structure containing the generated buffer and a keep-connection status.
 */
finald_t method_not_allowed(char *http_request __attribute__((unused)))
{
	char *path = "http_error_codes/405.html";
	http_response_t resp;

	set_resp_struct(&resp, METHOD_NOT_ALLOWED, "Method Not Allowed", "keep-alive");

	return (get_file_generate_response(path, &resp, 1));
}
