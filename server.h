#ifndef _ERIC_H_
#define _ERIC_H_

#define PORT 80  /* Default port */
#define error(msg) {perror(msg); exit(EXIT_FAILURE);}
#define strcmpbool(s1, s2) (strcmp(s1, s2) == 0)

#include <stdio.h> /* scanf family, printf family */
#include <stdlib.h> /* malloc, calloc, free */
#include <string.h> /* strcmp, strcpy, strcat, strrchr*/
#include <alloca.h>  /* alloca */
#include <unistd.h>  /* read, write */
#include <stdbool.h> /* -- */ /* type: bool */
#include <sys/socket.h>  /* socket, accept, bind, connect, listen, send, recv */
#include <sys/stat.h> /* stat() */
#include <arpa/inet.h> /* htons() */
#include <time.h>   /* time(), ctime(), localtime(), gmtime(), tzset() */
#include <fcntl.h>  /* open(), O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_TEMP */
#include <ctype.h>  /* isupper() */

/******* HTTP Status Codes ********/
/* Successful Operation */
#define OK 200
#define NO_CONTENT 204
/**/
#define NOT_MODIFIED 304
/* Client Errors 4xx */
#define BAD_REQUEST 400
#define NOT_FOUND 404
#define METHOD_NOT_ALLOWED 405

typedef struct index
{
	char file[50];
} index_t;

/* Configuration structure */
typedef struct config
{
	size_t listen;
	char root[300];
	index_t index[10];
} conf_t;
conf_t get_config(void);


/* File structure */
typedef struct file
{
	char *content;
	size_t size;
	char *type;
} file_t;


/* Structure used to map files' ".<type>" to their respective mimetype */
typedef struct type
{
	const char *extension;
	char *mime_type;
} type_t;


/**
 * finaldata - Struct to finalize the response
 * @response: Pointer to the response buffer
 * @keep_conn: Flag used to determine connection state
 *
 * Description: Struct used to conclude communication with a client
 */
typedef struct finaldata
{
	char *response;
	size_t size;
	bool keep_conn;
} finald_t;


struct handle_http_request
{
	char *http_method;
	finald_t (*handler)(char *);
};
typedef struct handle_http_request http_request_t;


struct http_response
{
	double http_ver;
	int status;
	char *reason;
	char server[20];
	char date[50];
	file_t file;
	char *connection;
};
typedef struct http_response http_response_t;


/* Structure used to communicate with the system's shell */
/* Like running cmdline commands and manipulating files  */
struct system
{
	int exitcode;
	char *output;
};
typedef struct system sys_t;


/* Method Handlers' helper function */
finald_t (*handle_request(char *http_method))(char *http_request);

/* Method handlers */
finald_t get(char     *http_request);
finald_t head(char    *http_request);
// finald_t post(char    *http_request);
// finald_t put(char     *http_request);
// finald_t deleter(char *http_request);
// finald_t options(char *http_request);
// finald_t trace(char   *http_request);

/* Error handlers */
finald_t bad_request(char        *);
finald_t not_found(char          *);
finald_t method_not_allowed(char *);

/* Util functions */
void set_resp_struct(http_response_t *resp, int status, char *reason, char      *connection);
finald_t get_file_generate_response(const char *path, http_response_t *resp, int enablefile);
char *mime_type(const char                    *ext);
char *index_path(const char *dir, const char *file);
int isupper_(const char *str);
void *alloc(int n);
void free_line(void *line, int memcount);
int strcmp_(const char *s1, const char *s2);

#endif /* _ERIC_H_ */
