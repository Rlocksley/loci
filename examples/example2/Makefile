CFLAGS = -std=c11 -O3
LDFLAGS = -lm  -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi 
OBJFILES = ../../src/core/flecs/flecs.o
FILES = main.c

LOCI:
	gcc $(CFLAGS) -o loci_app $(FILES) $(OBJFILES) $(LDFLAGS)

.PHONY: compile_loci_app

compile_loci_app: LOCI