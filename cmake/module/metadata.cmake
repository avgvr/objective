#[[
 * Copyright (c) 2026 Alexey Gavrilov <alexey.gavrilov@mail.com>
 *
 * This file is part of ObjectiveOS.
 *
 * ObjectiveOS is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * ObjectiveOS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ObjectiveOS. If not, see <https://www.gnu.org/licenses/>. 
 ]]

# FIX: get elements from ObjectiveProject_ROOT/arch directory
set(OBJECTIVE_SUPPORTED_PROCESSORS x86;arm CACHE STRING "Supported
processors by Objective kernel")

# FIX: get apps from docs
set(OBJECTIVE_SUPPORTED_ISO_CREATION_APPS grub-mkrescue CACHE STRING "Supported
apps to create an ISO image")

