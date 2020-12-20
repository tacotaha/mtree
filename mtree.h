#ifndef MTREE_H
#define MTREE_H

// 256MB blocks
#define BLK_SIZE 256

typedef unsigned char uchar;

typedef struct mtree {
  size_t size;                  // 2^k for some k
  uchar *nodes[];
} mtree_t;

void print_hash(uchar *hash);
uchar *concat(const uchar *, const uchar *);
uchar *hash(uchar *, size_t);
uchar *build_tree(mtree_t *, int, int);

#endif                          /* MTREE_H */
