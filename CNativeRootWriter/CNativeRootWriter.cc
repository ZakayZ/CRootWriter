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

#include "CNativeRootWriter.hh"

using namespace cola;

CNativeRootWriter::CNativeRootWriter(const std::string& f_name, size_t buff_size, bool write_coord)
    : CRootWriter(f_name, buff_size), write_coord_(write_coord) {
  output_tree_map_.emplace("ColaNative", new TTree("ColaNative", "ColaNative"));
  output_tree_ = output_tree_map_.at("ColaNative");
  output_tree_->Branch("events", &event_data_);
  // disable unfilled subbrranches
  if (not write_coord_) {
    output_tree_->SetBranchStatus("events.particles.position.x", false);
    output_tree_->SetBranchStatus("events.particles.position.y", false);
    output_tree_->SetBranchStatus("events.particles.position.z", false);
    output_tree_->SetBranchStatus("events.particles.momentum.x", false);
    output_tree_->SetBranchStatus("events.particles.momentum.y", false);
    output_tree_->SetBranchStatus("events.particles.momentum.z", false);
  }
}

void CNativeRootWriter::WriteEvent(std::unique_ptr<EventData>&& data) {
  event_data_ = std::move(*data);
  output_tree_->Fill();
}
