org 0x8000
bits 16

entry:
	cli ; Disable interrupts

	;; Setup segment registers
	mov ax, 0
	mov ds, ax
	mov es, ax

	; Setup stack
	mov ax, ds
	mov ss, ax
	mov sp, 0xFFF0
	mov bp, sp

	;; Start switching to protected mode
	call Enable_A20
	call LoadGDT

	;; Set protection flag in CR0
	mov eax, cr0
	or eax, 1
	mov cr0, eax

	;; Perform a far jump to protected mode
	jmp dword 0x08:.pmode
.pmode:
	[bits 32]

	;; Setup segment registers
	mov ax, 0x10
	mov ds, ax
	mov ss, ax

	; Print
	; mov eax, 'h'
	; mov [0xB8000], eax
	mov al, 'h'
	out 0xE9, al

.halt:
	cli
	hlt

Enable_A20:
	[bits 16]

	in al, 0x92
	or al, 2
	out 0x92, al

LoadGDT:
	[bits 16]

	lgdt [GDTDesc]
	ret

GDT:
	; NULL descriptor
	dq 0

	; 32 bit code segment
	dw 0xFFFF
	dw 0
	db 0
	db 10011010b
	db 11001111b
	db 0

	;; 32 bit data segment
	dw 0xFFFF
	dw 0
	db 0
	db 10010010b
	db 11001111b
	db 0

GDTDesc: 
	dw GDTDesc - GDT - 1 ;; Limit (Size of GDT)
	dd GDT ;; GDT base (address)

