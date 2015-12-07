#include "myfs.h"
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <stdarg.h>

using namespace std;

const bool DEBUG = true;
time_t initTime;
FILE * pFile;
void log(const char * s, ...)
{
	va_list vl;
	va_start(vl,s);
	
	if (DEBUG)
	{
		fprintf(pFile, "[%5d s]\t", time(0)-initTime);
		vfprintf(pFile, s, vl);
	}
}

myfs* myfs::_instance = NULL;

struct File {
	struct stat metadata;

	string data;
	map<string, File*> dirents;
};

struct File rootdir;
string rootPath;

myfs* myfs::Instance() {
	if(_instance == NULL) {
		_instance = new myfs();
	}
	return _instance;
}

myfs::myfs() {
	
}

myfs::~myfs() {

}

string myfs::AbsPath(const char *path) {
	return rootPath + path;
}

vector<string> split(const string s, const string pat ) {
  string c;
  vector<string> v;
  int i = 0;
  for (;;) {
    int t = s.find(pat,i);
    int j = ( t == string::npos ) ? s.size() : t;
    string c = s.substr(i,j-i);
    v.push_back(c);
    i = j+pat.size();
    if ( t == string::npos ) return v;
  }
}

File * getFile(vector<string> v) {
	File *f = &rootdir;
	for(int i = 0; i < v.size(); i++)
	{
		if (v[i] == "" || v[i] == ".")
			continue;
		if (f->dirents.count(v[i]))
			f = (f->dirents[v[i]]);
		else
			return NULL;
	}
	return f;
}


void myfs::setRootDir(const char *path) {
	rootPath = path;
	struct stat st = {};

	st.st_dev = 100;
	st.st_ino = 2;
	st.st_mode = 040777;
	st.st_nlink = 1;
	st.st_uid = geteuid();
	st.st_gid = getegid();
	st.st_rdev = 0;
	st.st_size = 0;
	st.st_blksize = 4096;
	st.st_blocks = 0;
	st.st_atime = time(0);
	st.st_mtime = time(0);
	st.st_ctime = time(0);

	rootdir.metadata = st;
	pFile = fopen ("test.txt","w");
}

int myfs::Getattr(const char *path, struct stat *statbuf) {
	File *f = getFile(split(path,"/"));
	if (f == NULL)
	{
		return -ENOENT;
	}
	*statbuf = f->metadata;
	return 0;
}

int myfs::Opendir(const char *path, struct fuse_file_info *fileInfo) {
	log("opendir \"%s, %o\"\n",path, fileInfo->flags);
	if(Access(path, R_OK))
		return -EACCES;	
	return 0;
}

int myfs::Readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
	log("readdir \"%s\"\n",path);

	File *f = getFile(split(path,"/"));
	for (map<string, File*>::iterator it = f->dirents.begin(); it != f->dirents.end(); it++ ) {
		filler(buf, it->first.c_str(), NULL, 0);
	}	
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	
	//filler(buf, "test", NULL, 0);
    
	return 0;
}

int myfs::Mkdir(const char *path, mode_t mode) {
	log("mkdir \"%s, %o\"\n", path, mode);

	vector<string> p = split(path,"/");
	string newName = p.back();
	p.pop_back();

	File *f = getFile(p);

	File *newFile = new File;
	struct stat st = {};

	st.st_dev = 100;
	st.st_ino = 2;
	st.st_mode = 040000 | mode;
	st.st_nlink = 1;
	st.st_uid = geteuid();
	st.st_gid = getegid();
	st.st_rdev = 0;
	st.st_size = 0;
	st.st_blksize = 4096;
	st.st_blocks = 0;
	st.st_atime = time(0);
	st.st_mtime = time(0);
	st.st_ctime = time(0);

	newFile->metadata = st;
	
	f->dirents.insert(pair<string, File*>(newName,newFile));
	return 0;
}

