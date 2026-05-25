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

#ifndef CROOT_CROOTWRITER_HH
#define CROOT_CROOTWRITER_HH

#include "Riostream.h"
#include "TFile.h"
#include "TTree.h"
#include "UEvent.hh"
#include "UParticle.hh"
#include "URun.hh"

#include <COLA.hh>

namespace cola {

  class CRootWriter : public VWriter {
   private:
    std::unique_ptr<TFile> output_file_;
    const size_t buff_size_;

   protected:
    std::map<std::string, TTree*> output_tree_map_;
    size_t count_;

    virtual void WriteEvent(std::unique_ptr<EventData>&&) = 0;

   public:
    CRootWriter() = delete;
    CRootWriter(const std::string& f_name, size_t buff_size);

    CRootWriter(const CRootWriter&) = delete;
    CRootWriter(CRootWriter&&) = delete;
    CRootWriter& operator=(const CRootWriter&) = delete;
    CRootWriter& operator=(CRootWriter&&) = delete;

    ~CRootWriter() override;

    void operator()(std::unique_ptr<EventData>&& /*data*/) final;
  };
}  // namespace cola

#endif  // CROOT_CROOTWRITER_HH
