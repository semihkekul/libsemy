HEADERS = tiny_obj_loader.h
OBJECTS = scene.o tiny_obj_loader.o
default: scene

%.o: %.c $(HEADERS)
	g++ -c $< -o $@  -g

scene: $(OBJECTS)
	g++ $(OBJECTS) -o $@ -lglut -lGL

clean: 
	-rm -f $(OBJECTS)
	-rm -f scene
