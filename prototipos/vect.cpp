#include <iostream>
#include <vector>

int main()
{
	std::vector<int> v, v2;
	for(int i = 0; i++ < 5;)
		v.push_back(i);

	int size = v.size();

	v2.resize(size);
	for(int x = 0; x < size; ++x)

}