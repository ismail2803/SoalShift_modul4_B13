
[Soal 1](#soal-1)  
[Soal 2](#soal-2)  
[Soal 3](#soal-3)  
[Soal 4](#soal-4)  
[Soal 5](#soal-5) 

## Soal 1
Semua nama file dan folder harus terenkripsi    
Enkripsi yang Atta inginkan sangat sederhana, yaitu [Caesar cipher](https://en.wikipedia.org/wiki/Caesar_cipher). Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:

qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r       bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0

Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:

“INI_FOLDER/halo” saat belum di-mount maka akan bernama “n,nsbZ]wio/QBE#”, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi “INI_FOLDER/halo” .

Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan

#### Penyelesaian
```
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
```
fungsi dekripsi akan mencari karakter dari file/direktori pada list Caesar cipher dan mengganti karakter tersebut dengan karakter ke 17 sebelum karakter tersebut pada list Caesar cipher, jika melebihi indeks maka dimulai dari indeks ke 93 lagi. ketika menemui '/' maka karakter tetap sama.
```
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
```
fungsi enkripsi akan mencari karakter dari file/direktori pada list Caesar cipher dan mengganti karakter tersebut dengan karakter ke 17 setelah character tersebut pada list Caesar cipher list, jika melebihi indeks maka dimulai dari indeks ke 0 lagi. dan ketika menemui '/' maka karakternya tetap sama.

Pada fungsi xmp_readdir semua direktori dan file dari folder yang dimount akan didekripsi kecuali direktori "." dan ".."
Setiap akan melakukan operasi di fuse pada suatu file/direktori maka fungsi yang dipakai akan mengenkripsi nama file/direktori tersebut agar pada folder yang dimount nama file/direktori tersebut telah di enkripsi
## Soal 2
Semua file video yang tersimpan secara terpecah-pecah (splitted) harus secara otomatis tergabung (joined) dan diletakkan dalam folder “Videos”

Urutan operasi dari kebutuhan ini adalah:
- Tepat saat sebelum file system di-mount
   	 1.  Secara otomatis folder “Videos” terbuat di root directory file system   	    
   	 2. Misal ada sekumpulan file pecahan video bernama “computer.mkv.000”, “computer.mkv.001”, “computer.mkv.002”, dst. Maka secara otomatis file pecahan tersebut akan di-join menjadi file video “computer.mkv” 	Untuk mempermudah kalian, dipastikan hanya video file saja yang terpecah menjadi beberapa file. File pecahan tersebut dijamin terletak di root folder fuse
   	 3. Karena mungkin file video sangat banyak sehingga mungkin saja saat 	menggabungkan file video, file system akan membutuhkan waktu yang lama untuk sukses ter-mount. Maka pastikan saat akan menggabungkan file pecahan video, file system akan membuat 1 thread/proses(fork) baru yang dikhususkan untuk menggabungkan file video tersebut
   	 4. Pindahkan seluruh file video yang sudah ter-join ke dalam folder “Videos”
   	 5.  Jangan tampilkan file pecahan di direktori manapun
- Tepat saat file system akan di-unmount
	1.  Hapus semua file video yang berada di folder “Videos”, tapi jangan hapus 	file pecahan yang terdapat di root directory file system	    
	2.  Hapus folder “Videos”

#### Penyelesaian

## Soal 3  
Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
-   Owner Name : ‘chipset’ atau ‘ic_controller’
    
-   Group Name  : ‘rusak’
    
-   Tidak dapat dibaca

Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.

#### Penyelesaian
```
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
```
Pada fungsi readdir ditambahkan pengecekan jika file tersebut memiliki owner chipset atau ic_controller dan group rusak dan tidak dapat dibaca, maka file tersebut akan dihapus. setelah file terhapus maka dibuat file dengan nama filemiris.txt yang akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses file tersebut
## Soal 4
Pada folder YOUTUBER, setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah “.iz1”.  File berekstensi “.iz1” tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”
#### Penyelesaian
```
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

	return  0;
}
```
Pada fungsi xmp_mkdir akan dilakukan pengecekan saat akan membuat direktori, jika direktori pembuatan direktori baru tersebut adalah YOUTUBER, maka direktori permissionnya  akan otomatis menjadi 750
```
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
```
Pada fungsi xmp_create akan dilakukan pengecekan saat akan membuat file, jika direktori pembuatan file baru tersebut adalah YOUTUBER, maka file permissionnya  akan otomatis menjadi 640 dan ekstensi filenya akan bertambah “.iz1”.
```
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
```
Pada fungsi xmp_chmod, sebelum melakukan perubahan permission file akan dicek ekstensi file terlebih dahulu, jika file tersebut memiliki eketensi iz1 maka permissionnya tidak akan diubah dan memunculkan error “File ekstensi iz1 tidak boleh diubah permissionnya.”

## Soal 5
Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama Backup kemudian hasil dari save tersebut akan disimpan pada backup dengan nama namafile_[timestamp].ekstensi. Dan ketika file asli dihapus, maka akan dibuat folder bernama RecycleBin, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama namafile_deleted_[timestamp].zip dan ditaruh ke dalam folder RecycleBin (file asli dan backup terhapus). Dengan format [timestamp] adalah yyyy-MM-dd_HH:mm:ss

#### Penyelesaian
```
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
```
Pada fungsi xmp_write, ketika ada file yang diedit dan disave maka direktori yang sama dengan file tersebut akan dibuat direktori Backup dan hasil perubahan file tersebut disimpan di Backup dengan format namafile_[timestamp].ekstensi
```
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
```
Pada fungsi xmp_unlink, ketika ada file yang akan dihapus maka direktori yang sama dengan file tersebut akan dibuat direktori RecycleBin dan file tersebut beserta semua backup dari file yang dihapus di zip dengan format nama namafile_deleted_[timestamp].zip dan disimpan dalam direktori RecycleBin(file asli dan backup terhapus)
