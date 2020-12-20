#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "mtree.h"

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

    root = build_tree(m, 0, num_blks - 1);
    printf("Root: ");
    print_hash(root);
    printf("\n");

    return 0;
}
