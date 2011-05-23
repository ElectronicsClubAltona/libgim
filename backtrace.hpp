
#include <string>
#include <vector>
#include <iostream>

namespace debug {
    class backtrace {
        protected:
            static const unsigned int DEFAULT_DEPTH = 16;
            std::vector<void *>       m_frames;

        public:
            backtrace (void);

            const decltype(m_frames)& frames(void) const
                { return m_frames; }
    };
}

std::ostream&
operator <<(std::ostream&, const debug::backtrace&);

