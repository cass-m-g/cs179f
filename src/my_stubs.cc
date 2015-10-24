#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
// #include <fuse.h> I don’t currently have this file and don’t need it.
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

// Mine
#include <fs.h>
#include <sys/stat.h> // this has our official definition of stat
#include <dirent.h> // this has our official definition of dirent
#include <errno.h>
#include <vector>
#include <string>
#include <cassert>
#include <sstream>
#include <map>

using namespace std;


struct inode {
    // we need to add the rest of what we need.
    off_t i_size;
};

struct inode ilist[300]; // a dummy ilist.


inline // a simple utility for splitting strings at a find−pattern.
vector<string>
split(const string s, const string pat ) {
    string c;
    vector<string> v;
    int i = 0;
    for (;;) {
        int t = s.find(pat,i);
        int j = ( t == string::npos ) ? s.size() : t;
        string c = s.substr(i,j−i);
        v.push_back(c);
        i = j+pat.size();
        if ( t == string::npos ) return v;
    }
}


class my_DIR { // This is my version of DIR
    public:
    dirent* base; // not a pointer, rather an off_t
    dirent* offset;
    ino_t fh;
    dirent* max_offset; // we need to initialize this.
    my_DIR( ino_t x )
    : fh(x), max_offset((dirent*)ilist[fh].i_size)
    {}
};


int ok = 0;
int err = −1;

// called at line #95 of bbfs.c
// for details see: http://manpages.ubuntu.com/manpages/hardy/man2/stat.2.html
int my_lstat( const char* path, struct stat *statbuf ) {
    // ino_t fh = find_inode(path);
    // collect attributes from ilist[fh] load into struct stat.
    return err;
}

// called at line #125 of bbfs.c
int my_readlink( const char *path, char *link, size_t size ) {
    return err;
}

// called at line #168 of bbfs.c. See line #151.
int my_mknod( const char *path, mode_t mode, dev_t dev ) {
    // returns error status only. To get the handle for the inode
    // simply search to the dirent via path.
    return err;
}

// called at line #186 of bbfs.c
int my_mkdir( const char *path, mode_t mode ) {
    return err;
}

// called at line #203 of bbfs.c
int my_unlink( const char *path ) {
    return err;
}

// called at line #220 of bbfs.c
int my_rmdir( const char *path ) {
    return err;
}

// called at line #241 of bbfs.c
int my_symlink(const char *path, const char *link) {
    return err;
}

// called at line #261 of bbfs.c
int my_rename( const char *path, const char *newpath ) {
    return err;
}

// called at line #279 of bbfs.c
int my_link(const char *path, const char *newpath) {
    return err;
}

// called at line #296 of bbfs.c
int my_chmod(const char *path, mode_t mode) {
    return err;
}

// called at line #314 of bbfs.c
int my_chown(const char *path, uid_t uid, gid_t gid) {
    return err;
}

// called at line #331 of bbfs.c
int my_truncate(const char *path, off_t newsize) {
    return err;
}
// called at line #349 of bbfs.c
int my_utime(const char *path, struct utimbuf *ubuf) {
    return err;
}

// called at line #376 of bbfs.c. Returns file handle not fd
int open( const char *path, int flags ) {
    // Write a function called "lookup" that takes a name and the handle
    // of a directory and returns the handle part of the entry having that
    // name.
    // Assume that path is a full path. Split path. Starting with the
    // handle of the root directory, look up each segment in the directory
    // whose handle you currently have. F
    // directory, lookup each segment.
    // Use a linear search that consists of successive calls to readdir to
    // find the corresponding directory entry, which has type struct dirent.
    // Return its d_fileno, or −1 if error.
    return err;
}

// called at line #411 of bbfs.c
int my_pread( int fh, char *buf, size_t size, off_t offset ) {
    return err;
}

// called at line #439 of bbfs.c
int my_pwrite( int fh, char *buf, size_t size, off_t offset ) {
    return err;
}

// called at line #463 of bbfs.c
int my_statvfs(const char *fpath, struct statvfs *statv) {
    return err;
}

// called at line #530 of bbfs.c
int my_close( ino_t fh ) {
    return err;
}

// called at line #553 of bbfs.c
int my_fdatasync( ino_t fh ) {
    return err;
}

// called at line #556 of bbfs.c
int my_fsync( ino_t fh ) {
    return err;
}

// called at line #575 of bbfs.c
int my_lsetxattr( const char *fpath, const char *name, const char *value, size_t size, int flags ) {
    return err;
}

// called at line #592 of bbfs.c
int my_lgetxattr( const char *fpath, const char *name, char *value, size_t size,int flags ) {
    return err;
}
// called at line #613 of bbfs.c
int my_llistxattr( const char *path, char *list, size_t size ) {
    return err;
}

// called at line #634 of bbfs.c
int my_lremovexattr( const char *path, const char *name ) {
    return err;
}


