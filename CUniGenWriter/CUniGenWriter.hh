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

#ifndef CROOT_CUNIGENWRITER_HH
#define CROOT_CUNIGENWRITER_HH

#include "CRootWriter.hh"
#include "Riostream.h"
#include "TFile.h"
#include "TTree.h"
#include "UEvent.hh"
#include "UParticle.hh"
#include "URun.hh"

#include <COLA.hh>

#include <utility>

namespace cola {

  class CUniGenWriter final : public CRootWriter {
   private:
    std::unique_ptr<UEvent> cur_event_;
    std::unique_ptr<URun> run_;
    bool write_coord_;

    bool run_filled_;
    TTree* output_tree_;  // only view to single map element

    void WriteEvent(std::unique_ptr<EventData>&& /*unused*/) final;

   public:
    CUniGenWriter() = delete;
    CUniGenWriter(const CUniGenWriter&) = delete;
    CUniGenWriter(CUniGenWriter&&) = delete;
    CUniGenWriter& operator=(const CUniGenWriter&) = delete;
    CUniGenWriter& operator=(CUniGenWriter&&) = delete;
    CUniGenWriter(const std::string& f_name, size_t buff_size, bool write_coord = false);
  };
}  // namespace cola

#endif  // CROOT_CUNIGENWRITER_HH
