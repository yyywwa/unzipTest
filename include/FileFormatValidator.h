#pragma once
#include <string>

class FileFormatValidator {
public:
  static bool validateFormat(const std::string &filePath);
  static bool isGltfValid(const std::string &filePath);

private:
  static std::string getFileExtension(const std::string &filePath);
  static bool validateByExtension(const std::string &filePath,
                                  const std::string &expectedMagic);
};
