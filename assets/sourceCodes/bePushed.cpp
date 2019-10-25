#include <iostream>
using namespace std;

int main()
{
    char c;
    
    c = getchar();
    if (c == 'P')
    {
        c = getchar();
        if (c == ':')
        {
            c = getchar();
            if (c == 'U')
                cout << "M:U" << endl;
            if (c == 'D')
                cout << "M:D" << endl;
            if (c == 'L')
                cout << "M:L" << endl;
            if (c == 'R')
                cout << "M:R" << endl;
        }
    }

    // Importante, devolver 0 para cuando se use wait en el padre, que 
    // pueda saber que el hijo ha terminado correctamente
    return 0;
}
