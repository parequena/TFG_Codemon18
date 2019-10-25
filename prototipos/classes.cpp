#include <iostream>

struct A
{
	int i = 0;

	A(): A(0) { };
	A(const A& a): i(a.i) { };
	A(int x): i(x) { };
	virtual A& operator=(const A& a);
	// virtual A& operator=(A a); 
	virtual void draw() const = 0;
};

struct B : public A
{
	B() { i = 0; }
	B(int x): A(x) { };

	inline void draw() const override { std::cout << "B->i: " << i << "\n"; };
};

struct C
{
	inline void saluda() const { std::cout << "Hola C\t"; }
};

struct D : public C
{
	inline void saluda() const { std::cout << "Hola D\t"; C::saluda(); }	
};

int main()
{
	C c;
	D d;
	c.saluda();
	std::cout << "\n";
	d.saluda();
	std::cout << "\n";
}