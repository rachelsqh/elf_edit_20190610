#include <stdio.h>
#include <elf.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc,char *argv[])
{
	int i = 0,fd = 0;
	void *file_mm = NULL;
	struct stat f_state;

	if(argc < 2){
		return -1;
	}

	fd = open(argv[1],O_RDWR);
	if(0 > fd)
		return -1;

	flock(fd,LOCK_UN);
	lstat(argv[1],&f_state);
	file_mm = mmap(NULL,f_state.st_size,PROT_WRITE | PROT_READ,MAP_SHARED,fd,0);
	if(!file_mm){
		flock(fd,LOCK_UN);
		close(fd);
	}
	printf("e_ident: %s\n",((Elf64_Ehdr *)file_mm)->e_ident);
	printf("e_entry: 0x%lx\n",((Elf64_Ehdr *)file_mm)->e_entry);
        
	((Elf64_Ehdr *)file_mm)->e_entry += 0x10;

	
	printf("e_entry: 0x%lx\n",((Elf64_Ehdr *)file_mm)->e_entry);
	
	munmap(file_mm,f_state.st_size);
	flock(fd,LOCK_UN);
	close(fd);

	return 0;
}

