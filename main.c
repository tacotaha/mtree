#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <openssl/sha.h>

// 256MB blocks
#define BLK_SIZE 256

typedef struct mtree{
    size_t size; // 2^k for some k
    char *nodes[];
}mtree;

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

char *hash(char* a, size_t len){
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

char* calculate_tree(mtree *tree, int l, int r){
    int m;
    char *cat, *left, *right;

    if(l >= r) return "";

    if(l == r - 1){
        cat = concat(tree->nodes[l], tree->nodes[r]);
        return hash(cat, SHA256_DIGEST_LENGTH << 1);
    }

    m = (l + r) / 2;
    left = calculate_tree(tree, l, m);
    right = calculate_tree(tree, m + 1, r);

    return hash(concat(left, right), SHA256_DIGEST_LENGTH << 1); 
}

int main(int argc, char* argv[]){
    mtree *m;
    size_t size;
    struct stat st;
    void *base_addr = NULL;
    int fd = 0, num_blks, offset;
    char *root, *base, *payload = argv[argc  > 1];

    fd = open(payload, O_RDONLY);
    if(fd < 0){
        perror("open:");
        return 1;
    }
     
    stat(payload, &st);
    size = st.st_size;

    base_addr = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    num_blks = size / BLK_SIZE + 1;
    
    printf("File size = %ld, Num Blocks: %d\n", size, num_blks);
    
    m = malloc(sizeof(size_t) + (num_blks * sizeof(char*)));
    if(!m){
        perror("malloc:");
        return 1;
    }

    m->size = num_blks;

    for(int i = 0; i < num_blks; ++i){
        base = (char*) base_addr + i;
        offset = (i == num_blks - 1) ? size : BLK_SIZE; 
        m->nodes[i] = hash(base, offset); 
        printf("Block %02d: ", i);
        print_hash(m->nodes[i]);
        printf("\n");
        size -= BLK_SIZE;
    }

    root = calculate_tree(m, 0, num_blks - 1);
    printf("Root: ");
    print_hash(root);
    printf("\n");

    return 0;
}
