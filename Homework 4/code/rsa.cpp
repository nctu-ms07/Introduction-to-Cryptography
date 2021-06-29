#include <iostream>
#include <fstream>

#include "cryptopp850/rsa.h"

using std::cout;
using std::hex;
using std::endl;

std::string hex_string_conversion(const std::string &hex_string) {
  std::string result;
  for (int i = 0; i < hex_string.length(); i += 2) {
    result += (char) stoul(hex_string.substr(i, 2), nullptr, 16);
  }
  return result;
}

int main() {
  std::fstream out;
  out.open("out.txt", std::fstream::out | std::fstream::trunc);

  CryptoPP::Integer n, e, d, r;
  CryptoPP::RSA::PublicKey public_key;
  CryptoPP::RSA::PrivateKey private_key;
  std::string message;
  std::string cipher;

  message = "Alice";
  n = CryptoPP::Integer("0xb14022eef719f1bb");
  e = CryptoPP::Integer("0x11");
  public_key.Initialize(n, e);
  cout << hex << public_key.ApplyFunction(CryptoPP::Integer((const CryptoPP::byte *) message.c_str(), message.size()))
       << endl;

  message = "Hello World!";
  n = CryptoPP::Integer("0xb2c8d1404ed5fc2f7ad1254bb428f0d5");
  e = CryptoPP::Integer("0x11");
  public_key.Initialize(n, e);
  out << hex << public_key.ApplyFunction(CryptoPP::Integer((const CryptoPP::byte *) message.c_str(), message.size()))
      << endl;

  message = "RSA is public key.";
  n = CryptoPP::Integer("0xcf625a8e47bc1cf9a3b517b31d870108c0cd97466003842a3b394d6cd857e9b7");
  e = CryptoPP::Integer("0x10001");
  public_key.Initialize(n, e);
  out << hex << public_key.ApplyFunction(CryptoPP::Integer((const CryptoPP::byte *) message.c_str(), message.size()))
      << endl;

  cipher = hex_string_conversion("a1676afd68a2fc67dac32c633600b76fa90aca9f9cca5201490a20c8b01a061a");
  n = CryptoPP::Integer("0xd6361e40b2d619970ead338912a273adb75a4ce21356304834753fe94e6de24b");
  d = CryptoPP::Integer("0x12e6a85100b889c9905a939b274a91bc57ca85d52e6c464fb455c86a29d63c89");
  public_key.Initialize(n, d);
  r = public_key.ApplyFunction(CryptoPP::Integer((const CryptoPP::byte *) cipher.c_str(), cipher.size()));
  message.resize(r.MinEncodedSize());
  r.Encode((CryptoPP::byte *) message.data(), message.size());
  out << message << endl;

  CryptoPP::Integer c("0xa1676afd68a2fc67dac32c633600b76fa90aca9f9cca5201490a20c8b01a061a");
  n = CryptoPP::Integer("0xd6361e40b2d619970ead338912a273adb75a4ce21356304834753fe94e6de24b");
  e = CryptoPP::Integer("11b");
  public_key.Initialize(n, e);
  while (public_key.ApplyFunction(CryptoPP::Integer((const CryptoPP::byte *) message.c_str(), message.size())) != c) {
    e = ((e - 1) << 1) + 1;
    public_key.Initialize(n, e);
  }
  out << hex << e;

  return 0;
}
