#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

void	split_args(const std::string str, const std::string sep, std::vector<std::string>& result);
void	sig_quit(int code);
