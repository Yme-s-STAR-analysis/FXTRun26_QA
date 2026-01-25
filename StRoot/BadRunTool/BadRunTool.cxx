#ifndef __BADRUNTOOL__
#define __BADRUNTOOL__

#include <iostream>
#include <algorithm>
#include "BadRunTool.h"

void BadRunTool::Init() {
    const char* dstName = "4p5";
    // put your bad run IDs here
    vBRList = {
        27021034,
        27023020,
        27023059,
        27024001,
        27021019,
        27022052,
        27023051,
        27024014,
        27024030,
        27024031,
        27024033,
        27024075
    };
    std::cout << "[LOG] - BadRunTool: Dataset name: " << dstName << ", w/ " << vBRList.size() << " bad runs." << std::endl;
}

bool BadRunTool::IsBadRun(int runId) {
    return std::count(vBRList.begin(), vBRList.end(), runId) > 0;
}


#endif