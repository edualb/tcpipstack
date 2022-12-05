.PHONY: build-objects test clean print-topology

OBJS=gluethread/gluethread.o \
	graph.o \
	net.o

TEST_FILES_NAME=graph_test \
	net_test

test: build-objects
	for file_name in $(TEST_FILES_NAME); do \
		gcc -c -I . -I gluethread $$file_name.c -o $$file_name.o; \
		gcc $$file_name.o ${OBJS} -o $$file_name; \
		./$$file_name; \
		rm ./$$file_name; \
	done

print-topology: build-objects
	gcc -c -I . -I gluethread topologies.c -o topologies.o
	gcc topologies.o ${OBJS} -o topologies
	./topologies
	rm ./topologies


build-objects:
	gcc -c -I . graph.c -o graph.o
	gcc -c -I . net.c -o net.o
	gcc -c -I gluethread gluethread/gluethread.c -o gluethread/gluethread.o

clean:
	rm *.o
	rm **/*.o