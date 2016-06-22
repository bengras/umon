/* elfld_memcpy() & elfld_memset() 
 * Front end to all bulk memory modification calls done by the
 * loader.  Do normal memory transfer, then caches are
 * flushed/invalidated.
 * These are only needed if a load is done with cache enabled and they
 * assume the cache operations are linkable.
 */
int
elfld_memcpy(char *to, char *from, int count, int verbose, int verify)
{
	int rc;

	rc = s_memcpy(to,from,count,verbose,verify);

	if (verbose <= 1) {
		flushDcache(to,count);
		invalidateIcache(to,count);
	}

	return(rc);
}

int
elfld_memset(uchar *to,uchar val,int count,int verbose,int verifyonly)
{
	int rc;

	rc = s_memset(to,val,count,verbose,verifyonly);

	if (verbose <= 1)  {
		flushDcache((char *)to, count);
		invalidateIcache((char *)to, count);
	}

	return(rc);
}

void
showEntrypoint(unsigned long entrypoint)
{
	printf(" entrypoint: 0x%lx\n",entrypoint);
}

void
showSection(char *sname)
{
	printf(" %-10s: ",sname);
}

typedef int (*elf_fileread_t)(char *, int, void *, int);

int elf_get_shdr(elf_fileread_t elf_fileread, char *filename, ELFFHDR *ehdr, ELFSHDR *shdr, int secno)
{
	/* read shdr */
	if(secno < 0 || secno >= ehdr->e_shnum) {
                printf("section number out of range\n");
                return(-1);
	}

	if(secno == SHN_UNDEF) {
                printf("section number is SHN_UNDEF\n");
                return(-1);
	}

	if(sizeof(*shdr) != ehdr->e_shentsize) {
                printf("section size mismatch\n");
                return(-1);
	}

	if(elf_fileread(filename, ehdr->e_shoff + secno * ehdr->e_shentsize, shdr, sizeof(*shdr)) != sizeof(*shdr)) {
                printf("can't read shdr of %s\n", filename);
                return(-1);
	}

	return 0;
}

/* elf_load():
 *    load the ELF formatted file named 'filename' into memory and return
 *    its entry point into 'entrypoint' if supplied.
 */
int
elf_load(char *filename, int verbose,long *entrypoint,char *sname,int verifyonly)
{
	Elf32_Word	size, notproctot;
	int			i, j, err;
	char		name[80];
	Elf32_Addr	sh_addr;
	ELFFHDR		ehdr;
	ELFSHDR		shdr;
	ELFSHDR		string_shdr;
	ELFPHDR		phdr;
	char		*env;
	elf_fileread_t	elf_fileread;

#define ELF_FILEREAD_NAME "ELF_FILEREAD"

        if (!(env = getenv(ELF_FILEREAD_NAME))) {
                printf("%s not set\n", ELF_FILEREAD_NAME);
                return(CMD_FAILURE);
        }

        elf_fileread = (elf_fileread_t) strtol(env,0,0);

	if(elf_fileread(filename, 0, &ehdr, sizeof(ehdr)) != sizeof(ehdr)) {
                printf("can't read header of %s\n", filename);
                return(CMD_FAILURE);
	}

	/* Verify basic file sanity... */
	if ((ehdr->e_ident[0] != 0x7f) || (ehdr->e_ident[1] != 'E') ||
		(ehdr->e_ident[2] != 'L') || (ehdr->e_ident[3] != 'F'))
		return(-1);

	err = 0;

	if(elf_get_shdr(elf_fileread, filename, &ehdr, &string_shdr, ehdr->e_shstrndx)) {
                printf("can't read string shdr of %s\n", filename);
                return(CMD_FAILURE);
	}

	/* For each section header, relocate or clear if necessary... */
	for (i=0;!err && i<ehdr->e_shnum;i++,shdr++) {
		if ((size = shdr->sh_size) == 0)
			continue;

		if(elf_get_string(elf_fileread, filename, &ehdr, &string_shdr, name, sizeof(name), shdr->sh_name)) {
	                printf("can't read string shdr of %s\n", filename);
	                return(CMD_FAILURE);
		}

		/* If incoming section name is specified, then we only load the
		 * section with that name...
		 */
		if ((sname != 0) && (strcmp(sname,name) != 0))
			continue;
	
		if (verbose) showSection(name);
	
		if (!(shdr->sh_flags & SHF_ALLOC)) {
			notproctot += size;
			if (verbose)
				printf("     %7ld bytes not processed (tot=%ld)\n",
					size,notproctot);
			continue;
		}

		sh_addr = shdr->sh_addr;

		/* Look to the program header to see if the destination address
		 * of this section needs to be adjusted.  If this section is
		 * within a program header and that program header's members
		 * p_vaddr & p_paddr are not equal, then adjust the section
		 * address based on the delta between p_vaddr and p_paddr...
		 */ 
		phdr = (ELFPHDR *)((int)ehdr + ehdr->e_phoff);
		for (j=0;j<ehdr->e_phnum;j++,phdr++) {
			if ((phdr->p_type == PT_LOAD) &&
				(sh_addr >= phdr->p_vaddr) &&
				(sh_addr < phdr->p_vaddr+phdr->p_filesz) &&
				(phdr->p_vaddr != phdr->p_paddr)) {
					sh_addr += (phdr->p_paddr - phdr->p_vaddr);
					break;
			}
		}
	
		if (shdr->sh_type == SHT_NOBITS) {
			if (elfld_memset((uchar *)(sh_addr),0,size,
				verbose,verifyonly) != 0)
				err++;
		}
		else {
			if (elfld_memcpy((char *)(sh_addr),
				(char *)((int)ehdr+shdr->sh_offset),
				size,verbose,verifyonly) != 0)
				err++;
		}
	}

	if (err)
		return(err);

	if (verbose && !verifyonly && !sname)
		showEntrypoint(ehdr->e_entry);

	/* Store entry point: */
	if (entrypoint)
		*entrypoint = (long)(ehdr->e_entry);

	return(0);
}

