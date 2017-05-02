

; Set OptionByte


OPT CSEG AT 0080H
OPTION:   DB 00H	; Watchdog timer disabled, 25% window, 3.88ms, can be stopped by software
POC:      DB 00H	; 1.59V POC mode
RESERVE1: DB 00H	; Reserved area - must be written with 0
RESERVE2: DB 00H	; Reserved area - must be written with 0
SECURITY: DB 03H	; OCD enabled, Erase flash memory if incorrect OCD security ID

ONC_SBR CSEG AT 007EH
ONCSBR:	DB	0FFH, 0FFH		; 7E,7F are reserved for OCD

END
