#include "FileFormatValidator.h"
#include <fstream>
#include <iostream>

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
  auto fun = [&filePath](const std::string &magic) -> bool {
    return validateByExtension(filePath, magic);
  };
  if (ext == "gz") {
    return fun("\x1F\x8B\x08\x00"); // gzip magic number
  } else if (ext == "zip") {
    return fun("PK\x03\x04"); // zip magic number
  } else if (ext == "rar") {
    return fun("Rar!"); // rar magic number
  } else if (ext == "7z") {
    return fun("7z\xBC\xAF\x27\x1C"); // 7z magic number
  }
  return false;
}

bool FileFormatValidator::isGltfValid(const std::string &filePath) {
  // A more specific validation can be implemented for GLTF
  return validateByExtension(filePath, "glTF");
}
