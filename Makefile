rebuild:	clean	build
build:
	cc	-Wall	-g	$(arg).c	-o	$(arg)
clean:
	rm	-f	$(arg)
