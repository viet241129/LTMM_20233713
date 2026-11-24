#include <iostream>
#include <iomanip>
#include <cstdint>

using namespace std;

static const uint8_t sbox_chuan[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

static const uint8_t rcon_bang[11] = {
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36
};

uint8_t nhan_2(uint8_t x) {
    uint8_t bit_cao = x & 0x80;
    uint8_t kq = (uint8_t)(x << 1);
    if (bit_cao) {
        kq ^= 0x1B;
    }
    return kq;
}

uint8_t nhan_3(uint8_t x) {
    return (uint8_t)(nhan_2(x) ^ x);
}

void in_ma_tran(const char* nhan, uint8_t m[4][4]) {
    cout << "--- " << nhan << " ---\n";
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            cout << hex << uppercase << setw(2) << setfill('0') << (int)m[r][c] << " ";
        }
        cout << "\n";
    }
}

void in_hex_16(const char* nhan, const uint8_t b[16]) {
    cout << "--- " << nhan << " ---\n";
    for (int i = 0; i < 16; ++i) {
        cout << hex << uppercase << setw(2) << setfill('0') << (int)b[i];
    }
    cout << "\n";
}

// chuyen mang 16 byte theo thu tu cot (nhu chuan AES) sang ma tran 4x4 [hang][cot]
void nap_trang_thai(const uint8_t in[16], uint8_t st[4][4]) {
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            st[r][c] = in[4 * c + r];
        }
    }
}

void xuat_trang_thai(const uint8_t st[4][4], uint8_t out[16]) {
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            out[4 * c + r] = st[r][c];
        }
    }
}

void thay_the_byte(uint8_t st[4][4]) {
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            st[r][c] = sbox_chuan[st[r][c]];
        }
    }
}

// loi co y: hang 2 dang dich 1 thay vi 2
void dich_hang(uint8_t st[4][4]) {
    uint8_t tam[4];
    // hang 1: dich trai 1
    for (int c = 0; c < 4; ++c) tam[c] = st[1][(c + 1) % 4];
    for (int c = 0; c < 4; ++c) st[1][c] = tam[c];

    // hang 2: dang bi nham dich 1 (chuan la 2)
    for (int c = 0; c < 4; ++c) tam[c] = st[2][(c + 1) % 4];
    for (int c = 0; c < 4; ++c) st[2][c] = tam[c];

    // hang 3: dich trai 3
    for (int c = 0; c < 4; ++c) tam[c] = st[3][(c + 3) % 4];
    for (int c = 0; c < 4; ++c) st[3][c] = tam[c];
}

void tron_cot(uint8_t st[4][4]) {
    for (int c = 0; c < 4; ++c) {
        uint8_t s0 = st[0][c], s1 = st[1][c], s2 = st[2][c], s3 = st[3][c];
        st[0][c] = nhan_2(s0) ^ nhan_3(s1) ^ s2 ^ s3;
        st[1][c] = s0 ^ nhan_2(s1) ^ nhan_3(s2) ^ s3;
        st[2][c] = s0 ^ s1 ^ nhan_2(s2) ^ nhan_3(s3);
        st[3][c] = nhan_3(s0) ^ s1 ^ s2 ^ nhan_2(s3);
    }
}

void sinh_khoa_mo_rong(const uint8_t khoa[16], uint8_t w[44][4]) {
    for (int i = 0; i < 4; ++i) {
        w[i][0] = khoa[4 * i];
        w[i][1] = khoa[4 * i + 1];
        w[i][2] = khoa[4 * i + 2];
        w[i][3] = khoa[4 * i + 3];
    }
    for (int i = 4; i < 44; ++i) {
        uint8_t tam[4] = {w[i - 1][0], w[i - 1][1], w[i - 1][2], w[i - 1][3]};
        if (i % 4 == 0) {
            uint8_t rot[4] = {tam[1], tam[2], tam[3], tam[0]};
            tam[0] = sbox_chuan[rot[0]] ^ rcon_bang[i / 4];
            tam[1] = sbox_chuan[rot[1]];
            tam[2] = sbox_chuan[rot[2]];
            tam[3] = sbox_chuan[rot[3]];
        }
        for (int b = 0; b < 4; ++b) {
            w[i][b] = w[i - 4][b] ^ tam[b];
        }
    }
}

void cong_khoa_vong(uint8_t st[4][4], uint8_t w[44][4], int vong) {
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            st[r][c] ^= w[4 * vong + c][r];
        }
    }
}

void tinh_w4_khoa_0(uint8_t w4[4]) {
    uint8_t khoa0[16] = {0};
    uint8_t w[44][4];
    sinh_khoa_mo_rong(khoa0, w);
    for (int i = 0; i < 4; ++i) w4[i] = w[4][i];
}

int main() {
    // 1. Test vector trong vo
    uint8_t ma_tran_vo[4][4] = {
        {0x63, 0x63, 0x63, 0x63},
        {0x63, 0x63, 0x63, 0x63},
        {0x63, 0x82, 0x63, 0x63},
        {0x82, 0x63, 0x63, 0x63}
    };
    uint8_t kq_mc[4][4];
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) kq_mc[r][c] = ma_tran_vo[r][c];
    tron_cot(kq_mc);
    in_ma_tran("Vo - Sau MixColumns", kq_mc);

    uint8_t w4[4];
    tinh_w4_khoa_0(w4);
    cout << "--- Vo - w[4] khi khoa = 0 ---\n";
    for (int i = 0; i < 4; ++i) {
        cout << hex << uppercase << setw(2) << setfill('0') << (int)w4[i] << " ";
    }
    cout << "\n\n";

    // 2. Test vector slide AES-128
    uint8_t pt[16] = {
        0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,
        0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10
    };
    uint8_t key[16] = {
        0x0f,0x15,0x71,0xc9,0x47,0xd9,0xe8,0x59,
        0x0c,0xb7,0xad,0xd6,0xaf,0x7f,0x67,0x98
    };
    uint8_t w[44][4];
    sinh_khoa_mo_rong(key, w);

    uint8_t st[4][4];
    nap_trang_thai(pt, st);
    in_ma_tran("AES-128 Trang thai dau (Plaintext)", st);
    cong_khoa_vong(st, w, 0);
    in_ma_tran("AES-128 Sau AddRoundKey vong 0", st);

    for (int round = 1; round <= 10; ++round) {
        thay_the_byte(st);
        dich_hang(st);
        if (round < 10) {
            tron_cot(st);
        }
        cong_khoa_vong(st, w, round);

        if (round == 1 || round == 10) {
            char nhan[64];
            snprintf(nhan, sizeof(nhan), "AES-128 Trang thai cuoi vong %d", round);
            in_ma_tran(nhan, st);
        }
    }

    uint8_t ct[16];
    xuat_trang_thai(st, ct);
    in_hex_16("AES-128 Ciphertext (chua sua ShiftRows)", ct);
    cout << "Ky vong slide: FF0B844A0853BF7C6934AB4364148FB9\n";
    return 0;
}
