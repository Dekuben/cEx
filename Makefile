CFLAGS=	-Wall	 -g

rebuild:	clean	build
build:
	cc	$(arg).c	-o	$(arg)
clean:
	rm	-f	$(arg)
