#include "doctest.h"
#include "Monte-Carlo.hpp"
#include <math.h>
#include <iomanip>
#include <vector>
#define pi 3.1415926535897932
#define e  2.7182818284590452

bool compare(double a, double b, int const sampling) { // необходимо для double и float
    if (std::round(a*sampling) == std::round(b*sampling)) {
        return true;
    }
    return false;
}

TEST_CASE("CHECK_RATIO") {
    REQUIRE(getRatio(1) == 0.0);
    REQUIRE(getRatio(2) == 31.599);
    REQUIRE(getRatio(11) == 4.436);
    REQUIRE(getRatio(22) == 3.792);
    REQUIRE(getRatio(30) == 3.645);
    REQUIRE(getRatio(60) == 3.460);
    REQUIRE(getRatio(120) == 3.373);
    REQUIRE(getRatio(12000000) == 3.373);
    REQUIRE(getRatio(1147483646) == 3.290);
}

TEST_CASE("CHECK_PARSER") {
    double numRound = 1000;
    double x = random(-10, 10);
    REQUIRE(compare(f(x, "abs(x)"), abs(x), 10000));
    x = random(0, 10);
    REQUIRE(compare(f(x, "x"), x, numRound));
    REQUIRE(compare(f(x, "x+x"), 2*x, numRound));
    REQUIRE(compare(f(x, "x-x"), 0, numRound));
    REQUIRE(compare(f(x, "x/x"), 1, numRound));
    REQUIRE(compare(f(x, "1/x"), 1/x, numRound));
    REQUIRE(compare(f(x, "x/1"), x, numRound));
    REQUIRE(compare(f(x, "2*x"), 2*x, numRound));
    REQUIRE(compare(f(x, "x/2"), x/2, numRound));
    REQUIRE(compare(f(x, "x+1^x"), x+1, numRound));
    REQUIRE(compare(f(x, "x^x"), pow(x, x), numRound));
    REQUIRE(compare(f(x, "x^x+1"), pow(x, x)+1, numRound));
    REQUIRE(compare(f(x, "x^(x+1)"), pow(x,x+1), numRound));
    REQUIRE(compare(f(x, "(x^x)+1"), pow(x,x)+1, numRound));
    REQUIRE(compare(f(x, "(x+1)^x"), pow(x+1, x), numRound));
    REQUIRE(compare(f(x, "1+x^x"), pow(x,x)+1, numRound));
    REQUIRE(compare(f(x, "(1+x)^x"), pow(1+x,x), numRound));
    REQUIRE(compare(f(x, "e^x"), pow(e, x), numRound));
    REQUIRE(compare(f(x, "pi^x"), pow(pi, x), numRound));
}

TEST_CASE("CHECK_RANDOM") {
    int a = 20;
    int b = 40;
    int n = 100000;
    double summ = 0;
    for (int j = 0; j < n; j++)
        summ += random(a, b);
    REQUIRE(std::round(2*summ/(a+b)/n) == 1);
}

TEST_CASE("CHECK_MAX") {
    int j = 0;
    double a = 0;
    double b = random(2, 20);
    double c = 0;
    double d = 0;
    double xmax = 0;
    double xmin = 0;
    double step = 0.001;
    double minus_border = 0;
    double border = 0;
    std::vector<std::string> vec;
    std::vector<double> val;
    vec.push_back("1");
    val.push_back(0.0);
    vec.push_back("abs(x)");
    val.push_back(b);
    vec.push_back("-abs(x)");
    val.push_back(0.0);
    vec.push_back("x^2");
    val.push_back(b);
    vec.push_back("e^x");
    val.push_back(b);
    vec.push_back("pi^x");
    val.push_back(b);
    vec.push_back("x^x");
    val.push_back(b);
    vec.push_back("1+2*x");
    val.push_back(b);
    vec.push_back("1-x");
    val.push_back(0.0);
    for (std::string function: vec) {
        higher_lower__point_x_and_minus_border(a, b, step, function, xmax, xmin, c, d);
        REQUIRE(compare(xmax, val.at(j), 100));
        j++;
    }
}

TEST_CASE("CHECK_RANDOM") {
    int a = 20;
    int b = 40;
    int n = 100000;
    double summ = 0;
    for (int j = 0; j < n; j++)
        summ += random(a, b);
    REQUIRE(std::round(2*summ/(a+b)/n) == 1);
}

TEST_CASE("CHECK_C_D") {
    int j = 0;
    double a = 0;
    double b = 10;
    double c = 0;
    double d = 0;
    double xmax = 0;
    double xmin = 0;
    double step = 0.001;
    double minus_border = 0;
    double border = 0;
    std::vector<std::string> vec;
    std::vector<double> val;
    vec.push_back("(x-5)^2-5");
    val.push_back(5-sqrt(5));
    val.push_back(5+sqrt(5));
    for (std::string function: vec) {
        higher_lower__point_x_and_minus_border(a, b, step, function, xmax, xmin, c, d);
        REQUIRE(compare(c, val.at(j++), 100));
        REQUIRE(compare(d, val.at(j++), 100));
    }
}

TEST_CASE("CHECK_CROSS") {
    // double a = -20;
    // double b = 20;
    double er = 0.01;
    std::vector<std::string> vec;
    std::vector<double> val;
    std::vector<double> a;
    std::vector<double> b;

    vec.push_back("x+10");
    a.push_back(-20);
    b.push_back(20);
    val.push_back(-10);
    vec.push_back("x^2-10");
    a.push_back(0);
    b.push_back(5);
    val.push_back(3.16);
    vec.push_back("x^3-50");
    a.push_back(0);
    b.push_back(5);
    val.push_back(3.68);
    // REQUIRE(val[0] == halfSplit(a, b, er, vec[0]));
    for (int k = 0; k < vec.size(); k++) {
        REQUIRE(compare(val[k], halfSplit(a[k], b[k], er, vec[k]), 10));
    }
}

