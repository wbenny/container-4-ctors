1. Klasicke pouziti konstruktoru.
2. Konstruktor globalniho objektu.
    - Kompilator pro kazdy globalni objekt, ktery potrebuje zavolat konstruktor,
      vytvori standalone funkci (bezparametrickou, bez navratove hodnoty).
    - Funkce pote zavola "metodu" konstruktoru s danymi parametry.
3. Co se deje s destruktory globalnich objektu?
    - Jsou registrovany funkci atexit() pri volani wrapperu pro konstruktor.
        - If an application terminates with _exit() or _Exit() or std::quick_exit() no destructors get called.
        - The standard says that if an exception is not caught, std::terminate() is called.
          std::terminate(), by default, calls abort(). And abort() is not allowed to call destructors.

4. Konstruktory jsou definovany ve specialni sekci.
    - V pripade MSVC je to sekce ".CRT"
    - Sekce je proiterovana a jsou zavolany veskere wrappery pro konstruktory.
    - GCC & clang:
        - Pro kazdy .cpp unit kompiler vytvori jednu funkci, ktera postupne vola vsechny konstruktory v onom .cpp.
        - Pointery na tyto funkce jsou vlozeny do sekce .init_array (a volany pred main()).
        - V pripade LTO se vice techto funkci muze spojit do jedne.

5. Kazdy nazev sekce muze mit specifikovan "dodatecny nazev" za znakem "$".
    - Vse za "$" je ve skutecnosti soucasti sekce s nazvem pred "$".
    - Kompiler radi promenne v sekci podle casti za "$".
    - Veskery obsah sekce .CRT je vlozen do sekce .rdata.
    - Ignorujeme padding - vzdy NULL.

(C)
6. Vkladat cele struktury do sekce muze byt lakave, ale je to neprakticke.
    - Kompiler/linker vklada struktury s ruznym zarovnanim a paddingem.
    - alignas, __declspec(align(n)), ani union + char padding[16] nepomaha.
    - Nejlepsi reseni je pouzit pointery.

7. Prvni funkcni prototyp.
    - Samotna struktura NESMI byt soucasti sekce .sec.
    - Doslo by k michani pointeru a obsahu struktury.
    - Pouze pointery na ni.

    - GCC & clang:
        __declspec(allocate(".sec$A"))  
                ----->
        extern struct person* __start_sec;

        __declspec(allocate(".sec$Z"))
                ----->
        extern struct person* __stop_sec;

        __declspec(allocate(".sec$M"))
                ----->
        __attribute__((section("sec")))
        
        (neni potreba nic jako #pragma section("sec"))
        (v nazvu sekce nesmi byt tecka)

8. $A a $Z muze byt na zacatku.
    - Veskere promenne v sekci mohou byt v ruznem poradi.
    - Mohou byt prokladane i ostatnimi promennymi (ktere nejsou soucasti sekce .sec).

9. Nekdo muze preferovat jine poradi.
    - Pri kompilaci na Release se vsak nic nevypise.
    - Ackoliv kompiler podle standardu nesmi odstranit globalni promenne,
      linker se zapnutou Link Time Optimization vyoptimalizuje pryc veskere
      nereferencovane promenne - i globalni.
    - volatile nepomuze
    - Tyka se to C i C++ (GCC i clang).
    - Musime donutit linker, aby symboly neodstranoval.

10. Oznamime linkeru aby neodstranoval symboly v nasi sekci.
    - Nyni je cas na to, abychom pouzili makra.
    - #pragma muze byt pouzita v makru pomoci __pragma()

    - GCC & clang:
        pragma comment(linker, "/include:ctors_middle1")
                ----->
        __attribute__((used))

11. Program opet funguje, ale promenne jsou v nahodnem poradi.
    - Na rozdil od konstruktoru v C++ (volani konstruktoru
      v jednom .cpp je vzdy postupne).

    - GCC & clang:
        - Poradi je taktez nahodne.

12. Pridame novy clen do struktury: id
    - V definici do nej pote priradime hodnotu __COUNTER__
    - __COUNTER__ zacina na 0 a pokazde, kdyz je pouzity, se jeho hodnota
      inkrementuje o 1
    - Funguje v MSVC, GCC i clangu
    - Musime si dat pozor, aby __COUNTER__ mezi definicemi nebyl pouzit
    - To znamena neincludovat zadny dalsi header.

13. Umele zvysime counter.
    - id nyni zacina od 2.

14. Poznacime si prvni a posledni pouzitou hodnotu __COUNTER__.
    - Vytvorime promennou pro ulozeni jejich poctu.
    - Vytvorime promennou pro mapovani "id" -> pointer to person.
    - Pocet elementu je posledni_id - prvni_id - 1 (poznacene posledni_id je o 1 vyssi nez pouzite posledni id)
    - Alokujeme misto pro mapu.
    - Opravime hodnotu ID tak, aby zacinala od 0.
    - Hodnota id_first je vzdy o 1 mensi nez hodnota realneho prvniho elementu,
      proto musime odecist 1 navic.
    - Namapujeme index (id) na pointer.
    - Nyni muzeme proenumerovat mapu. Prvky jsou serazene.
    - Merge sekci:
        - #pragma comment(linker, "/merge:.sec=.rdata")
    - Mozna vylepseni:
        - Ukladani pointeru na konstruktor primo do struktury.
        - Ukladani typu primo do struktury.
