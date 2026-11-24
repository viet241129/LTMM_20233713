# Bai tap ve nha - Ly thuyet mat ma (2026.1)

- Sinh vien: Nguyen Tuan Viet
- MSSV: 20233713

## Cau truc thu muc
- aes128/aes_test.cpp: Cai dat AES-128, kiem tra MixColumns va w[4] trong vo, test vector slide Chuong 2.
- aes128/ket_qua_aes128.txt: Log gia tri tung trang thai qua 10 vong AES-128.
- des/des_test.cpp: Cai dat DES (IP, sinh khoa con, ham F, S-box, hoan vi P, swap 32-bit, IP^-1).
- des/ket_qua_des.txt: Log khoa con va trang thai (L, R) qua 16 vong DES.

## Cach bien dich va chay
Bien dich AES-128:
g++ -std=c++17 aes128/aes_test.cpp -o aes128/aes_test && ./aes128/aes_test

Bien dich DES:
g++ -std=c++17 des/des_test.cpp -o des/des_test && ./des/des_test
