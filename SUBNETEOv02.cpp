#include<iostream>
#include<string.h>
#include<stdio.h>
#include<cmath>
#include<stdlib.h>
#include<cstdlib>

using namespace std;

void Extraer_Octetos();
int Calcular_Hosts();
void Obtener_Mascara();
void Calcular_Rangos();

char Dir_IP[15];
char *token;
char clase;
int Octetos[4], Mascara_Default[4], Nueva_Mascara[4];
int hosts = 0;
int hosts_otorgados = 0;
int bits_ocupados = 0;
int Subredes = 0;

void Extraer_Octetos(){
	int i = 0;
	cin >> Dir_IP;
	token = strtok(Dir_IP, ".");
    while(token){
        Octetos[i] = atoi(token);
        token = strtok(NULL, ".");
        i++;
    }
}

int Calcular_Hosts(int n){
	int hosts_solicitados = n;
    int i = 0;
    do{
        hosts_otorgados = pow(2, i);
        i++;
    }while(hosts_otorgados < hosts_solicitados);
    //bits_ocupados es la cantidad de bits que tendre que rellenar en la mascara de izquerda a derecha
    bits_ocupados = i - 1;
    return hosts_otorgados;
}

void Obtener_Mascara(){
	int i = 7;
	int vueltas = 0;

	if(Octetos[0] >= 0 && Octetos[0] <= 127){
		Mascara_Default[0] = 255;
		Mascara_Default[1] = 0;
		Mascara_Default[2] = 0;
		Mascara_Default[3] = 0;
		Subredes = pow(2, 24 - bits_ocupados);
		clase = 'A';
	} else if(Octetos[0] >= 128 && Octetos[0] <= 191){
		Mascara_Default[0] = 255;
		Mascara_Default[1] = 255;
		Mascara_Default[2] = 0;
		Mascara_Default[3] = 0;
		Subredes = pow(2, 16 - bits_ocupados);
		clase = 'B';
	} else if(Octetos[0] >= 192 && Octetos[0] <= 223){
		Mascara_Default[0] = 255;
		Mascara_Default[1] = 255;
		Mascara_Default[2] = 255;
		Mascara_Default[3] = 0;
		Subredes = pow(2, 8 - bits_ocupados);
		clase = 'C';
	}
	switch(clase){
		case 'A':
			Nueva_Mascara[0] = 255;
			if(bits_ocupados >= 0 && bits_ocupados <= 8){
				do{
					Nueva_Mascara[1] += pow(2, i);
					i--;
					vueltas++;
				}while(vueltas != bits_ocupados);
				Nueva_Mascara[2] = 0;
				Nueva_Mascara[3] = 0;
			} else if(bits_ocupados >= 9 && bits_ocupados <= 16){
				Nueva_Mascara[1] = 255;
				bits_ocupados = bits_ocupados - 8;
				do{
					Nueva_Mascara[2] += pow(2, i);
					i--;
					vueltas++;
				}while(vueltas != bits_ocupados);
				Nueva_Mascara[3] = 0;
			} else if(bits_ocupados >= 17 && bits_ocupados <= 24){
				Nueva_Mascara[1] = 255;
				Nueva_Mascara[2] = 255;
				bits_ocupados = bits_ocupados - 16;
				do{
					Nueva_Mascara[3] += pow(2, i);
					i--;
					vueltas++;
				}while(vueltas != bits_ocupados);
			}
			break;

		case 'B':
			Nueva_Mascara[0] = 255;
			Nueva_Mascara[1] = 255;
			if(bits_ocupados >= 0 && bits_ocupados <= 8){
				do{
					Nueva_Mascara[2] += pow(2, i);
					i--;
					vueltas++;
				}while(vueltas != bits_ocupados);
				Nueva_Mascara[3] = 0;
			} else if(bits_ocupados >= 9 && bits_ocupados <= 16){
				Nueva_Mascara[2] = 255;
				bits_ocupados = bits_ocupados - 8;
				do{
					Nueva_Mascara[3] += pow(2, i);
					i--;
					vueltas++;
				}while(vueltas != bits_ocupados);
			}
			break;

		case 'C':
			Nueva_Mascara[0] = 255;
			Nueva_Mascara[1] = 255;
			Nueva_Mascara[2] = 255;
			if(bits_ocupados >= 0 && bits_ocupados <= 8){
				do{
					Nueva_Mascara[3] += pow(2, i);
					i--;
					vueltas++;
				}while(vueltas != bits_ocupados);
			}
			break;

		default:
			break;
	}
	//cout << "Subredes: " << Subredes << endl;
	//cout << "Mascara: " << Mascara_Default[0] << "." << Mascara_Default[1] << "." << Mascara_Default[2] << "." << Mascara_Default[3] << endl;
	//cout << "Nueva mascara: " << Nueva_Mascara[0] << "." << Nueva_Mascara[1] << "." << Nueva_Mascara[2] << "." << Nueva_Mascara[3] << endl;
}

