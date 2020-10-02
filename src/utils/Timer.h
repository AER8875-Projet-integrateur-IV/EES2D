#include <chrono>
#include <string>

namespace ees2d::Utils {

    class Timer {
        //Calculates time from instanciation of a Timer object until its destruction
        //use it inside scopes : example to test Print function :
        /*      {
                    Timer Timeit;
                    Print();
                }                                   
        */

       
        public:
            Timer(std::string);
            ~Timer();

            // Will be called in destructor
            void stop();

        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
            std::string m_tag;
    };
} // namespace ees2d::Utils