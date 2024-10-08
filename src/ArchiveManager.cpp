#include "ArchiveManager.h"
#include <archive.h>
#include <archive_entry.h>
#include <iostream>

std::vector<std::string>
ArchiveManager::listContents(const std::string &archivePath) {
  std::vector<std::string> fileList;
  struct archive *a = archive_read_new();
  archive_read_support_format_all(a);
  archive_read_support_filter_all(a);

  if (archive_read_open_filename(a, archivePath.c_str(), 10240) == ARCHIVE_OK) {
    struct archive_entry *entry;
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
      fileList.push_back(archive_entry_pathname(entry));
      archive_read_data_skip(a);
    }
    archive_read_free(a);
  } else {
    std::cerr << "Failed to open archive." << std::endl;
  }
  return fileList;
}

bool ArchiveManager::extractToFolder(const std::string &archivePath,
                                     const std::string &outputDir) {
  struct archive *a;
  struct archive *ext;
  struct archive_entry *entry;
  int flags;
  int r;

  // Select which attributes we want to restore.
  flags = ARCHIVE_EXTRACT_TIME;
  flags |= ARCHIVE_EXTRACT_PERM;
  flags |= ARCHIVE_EXTRACT_ACL;
  flags |= ARCHIVE_EXTRACT_FFLAGS;

  a = archive_read_new();
  archive_read_support_format_all(a);
  archive_read_support_filter_all(a);
  ext = archive_write_disk_new();
  archive_write_disk_set_options(ext, flags);
  archive_write_disk_set_standard_lookup(ext);

  if ((r = archive_read_open_filename(a, archivePath.c_str(), 10240)) !=
      ARCHIVE_OK) {
    std::cerr << "Failed to open archive: " << archive_error_string(a)
              << std::endl;
    return false;
  }

  while (true) {
    r = archive_read_next_header(a, &entry);
    if (r == ARCHIVE_EOF)
      break;
    if (r < ARCHIVE_OK)
      std::cerr << archive_error_string(a) << std::endl;
    if (r < ARCHIVE_WARN)
      return false;

    // Construct the full output path.
    std::string currentFile = archive_entry_pathname(entry);
    std::string fullOutputPath = outputDir + "/" + currentFile;

    // Update the output path in the archive entry.
    archive_entry_set_pathname(entry, fullOutputPath.c_str());

    r = archive_write_header(ext, entry);
    if (r < ARCHIVE_OK)
      std::cerr << archive_error_string(ext) << std::endl;
    else if (archive_entry_size(entry) > 0) {
      r = copyData(a, ext);
      if (r < ARCHIVE_OK)
        std::cerr << archive_error_string(ext) << std::endl;
      if (r < ARCHIVE_WARN)
        return false;
    }
    r = archive_write_finish_entry(ext);
    if (r < ARCHIVE_OK)
      std::cerr << archive_error_string(ext) << std::endl;
    if (r < ARCHIVE_WARN)
      return false;
  }

  archive_read_close(a);
  archive_read_free(a);
  archive_write_close(ext);
  archive_write_free(ext);

  return true;
}

int ArchiveManager::copyData(struct archive *ar, struct archive *aw) {
  int r;
  const void *buff;
  size_t size;
  la_int64_t offset;

  while (true) {
    r = archive_read_data_block(ar, &buff, &size, &offset);
    if (r == ARCHIVE_EOF)
      return ARCHIVE_OK;
    if (r < ARCHIVE_OK)
      return r;

    r = archive_write_data_block(aw, buff, size, offset);
    if (r < ARCHIVE_OK) {
      std::cerr << archive_error_string(aw) << std::endl;
      return r;
    }
  }
}