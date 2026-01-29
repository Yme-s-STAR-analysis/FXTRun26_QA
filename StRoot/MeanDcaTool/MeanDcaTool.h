#ifndef __MEAN_DCA_TOOL__
#define __MEAN_DCA_TOOL__

class TF1;
class TProfile;
class StPicoDst;

class MeanDcaTool {

    private:
        // note: here xy/XY means sDCAxy
        TF1* funcUpperZ;
        TF1* funcLowerZ;
        TF1* funcUpperXY;
        TF1* funcLowerXY;

        bool haveCache;

        std::vector<double> vDCAz;
        std::vector<double> vDCAxy;

    public:

        double mDCAz; // mean
        double mDCAxy;
        double sDCAz; // std dev
        double sDCAxy;

        MeanDcaTool();
        ~MeanDcaTool(){}

        void clean();

        void SetUpperCurveParZ(double, double, double);
        void SetLowerCurveParZ(double, double, double);
        void SetUpperCurveParXY(double, double, double);
        void SetLowerCurveParXY(double, double, double);
        void ReadParams(); // from header file

        // when you want to fill histogram or profile, use Make() and get mDCAz/xy
        bool Make(StPicoDst* pico); 

        // when you just want to know an event is good or bad
        bool IsBadMeanDcaZEvent(StPicoDst* pico);
        bool IsBadMeanDcaXYEvent(StPicoDst* pico);
        bool IsBadMeanDcaZEvent(StPicoDst* pico, int mult);
        bool IsBadMeanDcaXYEvent(StPicoDst* pico, int mult);

        // getters
        double GetMeanZ() { return mDCAz; }
        double GetStdDevZ() { return sDCAz; }
        double GetMeanXY() { return mDCAxy; }
        double GetStdDevXY() { return sDCAxy; }
};

#endif
