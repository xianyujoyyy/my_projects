
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

typedef unsigned char* byte_pointer;

// 55
void show_bytes(byte_pointer p, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%.2x", p[i]);
    }
    printf("/n");
}
// 57
void show_short(short num) {
    return show_bytes(&num, sizeof(num));
}

void show_long(long num) {
    return show_bytes(&num, sizeof(num));
}

void show_double(double num) {
    return show_bytes(&num, sizeof(num));
}
// 58
bool is_little_endian() {
    int16_t num = 1;
    unsigned char* p = &num;
    if ((int8_t)p[0]) return true;
    return false;
}
// 59
void combine(int x, int y) {
    printf("x = 0x%x, y = 0x%x \n", x, y);
    int z = (x & 0x0000ffff) | (y & 0xffff0000);
    printf("z = 0x%x", z);
    return;
}
// 60
void replace_bytes(unsigned x, int i, unsigned char b) {
    
}

// 61
int check_x(int x) {
    size_t n = sizeof(int) << 3;
    return !(~x) || !x || !((~x) & 0xff) || !((x >> (n - 8)) & 0xff);
}

// 62
int int_shifts_are_arithmetic() {
    int x = INT_MIN;
    size_t n_bits = sizeof(int) << 3;
    return (x >> n_bits) & 1;
}

// 63
unsigned sr1(unsigned x, int k) {
    unsigned xsra = (int) x >> k;
    unsigned y = INT_MIN;
    y >>= k;
    return xsra & y;
}

int sra(int x, int k) {
    int xsr1 = (unsigned) x >> k;
    
}

// 64


