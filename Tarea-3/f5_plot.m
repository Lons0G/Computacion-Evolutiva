% F5 de De Jong en Octave

% Constantes
K = 500;
cj = 1:25;

% Matriz aij (25 columnas, 2 filas)
aij = [
  -32, -16,  0,  16, 32, -32, -16,  0,  16, 32, -32, -16,  0,  16, 32, -32, -16,  0,  16, 32, -32, -16,  0,  16, 32;
  -32, -32, -32, -32, -32, -16, -16, -16, -16, -16,   0,   0,   0,   0,  0,   16,  16, 16, 16, 16, 32, 32, 32, 32, 32
];

% Definir la función F5
function val = f5(x1, x2, aij, cj, K)
  sumatoria = 0;
  for j = 1:25
    temp = cj(j);
    temp += (x1 - aij(1,j))^6;
    temp += (x2 - aij(2,j))^6;
    sumatoria += 1 / temp;
  end
  val = 1 / (1/K + sumatoria);
end

% Crear la malla de puntos
x = linspace(-65.536, 65.536, 100);
y = linspace(-65.536, 65.536, 100);
[X, Y] = meshgrid(x, y);
Z = zeros(size(X));

% Evaluar la función en toda la malla
for i = 1:size(X, 1)
  for j = 1:size(X, 2)
    Z(i, j) = f5(X(i, j), Y(i, j), aij, cj, K);
  end
end

% Graficar
surf(X, Y, Z);
shading interp;
colorbar;
title("Función F5 de De Jong - Superficie 3D");
xlabel("x1");
ylabel("x2");
zlabel("f(x1, x2)");
