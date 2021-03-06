#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#include "mtree.h"

size_t get_tree_size(int num_blks) {
  if (__builtin_popcount(num_blks) == 1)
    return num_blks;
  else
    return 1 << ((sizeof(num_blks) << 3) - __builtin_clz(num_blks - 1));
}

void print_hash(uchar * hash) {
  const int buff_len = (SHA256_DIGEST_LENGTH << 1) + 1;
  char buff[buff_len];
  for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    sprintf(buff + (i << 1), "%02x", hash[i]);
  buff[buff_len - 1] = 0;
  printf("0x%s", (uchar *) buff);
}

uchar *concat(const uchar * a, const uchar * b) {
  uchar *c = NULL;
  if (a && b) {
    c = malloc(SHA256_DIGEST_LENGTH << 1);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
      c[i] = a[i];
      c[i + SHA256_DIGEST_LENGTH] = b[i];
    }
  }
  return c;
}

uchar *hash(uchar * a, size_t len) {
  uchar *h = NULL;
  if (a) {
    h = malloc(SHA256_DIGEST_LENGTH);
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, a, len);
    SHA256_Final(h, &sha256);
  }
  return h;
}

uchar *build_tree(mtree_t * tree, int l, int r) {
  int m;
  uchar *cat, *left, *right;

  if (l >= r)
    return (uchar *) "";

  if (l == r - 1) {
    cat = concat(tree->nodes[l], tree->nodes[r]);
    return hash(cat, SHA256_DIGEST_LENGTH << 1);
  }

  m = (l + r) / 2;
  left = build_tree(tree, l, m);
  right = build_tree(tree, m + 1, r);

  return hash(concat(left, right), SHA256_DIGEST_LENGTH << 1);
}
