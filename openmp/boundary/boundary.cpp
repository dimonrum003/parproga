#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <omp.h>

const double X_START = -10.0;
const double X_END = 10.0; 
const double ERROR_TOLERANCE = 1e-6;   
const int MAX_ITERATIONS = 100000;  

double compute_f (double y, double a)
{
    return a * (y - y * y * y);
}

double compute_d_f (double y, double a)
{
    return a * (1 - 3 * y * y);
}

void thomas_algorithm_solver (const std::vector<double> &lower_diag, 
                              const std::vector<double> &main_diag, 
                              const std::vector<double> &upper_diag, 
                              const std::vector<double> &rhs, 
                              std::vector<double> &solution)
{
    int N = rhs.size();
    std::vector<double> temp_upper(N, 0.0);
    std::vector<double> temp_rhs(N, 0.0);

    temp_upper[0] = upper_diag[0] / main_diag[0];
    temp_rhs[0] = rhs[0] / main_diag[0];
    for (int i = 1; i < N; ++i)
    {
        double factor = main_diag[i] - lower_diag[i] * temp_upper[i - 1];
        temp_upper[i] = upper_diag[i] / factor;
        temp_rhs[i] = (rhs[i] - lower_diag[i] * temp_rhs[i - 1]) / factor;
    }

    solution[N - 1] = temp_rhs[N - 1];
    for (int i = N - 2; i >= 0; --i)
    {
        solution[i] = temp_rhs[i] - temp_upper[i] * solution[i + 1];
    }
}

void numerov_newton_method_solver (double a, double y_start, double y_end, const std::string &output_filename)
{
    double step_size = 1.0 / (10.0 * std::sqrt(a));
    int num_points = static_cast<int>((X_END - X_START) / step_size) + 1;

    std::vector<double> x(num_points), y(num_points, 0.0), f_values(num_points, 0.0);

    for (int i = 0; i < num_points; ++i)
    {
        x[i] = X_START + i * step_size;
    }

    y[0] = y_start;
    y[num_points - 1] = y_end;

    for (int iter = 0; iter < MAX_ITERATIONS; ++iter)
    {
        std::vector<double> lower_diag(num_points - 2, 1.0);
        std::vector<double> main_diag(num_points - 2, -2.0);
        std::vector<double> upper_diag(num_points - 2, 1.0);
        std::vector<double> rhs(num_points - 2, 0.0); 

        #pragma omp parallel for
        for (int i = 1; i < num_points - 1; ++i)
        {
            double f_val = compute_f(y[i], a);
            double f_derivative = compute_d_f(y[i], a);
            double correction = (y[i - 1] - 2 * y[i] + y[i + 1]) / (step_size * step_size) - f_val;

            if (i > 1) lower_diag[i - 1] = 1.0;
            main_diag[i - 1] = -2.0 - step_size * step_size * f_derivative;
            if (i < num_points - 2) upper_diag[i - 1] = 1.0;

            rhs[i - 1] = -correction * step_size * step_size;
        }

        std::vector<double> delta_y(num_points - 2, 0.0);
        thomas_algorithm_solver(lower_diag, main_diag, upper_diag, rhs, delta_y);

        #pragma omp parallel for
        for (int i = 1; i < num_points - 1; ++i)
        {
            y[i] += delta_y[i - 1];
        }

        double max_error = 0.0;
        #pragma omp parallel for reduction(max : max_error)
        for (int i = 1; i < num_points - 1; ++i)
        {
            double f_val = (y[i - 1] - 2 * y[i] + y[i + 1]) / (step_size * step_size) - compute_f(y[i], a);
            max_error = std::max(max_error, std::fabs(f_val));
        }

        if (max_error < ERROR_TOLERANCE)
        {
            break;
        }
    }

    std::ofstream results(output_filename);
    if (results.is_open())
    {
        for (int i = 0; i < num_points; ++i)
        {
            results << x[i] << " " << y[i] << "\n";
        }
        results.close();
        std::cout << "Решение для a = " << a << " записано в файл: " << output_filename << std::endl;
    } else {
        std::cerr << "Не удалось открыть файл для записи: " << output_filename << std::endl;
    }
}

int main(int argc, char **argv)
{
    double y_start = sqrt(2.0);
    double y_end = sqrt(2.0);

    int num_threads = omp_get_max_threads();
    omp_set_num_threads(num_threads);
    // std::cout << "Используется " << num_threads << " потоков.\n";

    // Параметры a
    std::vector<double> a_values = {100, 1000, 10000, 100000, 1000000};

    // Для каждого a решаем задачу и записываем в файл
    for (double a : a_values)
    {
        std::string output_filename = "numerov_solution_a_" + std::to_string(static_cast<int>(a)) + ".txt";
        numerov_newton_method_solver(a, y_start, y_end, output_filename);
    }

    return 0;
}
