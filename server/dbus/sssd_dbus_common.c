#include <sys/time.h>
#include "events.h"
#include "dbus/dbus.h"
#include "util/util.h"

struct timeval _dbus_timeout_get_interval_tv(int interval) {
    struct timeval tv;
    struct timeval rightnow;

    gettimeofday(&rightnow,NULL);

    tv.tv_sec = interval / 1000 + rightnow.tv_sec;
    tv.tv_usec = (interval % 1000) * 1000 + rightnow.tv_usec;
    return tv;
}

/*
 * remove_watch
 * Hook for D-BUS to remove file descriptor-based events
 * from the libevents mainloop
 */
void remove_watch(DBusWatch *watch, void *data) {
    struct fd_event *fde;
    
    DEBUG(2, ("%lX\n", watch));
    fde = talloc_get_type(dbus_watch_get_data(watch), struct fd_event);

    /* Freeing the event object will remove it from the event loop */
    talloc_free(fde);
    dbus_watch_set_data(watch, NULL, NULL);
}


/*
 * remove_timeout
 * Hook for D-BUS to remove time-based events from the mainloop
 */
void remove_timeout(DBusTimeout *timeout, void *data) {
    struct timed_event *te;
    te = talloc_get_type(dbus_timeout_get_data(timeout), struct timed_event);

    /* Freeing the event object will remove it from the event loop */
    talloc_free(te);
    dbus_timeout_set_data(timeout, NULL, NULL);
}
