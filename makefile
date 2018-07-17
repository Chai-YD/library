library:main.c main.h menu.c menu.h
	gcc -g -o $@ $^
PHONE:clear
clear:
	rm -r library
