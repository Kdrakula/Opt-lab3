#include"user_funs.h"
#include <functional>

matrix ff3T(matrix x, matrix ud1, matrix ud2) {
    matrix y;
    return y;
}

matrix ff3R(matrix x, matrix ud1, matrix ud2) {
    // Parametry problemu
    double m = 0.6;         // masa (kg)
    double r = 0.12;        // promień (m)
    double C = 0.47;        // współczynnik oporu
    double rho = 1.2;       // gęstość powietrza (kg/m^3)
    double g = 9.81;        // przyspieszenie grawitacyjne (m/s^2)
    double omega = 10;      // rotacja w kierunku[−15,15](rad/s)
    double v0_x = 5;        // początkowa prędkość pozioma [−10,10](m/s)
    double v0_y = 0;        // początkowa prędkość pionowa (m/s)
    double t0 = 0.0;        // czas początkowy (s)
    double dt = 0.01;       // krok czasowy (s)
    double tend = 7.0;      // czas końcowy (s)

    // Warunki poczatkowe
    matrix Y0(4, 1);
    Y0(0) = 0;    // x(0)
    Y0(1) = 100;  // y(0)
    Y0(2) = v0_x; // vx(0)
    Y0(3) = v0_y; // vy(0)

    // Parametry dodatkowe
    ud1(0) = m;
    ud1(1) = r;
    ud1(2) = C;
    ud1(3) = rho;
    ud1(4) = omega;
    ud1(5) = g;

    matrix *Y = solve_ode(df3, t0, dt, tend, Y0, ud1, ud2);
    matrix result = Y[1](0, 0);
    return result;
}

matrix df3(double t, matrix Y, matrix ud1, matrix ud2) {
    double m = ud1(0);
    double r = ud1(1);
    double C = ud1(2);
    double rho = ud1(3);
    double omega = ud1(4);
    double g = ud1(5);

    double S = M_PI * r * r; // Powierzchnia przekroju piłki

    // Rozpakowanie zmiennych stanu
    double x = Y(0);
    double y = Y(1);
    double vx = Y(2);
    double vy = Y(3);

    // Siły
    double Dx = 0.5 * C * rho * S * vx * abs(vx);
    double Dy = 0.5 * C * rho * S * vy * abs(vy);
    double Fx = rho * vy * omega * M_PI * r * r * r;
    double Fy = rho * vx * omega * M_PI * r * r * r;

    // Równania różniczkowe
    matrix dY(4, 1);
    dY(0) = vx;                              // dx/dt
    dY(1) = vy;                              // dy/dt
    dY(2) = -(Dx + Fx) / m;                  // dvx/dt
    dY(3) = -(Dy + Fy + m * g) / m;          // dvy/dt
    return dY;
}