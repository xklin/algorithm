#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/string.h>
#include <fnctl.h>
#include <event.h>
#include <stdio.h>
#include <stdlib.h>


#define BACKLOG	32

FILE *LOG_STREAM ;


typedef struct {

	void *date ;
	int len ;

	time_t date_s ;
} time_info_t ;


// Global client fd event_base
struct event_base *g_base ;
// Global listen fd
int g_listen ;



time_info_t *
get_time_info ()
{
	time_t ct ;
	time_info_t *ti ;

	ct = time (NULL) ;
	ti = malloc (sizeof (time_info_t)) ;
	if (NULL == ti) {
		fprintf (LOG_STREAM, "[ERROR] Allocate spaces for 'ti' failed\n") ;
		return NULL ;
	}

	ti->date_s = ct ;
	ti->date = ctime (&ct) ;
	if (NULL == ti->date) {
		fprintf (LOG_STREAM, "[ERROR] ctime failed\n") ;
		free (ti) ;
		return NULL ;
	}
	ti->len = strlen (ti->date) ;

	fprintf (LOG_STREAM, "[NOTICE] Get time info : %s\n", ti->date) ;
	return ti ;
}


int
serv_listen (char *ip, int port)
{
	struct sockaddr_in serv ;
	int opt ;

	serv.sin_family = AF_INET ;
	serv.sin_port = htons (port) ;
	if (-1 == inet_pton (AF_INET, ip, &serv.sin_addr)) {
		fprintf (LOG_STREAM, "[ERROR] inet_pton failed\n") ;
		return -1 ;
	}

	opt = 1 ;
	// Set the socket to addrreuse
	if (-1 == setsockopt (g_listen, SOL_SOCKET, SO_REUSEADDR, &opt,
						sizeof (opt))) {
		fprintf (LOG_STREAM, "[ERROR] Set socket option SO_REUSEADDR"
							"failed\n") ;
		return -1 ;
	}

	// Bind the address to the socket
	if (-1 == bind (g_listen, &serv, sizeof(serv))) {
		fprintf (LOG_STREAM, "[ERROR] Bind the address failed\n") ;
		return -1 ;
	}

	// Listen to the socket
	if (-1 == listen (g_listen, BACKLOG)) {
		fprintf (LOG_STREAM, "[ERROR] Listen the address failed\n") ;
		return -1 ;
	}

	return 0 ;
}


void
on_cli (evutil_socket_t fd, short what, void *arg)
{
	if (NULL == arg) {
		fprintf (LOG_STREAM, "[ALARM] 'on_cli' has not get the"
			" valid argument\n") ;
		exit (1) ;
	}

	if (!(what & EV_WRITE)) {
		// The fd is not ready for writing
		goto _reloop ;
	}

	struct event *self = (struct event *)arg ;
	int ret, towrite ;
	time_info_t *ti ;
	char *pos ;

	if (NULL == (ti = get_time_info ())) {
		fprintf (LOG_STREAM, "[ERROR] Get time info failed\n") ;
		serv_exit () ;
	}

	for (ret=0, pos=ti->date, towrite=ti->len; towrite > 0; ) {

		errno = 0 ;
		ret = send (fd, pos, towrite, 0) ;
#ifdef EWOULDBLOCK
		if (errno == EWOULDBLOCK || errno == EAGAIN)
#else
		if (errno == EAGAIN)
#endif
			break ;

		if (-1 == ret) {
			fprintf (LOG_STREAM, "[ERROR] Sending time information failed\n");
			serv_exit () ;
		}

		towrite -= ret ;
		pos += ret ;
	}

_reloop :
	if (-1 == event_del (self)) {
		fprintf (LOG_STREAM, "[ERROR] event_del in 'on_cli' failed\n") ;
		serv_exit () ;
	}

	if (-1 == event_add (self, NULL)) {

	}
}



void
on_accept (evutil_socket_t fd, short what, void *arg)
{
	if (NULL == arg) {
		fprintf (LOG_STREAM, "[ALARM] 'on_accept' has not get the"
				" valid argument\n") ;
		exit (1) ;
	}

	if (!(what & EV_READ)) {
		// The event is not ready for reading
		goto _reloop ;
	}

	struct sockaddr_in cli ;
	int clifd ;
	struct event *evcli ;
	struct event *self = (struct event *)arg ;

	while (1) {

		errno = 0 ; // Clear the errno
		clifd = accept (fd, &cli, sizeof(cli)) ;
#ifdef EWOULDBLOCK
		if (errno == EWOULDBLOCK || errno == EAGAIN)
#else
		if (errno == EAGAIN)
#endif
			break ;

		if (-1 == clifd) {
			fprintf (LOG_STREAM, "[ERROR] accpet in 'on_accept' failed\n") ;
			serv_exit () ;
		}

		// Bind the fd to a event and add it to the g_base
		evcli = event_new (g_base, clifd, EV_WRITE | EV_ET, on_cli, &cli) ;
		if (NULL == evcli) {
			fprintf (LOG_STREAM, "[ERROR] event_new in 'on_accept' failed\n") ;
			serv_exit () ;
		}

		fprintf (LOG_STREAM, "[NOTICE] Accept a new client\n") ;
		print_client (&cli) ;
	}

_reloop :
	if (-1 == event_del (self)) {
		fprintf (LOG_STREAM, "[ERROR] event_del in 'on_accept' failed\n") ;
		serv_exit () ;
	}

	if (-1 == event_add (self, NULL))  {
		fprintf (LOG_STREAM, "[ERROR] event_add in 'on_accept' failed\n") ;
		serv_exit () ;
	}
}



int
serv_loop ()
{
	int fd ;
	int i ;
	struct event_base *base ;
	struct event *event ;

	base = event_base_new () ;
	if (NULL == base) {
		fprintf (LOG_STREAM, "[ERROR] even_base_new in 'serv_loop' failed\n") ;
		return -1 ;
	}

	event = event_new (base, g_listen, EV_READ | EV_ET | EV_PERSIST,
					on_accept, event_self_cbag ()) ;
	if (NULL == event) {
		fprintf (LOG_STREAM, "[ERROR] event_new in 'serv_loop' failed\n") ;
		event_base_free (base) ;
		return -1 ;
	}

	if (-1 == event_add (event, NULL)) {
		fprintf (LOG_STREAM, "[ERROR] event_add in 'serv_loop' failed\n") ;
		event_base_free (base) ;
		event_free (event) ;
		return -1 ;
	}

	if (-1 == event_base_loop (base, EVLOOP_NO_EXIT_ON_EMPTY)) {
		fprintf (LOG_STREAM, "[ERROR] event_base_loop 'serv_loop' failed\n") ;
		event_base_free (base) ;
		event_free (event) ;
		return -1 ;
	}

	return 0 ;
}


int main ()
{

}
