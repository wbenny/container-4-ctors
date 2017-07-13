#include <iostream>
#include <string>

class person
{
  public:
    person(const char* name, int age) : name_(name), age_(age)
    {
      std::cout << "ctor(): name: " << name_ << ", age: " << age_ << std::endl;
    }

    ~person()
    {
      std::cout << "dtor()" << std::endl;
    }

  private:
    const char* name_;
    int age_;
};

int main()
{
  person p("John", 31);

  return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Pseudocode.
//////////////////////////////////////////////////////////////////////////////

//
// - Klasicke pouziti konstruktoru.
//

int main()
{
  person p;
  person::person(&p, "John", 31);

  person::~person(&p);
  return 0;
}
