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

void dtor1()
{
    person::~person(&p);
}

void ctor1()
{
    person::person(&p, "John", 31);
    atexit(&dtor1);
}

/////////////////////////////////////////////////////////////////////////////////

//
// - Kazdy nazev sekce muze mit specifikovan "dodatecny nazev" za znakem "$".
// - Vse za "$" je ve skutecnosti soucasti sekce s nazvem pred "$".
// - Kompiler radi promenne v sekci podle casti za "$".
//

typedef void (*initfn_t)();

#pragma section(".CRT$XCA", read)
#pragma section(".CRT$XCM", read)
#pragma section(".CRT$XCZ", read)

//
// Veskery obsah sekce .CRT je vlozen do sekce .rdata.
//
#pragma comment(linker, "/merge:.CRT=.rdata")

__declspec(allocate(".CRT$XCA")) initfn_t ctors_begin;
__declspec(allocate(".CRT$XCM")) initfn_t ctors_middle1 = &ctor1;
__declspec(allocate(".CRT$XCZ")) initfn_t ctors_end;

void run_ctors()
{
    for (initfn_t* ctor = &ctors_begin; ctor != &ctors_end; ctor++)
    {
        if (*ctor == NULL)
        {
            //
            // - Ignorujeme padding - vzdy NULL.
            //
            continue;
        }

        (*ctor)();
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




