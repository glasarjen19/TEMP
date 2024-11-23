#pragma once

#include <Windows.h>
#include <iostream>
#include <string>


constexpr DWORD KOFFIEKOEK_PADDING = 0x20;
constexpr DWORD KOFFIEKOEK_ASM_LEN_OPCODE = 15;


extern DWORD64 supernormal_base;
extern DWORD64 supernormal_lower_bound;
extern DWORD64 supernormal_upper_bound;
extern DWORD64 supernormal_asm_decrypted;


LONG KoffieKoekAsmHeap(EXCEPTION_POINTERS* ExceptionInfo);
BOOL KoffieBreekPunt(PVOID addr);
DWORD KoffieKiekAsmIndex(DWORD64 supernormal_sc_base, DWORD64 supernormal_addr_asm, std::string sleutel);
typedef NTSTATUS(WINAPI* KoffIe)(void);
void LogWinapiError(std::string failedFunction);
