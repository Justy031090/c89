#ifndef __WD_H__
#define __WD_H__

#include <stddef.h>





/*
This function initializes the necessary resources, creates the 
watchdog process, and begins monitoring. It will send signals 
to the monitored process at the defined interval and check if 
the process is alive based on the threshold.

threshold         The number of signals that must be missed before the watchdog takes action.
interval          The time interval in seconds at which signals will be sent to the monitored process.
argc              The argument count for the monitored process.
argv              The arguments for the monitored process.
RETURN VALUE      Returns SUCCESS (0) if the watchdog started successfully, or FAIL (-1) on error.
 */
int WDStart(size_t threshold, size_t interval, int argc, char **argv);

/*
This function will terminate the watchdog process and free 
all resources that were allocated during the operation of the watchdog.
if failed to free any resources - returns non-zero value;
 */
int WDStop(void);

#endif