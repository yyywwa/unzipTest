#include "ArchiveManager.h"
#include "FileFormatValidator.h"
#include <iostream>

int main() {
  using namespace std::literals;
  auto filePath = "/home/ike/magic/x.zip"s;
  auto destFolder = "/home/ike/magic"s;

  if (!FileFormatValidator::validateFormat(filePath)) {
    std::cerr << "File format does not match the extension!" << std::endl;
    return 1;
  }

  auto contents = ArchiveManager::listContents(filePath);
  std::cout << "Contents of the archive:" << std::endl;
  for (const auto &file : contents) {
    std::cout << file << std::endl;
  }

  if (ArchiveManager::extractToFolder(filePath, destFolder)) {
    std::cout << "Extraction successful!" << std::endl;
  } else {
    std::cerr << "Extraction failed!" << std::endl;
  }

  return 0;
}
