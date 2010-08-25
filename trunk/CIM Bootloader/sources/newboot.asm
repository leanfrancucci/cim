;
;	boot.asm
;
;
			include '9S08GB60.inc'
;
			xdef	main
            xref 	__SEG_END_SSTACK 
;
;
;
;
;
;	####### Equates section #######
;
;
MCU_ID					equ		6
SPI_PRESCAL_X4			equ		$30
SPI_PRESCAL_X1			equ		$00
SPI_CS_PM				equ		2
SPI_CS_M				equ		6
mWDOG_CTRL				equ		$04
;
DEBOUNCE_TIME			equ		$0100	
WSUPER					equ		10
DUMMY					equ		0
WR_DATA					equ	    'W'
ERASE	  				equ	    'E'
QUIT					equ	    'Q'
FLASH_PROTECTION		equ	    %10001000			; FPOPEN and flash protected (from 0xFC00)

;	
;	Warning:
;
;	"The on-chip debug module cannot be enabled while MCU 
;	is secure (SEC0[1:0] register FOPT). The separate BDM controller
;	can still be used for background memory access commands, but the
;	MCU cannot enter active background mode except by holding BCGD/MS
;	low at the rising edge of reset."
;
;	Therefore, set FLASH_OPTION = $02 for using BDM.
;
;
;FLASH_OPTION			equ	    %00000000			; backdoor enable, redirection enable and
FLASH_OPTION			equ	    %00000010			; backdoor enable, redirection enable and
													; (un)secured flash [last 10]
MEM0					equ		$0B
MEM1					equ		$0D
DENSITY_MASK			equ		$0F
MM_PAGE_READ			equ		$52
STATUS_READ				equ		$57
FILE_SIZE				equ		62317	
BIN_PAGE_SIZE			equ		512
START_FILE_VERSION		equ		9					; 
VERSION_STRING_SIZE		equ		10					; "SWvv.vv.bb"
ERBLK_LEN				equ		512					; erase block lenght
WRBLK_LEN				equ		64					; write block lenght
START_USR_CODE_SECT		equ		$182C				; FLASH start
START_REPRO_AREA		equ		START_USR_CODE_SECT
END_USR_CODE_SECT		equ		$FBBE				; FLASH end
START_CHK_CODE			equ		END_USR_CODE_SECT
RELOC_VECT_TBL_ADDR		equ 	$FBC0				; relocated interrupt vector table
START_BOOT_CODE			equ		$FC00				; boot code start
VECT_TBL_ADDR			equ		$FFC0				; interrupt vector table start
END_VECT_TBL_ADDR		equ		$FFFF				; interrupt vector table end
END_REPRO_AREA			equ		END_VECT_TBL_ADDR
RESET_VECT				equ		END_VECT_TBL_ADDR-1	; reset vector address
USR_RESET_VECT			equ		START_BOOT_CODE-2 
PAGE_OFFSET				equ		216
;
; 
;
;	####### Macro section #######
;
;
;	Initialize according to the MCU and
;	hardware dependent of board.
;
init        MACRO
;
;			General settings
;
			lda	  	#%00000010
	    	sta		SOPT				; cop disable, bdm enable (for now)
;
;			ICG settings
;
			mov		#$78,ICGC1
			mov		#$00,ICGC2
;
\@1:		brclr   LOCK,ICGS1,\@1		; wait until ICG stable
;
;			FLASH settings
;
			lda		#$30
			sta		FSTAT
			lda		#$4A
			sta		FCDIV
;
;			SPI settings
;
			bset	MSTR,SPIC1
			bset	CPOL,SPIC1
			bset	CPHA,SPIC1
			bclr	SSOE,SPIC1
			bclr	LSBFE,SPIC1
;
			bclr	MODFEN,SPIC2
;
			bset	SPE,SPIC1
;
;
;			I/O port settings
;
			jsr		init_ioports
;			
;
			ENDM
;
;
;
; 	Read a data from spi channel.
;
spi_read:	MACRO
;
			lda		#DUMMY
			jsr		spi_xfer
;
			ENDM
;
;
;
spi_write:	MACRO
;
			jsr		spi_xfer
