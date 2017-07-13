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
// - Co se deje s destruktory globalnich objektu?
//   - Jsou registrovany funkci atexit() pri volani wrapperu pro konstruktor.
//   - If an application terminates with _exit() or _Exit() or std::quick_exit() no destructors get called.
//   - The standard says that if an exception is not caught, std::terminate() is called.
//     std::terminate(), by default, calls abort(). And abort() is not allowed to call destructors.
//

void dtor1()
{
    person::~person(&p);
}

void ctor1()
{
    person::person(&p, "John", 31);

    //
    // - Registrace destruktoru.
    //
    atexit(&dtor1);
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




