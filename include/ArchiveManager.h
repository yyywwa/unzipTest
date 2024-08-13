#pragma once
#include <string>
#include <vector>

class ArchiveManager {
public:
  static std::vector<std::string> listContents(const std::string &archivePath);
  static bool extractToFolder(const std::string &archivePath,
                              const std::string &destFolder);

private:
  // 复制数据块，从压缩文件读取并写入目标目录
  static int copyData(struct archive *archive, struct archive *outArchive);
};
