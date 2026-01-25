#include <iostream>
#include "TMath.h"
#include "VtxShiftTool.h"

VtxShiftTool::VtxShiftTool() {

    // put your vx and by shift here
    const char* name = "4p5";
    vx0 = -0.42;
    vy0 = -2.0;

    // put your vertex cut here
    vzLow = 199;
    vzHigh = 200.6;
    vrCut = 2;

    std::cout << "[LOG] VtxShiftTool: Current data set: [" << name << "]" << std::endl;
    std::cout << "\tVx will shift: " << vx0 << ", Vy will shift: " << vy0 << std::endl;  
    std::cout << "\tVz range: " << vzLow << " -> " << vzHigh << std::endl;  
    std::cout << "\tVr cut: " << vrCut << std::endl;  
}

double VtxShiftTool::GetShiftedVr(double vx, double vy) {
    vx = vx - vx0;
    vy = vy - vy0;
    return TMath::Sqrt(vx*vx + vy*vy);
}