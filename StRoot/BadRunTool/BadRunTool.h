#ifndef __YBADRUNTOOL__
#define __YBADRUNTOOL__

#include <vector>

class BadRunTool {
    private:
        std::vector<int> vBRList;
    public:
        BadRunTool(){}
        ~BadRunTool(){}
        void Init();
        bool IsBadRun(int runId);
};

#endif