int myfs::Rmdir(const char *path) {
	log("rmdir \"%s\"\n", path);

	vector<string> p = split(path,"/");
	File *f = getFile(p);
	string fileName = p.back();
	p.pop_back();

	File *parent = getFile(p);
	
	parent->dirents.erase(fileName);
	f->metadata.st_nlink--;
	
	return 0;
}

int myfs::Chmod(const char *path, mode_t mode) {
	log("chmod \"%s\", \"%d\"\n", path, mode);
	File *f = getFile(split(path,"/"));
	if (geteuid() != f->metadata.st_uid)
		return -EACCES;
	f->metadata.st_mode = mode;
	f->metadata.st_ctime = time(0);
	return 0;
}

int myfs::Chown(const char *path, uid_t uid, gid_t gid) {
	log("chown \"%s\", \"%d.%d\"\n", path, uid, gid);
	File *f = getFile(split(path,"/"));
	if (geteuid() != f->metadata.st_uid)
		return -EACCES;
	f->metadata.st_uid = uid;
	f->metadata.st_gid = gid;
	f->metadata.st_ctime = time(0);
	return 0;
}

int myfs::Rename(const char *path, const char *newpath) {
	log("rename \"%s to %s\"\n", path, newpath);
	
	vector<string> p = split(path,"/");
	File *oldFile = getFile(p);
	string oldName = p.back();
	p.pop_back();
	File *oldPar = getFile(p);

	vector<string> np = split(newpath,"/");
	string newName = np.back();
	np.pop_back();
	File *newPar = getFile(np);

	return 0;
}
int myfs::Open(const char *path, struct fuse_file_info *fileInfo) {
	log("open \"%s, %o\"\n",path, fileInfo->flags);
	int flag = fileInfo->flags;
	if ((flag & O_ACCMODE) == 0){
		if(Access(path, R_OK))
			return -EACCES;	
	}
	else if ((flag & O_WRONLY) != 0){
		if (Access(path, W_OK))
			return -EACCES;
	}
	else if ((flag & O_RDWR) != 0){
		if (Access(path, W_OK | R_OK))
			return -EACCES;
	}
	return 0;
}

int myfs::Read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
	log("read \"%s, size %d\", offset %d\n", path, size, offset);
	File *f = getFile(split(path,"/"));

	memset(buf, '\0', size);
	strncpy(buf, f->data.substr(offset, size).c_str(), size);
	log("buf: \"%s\"\n", buf);
	return f->data.substr(offset,size).length();
}

int myfs::Write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
	log("write \"%s, %d\"\n", path, size);

	File *f = getFile(split(path,"/"));
	f->data.replace(offset,size, buf);
	log("buf: \"%s\"\n", buf);
	f->metadata.st_size = offset + size;
	return size;
}

int myfs::Readlink(const char *path, char *link, size_t size) {
	log("readlink \"%s\"\n", path);
	
	File *f = getFile(split(path,"/"));

	memset(link, '\0', size);
	strncpy(link, f->data.c_str(), size);

	return 0;
}

int myfs::Mknod(const char *path, mode_t mode, dev_t dev) {
	log("mknod \"%s\"\n", path);

	vector<string> p = split(path,"/");
	string newName = p.back();
	p.pop_back();
	
	File *f = getFile(p);

	File *newFile = new File;
	struct stat st = {};

	st.st_dev = dev;
	st.st_ino = 2;
	st.st_mode = mode;
	st.st_nlink = 1;
	st.st_uid = geteuid();
	st.st_gid = getegid();
	st.st_rdev = 0;
	st.st_size = 0;
	st.st_blksize = 4096;
	st.st_blocks = 0;
	st.st_atime = time(0);
	st.st_mtime = time(0);
	st.st_ctime = time(0);

	newFile->metadata = st;
	
	f->dirents.insert(pair<string, File*>(newName,newFile));
	return 0;
}

int myfs::Unlink(const char *path) {
	log("unlink \"%s\"\n", path);

	vector<string> p = split(path,"/");
	File *f = getFile(p);
	string fileName = p.back();
	p.pop_back();

	File *parent = getFile(p);
	
	parent->dirents.erase(fileName);
	f->metadata.st_nlink--;
	
	return 0;
}

