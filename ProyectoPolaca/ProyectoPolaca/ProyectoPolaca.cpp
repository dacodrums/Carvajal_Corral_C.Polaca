// ProyectoPolaca.cpp: define el punto de entrada de la aplicaci�n de consola.
//
#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <string>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Pila.h"
#include "Nodo.h"
#include "PilaGenerica.h"
#include "PilaGenericaD.h"
#include "qrcodegen.h"


struct Elemento {
	string ope;
	bool operador;
};

struct Expresion {
	Elemento* expr;
	int n;
};

using namespace std;

void menuTeclas();
void menu();
void asignar(string);
bool valido(string);
bool operando(string c);
int prdadDentro(string operador);
int prdadFuera(string operando);
string postfija(string, int);
string separarDato(string , char *dato, int &pos);
void guardar(string , char *archivo);
double Evalua(Expresion postfija, double v[]);
void  generarQr(char *);
static void generarQrBasico(char dato1[]);
static void printQr(const uint8_t qrcode[]);


int main() {
	menu();
	return 0;
}

void menu() {
	int cont = 3;
	system("color f0");
	system("cls");
	string menu[] = {
		"1.- Conversion prefijo",
		"2.- Conversion postfijo",
		"3.- Salir             " };
	int cursor = 0;
	char tecla;
	for (;;) {
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		cout << "                         CALCULADORA POLACA" << endl;
		cout << "                 Daniel Corral  -  Abigail Carvajal" << endl << endl;
		for (int i = 0; i < 3; i++) {
			if (cursor == i) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
				cout << menu[i] << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
				cout << menu[i] << endl;
			}
		}
		for (;;) {
			tecla = _getch();
			if (tecla == 80) {
				cursor++;
				if (cursor == cont)
				{
					cursor = 0;
				}
				break;
			}
			if (tecla == 72) {
				cursor--;
				if (cursor == -1)
				{
					cursor = cont-1;
				}
				break;
			}
			if (tecla == 13) {
				switch (cursor) {
				case 0:
					system("cls");
					printf("\n*******************Conversion Prefijo****************\n");
					menuTeclas();
					break;
				case 1:
					system("cls");
					printf("\n*******************Conversion Postfijo****************\n");
					menuTeclas();
					break;
				case 2:
					system("cls");
					printf("\n GRACIAS POR UTILIZAR ESTE PROGRAMA...\n");
					exit(1);
					break;
				}
			}
		}
	}
}

void menuTeclas() {
	system("color f0");
	string menu1[] = {
		"1.- Insertar Expresion",
		"2.- Ver datos (QR)    ",
		"3.- Calcular          ",
		"4.- Pdf               ",
		"5.- About             ",
		"6.- Regresar          " };
	Pila *pila1 = NULL;
	int cursor = 0;
	string cad;
	char tecla;
	char nombreArchivo[11] = "polaca.txt";
	PilaGenerica pila;
	Expresion post;
	string cadena, cverdad;
	char cadena1[30];
	bool desapila;
	int n = -1, i = 0,num;
	
	for (;;) {
		system("cls");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		cout << "                         CALCULADORA POLACA" << endl;
		cout << "                 Daniel Corral  -  Abigail Carvajal" << endl << endl;
		for (int i = 0; i < 6; i++) {
			if (cursor == i) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
				cout << menu1[i] << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
				cout << menu1[i] << endl;
			}
		}
		for (;;) {
			tecla = _getch();
			if (tecla == 80) {
				cursor++;
				if (cursor == 6)
				{
					cursor = 0;
				}
				break;
			}
			if (tecla == 72) {
				cursor--;
				if (cursor == -1)
				{
					cursor = 5;
				}
				break;
			}
			if (tecla == 13) {
				switch (cursor) {
				case 0:
					system("cls");
					
					cout << "Ingrese funcion: " << endl;
					cin >> cadena;
					num = cadena.size();
					cverdad=postfija(cadena,num);
					//printf("\nCADENA POSTFIJO = %s", cverdad.c_str());
					guardar(cverdad.c_str(), nombreArchivo);
					/*for (int i = 0; i < cverdad.size(); i++) {
						cadena1[i] = cverdad[i]     ;
					}*/
					//printf("\n\n%c", cadena1[0]);
					printf("\nPulse cualquier tecla para generar codigo QR...");
					getch();
					generarQr(cadena1);
					pila.limpiarPila();
					menuTeclas();
					break;
				case 1:
					system("cls");
					
					menuTeclas();
					break;
				case 2:
					system("cls");

					menuTeclas();
					break;
				case 3:
					system("cls");
					menuTeclas();
					break;
				case 4:
					menuTeclas();
					break;
				case 5:
					menu();//exit(1);
					break;
				}
				break;
			}
		}
	}
}

