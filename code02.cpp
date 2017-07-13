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

person p("John", 31);

int main()
{
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
// Pseudocode.
//////////////////////////////////////////////////////////////////////////////

//
// - Konstruktor globalniho objektu.
//   - Kompilator pro kazdy globalni objekt, ktery potrebuje zavolat konstruktor,
//     vytvori standalone funkci (bezparametrickou, bez navratove hodnoty).
//   - Funkce pote zavola "metodu" konstruktoru s danymi parametry.
//

void ctor1()
{
    person::person(&p, "John", 31);
}

typedef void (*initfn_t)();
initfn_t ctors[] = { &ctor1 };

void run_ctors()
{
    for (int i = 0; i < sizeof(ctors); i++)
    {
        ctors[i]();
    }
}

void crt_startup()
{
    int argc; char** argv;
    get_args(&argc, &argv);

    run_ctors();
    int exit_code = main(argc, argv);

    exit(exit_code);
}




