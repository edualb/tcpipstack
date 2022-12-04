.PHONY: build-objects test

OBJS=gluethread/gluethread.o \
	graph.o

test: build-objects
	gcc -c -I . -I gluethread graph_test.c -o graph_test.o
	gcc graph_test.o ${OBJS} -o graph_test
	./graph_test
	rm ./graph_test

build-objects:
	gcc -c -I . graph.c -o graph.o
	gcc -c -I gluethread gluethread/gluethread.c -o gluethread/gluethread.o

clean:
	rm *.o
	rm **/*.o