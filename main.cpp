#include <iostream>
#include <bitset>
#include <iomanip>
#include <string>
#include <cmath>
#include <sstream>

using namespace std;

// Convierte un número decimal con signo a complemento a dos
string convertirAComplementoDos(int numero, int numeroBits) {
    if (numero >= 0)
        return bitset<32>(numero).to_string().substr(32 - numeroBits);
    else {
        int valor = (1 << numeroBits) + numero;
        return bitset<32>(valor).to_string().substr(32 - numeroBits);
    }
}

// Convierte binario en complemento a dos a decimal
int convertirADecimal(const string &numeroBinario) {
    int numeroBits = numeroBinario.size();
    if (numeroBinario[0] == '0') {
        return stoi(numeroBinario, nullptr, 2);
    } else {
        string numeroInvertido = numeroBinario;
        for (char &bit : numeroInvertido)
            bit = (bit == '0') ? '1' : '0';
        int magnitud = stoi(numeroInvertido, nullptr, 2) + 1;
        return -magnitud;
    }
}

// Suma binaria de n bits
string sumaBinaria(string numeroA, const string &numeroB, int numeroBits) {
    int acarreo = 0;
    string resultado(numeroBits, '0');

    for (int i = numeroBits - 1; i >= 0; --i) {
        int bitA = numeroA[i] - '0';
        int bitB = numeroB[i] - '0';
        int suma = bitA + bitB + acarreo;
        resultado[i] = (suma % 2) + '0';
        acarreo = suma / 2;
    }

    return resultado;
}

// Imprime el paso del algoritmo
void imprimirPaso(int numeroPaso, const string &acumulador, const string &multiplicador,
                  char bitAnterior, const string &operacion, int numeroBits) {
    cout << "| " << setw(4) << numeroPaso << " | "
         << setw(numeroBits + 2) << acumulador << " | "
         << setw(numeroBits + 2) << multiplicador << " |  "
         << bitAnterior << "   | "
         << setw(16) << left << operacion << " |\n";
}

// Corrimiento aritmético a la derecha
void desplazamientoAritmeticoDerecha(string &acumulador, string &multiplicador, char &bitAnterior) {
    bitAnterior = multiplicador.back();
    multiplicador = acumulador.back() + multiplicador.substr(0, multiplicador.length() - 1);
    acumulador = acumulador[0] + acumulador.substr(0, acumulador.length() - 1);
}

// Algoritmo de Booth
void multiplicacionBooth(int multiplicando, int multiplicador, int numeroBits) {
    string multiplicandoBinario = convertirAComplementoDos(multiplicando, numeroBits);
    string multiplicandoNegativo = convertirAComplementoDos(-multiplicando, numeroBits);
    string multiplicadorBinario = convertirAComplementoDos(multiplicador, numeroBits);
    string acumulador(numeroBits, '0');
    char bitAnterior = '0';
    int numeroPaso = 0;

    cout << "\nAlgoritmo de Booth (" << numeroBits << " bits)\n";
    cout << "Multiplicando (M)     = " << multiplicandoBinario << " (" << multiplicando << ")\n";
    cout << "Multiplicando (-M)    = " << multiplicandoNegativo << " (" << -multiplicando << ")\n\n";

    cout << "+------+----------------+----------------+------+------------------+\n";
    cout << "| Paso |   Acumulador   | Multiplicador  | Q-1  |    Operacion    |\n";
    cout << "+------+----------------+----------------+------+------------------+\n";

    imprimirPaso(numeroPaso++, acumulador, multiplicadorBinario, bitAnterior, "Inicializacion", numeroBits);

    for (int i = 0; i < numeroBits; ++i) {
        string operacion = "Sin operacion";
        string ultimosBits = string(1, multiplicadorBinario.back()) + bitAnterior;

        if (ultimosBits == "10") {
            acumulador = sumaBinaria(acumulador, multiplicandoNegativo, numeroBits);
            operacion = "A = A - M";
        } else if (ultimosBits == "01") {
            acumulador = sumaBinaria(acumulador, multiplicandoBinario, numeroBits);
            operacion = "A = A + M";
        }

        imprimirPaso(numeroPaso++, acumulador, multiplicadorBinario, bitAnterior, operacion, numeroBits);
        desplazamientoAritmeticoDerecha(acumulador, multiplicadorBinario, bitAnterior);
        imprimirPaso(numeroPaso++, acumulador, multiplicadorBinario, bitAnterior, "Desplazamiento", numeroBits);
    }

    string resultadoBinario = acumulador + multiplicadorBinario;
    int resultadoDecimal = convertirADecimal(resultadoBinario);

    stringstream convertidorHexadecimal;
    convertidorHexadecimal << hex << uppercase << (resultadoDecimal & ((1LL << (numeroBits * 2)) - 1));

    cout << "\nResultado en binario : " << resultadoBinario;
    cout << "\nResultado en decimal : " << resultadoDecimal;
    cout << "\nResultado en hexadecimal: 0x" << convertidorHexadecimal.str();
    cout << "\nVerificación         : " << multiplicando << " × " << multiplicador
         << " = " << multiplicando * multiplicador << "\n";
}

int main() {
    int multiplicando, multiplicador, numeroBits;

    cout << "Ingrese el multiplicando: ";
    cin >> multiplicando;
    cout << "Ingrese el multiplicador: ";
    cin >> multiplicador;
    cout << "Ingrese la cantidad de bits: ";
    cin >> numeroBits;

    multiplicacionBooth(multiplicando, multiplicador, numeroBits);
    return 0;
}
