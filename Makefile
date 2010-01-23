CC = g++
CXXFLAGS = -Wall
PROG = dimension
LIBS = -lglfw -lGLU -lX11 -lpthread -lm 
#-lXrandr -lGL
SRCS = dimension.cpp functions.cpp main_objects.cpp terrain.cpp vec3f.cpp tree.cpp


all: $(PROG)

$(PROG):	$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

clean:
	rm -f $(PROG)
