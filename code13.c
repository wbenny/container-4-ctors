#include <stdio.h>

struct person
{
  int id;
  const char* name;
  int age;
};

//
// - Umele zvysime counter.
//   - id nyni zacina od 2.
//
int unused1 = __COUNTER__;
int unused2 = __COUNTER__;

#pragma section(".sec$A", read)
#pragma section(".sec$M", read)
#pragma section(".sec$Z", read)

__declspec(allocate(".sec$A")) struct person* ctors_begin = NULL;
__declspec(allocate(".sec$Z")) struct person* ctors_end = NULL;

#define LINKER_INCLUDE_SYMBOL(symbol)                       \
  __pragma(comment(linker, "/include:" #symbol))

#define DECLSPEC_SECTION                                    \
  __declspec(allocate(".sec$M"))

#define MAKE_PERSON(var, arg_name, arg_age)                 \
  struct person var = {                                     \
    .id   = __COUNTER__,                                    \
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

    printf("id: %i, name: %s, age: %i\n", (*ctor)->id, (*ctor)->name, (*ctor)->age);
  }
}

int main()
{
  run_ctors();

  return 0;
}
