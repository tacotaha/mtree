#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "mtree.h"

int main(int argc, char *argv[]) {
  mtree_t *m;
  size_t size, tree_size;
  struct stat st;
  void *base_addr = NULL;
  int fd = 0, num_blks, offset;
  uchar *root, *base;
  char *payload = argv[argc > 1];

  fd = open(payload, O_RDONLY);
  if (fd < 0) {
    perror("open:");
    return 1;
  }

  stat(payload, &st);
  size = st.st_size;

  base_addr = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
  close(fd);

  num_blks = size / BLK_SIZE + 1;
  tree_size = get_tree_size(num_blks);

  printf("File size = %ld, Num Blocks: %d, Tree Size: %ld\n",
         size, num_blks, tree_size);

  m = malloc(sizeof(mtree_t) + (tree_size * sizeof(uchar *)));
  m->size = tree_size;

  for (int i = 0; i < num_blks; ++i) {
    base = (uchar *) base_addr + i;
    offset = (i == num_blks - 1) ? size : BLK_SIZE;
    m->nodes[i] = hash(base, offset);
    printf("Block %02d: ", i);
    print_hash(m->nodes[i]);
    printf("\n");
    size -= BLK_SIZE;
  }

  for (int i = num_blks; i < tree_size; ++i) {
    m->nodes[i] = hash((uchar *) "", 0);
    printf("Block %02d: ", i);
    print_hash(m->nodes[i]);
    printf("\n");
  }

  root = build_tree(m, 0, tree_size - 1);
  printf("Root: ");
  print_hash(root);
  printf("\n");

  return 0;
}
