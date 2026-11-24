#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>

using namespace std;

static const int bang_ip[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7
};

static const int bang_ip_inv[64] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25
};

static const int bang_e[48] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

static const int bang_p[32] = {
    16,  7, 20, 21, 29, 12, 28, 17,
     1, 15, 23, 26,  5, 18, 31, 10,
     2,  8, 24, 14, 32, 27,  3,  9,
    19, 13, 30,  6, 22, 11,  4, 25
};

static const int bang_pc1[56] = {
    57, 49, 41, 33, 25, 17,  9,
     1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
     7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
};

static const int bang_pc2[48] = {
    14, 17, 11, 24,  1,  5,
     3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

static const int so_dich[16] = {
    1, 1, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 2, 2, 2, 1
};

static const uint8_t sbox_des[8][4][16] = {
    {
        {14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7},
        { 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8},
        { 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0},
        {15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13}
    },
    {
        {15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10},
        { 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5},
        { 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15},
        {13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9}
    },
    {
        {10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1},
        {13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7},
        { 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12}
    },
    {
        { 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15},
        {13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9},
        {10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4},
        { 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14}
    },
    {
        { 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9},
        {14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6},
        { 4, 2, 1,11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14},
        {11, 8,12, 7, 1,14, 2,13, 6,15, 0, 9,10, 4, 5, 3}
    },
    {
        {12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11},
        {10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8},
        { 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6},
        { 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13}
    },
    {
        { 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1},
        {13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6},
        { 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2},
        { 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12}
    },
    {
        {13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7},
        { 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2},
        { 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8},
        { 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11}
    }
};

uint8_t lay_bit(const uint8_t* mang, int pos) {
    int idx = (pos - 1) / 8;
    int shift = 7 - ((pos - 1) % 8);
    return (mang[idx] >> shift) & 1;
}

void gan_bit(uint8_t* mang, int pos, uint8_t val) {
    int idx = (pos - 1) / 8;
    int shift = 7 - ((pos - 1) % 8);
    if (val & 1) mang[idx] |= (1 << shift);
    else mang[idx] &= ~(1 << shift);
}

void hoan_vi(const uint8_t* vao, uint8_t* ra, const int* bang, int so_bit) {
    memset(ra, 0, (so_bit + 7) / 8);
    for (int i = 0; i < so_bit; ++i) {
        gan_bit(ra, i + 1, lay_bit(vao, bang[i]));
    }
}

void dich_trai_28(uint8_t half[4], int n) {
    for (int step = 0; step < n; ++step) {
        uint8_t msb = (half[0] >> 7) & 1;
        half[0] = (uint8_t)((half[0] << 1) | ((half[1] >> 7) & 1));
        half[1] = (uint8_t)((half[1] << 1) | ((half[2] >> 7) & 1));
        half[2] = (uint8_t)((half[2] << 1) | ((half[3] >> 7) & 1));
        half[3] = (uint8_t)((half[3] << 1) | (msb << 4));
    }
}

void sinh_khoa_con(const uint8_t khoa64[8], uint8_t subkey[16][6]) {
    uint8_t pc1_out[7];
    hoan_vi(khoa64, pc1_out, bang_pc1, 56);

    uint8_t c[4] = {0}, d[4] = {0};
    for (int i = 0; i < 28; ++i) {
        uint8_t b_c = lay_bit(pc1_out, i + 1);
        uint8_t b_d = lay_bit(pc1_out, i + 29);
        int idx = i / 8, sh = 7 - (i % 8);
        if (b_c) c[idx] |= (1 << sh);
        if (b_d) d[idx] |= (1 << sh);
    }

    for (int vong = 0; vong < 16; ++vong) {
        dich_trai_28(c, so_dich[vong]);
        dich_trai_28(d, so_dich[vong]);

        uint8_t cd56[7] = {0};
        for (int i = 0; i < 28; ++i) {
            int idx = i / 8, sh = 7 - (i % 8);
            gan_bit(cd56, i + 1, (c[idx] >> sh) & 1);
            gan_bit(cd56, i + 29, (d[idx] >> sh) & 1);
        }
        hoan_vi(cd56, subkey[vong], bang_pc2, 48);
    }
}

void ham_f(const uint8_t r32[4], const uint8_t k48[6], uint8_t out32[4]) {
    uint8_t exp48[6];
    hoan_vi(r32, exp48, bang_e, 48);

    uint8_t x48[6];
    for (int i = 0; i < 6; ++i) x48[i] = exp48[i] ^ k48[i];

    uint8_t s_out16[2] = {0};
    for (int b = 0; b < 8; ++b) {
        int base_bit = b * 6;
        int b0 = lay_bit(x48, base_bit + 1);
        int b1 = lay_bit(x48, base_bit + 2);
        int b2 = lay_bit(x48, base_bit + 3);
        int b3 = lay_bit(x48, base_bit + 4);
        int b4 = lay_bit(x48, base_bit + 5);
        int b5 = lay_bit(x48, base_bit + 6);

        int hang = (b0 << 1) | b5;
        int cot = (b1 << 3) | (b2 << 2) | (b3 << 1) | b4;
        uint8_t val = sbox_des[b][hang][cot];

        int out_bit = b * 4;
        gan_bit(s_out16, out_bit + 1, (val >> 3) & 1);
        gan_bit(s_out16, out_bit + 2, (val >> 2) & 1);
        gan_bit(s_out16, out_bit + 3, (val >> 1) & 1);
        gan_bit(s_out16, out_bit + 4, val & 1);
    }

    hoan_vi(s_out16, out32, bang_p, 32);
}

void in_hex(const char* nhan, const uint8_t* m, int len) {
    cout << nhan << ": ";
    for (int i = 0; i < len; ++i) {
        cout << hex << uppercase << setw(2) << setfill('0') << (int)m[i];
    }
    cout << "\n";
}

int main() {
    uint8_t pt[8]  = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    uint8_t key[8] = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};

    uint8_t subkey[16][6];
    sinh_khoa_con(key, subkey);

    cout << "=== KHOA CON 16 VONG (48-BIT) ===\n";
    for (int i = 0; i < 16; ++i) {
        char nhan[24];
        snprintf(nhan, sizeof(nhan), "K%02d", i + 1);
        in_hex(nhan, subkey[i], 6);
    }

    uint8_t ip_out[8];
    hoan_vi(pt, ip_out, bang_ip, 64);

    uint8_t l[4], r[4];
    memcpy(l, ip_out, 4);
    memcpy(r, ip_out + 4, 4);

    cout << "\n=== TRANG THAI TUNG VONG DES ===\n";
    in_hex("Plaintext ", pt, 8);
    in_hex("After IP  ", ip_out, 8);

    for (int vong = 0; vong < 16; ++vong) {
        uint8_t f_ra[4], r_moi[4];
        ham_f(r, subkey[vong], f_ra);
        for (int i = 0; i < 4; ++i) r_moi[i] = l[i] ^ f_ra[i];
        memcpy(l, r, 4);
        memcpy(r, r_moi, 4);

        uint8_t lr[8];
        memcpy(lr, l, 4);
        memcpy(lr + 4, r, 4);
        char nhan[32];
        snprintf(nhan, sizeof(nhan), "Round %2d (L,R)", vong + 1);
        in_hex(nhan, lr, 8);
    }

    // da sua: ghep R16 truoc, L16 sau roi moi qua IP^-1
    uint8_t sau_swap[8];
    memcpy(sau_swap, r, 4);
    memcpy(sau_swap + 4, l, 4);
    in_hex("32-bit swap", sau_swap, 8);

    uint8_t ct[8];
    hoan_vi(sau_swap, ct, bang_ip_inv, 64);
    in_hex("Ciphertext DES", ct, 8);
    cout << "Ky vong DES   : 85E813540F0AB405\n";
    return 0;
}
