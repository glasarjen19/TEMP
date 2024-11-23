#include "SuperNormalVG.h"
#include "SuperNormalPL.h"


DWORD64 supernormal_base = 0;
DWORD64 supernormal_lower_bound = 0;
DWORD64 supernormal_upper_bound = 0;
DWORD64 supernormal_asm_decrypted = 0;

BOOL KoffieBreekPunt(PVOID addr)
{
    CONTEXT ctx = { 0 };
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

    HANDLE current_thread = GetCurrentThread();
    if (!GetThreadContext(current_thread, &ctx)) {
        return FALSE;
    }

    ctx.Dr0 = (UINT64)addr;
    ctx.Dr7 |= (1 << 0);     
    ctx.Dr7 &= ~(1 << 16);   
    ctx.Dr7 &= ~(1 << 17);   
    ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

    return SetThreadContext(current_thread, &ctx);
}

LONG KoffieKoekAsmHeap(EXCEPTION_POINTERS* exception_info)
{
    if (exception_info->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
    {
        if (exception_info->ContextRecord->Dr0)
        {
            exception_info->ContextRecord->Dr0 = 0;
        }

        exception_info->ContextRecord->EFlags |= (1 << 8);

        if (exception_info->ContextRecord->Rip < supernormal_lower_bound ||
            exception_info->ContextRecord->Rip > supernormal_upper_bound)
        {
            return EXCEPTION_CONTINUE_EXECUTION;
        }

        DWORD64 addr_asm = exception_info->ContextRecord->Rip;

        if (supernormal_asm_decrypted)
        {
            DWORD key_index = KoffieKiekAsmIndex(supernormal_base, supernormal_asm_decrypted, sleutel);

            PBYTE addr_decoded = (PBYTE)supernormal_asm_decrypted;
            for (INT i = 0; i < KOFFIEKOEK_ASM_LEN_OPCODE; i++)
            {
                if (key_index == sleutel.size())
                {
                    key_index = 0;
                }
                addr_decoded[i] ^= sleutel[key_index];
                key_index++;
            }
        }

        PBYTE current_asm = (PBYTE)addr_asm;
        DWORD key_index = KoffieKiekAsmIndex(supernormal_base, addr_asm, sleutel);
        for (INT i = 0; i < KOFFIEKOEK_ASM_LEN_OPCODE; i++)
        {
            if (key_index == sleutel.size())
            {
                key_index = 0;
            }
            current_asm[i] ^= sleutel[key_index];
            key_index++;
        }

        supernormal_asm_decrypted = addr_asm;
        return EXCEPTION_CONTINUE_EXECUTION;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

DWORD KoffieKiekAsmIndex(DWORD64 base, DWORD64 addr, std::string sleutel)
{
    DWORD key_size = sleutel.size();
    DWORD64 offset = addr - base;
    return offset % key_size;
}

void LogWinapiError(std::string failed_function)
{
    std::cout << "[FOUT] - " << failed_function << " mislukt met foutcode: " << GetLastError() << std::endl;
}
