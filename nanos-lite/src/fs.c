#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
} Finfo;


enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

extern size_t serial_write();
/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write},
  {"/dev/events", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define FILE_TAB_LEN (sizeof(file_table) / sizeof(file_table[0]))

struct {
  bool open;
  size_t open_offset;
} file_state[FILE_TAB_LEN];

int fs_open(const char *pathname, int flags, int mode) {
  int res = -1, i;
  for (i = 0; i < FILE_TAB_LEN; ++i) {
    if (!strcmp(file_table[i].name, pathname)) {
      res = i;
      file_state[res].open = true;
      file_state[res].open_offset = 0;
      break;
    }
  }

  assert(i < FILE_TAB_LEN);
  printf("[fs]: pathname: %s, res: %d\n", pathname, res);
  return res;
}

extern size_t ramdisk_read();
extern size_t ramdisk_write();
extern size_t events_read();
size_t fs_read(int fd, void *buf, size_t len) {
  assert(fd >= 0);
  assert(fd < FILE_TAB_LEN);
  assert(file_state[fd].open);
  assert((fd <= 2) || (fd >= 3 && file_state[fd].open));

  size_t res_len = 0;
  if (fd <= 2) {
    panic("nanos lite dont support stdin read");
  } else if (fd == 3) {
    res_len = events_read(buf, 0, 0);
  } else {
    printf("other files: %p\n", buf);
    int remain_len = file_table[fd].size - file_state[fd].open_offset;
    assert(remain_len >= 0);
    if (remain_len < len) len = remain_len;

    static int cnt = 0;
    static int filez = 0;
    int offset = 0;
    if (++cnt <= 2) {
      offset = 0;
      filez = len;
    } else if (cnt == 3) {
      offset = filez;
    } else {
      offset = file_state[fd].open_offset;
    }

    // Log("[fs]offset: %d", offset);
    res_len = ramdisk_read(buf, file_table[fd].disk_offset + offset, len);
    file_state[fd].open_offset += res_len;
  }

  return res_len;
}

size_t fs_write(int fd, const void *buf, size_t len) {
  assert(fd >= 0);
  assert(fd < FILE_TAB_LEN);
  // Log("fd: %d", fd);
  assert((fd <= 2) || (fd >= 3 && file_state[fd].open));

  // Log("[fs]offset: %d", offset);
  size_t res_len = 0;
  if(fd <= 2) { // NOTE: can refactor in one oper?
    res_len = file_table[fd].write(buf, 0, len);
  } else {
    int remain_len = file_table[fd].size - file_state[fd].open_offset;
    assert(remain_len >= 0);
    if (remain_len < len) len = remain_len;
    res_len = ramdisk_write(buf, file_table[fd].disk_offset + file_state[fd].open_offset, len);
    file_state[fd].open_offset += res_len;
  }

  return res_len;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
  if (fd <= 2) return -1;

  assert(fd < FILE_TAB_LEN);
  assert(file_state[fd].open);
  switch(whence) {
    case SEEK_SET: file_state[fd].open_offset = 0; break;
    case SEEK_CUR: break;
    case SEEK_END: file_state[fd].open_offset = file_table[fd].size; break;
    default: assert(0);
  }

  file_state[fd].open_offset += offset;
  assert(file_state[fd].open_offset >= 0);
  assert(file_state[fd].open_offset <= file_table[fd].size);
  return file_state[fd].open_offset;
}

int fs_close(int fd) {
  // assert(file_state[fd].open); // BUG: ?
  file_state[fd].open = false;
  return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}
