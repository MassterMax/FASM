;  Gudzikevich Maxim BSE198
;
;  Task 8:
;
;  Read an A-array, then create and print the B-array, according to the rule:
;
;         { A[i] + 5,   if  A[i]  >  5
;  B[i] = { A[i] - 5,   if  A[i]  < -5
;         { 0,          otherwise
;

format PE console

entry start

include 'win32a.inc'

section '.data' data readable writable

        formatNum       db '%d', 0

        msgInputAEl     db 'A[%d] = ', 0
        msgInputASize   db 'Enter the size of an A-array: ', 0

        msgA            db 'This is the A-array:', 10, 0
        msgB            db 'This is the B-array:', 10, 0
        msgAEl          db 'A[%d] = %d', 10, 0
        msgBEl          db 'B[%d] = %d', 10, 0
        msgEmpty        db '', 10, 0

        i       dd ?
        size    dd ?
        tmpA    dd ?
        tmpB    dd ?
        tmpSt   dd ?

        arrA    rd 100
        arrB    rd 100

section '.code' code readable executable

        start:
                call createA
                call createB
                call printB
               ;call printA
        finish:
                call [getch]
                push 0
                call [ExitProcess]

;-------------------------------------------------------------------------------------------;

        proc createA

                readSize:
                        invoke printf, msgInputASize
                        add esp, 4

                        invoke scanf, formatNum, size
                        add esp, 8

                        cmp [size], 0
                        jle readSize

                mov ebx, arrA
                xor ecx, ecx             ; This will be a counter of current element

                readLoop:
                        mov [tmpA], ebx
                        cmp ecx, [size]
                        jge readEnd

                        mov [i], ecx
                        invoke printf, msgInputAEl, ecx
                        add esp, 8

                        invoke scanf, formatNum, ebx        ; WTF CRUSH IF WE USE EAX INSTEAD OF EBX IN WHOLE PROCEDURE
                        add esp, 8

                        mov ecx, [i]
                        inc ecx

                        mov ebx, [tmpA]
                        add ebx, 4

                        jmp readLoop

                readEnd:
                        ret

        endp

;-------------------------------------------------------------------------------------------;

        proc createB

                mov [tmpSt], esp         ; Save stack pointer
                mov eax, arrA
                mov ebx, arrB
                xor ecx, ecx             ; Counter again

                createLoop:
                        cmp ecx, [size]
                        jge createEnd    ; Exit if we run out
                        mov [i], ecx

                        mov ecx, [eax]   ; We have A[i] in ecx now

                        cmp ecx, 5
                        jg createGF

                        cmp ecx, -5
                        jl createLF

                        jmp createE

                createGF:
                        add ecx, 5
                        jmp createLoopEnd

                createLF:
                        sub ecx, 5
                        jmp createLoopEnd

                createE:
                        xor ecx, ecx
                        jmp createLoopEnd

                createLoopEnd:
                        mov [ebx], ecx
                        mov ecx, [i]
                        inc ecx
                        add eax, 4
                        add ebx, 4
                        jmp createLoop

                createEnd:
                        mov esp, [tmpSt] ; Bring the pointer back
                        ret

        endp

;-------------------------------------------------------------------------------------------;

        proc printA

                invoke printf, msgEmpty
                add esp, 4

                invoke printf, msgA
                add esp, 4

                mov [tmpSt], esp
                mov ebx, arrA
                xor ecx, ecx

                printLoopA:
                        mov [tmpA], ebx

                        cmp ecx, [size]
                        jge printEndA

                        mov [i], ecx

                        invoke printf, msgAEl, ecx, dword [ebx]

                        mov ecx, [i]
                        inc ecx

                        mov ebx, [tmpA]
                        add ebx, 4

                        jmp printLoopA

                printEndA:
                        mov esp, [tmpSt]
                        ret

        endp

;-------------------------------------------------------------------------------------------;

        proc printB

                invoke printf, msgEmpty
                add esp, 4

                invoke printf, msgB
                add esp, 4

                mov [tmpSt], esp
                mov ebx, arrB
                xor ecx, ecx

                printLoopB:
                        mov [tmpB], ebx

                        cmp ecx, [size]
                        jge printEndB

                        mov [i], ecx

                        invoke printf, msgBEl, ecx, dword [ebx]

                        mov ecx, [i]
                        inc ecx

                        mov ebx, [tmpB]
                        add ebx, 4

                        jmp printLoopB

                printEndB:
                        mov esp, [tmpSt]
                        ret

        endp

;-------------------------------------------------------------------------------------------;

section '.idata' import data readable

        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess'
               ;HeapAlloc, 'HeapAlloc',\ 
               ;GetProcessHeap, 'GetProcessHeap'

        import msvcrt,\
               printf, 'printf',\
               scanf, 'scanf',\
               getch, '_getch' 