#include "file.h"

#include <dirent.h>
#include <fcntl.h>
#include <glob.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cerrno>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/dll.hpp>
#include <boost/filesystem.hpp>

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

#include "flags.h"
#include "spd_log_helper.h"
namespace {
#if defined(_WIN32)
const wchar_t separator = L'/';
const wchar_t *const separators = L"/\\";
const wchar_t *separator_string = L"/";
const wchar_t *preferred_separator_string = L"\\";
const wchar_t colon = L':';
const wchar_t dot = L'.';
const wchar_t questionmark = L'?';
// const fs::path dot_path(L".");
// const fs::path dot_dot_path(L"..");

inline bool is_letter(wchar_t c) {
  return (c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z');
}

#else

// const char separator = '/';
// const char *const separators = "/";
// const char dot = '.';
#endif
}  // namespace

using std::istreambuf_iterator;
using std::string;
using std::vector;

#ifdef PROTOBUF_FOUND
bool SetProtoToASCIIFile(const google::protobuf::Message &message, int file_descriptor) {
  using google::protobuf::TextFormat;
  using google::protobuf::io::FileOutputStream;
  using google::protobuf::io::ZeroCopyOutputStream;
  if (file_descriptor < 0) {
    std::cerr << "Invalid file descriptor.";
    return false;
  }
  ZeroCopyOutputStream *output = new FileOutputStream(file_descriptor);
  bool success = TextFormat::Print(message, output);
  delete output;
  close(file_descriptor);
  return success;
}

bool SetProtoToASCIIFile(const google::protobuf::Message &message, const std::string &file_name) {
  int fd = open(file_name.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
  if (fd < 0) {
    std::cerr << "Unable to open file " << file_name << " to write.";
    return false;
  }
  return SetProtoToASCIIFile(message, fd);
}

bool GetProtoFromASCIIFile(const std::string &file_name, google::protobuf::Message *message) {
  using google::protobuf::TextFormat;
  using google::protobuf::io::FileInputStream;
  using google::protobuf::io::ZeroCopyInputStream;
  int file_descriptor = open(file_name.c_str(), O_RDONLY);
  if (file_descriptor < 0) {
    LOG_ERROR("Failed to open file {} in text mode.", file_name);
    // Failed to open;
    return false;
  }

  ZeroCopyInputStream *input = new FileInputStream(file_descriptor);
  bool success = TextFormat::Parse(input, message);
  if (!success) {
    // std::cerr << "Failed to parse file " << file_name << " as text proto.";
    LOG_ERROR("Failed to parse file {} as text proto.", file_name);
  }
  delete input;
  close(file_descriptor);
  return success;
}

bool GetProtoFromString(const std::string &file_name, google::protobuf::Message *message) {
  using google::protobuf::TextFormat;
  // using google::protobuf::io::FileInputStream;
  // using google::protobuf::io::ZeroCopyInputStream;
  // using google::protobuf::io::String;
  // int file_descriptor = open(file_name.c_str(), O_RDONLY);
  // if (file_descriptor < 0) {
  //   std::cerr << "Failed to open file " << file_name << " in text mode.";
  //   // Failed to open;
  //   return false;
  // }

  // ZeroCopyInputStream *input = new FileInputStream(file_descriptor);
  bool success = TextFormat::ParseFromString(file_name, message);
  if (!success) {
    // std::cerr << "Failed to parse file " << file_name << " as text proto.";
    LOG_ERROR("Failed to parse file {} as text proto.", file_name);
  }
  // delete input;
  // close(file_descriptor);
  return success;
}

bool SetProtoToBinaryFile(const google::protobuf::Message &message, const std::string &file_name) {
  std::fstream output(file_name, std::ios::out | std::ios::trunc | std::ios::binary);
  return message.SerializeToOstream(&output);
}

bool GetProtoFromBinaryFile(const std::string &file_name, google::protobuf::Message *message) {
  std::fstream input(file_name, std::ios::in | std::ios::binary);
  if (!input.good()) {
    // std::cerr << "Failed to open file " << file_name << " in binary mode.";
    LOG_ERROR("Failed to open file {} in binary mode.", file_name);
    return false;
  }
  if (!message->ParseFromIstream(&input)) {
    // std::cerr << "Failed to parse file " << file_name << " as binary proto.";
    LOG_ERROR("Failed to open file {} as binary proto.", file_name);
    return false;
  }
  return true;
}

bool GetProtoFromFile(const std::string &file_name, google::protobuf::Message *message) {
  // Try the binary parser first if it's much likely a binary proto.
  static const std::string kBinExt = ".bin";
  if (std::equal(kBinExt.rbegin(), kBinExt.rend(), file_name.rbegin())) {
    return GetProtoFromBinaryFile(file_name, message) || GetProtoFromASCIIFile(file_name, message);
  }

  return GetProtoFromASCIIFile(file_name, message) || GetProtoFromBinaryFile(file_name, message);
}
#endif

bool GetContent(const std::string &file_name, std::string *content) {
  std::ifstream fin(file_name);
  if (!fin) {
    return false;
  }

  std::stringstream str_stream;
  str_stream << fin.rdbuf();
  *content = str_stream.str();
  return true;
}

std::string GetAbsolutePath(const std::string &prefix, const std::string &relative_path) {
  if (relative_path.empty()) {
    return prefix;
  }
  // If prefix is empty or relative_path is already absolute.
  if (prefix.empty() || relative_path.front() == '/') {
    return relative_path;
  }

  if (prefix.back() == '/') {
    return prefix + relative_path;
  }
  return prefix + "/" + relative_path;
}

std::string GetRealPath(const std::string &path_str) {
  boost::filesystem::path p(path_str);
  return boost::filesystem::canonical(p).string();
}
bool PathExists(const std::string &path) {
  struct stat info;
  return stat(path.c_str(), &info) == 0;
}

bool DirectoryExists(const std::string &directory_path) {
  struct stat info;
  return stat(directory_path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR);
}

std::vector<std::string> Glob(const std::string &pattern) {
  glob_t globs = {};
  std::vector<std::string> results;
  if (glob(pattern.c_str(), GLOB_TILDE | GLOB_BRACE, nullptr, &globs) == 0) {
    for (size_t i = 0; i < globs.gl_pathc; ++i) {
      results.emplace_back(globs.gl_pathv[i]);
    }
  }
  globfree(&globs);
  return results;
}

// bool CopyFile(const std::string &from, const std::string &to) {
//   std::ifstream src(from, std::ios::binary);
//   if (!src) {
//     std::cout << "Source path could not be normally opened: " << from << std::endl;
//     std::string command = "cp -r " + from + " " + to;
//     const int ret = std::system(command.c_str());
//     if (ret == 0) {
//       // ADEBUG << "Copy success, command returns " << ret;
//       return true;
//     } else {
//       std::cerr << "Copy error, command returns " << ret << std::endl;
//       return false;
//     }
//   }

//   std::ofstream dst(to, std::ios::binary);
//   if (!dst) {
//     std::cerr << "Target path is not writable: " << to << std::endl;
//     return false;
//   }

//   dst << src.rdbuf();
//   return true;
// }

// bool CopyDir(const std::string &from, const std::string &to) {
//   DIR *directory = opendir(from.c_str());
//   if (directory == nullptr) {
//     std::cerr << "Cannot open directory " << from << std::endl;
//     return false;
//   }

//   bool ret = true;
//   if (EnsureDirectory(to)) {
//     struct dirent *entry;
//     while ((entry = readdir(directory)) != nullptr) {
//       // skip directory_path/. and directory_path/..
//       if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
//         continue;
//       }
//       const std::string sub_path_from = from + "/" + entry->d_name;
//       const std::string sub_path_to = to + "/" + entry->d_name;
//       if (entry->d_type == DT_DIR) {
//         ret &= CopyDir(sub_path_from, sub_path_to);
//       } else {
//         ret &= CopyFile(sub_path_from, sub_path_to);
//       }
//     }
//   } else {
//     std::cerr << "Cannot create target directory " << to << std::endl;
//     ret = false;
//   }
//   closedir(directory);
//   return ret;
// }

// bool Copy(const std::string &from, const std::string &to) {
//   return DirectoryExists(from) ? CopyDir(from, to) : CopyFile(from, to);
// }

bool EnsureDirectory(const std::string &directory_path) {
  std::string path = directory_path;
  for (size_t i = 1; i < directory_path.size(); ++i) {
    if (directory_path[i] == '/') {
      // Whenever a '/' is encountered, create a temporary view from
      // the start of the path to the character right before this.
      path[i] = 0;

      if (mkdir(path.c_str(), 0777) != 0) {
        if (errno != EEXIST) {
          return false;
        }
      }

      // Revert the temporary view back to the original.
      path[i] = '/';
    }
  }

  // Make the final (full) directory.
  if (mkdir(path.c_str(), 0777) != 0) {
    if (errno != EEXIST) {
      return false;
    }
  }

  return true;
}

bool RemoveAllFiles(const std::string &directory_path) {
  DIR *directory = opendir(directory_path.c_str());
  if (directory == nullptr) {
    std::cerr << "Cannot open directory " << directory_path << std::endl;
    return false;
  }

  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    // skip directory_path/. and directory_path/..
    if (!strcmp(file->d_name, ".") || !strcmp(file->d_name, "..")) {
      continue;
    }
    // build the path for each file in the folder
    std::string file_path = directory_path + "/" + file->d_name;
    if (unlink(file_path.c_str()) < 0) {
      std::cerr << "Fail to remove file " << file_path << ": " << strerror(errno) << std::endl;
      closedir(directory);
      return false;
    }
  }
  closedir(directory);
  return true;
}

bool RemoveAllFiles(const std::string &directory_path, unsigned int max_log_num) {
  DIR *directory = opendir(directory_path.c_str());
  if (directory == nullptr) {
    std::cerr << "Cannot open directory " << directory_path << std::endl;
    return false;
  }
  unsigned int log_num = 0;
  std::vector<char *> file_names;
  bool is_delete_log = false;
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    file_names.push_back(file->d_name);
    if (strstr(file->d_name, "_INFO_") != nullptr) {
      log_num++;
    }
    if (log_num >= max_log_num) {
      is_delete_log = true;
      break;
    }
  }
  if (is_delete_log) {
    RemoveAllFiles(directory_path);
  }
  closedir(directory);
  return true;
}

