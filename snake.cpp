#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <ctime>
using namespace std;

bool KoniecGry;
bool invalidCoord;
const int width = 35;
const int height = 25;
int x, y, papuX, papuY, wynik;
int ogonX[100], ogonY[100];
int ogonDlug;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void ClearScreen()
{
    // funkcja ktora powstrzymuje trzesienie sie ekranu
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void Setup()
{   // zmienne
    KoniecGry = false;
    dir = STOP;
    srand(time(0));
    x = width / 2;
    y = height / 2;
    papuX = rand() % width;
    papuY = rand() % height;
    wynik = 0;

}

void Draw() // snake, papu i mapa
{
    ClearScreen();

    // sufit
    for (int i = 0; i < width + 2; i++)
        cout << 'M';
    cout << endl;

    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            // lewa sciana
            if (k == 0)
                cout << 'H';
            // glowa snake'a
            if (i == y && k == x)
                cout << '@';
            // papu
            else if (i == papuY && k == papuX)
                cout << '*';

            else
            {
                // wyswietla ogon w poprawnych koordynatach 
                bool printOgon = false;
                for (int j = 0; j < ogonDlug; j++)
                {
                    if (ogonX[j] == k && ogonY[j] == i)
                    {
                        cout << 'o';
                        printOgon = true;
                    }
                }
                // pokazuje pustke jak nie ma nic do pokazania
                if (!printOgon)
                    cout << ' ';
            }

            // prawa sciana
            if (k == width - 1)
                cout << 'H';

        }
        cout << endl;
    }

    // podloga
    for (int i = 0; i < width + 2; i++)
        cout << 'W';
    cout << endl;

    // Wyswietla wynik
    cout << endl;
    cout << "Wynik: " << wynik << endl;

}
void Input()
{
    // zmiena kierunek snake'a i ignoruje nie poprawne imput'y
    if (_kbhit())
    {
        switch (_getch())
        {
        case 'w':
            if (dir != DOWN)
                dir = UP;
            break;
        case 'a':
            if (dir != RIGHT)
                dir = LEFT;
            break;
        case 's':
            if (dir != UP)
                dir = DOWN;
            break;
        case 'd':
            if (dir != LEFT)
                dir = RIGHT;
            break;
        case 'k':
            KoniecGry = true;
            break;
        }

    }
}

void Logic()
{
    // logika ogólna przy kazdym ruchu zapamietuje poprzednia pozycje glowy i zapisuje ja jako popY, popY.
    // zmienia wtedy szyk snake'a do nowej pozycji, zamieniajac pierwsza czesc ogonu ogonX, ogonY na nowa pozycje glowy
    // i potem robi tak samo z cala reszta.
    // zapisz ogonX[i], ogonlY[i] jako popX2, popY2 i zamien tailX[i], tailY[i] na popX, popY.
    // zamien popX, popY na popX2, popY2.
    // i wtedy zmien pozostalosc w ten sam sposob 

    int popX = ogonX[0];
    int popY = ogonY[0];
    int popX2, popY2;
    ogonX[0] = x;
    ogonY[0] = y;

    for (int i = 1; i < ogonDlug; i++)
    {
        popX2 = ogonX[i];
        popY2 = ogonY[i];
        ogonX[i] = popX;
        ogonY[i] = popY;
        popX = popX2;
        popY = popY2;
    }
    // zmienia polozenie snake'a na podstawie jego kierunku
    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }

    // wykrywa dotkniecie ogona
    for (int i = 0; i < ogonDlug; i++)
        if (ogonX[i] == x && ogonY[i] == y)
            KoniecGry = true;

    // wykrywa dotkniecie papu 
    if (x == papuX && y == papuY)
    {
        wynik += 1;
        papuX = rand() % width;
        papuY = rand() % height;
        // tworzy nowa pozycje papu ktora nie styka sie z ogonem 
        for (int i = 0; i < ogonDlug; )
        {
            invalidCoord = false;
            if (ogonX[i] == papuX && ogonY[i] == papuY) {
                invalidCoord = true;
                papuX = rand() % width;
                papuY = rand() % height;
                break;
            }
            if (!invalidCoord)
                i++;
        }
        ogonDlug++;
    }

    // zmiena pozycje snake'a jak przechodzi przez sciane 
    if (y >= height)
        y = 0;
    else if (y < 0)
        y = height - 1;
    if (x >= width)
        x = 0;
    else if (x < 0)
        x = width - 1;
}

int main()
{
    Setup();
    while (!KoniecGry)
    {
        Draw();
        if (dir == UP || DOWN)
            Sleep(25); // pomaga kontrolowac predkosc snake'a
        Sleep(40);
        Input();
        Logic();

    }

    return 0;
}
