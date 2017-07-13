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
__declspec(allocate(".sec$Z")) struct person* ctors_end   = NULL;

//
// - Nekdo muze preferovat jine poradi.
// - Pri kompilaci na Release se vsak nic nevypise.
//   - Ackoliv kompiler podle standardu nesmi odstranit globalni promenne,
//     linker se zapnutou Link Time Optimization vyoptimalizuje pryc veskere
//     nereferencovane promenne - i globalni.
//   - volatile nepomuze
//   - Tyka se to C i C++ (GCC i clang).
//   - Musime donutit linker, aby symboly neodstranoval.
//

struct person middle1 = { .name = "John", .age = 31 };
__declspec(allocate(".sec$M")) struct person* ctors_middle1 = &middle1;

struct person middle2 = { .name = "Mike", .age = 42 };
__declspec(allocate(".sec$M")) struct person* ctors_middle2 = &middle2;

struct person middle3 = { .name = "Gabe", .age = 53 };
__declspec(allocate(".sec$M")) struct person* ctors_middle3 = &middle3;

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
