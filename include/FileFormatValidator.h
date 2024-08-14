#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class FileFormatValidator {
public:
  bool validateFormat(const std::string &filePath);
  bool isGltfValid(const std::string &filePath);
  static FileFormatValidator &instanse();
  void registFileMagicNum(std::pair<std::string, std::string>);

  std::vector<std::string>
  findFilesWithExtension(const std::vector<std::string> &files,
                         const std::string &extension);

private:
  FileFormatValidator(){};
  std::string getFileExtension(const std::string &filePath);
  bool validateByExtension(const std::string &filePath,
                           const std::string &expectedMagic);
  using MapType = std::unordered_map<std::string, std::string>;
  static std::unique_ptr<MapType> magicNumbers_ptr_;
  static std::unique_ptr<FileFormatValidator> ptr_;
};
