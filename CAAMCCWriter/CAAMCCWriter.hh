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

#ifndef CROOT_CAAMCCWRITER_HH
#define CROOT_CAAMCCWRITER_HH

#include "COLA.hh"
#include "CRootWriter.hh"
#include "TFile.h"
#include "TTree.h"

#include <array>
#include <memory>
#include <string>
#include <vector>

namespace cola {

  namespace caamcc {
    constexpr double kNucleonAverMass = 0.93891875434 * 1e3;
    constexpr double kPi = 3.14159265358979323846;
  }  // namespace caamcc

  struct AAMCCEvent {
    std::vector<float> mass_on_side_a;
    std::vector<float> mass_on_side_b;
    std::vector<float> charge_on_side_a;
    std::vector<float> charge_on_side_b;
    std::vector<double> p_xon_side_a;
    std::vector<double> p_yon_side_a;
    std::vector<double> p_zon_side_a;
    std::vector<double> p_xon_side_b;
    std::vector<double> p_yon_side_b;
    std::vector<double> p_zon_side_b;
    std::vector<double> pseudorapidity_a;
    std::vector<double> pseudorapidity_b;

    float b = 0;
    float ex_en_a = 0;
    float ex_en_b = 0;
    int id = 0;
    int nhard = 0;
    int ncoll = 0;
    int ncollpp = 0;
    int ncollpn = 0;
    int ncollnn = 0;
    int npart = 0;
    int npart_a = 0;
    int npart_b = 0;

    double fermi_mom_a_x = 0;
    double fermi_mom_a_y = 0;
    double fermi_mom_a_z = 0;
    double fermi_mom_b_x = 0;
    double fermi_mom_b_y = 0;
    double fermi_mom_b_z = 0;

    float phi_rot_a = 0;
    float theta_rot_a = 0;
    float phi_rot_b = 0;
    float theta_rot_b = 0;
    std::array<float, 10> ecc = {};

    int clust_num_a = 0;
    int clust_num_b = 0;
    double d_mst_a = 0;
    double d_mst_b = 0;
    std::vector<int> a_cl{0};
    std::vector<int> z_cl{0};
    std::vector<int> ab_cl{0};
    std::vector<int> zb_cl{0};

    AAMCCEvent& operator=(const EventData&);
  };

  struct AAMCCrun {
    int zinit_a = -1;
    int ainit_a = -1;
    int zinit_b = -1;
    int ainit_b = -1;

    std::string sys_a;
    std::string sys_b;
    std::string file_name;
    std::string file_r_name;

    double kin_en_per_nucl = -1.0;
    double sqrt_snn = -1.0;
    double pz_a = -1.0;
    double pz_b = -1.0;
    bool is_collider;

    int iterations = -1;

    double xsect_nn = -1.0;
    double xsect_tot = -1.0;

    std::string de_ex_model;
    int ex_ex_stat_label = -1;

    double low_limit_b = -1.0;    // MB if negative
    double upper_limit_b = -2.0;  // MB if upperLimitB < lowLimitB

    double crit_dist = 2.7;  // 2.7

    bool in_file_or_not = false;

    bool is_qmd = false;

    AAMCCrun& operator=(const EventData&);
  };

  class CAAMCCWriter : public CRootWriter {
   private:
    std::unique_ptr<TTree> t_run_;  // tRun is purposefuly not in map, so auto buffering isn't applied
    AAMCCEvent event_;
    AAMCCrun run_data_;

    bool callflag_ = true;
    bool write_coord_;

    void WriteEvent(std::unique_ptr<EventData>&& /*unused*/) final;

   public:
    CAAMCCWriter() = delete;
    CAAMCCWriter(const CAAMCCWriter&) = delete;
    CAAMCCWriter(CAAMCCWriter&&) = delete;
    CAAMCCWriter& operator=(const CAAMCCWriter&) = delete;
    CAAMCCWriter& operator=(CAAMCCWriter&&) = delete;

    CAAMCCWriter(const std::string& f_name, size_t buff_size, bool write_coord);
  };
}  // namespace cola

#endif  // CROOT_CAAMCCWRITER_HH
