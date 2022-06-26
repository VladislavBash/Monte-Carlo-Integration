#define MONTE_CARLO_DRAW_H

#include "Monte-Carlo.hpp"
#include <string>
#include "gnuplot.h"

// #endif // MONTE_CARLO_DRAW_H
int increase(double &numb, double ratio) { 
    return numb*=ratio; 
}

double compute(double a, double b, std::string g) { 
    return f(b, g) - f(a, g); 
}

void DrawDepend(double a, double b, double step, double minus_border, double border, std::string function, int pointsOnGraph) { //
    GnuplotPipe gp;
    std::stringstream ss; 
    double hight = 0; 
    double low = 0; 
    int count = 1; 
    double truth = 0; 
    double accur = 0; 
    int limit = pow(10, 6); 
    double ratio = pow(limit, 1/(double)(pointsOnGraph)); 
    double numb = ratio; 
    std::vector<std::string> vecFunction;  
    std::vector<std::string> vecPrimitive; 
    vecFunction.push_back("(x-5)^2-5"); 
    vecPrimitive.push_back("(x-5)^3/3-5*x");
    vecFunction.push_back("x");
    vecPrimitive.push_back("x^2/2");
    vecFunction.push_back("10+2*x");
    vecPrimitive.push_back("x^2+10*x");
    vecFunction.push_back("x^2");
    vecPrimitive.push_back("x^3/3");
    vecFunction.push_back("1+x+x^2");
    vecPrimitive.push_back("x+x^2/2+x^3/3");
    vecFunction.push_back("e^x");
    vecPrimitive.push_back("e^x");
    double lineThroughtLastPoint = 0; 
    std::ofstream data("speed.dat"); 
    double lastNum = 0;  
    double c = 0; 
    double d = 0; 
    startCond(a, b, c, d, step, function, minus_border, border); 
    for (numb; numb < limit; increase(numb, ratio)) { 
        calcIntegral(a, b, c, d, function, minus_border, border, (int)numb, count, truth, accur); 
        lineThroughtLastPoint = truth; 
        hight = std::max(hight, truth); 
        low = std::min(low, truth); 
        data << numb << ' ' << truth << ' ' << "#ed9a09" << '\n'; 
        lastNum = numb; 
    }
    data.close(); 
    ss << " set yr [" << 2*low << ":" << 2*hight << "]\n"; 
    gp.sendLine(ss.str()); 
    ss.clear(); 
    ss << " set xr [" << -2*lastNum << ":" << 2*lastNum << "]\n"; 
    gp.sendLine(ss.str()); 
    ss.clear(); 
    ss << "set logscale x " << std::to_string(ratio) << "\n"; 
    gp.sendLine(ss.str()); 
    gp.sendLine("set xtics (1, 10, 100, 1000, 10000, 100000, 1000000)\n"); 
    gp.sendLine("set ylabel 'error'\n"); 
    gp.sendLine("set xlabel 'count of dots'\n"); 
    gp.sendLine("set key textcolor '#ed9a09'\n"); 
    ss.clear(); 
    bool flag = true; 
    for (int k = 0; k < std::size(vecFunction); k++) { 
        if (vecFunction[k] == function) { 
            ss << "plot " << compute(a, b, vecPrimitive[k]) << ", (\"speed.dat\") title 'Depend line' with linespoints ls 7"; 
            flag = false; 
            break; 
        }
    }
    if (flag) 
        ss << "plot " << lineThroughtLastPoint << ", (\"speed.dat\") title 'Depend line' with linespoints ls 7"; 
}
