CFLAGS = \
	-O3 \
	-W \
	-Wall \
	-I include \
	`pkg-config xorg-server --cflags` \
	`pkg-config mtdev --cflags` \
	`pkg-config libevdev --cflags` \


CC = $(CROSS)gcc

all: multitrack

multitrack_objs = \
	src/capabilities.o \
	src/mouse.o \
	src/gestures.o \
	src/hwstate.o \
	src/mconfig.o \
	src/mtouch.o \
	src/mtstate.o \
	src/trig.o \
	tools/mtrack-test.o \

multitrack: $(multitrack_objs)
	$(CC) \
		$(LDFLAGS) \
		-o $@ \
		$^ \
		`pkg-config xorg-server --libs` \
		`pkg-config mtdev --libs` \
		`pkg-config libevdev --libs` \
		-lXtst \
		-lX11 \
		-lm \

clean:
	$(RM) $(multitrack_objs) core a.out

