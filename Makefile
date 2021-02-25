all: msword-cracker

msword-cracker:
	g++ msword-cracker.cpp -o msword-cracker \
		-I /usr/include/wv \
		-I /usr/include/libgsf-1 \
		-I /usr/include/glib-2.0 \
		-I /usr/lib/x86_64-linux-gnu/glib-2.0/include/ \
		-lwv -lgsf-1 $(pkg-config --libs glib-2.0) -std=c++03 -O3
