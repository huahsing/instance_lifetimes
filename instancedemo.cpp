#include <iostream>
#include <string>

using namespace std;

#define DEMO_PASSING_VALUES_OUT 1

class foo
{
public:
	string name;
	
	foo()
	{ 
		this->name = "no_name";
		cout << "foo first constructor(address=" << this << ") called, valueof(name)=" << this->name << endl; 
	}
	
	foo(string name) 
	{ 
		this->name = name;
		cout << "foo second constructor(address=" << this << ") called, valueof(name)=" << this->name << endl; 
	}
	
	~foo() 
	{ 
		cout << "foo destructor(address=" << this << ") called, valueof(name)=" << this->name << endl; 
	}
	
#if 1
	// default copy constructor
#else
	// we define our own copy constructor
	foo(foo &copy)
	{
		cout << "foo copy constructor(address of this=" << this << ") called, valueof(name)=" << this->name << endl;
		cout << "--> making a copy of instance at address=" << &copy << " with valueof(name)=" << copy.name << endl;
		
		// it's okay for basic types and types like strings to be copied with '='
		// it might or might not be okay for composite types like classes, depending on how the members are allocated
		this->name = copy.name;
		 
		cout << "--> now this(" << this << ") has valueof(name)=" << this->name << endl;
		cout << "leaving copy constructor of this=" << this << endl;
	}
#endif
};


// demonstrating local scope/lifetime of object
void f1()
{
	cout << "just entered " << __FUNCTION__ << endl;
	foo f1Obj("f1Obj");
	cout << "about to exit " << __FUNCTION__ << endl;
}

// pass-by-value method creates extra copies of object
void f2(foo copyOfFoo)
{
	cout << "just entered " << __FUNCTION__ << endl;
	foo f2Obj = copyOfFoo;
	cout << "about to exit " << __FUNCTION__ << endl;
}

// why we cannot pass a copy of object out of a function with pass-by-value method (all the instances are different)
void f2_1(foo copyOfFoo)
{
	cout << "just entered " << __FUNCTION__ << endl;
	foo f2_1("f2_1");
	copyOfFoo = f2_1;
	cout << "about to exit " << __FUNCTION__ << endl;
}

// why we cannot modify value of object with pass-by-value method (because we are modifying a copy in fact)
void f2_2(foo copyOfFoo)
{
	cout << "just entered " << __FUNCTION__ << endl;
	copyOfFoo.name = "aNewName_f2_2";
	cout << "about to exit " << __FUNCTION__ << endl;
}

// pass-by-pointer does not allocate new copies
void f3(foo *pointerToFoo)
{
	cout << "just entered " << __FUNCTION__ << endl;
	foo f3Obj = *pointerToFoo;
	cout << "about to exit " << __FUNCTION__ << endl;
}
/*
Supplementary knowledge for f3()
	1: 
		foo f3Obj("f3Obj"); // allocates automatic storage within f3()
		pointerToFoo = &f3Obj; // reassigns pointer (may cause memory leak of original object), f3Obj will also be lost after exiting f3
	
	2:
		pointerToFoo = new foo("f3Obj"); // reassigns pointer to dynamic memory (which will not be lost after exit, but memory leak may still happen if there used to be a valid object pointed to by pointerToFoo)
		
*/

// pass-by-pointer allow changes to the object be reflected in the original object
void f3_1(foo *pointerToFoo)
{
	cout << "just entered " << __FUNCTION__ << endl;
	pointerToFoo->name = "aNewName_f3_1";
	cout << "about to exit " << __FUNCTION__ << endl;
}

// pass-by-reference does not allocate new copies
void f4(foo &refToFoo)
{
	cout << "just entered " << __FUNCTION__ << endl;
	foo& f4ObjRef = refToFoo;
	cout << "about to exit " << __FUNCTION__ << endl;
	
	// we have two references here actually, one is refToFoo, the other is f4ObjRef (no memory is allocated other than variables to store the references)
}

// references are just alias, also allows changes to the object be reflected in the original object
void f4_1(foo &refToFoo)
{
	cout << "just entered " << __FUNCTION__ << endl;
	refToFoo.name = "aNewName_f4";
	cout << "about to exit " << __FUNCTION__ << endl;
}


// returning an object also creates a copy
foo f5()
{
	foo f5Obj("f5Obj"); 
	return f5Obj;
}

int main()
{
	cout << "calling f1" << endl;	f1();	cout << "exited f1\n" << endl;
	
	// all these objects are 'automatically' allocated on the process stack (they are NOT dynamic memory)
	// the type of storage they reside in is known as 'automatic storage', other types that will be encountered are 'static', 'dynamic'
	foo mainObjA("mainObjA"), mainObjB("mainObjB"), mainObjC("mainObjC");
	
	cout << "calling f2" << endl;	f2(mainObjA);	cout << "exited f2\n" << endl;
	
#if DEMO_PASSING_VALUES_OUT
	cout << "calling f2_1" << endl;	f2_1(mainObjA);	cout << "exited f2_1\n" << endl;
	cout << "calling f2_2" << endl;	f2_2(mainObjA);	cout << "exited f2_2\n" << endl;
#endif
	
	cout << "calling f3" << endl;	f3(&mainObjB);	cout << "exited f3\n" << endl;
	
#if DEMO_PASSING_VALUES_OUT
	cout << "calling f3_1, valueof(name) of mainObjB=" << mainObjB.name << endl;	
	f3_1(&mainObjB);	
	cout << "exited f3_1, valueof(name) of mainObjB=" << mainObjB.name << "\n" << endl;
#endif

	cout << "calling f4" << endl;	f4(mainObjC);	cout << "exited f4\n" << endl;
	
#if DEMO_PASSING_VALUES_OUT
	cout << "calling f4_1, valueof(name) of mainObjC=" << mainObjC.name << endl;	
	f4_1(mainObjC);	
	cout << "exited f4_1, valueof(name) of mainObjC=" << mainObjC.name << "\n" << endl;
#endif

	cout << "calling f5" << endl;	
	cout << "name of object returned by f5 = " << f5().name << endl;
	cout << "exited f5\n" << endl;

	return 0;
}