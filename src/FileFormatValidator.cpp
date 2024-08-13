#include "FileFormatValidator.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

std::unique_ptr<FileFormatValidator> FileFormatValidator::ptr_;
std::unique_ptr<FileFormatValidator::MapType>
    FileFormatValidator::magicNumbers_ptr_;
FileFormatValidator &FileFormatValidator::instanse() {
  if (ptr_.get() == nullptr) {
    class Helper : public FileFormatValidator {};
    ptr_ = std::make_unique<Helper>();
    magicNumbers_ptr_ = std::make_unique<MapType>();

    magicNumbers_ptr_->insert({"gz", "\x1F\x8B\x08\x00"}); // gzip magic number
    magicNumbers_ptr_->insert({"zip", "PK\x03\x04"});      // zip magic number
    magicNumbers_ptr_->insert({"rar", "Rar!"});            // rar magic number
    magicNumbers_ptr_->insert({"7z", "7z\xBC\xAF\x27\x1C"}); // 7z magic number
  }
  return *(ptr_.get());
}

void FileFormatValidator::registFileMagicNum(
    std::pair<std::string, std::string> pair) {
  magicNumbers_ptr_->insert(pair);
}

std::string FileFormatValidator::getFileExtension(const std::string &filePath) {
  return filePath.substr(filePath.find_last_of(".") + 1);
}

bool FileFormatValidator::validateByExtension(
    const std::string &filePath, const std::string &expectedMagic) {
  std::ifstream file(filePath, std::ios::binary);
  if (!file.is_open())
    return false;

  char magic[32]{};
  file.read(magic, expectedMagic.size());

  return std::string(magic, expectedMagic.size()) == expectedMagic;
}

bool FileFormatValidator::validateFormat(const std::string &filePath) {
  std::string ext = getFileExtension(filePath);
  auto &magicNumbers = *magicNumbers_ptr_;

  if (auto it = magicNumbers.find(ext); it != magicNumbers.end()) {
    return validateByExtension(filePath, it->second);
  }

  return false;
}