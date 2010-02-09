#include <libalarm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BASEDIR "/home/user/.apt-howdy"
#define CACHEDIR BASEDIR "/cache"
#define LASTUPDATE CACHEDIR "/last-apt-update"
#define ALARMID CACHEDIR "/alarm-id"
#define APT_HOWDY_CHECK "/usr/lib/apt-howdy-check"
#define APT_HOWDY "/usr/lib/apt-howdy"
#define INTERVAL 60

static void
register_alarm (void)
{
	alarm_event_t *new_event = NULL;
	alarm_action_t *act = NULL;
	cookie_t result_cookie;
	int fd;

	new_event = alarm_event_create ();
	alarm_event_set_alarm_appid (new_event, "apt-howdy-check");
	new_event->recur_secs = INTERVAL*60;
	new_event->recur_count = -1;
	new_event->alarm_time = (time_t) time (NULL) + INTERVAL*60;
	act = alarm_event_add_actions (new_event, 1);
	act->flags = ALARM_ACTION_WHEN_TRIGGERED | ALARM_ACTION_TYPE_EXEC;
	alarm_action_set_exec_command (act, APT_HOWDY_CHECK);
	result_cookie = alarmd_event_add (new_event);

	fd = open (ALARMID, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd < 0) return;
	write (fd, &result_cookie, sizeof (result_cookie));
	close (fd);
}

static void
unregister_alarm (void)
{
	int fd;
	cookie_t cookie;

	fd = open (ALARMID, O_RDONLY);
	if (fd < 0) return;
	read (fd, &cookie, sizeof (cookie));
	close (fd);
	unlink (ALARMID);
	alarmd_event_del(cookie);
}

static int
apt_was_updated (void)
{
	int fd;
	struct stat stats;
	time_t old_time;
	
	stat ("/var/lib/apt/lists", &stats);

	fd = open (LASTUPDATE, O_RDWR);
	if (fd < 0) {
		fd = open (LASTUPDATE, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fd < 0) return 1;
		write (fd, &stats.st_mtime, sizeof (stats.st_mtime));
		close (fd);
		return 1;
	}

	read (fd, &old_time, sizeof (old_time));
	if (old_time != stats.st_mtime) {
		lseek (fd, 0, SEEK_SET);
		write (fd, &stats.st_mtime, sizeof (stats.st_mtime));
		close (fd);
		return 1;
	} else {
		close (fd);
		return 0;
	}
}

int main (int argc, char *argv[])
{
	if (argc > 1) {
		if (strcasecmp (argv[1], "register") == 0) {
			unregister_alarm ();
			register_alarm ();
		} else if (strcasecmp (argv[1], "unregister") == 0) {
			unregister_alarm ();
		}
	} else if (apt_was_updated ()) {
		system (APT_HOWDY);	
	}

	return 0;
}

