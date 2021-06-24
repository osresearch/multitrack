#ifndef _multitrack_mouse_h_
#define _multitrack_mouse_h_

/*
 * Generate a relative move command by querying the current mouse
 * coordinates and then sending a motion event.
 */
extern void
mouse_move(int dx, int dy);

/*
 * Press or release a mouse button.
 */
extern void
mouse_button(int button, int is_press);


/*
 * Press or release the bit mask of modifiers, as defined in
 * <X11/X.h>
#define ShiftMask               (1<<0)
#define LockMask                (1<<1)
#define ControlMask             (1<<2)
#define Mod1Mask                (1<<3)
#define Mod2Mask                (1<<4)
#define Mod3Mask                (1<<5)
#define Mod4Mask                (1<<6)
#define Mod5Mask                (1<<7)
 */
extern void
mouse_modifiers(int modmask);

#endif
