CC=gcc
LINK_OPTIONS=-shared -o $(LIB_NAME)
OBJECT_OPTIONS=-fPIC -o $@

LIB_NAME=libso_stdio.so
SOURCE=so_stdio.c
COMPILE_LOG=compile.log
CHECKER_DIR=./checker-lin

build: so_stdio.o
	$(CC) $(LINK_OPTIONS) $^ 2>> $(COMPILE_LOG)
	rm *.o
	if [ -d $(CHECKER_DIR) ]; then mv $(LIB_NAME) $(CHECKER_DIR); fi

so_stdio.o:
	$(CC) $(OBJECT_OPTIONS) -c $(SOURCE) 2> $(COMPILE_LOG)

clean:
	if [ -f $(LIB_NAME) ]; then rm $(LIB_NAME); fi
	if [ -f $(COMPILE_LOG) ]; then rm $(COMPILE_LOG); fi
	if [ -f *.o ]; then rm *.o; fi
