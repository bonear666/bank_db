#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <libpq-fe.h>
//#include <Windows.h>
#include <microhttpd.h>

#define C_STD_ERROR -1

#define PORT 80
#define FILE_NAME "/res/cat.jpg"

#define HOME_PAGE_NAME "/res/home_page.html"
#define EMPLOYEE_PAGE_NAME "/res/employee_page.html"
#define CLIENT_PAGE_NAME "/res/client_page.html"

#define MIME_TYPE "image/jpg"
#define MIME_HTML "text/html; charset=UTF-8"
#define MIME_JPEG "image/jpeg"

#define USER_NAME ""
#define USER_PASSWORD ""

#define REQUEST_BUFFER_LENGTH 150
#define HEADER_VALUES_BUFFER_LENGTH 150

#define HOST_NAME "sigma-bank.ru"
#define HOST_NAME_LENGTH 14 //null-terminated 
#define HOST_NAME_OFFSET (sizeof("http://") - 1)

char* db_request_buffer = NULL;
char* header_values_buffer= NULL;
const char* const page_names[] = 
{
	"sigma-bank.ru",
	"sigma-bank.ru/employee",
	"sigma-bank.ru/client"
};
const char* const resource_names[] = 
{
	"/res/home_img1.jpg"
};

MHD_Result Print_Out_Key(
	void* cls, enum MHD_ValueKind kind, 
	const char* key, const char* value)
{
	printf("%s: %s\n", key, value);

	return MHD_YES;
};

int DB_Get_Full_Private_Table(int member_id, char member_type, PGresult** request_result)
{
	*request_result = NULL;
	
	switch(member_type)
	{
		case 'e':
		strcat(db_request_buffer, );
		break;
		
		case 'c':
		strcat(db_request_buffer, );
		break;
	}
};

MHD_Result Sending_Response(FILE** response_file, stat* file_stat, const char* resource_file_name, int member_id, const char* header_field, const char* mime_type, MHD_Response** response, MHD_Connection** connection) 
{
	if (((*response_file = fopen(resource_file_name, "r")) == NULL) ||
		 (fstat(_fileno(*response_file), &file_stat) == C_STD_ERROR))
	{
		Iternal_Error_Handling(NULL);

		return MHD_NO;
	}

	//sending response
	*response = MHD_create_response_from_fd_at_offset64(file_stat.st_size, _fileno(*response_file), 0);
	MHD_add_response_header(*response, header_field, mime_type);
	ret = MHD_queue_response(*connection, MHD_HTTP_OK, *response);
	MHD_destroy_response(*response);

	return ret;
};

//member_type('e' means employee, 'c' means client), request_res - sql-request result storage
int DB_Auth_Request(char member_type, const char* username, const char* password, PGresult** request_result, int* member_id)
{
	*request_result = NULL;
	
	switch (member_type)
	{
		case 'e':
		strcat(db_request_buffer, "SELECT employee_login, employee_password \
								   FROM employee_account \
								   WHERE employee_login = ");
		strcat(db_request_buffer, username);
		strcat(db_request_buffer, " ");
		strcat(db_request_buffer, "AND employee_password = ");
		strcat(db_request_buffer, password);
		strcat(db_request_buffer, ";");
		
		*request_result = PQexec(db_connection, db_request_buffer);
		break;
		
		case 'c':
		strcat(db_request_buffer, "SELECT client_login, client_password \
								   FROM client_account \
								   WHERE client_login = ");
		strcat(db_request_buffer, username);
		strcat(db_request_buffer, " ");
		strcat(db_request_buffer, "AND client_password = ");
		strcat(db_request_buffer, password);
		strcat(db_request_buffer, ";");
		
		*request_result = PQexec(db_connection, db_request_buffer);
		break;
	}	
	
	if (*request_result != NULL)
	{
		if (PQntuples(*request_result) != 0) 
		{
			*member_id = atoi(PQgetvalue(*request_result, 0, 0)); 
			
			return EXIT_SUCCESS;
		}
	} 

	return EXIT_FAILURE;
};