int myfs::Symlink(const char *link, const char *path) {
	log("symlink \"%s, %s\"\n", path, link);

	vector<string> p = split(path,"/");
	string newName = p.back();
	p.pop_back();

	File *f = getFile(p);

	File *linkFile = getFile(split(link,"/"));

	File *newFile = new File;
	struct stat st = linkFile->metadata;

	st.st_mode = 0120000 | (linkFile->metadata.st_mode & 0777);

	newFile->metadata = st;

	newFile->data = link;
	
	f->dirents.insert(pair<string, File*>(newName,newFile));
	return 0;
}

int myfs::Link(const char *path, const char *newpath) {
	log("link \"%s\"\n", path);
	
	vector<string> p = split(path,"/");
	File *oldFile = getFile(p);
	

	vector<string> np = split(newpath,"/");
	string newName = np.back();
	np.pop_back();
	File *newPar = getFile(np);

	newPar->dirents.insert(pair<string, File*>(newName,oldFile));	
	oldFile->metadata.st_nlink++;
	return 0;
}

int myfs::Truncate(const char *path, off_t newSize) {
	log("truncate \"%s\"\n", path);
	return 0;
}

int myfs::Utime(const char *path, struct utimbuf *ubuf) {
	log("utime \"%s\"\n", path);
	
	struct utimbuf t = {};
	t.actime = time(0);
	t.modtime = time(0);
	
	ubuf = &t;

	return 0;
}


int myfs::Access(const char *path, int mask) {
	log("access \"%s, %d\"\n", path, mask);
	File * f = getFile(split(path,"/"));
	
	if(mask == F_OK){
		return f == NULL ? -1 : 0;
	}
	else if (f!=NULL){
		mode_t mode = f->metadata.st_mode;
		uid_t uid = f->metadata.st_uid;
		gid_t gid = f->metadata.st_gid;
		if(mask & X_OK){
			
			if (!(mode & 01) &&
			!((mode & 010) && (gid == getegid())) &&
			!((mode & 0100) && (uid == geteuid())) &&
			(geteuid()!=0))
			{
				return -1;
			}
				
		}
		if(mask & W_OK){
			if (!(mode & 02) &&
			!((mode & 020) && (gid == getegid())) &&
			!((mode & 0200) && (uid == geteuid())))
				return -1;
		}
		if(mask & R_OK){
			if (!(mode & 04) &&
			!((mode & 040) && (gid == getegid())) &&
			!((mode & 0400) && (uid == geteuid())))
				return -1;
		}
		return 0;
	}

	return -1;
}

int myfs::Statfs(const char *path, struct statvfs *statInfo) {
	log("statfs \"%s\"\n", path);
	return 0;
}

int myfs::Flush(const char *path, struct fuse_file_info *fileInfo) {
	log("flush \"%s\"\n", path);
	return 0;
}

int myfs::Release(const char *path, struct fuse_file_info *fileInfo) {
	log("release \"%s\"\n", path);
	return 0;
}

int myfs::Fsync(const char *path, int datasync, struct fuse_file_info *fi) {
	log("fsync \"%s\"\n", path);
	return 0;
}

int myfs::Setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
	return 0;
}

int myfs::Getxattr(const char *path, const char *name, char *value, size_t size) {
	return 0;
}

int myfs::Listxattr(const char *path, char *list, size_t size) {
	return 0;
}

int myfs::Removexattr(const char *path, const char *name) {
	return 0;
}

int myfs::Releasedir(const char *path, struct fuse_file_info *fileInfo) {
	log("releasedir \"%s\"\n", path);
	return 0;
}

int myfs::Fsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
	log("fsyncdir \"%s\"\n", path);
	return 0;
}

int myfs::Init(struct fuse_conn_info *conn) {
	initTime = time(0);
	log("Initializing\n");
	return 0;
}

int myfs::Truncate(const char *path, off_t offset, struct fuse_file_info *fileInfo) {
	log("truncate \"%s\"\n", path);
	return 0;
}
