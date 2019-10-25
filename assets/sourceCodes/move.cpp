#include <iostream>
#include <stdlib.h>

int main(int argc, char** argv)
{
	srand (time(NULL));
	
	if(argc < 2)
		return -1;

	int x = atoi(argv[0]), y = atoi(argv[1]);

	switch(rand() % 4)
	{
		case 0:
			x+=5;
		break;

		case 1:
			x+=6;
		break;

		case 2:
			++x;
		break;

		case 3:
			y;
		break;
	}

 	std::cout << "V D" << "\n";
	std::cin >> d >> id;

 	std::cout << x << " " << y << "\n";

	return 0;
}
