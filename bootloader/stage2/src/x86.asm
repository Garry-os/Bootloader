
;; Enter real mode
%macro EnterRealMode 0
	[bits 32]
	jmp word 0x18:.pmode16 ; Jump to 16 bit protected segment
.pmode16:
	[bits 16]
	; Disable protection bit
	mov eax, cr0
	and eax, ~(1)
	mov cr0, eax

	;; Perform a jump to real mode
	jmp word 0x00:.rmode
.rmode:
	; Setup segmnents
	mov ax, 0
	mov ds, ax
	mov es, ax

	; Enable interrupts
	sti
%endmacro

;; Enter protected mode
%macro EnterProtectedMode 0
	cli

	;; Enable protection bit
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	jmp dword 0x08:.pmode
.pmode:
	[bits 32]

	; Setup segment registers
	mov ax, 0x10
	mov ds, ax
	mov ss, ax
%endmacro

global x86_Test
x86_Test:
	[bits 32]
	
	push ebp
	mov ebp, esp
	
	EnterRealMode
	[bits 16]
	
	push ax

	; Test 0x10 interrupts
	mov ah, 0x0E
	mov al, 'i'
	stc
	int 0x10

	pop ax

	EnterProtectedMode
	[bits 32]

	mov esp, ebp
	pop ebp
	ret

	