#if 0
std::vector<std::string> ListSubPaths(const std::string &directory_path) {
  std::vector<std::string> result;
  namespace fs = boost::filesystem;
  fs::path fullPath(directory_path);
  fs::directory_iterator endIter;
  for (fs::directory_iterator iter(fullPath); iter != endIter; ++iter) {
    try {
      if (fs::is_directory(*iter)) {
        result.push_back(iter->path().string());
      }
    } catch (const std::exception &ex) {
      std::cerr << ex.what() << std::endl;
      continue;
    }
  }
  return result;
}
#endif

std::string GetFileName(const std::string &path, const bool remove_extension) {
  std::string::size_type start = path.rfind('/');
  if (start == std::string::npos) {
    start = 0;
  } else {
    // Move to the next char after '/'.
    ++start;
  }

  std::string::size_type end = std::string::npos;
  if (remove_extension) {
    end = path.rfind('.');
    // The last '.' is found before last '/', ignore.
    if (end != std::string::npos && end < start) {
      end = std::string::npos;
    }
  }
  const auto len = (end != std::string::npos) ? end - start : end;
  return path.substr(start, len);
}

std::string GetCurrentPath() {
  char tmp[PATH_MAX];
  return getcwd(tmp, sizeof(tmp)) ? std::string(tmp) : std::string("");
}

