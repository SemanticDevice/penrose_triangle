CC=gcc
CFLAGS=$$(pkg-config allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5 --libs --cflags)

penrose_triangle: penrose_triangle.c
	$(CC) -o penrose_triangle penrose_triangle.c -ggdb $(CFLAGS)