;
			ENDM
;
;
;
;	Force a reset for ilegal operation
;
ilop        MACRO
;
            dc.b    $8d             	; this is illegal operation code
;
            ENDM
;
;
;
;	Shift left all bits of passed argument
;
sl:		MACRO
;
			lsl		\1 + 2
			rol		\1 + 1
			rol		\1
;
		ENDM
;	
;	
;	
;	####### RAM section for general variables #######
;
;
MY_ZEROPAGE:	SECTION		SHORT
;
ram_ix:		ds.w	1
file_ix:	ds.w	1
num_page:	ds.w	1
page_addr:	ds.b	3
faddress:	ds.w	1
raddress:	ds.w	1
stack:		ds.w	1
data_count:	ds.b	1
len:		ds.b	1
high_check:	ds.b	1
file_check:	ds.w	1
check:		ds.w	1
mem_status:	ds.b	1
mem_cmd:	ds.b	1
;
;
;
;
;	####### RAM section with direct addressing mode #######
;
;
DEFAULT_RAM:    SECTION
;
ram_buff:	ds.b	BIN_PAGE_SIZE
ram_data:	ds.b	WRBLK_LEN
;
;
;
;	####### ROM section data segments #######
;
;
			xdef 		NVPROT	
			xdef		NVOPT 	
NVPROT_ROM:		SECTION
;
NVPROT		dc.b	FLASH_PROTECTION
;
NVOPT_ROM:		SECTION
;
NVOPT	  	dc.b	FLASH_OPTION
;
;
;
;	####### ROM section for code segments #######
;
;
;
DEFAULT_ROM:	SECTION
;
;
;
;
;
;	####### HERE is the beginning from any reset source #######
;
;
main:
;
; 			Initialize the Stack.
;
            ldhx   #__SEG_END_SSTACK 		; initialize the sp
			txs
;
;
;			Bootloader entry log
;
			lda     SRS						; fetch reset status reg.
			tsta							; check if zero (this happens if reset
											; pulse is too short)
			beq     selfprg					; if so, jump to self programming
			and     #(mPOR|mPIN|mCOP|mLVD)	; mask POR, PIN, LVD and COP reset source
			bne     selfprg					; any of these sources, go to self programming
;
			jsr		check_usr_code
			bne		selfprg	
;
start_app:									; jump to application code
			ldhx	#USR_RESET_VECT
			lda		,x
			psha
			ldx		1,x
			pulh
			jmp		,x
;
;
;
selfprg:
;
			init
;
			ldhx	#WSUPER
			jsr		delay					; if reset source is POR or LVD then reset from
											; supervisory reset.
;
			jsr		query_memory 			; store into 'mem_status' the result of this operation
;
			jsr		check_usr_code
			beq		sf__0					; is user code right?
;
			brset	0,mem_status,fail		; is installed memory?
;
			jsr		check_file				; is file right?
			bne		fail
			jmp		download_file			; load image into MUC flash from dataflash
;
sf__0:		brset	0,mem_status,reboot		; is installed memory?
;
			jsr		check_file				; is file right?
			bne		reboot
			jsr		compare_versions		; update?
			beq		reboot
			jmp		download_file			; load image into MUC flash from dataflash
;
;
fail:		
f__0:		jsr		trigger_wdg
			bra		f__0
;
;
reboot:
usr_code:
			ilop
;
;
;	
; init_page:
;
init_page:
;
			pshh
			pshx
			psha
;
			ldhx	#FILE_SIZE
			sthx	file_ix
;
			ldhx	#PAGE_OFFSET
			sthx	num_page
;
			ldhx	#ram_buff
			sthx	ram_ix
;
			pula
			pulx
			pulh
;
			rts	
;
;
;
; spi_xfer:
;
;	Transfer a byte through SPI.
;
spi_xfer:
;
sx__1:		brclr 	SPTEF,SPIS,sx__1
			sta		SPID
sx__2:		brclr 	SPRF,SPIS,sx__2
			lda		SPID
			rts
;
;
;
; xfer_page_ram:
;
;
xfer_page_ram:
;
			pshh
			pshx
;
			ldhx	#ram_buff
