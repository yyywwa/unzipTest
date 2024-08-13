#include "FileFormatValidator.h"
#include <fstream>
#include <iostream>
#include <unordered_map>

std::string FileFormatValidator::getFileExtension(const std::string &filePath) {
  return filePath.substr(filePath.find_last_of(".") + 1);
}

bool FileFormatValidator::validateByExtension(
    const std::string &filePath, const std::string &expectedMagic) {
  std::ifstream file(filePath, std::ios::binary);
  if (!file.is_open())
    return false;

  char magic[16]{};
  file.read(magic, expectedMagic.size());

  return std::string(magic, expectedMagic.size()) == expectedMagic;
}

bool FileFormatValidator::validateFormat(const std::string &filePath) {
  std::string ext = getFileExtension(filePath);
  static const std::unordered_map<std::string, std::string> magicNumbers = {
      {"gz", "\x1F\x8B\x08\x00"},  // gzip magic number
      {"zip", "PK\x03\x04"},       // zip magic number
      {"rar", "Rar!"},             // rar magic number
      {"7z", "7z\xBC\xAF\x27\x1C"} // 7z magic number
  };

  if (auto it = magicNumbers.find(ext); it != magicNumbers.end()) {
    return validateByExtension(filePath, it->second);
  }

  return false;
}

bool FileFormatValidator::isGltfValid(const std::string &filePath) {
  // A more specific validation can be implemented for GLTF
  return validateByExtension(filePath, "glTF");
}
