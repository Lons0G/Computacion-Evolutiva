% Definir la función
f = @(x1, x2) (4 - 2.1*x1.^2 + (x1.^4)/3) .* x1.^2 + x1.*x2 + (-4 + 4*x2.^2) .* x2.^2;

% Crear una malla de puntos para x1 y x2
x1 = linspace(-3, 3, 100);  % Rango de x1
x2 = linspace(-2, 2, 100);  % Rango de x2
[X1, X2] = meshgrid(x1, x2); % Matrices de coordenadas

% Evaluar la función en cada punto de la malla
Z = f(X1, X2);

% Graficar en 3D
figure;
surf(X1, X2, Z); % Superficie 3D
title('Gráfica 3D de f(x_1, x_2)');
xlabel('x_1'); ylabel('x_2'); zlabel('f(x_1, x_2)');
colorbar; % Barra de colores para referencia
shading interp; % Suavizado de colores
