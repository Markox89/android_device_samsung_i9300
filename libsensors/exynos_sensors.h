/*
 * Copyright (C) 2013 Paul Kocialkowski
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <poll.h>

#include <linux/input.h>

#include <hardware/sensors.h>
#include <hardware/hardware.h>

#ifndef _EXYNOS_SENSORS_H_
#define _EXYNOS_SENSORS_H_

#define EXYNOS_SENSORS_NEEDED_API		(1 << 0)
#define EXYNOS_SENSORS_NEEDED_ORIENTATION	(1 << 1)

struct exynos_sensors_device;

struct exynos_sensors_handlers {
	char *name;
	int handle;

	int (*init)(struct exynos_sensors_handlers *handlers, struct exynos_sensors_device *device);
	int (*deinit)(struct exynos_sensors_handlers *handlers);
	int (*activate)(struct exynos_sensors_handlers *handlers);
	int (*deactivate)(struct exynos_sensors_handlers *handlers);
	int (*set_delay)(struct exynos_sensors_handlers *handlers, long int delay);
	int (*get_data)(struct exynos_sensors_handlers *handlers, struct sensors_event_t *event);

	int activated;
	int needed;
	int poll_fd;

	void *data;
};

struct exynos_sensors_device {
	struct sensors_poll_device_t device;

	struct exynos_sensors_handlers **handlers;
	int handlers_count;

	struct pollfd *poll_fds;
	int poll_fds_count;
};

extern struct exynos_sensors_handlers *exynos_sensors_handlers[];
extern int exynos_sensors_handlers_count;

int exynos_sensors_activate(struct sensors_poll_device_t *dev, int handle, int enabled);
int exynos_sensors_set_delay(struct sensors_poll_device_t *dev, int handle, int64_t ns);
int exynos_sensors_poll(struct sensors_poll_device_t *dev,
	struct sensors_event_t* data, int count);

/*
 * Input
 */

void input_event_set(struct input_event *event, int type, int code, int value);
long int timestamp(struct timeval *time);
long int input_timestamp(struct input_event *event);
int uinput_rel_create(const char *name);
void uinput_destroy(int uinput_fd);
int input_open(char *name);
int sysfs_path_prefix(char *name, char *path_prefix);
int sysfs_value_read(char *path);
int sysfs_value_write(char *path, int value);
int sysfs_string_read(char *path, char *buffer, int length);
int sysfs_string_write(char *path, char *buffer, int length);

/*
 * Sensors
 */

int orientation_fill(struct exynos_sensors_handlers *handlers,
	sensors_vec_t *acceleration, sensors_vec_t *magnetic);

extern struct exynos_sensors_handlers lsm330dlc_acceleration;
extern struct exynos_sensors_handlers akm8975;
extern struct exynos_sensors_handlers orientation;
extern struct exynos_sensors_handlers cm36651_proximity;
extern struct exynos_sensors_handlers cm36651_light;
extern struct exynos_sensors_handlers lsm330dlc_gyroscope;
extern struct exynos_sensors_handlers lps331ap;

#endif