void asignar(string ope)
{
	string aux;
	string aux1;
	for (int i = 0; i < ope.length(); i++) //obtengo la longitud de la cadena ingresada
	{
		stringstream ss;
		Pila *auxp = pila;
		int ba = 0;
		ss << ope[i];
		ss >> aux;
		if (aux == "*" || aux == "+" || aux == "/" || aux == "^" || aux == "-" || aux == "(") //Identifico si esa posicion es un signo de interrogacion o un n�mero
		{
			if (pila == NULL)
			{
				poner(aux);
			}
			else
			{
				switch (aux[0])
				{
					ba = 0;
				case '+':
					if (pila != NULL || pila->getDat() != "(" || pila->getDat() != "+")
					{
						do
						{
							aux1 = aux1 + pila->getDat();
							pila = pila->getSgt();
							if (pila != NULL)
							{
								if (pila->getDat() == "c")
									ba = 1;
							}
						} while (pila != NULL&&ba == 0);
					}
					if (pila == NULL || aux == "(")
					{
						poner(aux);

					}
					break;
				case '-':
				{
					if (pila != NULL || pila->getDat() != "(")
					{
						do
						{
							aux1 = aux1 + pila->getDat();
							pila = pila->getSgt();
							if (pila != NULL)
							{
								if (pila->getDat() == "c")
									ba = 1;
							}
						} while (pila != NULL&&ba == 0);
					}
					if (pila == NULL || aux == "(")
					{
						poner(aux);

					}
					break;
				case '*':
					if (pila != NULL || pila->getDat() != "(" || pila->getDat() != "+" || pila->getDat() != "-")
					{
						do
						{
							aux1 = aux1 + pila->getDat();
							pila = pila->getSgt();
							if (pila != NULL)
							{
								if (pila->getDat() == "c" || pila->getDat() != "+" || pila->getDat() != "-")
									ba = 1;
							}
						} while (pila != NULL&&ba == 0);
					}
					if (pila == NULL || aux == "(" || pila->getDat() != "+" || pila->getDat() != "-")
					{
						poner(aux);

					}
					break;
				case '/':
					if (pila != NULL || pila->getDat() != "(" || pila->getDat() != "+" || pila->getDat() != "-")
					{
						do
						{
							aux1 = aux1 + pila->getDat();
							pila = pila->getSgt();
							if (pila != NULL)
							{
								if (pila->getDat() == "c" || pila->getDat() != "+" || pila->getDat() != "-")
									ba = 1;
							}
						} while (pila != NULL&&ba == 0);
					}
					if (pila == NULL || aux == "(" || pila->getDat() != "+" || pila->getDat() != "-")
					{
						poner(aux);

					}
					break;
				}
				}
			}
		}
		else
		{
			aux1 = aux1 + aux;
		}
	}
	if (pila != NULL)
	{
		do
		{
			aux1 = aux1 + pila->getDat();

			pila = pila->getSgt();
		} while (pila != NULL);
	}

	printf("\n\t");
	cout << aux1;
}


bool valido(char expr[20]) {
	bool sw = true;
	for (int i = 0; (i<strlen(expr) && sw); i++) {
		char c;
		c = expr[i];
		sw = sw && (
			(c >= 'A' && c <= 'Z') ||
			(c >= 'a' && c <= 'z') ||
			(c >= '0' && c <= '9') ||
			(c == '^') || (c == '/') || (c == '*') ||
			(c == '+') || (c == '-') || (c == '\n') ||
			(c == '(') || (c == ')')
			);
	}
	return sw;
}

bool operando(char c) { //determina si el caracter es un operando
	return(c >= '0'&& c <= '9');
}

int prdadDentro(char operador) { //prioridad del operador en la expresion
	int pdp=0;
	switch (operador) {
	case '^':
		pdp = 3;
		break;
	case '*': case '/':
		pdp = 2;
		break;
	case '+': case '-':
		pdp = 1;
		break;
	case '(':
		pdp = 0;

	}
	return pdp;
}

int prdadFuera(char operando) { //prioridad del operador en la expresion infija
	int pfp=0;
	switch (operando)
	{
	case '^':
		pfp = 4;
		break;
	case '*': case '/':
		pfp = 2;
		break;
	case '+': case '-':
		pfp = 1;
		break;
	case '(':
		pfp = 5;
	}
	return pfp;
}

