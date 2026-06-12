#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>
#include <sys/reboot.h>
#include <linux/input.h>
#include <bcmnvram.h>
#include <shutils.h>
#include <shared.h>
#include <rc.h>

static int running = 1;
static int reset_pressed = 0;
static time_t reset_press_time = 0;
#define RESET_LONG_PRESS_SEC 5

static void inputbtn_exit(int sig)
{
	running = 0;
}

int inputbtn_main(int argc, char *argv[])
{
	int fd;
	struct input_event ev;
	FILE *fp;
	sigset_t sigs_to_catch;

	if ((fp = fopen("/var/run/inputbtn.pid", "w")) != NULL) {
		fprintf(fp, "%d", getpid());
		fclose(fp);
	}

	sigemptyset(&sigs_to_catch);
	sigaddset(&sigs_to_catch, SIGTERM);
	sigaddset(&sigs_to_catch, SIGINT);
	sigprocmask(SIG_UNBLOCK, &sigs_to_catch, NULL);
	signal(SIGTERM, inputbtn_exit);
	signal(SIGINT, inputbtn_exit);

	fd = open("/dev/input/event0", O_RDONLY);
	if (fd < 0) {
		_dprintf("Cannot open /dev/input/event0\n");
		return 1;
	}

	_dprintf("inputbtn started, monitoring WPS and RESET buttons\n");

	while (running) {
		int ret = read(fd, &ev, sizeof(struct input_event));
		if (ret != sizeof(struct input_event))
			continue;

		if (ev.type == EV_KEY) {
			switch (ev.code) {
			case KEY_WPS_BUTTON:
				if (ev.value == 1) {
					_dprintf("WPS button pressed\n");
					start_wps_pbc(0);
				}
				break;
			case KEY_RESTART:
				if (ev.value == 1) {
					_dprintf("RESET button pressed\n");
					reset_pressed = 1;
					reset_press_time = time(NULL);
				} else if (ev.value == 0 && reset_pressed) {
					time_t now = time(NULL);
					time_t press_duration = now - reset_press_time;
					_dprintf("RESET button released, pressed for %d seconds\n", (int)press_duration);
					if (press_duration >= RESET_LONG_PRESS_SEC) {
						_dprintf("Long press detected, restoring factory defaults...\n");
						nvram_set("restore_defaults", "1");
						nvram_commit();
#ifdef RTCONFIG_QCA
						eval("mtd-erase", "-d", "nvram");
#else
						ResetDefault();
#endif
						sync();
						reboot(RB_AUTOBOOT);
					}
					reset_pressed = 0;
				}
				break;
			}
		}
	}

	close(fd);
	remove("/var/run/inputbtn.pid");
	return 0;
}

int start_inputbtn(void)
{
	pid_t pid;

	if (f_exists("/var/run/inputbtn.pid")) {
		_dprintf("inputbtn is already running\n");
		return 0;
	}

	pid = fork();
	if (pid < 0) {
		_dprintf("fork failed\n");
		return -1;
	}

	if (pid == 0) {
		inputbtn_main(0, NULL);
		exit(0);
	}

	return 0;
}