void Calcular_Rangos(){
	int i = 0;
	int suma = 0;
	int division;
	int residuo;

	for(i = 0; i <= Subredes - 1; i++){
		//Calcular la primer dirección de cada subred
        cout << i + 1 << ".- ";
		if(suma < 256){
			Octetos[3] = suma;
		} else if(suma >= 256 && suma <= 65536){
			division = suma / 256;
			residuo = suma % 256;
			if(residuo == 0){
				Octetos[3] = 0;
				Octetos[2] = division;
			} else {
				Octetos[3] = residuo;
				Octetos[2] = division;
			}
		}else if(suma > 65536 && suma <= 16777216){
			division = suma / 65536;
			residuo = suma % 65536;

			if(residuo == 0){
				Octetos[3] =  0; //((suma / division) / 256) - 1;
				Octetos[2] = 0; //((suma / 256) / division) - 1;
				Octetos[1] = division;
			} else if(residuo >= 1 && residuo < 256){
				Octetos[3] = residuo;
				Octetos[2] = 0;
				Octetos[1] = division;
			} else if(residuo > 256 && residuo <= 65536){
				Octetos[3] = (residuo % 256);
				Octetos[2] = (residuo / 256);
				Octetos[1] = division;
			}
		}

        cout << Octetos[0] << "." << Octetos[1] << "." << Octetos[2] << "." << Octetos[3] << " - ";

		//Calcular la ultima dirección de cada subred
		suma += hosts_otorgados;

		if(suma < 256){
			Octetos[3] = suma - 1;
		} else if(suma >= 256 && suma <= 65536){
			division = suma / 256;
			residuo = suma % 256;
			if(residuo == 0){
				Octetos[3] = 255;
				Octetos[2] = division - 1;
			} else {
				Octetos[3] = residuo - 1;
				Octetos[2] = division;
			}
		} else if(suma > 65536 && suma <= 16777216){
			division = suma / 65536;
			residuo = suma % 65536;

			if(residuo == 0){
				Octetos[3] =  255; //((suma / division) / 256) - 1;
				Octetos[2] = 255; //((suma / 256) / division) - 1;
				Octetos[1] = division -1;
			} else if(residuo >= 1 && residuo < 256){
				Octetos[3] = residuo - 1;
				Octetos[2] = 0;
				Octetos[1] = division;
			} else if(residuo >= 256 && residuo <= 65536){
				Octetos[3] = (residuo % 256) - 1;
				if(Octetos[3] < 0){
                    Octetos[3] = 255;
				}
				Octetos[2] = (residuo / 256);
				Octetos[1] = division;
			}

		}

        cout << Octetos[0] << "." << Octetos[1] << "." << Octetos[2] << "." << Octetos[3] << endl;
	}
}

int main(){
    int hosts = 0;
	cout << "********* SUBNETEO *********" << endl;
	cout << "\nIngrese la direccion IP: ";
    Extraer_Octetos();
    cout << "Ingrese la cantidad de host's por subred: "; cin >> hosts;
    Calcular_Hosts(hosts);
    Obtener_Mascara();
    cout << " " << endl;
    Calcular_Rangos();
    cout << "\nHost teoricos: " << Calcular_Hosts(hosts) << endl;
    cout << "Host practicos: " << Calcular_Hosts(hosts) - 2 << endl;
    cout << "\nSubredes teoricas: " << Subredes << endl;
    cout << "Subredes practicas: " << Subredes - 2 << endl;
    cout << "\nMascara default: " << Mascara_Default[0] << "." << Mascara_Default[1] << "." << Mascara_Default[2] << "." << Mascara_Default[3] << endl;
    cout << "Nueva mascara: " << Nueva_Mascara[0] << "." << Nueva_Mascara[1] << "." << Nueva_Mascara[2] << "." << Nueva_Mascara[3] << endl;
    return 0;
}