string postfija(string expresion, int num) {
	PilaGenerica pila;
	Expresion post;
	Elemento elemento[25];
	string tokens[25];
	string ch, opeCima;
	char dato[25] = "";
	int cont = 0, n = -1, i = 0;
	bool desapila;
	//Valido que la expresion sea valida
	if (!valido(expresion))
		printf("Caracter no valido en la expresion");

	for (int j = 0; j < num; j++) {
		//separo la expresion
		tokens[j] = separarDato(expresion, dato, j);
		ch = tokens[j];
		//cout << "\ntoken = " << ch.c_str();
		//printf("\n");
		if (operando(ch.c_str())) {
			n++;
			elemento[n].ope = ch.c_str();
			elemento[n].operador = false;
			//printf("\nELEMENTO = %s Operando = %d\n", elemento[n].ope.c_str(), elemento[n].operador);
		}
		else if (ch.compare(")")) {
			desapila = true;
			while (desapila) {
				opeCima = " ";
				if (!pila.pilaVacia()) {
					opeCima = pila.cimaPila();
				}
				if (pila.pilaVacia() || (prdadFuera(ch.c_str()) > prdadDentro(opeCima))) { //si la pila esta vacia o si la prioridad del caracter de fuera > prioridad que el que esta dentro
					//printf("\naqui llego 4 ch = %s", ch.c_str());
					pila.push(ch.c_str());
					desapila = false;
					
				}
				else if (prdadFuera(ch.c_str()) <= prdadDentro(opeCima)) {
					elemento[++n].ope = pila.pop();
					elemento[n].operador = true;
					//printf("\naqui llego 5");
				}
			}
		}

		else { //si es )
			opeCima = pila.pop();
			if (opeCima.compare("(")) {
				//printf("\n\nOPECIMA = %s", opeCima.c_str());
				do {
					elemento[++n].ope = opeCima;
					elemento[n].operador = true;
					opeCima = pila.pop();
					//printf("\n\nOPECIMA = %s",opeCima.c_str());
				} while (opeCima != "(");
			}
		}

	}

	while (!pila.pilaVacia()) {
		elemento[++n].ope = pila.pop();
		elemento[n].operador = true;
	}
	//Expresion post;
	post.expr = elemento;
	post.n = n;
	string cverdad;
	double valor, v[26];
	//printf("\ntotal elementos %d\n", n);
	cout << "\n\tExpresion:\t";
	for (int i = 0; i <= post.n; i++)
	{
			printf("%s", post.expr[i].ope.c_str());
			cverdad.append(post.expr[i].ope.c_str());
	}
	
	valor = Evalua(post, v);
	cout << "\n Valor de la expresion =  " << valor;

	return cverdad;
}

double Evalua(Expresion postfija, double v[]) {
	PilaGenericaD pilaGen;
	double valor, a, b;
	int cont = 0;
	for (int i = 0; i <= postfija.n ; i++) {
		string op;
		if (postfija.expr[i].operador) {
			op = postfija.expr[i].ope;
			/*printf("operador: %s\n", op.c_str());*/
			if (!op.compare("sen") || !op.compare("cos") || !op.compare("tan")) {
				if (cont != 1) {
					b = pilaGen.pop();
					a = pilaGen.pop();
					/*printf(" \na: %f\n", a);
					printf(" \nb:%f\n", b);*/
					if (!op.compare("sen")) {
						valor = sin(b);
					}
					else if (!op.compare("cos")) {
						valor = cos(b);
					}
					else if (!op.compare("tan")) {
						valor = tan(b);
					}
					pilaGen.push(a);
					pilaGen.push(valor);
				}
				else {
					a = pilaGen.pop();
					if (!op.compare("sen")) {
						valor = sin(a);
					}
					else if (!op.compare("cos")) {
						valor = cos(a);
					}
					else if (!op.compare("tan")) {
						valor = tan(a);
					}
					pilaGen.push(valor);
				}
				
				
			}
			else if(!op.compare("+") || !op.compare("-") || !op.compare("*") || !op.compare("/") || !op.compare("^")) {
				b = pilaGen.pop();
				a = pilaGen.pop();
				printf(" \nImprimen en else a: %f\n", a);
				printf(" \nImprime en else b:%f\n", b);
				printf("operador: %s\n", op.c_str());
				if (!op.compare("^")) {
					valor = pow(a, b);
				}
				else if (!op.compare("*")) {
					valor = a*b;
				}
				else if (!op.compare("/")) {
					if (b != 0.0)
						valor = a / b;
					else {
						printf("\nNo se puede realizar division para cero\nIntente ingresando otra expresion!");
						getch();
						menuTeclas();
						break;
					}
						
				}
				else if (!op.compare("+")) {
					valor = a + b;
				}
				else if (!op.compare("-")) {
					valor = a - b;
				}
				pilaGen.push(valor);
			}
			
		}
		else {
			int indice;
			double aa;
			op = postfija.expr[i].ope;
			aa = atof(op.c_str());
			pilaGen.push(aa);
			cont++;
		}
	}

	return pilaGen.pop();
}

