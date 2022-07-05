#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Monte-Carlo.hpp"
#include "DrawGraph.h"
#include "DrawDepend.h"

void calcIntegral(double a, double b, double c, double d, std::string function, double &minus_border, double &border, int num, int count, double &truth, double &accur);
double accuracy(double * S, int num, int count);
void DrawGraph(int num, double a, double b, double e, double minus_border, double border, std::string function);
void DrawDepend(double a, double b, double e, double minus_border, double border, std::string function);
void startCond(double a, double b, double &c, double &d, double e, std::string function, double &minus_border, double &border);

int main() {
    int choice = 1;
    printf("Press 1 for cartesian coordinate system or 2 for polar coordinate system ");
    scanf("%d", &choice);
    if (choice != 1 && choice != 2) {
        printf("EROR: you have not wrote 1 or 2");
        exit(0);
    }
    std::string function = "";
    printf("Write the function ");
    std::cin >> function; // вводим функцию (в будущем она запарсится)
    if (choice == 2) {
        printf("Do it in polar coordinate system is unreal, but we can use formula f = 0.5*(f)^2\n");
        function = "0.5*(" + function + ")^2";
    }
    int num = 50; // кол-во точек
    printf("Set count of dots (10^5 dots - 3 sec for each integral) ");
    scanf("%d", &num);
    double a = -5; // предел интегрирования слева
    printf("Set left limit of integration ");
    scanf("%lf", &a);
    double b = 5; // предел интегрирования справа
    printf("Set right limit of integration ");
    scanf("%lf", &b);
    double step = (b-a)/1000; // дискретизация
    int count = 10;
    printf("Set count of integrals ");
    scanf("%d", &count);
    int pointsOnGraph = 8;
    printf("Set dots for graph (min 8 dots) ");
    scanf("%d", &pointsOnGraph);
    if (pointsOnGraph < 8)
        pointsOnGraph = 8;
    printf("Are you sure to continue?\n");
    printf("Press 1 (is yes) or another number (is no)\n");
    int flag = 1;
    scanf("%d", &flag);
    if (flag != 1)
        exit(0);
    double c = 0;
    double d = 0;
    double xmin = 0;
    double xmax = 0;
    double border = 0;
    double minus_border = 0;
    double truth = 0;
    double accur = 0;
    int counter = 0;
    startCond(a, b, c, d, step, function, minus_border, border);
    calcIntegral(a, b, c, d, function, minus_border, border, num, count, truth, accur);
    printf("\nResult:  %lf +- %lf\n", truth, accur);
    printf("Absolute error:  %lf\n", accur);
    printf("Relative error:  %lf%%\n\n", accur/abs(truth)*100);
    DrawGraph(num, a, b, step, minus_border, border, function);
    printf("Are you sure to continue?\n");
    printf("Press 1 (is yes) or another number (is no)\n");
    flag = 0;
    scanf("%d", &flag);
    if (flag != 1)
        exit(0);
    DrawDepend(a, b, step, minus_border, border, function, pointsOnGraph);
    return 0;
}