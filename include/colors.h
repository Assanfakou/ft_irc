#pragma once

#include <string>

/*
** ==========================================================
** IRC Formatting Codes (RFC 1459 / mIRC)
** ==========================================================
*/

// Control characters


const std::string IRC_COLOR      = "\x03";
const std::string IRC_BOLD       = "\x02";
const std::string IRC_ITALIC     = "\x1D";
const std::string IRC_UNDERLINE  = "\x1F";
const std::string IRC_REVERSE    = "\x16";
const std::string IRC_RESET      = "\x0F";

/*
** ==========================================================
** IRC Colors
** ==========================================================
*/

const std::string IRC_WHITE       = IRC_COLOR + "00";
const std::string IRC_BLACK       = IRC_COLOR + "01";
const std::string IRC_BLUE        = IRC_COLOR + "02";
const std::string IRC_GREEN       = IRC_COLOR + "03";
const std::string IRC_RED         = IRC_COLOR + "04";
const std::string IRC_BROWN       = IRC_COLOR + "05";
const std::string IRC_PURPLE      = IRC_COLOR + "06";
const std::string IRC_ORANGE      = IRC_COLOR + "07";
const std::string IRC_YELLOW      = IRC_COLOR + "08";
const std::string IRC_LIGHTGREEN  = IRC_COLOR + "09";
const std::string IRC_CYAN        = IRC_COLOR + "10";
const std::string IRC_LIGHTCYAN   = IRC_COLOR + "11";
const std::string IRC_LIGHTBLUE   = IRC_COLOR + "12";
const std::string IRC_PINK        = IRC_COLOR + "13";
const std::string IRC_GREY        = IRC_COLOR + "14";
const std::string IRC_LIGHTGREY   = IRC_COLOR + "15";

/*
** ==========================================================
** ANSI / Bash Formatting
** ==========================================================
*/

// const std::string IRC_RESET      = "\033[0m";
// const std::string IRC_BOLD       = "\033[1m";
// const std::string IRC_ITALIC     = "\033[3m";
// const std::string IRC_UNDERLINE  = "\033[4m";
// const std::string IRC_REVERSE    = "\033[7m";

// /*
// ** ==========================================================
// ** ANSI / Bash Colors
// ** ==========================================================
// */

// const std::string IRC_WHITE       = "\033[37m";
// const std::string IRC_BLACK       = "\033[30m";
// const std::string IRC_BLUE        = "\033[34m";
// const std::string IRC_GREEN       = "\033[32m";
// const std::string IRC_RED         = "\033[31m";
// const std::string IRC_BROWN       = "\033[33m";   // closest match
// const std::string IRC_PURPLE      = "\033[35m";
// const std::string IRC_ORANGE      = "\033[33m";   // closest match
// const std::string IRC_YELLOW      = "\033[93m";
// const std::string IRC_LIGHTGREEN  = "\033[92m";
// const std::string IRC_CYAN        = "\033[36m";
// const std::string IRC_LIGHTCYAN   = "\033[96m";
// const std::string IRC_LIGHTBLUE   = "\033[94m";
// const std::string IRC_PINK        = "\033[95m";
// const std::string IRC_GREY        = "\033[90m";
// const std::string IRC_LIGHTGREY   = "\033[97m";