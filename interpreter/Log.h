#pragma once


#define LOG(expr) std::cerr << "#" << expr << std::endl;
#define LOGVAR(var) std::cerr << "#" << #var << ": " << var << std::endl;
