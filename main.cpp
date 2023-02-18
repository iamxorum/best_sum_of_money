// ! ############################################################################################
// ! Considerând  daten  tipuri  de monede și o sumă S, scriețiun programcare să determine o
// ! modalitate de plată a sumei S cu un numărde minim de monezi
// ! ############################################################################################

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

void printMatrix(vector<vector<int>> memo, int M, int N, ostream &fout)
{

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (i == 0)
            {
                fout << "\t" << j << "\t";
            }
            else
            {
                fout << "\t" << memo[i][j] << "\t";
            }
        }
        fout << "\n";
    }
    fout << "---------------------------------------------\n";
}

vector<vector<int>> ex2_programareDinamica(int suma, int N, vector<int> monezi, ofstream &fout)
{
    vector<vector<int>> memo(N + 1, vector<int>(suma + 1, suma));
    memo[0][0] = 0; // * Nu avem nevoie de nicio monedă pentru suma zero
    for (int i = 1; i <= N; i++)
    {
        for (int j = 0; j <= suma; j++)
        {
            memo[i][j] = memo[i - 1][j]; // * Initializam sa nu folosim moneda curenta (pentru ca pornim cu i=1 si asta ar insemna sa nu folosim prima moneda din vectorul monezi)
            if (j - monezi[i - 1] >= 0)
            {
                // * Calculam folosind moneda curenta si dupa alegem valoarea minima de stocat in matricea de memoizare
                memo[i][j] = min(memo[i][j], memo[i][j - monezi[i - 1]] + 1);
            }
        }
    }
    printMatrix(memo, N + 1, suma + 1, fout);
    return memo;
}

vector<int> shortestMonezi(int suma, int N, vector<int> monezi, vector<vector<int>> memo)
{
    vector<int> monezi_alease;
    int i = N, j = suma;
    while (i > 0 && j > 0)
    {
        if (memo[i][j] == memo[i - 1][j])
        // * Comparatia asta controleaza daca, suma cu J-ul respectiv, intre randul curent si randul precedent a fost folosita o moneda
        // * (adica, daca se schimba valoarea, in matricea memo, intre 2 randuri la aceasi valoare j, asta inseamna ca moneda cu indicele i a fost
        // * folosita pentrua ajunge la suma respectiva)
        {
            // * Moneda curentă nu a fost folosită, trecem la moneda următoare
            i--;
        }
        else
        {
            // * Moneda curentă a fost folosită, o adăugăm la vectorul de monezi alese
            monezi_alease.push_back(monezi[i - 1]);
            // * Scădem valoarea monedei din suma rămasă
            j -= monezi[i - 1];
        }
    }
    return monezi_alease;
}

int main()
{
    ifstream fin("Variabile de intrare.txt"); // * Citeste din fisier
    if (!fin)
    {
        cerr << "INPUT INEXISTENT!!!" << endl; // * Nu a reusit sa citeasca fisierul
        exit(-1);
    }

    ofstream fout("Matrice MEMO de monezi.txt");

    int t;
    // Primul rand va citi valoarea sumei
    int suma;
    fin >> suma; // * Citeste suma
    if (suma <= 0)
    {
        if (suma < 0)
        {
            cerr << "SUMA NEGATIVA!" << endl; // * Suma citita e mai mica de 0
            exit(-2);
        }
        fout << "Rezultatul e imposibil de calculat pentru ca nu exista nicio SUMA!";
        return 1;
    }

    // Al doilea rand va citi lungimea vectorului de monezi
    int N;
    fin >> N; // * Citeste lungimea vectorului de monezi
    if (N < 0)
    {
        cerr << "IMPOSIBIL SA AI UN VECTOR DE MARIME NEGATIVA!" << endl; // * Lungimea citita e nevalida
        exit(-3);
    }

    // Al treilea rand va citi vectorul de monezi
    vector<int> monezi;
    int input; // * Citeste monezile
    for (int i = 0; i < N; i++)
    {
        fin >> input;
        monezi.push_back(input); // * Introduce in vector monezile
    }
    for (int i = 0; i < N; i++)
    {
        if (count(monezi.begin(), monezi.end(), monezi[i]) > 1)
        {
            cerr << "EXISTA VALORI DUPLICATE IN VECTORUL DE MONEZI! sau ATI INTRODUS MAI PUTINE MONEZI FATA DE MARIMEA DECLARATA!" << endl;
            exit(-4);
        }
    }

    fin.close();

    // Construim tabela de programare dinamică
    int counter = 1;
    while (counter <= N)
    {
        fout << "{ ";
        for (int i = 0; i < counter; i++)
        {
            fout << monezi[i] << " ";
        }
        fout << "} Monezile astea sunt cele folosite in randul " << counter << " din matricea memo pentru a ajunge la suma fiecarui coloana;\n";
        counter++;
    }

    fout << "---------------------------------------------\n";
    vector<vector<int>> memo;
    memo = ex2_programareDinamica(suma, N, monezi, fout);
    fout.close();
    fout.clear();
    fout.open("Rezultat.txt");

    // Obținem vectorul cu cele mai puține monezi
    vector<int> shortMonezi;
    shortMonezi = shortestMonezi(suma, N, monezi, memo);

    // Afișăm vectorul cu cele mai puține monezi
    fout << "Suma introdusa, pentru care s-a vrut cantitatea de monezi cea mai mica pentru a ajunge la ea, este: " << suma << endl;
    fout << "Vectorul de monezi este: { ";
    for (int i = 0; i < N - 1; i++)
    {
        fout << monezi[i] << ", ";
    }
    fout << monezi[N - 1] << " }" << endl;
    fout << "--------------------------------" << endl;

    fout << "Monezile alease sunt:\n";
    for (int i = 0; i < N; i++)
    {
        fout << "\tMoneda " << monezi[i] << " a fost folosita de " << count(shortMonezi.begin(), shortMonezi.end(), monezi[i]) << " ori.\n";
    }
    fout.close();

    system("cls");

    monezi.clear();
    shortMonezi.clear();
    memo.clear();

    return 0;
}