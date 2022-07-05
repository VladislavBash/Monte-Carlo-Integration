#pragma once

#include <math.h>
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "../parser/fparser.hh"
#include <vector>

double f(double x, std::string function) {
    FunctionParser fparser;
    fparser.AddConstant("pi", 3.1415926535897932);
    fparser.AddConstant("e", 2.7182818284590452);
    fparser.Parse(function, "x");
    return fparser.Eval(&x);
}

double random(double a, double b) {
    std::random_device radm;
    std::mt19937_64 mt(radm());
    std::uniform_real_distribution<double> distribution(a, b);
    return distribution(mt);
}

double intergral(double a, double b, double c, double d, double minus_border, double border, int num, std::string function) {
    int countPoints = 0;
    double x = 0;
    double y = 0;
    double compareFunction = 0;
    for (int i = 0; i < num; i++) {
        x = random(a, b);
        y = random(minus_border, border);
        compareFunction = f(x, function);
        if (y > 0 && y < compareFunction)
            countPoints++;
        if (y < 0 && y > compareFunction)
            countPoints--;
    }
    return ((double)countPoints*(b-a)*(border-minus_border))/(double)num;
}

double truth_value(double * S, int count) {
    double result = 0;
    for (int i = 0; i < count; i++) {
        result += S[i];
    }
    return result / (double)count;
}

double summ_of_delta_x(double * S, int count) {
    double result = 0;
    double truth_x = truth_value(S, count);
    for (int i = 0; i < count; i++) {
        result += pow((S[i]-truth_x), 2);
    }
    return result;
}

double getRatio(int num) {
    FILE * fp;
    double ratio = 0;
    int n = 0;
    double ratio1 = 0;
    int n1 = 0;
    fp = fopen("../../Monte-Carlo/ratios_of_Student.txt", "r");
    do {
        n1 = n;
        ratio1 = ratio;
        fscanf(fp, "%d",&n);
        fscanf(fp, "%lf",&ratio);
    } while (n <= num);
    fclose(fp);
    if (n+(n1 - n)/2 >= num)
        return ratio1;
    return ratio;
}

double accuracy(double * S, int num, int count) {
    return getRatio(num)*sqrt(summ_of_delta_x(S, count)/((count-1)*count));
}

void integral_cycle(double * S, double a, double b, double c, double d, double minus_border, double border, int num, std::string function, int count) {
    for (int j = 0; j < count; j++) {
        S[j] = intergral(a, b, c, d, minus_border, border, num, function);
    }
}

void printIntegral(double * S, int count) {
    for (int j = 0; j < count; j++) {
        printf("Integral is %lf\n", S[j]);
    }
}

void calcIntegral(double a, double b, double c, double d, std::string function, double &minus_border, double &border, int num, int count, double &truth, double &accur) {
    double * S = (double *)malloc(sizeof(double)*count);
    integral_cycle(S, a, b, c, d, minus_border, border, num, function, count);
    printIntegral(S, count);
    truth = truth_value(S, count);
    accur = abs(accuracy(S, num, count));
}

double halfSplit(double a, double b, double er, std::string function) {
    double p = (a+b)/2;;
    while (b - a > er) {
        p = (a+b)/2;
        if (f(a, function)*f(p, function) > 0) {
            a = p;
        } else {
            b = p;
        }
    }
    return p;
}

double derivative(double a, double er, std::string function) {
    return (f(a + er, function) - f(a, function)) / er; 
}

double halfExtr(double a, double b, double er, std::string function) {
    double p = (a+b)/2;
    if (derivative(a, er, function) * derivative(b, er, function) >= 0)
        return 0;
    while (b - a > er) {
        p = (a+b)/2;
        if (derivative(a, er, function)*derivative(p, er, function) > 0) {
            a = p;
        } else {
            b = p;
        }
    }
    return p;
}

void higher_lower_point(double a, double b, double &c, double &d, double step, std::string function, double &border, double &minus_border) {
    double h = 0;
    double hmin = f(a, function);;
    double hmax = hmin;
    double fun = 0;
    double funStep = 0;
    double fa = 0;
    double fb = 0;
    std::vector<double> hzero;
    for (double x = a+step; x <= b; x += step) {
        fun = f(x, function);
        funStep = f(x-step, function);
        if ((fun >= 0 && funStep >=0) || (fun <= 0 && funStep <=0)) { 
            h = f(halfExtr(x-step, x, step/10, function), function);
            fa = f(a, function);
            fb = f(b, function);
            hmin = std::min(std::min(hmin, h), std::min(fa, fb));
            hmax = std::max(std::max(hmax, h), std::max(fa, fb));
        } else {
            hzero.push_back(halfSplit(x-step, x, step, function));
        }
    }
    border = hmax;
    if (hmin > 0)
        hmin = 0;
    minus_border = hmin;
    if (hzero.size()) {
        c = hzero.at(0);
        d = hzero.at(hzero.size()-1);
    }
}

void startCond(double a, double b, double &c, double &d, double step, std::string function, double &minus_border, double &border) {
    c = 0;
    d = 0;
    border = 0;
    minus_border = 0;
    higher_lower_point(a, b, c, d, step, function, border, minus_border);
}