string separarDato(string expresion, char *dato, int &pos)
{
	string aux;
	char p, p1[2];
	p = expresion[pos];
	//printf("\np : %c\n", p);
	if (p == '+' || p == '-' || p == '*' || p == '/' || p == '^' || p == '(' || p == ')')
	{
		p1[0] = p;
		p1[1] = '\0';
		strcpy(dato, p1);
		//pos++;
		aux = dato;
		return aux;
	}
	else
	{
		p1[0] = '\0';
		strcpy(dato, p1);
		do
		{
			p = expresion[pos];
			if (p == '\0' || p == '+' || p == '-' || p == '*' || p == '/' || p == '^' || p == '(' || p == ')')break;
			p1[0] = p;
			p1[1] = '\0';
			strcat(dato, p1);
			//printf(" p es: %c \n", p);
			pos++;
		} while (true);
		pos--;
		//if (strcmp(dato, "sen") == 0) strcpy(dato, "sen");
		//if (strcmp(dato, "cos") == 0) strcpy(dato, "cos");
		//if (strcmp(dato, "tan") == 0) strcpy(dato, "tan");
	}

	//cout << "dato: " << dato;
	aux = dato;
	return aux;
}

bool valido(string expr) {
	bool sw = true;
	for (int i = 0; (i<expr.size() && sw); i++) {
		string c;
		c = expr[i];
		sw = sw && (
			(c >= "A" && c <= "Z") ||
			(c >= "a" && c <= "z") ||
			(c >= "0" && c <= "9") ||
			(c == "^") || (c == "/") || (c == "*") ||
			(c == "+") || (c == "-") || (c == "\n") ||
			(c == "(") || (c == ")")
			);
	}
	return sw;
}

bool operando(string c) { //determina si el caracter es un operando
	return(c >= "0" && c <= "9");
}

int prdadDentro(string operador) { //prioridad del operador en la expresion (dentro de la pila)
	int pdp = 0;

	if (operador == "^" || operador == "sen" || operador =="cos" || operador =="tan")
		pdp = 3;
	else if (operador == "*" || operador == "/")
		pdp = 2;
	else if (operador == "+" || operador == "-")
		pdp = 1;
	else if (operador == "(")
		pdp = 0;
	return pdp;
}

int prdadFuera(string operando) { //prioridad del operador en la expresion infija
	int pfp = 0;
	if (operando == "^" || operando == "sen" || operando=="cos"|| operando=="tan")
		pfp = 4;
	else if (operando == "*" || operando == "/")
		pfp = 2;
	else if (operando == "+" || operando == "-")
		pfp = 1;
	else if (operando == "(")
		pfp = 5;
	return pfp;
}

void guardar(string expresion, char *archivo) {
	FILE *ptr;
	string nombre;

	ptr = fopen(archivo, "a"); //REALIZO LA APERTURA DEL ARCHIVO
	if (ptr == NULL)
	{
		printf("ERROR.\n");
	}
	else
	{
		nombre = expresion.c_str();
		//fputs(nombre,ptr);
		fprintf(ptr, "Postfijo: %s  ", nombre.c_str());
		fclose(ptr);
	}
}


void generarQrBasico(char dato1[]) {
	char *dato = dato1;  // User-supplied text
	enum qrcodegen_Ecc errCorLvl = qrcodegen_Ecc_LOW;  // Error correction level

													   // Make and print the QR Code symbol
	uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
	uint8_t tempBuffer[qrcodegen_BUFFER_LEN_MAX];
	bool ok = qrcodegen_encodeText(dato, tempBuffer, qrcode, errCorLvl,
		qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX, qrcodegen_Mask_AUTO, true);
	if (ok)
		printQr(qrcode);
}

static void printQr(const uint8_t qrcode[]) {
	int size = qrcodegen_getSize(qrcode);
	int border = 4;
	for (int y = -border; y < size + border; y++) {
		for (int x = -border; x < size + border; x++) {
			fputs((qrcodegen_getModule(qrcode, x, y) ? "\333\333" : "  "), stdout);
		}
		fputs("\n", stdout);
	}
}

static void generarQr(char *mensaje)
{
	printf("GENERADOR DE QR\n");
	generarQrBasico(mensaje);
	system("pause");
}
