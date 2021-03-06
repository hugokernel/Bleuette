; +-----------------------------------------------------------------------------------------------------------------------------+
; | Gestion de 24 servos avec un PIC18F452																						|
; | Copyright (C) 2012 Charles RINCHEVAL (contact@digitalspirit.org)															|
; | Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les termes de la Licence Publique G�n�rale GNU 	|
; | publi�e par la Free Software Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).				|
; | Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE GARANTIE, ni explicite ni implicite,					|
; | y compris les garanties de commercialisation ou d'adaptation dans un but sp�cifique.										|
; | Reportez-vous � la Licence Publique G�n�rale GNU pour plus de d�tails.														|
; | Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me temps que ce programme ; 						|
; | si ce n'est pas le cas,																										|
; | �crivez � la Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, �tats-Unis. 				|
; +-----------------------------------------------------------------------------------------------------------------------------+
; Les macros !

;	NOLIST	; Turn Off Listing File Output !

; Move Literal to F
MOVLF	MACRO   literal, dest
		movlw   literal
		movwf   dest
		ENDM

; Pour le DEBUG
DON		MACRO
;		bsf		LATC, 7
;		bsf		LATC, 6
		bsf		LATC, 5
		ENDM

DOFF	MACRO
;		bcf		LATC, 7
;		bcf		LATC, 6
		bcf		LATC, 5
		ENDM


; Move Literal to F Banked
MOVLFB	MACRO   literal, dest
		movlw   literal
		movwf   dest, BANKED
		ENDM

; Envoie un caract�re sur le port s�rie
SEND	MACRO   char
		movlw	char
		movwf	TXREG
		btfss	PIR1, TXIF
		goto	$ - 2
		ENDM

; Send AD conversion result
SEND_AD MACRO 	channel
		movlw 	channel
		call 	Adc_Read

		movf	Adc_Result_H, W
		call 	Serial_Send_Delay

		movf	Adc_Result_L, W
		call 	Serial_Send_Delay

		; Checksum calculation
		movf 	Adc_Result_H, W
		addwf 	Adc_Result_L, W
		call 	Serial_Send
		ENDM

INT_ON 	MACRO
		bsf 	PORTC, 0
		ENDM

INT_OFF MACRO
		bcf		PORTC, 0
		ENDM
;	LIST
