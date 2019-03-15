# Little-GC
A toy garbage collector for C++
## What is Little-GC?
It's just a toy project that implements a Garbage Collector for C++. Don't use it, except for toy projects.
## Usage
It's pretty straightforward.
### Adding the library to your project
Copy the two files *littlegc.inl* and *referencer.inl*. Include the first one in every place where it is necessary.
```c++
#include "littlegc.inl"
```
### Namespaces
The library defines a user namespace called *lgc_user*. Two names are defined inside this namespace:
* lgc : (which stands for Little Garbage Collector), a templated class that represents a variable (or, more accuratly, a reference)
* gc : a factory that you should use to create *lgc*
We strongly discourage you to use elements which are in other namespaces. We also suggest the usage of the *using* statement:
```c++
using namespace lgc_user;
```
### Storing data
You can use the garbage collector aside from another form of memory management in the same project.
You can store data in the following ways:
```c++
auto str { lgc<std::string>("A string") };
auto str { gc(std::string("A string") };
auto str { gc<std::string>("A string") };

// you can also use move constructor
std::string s { "String" };
auto str { gc(std::move(s)) };

// you can also create an empty value
auto str { gc<std::string>() };
```
### Using underlying values
Using underlying values is pretty easy: use the * operator to get a reference to the object, and the -> operator to access one of its members.
```c++
auto s { gc<std::string>("A string") };
std::cout << *s; // prints "A string"
*s = "str";
std::cout << s->size(); // print "3"
```
Note that if the lgc object is declared constant, the underlying value can not be modified.
```c++
const auto i { gc(42) };
*i = 45; // compile error
```
### Changing the reference
It is possible to make a reference point to another value.
```c++
auto i { gc(56) };
auto y { gc(57) };
i = y; // i points now to 57
```
### Passing and returning a value to and from a function
Always pass and return a lgc by value. The data will not be copied, only the reference.
```c++
lgc<int> f(lgc<int> x) {
	auto y { gc(x*x) };
	return y;
	}
```
### Comparing references
It is possible to compare references with the operator *==* and *!=*. Note that it is does not compare the underlying value, but only if the references are the same.
```c++
auto s { gc<int>(1) };
auto s2 { s }; // s points to the same object as s
auto s3 { gc<int>(1) }; // s3 is equal to s, but does not points to the same object

s == s2; // true
s == s3; // false
s != s2; // false
s != s3; // true
```
## What you should not do
* Never ever call `delete` with a lgc or a lgc member: always let the Garbage Collector do its job.
* Do not use any class, function or other which is not in the user namespace.
* Do not use any array with the Garbage Collector. Seriously, who whants to use so low-level tools like arrays with a garbage collector? 
* Do not use this library. Instead, learn to use pointers correctly and so-called smart pointers.
## When is garbage destroyed?
This is one issue of the Garbage Collector in general: when does the GC do its work?
Little Garbage Collector destroys the data (frees them, actually) when the last reference of a variable is destroyed. Each destruction of a reference or each copy notifies the instance in charge of counting the references and, when the last reference is destroyed, or when it receives another value, the count of references is equal to 0 and the memory is freed.
## Tests
I've got no time to make tests. I compiled it with Clang and GCC, with all warnings and used Valgrind at runtime. It works. Do the same or write tests if you're so bored that you've got nothing better to do. Pull requests are accepted.
```c++
```