;
xf__1:		cphx	#ram_buff + BIN_PAGE_SIZE
			beq		xf__2
			spi_read
			sta		,x
			aix		#1
			bra		xf__1
;
xf__2:
			pulx
			pulh
;
			rts
;
;
;
; read_page:
;
;
read_page:
;
			pshh
			pshx
			psha
;
			jsr		trigger_wdg
;
;	First, prepare command MM_PAGE_READ
;
			mov		num_page + 1,page_addr + 2			; load 'page_addr' with 'num_page'
			mov		num_page,page_addr + 1
			clr		page_addr
;
			lda		#10
			clc
rp__1:		sl		page_addr
			dbnza	rp__1
;
;	Second, transfer command to dataflash memory
;
			jsr		spi_select	
			mov		#MM_PAGE_READ,mem_cmd
			jsr		send_cmd							; send cmd + 4 reserved bits + 20 address bits 
			jsr		send_cmd							; and 32 don't care bits.
;
;	Third, read data from page and transfer it
;	to ram buffer.
;
			jsr		xfer_page_ram
			jsr		spi_deselect
;
			pula
			pulx
			pulh
;
			rts
;
;
;
; fgetc:
;
;
fgetc:
;
			pshh								; save H:X registers
			pshx
;
			ldhx	file_ix						; begin of file?
			cphx	#FILE_SIZE
			bne		fg__3						; is not begin of file
			jsr		read_page					; begin of file then read first page
			
;
fg__3:		ldhx	ram_ix						; get current index of ram buffer
			lda		,x							; and store content into 'A'
			psha								; to be used later.
			aix		#1
			sthx	ram_ix						; update ram buffer index
;
			ldhx	file_ix
			aix		#-1
			sthx	file_ix						; update file index
;
			bne		fg__1						; end of file?
			jsr		init_page					; reachs end of file then file wrap-around
			bra		fg__2						; and continue
;
fg__1:		ldhx	ram_ix						; prepare for next file read
			cphx	#ram_buff + BIN_PAGE_SIZE
			bne		fg__2						; end of current page?
;
			ldhx	num_page					; reachs end of current page
			aix		#1
			sthx	num_page					; take next page and update 'num_page'
;
			jsr		read_page					; and read it
;
			ldhx	#ram_buff					; initialize index of ram buffer
			sthx	ram_ix
;
fg__2:
			pula								; 'A' contains the read byte from ram buffer
			pulx								; restore H:X registers
			pulh
;
			rts
;
;
;
; compare_versions:
;
;
compare_versions:
;
			ldhx	#PAGE_OFFSET
			sthx	num_page
;
			jsr		read_page
;
			ldhx	#0
;
cv__1:		cphx	#VERSION_STRING_SIZE			
			beq		cv__2
			lda		START_USR_CODE_SECT,x
			cmp		ram_buff + START_FILE_VERSION,x
			bne		cv__3
			aix		#1
			bra		cv__1
cv__3:
			lda		#1
			bra		cv__4
cv__2:
			clra
cv__4:
			rts
;
;
; download_file:
;
;
download_file:
;
			jsr		init_page
;
			jsr		fgetc				; the first two bytes are file checksum 
			jsr		fgetc				; therefore are dropped
;
dw__0:		jsr		fgetc				; parse file
;
			cbeqa	#WR_DATA,wr_data
			cbeqa	#ERASE,erase 
			cbeqa	#QUIT, quit
;
			bra		dw__0	
;
;
quit:
			ilop
;
;
erase:
;
;			read erase block address MSB
;
			jsr		fgetc
    	  	sta	   	faddress 
;
;			read erase block address LSB 
;
			jsr		fgetc
	      	sta	    faddress + 1
;
    		lda   	#(mFPVIOL+mFACCERR) 		; mask
    		sta   	FSTAT 						; abort any command and clear errors
;
    		mov		#EraseSubSize,data_count 	; length of flash erase routine to 'data_count'
    		tsx
    		sthx  	stack
    		ldhx  	#EraseSubEnd-1 				; point at last byte to move to stack
;
    		bra   	DoOnStack 					; execute prog code from stack ram
