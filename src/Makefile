all: testmyfs

test: testmyfs

myfs:
	@gcc -g `pkg-config fuse --cflags --libs` myfs.c -o myfs	

testmyfs: cleandir cleanmyfs myfs mkdirs
	@./myfs rootdir/ mountdir/

bbfs: 
	@gcc -g `pkg-config fuse --cflags --libs` bbfs.c -o bbfs	
	
testbbfs: cleandir cleanbbfs bbfs mkdirs
	@./bbfs rootdir/ mountdir/

mkdirs: 
	@mkdir -p mountdir
	@mkdir -p rootdir
	@echo "This file was created and placed in the root directory" > rootdir/file.txt

clean: cleandir cleanmyfs cleanbbfs

cleanmyfs:
	@rm -f myfs

cleanbbfs:
	@rm -f bbfs

cleandir:
	@fusermount -u mountdir/ 2> /dev/null || true
	@rm -rf mountdir/ rootdir/
