#ifndef __VTXSHIFTTOOL__
#define __VTXSHIFTTOOL__

class VtxShiftTool {

    private:
        double vx0;
        double vy0;

        double vzLow;
        double vzHigh;
        double vrCut;
    
    public:
        VtxShiftTool();
        ~VtxShiftTool(){}

        double GetShiftedVr(double vx, double vy);
        double IsGoodVertex(double vx, double vy, double vz) {
            auto vr = GetShiftedVr(vx, vy);
            return (vz < vzHigh) && (vz > vzLow) && (vr < vrCut) && (vr > -vrCut);
        }
};

#endif