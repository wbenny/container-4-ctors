#include <stdio.h>
#include <stdlib.h>

struct person
{
  int id;
  const char* name;
  int age;
};

int unused1 = __COUNTER__;
int unused2 = __COUNTER__;

#pragma section(".sec$A", read)
#pragma section(".sec$M", read)
#pragma section(".sec$Z", read)
#pragma comment(linker, "/merge:.sec=.rdata")

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
  LINKER_INCLUDE_SYMBOL(ctors_##var);                       \
  DECLSPEC_SECTION struct person* ctors_##var = &var

//
// - Poznacime si prvni a posledni pouzitou hodnotu __COUNTER__.
// - Vytvorime promennou pro ulozeni jejich poctu.
//
int person_id_first = __COUNTER__;

  MAKE_PERSON(middle1, "John", 31);
  MAKE_PERSON(middle2, "Mike", 42);
  MAKE_PERSON(middle3, "Gabe", 53);

int person_id_last = __COUNTER__;
int person_count = 0;

//
// - Vytvorime promennou pro mapovani "id" -> pointer to person.
//
struct person** person_map = NULL;

void run_ctors()
{
  //
  // - Pocet elementu je posledni_id - prvni_id - 1 (poznacene posledni_id je o 1 vyssi nez pouzite posledni id)
  //
  person_count = person_id_last - person_id_first - 1;
  //
  // - Alokujeme misto pro mapu.
  //
  person_map   = malloc(sizeof(struct person*) * person_count);

  printf("run_ctors():\n");
  printf("  person_id_first = %d\n", person_id_first);
  printf("  person_id_last  = %d\n", person_id_last);
  printf("  person_count    = %d\n", person_count);
  printf("  --------------------\n");

  for (struct person** ctor = &ctors_begin; ctor != &ctors_end; ctor++)
  {
    if (*ctor == NULL)
    {
      continue;
    }

    //
    // - Opravime hodnotu ID tak, aby zacinala od 0.
    //   - Hodnota id_first je vzdy o 1 mensi nez hodnota realneho prvniho elementu,
    //     proto musime odecist 1 navic.
    //
    (*ctor)->id -= person_id_first + 1;

    //
    // - Namapujeme index (id) na pointer.
    //
    person_map[(*ctor)->id] = *ctor;

    printf("  id: %i, name: %s, age: %i\n", (*ctor)->id, (*ctor)->name, (*ctor)->age);
  }

  printf("\n");
}

void enumerate_from_map()
{
  printf("enumerate_from_map():\n");

  //
  // - Nyni muzeme proenumerovat mapu. Prvky jsou serazene.
  //
  for (int i = 0; i < person_count; i++)
  {
    printf("  [%i] id: %i, name: %s, age: %i\n", i,
      person_map[i]->id, person_map[i]->name, person_map[i]->age);
  }
}

int main()
{
  run_ctors();
  enumerate_from_map();

  return 0;
}

//
// - Mozna vylepseni:
//   - Ukladani pointeru na konstruktor primo do struktury.
//   - Ukladani typu primo do struktury.
//
