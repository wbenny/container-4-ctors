#include <stdio.h>

struct person
{
  const char* name;
  int age;
};

#pragma section(".sec$A", read)
#pragma section(".sec$M", read)
#pragma section(".sec$Z", read)

__declspec(allocate(".sec$A")) struct person* ctors_begin = NULL;
__declspec(allocate(".sec$Z")) struct person* ctors_end = NULL;

//
// - Program opet funguje, ale promenne jsou v nahodnem poradi.
//   - Na rozdil od konstruktoru v C++ (volani konstruktoru
//     v jednom .cpp je vzdy postupne).
//
//   - GCC & clang:
//      - Poradi je taktez nahodne.
//

#define LINKER_INCLUDE_SYMBOL(symbol)                       \
  __pragma(comment(linker, "/include:" #symbol))

#define DECLSPEC_SECTION                                    \
  __declspec(allocate(".sec$M"))

#define MAKE_PERSON(var, arg_name, arg_age)                 \
  struct person var = {                                     \
    .name = arg_name,                                       \
    .age  = arg_age                                         \
  };                                                        \
  LINKER_INCLUDE_SYMBOL(var);                               \
  DECLSPEC_SECTION struct person* ctors_##var = &var

MAKE_PERSON(middle1, "John", 31);
MAKE_PERSON(middle2, "Mike", 42);
MAKE_PERSON(middle3, "Gabe", 53);

void run_ctors()
{
  for (struct person** ctor = &ctors_begin; ctor != &ctors_end; ctor++)
  {
    if (*ctor == NULL)
    {
      continue;
    }

    printf("name: %s, age: %i\n", (*ctor)->name, (*ctor)->age);
  }
}

int main()
{
  run_ctors();

  return 0;
}