TEST_CASE("CHECK_EXTR") {
    // double a = -20;
    // double b = 20;
    double er = 0.01;
    std::vector<std::string> vec;
    std::vector<double> val;
    std::vector<double> a;
    std::vector<double> b;

    // vec.push_back("x+10");
    // a.push_back(-20);
    // b.push_back(20);
    // val.push_back(20);
    vec.push_back("x^2");
    a.push_back(8);
    b.push_back(8.05);
    val.push_back(0);
    // vec.push_back("(x-5)^2-5");
    // a.push_back(0);
    // b.push_back(11);
    // val.push_back(5);
    // vec.push_back("-abs(x-10)");
    // a.push_back(0);
    // b.push_back(11);
    // val.push_back(10);
    // vec.push_back("sin(x)");
    // a.push_back(0);
    // b.push_back(pi);
    // val.push_back(pi/2);
    // REQUIRE(val[0] == halfExtr(a, b, er, vec[0]));
    for (int k = 0; k < vec.size(); k++) {
        REQUIRE(val[k] == halfExtr(a[k], b[k], er, vec[k]));
        REQUIRE(compare(val[k], halfExtr(a[k], b[k], er, vec[k]), 10));
    }
}

TEST_CASE("CHECK_EXTR2") {
    // double er = 0.01;
    // std::vector<std::string> vec;
    // std::vector<double> val;
    // std::vector<double> a;
    // std::vector<double> b;

    // vec.push_back("x+10");
    // a.push_back(-20);
    // b.push_back(20);
    // val.push_back(20);
    // higher_lower__point_x_and_minus_border(a[k], b[k], er, vec[k], xmax, xmin, c, d);
    double a = -10;
    double b = 10;
    double c = 0;
    double d = 0;
    double step = (b-a)/100;
    std::string function = "(x-5)^2-5";
    double border = 0;
    double minus_border = 0;
    higher_lower_point2(a, b, c, d, step, function, border, minus_border);
    REQUIRE(compare(minus_border, -5, 10));
}

TEST_CASE("CHECK_CROSS2") {
    // double er = 0.01;
    // std::vector<std::string> vec;
    // std::vector<double> val;
    // std::vector<double> a;
    // std::vector<double> b;

    // vec.push_back("x+10");
    // a.push_back(-20);
    // b.push_back(20);
    // val.push_back(20);
    // higher_lower__point_x_and_minus_border(a[k], b[k], er, vec[k], xmax, xmin, c, d);
    double a = -10;
    double b = 10;
    double c = 0;
    double d = 0;
    double step = (b-a)/100;
    std::string function = "(x-5)^2-5";
    double border = 0;
    double minus_border = 0;
    higher_lower_point2(a, b, c, d, step, function, border, minus_border);
    // REQUIRE(d == 7.23);
    REQUIRE(compare(d, 7.23, 1));
    REQUIRE(compare(c, 2.76, 1));
}

TEST_CASE("CHECK_CARTESIAN") {
    int j = 0;
    double minus_border = 0;
    double border = 0;
    double a = 0;
    double b = 10;
    double c = 0;
    double d = 0;
    int num = (int)random(2000, 2000); // можно выбрать точность (лучше от 1000 точек, иначе большие числа может не посчитать)
    int count = 10;
    std::vector<std::string> vec;
    std::vector<double> val;
    double truth = 0;
    double accur = 0;
    double step = (b-a)/100;;
    
    vec.push_back("(x-5)^2-5");
    val.push_back(33.333);
    vec.push_back("x");
    val.push_back(50);
    vec.push_back("10+2*x");
    val.push_back(200);
    vec.push_back("x^2");
    val.push_back(333.333);
    vec.push_back("1+x+x^2");
    val.push_back(393.333);
    vec.push_back("e^x");
    val.push_back(22025);
    printf("\nCatesian coordinate system\n");
    for (std::string function : vec) {
        std::cout << "\n" << function << "\n";
        startCond(a, b, c, d, step, function, minus_border, border);
        calcIntegral(a, b, c, d, function, minus_border, border, num, count, truth, accur);
        REQUIRE(val.at(j) < truth + accur);
        REQUIRE(val.at(j) > truth - accur);
        j++;
    }
}

TEST_CASE("CHECK_POLAR") {
    int j = 0;
    double minus_border = 0;
    double border = 0;
    double a = 0;
    double b = 10;
    double c = 0;
    double d = 0;
    int num = (int)random(2000, 2000); // можно выбрать точность (лучше от 1000 точек, иначе большие числа может не посчитать)
    int count = 10;
    std::vector<std::string> vec;
    std::vector<double> val;
    double truth = 0;
    double accur = 0;
    double step = (b-a)/100;;
    

    vec.push_back("x");
    val.push_back(166.66);
    vec.push_back("10+2*x");
    val.push_back(2167.67);
    vec.push_back("x^2");
    val.push_back(10000);
    vec.push_back("1+x+x^2");
    val.push_back(13055);
    vec.push_back("e^x");
    val.push_back(121000000);
    printf("\nPolar coordinate system\n");
    for (std::string function : vec) {
        function = "0.5*(" + function + ")^2";
        std::cout << "\n" << function << "\n";
        startCond(a, b, c, d, step, function, minus_border, border);
        calcIntegral(a, b, c, d, function, minus_border, border,num, count, truth, accur);
        REQUIRE(val.at(j) < truth + accur);
        REQUIRE(val.at(j) > truth - accur);
        j++;
    }
}