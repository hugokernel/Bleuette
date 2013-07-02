; +-----------------------------------------------------------------------------------------------------------------------------+
; | Gestion de 24 servos avec un PIC18F452																						|
; | Copyright (C) 2012 Charles RINCHEVAL (contact@digitalspirit.org)															|
; | Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les termes de la Licence Publique Générale GNU 	|
; | publiée par la Free Software Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).				|
; | Ce programme est distribué car potentiellement utile, mais SANS AUCUNE GARANTIE, ni explicite ni implicite,					|
; | y compris les garanties de commercialisation ou d'adaptation dans un but spécifique.										|
; | Reportez-vous à la Licence Publique Générale GNU pour plus de détails.														|
; | Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même temps que ce programme ; 						|
; | si ce n'est pas le cas,																										|
; | écrivez à la Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, États-Unis. 				|
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

; Envoie un caractère sur le port série
SEND	MACRO   char
		movlw	char
		movwf	TXREG
		btfss	PIR1, TXIF
		goto	$ - 2
		ENDM

;	LIST
