#ifndef MTREE_H
#define MTREE_H 

// 256MB blocks
#define BLK_SIZE 256

typedef struct mtree{
    size_t size; // 2^k for some k
    char *nodes[];
}mtree;

void print_hash(char* hash);
char* concat(const char *a, const char* b);
char* hash(char* a, size_t len);
char* build_tree(mtree *tree, int l, int r);

#endif /* MTREE_H */
