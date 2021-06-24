#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include "mouse.h"

static Display * display;
static XModifierKeymap * modifiers;


void
mouse_move(int dx, int dy)
{
	if (!display)
		display = XOpenDisplay(0);

	XTestFakeRelativeMotionEvent(display, dx, dy, CurrentTime);
	XFlush(display);

	//XSync(display, False);
}


void
mouse_button(int button, int is_press)
{
	if (!display)
		display = XOpenDisplay(0);

	XTestFakeButtonEvent(display, button, is_press, CurrentTime);
	//XSync(display, False);
	XFlush(display);
	usleep(50000);
}

void
mouse_modifiers(int modmask)
{
	if (!display)
		display = XOpenDisplay(0);
	if (!modifiers)
		modifiers = XGetModifierMapping(display);

	static int last_modmask;

	const int keypermod = modifiers->max_keypermod;

	for(int i = 0 ; i < 7 ; i++)
	{
		const int is_press = (modmask >> i) & 1;
		const int last_press = (last_modmask >> i) & 1;

		if (is_press == last_press)
			continue;

		for (int j = 0 ; j < keypermod ; j++)
		{
			const int keycode = modifiers->modifiermap[i * keypermod +  j];
			if (!keycode)
				continue;
printf("modifier %d: %d %s\n", i, keycode, is_press ? "DOWN" : "UP");
			XTestFakeKeyEvent(display, keycode, is_press, CurrentTime);
			//XSync(display, False);
			XFlush(display);
			usleep(50000);
			break;
		}
	}

	last_modmask = modmask;
	usleep(1000);
}
