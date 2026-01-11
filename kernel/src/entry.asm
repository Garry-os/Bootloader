bits 32
extern long_mode_start

CPUID_EXTENSIONS equ 0x80000000
CPUID_EXT_FEATURES equ 0x80000001

section .entry

global entry
entry:
	cli

	mov esp, stack_top ;; Load a new stack

	;; Do the checks
	call check_cpuid
	call check_long_mode

	; Setup for long mode
	call setup_page_tables
	call enable_long_mode

	; Load GDT
	lgdt [GDT.Ptr]
	jmp GDT.Code:long_mode_start

.halt:
	cli
	hlt

;; Check if CPUID is avaliable
check_cpuid:
	pushfd ;; eflags cannot be directly accessed
	pop eax

	;; Save the original eflags
	mov ecx, eax
	xor eax, (1 << 21) ;; Flip the bit

	;; Store back into the eflags
	;; It will check if CPUID is supported
	push eax
	popfd
	pushfd
	pop eax

	; Restore eflags
	push ecx
	popfd

	;; Check
	xor eax, ecx
	jnz .supported
.not_supported:
	mov [0xb8000], 'C' ; Error code
	cli
	hlt
.supported:
	ret

;; Check if long mode is supported
check_long_mode:
	mov eax, CPUID_EXTENSIONS
	cpuid
	cmp eax, CPUID_EXT_FEATURES
	jb .not_supported

	mov eax, CPUID_EXT_FEATURES
	cpuid
	test edx, (1 << 29) ; If long mode is supported
	jz .not_supported

	ret

.not_supported:
	mov [0xb8000], 'L'
	cli
	hlt

setup_page_tables:
	mov eax, page_table_l3
	or eax, 0b11 ; Present, writable
	mov [page_table_l4], eax

	mov eax, page_table_l2
	or eax, 0b11 ; Present, writable
	mov [page_table_l3], eax

	mov ecx, 0
.loop:
	;; Identity mapping
	mov eax, 0x200000
	mul ecx
	or eax, 0b10000011 ; Present, writable, huge page
	mov [page_table_l2 + ecx * 8], eax

	inc ecx
	cmp ecx, 512
	jne .loop

	ret
enable_long_mode:
	; Pass page table address to the CPU
	mov eax, page_table_l4
	mov cr3, eax

	; Enable PAE
	mov eax, cr4
	or eax, (1 << 5)
	mov cr4, eax

	; Enable long mode
	mov ecx, 0xC0000080
	rdmsr
	or eax, (1 << 8)
	wrmsr

	; Enable paging
	mov eax, cr0
	or eax, (1 << 31)
	mov cr0, eax

	ret


section .rodata

; Access bits
PRESENT        equ 1 << 7
NOT_SYS        equ 1 << 4
EXEC           equ 1 << 3
DC             equ 1 << 2
RW             equ 1 << 1
ACCESSED       equ 1 << 0

; Flags bits
GRAN_4K       equ 1 << 7
SZ_32         equ 1 << 6
LONG_MODE     equ 1 << 5

GDT:
	.Null: equ $ - GDT
		dq 0 ; NULL descriptor
	.Code: equ $ - GDT
		dw 0xFFFF
		dw 0
		db 0
		db PRESENT | NOT_SYS | EXEC | RW
		db GRAN_4K | LONG_MODE | 0xF
		db 0
	.Data: equ $ - GDT
		dw 0xFFFF
		dw 0
		db 0
		db PRESENT | NOT_SYS | RW
		db GRAN_4K | SZ_32 | 0xF
		db 0
	.Ptr:
		dw $ - GDT - 1 ; Limit
		dq GDT ; Base


section .bss
;; Temperary page table
;; The kernel will switch to its own later
align 4096
page_table_l4:
	resb 4096
page_table_l3:
	resb 4096
page_table_l2:
	resb 4096
stack_bottom:
	resb 4096 * 4 ; 16 Kib
stack_top:

