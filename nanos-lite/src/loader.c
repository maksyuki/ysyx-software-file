#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

static uintptr_t loader(PCB *pcb, const char *filename) {
  FILE * file = fopen(filename, "rb");
  
  if (file) {
    Elf_Ehdr hdr;
    int num = fread(&hdr, sizeof(hdr), 1, file);
    if (memcmp(hdr.e_ident, ELFMAG, SELFMAG) == 0) {
      Log("===========================VALID ELF FILE %d", num);

    }
    fclose(file);
  }
  // ramdisk_read();
  // ramdisk_write();
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  // ((void(*)())entry) ();
}

