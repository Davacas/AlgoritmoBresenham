#include <iostream>
#include <vector>
#include "Main.h"

typedef struct punto_t {
	float x;
	float y;
} punto_t;

float xi, yi, xf, yf, cx, cy, r;
std::vector <std::vector<punto_t>> lineas;
std::vector <std::vector<punto_t>> circulos;
int cantidad_lineas = 0;
int cantidad_circulos = 0;


void dibujarLinea(float xi, float yi, float xf, float yf);
void calcLineaNeg(float xi, float yi, float xf, float yf);
void calcLineaPos(float xi, float yi, float xf, float yf);
void dibujarCirculo(float cx, float cy, float r);
void calcularCirculo(float cx, float cy, float r);

void calcularCirculo(float cx, float cy, float r) {
	float p = 1 - r;
	unsigned int i;
	punto_t punto;
	std::vector <punto_t> circulo;
	punto.x = 0;
	punto.y = r;

	while (punto.x <= punto.y) {
		if (p < 0) {
			p = p + 2 * punto.x + 3;
			punto.x ++;
		}
		else if (p >= 0) {
			p = p + (2 * punto.x) - (2 * punto.y) + 5;
			punto.x ++;
			punto.y --;
		}
		circulo.push_back(punto);
	}
	//Terminamos de calcular puntos del primer cuadrante.
	for (i = circulo.size()-1; i > 0 ; i --) {
		punto.x = circulo[i].y;
		punto.y = circulo[i].x;
		circulo.push_back(punto);
	}
	//Calculamos puntos del cuarto cuadrante.
	for (i = circulo.size() - 1; i > 0; i--) {
		punto.x = circulo[i].x;
		punto.y = -circulo[i].y;
		circulo.push_back(punto);
	}
	//Calculamos puntos del tercer y cuarto cuadrante.
	for (i = circulo.size() - 1; i > 0; i--) {
		punto.x = -circulo[i].x;
		punto.y = circulo[i].y;
		circulo.push_back(punto);
	}
	//Desplazamos cada punto a su lugar correspondiente.
	for (i = 0; i < circulo.size(); i++) {
		circulo[i].x += cx;
		circulo[i].y += cy;
	}
	circulos.push_back(circulo);
	cantidad_circulos++;
	return;
}

void dibujarCirculo(float cx, float cy, float r) {
	unsigned int i, j;
	calcularCirculo(cx, cy, r);
	
	for (i = 0; i < cantidad_circulos; i++) {
		glBegin(GL_LINE_STRIP);
		for (j = 0; j < circulos[i].size(); j++)
			glVertex2f(circulos[i][j].x, circulos[i][j].y);
		glVertex2f(circulos[i][0].x, circulos[i][0].y);
		glEnd();
	}
	glVertex2f(circulos[0][0].x, circulos[0][0].y);
	
	glutSwapBuffers();
	return;
}

void calcLineaNeg(float xi, float yi, float xf, float yf) {
	float dx, dy, dx2, dy2, pn;
	std::vector <punto_t> linea;
	punto_t punto;
	/*CALCULAMOS CONSTANTES.*/
	dx = xf - xi;
	dy = yf - yi;
	dx2 = 2 * dx;
	dy2 = 2 * dy;
	if (dx < 0) dx *= -1;
	if (dy < 0) dy *= -1;
	if (dx2 < 0) dx2 *= -1;
	if (dy2 < 0) dy2 *= -1;
	/*OBTENEMOS VARIABLES*/
	pn = dy2 - dx;
	punto.x = xi;
	punto.y = yi;
	linea.push_back(punto);
	/*COMIENZA AL ALGORITMO*/
	while (punto.x <= xf && punto.y >= yf) {
		if (pn >= 0) {
			punto.x++;
			punto.y--;
			linea.push_back(punto);
			pn = pn + dy2 - dx2;
		}
		else if (pn < 0) {
			punto.x++;
			linea.push_back(punto);
			pn = pn + dy2;
		}
	}
	lineas.push_back(linea);
	cantidad_lineas++;

	return;
}

