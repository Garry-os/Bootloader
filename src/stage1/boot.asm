org 0x7C00
bits 16

jmp start
nop

;; FAT BPB
;; ============ BPB bytes ===========
;;
times 138-($-$$) db 0

start:
	;; Resetting cs by performing a jump
	cli
	cld
	jmp 0x000:.reset_cs
.reset_cs:
	;; Reset segments & setup stack
	xor ax, ax
	mov es, ax
	mov ds, ax

	mov ss, ax
	mov sp, 0x7C00 ; Stack grows downward
	
	sti
	
	;; Check disk extension
	mov ah, 0x41
	mov bx, 0x55AA
	stc
	int 0x13

	jc .no_disk_extension

	;; Verify
	cmp bx, 0xAA55
	jne .no_disk_extension
	
	jmp .after
.no_disk_extension:
	;; TODO: Print a message
	jmp .halt
.after:
	; A test
	mov al, 'h'
	mov ah, 0x0E
	int 0x10

.halt:
	cli
	hlt

times 510 - ($ - $$) db 0
dw 0xAA55 ;; BIOS magic


