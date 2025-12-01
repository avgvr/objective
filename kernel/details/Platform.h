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
