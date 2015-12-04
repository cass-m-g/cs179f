
CFLAGS = -DFUSE_USE_VERSION=26 `pkg-config fuse --cflags`
LINKFLAGS = `pkg-config fuse --libs`

all: test

clean: cleandir
	@rm -rf bin obj
	@cat test.txt
	@> test.txt

bin: 
	@mkdir -p bin

bin/myfs: bin obj/myfs.o obj/wrap.o obj/main.o
	@g++ -g $(LINKFLAGS) -o bin/myfs obj/*

obj:
	@mkdir -p obj

obj/myfs.o: obj myfs.cc myfs.h
	@g++ -g $(CFLAGS) -c myfs.cc -o obj/myfs.o

obj/main.o: obj main.c wrap.h
	@gcc -g $(CFLAGS) -c main.c -o obj/main.o

obj/wrap.o: obj wrap.cc wrap.h myfs.h
	@g++ -g $(CFLAGS) -c wrap.cc -o obj/wrap.o

test: clean mkdirs bin/myfs
	@bin/myfs rootdir/ mountdir/

mkdirs: 
	@mkdir -p mountdir
	@mkdir -p rootdir
	@echo "This file was created and placed in the root directory" > rootdir/file.txt

cleandir:
	@fusermount -u mountdir/ 2> /dev/null || true
	@rm -rf mountdir/ rootdir/
