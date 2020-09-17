                                format  PE console 4.0
    include 'win32a.inc'
 
start:  cinvoke printf, sent0
    cinvoke scanf, sent4, A
    mov eax, [A]
    mov ebx, eax
    mov ecx, eax
    shl eax, 2
    shr ebx, 2
    cinvoke printf, sent1, eax, ebx
 
;   cinvoke printf, endl
;
;   mov eax, [A]
;   rol ebx, 2d
;   cinvoke printf, sent2, eax
;   cinvoke printf, endl
 
;   mov eax, [A]
;   rcl eax, 2d
;   cinvoke printf, sent3, eax
;   cinvoke printf, endl
 
;   invoke  sleep, 50000
    invoke  _getch
;   jmp start   ; e??? ?? ctr/c
 
sentences:
 
    invoke  exit, 0
;
    sent0   db 'Enter A:', 0
    sent1   db 'shl [A] by 2 = %d', 0Dh, 0Ah
    sent2   db 'shr [A] by 2 = %d', 0Dh, 0Ah,0
    sent4   db '%d', 0
;   endl    db '', 10
    A   dd ?
 
 
    data    import
 
    library msvcrt,'MSVCRT.DLL';,\
;   kernel32,'KERNEL32.DLL'
 
;   import  kernel32,\
;   sleep,'Sleep'
 
    import  msvcrt,\
    _getch,'_getch',\
    scanf,'scanf',\
    printf,'printf',\
    exit,'exit'
    end data