#ifndef MONTE_CARLO_DRAW_H
#define MONTE_CARLO_DRAW_H

#include "Monte-Carlo.hpp"
#include <map>
#include <string>
#include "gnuplot.h"

void Draw(int num, double a, double b, double e, double border, std::string function) {
    GnuplotPipe gp;
    std::stringstream ss;
    int n = 0;
    while (function.find("^") != -1) { // замена ^ (использует parser) на ** (gnuplot использует)
        n = function.find("^");
        function.replace(n, 1, "**");
    }
    while (function.find("e") != -1) { // замена e (использует parser) на 2.7182818284590452 (gnuplot использует)
        n = function.find("e");
        function.replace(n, 1, "2.7182818284590452");
    }
    while (function.find("pi") != -1) { // замена pi (использует parser) на 3.1415926535897932 (gnuplot использует)
        n = function.find("pi");
        function.replace(n, 2, "3.1415926535897932");
    }
    ss << " set yr [" << 0 << ":" << border << "]\n";
    gp.sendLine(ss.str());
    ss.clear();
    ss << " set xr [" << a << ":" << b << "]\n";
    gp.sendLine(ss.str());
    ss.clear();
    gp.sendLine("set palette defined ( 0 'red', 1 'blue')\n");
    ss << "plot " << function << ", ";
    ss.clear();
    std::ofstream data("data.dat");
    for (int i = 0; i < num; ++i) {
        double x = random(a, b);
        double y = random(0, border);
        double yf = f(x, function);
        if (yf < y) {
            data << x << ' ' << y << ' ' << '0' << '\n';
        } else {
            data << x << ' ' << y << ' ' << '1' << '\n';
        }
    }
    data.close();
    ss << "(\"data.dat\") with points palette";
    gp.sendLine(ss.str());
}

#endif // MONTE_CARLO_DRAW_H
