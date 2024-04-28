#include <iostream>
#include <signal.h>
#include <chrono>
#include <thread>
#include <microhttpd.h>

extern "C"{
#include "ev3c.h"
}

volatile sig_atomic_t stop = 0;

void inthand(int signum)
{
	stop = 1;
	std::cout << std::endl;
}

char PAGE[] = "<html><head><title>hello world</title></head><body>hello world</body></html>";

static int
ahc_echo(void *cls,
	struct MHD_Connection *connection,
	const char *url,
	const char *method,
	const char *version,
	const char *upload_data,
	size_t *upload_data_size,
	void **ptr
)
{
	static int aptr;
	const char *me = static_cast<const char*>(cls);
	struct MHD_Response *response;
	int ret;

	if (0 != strcmp (method, "GET"))
		return MHD_NO;              /* unexpected method */
	if (&aptr != *ptr)
	{
		/* do never respond on first call */
		*ptr = &aptr;
		return MHD_YES;
	}
	*ptr = NULL;                  /* reset when done */
	response = MHD_create_response_from_buffer (strlen (me),
	(void *) me,
	MHD_RESPMEM_PERSISTENT);
	ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
	MHD_destroy_response (response);
	return ret;
}

int main(int argc, char *const *argv)
{
	std::cout << "Running s3nsor..." << std::endl;

	//~ ev3_init_lcd();
	ev3_init_button();

	struct MHD_Daemon *d;

	if (argc != 2)
	{
		printf ("%s PORT\n", argv[0]);
		return 1;
	}

	d = MHD_start_daemon (
		MHD_USE_SELECT_INTERNALLY,
		atoi (argv[1]),
		NULL, NULL, &ahc_echo, PAGE,
		MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int) 120,
		MHD_OPTION_END
	);
	if (d == NULL)
		return 1;

	while ( !stop )
	{
		if ( ev3_button_pressed(BUTTON_BACK) )
			break;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	MHD_stop_daemon(d);

	return 0;
}
