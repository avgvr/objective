/**
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
 */

#ifndef PLATHFORMDETAILS_H
#define PLATHFORMDETAILS_H


namespace cpuid
{

/**
 * Platform independent class to acquire processor details
 */
class Executor
{
    bool isCpuidAvailable;
public:
    /**
     * Standard constructor of Executor class
     *
     * @note Must be declare in platform depended part of code by path
     *     `arch/{architecture}/cpuid/`.
     */
    Executor();

    /**
     * Perform cpuid command
     *
     * @tparam T the type posed processor details by one kind
     * @param[out] instance to fill the details
     */
    template<typename T>
    void cpuid(T &cmd)
    {
       if(isCpuidAvailable)
        {
            acquireInformation(cmd);
        }
    }

    /**
     * Availability of cpuid command
     *
     * @return true if cpuid command would execute,
     *     false otherwise
     */
    bool isCpuidWouldExecute() {return isCpuidAvailable;};
};

};

#endif
