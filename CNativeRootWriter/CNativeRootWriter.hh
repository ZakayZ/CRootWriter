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

#ifndef CROOT_CNATIVEROOTWRITER_HH
#define CROOT_CNATIVEROOTWRITER_HH

#include "CRootWriter.hh"

#include <COLA.hh>

namespace cola {

  class CNativeRootWriter final : public CRootWriter {
   private:
    EventData event_data_;
    bool write_coord_;

    TTree* output_tree_;  // only view to single map element

    void WriteEvent(std::unique_ptr<EventData>&& /*unused*/) final;

   public:
    CNativeRootWriter() = delete;
    CNativeRootWriter(const CNativeRootWriter&) = delete;
    CNativeRootWriter(CNativeRootWriter&&) = delete;
    CNativeRootWriter& operator=(const CNativeRootWriter&) = delete;
    CNativeRootWriter& operator=(CNativeRootWriter&&) = delete;
    CNativeRootWriter(const std::string& f_name, size_t buff_size, bool write_coord = false);
  };
}  // namespace cola

#endif  // CROOT_CNATIVEROOTWRITER_HH