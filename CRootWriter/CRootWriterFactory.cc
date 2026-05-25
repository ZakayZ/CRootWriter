/**
 * CRoot - COLA Library Module for ROOT data storage support.
 * Copyright (C) 2025-2026 Savva Savenkov
 *
 * This file is part of CRoot
 *
 * CRoot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CRoot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CRoot.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "CRootWriterFactory.hh"

#include "CAAMCCWriter.hh"
#include "CNativeRootWriter.hh"
#include "CUniGenWriter.hh"

#include <COLA.hh>

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

using namespace cola;

constexpr size_t kDefaultBufferSize = 10000;

std::unique_ptr<VFilter> CRootWriterFactory::Create(const std::unordered_map<std::string, std::string>& param_map) {
  size_t buffer_size = kDefaultBufferSize;
  bool write_coord = false;
  std::string format = "COLANative";
  if (param_map.find("buff_size") != param_map.end()) {
    buffer_size = std::stoi(param_map.at("buff_size"));
  }
  if (param_map.find("write_coord") != param_map.end()) {
    write_coord = (std::stoi(param_map.at("write_coord")) != 0);
  }
  if (param_map.find("format") != param_map.end()) {
    format = param_map.at("format");
  }
  if (format == "UniGen") {
    return std::make_unique<CUniGenWriter>(param_map.at("file_name"), buffer_size, write_coord);
  }
  if (format == "AAMCC") {
    return std::make_unique<CAAMCCWriter>(param_map.at("file_name"), buffer_size, write_coord);
  }
  return std::make_unique<CNativeRootWriter>(param_map.at("file_name"), buffer_size, write_coord);
}