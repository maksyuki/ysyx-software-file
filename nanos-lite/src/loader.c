#include <proc.h>
#include <elf.h>
#include <string.h>

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

  Elf_Ehdr *elf_hdr;
  Elf_Phdr *ph = NULL;
  uint8_t buf[4096];
  fs_read(fd, buf, 1024); // HACK: read len is not right?
  // ramdisk_read(buf, file_table[fd].disk_offset, 4096);
  elf_hdr = (void*) buf;
  
  // const uint32_t elf_magic = 0x464c457f;
  Log("elf_hdr's size: %d", sizeof(Elf_Ehdr));
  Log("elf->e_phoff: %d", elf_hdr->e_phoff);
	uint32_t *p_magic = (void *)buf;
  assert (*p_magic == 0x464c457f);
  // Log("ELF: %p", elf_hdr);

  ph = (void *)(buf + elf_hdr->e_phoff);
  Log("elf_ph's size: %d", sizeof(Elf_Phdr));
  for (int i = 0; i < elf_hdr->e_phnum; ++i, ++ph) {
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

  volatile uint32_t entry = elf_hdr->e_entry;
  return entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

void assign_str(int idx, const char *str, uintptr_t *str_addr, void **heap) {
  *heap -= strlen(str) + 1;
  str_addr[idx] = (uintptr_t)(*heap);
  // printf("heap.end: %p\n", heap.end);
  // HACK: can refactor below impl
  for(int j = 0; str[j]; ++j) {
    *(char *)(*heap + j) = str[j];
  }
}

void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]) {
  uintptr_t entry = loader(pcb, filename);
  Area kra;
  kra.start = &pcb->cp;
  kra.end = kra.start + STACK_SIZE;
  printf("[context_uload]entry: %p\n", (void *)entry);

  int argc_cnt = 1;
  bool is_incl_fn = false;
  // NOTE: need to use 'argc' to get the loop num!
  for(int i = 0; argv[i]; ++i) {
    printf("[context_uload] argv[%d]: %s len: %d\n", i, argv[i], strlen(argv[i]));
    if(!strcmp(filename, argv[i])) {
      is_incl_fn = true;
      continue;
    }
    ++argc_cnt;
  }

  Context *contx = ucontext(NULL, kra, (void *)entry);
  // allocate user heap
  void *proc_heap = new_page(8); // 8 * 4Kb size
  proc_heap += 8 * PGSIZE; // get stack top
  printf("[context_uload]proc heap: %p\n", proc_heap);

  // string area(include exec program path[argv[0]])
  uintptr_t str_addr[argc_cnt];
  // HACK: handle argv[0] == filename 
  for(int i = argc_cnt - 1; i >= 0; --i) {
    if(i == 0) {
      assign_str(i, filename, str_addr, &proc_heap);
    } else {
      if(is_incl_fn) {
        assign_str(i, argv[i], str_addr, &proc_heap);
      } else {
        assign_str(i, argv[i-1], str_addr, &proc_heap);
      }
    }
  }

  for(int i = 0; i < argc_cnt; ++i) {
    printf("[context_uload]str addr: %p val: %s\n", str_addr[i], str_addr[i]);
  }

  // argv pointer
  proc_heap -= sizeof(void *);
  *((char **)proc_heap) = NULL;
  for(int i = argc_cnt - 1; i >= 0; --i) {
    proc_heap -= sizeof(void *);
    *((uintptr_t *)proc_heap) = str_addr[i];
  }

  // argc
  proc_heap -= sizeof(int);
  *(int *)(proc_heap) = argc_cnt;
  contx->GPRx = (uintptr_t)(proc_heap);
  pcb->cp = contx;
  printf("[context_uload] proc stack top: %p contx: %p argc_cnt: %d\n", proc_heap, pcb->cp, argc_cnt);
}