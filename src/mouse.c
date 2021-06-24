#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <X11/Xlib.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>
#include "mouse.h"


static struct libevdev * dev;
static struct libevdev_uinput * uidev;


void mouse_setup(void)
{
	if (dev)
		return;
	dev = libevdev_new();
	libevdev_set_name(dev, "multitrack");

	libevdev_enable_event_type(dev, EV_REL);
	libevdev_enable_event_code(dev, EV_REL, REL_X, NULL);
	libevdev_enable_event_code(dev, EV_REL, REL_Y, NULL);

	libevdev_enable_event_type(dev, EV_KEY);
	libevdev_enable_event_code(dev, EV_KEY, BTN_LEFT, NULL);
	libevdev_enable_event_code(dev, EV_KEY, BTN_MIDDLE, NULL);
	libevdev_enable_event_code(dev, EV_KEY, BTN_RIGHT, NULL);
	libevdev_enable_event_code(dev, EV_KEY, KEY_LEFTCTRL, NULL);
	libevdev_enable_event_code(dev, EV_KEY, KEY_LEFTSHIFT, NULL);

	int err = libevdev_uinput_create_from_device(
		dev,
		LIBEVDEV_UINPUT_OPEN_MANAGED,
		&uidev
	);

	if (err)
		fprintf(stderr, "libevdev create error %d\n", err);
}



void
mouse_move(int dx, int dy)
{
	mouse_setup();

	libevdev_uinput_write_event(uidev, EV_REL, REL_X, dx);
	libevdev_uinput_write_event(uidev, EV_REL, REL_Y, dy);
	libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);

	//XTestFakeRelativeMotionEvent(display, dx, dy, CurrentTime);
	//XFlush(display);

	//XSync(display, False);
}


void
mouse_button(int button, int is_press)
{
	mouse_setup();

	static const int buttons[] = {
		0,
		BTN_LEFT,
		BTN_MIDDLE,
		BTN_RIGHT,
	};

	libevdev_uinput_write_event(uidev, EV_KEY, buttons[button], is_press);
	libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
	//XTestFakeButtonEvent(display, button, is_press, CurrentTime);
	//XSync(display, False);
	//XFlush(display);
	//usleep(5000);
}

void
mouse_modifiers(int modmask)
{
	mouse_setup();

	static int last_modmask;
	static const int modifiers[] = {
		KEY_LEFTSHIFT,
		KEY_CAPSLOCK,
		KEY_LEFTCTRL,
	};

	for(int i = 0 ; i < 7 ; i++)
	{
		const int is_press = (modmask >> i) & 1;
		const int last_press = (last_modmask >> i) & 1;
		if (is_press == last_press)
			continue;

		printf("modified %d=%d\n", i, is_press);
		libevdev_uinput_write_event(uidev, EV_KEY, modifiers[i], is_press);
		//usleep(5000);
	}

	libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
	last_modmask = modmask;
}
