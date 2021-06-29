#include <fstream>
#include "cryptopp850/sha.h"
#include "cryptopp850/osrng.h"
#include "cryptopp850/hex.h"
#include "cryptopp850/files.h"

int main() {
  CryptoPP::byte zero[CryptoPP::SHA256::DIGESTSIZE];
  memset(zero, 0, CryptoPP::SHA256::DIGESTSIZE);

  CryptoPP::SHA256 hash;
  CryptoPP::byte digest[CryptoPP::SHA256::DIGESTSIZE];
  CryptoPP::AutoSeededRandomPool prng;
  CryptoPP::byte nonce[4];
  CryptoPP::byte preimage[CryptoPP::SHA256::DIGESTSIZE + 4];

  CryptoPP::HexEncoder hex_encoder;
  std::fstream file;
  file.open("out.txt", std::fstream::out | std::fstream::trunc);
  hex_encoder.Attach(new CryptoPP::FileSink(file));
  CryptoPP::HexDecoder hex_decoder;
  std::string hex_string;

  // Part 1
  std::string msg = "Bitcoin is a cryptocurrency, a form of electronic cash.";
  hash.Update((const CryptoPP::byte *) msg.data(), msg.length());
  hash.Final(digest);
  hex_encoder.PutMessageEnd(digest, CryptoPP::SHA256::DIGESTSIZE);
  file << '\n';
  //

  msg = "Bitcoin";
  hash.Update((const CryptoPP::byte *) msg.data(), msg.length());
  hash.Final(digest);

  file << 0 << "\n";
  hex_encoder.PutMessageEnd(digest, CryptoPP::SHA256::DIGESTSIZE);
  file << "\n";

  msg = "00000000";
  hex_decoder.Attach(new CryptoPP::StringSink(hex_string));
  hex_decoder.PutMessageEnd((const CryptoPP::byte *) msg.data(), msg.length());
  memcpy(preimage, digest, CryptoPP::SHA256::DIGESTSIZE);
  memcpy(preimage + CryptoPP::SHA256::DIGESTSIZE, hex_string.data(), 4);
  hex_decoder.Detach();
  hex_string.clear();
  hash.Update(preimage, CryptoPP::SHA256::DIGESTSIZE + 4);
  hash.Final(digest);

  file << msg << "\n";
  hex_encoder.PutMessageEnd(digest, CryptoPP::SHA256::DIGESTSIZE);
  file << "\n";
  file << 1 << "\n";
  hex_encoder.PutMessageEnd(digest, CryptoPP::SHA256::DIGESTSIZE);
  file << "\n";

  msg = "0000000A";
  hex_decoder.Attach(new CryptoPP::StringSink(hex_string));
  hex_decoder.PutMessageEnd((const CryptoPP::byte *) msg.data(), msg.length());
  memcpy(preimage, digest, CryptoPP::SHA256::DIGESTSIZE);
  memcpy(preimage + CryptoPP::SHA256::DIGESTSIZE, hex_string.data(), 4);
  hex_decoder.Detach();
  hex_string.clear();
  hash.Update(preimage, CryptoPP::SHA256::DIGESTSIZE + 4);
  hash.Final(digest);

  file << msg << "\n";
  hex_encoder.PutMessageEnd(digest, CryptoPP::SHA256::DIGESTSIZE);
  file << "\n";

  for (uint8_t num = 2; num < 10; ++num) {
    file << +num << "\n";
    hex_encoder.PutMessageEnd(digest, CryptoPP::SHA256::DIGESTSIZE);
    file << "\n";

    memcpy(preimage, digest, CryptoPP::SHA256::DIGESTSIZE);
    do {
      prng.GenerateBlock(nonce, 4);
      memcpy(preimage + CryptoPP::SHA256::DIGESTSIZE, nonce, 4);

      hash.Update(preimage, CryptoPP::SHA256::DIGESTSIZE + 4);
      hash.Final(digest);
    } while (memcmp(digest, zero, num >> 1) != 0 || ((num % 2) && ((digest[(num >> 1)] >> 4) ^ zero[num >> 1]) != 0));

    hex_encoder.PutMessageEnd(nonce, 4);
    file << "\n";
    hex_encoder.PutMessageEnd(digest, CryptoPP::SHA256::DIGESTSIZE);
    file << "\n";
  }

  hex_encoder.Detach();
  file.close();
  return 0;
}
