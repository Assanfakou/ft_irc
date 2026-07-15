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