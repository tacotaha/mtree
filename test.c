#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mtree.h"

#define TREE_SIZE (1 << 4)
#define NODE_SIZE (1 << 5)

uchar *test_concat(const uchar * a, const uchar * b) {
  uchar *c = NULL;
  int i = -1, j = 0;
  if (a && b) {
    c = malloc(TREE_SIZE);
    while (a[++i])
      c[i] = a[i];
    while (b[j])
      c[i++] = b[j++];
    c[i] = 0;
  }
  return c;
}

uchar *test_build_tree(mtree_t * tree, int l, int r) {
  int m;
  uchar *left, *right;

  if (l >= r)
    return (uchar *) "";

  if (l == r - 1)
    return test_concat(tree->nodes[l], tree->nodes[r]);

  m = (l + r) / 2;
  left = test_build_tree(tree, l, m);
  right = test_build_tree(tree, m + 1, r);

  return test_concat(left, right);
}

int main(int argc, char *argv[]) {
  int index;
  mtree_t *m;
  uchar *root, *expected, base = 0x41;

  m = malloc(sizeof(mtree_t) + (TREE_SIZE * sizeof(uchar *)));
  m->size = TREE_SIZE;

  // ABCDE...
  for (uchar c = base; c < base + TREE_SIZE; ++c) {
    index = c - base;
    m->nodes[index] = calloc(1, NODE_SIZE);
    m->nodes[index][0] = c;
    printf("Block %02d: %s\n", index, m->nodes[index]);
  }

  root = test_build_tree(m, 0, TREE_SIZE - 1);
  printf("Root = %s\n", root);

  expected = malloc(TREE_SIZE + 1);
  for (int i = 0; i < TREE_SIZE; ++i)
    expected[i] = base + i;
  expected[TREE_SIZE] = 0;

  assert(!strcmp((char *) root, (char *) expected));

  return 0;
}
