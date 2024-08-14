#include "ArchiveManager.h"
#include "FileFormatValidator.h"
#include <iostream>
#include <vector>

int work() {
  using namespace std::literals;
  auto filePath = "/home/ike/magic/x.zip"s;
  auto destFolder = "/home/ike/magic"s;

  FileFormatValidator::instanse().registFileMagicNum({"zip", "PK\x03\x04"});
  if (!FileFormatValidator::instanse().validateFormat(filePath)) {
    return 1;
  }

  auto contents = ArchiveManager::listContents(filePath);
  std::cout << "Contents of the archive:" << std::endl;
  for (const auto &file : contents) {
    std::cout << file << std::endl;
  }

  std::cout << "----------\n";
  auto files =
      FileFormatValidator::instanse().findFilesWithExtension(contents, "txt");
  for (const auto &file : files) {
    std::cout << file << '\n';
  }
  std::cout << "----------\n";

  if (ArchiveManager::extractToFolder(filePath, destFolder)) {
    std::cout << "Extraction successful!" << std::endl;
  } else {
    std::cerr << "Extraction failed!" << std::endl;
  }
  return 0;
}
int main() { work(); }
