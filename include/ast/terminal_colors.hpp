#ifndef LAMBDA_TERMINAL_COLORS_HPP
#define LAMBDA_TERMINAL_COLORS_HPP

// Enable terminal colors
//#define ENABLE_TERMINAL_COLORS
//

#define TERM_RED "31"
#define TERM_GREEN "32"
#define TERM_LIGHT_GREEN "150"

#ifdef ENABLE_TERMINAL_COLORS
#define TERM_FG_START(color) "\e[" color "m"
#define TERM_BOLD_START() "\e[1m"
#define TERM_RESET() "\e[0m"
#else
#define TERM_FG_START(color) ""
#define TERM_BOLD_START() ""
#define TERM_RESET() ""
#endif

#endif //LAMBDA_TERMINAL_COLORS_HPP
