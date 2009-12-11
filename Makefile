all: msword-cracker

msword-cracker:
	g++ msword-cracker.cpp -o msword-cracker \
		-I /usr/include/wv \
		-I /usr/include/libgsf-1 \
		-I /usr/include/glib-2.0 \
		-I /usr/lib/glib-2.0/include \
		-lwv -ggdb