MHD_Result Answer_To_Connection(
	void* cls, struct MHD_Connection* connection,
	const char* url, const char* method,
	const char* version, const char* upload_data,
	size_t* upload_data_size, void** con_cls) 
{
	struct MHD_Response* response;
	MHD_Result ret = MHD_NO;
	FILE* response_file;
	struct stat file_stat;
	char* username = NULL;
	char* password = NULL;
	int member_id = 0;
	int access_fail;
	int access_res;
	PGresult* request_result;
	
	//printing connection info
	printf("New %s request for %s using version %s\n", method, url, version);
	MHD_get_connection_values(connection, MHD_HEADER_KIND, &Print_Out_Key, NULL);
	
	//handling GET-request
	if (strcmp("GET", method) != 0) return MHD_NO;
	if (*con_cls == NULL) {*con_cls = connection; return MHD_YES;}
	
	//url exploration (http, no secure)(const char* url mb contains only urn, hostname contains in same-name header field)
	//home page has been requsted
	if 		(strcmp(url + HOST_NAME_OFFSET, page_names[0]) == 0)
	{
		if 		(strncmp(upload_data, "Clinet Auth Req", 15) == 0)
		{
			username = MHD_basic_auth_get_username_password(connection, &password);
			
			access_res = DB_Auth_Request('c', username, password, &request_result, &member_id);
			
			if (access_res == EXIT_SUCCESS)
			{
				ret = Sending_Response(&response_file, &file_stat, CLIENT_PAGE_NAME, member_id, "Content-Type", MIME_HTML, &response, &connection);
			}
			else
			{
				Iternal_Error_Handling("Wrong client info or Iternal error occured");
				
				ret = MHD_NO;
			}
			
			if (request_result != NULL) {PQclear(request_result);}
			
			return ret;
		}
		else if (strncmp(upload_data, "Employee Auth Req", 17) == 0)
		{
			username = MHD_basic_auth_get_username_password(connection, &password);
			
			access_res = DB_Auth_Request('e', username, password, &request_result, &member_id);
			
			if (access_res == EXIT_SUCCESS)
			{
				ret = Sending_Response(&response_file, &file_stat, EMPLOYEE_PAGE_NAME, member_id, "Content-Type", MIME_HTML, &response, &connection);
			}
			else
			{
				Iternal_Error_Handling("Wrong employee info or Iternal error occured");
				
				ret = MHD_NO;
			}
			
			if (request_result != NULL) {PQclear(request_result);}
			
			return ret;
		}
		ret = Sending_Response(&response_file, &file_stat, HOME_PAGE_NAME, 0, "Content-Type", MIME_HTML, &response, &connection);
		
		return ret;
	}
	//employee page has been requested
	else if (strcmp(url + HOST_NAME_OFFSET, page_names[1]) == 0)
	{
		ret = Sending_Response(&response_file, &file_stat, HOME_PAGE_NAME, 0, "Content-Type", MIME_HTML, &response, &connection);

		return ret;
	}
	//clinet page has been requested
	else if (strcmp(url + HOST_NAME_OFFSET, page_names[2]) == 0)
	{
		ret = Sending_Response(&response_file, &file_stat, HOME_PAGE_NAME, 0, "Content-Type", MIME_HTML, &response, &connection);

		return ret;
	}
	//page resource has been requested(http-request url contains resource path)
	else if (strncmp(url, "/res/", 5) == 0)
	{
		ret = MHD_NO;
		const char* header_values = NULL;
		size_t header_values_size = NULL;
		
		MHD_lookup_connection_value_n(connection, MHD_HEADER_KIND, "Accept", &header_values, &header_values_size);
		
		if 		(strncmp(header_values, "image/", 6) == 0)
		{
			ret = Sending_Response(&response_file, &file_stat, url, "Content-Type", MIME_JPEG, &response, &connection);
			
			return ret;
		}
		else if (strncmp(header_values, "text/", 5) == 0)
		{
			//ret = Sending_Response(&response_file, &file_stat, url, "Content-Type", , &response, &connection);
			
			return ret;
		}
	}
	
	//handling authentication
	password = NULL;
	username = MHD_basic_auth_get_username_password(connection, &password);
	access_fail = ((username == NULL) 				  ||
				   (strcmp(username, USER_NAME) != 0) ||
				   (strcmp(password, USER_PASSWORD) != 0));
	
	if (username != NULL) MHD_free(username);
	if (password != NULL) MHD_free(password);
	
	if (access_fail != NULL) 
	{
		const char* error_str = "<html><body>Access failed</body></html>";
		response = MHD_create_response_from_buffer(strlen(error_str), (void*)error_str, MHD_RESPMEM_PERSISTENT);
		ret = MHD_queue_basic_auth_fail_response(connection, "Main Realm", response);
		
		return MHD_NO;
	}
	else 
	{
		const char* page = "<html><body>Access succeed</body></html>";
		response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_PERSISTENT);
		ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
	}
	
	//response file errors handling
	if (((response_file = fopen(FILE_NAME, "r"))	== NULL) ||
		 (fstat(_fileno(response_file), &file_stat) == C_STD_ERROR))
	{
		const char* error_str = "<html><body>An internal server error has occurred!</body></html>";

		if (response_file != NULL) fclose(response_file);

		response = MHD_create_response_from_buffer(strlen(error_str), (void*)error_str, MHD_RESPMEM_PERSISTENT);

		if (response == NULL) return MHD_NO;

		ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
		MHD_destroy_response(response);

		return MHD_NO;
	}
	
	response = MHD_create_response_from_fd_at_offset64(file_stat.st_size, _fileno(response_file), 0);
	MHD_add_response_header(response, "Content-Type", MIME_TYPE);
	ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
	MHD_destroy_response(response);

	//fclose(response_file); closed when response is destroyed
	return ret;
};

int main(
	int argc, char* argv[])
{
	try 
	{
		const char* connect_info = "host=localhost port=5432 dbname=bank_db user=postgres password=111";
		PGconn* db_connection = PQconnectdb(connect_info);

		//checking connetction to data base
		if (PQstatus(db_connection) != CONNECTION_OK)
		{	
			printf("Bad connection\n");
			getchar();

			return EXIT_FAILURE;
		}
		
		db_request_buffer = (char*)malloc(REQUEST_BUFFER_LENGTH);
		db_request_buffer[0] = NULL;
		header_values_buffer = (char*)malloc(HEADER_VALUES_BUFFER_LENGTH);
		header_values_buffer[0] = NULL;
		
		//creating and starting web-server daemon
		struct MHD_Daemon* daemon;
		daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL, &Answer_To_Connection, NULL, MHD_OPTION_END);
		if (daemon == NULL) return EXIT_FAILURE;
		
		getchar();
		
		MHD_stop_daemon(daemon);
	}
	catch(const std::exception& error)
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
};