format PE console

entry start

include 'win32a.inc'

section '.data' data readable writeable

        formatStr db '%s', 0
        formatNum db '%d', 0

        name rd 2
        age rd 1

        qName db 'What is your name? ', 0
        qAge db 'How old are you? ', 0
        hello db 'Hello %s, %d! ', 0

       ; address db 10, 13, 'hello var address is %d',0

        NULL = 0

section '.code' code readable executable

        start:
                push qName
                call [printf]

                push name
                push formatStr
                call [scanf]

                push qAge
                call [printf]

                push age
                push formatNum
                call [scanf]

                push [age]
                push name
                push hello
                call [printf]

              ;  lea eax, [hello]
              ;  push eax
             ;   push address
             ;   call [printf]

                call [getch]
                push NULL
                call [ExitProcess]

section '.idata' import data readable

        library kernel, 'kernel32.dll',\
                msvcrt, 'msvcrt.dll'

        import kernel,\
               ExitProcess, 'ExitProcess'

        import msvcrt,\
               printf, 'printf',\
               scanf, 'scanf',\
               getch, '_getch'