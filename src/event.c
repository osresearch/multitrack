#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

static Display * display;
static XModifierKeymap * modifiers;


/*
 * Generate a relative move command by querying the current mouse
 * coordinates and then sending a motion event.
 */
void
mouse_move(int dx, int dy)
{
	if (!display)
		display = XOpenDisplay(0);

	XTestFakeRelativeMotionEvent(display, dx, dy, CurrentTime);
	XFlush(display);
}


void
mouse_button(int button, int is_press)
{
	if (!display)
		display = XOpenDisplay(0);

	XTestFakeButtonEvent(display, button, is_press, CurrentTime);
	XFlush(display);
}

void
mouse_modifiers(int modmask)
{
	if (!display)
		display = XOpenDisplay(0);
	if (!modifiers)
		modifiers = XGetModifierMapping(display);

	const int keypermod = modifiers->max_keypermod;

	for(int i = 0 ; i < 7 ; i++)
	{
		const int is_press = (modmask >> i) & 1;

		for (int j = 0 ; j < keypermod ; j++)
		{
			const int keycode = modifiers->modifiermap[i * keypermod +  j];
			if (!keycode)
				continue;
			XTestFakeKeyEvent(display, keycode, is_press, CurrentTime);
			XSync(display, False);
			break;
		}
	}
}

/*
int main(int argc, char * argv[])
{
	Display *display = XOpenDisplay(0);
	if(display == NULL)
		return -1;


	for(int i = 0 ; i < 10 ; i++)
	{
		printf("%d sending\n", i);
		mouse_move(display, 20 * sin(i), 20 * cos(i), None, None);
		sleep(1);
	}

	XCloseDisplay(display);
	return 0;
}
*/
