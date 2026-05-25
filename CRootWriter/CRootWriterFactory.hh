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

#ifndef CROOT_CUNIGENFACTORY_HH
#define CROOT_CUNIGENFACTORY_HH

#include <COLA.hh>

namespace cola {

  class CRootWriterFactory final : public VWriterFactory {
   public:
    std::unique_ptr<VFilter> Create(const std::unordered_map<std::string, std::string>& /*param_map*/) final;

    const std::string& GetFilterName() const override {
      static const std::string name{"CRootWriter"};
      return name;
    }
  };
}  // namespace cola

#endif  // CROOT_CUNIGENFACTORY_HH
