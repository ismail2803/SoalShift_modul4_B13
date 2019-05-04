#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>

static const char *dirpath = "/home/ismail/shift4";

char* Decrypt(char dec[1000]){
    char caesar[1000] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
    int z, i, j;

    for (i=0; dec[i]!='\0'; i++){
        for (j=0; j<strlen(caesar); j++)
        {
            if(dec[i] == caesar[j]){
                break;
            }
        }
        z = (j-17)%94;
        if(z < 0){
            z = z + strlen(caesar);
        }
        if(dec[i] != '/'){
            dec[i] = caesar[z];
        }else{
            dec[i] = '/';
        }
    }
    return dec;
}

char* Encrypt(char enc[1000]){
    char caesar[1000] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
    int i, j;

    for (i=0; enc[i]!='\0'; i++){
        for (j=0; j<strlen(caesar); j++)
        {
            if(enc[i] == caesar[j]){
                break;
            }
        }
        if(enc[i] != '/'){
            enc[i] = caesar[(j+17)%94];
        }else{
            enc[i] = '/';
        }
    }
    return enc;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
	char fpath[1000], fname[1000];

    if(strcmp(path, ".")!=0 && strcmp(path, "..")!=0){
        memset(fname, '\0', sizeof(fname));
        strcpy(fname, path);
        Encrypt(fname);
        sprintf(fpath, "%s%s", dirpath, fname);
    }
    else{
	    sprintf(fpath,"%s%s",dirpath,path);
    }
	
    res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000], fname[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else{
        memset(fname, '\0', sizeof(fname));
        strcpy(fname, path);
        Encrypt(fname);
        sprintf(fpath, "%s%s", dirpath, fname);
    }
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
        char fstat[1000], error[1000], file[1000];

		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

        memset(fstat, '\0', sizeof(fstat));
        sprintf(fstat, "%s/%s", fpath, de->d_name);

        stat(fstat, &st);
        struct passwd *pw = getpwuid(st.st_uid);
        struct group  *gr = getgrgid(st.st_gid);

        int mod = st.st_mode & S_IRWXO;
 
        if(mod < 4 && (strcmp(pw->pw_name, "chipset") == 0 || strcmp(pw->pw_name, "ic_controller") == 0) && strcmp(gr->gr_name, "rusak") == 0){
            memset(error, '\0', sizeof(error));
            sprintf(error, "%s %d %d %s", de->d_name, gr->gr_gid, pw->pw_uid, ctime(&st.st_atime));

            memset(file, '\0', sizeof(file));
            sprintf(file, "%s/V[EOr[c[Y`HDH", dirpath);
            FILE *fp = fopen(file, "a+");
            if(fp){
                fputs(error, fp);
            }
            fclose(fp);
            remove(fstat);
        }

        if(strcmp(de->d_name, ".")!=0 && strcmp(de->d_name, "..")!=0){
            Decrypt(de->d_name);
        }

        res = (filler(buf, de->d_name, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
    char fname[1000], fpath[1000];
    char ext[100] = {0};

    memset(fname, '\0', sizeof(fname));

    for(int i=strlen(path); i>=0; i--){
        if(path[i]=='/'){
            int j=i-8;
            int z=0;
            for(int k=j; k<(j+8); k++){
                ext[z] = path[k];
                z++;
            }
            break;
        }
    }

    if(strcmp(ext, "YOUTUBER") == 0){
        strcpy(fname, path);
        Encrypt(fname);
        sprintf(fpath, "%s%s", dirpath, fname);
        res = mkdir(fpath, 0750);
    }else{
        strcpy(fname, path);
        Encrypt(fname);
        sprintf(fpath, "%s%s", dirpath, fname);
        res = mkdir(fpath, mode);
    }

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
	int res;
    char fname[1000], fpath[1000];

    memset(fname, '\0', sizeof(fname));
    strcpy(fname, path);
    Encrypt(fname);
    sprintf(fpath, "%s%s", dirpath, fname);

    pid_t child_id;
    child_id = fork();

    if(child_id == 0){
        char waktu[1000], namafix[1000], fpathl[1000], dirpathl[1000], fnamel[1000], file[1000], namafile[1000], Backup[1000], dirpathfolder[1000], dirpathnama[1000], dirpathzip[1000];

        char cut[1000] = {0};
        char *fuses;
        time_t rawtime;
        struct tm *info;
        
        time(&rawtime);
        info = localtime(&rawtime);
        memset(waktu, '\0', sizeof(waktu));
        sprintf(waktu, "%d:%02d:%02d_%02d:%02d:%02d", info->tm_year+1900, info->tm_mon+1, info->tm_mday, info->tm_hour, info->tm_min, info->tm_sec);

        memset(namafix, '\0', sizeof(namafix));
        memset(fpathl, '\0', sizeof(fpathl));
        memset(fname, '\0', sizeof(fname));
        memset(file, '\0', sizeof(file));
        memset(namafile, '\0', sizeof(namafile));
        memset(dirpathl, '\0', sizeof(dirpathl));
        memset(Backup, '\0', sizeof(Backup));
        memset(dirpathfolder, '\0', sizeof(dirpathfolder));
        memset(dirpathnama, '\0', sizeof(dirpathnama));
        memset(dirpathzip, '\0', sizeof(dirpathzip));

        strcpy(fname, path);
        strcpy(dirpathl, dirpath);
        strcpy(dirpathfolder, dirpath); 
        strcpy(dirpathnama, dirpath);
        strcpy(dirpathzip, dirpath);

        fuses = strrchr(fname, '/');
        strcpy(file, fuses);

        for(int i=strlen(fname); i>0; i--){
            if(fname[i]=='/'){
                int j=i;
                for(int z=0; z<j; z++){
                    cut[z] = fname[z];
                }
                break;
            }
        }
        if(cut == NULL)
            strcpy(cut, "/");
    
        strcpy(namafile, path);
        Encrypt(namafile);
        strcat(dirpathnama, namafile);
        // printf("nama file asli %s\n", dirpathnama);
        
        sprintf(Backup, "%s/Backup", cut);
        Encrypt(Backup);
        strcat(dirpathfolder, Backup);
        // printf("folder yang mau dibackup asli %s\n", dirpathfolder);
        
        sprintf(fname, "%s/RecycleBin", cut);
        strcpy(fnamel, fname);
        Encrypt(fnamel);
        strcat(dirpathl, fnamel);
        mkdir(dirpathl, 0775);

        sprintf(namafix, "%s_deleted_%s", file, waktu);
        Encrypt(namafix);

        sprintf(fpathl, "%s%s", fnamel, namafix);
        strcat(dirpathzip, fpathl);
        // printf("file hasil zip asli : %s\n", dirpathzip);

        char *argv[5] ={"zip", "-jrm", dirpathzip, dirpathnama, dirpathfolder, NULL};
        execv("/usr/bin/zip", argv);
    }

	res = unlink(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;
    char fname[1000], fpath[1000];

    memset(fname, '\0', sizeof(fname));
    strcpy(fname, path);
    Encrypt(fname);
    sprintf(fpath, "%s%s", dirpath, fname);

	res = rmdir(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
	int res;
    char frname[1000], toname[1000], frpath[1000], topath[1000];

    memset(frname, '\0', sizeof(frname));
    memset(toname, '\0', sizeof(toname));
    strcpy(frname, from);
    strcpy(toname, to);
    Encrypt(frname);
    Encrypt(toname);
    sprintf(frpath, "%s%s", dirpath, frname);
    sprintf(topath, "%s%s", dirpath, toname);

	res = rename(frpath, topath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
    char fname[1000], fpath[1000];
    char *ext;
    pid_t child_id;

    child_id = fork();

    memset(fname, '\0', sizeof(fname));
    strcpy(fname, path);
    ext = strrchr(fname, '.');

    if(ext == NULL)
        ext = "0";

    if(strcmp(ext, ".iz1")==0){
        if(child_id == 0){
            char  *argv[4] ={"zenity", "--error", "--text=File ekstensi iz1 tidak boleh diubah permissionnya.\n", NULL};
            execv("/usr/bin/zenity", argv);
        }
    }else{
        Encrypt(fname);
        sprintf(fpath, "%s%s", dirpath, fname);
        res = chmod(fpath, mode);

        if (res == -1)
            return -errno;   
    }
    return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
    char fname[1000], fpath[1000];

    memset(fname, '\0', sizeof(fname));
    strcpy(fname, path);
    Encrypt(fname);
    sprintf(fpath, "%s%s", dirpath, fname);

	res = lchown(fpath, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
    char fname[1000], fpath[1000];

    memset(fname, '\0', sizeof(fname));
    strcpy(fname, path);
    Encrypt(fname);
    sprintf(fpath, "%s%s", dirpath, fname);

	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	struct timeval tv[2];
    char fname[1000], fpath[1000];

    memset(fname, '\0', sizeof(fname));
    strcpy(fname, path);
    Encrypt(fname);
    sprintf(fpath, "%s%s", dirpath, fname);

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
    char fname[1000], fpath[1000];

    memset(fname, '\0', sizeof(fname));
    strcpy(fname, path);
    Encrypt(fname);
    sprintf(fpath, "%s%s", dirpath, fname);

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000], fname[1000];

	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
    else{
        memset(fname, '\0', sizeof(fname));
        strcpy(fname, path);
        Encrypt(fname);
        sprintf(fpath, "%s%s", dirpath, fname);
    }
    int res = 0;
    int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fname[1000], fpath[1000];

    memset(fname, '\0', sizeof(fname));
    strcpy(fname, path);
    Encrypt(fname);
    sprintf(fpath, "%s%s", dirpath, fname);

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

    pid_t child_id;
    child_id = fork();

    if(child_id == 0){
        char cut[1000] = {0};
        char *fuses, *ext;
        time_t rawtime;
        struct tm *info;
        
        char waktu[1000], namafix[1000], fpathl[1000], fpathll[1000], dirpathl[1000], fnamel[1000], file[1000], namafile[1000];

        time(&rawtime);
        info = localtime(&rawtime);
        memset(waktu, '\0', sizeof(waktu));
        sprintf(waktu, "%d:%02d:%02d_%02d:%02d:%02d", info->tm_year+1900, info->tm_mon+1, info->tm_mday, info->tm_hour, info->tm_min, info->tm_sec);
             
        memset(namafix, '\0', sizeof(namafix));
        memset(fpathl, '\0', sizeof(fpathl));
        memset(fpathll, '\0', sizeof(fpathll));
        memset(fname, '\0', sizeof(fname));
        memset(file, '\0', sizeof(file));
        memset(namafile, '\0', sizeof(namafile));

        strcpy(fname, path);
        strcpy(dirpathl, dirpath);
        fuses = strrchr(fname, '/');
        strcpy(file, fuses);

        int i=0;
        while(file[i]!='.'){
            namafile[i] = file[i];
            i++;
        }
        ext = strrchr(file, '.');

        for(int i=strlen(fname); i>0; i--){
            if(fname[i]=='/'){
                int j=i;
                for(int z=0; z<j; z++){
                    cut[z] = fname[z];
                }
                break;
            }
        }
        if(cut == NULL)
            strcpy(cut, "/");

        sprintf(fname, "%s/Backup", cut);
        strcpy(fnamel, fname);
        Encrypt(fnamel);
        strcat(dirpathl, fnamel);
        mkdir(dirpathl, 0775);

        sprintf(namafix, "%s_%s%s", namafile, waktu, ext);
        
        sprintf(fpathl, "%s%s", fname, namafix);
        Encrypt(fpathl);

        sprintf(fpathll, "%s%s", dirpath, fpathl);

        char *argv[4] ={"cp", fpath, fpathll, NULL};
        execv("/bin/cp", argv);
    }
	return res;

}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) {

    (void) fi;

    int res;
    char fname[1000], fpath[1000];
    char ext[100] = {0};

    memset(fname, '\0', sizeof(fname));

    for(int i=strlen(path); i>=0; i--){
        if(path[i]=='/'){
            int j=i-8;
            int z=0;
            for(int k=j; k<(j+8); k++){
                ext[z] = path[k];
                z++;
            }
            break;
        }
    }

    if(strcmp(ext, "YOUTUBER") == 0){
        strcpy(fname, path);
        sprintf(fname, "%s.iz1", fname);
        Encrypt(fname);
        sprintf(fpath, "%s%s", dirpath, fname);
        res = creat(fpath, 0640);
    }else{
        strcpy(fname, path);
        Encrypt(fname);
        sprintf(fpath, "%s%s", dirpath, fname);
        res = creat(fpath, mode);
    }

    if(res == -1)
	    return -errno;

    close(res);

    return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
    .mkdir		= xmp_mkdir,
    .unlink		= xmp_unlink,
    .rmdir		= xmp_rmdir,
    .rename		= xmp_rename,
    .chmod		= xmp_chmod,
    .chown		= xmp_chown,
    .truncate	= xmp_truncate,
	.utimens	= xmp_utimens,
    .open		= xmp_open,
	.read		= xmp_read,
    .write		= xmp_write,
    .create     = xmp_create,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}