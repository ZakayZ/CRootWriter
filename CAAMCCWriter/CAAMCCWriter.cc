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

#include "CAAMCCWriter.hh"

using namespace cola;

CAAMCCWriter::CAAMCCWriter(const std::string& f_name, size_t buff_size, bool write_coord)
    : CRootWriter(f_name, buff_size), write_coord_(write_coord) {
  // data trees
  output_tree_map_.emplace("Glauber", new TTree("Glauber", "Events from glauber modeling"));
  TTree* t_glauber = output_tree_map_.at("Glauber");

  t_glauber->SetDirectory(nullptr);

  if (write_coord_) {
    t_glauber->Branch("pseudorapidity_on_A", "std::vector", &event_.pseudorapidity_a);
    t_glauber->Branch("pseudorapidity_on_B", "std::vector", &event_.pseudorapidity_b);
    t_glauber->Branch("pX_on_A", "std::vector", &event_.p_xon_side_a, 128000, 1);
    t_glauber->Branch("pY_on_A", "std::vector", &event_.p_yon_side_a, 128000, 1);
    t_glauber->Branch("pZ_on_A", "std::vector", &event_.p_zon_side_a, 128000, 1);
    t_glauber->Branch("pX_on_B", "std::vector", &event_.p_xon_side_b, 128000, 1);
    t_glauber->Branch("pY_on_B", "std::vector", &event_.p_yon_side_b, 128000, 1);
    t_glauber->Branch("pZ_on_B", "std::vector", &event_.p_zon_side_b, 128000, 1);
  }

  t_glauber->Branch("id", &event_.id, "id/i");
  t_glauber->Branch("A_on_A", "std::vector", &event_.mass_on_side_a);
  t_glauber->Branch("A_on_B", "std::vector", &event_.mass_on_side_b);
  t_glauber->Branch("Z_on_A", "std::vector", &event_.charge_on_side_a);
  t_glauber->Branch("Z_on_B", "std::vector", &event_.charge_on_side_b);
  t_glauber->Branch("Ncoll", &event_.ncoll, "Ncoll/I");
  t_glauber->Branch("Ncollpp", &event_.ncollpp, "Ncollpp/I");
  t_glauber->Branch("Ncollpn", &event_.ncollpn, "Ncollpn/I");
  t_glauber->Branch("Ncollnn", &event_.ncollnn, "Ncollnn/I");
  t_glauber->Branch("Npart", &event_.npart, "Npart/I");
  t_glauber->Branch("NpartA", &event_.npart_a, "NpartA/I");
  t_glauber->Branch("NpartB", &event_.npart_b, "NpartB/I");

  t_glauber->Branch("impact_parameter", &event_.b, "impact_parameter/f");

  t_glauber->Branch("PhiRotA", &event_.phi_rot_a, "PhiRotA/f");
  t_glauber->Branch("ThetaRotA", &event_.theta_rot_a, "ThetaRotA/f");
  t_glauber->Branch("PhiRotB", &event_.phi_rot_b, "PhiRotB/f");
  t_glauber->Branch("ThetaRotB", &event_.theta_rot_b, "ThetaRotB/f");

  t_run_ = std::make_unique<TTree>(
      "Conditions", "preconditions for modeling");  // tRun is purposefuly not in map, so auto buffering isn't applied

  t_run_->SetDirectory(nullptr);

  t_run_->Branch("Xsect_NN", &run_data_.xsect_nn, "Xsect_total/d");
  t_run_->Branch("Kinetic_energy_per_nucleon_of_projectile_in_MeV", &run_data_.kin_en_per_nucl,
                 "Kinetic_energy_of_per_nucleon_projectile_in_MeV/d");
  t_run_->Branch("SqrtS_nn_in_MeV", &run_data_.sqrt_snn, "SqrtS_nn_in_MeV/d");
  t_run_->Branch("pZ_in_MeV_on_A", &run_data_.pz_a, "pZ_in_MeV_on_A/d");
  t_run_->Branch("pZ_in_MeV_on_B", &run_data_.pz_b, "pZ_in_MeV_on_B/d");
  t_run_->Branch("Mass_on_A", &run_data_.ainit_a, "Mass_on_A/I");
  t_run_->Branch("Mass_on_B", &run_data_.ainit_b, "Mass_on_B/I");
  t_run_->Branch("Charge_on_A", &run_data_.zinit_a, "Charge_on_A/I");
  t_run_->Branch("Charge_on_B", &run_data_.zinit_b, "Charge_on_B/I");
}

