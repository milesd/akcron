#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


/* If the location of the akcron perl script hasn't been defined, then do so. */

#ifndef AKCRON_SCRIPT
#define AKCRON_SCRIPT "/usr/bin/akcron"
#endif

#ifndef PAGSH
#define PAGSH "/usr/bin/pagsh.krb"
#endif

char * make_message(const char *fmt, ...);

int main(int argc, const char* argv[]) {
	char* exec_string;
	int uid = getuid();
	int euid = geteuid();

	if(argc < 2) {
		return(-1);
	}

	if(strcmp("",argv[1])==0) {
		return(-2);
	}

	/* Well, this is just asking for a buffer overflow exploit...
	sprintf(exec_string,"%s -subshell %d -c '%s'", AKCRON_SCRIPT, uid, argv[1]);
	*/

	/* Better. Allocate what's needed. */
	exec_string = make_message("%s -subshell %d -c '%s'", AKCRON_SCRIPT, uid, argv[1]);

    /*
	printf("My uid is %d \n", uid);
	printf("About to setuid", euid);
    */

	setuid(0);
	seteuid(uid);

    /*
	printf("****************************************************\n");
	printf("My uid appears to be %d \n", uid);
	printf("My euid appears to be %d \n", euid);
	printf("Going to run %s\n", exec_string);
	printf("Length of exec string is  %d\n", strnlen(exec_string));
	printf("****************************************************\n");

    */
	/* Exec the akcron perl script, passing it the real UID. */
	execl(PAGSH,PAGSH,"-c",exec_string,NULL);

	// How do we get to return? We shouldn't.
	perror("can't execl");
	exit(-1);
}


char * make_message(const char *fmt, ...) {
	/* Guess we need no more than 100 bytes. */
	int n, size = 100;
	char *p;
	va_list ap;
	if ((p = malloc (size)) == NULL)
		return NULL;
	while (1) {
		/* Try to print in the allocated space. */
		va_start(ap, fmt);
		n = vsnprintf (p, size, fmt, ap);
		va_end(ap);
		/* If that worked, return the string. */
		if (n > -1 && n < size)
			return p;
		/* Else try again with more space. */
		if (n > -1)    /* glibc 2.1 */
			size = n+1; /* precisely what is needed */
		else           /* glibc 2.0 */
			size *= 2;  /* twice the old size */
		if ((p = realloc (p, size)) == NULL)
		return NULL;
	}
}

