format PE console

entry start

include 'win32a.inc'

section '.data' data readable writable

        helloStr db 'Hello World!', 0

        NULL = 0

section '.code' code readable executable

        start:
                push helloStr
                call [printf]

                call [getch]

                push NULL
                call[ExitProcess]

section '.data' import data readable

        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess'

        import msvcrt,\
               printf, 'printf',\
               getch, '_getch'