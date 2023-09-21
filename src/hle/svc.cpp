/*
    Nozomi is an experimental HLE Switch emulator.
    Copyright (C) 2023  noumidev

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "svc.hpp"

#include <cstdlib>
#include <ios>

#include <plog/Log.h>

#include "cpu.hpp"
#include "handle.hpp"
#include "result.hpp"

namespace hle::svc {

namespace SupervisorCall {
    enum : u32 {
        GetInfo = 0x29,
    };
}

// GetInfo
namespace InfoType {
    enum : u32 {
        AliasRegionAddress = 2,
        AliasRegionSize = 3,
    };
}

void handleSVC(u32 svc) {
    switch (svc) {
        case SupervisorCall::GetInfo:
            svcGetInfo();
            break;
        default:
            PLOG_FATAL << "Unimplemented SVC " << std::hex << svc;

            exit(0);
    }
}

void svcGetInfo() {
    const u32 type = sys::cpu::get(1);
    const Handle handle = sys::cpu::get(2);
    const u64 subType = sys::cpu::get(3);

    PLOG_INFO << "svcGetInfo (type = " << std::hex << type << ", handle = " << handle << ", sub type = " << subType << ")";

    sys::cpu::set(0, Result::Success);

    switch (type) {
        case InfoType::AliasRegionAddress:
            if ((handle != KernelHandles::CurrentProcess) || (subType != 0)) {
                PLOG_WARNING << "Unexpected handle/sub type for AliasRegionAddress";
            }

            sys::cpu::set(1, 0); // What is this?
            break;
        case InfoType::AliasRegionSize:
            if ((handle != KernelHandles::CurrentProcess) || (subType != 0)) {
                PLOG_WARNING << "Unexpected handle/sub type for AliasRegionSize";
            }

            sys::cpu::set(1, 0); // What is this?
            break;
        default:
            PLOG_FATAL << "Unknown type " << type;

            exit(0);
    }
}

}
