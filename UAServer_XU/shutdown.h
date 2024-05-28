
#ifndef __SHUTDOWN_H__
#define __SHUTDOWN_H__


/* Call this on startup. */
void RegisterSignalHandler();

/* Use this to check if the shutdown flag is set. */
unsigned int ShutDownFlag();

/* Use this to get the application path created with new. Delete returned char array if not NULL. */
char* getAppPath();

#endif