void CAAMCCWriter::WriteEvent(std::unique_ptr<EventData>&& data) {
  if (callflag_) {
    auto aza = PdgToAZ(data->ini_state.pdg_code_a);
    auto azb = PdgToAZ(data->ini_state.pdg_code_b);

    run_data_.ainit_a = aza.first;
    run_data_.zinit_a = aza.second;
    run_data_.ainit_b = azb.first;
    run_data_.zinit_b = azb.second;

    run_data_.is_collider = data->ini_state.pz_b != 0;
    run_data_.pz_a = data->ini_state.pz_a;
    run_data_.pz_b = data->ini_state.pz_b;

    if (run_data_.is_collider) {
      run_data_.sqrt_snn = data->ini_state.energy;
      run_data_.kin_en_per_nucl = run_data_.sqrt_snn / 2.0 - caamcc::kNucleonAverMass;
    } else {
      run_data_.kin_en_per_nucl = data->ini_state.energy;
      run_data_.sqrt_snn = pow(2 * caamcc::kNucleonAverMass * caamcc::kNucleonAverMass +
                                   2 * run_data_.kin_en_per_nucl * caamcc::kNucleonAverMass,
                               0.5);
    }

    run_data_.xsect_nn = data->ini_state.sect_nn;

    t_run_->Fill();
    t_run_->Write();
    callflag_ = false;
  }

  event_.id = count_;
  event_.b = data->ini_state.b;
  event_.ncoll = data->ini_state.num_coll;
  event_.ncollnn = data->ini_state.num_coll_nn;
  event_.ncollpn = data->ini_state.num_coll_pn;
  event_.ncollpp = data->ini_state.num_coll_pp;
  event_.npart = data->ini_state.num_part;
  event_.npart_a = data->ini_state.num_part_a;
  event_.npart_b = data->ini_state.num_part_b;

  event_.phi_rot_a = data->ini_state.phi_rot_a;
  event_.theta_rot_a = data->ini_state.theta_rot_a;
  event_.phi_rot_b = data->ini_state.phi_rot_b;
  event_.theta_rot_b = data->ini_state.theta_rot_b;

  for (const auto& particle : data->particles) {
    switch (particle.p_class) {
      case ParticleClass::kSpectatorA:
        event_.mass_on_side_a.push_back(static_cast<float>(particle.GetAZ().first));
        event_.charge_on_side_a.push_back(static_cast<float>(particle.GetAZ().second));
        if (write_coord_) {
          event_.p_xon_side_a.push_back(particle.momentum.x);
          event_.p_yon_side_a.push_back(particle.momentum.y);
          event_.p_zon_side_a.push_back(particle.momentum.z);

          event_.pseudorapidity_a.push_back(std::atanh(particle.momentum.z / particle.momentum.Mag()));
        }
        break;

      case ParticleClass::kSpectatorB:
        event_.mass_on_side_b.push_back(static_cast<float>(particle.GetAZ().first));
        event_.charge_on_side_b.push_back(static_cast<float>(particle.GetAZ().second));
        if (write_coord_) {
          event_.p_xon_side_b.push_back(particle.momentum.x);
          event_.p_yon_side_b.push_back(particle.momentum.y);
          event_.p_zon_side_b.push_back(particle.momentum.z);

          event_.pseudorapidity_b.push_back(std::atanh(particle.momentum.z / particle.momentum.Mag()));
        }
        break;
      default:
        break;
    }
  }

  for (const auto& tree : output_tree_map_) {
    tree.second->Fill();
  }

  // clear after fillng
  event_.mass_on_side_a.clear();
  event_.mass_on_side_b.clear();
  event_.charge_on_side_a.clear();
  event_.charge_on_side_b.clear();
  if (write_coord_) {
    event_.p_xon_side_a.clear();
    event_.p_yon_side_a.clear();
    event_.p_zon_side_a.clear();
    event_.pseudorapidity_a.clear();
    event_.p_xon_side_b.clear();
    event_.p_yon_side_b.clear();
    event_.p_zon_side_b.clear();
    event_.pseudorapidity_b.clear();
  }
}
