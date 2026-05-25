/**
 * CRootWriter - COLA Library Module for ROOT data storage support.
 * Copyright (C) 2025 Savva Savenkov
 *
 * This file is part of CRootWriter
 *
 * CRootWriter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CRootWriter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CRootWriter.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "CRootWriterModule.hh"

extern "C" cola::VModule* LoadCOLAModule() { return new cola::CRootWriterModule(); }