std::string GetRealFilePath(const std::string &path) {
  std::string real_path = path;
  if (!IsComplete(real_path)) {
    std::string root_path = GetCurrentWorkspace();
    real_path = GetAbsolutePath(root_path, real_path);
  }
  return real_path;
}

std::string GetCurrentExePath() {
  return boost::dll::program_location().parent_path().string();
  //   unsigned int bufferSize = 512;
  //   std::vector<char> buffer(bufferSize + 1);

  // #if defined(_WIN32)
  //   ::GetModuleFileName(NULL, &buffer[0], bufferSize);

  // #elif defined(__linux__)
  //   // Get the process ID.
  //   int pid = getpid();

  //   // Construct a path to the symbolic link pointing to the process executable.
  //   // This is at /proc/<pid>/exe on Linux systems (we hope).
  //   std::ostringstream oss;
  //   oss << "/proc/" << pid << "/exe";
  //   std::string link = oss.str();

  //   // Read the contents of the link.
  //   int count = readlink(link.c_str(), &buffer[0], bufferSize);
  //   if (count == -1)
  //     throw std::runtime_error("Could not read symbolic link");
  //   buffer[count] = '\0';

  // #elif defined(__APPLE__)
  //   if (_NSGetExecutablePath(&buffer[0], &bufferSize)) {
  //     buffer.resize(bufferSize);
  //     _NSGetExecutablePath(&buffer[0], &bufferSize);
  //   }

  // #else
  // #error Cannot yet find the executable on this platform
  // #endif

  //   std::string s = &buffer[0];
  //   size_t pos = s.rfind("/");
  //   if (pos != std::string::npos) {
  //     s.erase(pos, s.size() - pos + 1);
  //   } else {
  //     pos = s.rfind("\\");
  //     if (pos != std::string::npos)
  //       s.erase(pos, s.size() - pos + 1);
  //   }
  //   return s;
}

