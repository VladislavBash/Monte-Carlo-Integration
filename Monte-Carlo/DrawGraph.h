#ifndef MONTE_CARLO_DRAW_H
#define MONTE_CARLO_DRAW_H

#include "Monte-Carlo.hpp"
#include <string>
#include "gnuplot.h"

void DrawGraph(int num, double a, double b, double e, double minus_border, double border, std::string function) {
    GnuplotPipe gp;
    std::stringstream ss; 
    std::ofstream data("data.dat"); 
    std::ofstream line("line.dat"); 
    std::string namefunction = function; 
    line << 0 << ' ' << minus_border << ' ' << '\n';  
    line << 0 << ' ' << border << ' ' << '\n'; 
    line.close(); 
    for (int i = 0; i < num; ++i) { 
        double x = random(a, b); 
        double y = random(minus_border, border); 
        double yf = f(x, function); 
        if (y < yf && 0 < y) 
            data << x << ' ' << y << ' ' << '0' << '\n'; 
        if (y > yf && 0 > y) 
            data << x << ' ' << y << ' ' << '1' << '\n'; 
        if ((y < yf && 0 > y) || (y > yf && 0 < y)) 
            data << x << ' ' << y << ' ' << '2' << '\n'; 
    }
    data.close(); 
    int n = 0; 
    while (function.find("^") != -1) { 
        n = function.find("^"); 
        function.replace(n, 1, "**"); 
    }
    while (function.find("e") != -1) { 
        n = function.find("e"); 
        function.replace(n, 1, "2.71828"); 
    }
    ss << " set yr [" << minus_border << ":" << border << "]\n"; 
    gp.sendLine(ss.str()); 
    ss.clear(); 
    ss << " set xr [" << a << ":" << b << "]\n"; 
    gp.sendLine(ss.str()); 
    ss.clear(); 
    gp.sendLine("set key textcolor '#ed9a09'\n"); 
    gp.sendLine("set palette model RGB maxcolors 3\n"); 
    gp.sendLine("set palette defined ( 0 '#1B8E13', 1 '#EF0000', 2 '#13238E')\n"); 
    ss << "plot 0 title 'x' lc '#000000', (\"line.dat\") title 'y' with lines lc '#000000' lt 1, " << function << "title '" << namefunction << "' lc '#319208', ";
    ss.clear(); 
    ss << "(\"data.dat\") title 'points' with points pt 7 ps 0.1 palette"; 
    gp.sendLine(ss.str()); 
}

#endif // MONTE_CARLO_DRAW_H
