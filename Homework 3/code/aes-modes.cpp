#include <iostream>
#include <fstream>
#include <iomanip>

#include "cryptopp850/modes.h"
#include "cryptopp850/aes.h"
#include "cryptopp850/filters.h"

std::string hex_string_conversion(const std::string &hex_string) {
  std::string result;
  for (int i = 0; i < hex_string.length(); i += 2) {
    result += (char) stoul(hex_string.substr(i, 2), nullptr, 16);
  }
  return result;
}

int main() {

  std::fstream out;
  out.open("Out.txt", std::fstream::out | std::fstream::trunc);

  std::string plaintext = "AES is the block cipher standard.";
  std::string ciphertext;

  std::string key_string = "1234567890ABCDEF";
  CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];
  memcpy(key, key_string.c_str(), CryptoPP::AES::DEFAULT_KEYLENGTH);
  CryptoPP::AES::Encryption AES_Encryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);

  std::string IV_string;
  CryptoPP::byte IV[CryptoPP::AES::BLOCKSIZE];

  //--------------------------------------------------------------------------------------------------------------------//
  IV_string = "0000000000000000";
  memcpy(IV, IV_string.c_str(), CryptoPP::AES::BLOCKSIZE);
  CryptoPP::CFB_Mode_ExternalCipher::Encryption AES_CFB(AES_Encryption, IV, 4);
  CryptoPP::StreamTransformationFilter AES_CFB_Encryptor(AES_CFB, new CryptoPP::StringSink(ciphertext));
  AES_CFB_Encryptor.Put(reinterpret_cast<const unsigned char *>( plaintext.c_str()), plaintext.length());
  AES_CFB_Encryptor.MessageEnd();

  //std::cout << "Cipher Text (" << ciphertext.size() << " bytes)" << std::endl;
  for (int i = 0; i < ciphertext.size(); i++) {
    out << std::setw(2) << std::setfill('0') << std::hex << (0xFF & static_cast<CryptoPP::byte>(ciphertext[i])) << " ";
  }
  out << std::endl;
  ciphertext.clear();
  //--------------------------------------------------------------------------------------------------------------------//

  //--------------------------------------------------------------------------------------------------------------------//
  IV_string = "0000000000000000";
  memcpy(IV, IV_string.c_str(), CryptoPP::AES::BLOCKSIZE);
  CryptoPP::CBC_Mode_ExternalCipher::Encryption AES_CBC_1(AES_Encryption, IV);
  CryptoPP::StreamTransformationFilter AES_CBC_1_Encryptor
      (AES_CBC_1, new CryptoPP::StringSink(ciphertext), CryptoPP::BlockPaddingSchemeDef::ZEROS_PADDING);
  AES_CBC_1_Encryptor.Put(reinterpret_cast<const unsigned char *>( plaintext.c_str()), plaintext.length());
  AES_CBC_1_Encryptor.MessageEnd();

  //std::cout << "Cipher Text (" << ciphertext.size() << " bytes)" << std::endl;
  for (int i = 0; i < ciphertext.size(); i++) {
    out << std::setw(2) << std::setfill('0') << std::hex << (0xFF & static_cast<CryptoPP::byte>(ciphertext[i])) << " ";
  }
  out << std::endl;
  ciphertext.clear();
  //--------------------------------------------------------------------------------------------------------------------//

  //--------------------------------------------------------------------------------------------------------------------//
  IV_string = "9999999999999999";
  memcpy(IV, IV_string.c_str(), CryptoPP::AES::BLOCKSIZE);
  CryptoPP::CBC_Mode_ExternalCipher::Encryption AES_CBC_2(AES_Encryption, IV);
  CryptoPP::StreamTransformationFilter AES_CBC_2_Encryptor
      (AES_CBC_2, new CryptoPP::StringSink(ciphertext), CryptoPP::BlockPaddingSchemeDef::PKCS_PADDING);
  AES_CBC_2_Encryptor.Put(reinterpret_cast<const unsigned char *>( plaintext.c_str()), plaintext.length());
  AES_CBC_2_Encryptor.MessageEnd();

  //std::cout << "Cipher Text (" << ciphertext.size() << " bytes)" << std::endl;
  for (int i = 0; i < ciphertext.size(); i++) {
    out << std::setw(2) << std::setfill('0') << std::hex << (0xFF & static_cast<CryptoPP::byte>(ciphertext[i])) << " ";
  }
  out << std::endl;
  ciphertext.clear();
  //--------------------------------------------------------------------------------------------------------------------//

  //--------------------------------------------------------------------------------------------------------------------//
  CryptoPP::ECB_Mode_ExternalCipher::Encryption AES_EBC(AES_Encryption);
  CryptoPP::StreamTransformationFilter
      AES_EBC_Encryptor(AES_EBC, new CryptoPP::StringSink(ciphertext), CryptoPP::BlockPaddingSchemeDef::PKCS_PADDING);
  AES_EBC_Encryptor.Put(reinterpret_cast<const unsigned char *>( plaintext.c_str()), plaintext.length());
  AES_EBC_Encryptor.MessageEnd();

  //std::cout << "Cipher Text (" << ciphertext.size() << " bytes)" << std::endl;
  for (int i = 0; i < ciphertext.size(); i++) {
    out << std::setw(2) << std::setfill('0') << std::hex << (0xFF & static_cast<CryptoPP::byte>(ciphertext[i])) << " ";
  }
  out << std::endl;
  ciphertext.clear();
  //--------------------------------------------------------------------------------------------------------------------//

  /*
  //ciphertext = hex_string_conversion("");
  std::string decryptedtext;
  CryptoPP::AES::Decryption AES_Decryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);

  CryptoPP::ECB_Mode_ExternalCipher::Decryption AES_EBC_D(AES_Decryption);
  CryptoPP::StreamTransformationFilter AES_EBC_Decryptor
      (AES_EBC_D, new CryptoPP::StringSink(decryptedtext), CryptoPP::BlockPaddingSchemeDef::PKCS_PADDING);
  AES_EBC_Decryptor.Put(reinterpret_cast<const unsigned char *>( ciphertext.c_str()), ciphertext.length());
  AES_EBC_Decryptor.MessageEnd();

  std::cout << "Decrypted Text: " << std::endl;
  std::cout << decryptedtext;
  std::cout << std::endl;
  */

  return 0;
}