// In the next three functions, we need to create a my_DIR* from a file
// handle, i.e., an (ino_t) fh. So, how to do that?

// It doesn’t say this anywhere that I can find, but a my_DIR has to be
// similar to an open file. However, rather than creating a stream of
// bytes (chars) from the file, it creates a stream of directory
// entries. The key difference is how much you increment the offset
// counter each time you move to the next item.

// Note that at line #742 of bbfs.c, Professor Pfeiffer converts a
// file handle into a my_DIR via "(uintptr_t) fh." But, his file handles
// are indices of byte streams, while our are the addresses of inodes.

// I recommend that we simply maintain the counter as an
// directory−entry index and increment it by one each time. Then
// multiply it by the size of a directory entry to get the offset of
// the directory entry that it refers to within the directory. To get
// the address of that directory entry, we simply add the address of
// the corresponding block of the directory.

#define my_DIR_PTR (uniptr_t)

// // called at line #659 of bbfs.c
// my_DIR* my_opendir( const char* fpath ) {
    // return err;
// }

// // called at line #707 of bbfs.c
// int my_readdir( my_DIR* dp ) {
    // return err;
// }

// // called at line #742 of bbfs.c
// int my_closedir( my_DIR* dp ) {
    // return err;
// }

// called at line #826 of bbfs.c
int my_access( const char *fpath, int mask ) {
    return err;
}

// called at line #856 of bbfs.c
int my_creat( const char *fpath, mode_t mode ) {
    return err;
}

// called at line #887 of bbfs.c
int my_ftruncate( ino_t fh, off_t offset ) {
    return err;
}

// called at line #921 of bbfs.c
// for details see: http://manpages.ubuntu.com/manpages/hardy/man2/stat.2.html
int my_fstat( ino_t fh, struct stat* statbuf ) {
    return err;
}

// called at line #1015 of bbfs.c
char* my_realpath( const char* path, char* resolved_path ) {
    assert( resolved_path != NULL );
    return 0;
}


// Here are my helper functions ==================================

// In the code below opendir is defined in terms of find_inode,
// which is defined in terms of lookup,
// which is defined in terms of opendir and readdir.
// The header file takes care of that circularity.

typedef char block[4096];

block* blocks[15];

my_DIR* fhopendir( ino_t fh ) {
    // if ( fh is not the handle of a directory ) return not−a−directory error;
    return new my_DIR( fh );
}

dirent* readdir( my_DIR* dirp ) {
    
    off_t tmp = (dirp−>offset)−>d_reclen;
    dirp−>offset += tmp;
    return ( dirp−>offset < dirp−>max_offset) ? dirp−>offset : 0 ;
}


int closedir( my_DIR* dirp ) {
    delete dirp;
}


ino_t lookup( string name, ino_t fh ) {
    // finds and returns ino_t of file whose name is name in directory fh.
    // This function will be used by open() and opendir().
    my_DIR* dirp = fhopendir( fh ); // fhopendir checks if fh is handle of a dir.
    if ( ! dirp ) return err; // cannot_open_error
    while ( dirent* dp = readdir(dirp) ) {
        string s = dp−>d_name; // converts C string to C++ string
        if ( s == name ) {
            closedir(dirp);
            if ( dp−>d_type != DT_REG && dp−>d_type != DT_DIR ) {
                return err; // wrong−file−type error
                // later we may add more types
                } else {
                return dp−>d_fileno;
            }
        }
    }
    closedir(dirp); // close my_DIR asap, to reset internal data
    return err; // name−not−found
}


ino_t find_inode( string fullpath ) {
    // fullpath must starts at the root. // See how Pfeiffer does it.
    ino_t fh = 2; // 2 is the i_no of the root directory.
    vector<string> v = split( fullpath, "/" );
    for ( int i = 1; i != v.size(); ++ i ) { // omit the initial null segment
        fh = lookup( v[i], fh );
    }
}

struct inode * find_inode( ino_t ) {
    // the actual code for this will depend on how you implement this map
    return 0;
}

my_DIR* opendir( char* fullpath ) {
    return fhopendir( find_inode( fullpath ) );
}
// ===================== a crude c++ approach ======================

class file {
    public:
    inode ino;
};

class regfile : file {
    string content;
};

class openfile : regfile {
    stringstream(content);
};

class directory : file {
    public:
    map<string,file> themap; // a balanced binary tree.
};

class my_dir { // C++ verion of DIR, i.e., an opened directory
    directory d;
    map<string,file>::iterator it; // readdir must post−increment it.
    my_dir( directory d )
    : d(d)
    {
        it = d.themap.begin();
    }
};


template< typename T1, typename T2>
string pickle(map<T1,T2> m) {
    typename map<T1,T2>::iterator it;
    string s;
    stringstream ss(s);
    for ( it = m.begin(); it != m.end(); ++it ) {
        // This requires some kind of separation/terminaton symbol at the end of each.
        ss << it.first;
        ss << it.second;
    }
}
