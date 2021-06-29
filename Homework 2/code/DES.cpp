#include <iostream>
#include <iomanip>
#include <bitset>
#include <string>
#include <ctime>
#include <fstream>

using namespace std;

bitset<48> subKeys[16];

int permuted_choice_1[] = {57, 49, 41, 33, 25, 17, 9,
                           1, 58, 50, 42, 34, 26, 18,
                           10, 2, 59, 51, 43, 35, 27,
                           19, 11, 3, 60, 52, 44, 36,
                           63, 55, 47, 39, 31, 23, 15,
                           7, 62, 54, 46, 38, 30, 22,
                           14, 6, 61, 53, 45, 37, 29,
                           21, 13, 5, 28, 20, 12, 4};

int permuted_choice_2[] = {14, 17, 11, 24, 1, 5,
                           3, 28, 15, 6, 21, 10,
                           23, 19, 12, 4, 26, 8,
                           16, 7, 27, 20, 13, 2,
                           41, 52, 31, 37, 47, 55,
                           30, 40, 51, 45, 33, 48,
                           44, 49, 39, 56, 34, 53,
                           46, 42, 50, 36, 29, 32};

int shiftLeftSchedule[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

template<std::size_t N>
inline void rotate(std::bitset<N> &b, unsigned m) {
  b = b >> m | b << (N - m);
}

void generateKeys(bitset<64> masterKey) {
  bitset<56> permuted;
  for (int i = 0; i < 56; i++) {
    permuted[i] = masterKey[permuted_choice_1[i] - 1];
  }

  bitset<28> C, D;
  for (int i = 0; i < 28; i++) {
    C[i] = permuted[i];
    D[i] = permuted[i + 28];
  }

  for (int round = 0; round < 16; round++) {
    rotate(C, shiftLeftSchedule[round]);
    rotate(D, shiftLeftSchedule[round]);

    for (int i = 0; i < 28; i++) {
      permuted[i] = C[i];
      permuted[i + 28] = D[i];
    }

    bitset<48> subKey;
    for (int i = 0; i < 48; i++) {
      subKey[i] = permuted[permuted_choice_2[i] - 1];
    }

    subKeys[round] = subKey;
  }
}

int expansion_permutation[] = {32, 1, 2, 3, 4, 5,
                               4, 5, 6, 7, 8, 9,
                               8, 9, 10, 11, 12, 13,
                               12, 13, 14, 15, 16, 17,
                               16, 17, 18, 19, 20, 21,
                               20, 21, 22, 23, 24, 25,
                               24, 25, 26, 27, 28, 29,
                               28, 29, 30, 31, 32, 1};

int S_box[8][4][16] = {
    {
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
    },
    {
        {15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
    },
    {
        {10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
    },
    {
        {7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
    },
    {
        {2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
    },
    {
        {12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
    },
    {
        {4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
    },
    {
        {13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
    }
};

int permutation[] = {16, 7, 20, 21,
                     29, 12, 28, 17,
                     1, 15, 23, 26,
                     5, 18, 31, 10,
                     2, 8, 24, 14,
                     32, 27, 3, 9,
                     19, 13, 30, 6,
                     22, 11, 4, 25};

bitset<32> F(bitset<32> R, bitset<48> K) {
  bitset<48> R_expand;
  for (int i = 0; i < 48; ++i) {
    R_expand[i] = R[expansion_permutation[i] - 1];
  }

  R_expand = R_expand ^ K;
  for (int i = 0; i < 8; i++) {
    int row = R_expand[i * 6] * 2 + R_expand[i * 6 + 5];
    int column = R_expand[i * 6 + 1] * 8 + R_expand[i * 6 + 2] * 4 + R_expand[i * 6 + 3] * 2 + R_expand[i * 6 + 4];
    int value = S_box[i][row][column];
    bitset<4> binary(value);
    for (int j = 0; j < 4; j++) {
      R[i * 4 + j] = binary[3 - j];
    }
  }

  bitset<32> output;
  for (int i = 0; i < 32; ++i) {
    output[i] = R[permutation[i] - 1];
  }
  return output;
}

int IP[] = {58, 50, 42, 34, 26, 18, 10, 2,
            60, 52, 44, 36, 28, 20, 12, 4,
            62, 54, 46, 38, 30, 22, 14, 6,
            64, 56, 48, 40, 32, 24, 16, 8,
            57, 49, 41, 33, 25, 17, 9, 1,
            59, 51, 43, 35, 27, 19, 11, 3,
            61, 53, 45, 37, 29, 21, 13, 5,
            63, 55, 47, 39, 31, 23, 15, 7};

int IP_1[] = {40, 8, 48, 16, 56, 24, 64, 32,
              39, 7, 47, 15, 55, 23, 63, 31,
              38, 6, 46, 14, 54, 22, 62, 30,
              37, 5, 45, 13, 53, 21, 61, 29,
              36, 4, 44, 12, 52, 20, 60, 28,
              35, 3, 43, 11, 51, 19, 59, 27,
              34, 2, 42, 10, 50, 18, 58, 26,
              33, 1, 41, 9, 49, 17, 57, 25};

bitset<64> encrypt(bitset<64> plain) {
  bitset<64> permuted;
  bitset<32> left, right;

  for (int i = 0; i < 64; ++i) {
    permuted[i] = plain[IP[i] - 1];
  }

  for (int i = 0; i < 32; i++) {
    left[i] = permuted[i];
    right[i] = permuted[i + 32];
  }

  for (int round = 0; round < 16; round++) {
    bitset<32> newLeft = right;
    right = left ^ F(right, subKeys[round]);
    left = newLeft;
  }

  for (int i = 0; i < 32; i++) {
    permuted[i] = right[i];
    permuted[i + 32] = left[i];
  }

  bitset<64> cipher;
  for (int i = 0; i < 64; i++) {
    cipher[i] = permuted[IP_1[i] - 1];
  }

  return cipher;
}

bitset<64> stringToBitSet(const string &s) {
  bitset<64> bits;
  for (int i = 0; i < 8; i++) {
    bitset<8> c_bit(s[i]);
    for (int j = 0; j < 8; j++) {
      bits[i * 8 + j] = c_bit[7 - j];
    }
  }
  return bits;
}

int main() {
  fstream in;
  in.open("DES-Key-Plaintext.txt", fstream::in);

  fstream out;
  out.open("out.txt", fstream::out | fstream::trunc);

  clock_t start, end;
  start = clock();
  int n = 5;
  while (n--) {
    string key, plain_text;
    bitset<64> cipher;
    in >> key >> plain_text;

    for (int i = 0; i < 1000; i++) {
      generateKeys(stringToBitSet(key));
      cipher = encrypt(stringToBitSet(plain_text));
    }

    string cipher_str = cipher.to_string();
    reverse(cipher_str.begin(), cipher_str.end());
    out << setw(16) << setfill('0') << hex << uppercase << stoull(cipher_str, nullptr, 2) << endl;
  }
  end = clock();
  out << ((double) (end - start)) / (CLOCKS_PER_SEC * 5);

  in.close();
  out.close();
  return 0;
}