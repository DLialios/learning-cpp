#include "mem_align.h"

#pragma GCC diagnostic ignored "-Wuninitialized"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

void union_in_struct()
{
    typedef struct {
        unsigned char variant;
        union {
            int i;
            float f;
        } variants;
    } object;

    object foo;
    foo.variant = 0;
    foo.variants.f = 64.f;
}

void struct_in_union() 
{
    typedef union {
        struct {
            unsigned char byte1;
            unsigned char byte2;
            unsigned char byte3;
            unsigned char byte4;
        } bytes;
        unsigned int dword;
    } HWRegister;

    HWRegister reg; 
    reg.dword = 0x0000; // why would this work?
    reg.bytes.byte1 = 3;
}

void bit_fields()
{
    struct {
        int x;
        int y;
    } s1;

    struct {
        unsigned int x :29;
        unsigned int y :3;
    } s2;

    sizeof(s1);
    sizeof(s2);
}

void mem_align()
{
    struct {     
        char x;
        short y;
        int z;
    } s1 = {'A', 128, 2048};

    char *ptr = &s1;

    printf("\tsizeof(s1) = %zu\n",  sizeof(s1));
    printf("\t%d ",                 ptr[0]);
    printf("%d ",                   *(ptr + 1)); // this is here for memory alignment
    printf("%d ",                   *((short*)(ptr + 2)));
    printf("%d",                    *((int*)(ptr + 4)));
}

// -- structs have an alignment that is equal to their widest member
// -- the address of a struct is equal to the address of its first member
// -- structs do not have leading padding
// -- the stride address of a struct is the first address following the
//    structure data that has the same alignment as the structure
void mem_align2()
{
    // *000 0000 **** **** *000 0000 O  <-- O is the stride addr 
    struct s1 {    
        char x;
        double y;
        char z;
    } s1 = {67,78.f,68};

    printf("\t\ts1.x=%d\n",                      *(((char*)(&s1)) + 0));
    printf("\t\ts1.y=%f\n",                      *((double*)(((char*)(&s1)) + 8)));
    printf("\t\ts1.z=%d\n",                      *(((char*)(&s1)) + 16));
    printf("\tsizeof(s1) = %zu\n",               sizeof(s1));
    printf("\t\toffsetof(struct s1, x) = %zu\n", offsetof(struct s1, x));
    printf("\t\toffsetof(struct s1, y) = %zu\n", offsetof(struct s1, y));
    printf("\t\toffsetof(struct s1, z) = %zu\n", offsetof(struct s1, z));

    // **00 0000 **** **** 
    struct s2 {     
        char x, z;
        double y;
    } s2 = {'A','B',64.f};

    printf("\t\ts2.x=%d\n",                      *(((char*)(&s2)) + 0));
    printf("\t\ts2.z=%d\n",                      *(((char*)(&s2)) + 1));
    printf("\t\ts2.y=%f\n",                      *(((double*)(&s2)) + 1));
    printf("\tsizeof(s2) = %zu\n",               sizeof(s2));
    printf("\t\toffsetof(struct s2, x) = %zu\n", offsetof(struct s2, x));
    printf("\t\toffsetof(struct s2, z) = %zu\n", offsetof(struct s2, z));
    printf("\t\toffsetof(struct s2, y) = %zu\n", offsetof(struct s2, y));

    // **** **** **00 0000 *000 0000
    struct foo5 {    
        struct foo5_inner {
            char *p;
            short x;
        } inner;
        char c;
    };

    printf("\tsizeof(struct foo5) = %zu\n",              sizeof(struct foo5));
    printf("\t\toffsetof(struct foo5, inner) = %zu\n",   offsetof(struct foo5, inner));
    printf("\t\toffsetof(struct foo5, inner.p) = %zu\n", offsetof(struct foo5, inner.p));
    printf("\t\toffsetof(struct foo5, inner.x) = %zu\n", offsetof(struct foo5, inner.x));
    printf("\t\toffsetof(struct foo5, c) = %zu\n",       offsetof(struct foo5, c));

    // *000 0000 0000 0000 *000 0000 0000 0000 **** **** **** **** *000 0000 0000 0000 *000 0000 0000 0000
    // 80% of this struct is slop!
    struct foo0 {  
        char x;
        struct foo1 {
            char y;
            long double z;
            char a;
        } inner;
        char b;
    };

    // **** *0O   
    struct foofoo { 
        short s;
        char c;
        char c2;
        char c3;
    };

    printf("\tsizeof(struct foofoo) = %zu\n",         sizeof(struct foofoo));
    printf("\t\toffsetof(struct foofoo, s) = %zu\n",  offsetof(struct foofoo, s));
    printf("\t\toffsetof(struct foofoo, c) = %zu\n",  offsetof(struct foofoo, c));
    printf("\t\toffsetof(struct foofoo, c2) = %zu\n", offsetof(struct foofoo, c2));
    printf("\t\toffsetof(struct foofoo, c3) = %zu\n", offsetof(struct foofoo, c3));

    // **** *000 O
    // padding can preceed or follow the payload bits (implementation defined)
    struct foo6 { 
        short s;
        char c;
        int flip:1;
        int nybble:4;
        int septet:7;
    };

    printf("\tsizeof(struct foo6) = %zu\n",        sizeof(struct foo6));
    printf("\t\toffsetof(struct foo6, s) = %zu\n", offsetof(struct foo6, s));
    printf("\t\toffsetof(struct foo6, c) = %zu\n", offsetof(struct foo6, c));
}