;
wr_data:
;
;			read flash address MSB
;
	      	jsr     fgetc
    	  	sta	    faddress
;
;			read flash address LSB
;
	      	jsr     fgetc
	      	sta	    faddress + 1
;
;			read length of data block to program
;
	      	jsr     fgetc
	        sta     len
      		sta	   	data_count 
;
    	    ldhx    #ram_data
        	sthx  	raddress
;
wd__0:
;
;			read data and store onto buffer
;
	      	jsr     fgetc
    	    sta     ,x
        	aix     #1
	        dbnz    data_count,wd__0
;
    		lda   	#(mFPVIOL+mFACCERR) 	; mask
    		sta   	FSTAT 					; abort any command and clear errors
;
    		mov		#ProgSubSize,data_count	; length of flash prog routine to 'data_count'
    		tsx
    		sthx  	stack
    		ldhx 	#ProgSubEnd - 1			; point at last byte to move to stack
;
; 			fallthru to this routine
;
DoOnStack: 
    		lda   	,x 						; read from flash
    		psha 							; move onto stack
    		aix 	#-1 					; next byte to move
    		dbnz  	data_count,DoOnStack
;
    		tsx 							; point to sub on stack
    		jmp 	,x 						; execute the sub on the stack
											; (will return on it's own)
;
;
EraseSub: 	
    		ldhx  	faddress	 			; get flash address
    		sta   	0,x 					; write to flash; latch addr and data
    		lda   	#mPageErase 			; get flash command
    		sta   	FCMD 					; write the flash command
    		lda   	#mFCBEF 				; mask to initiate command
    		sta   	FSTAT 					; [pwpp] register command
    		nop 							; [p] want min 4~ from w cycle to r
ChkDoneErase: 
    		lda   	FSTAT 					; [prpp] so FCCF is valid
    		lsla 							; FCCF now in MSB
    		bpl   	ChkDoneErase 			; loop if FCCF = 0
;
    		ldhx    stack
    		txs
	        jmp     dw__0		    		;refer status back to PC
;
EraseSubEnd: 
EraseSubSize: equ (*-EraseSub)
;
;
ProgSub: 	
    		lda   	FSTAT 					; check FCBEF
    		and   	#mFCBEF 				; mask it
    		beq	  	ProgSub					; loop if not empty
;
    		ldhx  	raddress				; get address of ram data
    		lda	  	0,x
    		aix	  	#1
    		sthx  	raddress
;   		
    		ldhx  	faddress	 			; get flash address
    		sta   	0,x 					; write to flash; latch address and data
    		aix	  	#1
    		sthx  	faddress
;
    		lda   	#mBurstProg 			; get flash command
    		sta   	FCMD 					; write the flash command
    		lda   	#mFCBEF 				; mask to initiate command
    		sta   	FSTAT 					; [pwpp] register command
    		dbnz  	len,ProgSub				; all bytes in a row?
;
ChkDoneProg: 
    		lda   	FSTAT 					; [prpp] so FCCF is valid
    		lsla 							; FCCF now in MSB
    		bpl   	ChkDoneProg 			; loop if FCCF = 0
;
    		ldhx	stack					; deallocate sub body
    		txs
	        jmp     dw__0	    			; refer status back to PC
;
ProgSubEnd: 
ProgSubSize: equ (*-ProgSub)
;
;
;
; check_usr_code:
;
;	If detected any error then return 'A'<-1 else
;	return 'A'<-0.
;
check_usr_code:
;
			ldhx	#USR_RESET_VECT			; verify if user reset vector is not empty
			lda		,x
			psha
			and		1,x
			ldx		1,x
			pulh
			inca
			beq		cc__3	
;
			jsr		trigger_wdg
;
;			calculate checksum
;
calc_check:
;
			clra
			clr		high_check
;
			ldhx	#START_USR_CODE_SECT
;
cc__1:		sta		SRS	
			add		,x
			bcc		cc__2
			inc		high_check
cc__2:		aix		#1
			cphx	#END_USR_CODE_SECT
			bne		cc__1
;
			jsr		trigger_wdg
;
			coma								; complement calculated sum
			ldx		high_check
			comx
;
			cpx		START_CHK_CODE				; and compare it with stored sum
			bne		cc__3	
			cmpa	START_CHK_CODE + 1
			bne		cc__3	
			bra		cc__4
;
cc__3		lda		#1
			bra		cc__5
cc__4		clra
cc__5:
			rts
;
;
;
; check_file:
;	
;	Verifies the file stored in dataflash 
;	memory (M1). If detected any errors the return 'A'<- 1,
;	else 'A'<- 0.
;
check_file:
;
			jsr 	init_page
;
			jsr		fgetc				; get file checksum
			sta		file_check			; and store it in 'file_check'
			jsr		fgetc
			sta		file_check + 1	
;
			clr		check				; clear high byte of sum
			clr		check + 1			; clear low byte of sum
;
			ldhx	#FILE_SIZE - 2
;
cf__1:									; perform 16-bit checksum of file
			jsr		fgetc
			add		check + 1	
			sta		check + 1
			bcc		cf__2
			inc		check
cf__2:		aix		#-1
			cphx	#0
			bne		cf__1
;										; compare calculated file checksum with user code
			lda		check + 1			; checksum stored into MCU flash.
			coma
			ldx		check
			comx
;
			cpx		file_check	
			bne		cf__3	
			cmpa	file_check + 1
			bne		cf__3
			bra		cf__4
;
cf__3		lda		#1
			bra		cf__5
cf__4		clra
cf__5:
			rts
;
;
;
init_ioports:
;
			mov		#$00,PTAD
			mov		#$00,PTADD
			mov		#$CF,PTAPE
;
			mov		#$00,PTBD
			mov		#$00,PTBDD
			mov		#$00,PTBPE
;
			mov		#$01,PTCD
			mov		#$3D,PTCDD
			mov		#$82,PTCPE
;
			mov		#$00,PTDD
			mov		#$1F,PTDDD
			mov		#$00,PTDPE
;
			mov		#$75,PTED
			mov		#$F5,PTEDD
			mov		#$00,PTEPE
;
			mov		#$80,PTFD
			mov		#$FF,PTFDD
			mov		#$00,PTFPE
;
			mov		#$00,PTGD
			mov		#$08,PTGDD
			mov		#$F0,PTGPE
;
			rts
;
;
;
spi_select:
;
			brclr	MCU_ID,PTCD,ss__1
;
			mov		#SPI_PRESCAL_X1,SPIBR
			bset	SPI_CS_PM,PTED
			bclr	SPI_CS_M,PTED
			bra		ss__0
ss__1:
			mov		#SPI_PRESCAL_X4,SPIBR
			bset	SPI_CS_M,PTED
			bclr	SPI_CS_PM,PTED
;
ss__0:		rts
;
;
;
spi_deselect:
;
			brclr	MCU_ID,PTCD,sd__1
			bset	SPI_CS_M,PTED
			bra		sd__0
;
sd__1:		bset	SPI_CS_PM,PTED
;
sd__0:		rts
;
;
;
trigger_wdg:
;
			psha
;
			lda		PTDD	
			eor		#mWDOG_CTRL
			sta		PTDD
;
			pula
;
			rts
;
;
;
query_memory:
;
			jsr		spi_select
			lda		#STATUS_READ	
			spi_write
			spi_read
;
			lsra
			lsra
			and		DENSITY_MASK
;
			cbeqa	#MEM0,qm__0
			cbeqa	#MEM1,qm__0
;
			mov		#1,mem_status
			bra		qm__1
qm__0:		mov		#0,mem_status
;
qm__1:		jsr		spi_deselect
;
			rts
;
;
;
delay:
;
			clra
d__0:		nop
			dbnza	d__0
			aix		#-1
			cphx	#0
			bne		d__0
;
			rts
;
;
;
send_cmd:
;
;	Send command
;
			lda		mem_cmd	
			spi_write
;
;	Send high byte of page_addr
;
			lda		page_addr
			spi_write
;
;	Send middle byte of page_addr
;
			lda		page_addr + 1
			spi_write
;
;	Send low byte of page_addr
;
			lda		page_addr + 2
			spi_write
;
			rts
;
;
;
;
;
;
end
