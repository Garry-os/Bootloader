
section .text

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

; Convert linear address to segment:offset address
; Args:
;    1 - linear address
;    2 - (out) target segment (e.g. es)
;    3 - target 32-bit register to use (e.g. eax)
;    4 - target lower 16-bit half of #3 (e.g. ax)
%macro LinearToSegOffset 4
    mov %3, %1      ; linear address to eax
    shr %3, 4
    mov %2, %4
    mov %3, %1      ; linear address to eax
    and %3, 0xf
%endmacro

;; Disk reset operation
;; Arguments: Drive
;; Return: 1 if succeed, otherwise, 0
global x86_DiskReset
x86_DiskReset:
	[bits 32]
	push ebp
	mov ebp, esp

	EnterRealMode
	[bits 16]

	mov ah, 0 ; Disk reset
	mov dl, [bp + 8] ; Boot drive
	stc
	int 0x13

	mov eax, 1
	sbb eax, 0

	push eax
	EnterProtectedMode
	[bits 32]
	pop eax

	mov esp, ebp
	pop ebp
	ret

;; Disk reading operation
;; Arguments: Drive, LBA, sector count, data output address
;; Return: 1 if succeed, otherwise, 0
global x86_DiskRead
x86_DiskRead:
	[bits 32]
	
	push ebp
	mov ebp, esp

	EnterRealMode
	[bits 16]

	; Save registers
	push ebx
	push es

	mov dl, [bp + 8] ; Boot drive
	mov eax, [bp + 12] ; LBA
	mov cl, [bp + 16] ; Sector count
	
	LinearToSegOffset [bp + 20], es, ebx, bx

	;; Setup DAP
	mov [extension_dap.lba], eax
	mov [extension_dap.segment], es
	mov [extension_dap.offset], bx
	mov [extension_dap.count], cl

	mov si, extension_dap

	;; Read disk
	mov ah, 0x42 ; Enchanced disk reading
	stc
	int 0x13

	;; Set return value
	mov eax, 1
	sbb eax, 0

	; Restore registers
	pop es
	pop ebx

	push eax
	
	EnterProtectedMode
	[bits 32]

	pop eax

	mov esp, ebp
	pop ebp
	ret
	
section .data

;; Extension data address packet structure
extension_dap:
	.size:    db 0x10
		      db 0 ; Unused
	.count:   dw 0
	.offset:  dw 0
	.segment: dw 0
	.lba:     dq 0

