#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

#define ELF_OFFSET_IN_DISK 0

extern size_t ramdisk_read();
extern int fs_open();
extern size_t fs_read();
static uintptr_t loader(PCB *pcb, const char *filename) {
  int fd = fs_open(filename, 0, 0);
  Log("fd: %d", fd);

  Elf_Ehdr *elf;
  Elf_Phdr *ph = NULL;
  uint8_t buf[4096];
  fs_read(fd, buf, 4096);
  // ramdisk_read(buf, file_table[fd].disk_offset, 4096);
  elf = (void*) buf;
  
  const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
  assert (*p_magic == elf_magic);
  // Log("ELF: %p", elf);

  ph = (void *)(buf + elf->e_phoff);
  for (int i = 0; i < elf->e_phnum; ++i, ++ph) {
    if (ph->p_type == PT_LOAD) {
      // read the content of the segment from the ELF file 
      // to the memory region [VirtAddr, VirtAddr + FileSiz)

      Log("p_vaddr: %x p_offset: %x p_filesz: %x", ph->p_vaddr, ph->p_offset, ph->p_filesz);
      // ramdisk_read((void *)ph->p_vaddr,ph->p_offset + file_table[fd].disk_offset, ph->p_filesz);
      fs_read(fd, (void *)ph->p_vaddr, ph->p_filesz);
      // zero the memory region [VirtAddr + FileSiz, VirtAddr + MemSiz)
      memset ((void *)(ph->p_vaddr+ph->p_filesz),0,ph->p_memsz-ph->p_filesz);
    }
  }

  volatile uint32_t entry = elf->e_entry;
  return entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