void calcLineaPos(float xi, float yi, float xf, float yf) {
	float dx, dy, dx2, dy2, pn;
	std::vector <punto_t> linea;
	punto_t punto;
	/*CALCULAMOS CONSTANTES.*/
	dx = xf - xi;
	dy = yf - yi;
	dx2 = 2 * dx;
	dy2 = 2 * dy;
	/*OBTENEMOS VARIABLES*/
	pn = dy2 - dx;
	punto.x = xi;
	punto.y = yi;
	linea.push_back(punto);
	/*COMIENZA AL ALGORITMO DE BRESENHAM*/
	while ( punto.x <= xf && punto.y <= yf) {
		if (pn >= 0) {
			punto.x ++;
			punto.y ++;
			linea.push_back(punto);
			pn = pn + dy2 - dx2;
		}
		else if (pn < 0) {
			punto.x ++;
			linea.push_back(punto);
			pn = pn + dy2;
		}
	}
	lineas.push_back(linea);
	cantidad_lineas++;

	return;
}

/*FUNCIÓN PARA DIBUJADO DE LÍNEAS*/
void dibujarLinea(float xi, float yi, float xf, float yf) {
	float m;
	int i, j;

	/*SE VERIFICAN RESTRICCIONES.*/
	if (xi > xf) {
		std::cout << "\n*ERROR: xi es mayor a xf\n";
		return;
	}
	m = (yf - yi) / (xf - xi);
	if (m < -1 || m > 1) {
		std::cout << "\n*** ERROR: m = " << m << ". m debe tener un valor entre -1 y 1.***\n";
		return;
	}
	/*SE CALCULAN LOS PUNTOS CORRESPONDIENTES*/
	else if (m >= 0 && m <= 1) calcLineaPos(xi, yi, xf, yf);
	else if (m < 0 && m >= -1) calcLineaNeg(xi, yi, xf, yf);

	for (i = 0; i < cantidad_lineas; i ++) {
		glBegin(GL_LINE_STRIP);
		for (j = 0; j < lineas[i].size(); j++)
			glVertex2f(lineas[i][j].x, lineas[i][j].y);
		glEnd();
	}
}

void menu(void) {
	//float xi, yi, xf, yf, cx, cy, r;
	char opcion = 0;
	//while (opcion == 0) {
		std::cout << "\nSelecciona una opcion:";
		std::cout << "\n\t'C' para dibujar una circunferencia.";
		std::cout << "\n\t'L' para dibujar una linea.";
		std::cout << "\n\t'S' para salir.";
		std::cout << "\nTu opcion: ";
		std::cin >> opcion;
		std::cout << "\n";
		switch (opcion) {
		case 'C':
		case 'c':
			opcion = 0;
			/*OBTENEMOS LOS VALORES DEL USUARIO.*/
			std::cout << "\n\tIngresa la coordenada en x del centro: ";
			std::cin >> cx;
			std::cout << "\tIngresa la coordenada en y del centro: ";
			std::cin >> cy;
			std::cout << "\tIngresa el radio del circulo: ";
			std::cin >> r;
			//dibujarCirculo(cx, cy, r);
			break;
		case 'L':
		case 'l':
			opcion = 0;
			/*OBTENEMOS LOS VALORES DEL USUARIO.*/
			std::cout << "\n*NOTA: La coordenada de xi debe ser positiva y menor a xf. ";
			std::cout << "\n\tIngresa xi: ";
			std::cin >> xi;
			std::cout << "\tIngresa yi: ";
			std::cin >> yi;
			std::cout << "\tIngresa xf: ";
			std::cin >> xf;
			std::cout << "\tIngresa yf: ";
			std::cin >> yf;
			//dibujarLinea(xi, yi, xf, yf);
			//glutPostRedisplay();
			break;
		case 'S':
		case 's':
			exit(0);
			break;
		default:
			opcion = 0;
			break;
		}
	//}
}

void reshape(int w, int h)
{
	 if (!h)
		return;

	 glViewport(0, 0,  (GLsizei) w, (GLsizei) h);
	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 glOrtho(-100, 100, -100, 100, -1, 1);
	 glMatrixMode(GL_MODELVIEW);
	 glLoadIdentity();
	 return;
}

void display(void)
{ 
	dibujarLinea(xi, yi, xf, yf);
	dibujarCirculo(cx, cy, r);
	
	return;
} 

void keyboard(unsigned char key, int x, int y)
{
	 switch (key) 
	 {
		case 27: 
			exit(0);

			break;
	 }
	  
	  return;
}    

int main(int argc, char **argv)
{ 
	menu();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Dibujado de Líneas y Circunferencias");
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}




