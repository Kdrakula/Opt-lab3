#include "opt_alg.h"
#include<iomanip>

void lab3();

int main() {
    try {
        lab3();
    } catch (string EX_INFO) {
        cerr << "ERROR:\n";
        cerr << EX_INFO << endl << endl;
    }
    system("pause");
    return 0;
}
//Ala spi

void lab3() {

    double c = 1.22, dc = 2.0, epsilon = 1e-3;
    int Nmax = 1000;
    matrix ud1, ud2;
    matrix x0 = rand_mat(2, 1);
    x0(0, 0) *= 1.0;  // Zakres dla x1 [-1, 1]
    x0(1, 0) *= 1.0;  // Zakres dla x2 [-1, 1]
    solution opt = pen(ff3R, x0, c, dc, epsilon, Nmax, ud1, ud2);
    cout << opt << endl << endl;

//    // Wyświetlenie wyników
//    matrix T = opt[0]; // Czas
//    matrix Y = opt[1]; // Pozycje i prędkości
//
//    bool limit = true;
//    double prev = 100;
//    for (int i = 0; i < get_size(T)[0]; ++i) {
//        if(limit)
//        while (prev > abs(Y(i, 1))) {
//            prev = Y(i, 1);
//            std::cout << "t = " << T(i)
//                      << ", x = " << Y(i, 0)
//                      << ", y = " << Y(i, 1) << std::endl;
//        }
//    }
//
//    // Zwolnienie pamięci
//    delete[] result;
}
