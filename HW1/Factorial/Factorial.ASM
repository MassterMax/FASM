format PE console
entry _start

include 'win32a.inc'

section '.data' data readable writable
p: db 'pause > null', 0
entry_message: db 'enter a number to calculate his factorial: ', 10, 0
n: dd ?
num: db 'result %d', 10, 0

section '.code' readable writable 
_start:
    start:
        push entry_message
        call [printf]
        push    12
        push    n
        push    0
        call    [_read]
        add     ebp, 4

        push    10
        push    NULL     
        push    n
        call    [strtol]
        add     esp, 4

        push    eax
        call _fact
        add esp, 4
        push 0
        push eax
        push num
        call [printf]
        add esp, 4
        call [getch]
        call [ExitProcess]
    
_fact:
        push ebp
        mov ebp, esp
        
        mov eax,[ebp+8]
        
        cmp eax, 1
        je _end

        dec eax
        push eax
        call _fact

        mov ebx, [ebp+8]
        mul ebx

_end:
    mov esp, ebp
    pop ebp
    ret
 
section '.idata' import data readable 
library kernel32, 'kernel32.dll', \
        msvcrt, 'msvcrt.dll'

import kernel32, \
        ExitProcess, 'ExitProcess' \ 

import msvcrt, \
        printf, 'printf', \
        _read, '_read', \
        strtol,'strtol',\
        system, 'system',\
        getch, '_getch'