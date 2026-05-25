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

#include "CUniGenWriter.hh"

using namespace cola;

CUniGenWriter::CUniGenWriter(const std::string& f_name, size_t buff_size, bool write_coord)
    : CRootWriter(f_name, buff_size)
    , cur_event_(std::make_unique<UEvent>())
    , run_(std::make_unique<URun>())
    , write_coord_(write_coord)
    , run_filled_(false) {
  output_tree_map_.emplace("UniGen", new TTree("UniGen", "UniGen"));
  output_tree_ = output_tree_map_.at("UniGen");
  output_tree_->Branch("events", cur_event_.get());
  // disable unfilled subbrranches
  output_tree_->SetBranchStatus("events.fPhi", false);
  output_tree_->SetBranchStatus("events.fNes", false);
  output_tree_->SetBranchStatus("events.fStepNr", false);
  output_tree_->SetBranchStatus("events.fStepT", false);
  output_tree_->SetBranchStatus("events.fComment", false);
  output_tree_->SetBranchStatus("events.fParticles.fParent", false);
  output_tree_->SetBranchStatus("events.fParticles.fParentDecay", false);
  output_tree_->SetBranchStatus("events.fParticles.fMate", false);
  output_tree_->SetBranchStatus("events.fParticles.fDecay", false);
  output_tree_->SetBranchStatus("events.fParticles.fChild[2]", false);
  if (not write_coord_) {
    output_tree_->SetBranchStatus("events.fParticles.fPx", false);
    output_tree_->SetBranchStatus("events.fParticles.fPy", false);
    output_tree_->SetBranchStatus("events.fParticles.fPz", false);
    output_tree_->SetBranchStatus("events.fParticles.fE", false);
    output_tree_->SetBranchStatus("events.fParticles.fX", false);
    output_tree_->SetBranchStatus("events.fParticles.fY", false);
    output_tree_->SetBranchStatus("events.fParticles.fZ", false);
    output_tree_->SetBranchStatus("events.fParticles.fT", false);
  }
}

void CUniGenWriter::WriteEvent(std::unique_ptr<EventData>&& data) {
  const auto& ini_state = data->ini_state;
  const auto& particles = data->particles;
  // currently no decay info in COLA, plug is needed
  int child_plug[2]{-1, -1};  // NOLINT(modernize-avoid-c-arrays)

  // fill run data (only once)
  if (not run_filled_) {
    auto nucl_a = PdgToAZ(ini_state.pdg_code_a);
    auto nucl_b = PdgToAZ(ini_state.pdg_code_b);

    run_ =
        std::make_unique<URun>("", "COLA output, -1 fields mean no info in DO", nucl_a.first, nucl_a.second,
                               ini_state.pz_a, nucl_b.first, nucl_b.second, ini_state.pz_b, -1, -1, -1, -1, -1, -1, -1);

    run_->Write("run");
    run_filled_ = true;
  }

  cur_event_->SetParameters(static_cast<Int_t>(count_), ini_state.b, -1, -1, -1, -1);

  // Add particles
  int i = 0;
  for (const auto particle : particles) {
    if (write_coord_) {
      cur_event_->AddParticle(i++, particle.pdg_code, static_cast<int>(particle.p_class), -1, -1, -1, -1, child_plug,
                              particle.momentum.x, particle.momentum.y, particle.momentum.z, particle.momentum.e,
                              particle.position.x, particle.position.y, particle.position.z, particle.position.t, -1);
    } else {
      cur_event_->AddParticle(i++, particle.pdg_code, static_cast<int>(particle.p_class), -1, -1, -1, -1, child_plug,
                              -1, -1, -1, -1, -1, -1, -1, -1, -1);
    }
  }

  output_tree_->Fill();

  // Clear particles
  cur_event_->Clear();
}
