CFLAGS=	-Wall	 -g

all:	clean	build
build:
	cc	$(arg).c	-o	$(arg)
clean:
	rm	-f	$(arg)
