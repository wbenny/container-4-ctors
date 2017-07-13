#include <stdio.h>

struct person
{
  const char* name;
  int age;
};

#pragma section(".sec$A", read)
#pragma section(".sec$M", read)
#pragma section(".sec$Z", read)

//
// - Vkladat cele struktury do sekce muze byt lakave, ale je to neprakticke.
// - Kompiler/linker vklada struktury s ruznym zarovnanim a paddingem.
// - alignas, __declspec(align(n)), ani union + char padding[16] nepomaha.
// - Nejlepsi reseni je pouzit pointery.
//

__declspec(allocate(".sec$A")) struct person ctors_begin   = { .name = NULL,   .age = 0  };
__declspec(allocate(".sec$M")) struct person ctors_middle1 = { .name = "John", .age = 31 };
__declspec(allocate(".sec$M")) struct person ctors_middle2 = { .name = "Mike", .age = 42 };
__declspec(allocate(".sec$M")) struct person ctors_middle3 = { .name = "Gabe", .age = 53 };
__declspec(allocate(".sec$Z")) struct person ctors_end     = { .name = NULL,   .age = 0  };

void run_ctors()
{
  for (struct person* ctor = &ctors_begin; ctor != &ctors_end; ctor++)
  {
    printf("name: %s, age: %i\n", ctor->name, ctor->age);
  }
}

int main()
{
  run_ctors();

  return 0;
}

