# Little-GC
A toy garbage collector for C++
## What is Little-GC?
It's just a toy project that implements a Garbage Collector for C++. Don't use it, except for toy projects.
## Usage
It's pretty straightforward.
```c++
// include the header
#include <iostream>
#include <string>
#include "littlegc.inl"

/* Put the user namespace in the main scope
 * lgc_user defines only two names, which are templates:
 * - the class 'lgc' (little garbage collector)
 * - the factory, that you should use, 'gc'
 */
using namespace lgc_user;

lgc<std::string> f_that_creates_a_string(const char s[]) {
  /* This function creates a standard string with the given
   * c-style string. As you can see,
   * the string does not go out of scope at the end of the function.
   * You can see also that the return value is passed by value and not
   * by reference: an lgc object should never be used by reference.
   * Instead, copy the object: the wrapped object will not be copied.
   */
   return gc<std::string>(s);
   }

int main () {
  // declare a new variable thanks to the factory
  auto str { gc(std::string("ok")) };
  
  // other syntax
  auto str2 { gc<std::string>("ok") };
  
  // dereference it with the * operator, as a pointer
  *str = "not ok";
  std::cout << *str2;
  
  // if the object has members, you can use them
  // with the '->' operator
  std::cout << str->size();
  
  return 0;
  }
```
## What you should not do
* Never ever call `delete` with a lgc or a lgc member: always let the Garbage Collector do his job.
* Do not use any class, function or other which is not in the user namespace.
* Do not use this library. Instead, learn to use pointers correctly and so-called smart pointers.
## When is garbage destroyed?
This is one issue of the Garbage Collector in general: when does the GC do its work?
Little Garbage Collector destroys the data (frees them, actually) when the last reference of a variable is destroyed. Each destruction of a reference notifies the instance in charge of counting the reference and, when the last reference is destroyed, the count of references is equal to 0 and the memory is freed.
## Tests
I've got no time to make tests. I compiled it with Clang and GCC, with all warnings and used Valgrind at runtime. It works. Do the same or write tests if you're so bored that you've got nothing better to do. Pull requests are accepted.
