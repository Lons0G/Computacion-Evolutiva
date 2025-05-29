#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

// FUNCIONES UTILIZADAS DEL CODIGO PROPORCIONADO
void initrandom(int seed);
double N(double m, double sigma);

// FUNCION A EVALUAR
float f(float x1, float x2) {
  float x1_2 = x1 * x1;
  float x2_2 = x2 * x2;
  float x1_4 = x1_2 * x1_2;
  return (4 - 2.1 * (x1 * x1) + (x1_4 / 3)) * x1_2 + x1 * x2 +
         (-4 + 4 * (x2_2)) * x2_2;
}

int main(int argc, char *argv[]) {

  // VERIFICACION DEL ARGUMENTO
  if (argc < 2) {
    std::cout << "falta proporcionar un valor para Gmax" << std::endl;
    return 1;
  }
  int gmax = std::atoi(argv[1]);

  // GUARDAR LOS RESULTADOS DE LAS EVALUACIONES EN UN TXT
  std::ofstream archivo("resultados.txt");
  if (!archivo.is_open()) {
    std::cerr << "Error al abrir el archivo." << std::endl;
    return 1;
  }

  // PARAMETROS
  const int n_vars = 2;
  const float c = 0.817;
  int t = 0;
  int exito = 0;

  float vars_x[2] = {0.0, 0.0};
  float vars_x_[2] = {0.0, 0.0};

  float ps = 0.0;
  float fold = 0;
  float fnew = 0;

  float *sigma = new float[gmax]();
  sigma[0] = 3.0;

  // INICIALIZACION DE VARIABLES ALEATORIAS
  initrandom(time(0));
  vars_x[0] = N(0, 1);
  vars_x[1] = N(0, 1);

  fold = f(vars_x[0], vars_x[1]);
  fnew = fold;

  archivo << "primera evaluacion f(" << vars_x[0] << ", " << vars_x[1]
          << ") = " << fold << std::endl;
  std::cout << "primera evaluacion f(" << vars_x[0] << ", " << vars_x[1]
            << ") = " << fold << std::endl;

  while (t < gmax) {
    // SEMILLA ALEATORIO
    initrandom(time(0));

    // MUTACION DE LAS VARIABLES
    vars_x_[0] = vars_x[0];
    vars_x_[1] = vars_x[1];
    for (int i = 0; i < n_vars; i++) {
      double random_value = N(0, 1);
      vars_x_[i] += sigma[t] * random_value;
    }

    // VERIFICACIOON DEL RANGO DE LAS VARIABLES
    if (vars_x_[0] < -3.0 || vars_x_[0] > 3.0) {
      if (vars_x_[0] > 0) {
        vars_x_[0] = 0 + N(0, 1);
      } else {
        vars_x_[0] = 0 - N(0, 1);
      }
    }
    if (vars_x_[1] < -2.0 || vars_x_[1] > 2.0) {
      if (vars_x_[1] > 0) {
        vars_x_[1] = 0 + N(0, 1);
      } else {
        vars_x_[1] = 0 - N(0, 1);
      }
    }

    // EVALUAR EL NUEVO PUNTO
    fnew = f(vars_x_[0], vars_x_[1]);

    // SI EL NUEVO PUNTO ES MEJOR QUE EL ANTERIOR
    if (fnew < fold) {
      exito++;
      fold = fnew;
      for (int i = 0; i < n_vars; i++) {
        vars_x[i] = vars_x_[i];
      }
    }

    // ACTUALIZAR VALOR DE PS CADA 10 * N GENERACIONES
    if (t % (10 * n_vars) == 0) {
      ps = (float)exito / (10 * (float)n_vars);
      exito = 0;
    }

    archivo << "evaluacion " << t << ": f(" << vars_x[0] << ", " << vars_x[1]
            << ") = " << fold << std::endl;

    t++;

    // ACTUALIZACION DEL VALOR DE SIGMA
    if (t > 0 && t % n_vars == 0) {
      if (ps > 0.2) {
        sigma[t] = sigma[t - n_vars] / c;
      } else if (ps < 0.2) {
        sigma[t] = sigma[t - n_vars] * c;
      } else {
        sigma[t] = sigma[t - n_vars];
      }
    } else if (t > 0) {
      sigma[t] = sigma[t - 1];
    }
  }
  std::cout << "evaluacion " << t << ": f(" << vars_x[0] << ", " << vars_x[1]
            << ") = " << fold << std::endl;

  delete[] sigma;
  archivo.close();
  return 0;
}
