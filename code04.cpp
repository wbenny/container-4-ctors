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

void dtor1()
{
    person::~person(&p);
}

void ctor1()
{
    person::person(&p, "John", 31);
    atexit(&dtor1);
}

//////////////////////////////////////////////////////////////////////////////

//
// - Konstruktory jsou definovany ve specialni sekci.
//   - V pripade MSVC je to sekce ".CRT"
// - Sekce je proiterovana a jsou zavolany veskere wrappery pro konstruktory.
// - GCC & clang:
//   - Pro kazdy .cpp unit kompiler vytvori jednu funkci, ktera postupne vola vsechny konstruktory v onom .cpp.
//   - Pointery na tyto funkce jsou vlozeny do sekce .init_array (a volany pred main()).
//   - V pripade LTO se vice techto funkci muze spojit do jedne.
//

typedef void (*initfn_t)();

#pragma section(".CRT", read)

__declspec(allocate(".CRT")) initfn_t ctors[] = { &ctor1 };

void run_ctors()
{
    for (int i = 0; i < countof(ctors); i++)
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