std::string GetCurrentWorkspace() {
  std::string root_path = g_WorkspacePath;
  if (!root_path.empty())
    return root_path;

  SPDLOG_WARN("g_WorkspacePath not set!!!");
  root_path = GetCurrentExePath();
  SPDLOG_INFO("root_path: {}", root_path);
  LOG_INFO("root_path: {}", root_path);
  const static std::string key_path = "/object_fusion_plus/";
  std::string::size_type pos = root_path.rfind(key_path);
  if (pos != std::string::npos) {
    std::string tmp = root_path;
    root_path = tmp.substr(0, pos + key_path.size());
  } else {
    root_path += "/../";
    root_path = GetRealPath(root_path);
  }

  // ACHECK(PathExists(root_path), " not exists:{}", root_path);
  g_WorkspacePath = root_path;
  return root_path;
}

bool GetType(const string &filename, FileType *type) {
  struct stat stat_buf;
  if (lstat(filename.c_str(), &stat_buf) != 0) {
    return false;
  }
  if (S_ISDIR(stat_buf.st_mode) != 0) {
    *type = TYPE_DIR;
  } else if (S_ISREG(stat_buf.st_mode) != 0) {
    *type = TYPE_FILE;
  } else {
    std::cout << "failed to get type: " << filename << std::endl;
    return false;
  }
  return true;
}

bool IsDir(const std::string &filename) {
  FileType type;
  GetType(filename, &type);
  return type == TYPE_DIR;
}
bool DeleteFile(const string &filename) {
  if (!PathExists(filename)) {
    return true;
  }
  FileType type;
  if (!GetType(filename, &type)) {
    return false;
  }
  if (type == TYPE_FILE) {
    if (remove(filename.c_str()) != 0) {
      std::cerr << "failed to remove file: " << filename << std::endl;
      return false;
    }
    return true;
  }
  DIR *dir = opendir(filename.c_str());
  if (dir == nullptr) {
    std::cout << "failed to opendir: " << filename << std::endl;
    return false;
  }
  dirent *dir_info = nullptr;
  while ((dir_info = readdir(dir)) != nullptr) {
    if (strcmp(dir_info->d_name, ".") == 0 || strcmp(dir_info->d_name, "..") == 0) {
      continue;
    }
    string temp_file = filename + "/" + string(dir_info->d_name);
    FileType temp_type;
    if (!GetType(temp_file, &temp_type)) {
      std::cout << "failed to get file type: " << temp_file << std::endl;
      closedir(dir);
      return false;
    }
    if (type == TYPE_DIR) {
      DeleteFile(temp_file);
    }
    remove(temp_file.c_str());
  }
  closedir(dir);
  remove(filename.c_str());
  return true;
}

bool CreateDir(const string &dir) {
  int ret = mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
  if (ret != 0) {
    std::cout << "failed to create dir. [dir: " << dir << "] [err: " << strerror(errno) << "]"
              << std::endl;
    return false;
  }
  return true;
}

double NowInMilliSeconds() {
  // return static_cast<double>(absl::ToUnixNanos(Now())) / 1e9;
  std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
  auto time_since_epoch =
      std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
  return static_cast<double>(time_since_epoch) / 1e3;
}

bool IsComplete(const std::string &path) {
  return IsAbsolute(path);
}

bool IsAbsolute(const std::string &path_name) {
  return boost::filesystem::path(path_name).is_absolute();
}
bool IsRelative(const std::string &path_name) {
  return boost::filesystem::path(path_name).is_relative();
}
