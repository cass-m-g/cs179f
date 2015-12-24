# Linux FUSE Wrapper File System


### Contributors:

| Name | Phone | Email |
| ---- | ----- | ----- |
| Cassandra Garner | (909) 908-7997 | cgarn002@ucr.edu |
| Thomas Liu | (408) 891-5702 | tliu020@ucr.edu |
| Itsuki David Fukada | (949) 413-5164 | ifuka001@ucr.edu |
| Christopher Hernandez | (818) 428-0533 | chern030@ucr.edu |
| Kevin Chan | (510) 648-5205 | kchan039@ucr.edu|

### Synopsis:

The linux FUSE wrapper file system is a file system that is built on the prototyping environment of FUSE. The file system is a simplified version of a linux file system that uses a tree based data structure as its directories. The directories are stored in a directory entry called a “dirent”, which is a map of string file names to the address of a “File”. A “File” is a structure that holds all of the file/directory data. It contains three fields: “metadata” which is a stat block, “data” which is a vector of strings, and “dirent” which is a map. There is no difference in the data structure of a file and a directory. This data structure is used as the wrapper on FUSE. FUSE allowed us to easily handle linux bash commands as if the functions we wrote were a part of linux standard libraries, thus we were able to test the file system using the standard linux bash commands.

### Running the file:

```
$ cd src
$ make
```

### Overview:

Running make will compile `myfs.c`.

It will create a folder `rootDir` and `mountDir`.

It will then mount `rootDir` to `mountDir` using the commands written in `myfs.c`


IMAGE: http://gemsres.com/story/aug05/117909/linux-carrier-fig1.gif
