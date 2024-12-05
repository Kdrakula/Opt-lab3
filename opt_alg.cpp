#include "opt_alg.h"

solution pen(matrix(*ff)(matrix, matrix, matrix), matrix x0, double c, double dc, double epsilon, int Nmax, matrix ud1, matrix ud2) {
    try {
        double alpha = 1.0, beta = 0.5, gamma = 2.0, delta = 0.5, s = 0.5;
        solution Xopt(x0), Xopt_new;
        matrix penalty_coeff(2, new double[2]{ c, dc });

        while (true) {
            Xopt_new = sym_NM(ff, Xopt.x, s, alpha, beta, gamma, delta, epsilon, Nmax, ud1, penalty_coeff);

             cout << "x_opt: " << Xopt.x(0) << ", " << Xopt.x(1) << std::endl;
             std::cout << "y_opt: " << Xopt.y(0) << std::endl;
             std::cout << "Liczba wywo�a� funkcji celu: " << solution::f_calls << std::endl;

            if (norm(Xopt_new.x - Xopt.x) < epsilon || solution::f_calls > Nmax) {
                Xopt_new.flag = 0;
                return Xopt_new;
            }

            Xopt = Xopt_new;
            penalty_coeff(0) = penalty_coeff(0) * dc;
        }
    } catch (string ex_info) {
        throw ("solution pen(...):\n" + ex_info);
    }
}

solution sym_NM(matrix(*ff)(matrix, matrix, matrix), matrix x0, double s, double alpha, double beta, double gamma, double delta, double epsilon, int Nmax, matrix ud1, matrix ud2) {
    try {
        int dim = get_len(x0);
        matrix identity = ident_mat(dim);
        int vertices_count = dim + 1;
        auto* simplex = new solution[vertices_count];

        simplex[0].x = x0;
        simplex[0].fit_fun(ff, ud1, ud2);

        for (int i = 1; i < vertices_count; ++i) {
            simplex[i].x = simplex[0].x + s * identity[i - 1];
            simplex[i].fit_fun(ff, ud1, ud2);
        }

        solution reflected, expanded, contracted;
        matrix centroid;
        int idx_min, idx_max;

        while (true) {
            idx_min = idx_max = 0;
            for (int i = 1; i < vertices_count; ++i) {
                if (simplex[i].y(0) < simplex[idx_min].y(0))
                    idx_min = i;
                if (simplex[i].y(0) > simplex[idx_max].y(0))
                    idx_max = i;
            }

            centroid = matrix(dim, 1);
            for (int i = 0; i < vertices_count; ++i) {
                if (i != idx_max) centroid = centroid + simplex[i].x;
            }
            centroid = centroid / dim;

            reflected.x = centroid + alpha * (centroid - simplex[idx_max].x);
            reflected.fit_fun(ff, ud1, ud2);

            if (reflected.y(0) < simplex[idx_max].y(0) && simplex[idx_min].y(0) <= reflected.y(0)) {
                simplex[idx_max] = reflected;
            } else if (reflected.y(0) < simplex[idx_min].y(0)) {
                expanded.x = centroid + gamma * (reflected.x - centroid);
                expanded.fit_fun(ff, ud1, ud2);

                simplex[idx_max] = (expanded.y(0) < reflected.y(0)) ? expanded : reflected;
            } else {
                contracted.x = centroid + beta * (simplex[idx_max].x - centroid);
                contracted.fit_fun(ff, ud1, ud2);

                if (contracted.y(0) < simplex[idx_max].y(0)) {
                    simplex[idx_max] = contracted;
                } else {
                    for (int i = 0; i < vertices_count; ++i) {
                        if (i != idx_min) {
                            simplex[i].x = delta * (simplex[i].x + simplex[idx_min].x);
                            simplex[i].fit_fun(ff, ud1, ud2);
                        }
                    }
                }
            }

            double max_distance = norm(simplex[idx_min].x - simplex[0].x);
            for (int i = 1; i < vertices_count; ++i) {
                double dist = norm(simplex[idx_min].x - simplex[i].x);
                if (dist > max_distance) max_distance = dist;
            }

            if (max_distance < epsilon) return simplex[idx_min];
        }
    } catch (std::string ex_info) {
        throw ("solution sym_NM(...):\n" + ex_info);
    }
}