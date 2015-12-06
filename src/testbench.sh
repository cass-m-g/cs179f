#!/bin/bash
trap 'echo "vagrant@localhost \$ $BASH_COMMAND"' DEBUG

make
cd mountdir/
echo abc > file
echo 1234565 > file
cat file
echo abc >> file
cat file
mkdir dir
ln file dir/newFile
ls -al
ls -al dir/
cat dir/newFile
echo rawr >> dir/newFile
cat file
rm file
ls -al
ls -al dir/
echo abc > file2
ls -al
mv file2 file3
ls -al
cat file3
chmod 777 file3
ls -al
chown root.root file3
ls -al
cd ..
make clean
