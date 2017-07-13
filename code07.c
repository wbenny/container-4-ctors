#include <stdio.h>

struct person
{
  const char* name;
  int age;
};

struct person middle1 = { .name = "John", .age = 31 };
struct person middle2 = { .name = "Mike", .age = 42 };
struct person middle3 = { .name = "Gabe", .age = 53 };

#pragma section(".sec$A", read)
#pragma section(".sec$M", read)
#pragma section(".sec$Z", read)

//
// - Prvni funkcni prototyp.
// - Samotna struktura NESMI byt soucasti sekce .sec.
//   - Doslo by k michani pointeru a obsahu struktury.
//   - Pouze pointery na ni.
//
//   - GCC & clang:
//       __declspec(allocate(".sec$A"))
//               ----->
//       extern struct person* __start_sec;
//
//       __declspec(allocate(".sec$Z"))
//               ----->
//       extern struct person* __stop_sec;
//
//       __declspec(allocate(".sec$M"))
//               ----->
//       __attribute__((section("sec")))
//        
//       (neni potreba nic jako #pragma section("sec"))
//       (v nazvu sekce nesmi byt tecka)
//

__declspec(allocate(".sec$A")) struct person* ctors_begin   = NULL;
__declspec(allocate(".sec$M")) struct person* ctors_middle1 = &middle1;
__declspec(allocate(".sec$M")) struct person* ctors_middle2 = &middle2;
__declspec(allocate(".sec$M")) struct person* ctors_middle3 = &middle3;
__declspec(allocate(".sec$Z")) struct person* ctors_end     = NULL;

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

