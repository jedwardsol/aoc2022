#include <chrono>

namespace jle
{

class stopwatch
{
    using Clock = std::chrono::steady_clock;

public:

    stopwatch() : start{Clock::now()}
    {
    }

    double microseconds()
    {
        std::chrono::duration<double, std::micro>   duration{Clock::now()-start};
        
        return duration.count();
    }


    double milliseconds()
    {
        std::chrono::duration<double, std::milli>   duration{Clock::now()-start};
        
        return duration.count();
    }


    double seconds()
    {
        std::chrono::duration<double>   duration{Clock::now()-start};
        
        return duration.count();
    }

    double minutes()
    {
        std::chrono::duration<double, std::ratio<60>>   duration{Clock::now()-start};
        
        return duration.count();
    }


private:

    Clock::time_point  start;
};

}