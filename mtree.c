#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#include "mtree.h"

void print_hash(char* hash){
    int i = 0;
    char buff[(SHA256_DIGEST_LENGTH << 1) + 1];
    for(i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        sprintf(buff + (i << 1), "%02x", hash[i]);
    buff[i+1] = 0;
    printf("0x%s", buff);
}

char* concat(const char *a, const char* b){
    char *c = NULL;
    if(a && b){
        c = malloc(SHA256_DIGEST_LENGTH << 1);
        for(int i = 0; i < SHA256_DIGEST_LENGTH; ++i){
            c[i] = a[i];
            c[i + SHA256_DIGEST_LENGTH] = b[i];
        }
    }
    return c;
}

char* hash(char* a, size_t len){
	char *h = NULL;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    if(a){
        h = malloc(SHA256_DIGEST_LENGTH);
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, a, len);
        SHA256_Final(hash, &sha256);
        memcpy(h, hash, SHA256_DIGEST_LENGTH);
    }
    return h;
}

char* build_tree(mtree *tree, int l, int r){
    int m;
    char *cat, *left, *right;

    if(l >= r) return "";

    if(l == r - 1){
        cat = concat(tree->nodes[l], tree->nodes[r]);
        return hash(cat, SHA256_DIGEST_LENGTH << 1);
    }

    m = (l + r) / 2;
    left = build_tree(tree, l, m);
    right = build_tree(tree, m + 1, r);

    return hash(concat(left, right), SHA256_DIGEST_LENGTH << 1); 
}
