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
