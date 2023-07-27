#pragma once


#define LOG(expr) std::cerr << "#" << expr << std::endl;
#define LOGVAR(var) std::cerr << "#" << #var << ": " << var << std::endl;
#define LOG_ERR(text) std::cerr << text << std::endl;
