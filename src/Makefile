all: testmyfs

test: testmyfs

myfs:
	@gcc -g `pkg-config fuse --cflags --libs` myfs.c -o myfs	

testmyfs: myfs mkdirs
	@./myfs rootdir/ mountdir/

bbfs:
	@gcc -g `pkg-config fuse --cflags --libs` bbfs.c -o bbfs	
	
testbbfs: bbfs mkdirs
	@./bbfs rootdir/ mountdir/

mkdirs: cleandir
	
	@mkdir -p mountdir
	@mkdir -p rootdir
	@echo "This file was created and placed in the root directory" > rootdir/file.txt

clean: cleandir
	@rm -f myfs bbfs

cleandir:
	@fusermount -u mountdir/ 2> /dev/null || true
	@rm -rf mountdir/ rootdir/
