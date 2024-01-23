.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib C:\Users\givi\Documents\Курсовой проект КПО\StaticLibrary\StaticLiblary\StaticLibrary.lib
ExitProcess proto :dword

EXTRN BREAKL: proc
EXTRN OutputInt: proc
EXTRN OutputChar: proc
EXTRN OutputBool: proc

.stack 4096

.const
	L1 sdword 2
	L2 sdword 0
	L3 sdword 1
	L4 sdword 0
	L5 sdword 214013
	L6 sdword 2531011
	L7 sdword 2147483647
	L8 sdword 10
	L9 word '6'
	L10 sdword 6
	L11 sdword 1

.data
	buffer BYTE 256 dup(0)
	EvenCheckh word 0
	primaryiseven word 0
	primaryq word ?
	primaryn sdword 0
	primaryl sdword 0
	primaryz sdword 0

.code
EvenCheck proc EvenCheckk : sdword
	push EvenCheckk
	push L1
	pop ebx
	pop eax
	cdq
	idiv ebx
	push edx
	pop EvenCheckk
	mov eax, EvenCheckk
	cmp eax, L2
	jz m0
	jnz m1
	je m1
m0:
	push L3
	pop EvenCheckh
	jmp e0
m1:
	push L4
	pop EvenCheckh
e0:
	push EvenCheckh
	jmp local0
local0:
	pop eax
	ret
EvenCheck ENDP

r proc rs : sdword
	push rs
	push L5
	pop eax
	pop ebx
	mul ebx
	push eax
	push L6
	pop eax
	pop ebx
	add eax, ebx
	push eax
	push L7
	pop ebx
	pop eax
	cdq
	idiv ebx
	push edx
	pop rs
	push rs
	jmp local1
local1:
	pop eax
	ret
r ENDP

main proc
	push L8
	pop edx
	push L8
	call EvenCheck
	push eax
	pop primaryiseven
	push primaryiseven
	call OutputBool
	call BREAKL
	push L9
	pop primaryq
	push primaryq
	call OutputChar
	push L10
	pop primaryn
	push L10
	pop primaryl
	push primaryl
	call OutputInt
	call BREAKL
	push L11
	pop edx
	push L11
	call r
	push eax
	pop primaryz
	push primaryz
	call OutputInt
	call ExitProcess
main ENDP
end main