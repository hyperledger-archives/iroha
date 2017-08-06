/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ametsuchi/impl/flat_file/flat_file.hpp"
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <iostream>

std::string id_to_name(uint32_t id) {
  std::string new_id(16, '\0');
  sprintf(&new_id[0], "%016u", id);
  return new_id;
}

uint32_t name_to_id(std::string name) {
  std::string::size_type sz;
  return std::stoul(name, &sz);
}

void remove(std::string dump_dir, uint32_t id) {
  // Assume that id exists
  auto f_name = dump_dir + "/" + id_to_name(id);
  if (std::remove(f_name.c_str()) != 0) {
    perror("Error deleting file");
  }
}

nonstd::optional<uint32_t> check_consistency(std::string dump_dir) {
  uint32_t tmp_id = 0u;
  if (!dump_dir.empty()) {
    // Directory iterator:
    struct dirent **namelist;
    auto status = scandir(dump_dir.c_str(), &namelist, NULL, alphasort);
    if (status < 0) {
      // TODO: handle internal error
      return nonstd::nullopt;
    } else if (status > 2) {
      uint n = status;
      tmp_id++;
      uint i = 1;
      while (++i < n) {
        if (id_to_name(tmp_id) != namelist[i]->d_name) {
          for (uint j = i; j < n; ++j) {
            remove(dump_dir, name_to_id(namelist[j]->d_name));
          }
          break;
        }
        tmp_id = name_to_id(namelist[i]->d_name);
      }

      for (uint j = 0; j < n; ++j) {
        free(namelist[j]);
      }
      free(namelist);
    }

  } else {
    // Not a directory
    // TODO: handle not a directory
    return nonstd::nullopt;
  }
  return tmp_id;
}

namespace iroha {
  namespace ametsuchi {

    FlatFile::FlatFile(uint32_t current_id, const std::string &path)
        : current_id(current_id), dump_dir(path) {}

    FlatFile::~FlatFile() {}

    void FlatFile::add(uint32_t id, const std::vector<uint8_t> &block) {
      auto next_id = id;
      std::string file_name = dump_dir + "/" + id_to_name(id);
      // Write block to binary file
      if (file_exist(file_name)) {
        // File already exist

      } else {
        // New file will be created
        FILE *pfile;
        pfile = fopen(file_name.c_str(), "wb");
        if (!pfile) {
          // TODO log file error
          return;
        }
        /*auto res = */ fwrite(block.data(), sizeof(uint8_t), block.size(),
                               pfile);
        fflush(pfile);
        fclose(pfile);

        // Update internals, release lock
        current_id = next_id;
      }
    }

    nonstd::optional<std::vector<uint8_t>> FlatFile::get(uint32_t id) const {
      std::string filename = dump_dir + "/" + id_to_name(id);
      if (file_exist(filename)) {
        auto f_size = file_size(filename);
        std::vector<uint8_t> buf(f_size);
        FILE *pfile = fopen(filename.c_str(), "rb");
        fread(&buf[0], sizeof(uint8_t), f_size, pfile);
        fclose(pfile);
        return buf;
      } else {
        // TODO log block not found
        return nonstd::nullopt;
      }
    }

    bool FlatFile::file_exist(const std::string &name) const {
      struct stat buffer;
      return (stat(name.c_str(), &buffer) == 0);
    }

    long FlatFile::file_size(const std::string &filename) const {
      struct stat stat_buf;
      int rc = stat(filename.c_str(), &stat_buf);
      return rc == 0 ? stat_buf.st_size : 0u;
    }

    std::unique_ptr<FlatFile> FlatFile::create(const std::string &path) {
      // TODO directory check
      auto res = check_consistency(path);
      if (!res) {
        return nullptr;
      }
      return std::unique_ptr<FlatFile>(new FlatFile(*res, path));
    }

    std::string FlatFile::directory() const { return dump_dir; }

    uint32_t FlatFile::last_id() const { return current_id; }

  }  // namespace ametsuchi
}  // namespace iroha
