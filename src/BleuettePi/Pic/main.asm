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
; 2004-11-19 : Lorsque la consigne est 0, le port reste à 1
; 2005-12-03 : Version finale débugguée en test sur Bleuette

	title           "Gestion de 24 servos !"
	processor       18F452

	#include		"p18F452.inc"
	#include		"macro.inc"

; Program Configuration Registers
	__CONFIG    _CONFIG1H, _HSPLL_OSC_1H	; _OSCS_OFF_1H & _EC_OSC_1H & 
	__CONFIG    _CONFIG2L, _BOR_OFF_2L & _PWRT_OFF_2L
	__CONFIG    _CONFIG2H, _WDT_OFF_2H
;	__CONFIG    _CONFIG3H, _CCP2MX_OFF_3H
	__CONFIG    _CONFIG4L, _STVR_OFF_4L & _LVP_OFF_4L & _DEBUG_ON_4L
	__CONFIG    _CONFIG5L, _CP0_OFF_5L & _CP1_OFF_5L & _CP2_OFF_5L & _CP3_OFF_5L 
;	__CONFIG    _CONFIG5H, _CPB_OFF_5H & _CPD_OFF_5H
	__CONFIG    _CONFIG6L, _WRT0_OFF_6L & _WRT1_OFF_6L & _WRT2_OFF_6L & _WRT3_OFF_6L 
;	__CONFIG    _CONFIG6H, _WRTC_OFF_6H & _WRTB_OFF_6H & _WRTD_OFF_6H
	__CONFIG    _CONFIG7L, _EBTR0_OFF_7L & _EBTR1_OFF_7L & _EBTR2_OFF_7L & _EBTR3_OFF_7L
;	__CONFIG    _CONFIG7H, _EBTRB_OFF_7H


; +--------------------------+
; | Définition de constantes |
; +--------------------------+

#DEFINE		NBR_SERVO	.24

#DEFINE		PHASE_1		0x00
#DEFINE		PHASE_2		0x01
#DEFINE		PHASE_3		0x02
#DEFINE		PHASE_4		0x03

#DEFINE 	HEADER 		0xFF 	; 0xAA (0b10101010)

#DEFINE    	COMMAND_INIT 	'I'
#DEFINE		COMMAND_PAUSE   'P'
#DEFINE 	COMMAND_RESUME  'R'
#DEFINE 	COMMAND_CLEAR   'C'
#DEFINE 	COMMAND_SET     'S'
#DEFINE 	COMMAND_CURRENT 'A'
#DEFINE 	COMMAND_STATUS 	'T'

#DEFINE 	COMMAND_READAN0	'0'
#DEFINE 	COMMAND_READAN1	'1'
#DEFINE 	COMMAND_READAN2	'2'
#DEFINE 	COMMAND_READAN3	'3'
#DEFINE 	COMMAND_READAN4	'4'
#DEFINE 	COMMAND_READAN5	'5'
#DEFINE 	COMMAND_READAN6	'6'
#DEFINE 	COMMAND_READAN7	'7'

#DEFINE 	ADC_AN0 b'00000000'
#DEFINE 	ADC_AN1 b'00001000'
#DEFINE 	ADC_AN2 b'00010000'
#DEFINE 	ADC_AN3 b'00011000'
#DEFINE 	ADC_AN4 b'00100000'
#DEFINE 	ADC_AN5 b'00101000'
#DEFINE 	ADC_AN6 b'00110000'
#DEFINE 	ADC_AN7 b'00111000'

#DEFINE 	INT_STATUS_MAX_REACHED 	b'00000001'

#DEFINE 	CURRENT_CHANNEL ADC_AN7

	CBLOCK	0x00	; zone access ram de la banque 0
		servo_cons0 	: 1
		servo_cons1 	: 1
		servo_cons2 	: 1
		servo_cons3 	: 1
		servo_cons4 	: 1
		servo_cons5 	: 1
		servo_cons6 	: 1
		servo_cons7 	: 1
		servo_cons8 	: 1
		servo_cons9		: 1
		servo_cons10	: 1
		servo_cons11	: 1
		servo_cons12	: 1
		servo_cons13	: 1
		servo_cons14	: 1
		servo_cons15	: 1
		servo_cons16	: 1
		servo_cons17	: 1
		servo_cons18	: 1
		servo_cons19	: 1
		servo_cons20	: 1
		servo_cons21	: 1
		servo_cons22	: 1
		servo_cons23	: 1

		servo_sorted	: 24

		cmpt_pass		: 1		; Le compteur de passage
		current_cons	: 1		; La consigne courante
		current_servo	: 1 	; Offset du servo courant
		current_qnt		: 1		; Quantité de servo courante

		total_qnt		: 1		; Quantité totale de servos qui y sont passés
		cons_prec		: 1		; Valeur de la consigne précédente

		buf_PORTA		: 1		; Les buffers des ports
		buf_PORTB		: 1
		buf_PORTC		: 1
		buf_PORTD		: 1
		buf_PORTE		: 1

		Adc_Result_H 	: 1
		Adc_Result_L 	: 1

		Current_Last_H 	: 1
		Current_Last_L 	: 1

		Current_Max_H 	: 1
		Current_Max_L 	: 1

		Int_Status 		: 1

		Mac

		phase			: 1		; La phase courante
		phase_cmpt		: 1		; Phase de comptage


		; Variables spécifiques pour le tri
		var_min 		: 1
		var_max 		: 1

		mask 			: 1
		mask_tmp 		: 1

		buf_tab0 		: 1
		buf_tab1 		: 1
		buf_tab2 		: 1

		cmpt_bcl 		: 1

		tmp_FSR0L 		: 1
;		tmp_FSR0H 		: 1

		tmp_FSR1L 		: 1
		tmp_FSR1H 		: 1

		tmp_FSR2L 		: 1
;		tmp_FSR2H 		: 1

		cmpt 			: 1
		bcl				: 1

		; Masque de bit
		; Fini les mask de bit
        ;mask_recept0	: 1		; Octets contenant les masques de bits
		;mask_recept1	: 1
		;mask_recept2	: 1

		;pos_recept		: 1		; La position voulue
		;pos_recept1		: 1
		;pos_recept2		: 1

		received_counter : 1
        pos_0   : 1
        pos_1   : 1
        pos_2   : 1
        pos_3   : 1
        pos_4   : 1

        pos_5   : 1
        pos_6   : 1
        pos_7   : 1
        pos_8   : 1
        pos_9   : 1

        pos_10   : 1
        pos_11   : 1
        pos_12   : 1
        pos_13   : 1

		reception_control : 1
		pause_mode : 1

;		cmpt_word		: 1		; Compteur d'octets
;		cmpt_maj		: 1

		wreg_backup : 1

		csaving_IN_FSR2L	: 1		; Context saving
		csaving_IN_FSR2H	: 1

		csaving_OUT_FSR2L	: 1		; Context saving
		csaving_OUT_FSR2H	: 1
	ENDC

	CBLOCK 0x100	; Zone Bank 1
		servo_nbr00	: 1
		servo_nbr01	: 1
		servo_nbr02	: 1
		servo_nbr03	: 1
		servo_nbr04	: 1
		servo_nbr05	: 1
		servo_nbr06	: 1
		servo_nbr07	: 1
		servo_nbr08	: 1
		servo_nbr09	: 1
		servo_nbr10	: 1
		servo_nbr11	: 1
		servo_nbr12	: 1
		servo_nbr13	: 1
		servo_nbr14	: 1
		servo_nbr15	: 1
		servo_nbr16	: 1
		servo_nbr17	: 1
		servo_nbr18	: 1
		servo_nbr19	: 1
		servo_nbr20	: 1
		servo_nbr21	: 1
		servo_nbr22	: 1
		servo_nbr23	: 1

		servo_val00 : 1	; 24 * 3 zones contenants les états des ports à chaque itération
		servo_val01 : 1
		servo_val02 : 1
		servo_val03 : 1
		servo_val04 : 1
		servo_val05 : 1
		servo_val06 : 1
		servo_val07 : 1
		servo_val08 : 1
		servo_val09 : 1
		servo_val10 : 1
		servo_val11 : 1
		servo_val12 : 1
		servo_val13 : 1
		servo_val14 : 1
		servo_val15 : 1
		servo_val16 : 1
		servo_val17 : 1
		servo_val18 : 1
		servo_val19 : 1
		servo_val20 : 1
		servo_val21 : 1
		servo_val22 : 1
		servo_val23 : 1
		servo_val24 : 1
		servo_val25 : 1
		servo_val26 : 1
		servo_val27 : 1
		servo_val28 : 1
		servo_val29 : 1
		servo_val30 : 1
		servo_val31 : 1
		servo_val32 : 1
		servo_val33 : 1
		servo_val34 : 1
		servo_val35 : 1
		servo_val36 : 1
		servo_val37 : 1
		servo_val38 : 1
		servo_val39 : 1
		servo_val40 : 1
		servo_val41 : 1
		servo_val42 : 1
		servo_val43 : 1
		servo_val44 : 1
		servo_val45 : 1
		servo_val46 : 1
		servo_val47 : 1
		servo_val48 : 1
		servo_val49 : 1
		servo_val50 : 1
		servo_val51 : 1
		servo_val52 : 1
		servo_val53 : 1
		servo_val54 : 1
		servo_val55 : 1
		servo_val56 : 1
		servo_val57 : 1
		servo_val58 : 1
		servo_val59 : 1
		servo_val60 : 1
		servo_val61 : 1
		servo_val62 : 1
		servo_val63 : 1
		servo_val64 : 1
		servo_val65 : 1
		servo_val66 : 1
		servo_val67 : 1
		servo_val68 : 1
		servo_val69 : 1
		servo_val70 : 1
		servo_val71 : 1
	ENDC

	; Reset Vector
	org		0
	nop
	bra		_init


	; High Priority Vector
	org		0x08
	nop
	bra		inth

	; Low Priority Vector
	org		0x18
	nop
	bra		intl



; High Priority Vector
inth
	; Pause ?
	movlw 	0
	cpfseq 	pause_mode
	goto  	inth_end

	; "Switch" des interruptions
	btfsc   INTCON, TMR0IF
	goto	_inth_Timer

	bcf		T1CON, TMR1ON
	bsf		T0CON, TMR0ON
	btfsc   PIR1, TMR1IF
	goto	_inth_Timer

	retfie	FAST


; Low Priority Vector
intl
	btfsc   PIR1, RCIF
	goto	_intl_usart
	retfie


; Interruption venant de l'USART
_intl_usart
	; Backup WREG
	movwf 	wreg_backup

; 0: Reception de l'entete
	movlw 	0
	cpfseq 	received_counter
	goto 	_intl_usart_received_test_cmd

	; Test si le premier est bien egal a 255
	movlw 	HEADER
	cpfseq 	RCREG
	goto 	_int1_usart_clear
	goto 	_intl_usart_end

; 1: Command
_intl_usart_received_test_cmd
	movlw 	.1
	cpfseq 	received_counter
	goto 	_intl_usart_received_test_pos0

; Test for set command
_int1_usart_received_test_set
	movlw 	COMMAND_SET
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_init
	goto 	_intl_usart_end

; Test for init command
_int1_usart_received_test_init
	movlw 	COMMAND_INIT
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_pause
	; Set all value to 0
	movlw	0
	call	setValue
	bsf 	INTCON, TMR0IE
	goto 	_int1_usart_received_send_ack

; Test for pause command
_int1_usart_received_test_pause
	movlw 	COMMAND_PAUSE
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_resume

	setf 	pause_mode, 1
	call	clearAllOut
	goto 	_int1_usart_received_send_ack

; Test for resume command
_int1_usart_received_test_resume
	movlw 	COMMAND_RESUME
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_cur

	clrf 	pause_mode
	goto 	_int1_usart_received_send_ack

; Test for read current
_int1_usart_received_test_cur
	movlw 	COMMAND_CURRENT
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_stat

	movf	Current_Last_H, W
	call 	Serial_Send_Delay

	movf	Current_Last_L, W
	call 	Serial_Send_Delay

	; Checksum calculation
	movf 	Current_Last_H, W
	addwf 	Current_Last_L, W
	call 	Serial_Send

	goto 	_int1_usart_clear

; Test for get status
_int1_usart_received_test_stat
	movlw 	COMMAND_STATUS
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_ana0

	movf 	Int_Status, W
	call 	Serial_Send

	clrf 	Current_Last_H
	clrf	Current_Last_L
	bcf 	Int_Status, INT_STATUS_MAX_REACHED
	INT_OFF

	goto 	_int1_usart_clear

; Test for read analog 0 command
_int1_usart_received_test_ana0
	movlw 	COMMAND_READAN0
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_ana1

	SEND_AD ADC_AN0
	goto 	_int1_usart_clear

; Test for read analog 1 command
_int1_usart_received_test_ana1
	movlw 	COMMAND_READAN1
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_ana2

	SEND_AD ADC_AN1
	goto 	_int1_usart_clear

; Test for read analog 2 command
_int1_usart_received_test_ana2
	movlw 	COMMAND_READAN2
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_ana3

	SEND_AD ADC_AN2
	goto 	_int1_usart_clear

; Test for read analog 3 command
_int1_usart_received_test_ana3
	movlw 	COMMAND_READAN3
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_ana4

	SEND_AD ADC_AN3
	goto 	_int1_usart_clear

; Test for read analog 4 command
_int1_usart_received_test_ana4
	movlw 	COMMAND_READAN4
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_ana5

	SEND_AD ADC_AN4
	goto 	_int1_usart_clear

; Test for read analog 5 command
_int1_usart_received_test_ana5
	movlw 	COMMAND_READAN5
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_ana6

	SEND_AD ADC_AN5
	goto 	_int1_usart_clear

; Test for read analog 6 command
_int1_usart_received_test_ana6
	movlw 	COMMAND_READAN6
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_ana7

	SEND_AD ADC_AN6
	goto 	_int1_usart_clear

; Test for read analog 7 command
_int1_usart_received_test_ana7
	movlw 	COMMAND_READAN7
	cpfseq 	RCREG
	goto 	_int1_usart_received_test_clear

	bsf 	LATC, 1
	SEND_AD ADC_AN7
	bcf 	LATC, 1
	goto 	_int1_usart_clear

; Test for clear command
_int1_usart_received_test_clear
	movlw 	COMMAND_CLEAR
	cpfseq 	RCREG
	goto 	_intl_usart_unknow_command

	clrf 	pause_mode
	; Set all value to 0
	movlw	0
	call	setValue
	goto 	_int1_usart_received_send_ack

	; Si command n'est pas egal a 1, on clear et on quitte
	movlw .1
	cpfseq RCREG
	goto _int1_usart_clear
	goto _intl_usart_end

_int1_usart_received_send_ack
	SEND 'O'
	goto _int1_usart_clear

_intl_usart_unknow_command
	SEND 'N'
_int1_usart_clear
	clrf received_counter
	goto _intl_usart_exit

; 2: Position 0
_intl_usart_received_test_pos0
	movlw .2
	cpfseq received_counter
	goto _intl_usart_received_test_pos1
	movf RCREG, W
	movwf pos_0
	goto _intl_usart_end

; 3: Position 1
_intl_usart_received_test_pos1
	movlw .3
	cpfseq received_counter
	goto _intl_usart_received_test_pos2
	movf RCREG, W
	movwf pos_1
	goto _intl_usart_end

; 4: Position 2
_intl_usart_received_test_pos2
	movlw .4
	cpfseq received_counter
	goto _intl_usart_received_test_pos3
	movf RCREG, W
	movwf pos_2
	goto _intl_usart_end

; 5: Position 3
_intl_usart_received_test_pos3
	movlw .5
	cpfseq received_counter
	goto _intl_usart_received_test_pos4
	movf RCREG, W
	movwf pos_3
	goto _intl_usart_end

; 6: Position 4
_intl_usart_received_test_pos4
	movlw 6
	cpfseq received_counter
	goto _intl_usart_received_test_pos5
	movf RCREG, W
	movwf pos_4
	goto _intl_usart_end

; 7: Position 5
_intl_usart_received_test_pos5
	movlw .7
	cpfseq received_counter
	goto _intl_usart_received_test_pos6
	movf RCREG, W
	movwf pos_5
	goto _intl_usart_end

; 8: Position 6
_intl_usart_received_test_pos6
	movlw .8
	cpfseq received_counter
	goto _intl_usart_received_test_pos7
	movf RCREG, W
	movwf pos_6
	goto _intl_usart_end

; 9: Position 7
_intl_usart_received_test_pos7
	movlw .9
	cpfseq received_counter
	goto _intl_usart_received_test_pos8
	movf RCREG, W
	movwf pos_7
	goto _intl_usart_end

; 10: Position 8
_intl_usart_received_test_pos8
	movlw .10
	cpfseq received_counter
	goto _intl_usart_received_test_pos9
	movf RCREG, W
	movwf pos_8
	goto _intl_usart_end

; 11: Position 9
_intl_usart_received_test_pos9
	movlw .11
	cpfseq received_counter
	goto _intl_usart_received_test_pos10
	movf RCREG, W
	movwf pos_9
	goto _intl_usart_end

; 12: Position 10
_intl_usart_received_test_pos10
	movlw .12
	cpfseq received_counter
	goto _intl_usart_received_test_pos11
	movf RCREG, W
	movwf pos_10
	goto _intl_usart_end

; 13: Position 11
_intl_usart_received_test_pos11
	movlw .13
	cpfseq received_counter
	goto _intl_usart_received_test_pos12
	movf RCREG, W
	movwf pos_11
	goto _intl_usart_end

; 14: Position 12
_intl_usart_received_test_pos12
	movlw .14
	cpfseq received_counter
	goto _intl_usart_received_test_pos13
	movf RCREG, W
	movwf pos_12
	goto _intl_usart_end

; 15: Position 13
_intl_usart_received_test_pos13
	movlw .15
	cpfseq received_counter
	goto _intl_usart_received_test_ctrl
	movf RCREG, W
	movwf pos_13
	goto _intl_usart_end

; 17: Control byte
_intl_usart_received_test_ctrl
	movf RCREG, W
	movwf reception_control

_intl_usart_end
	incf received_counter

_intl_usart_exit
	; Restore WREG
	movlw 	wreg_backup
	bcf		PIR1, RCIF
	retfie




; Interruption du Timer
_inth_Timer

	; Aiguillage vers la bonne phase
	btfsc	phase_cmpt, .1
	goto	phase3


; ###########
; # PHASE 1 #
; ###########

	call	sort

	movlw	0x48				; Ajout hugo, le 10/02/2005
	movwf	TMR1H				; Pour être sûr que la période soit de 20ms
	movlw	0xA0
	movwf	TMR1L
	bsf		T1CON, TMR1ON		; Pour revenir dans 20ms

	call	setAllPort			; Allumage des ports !


	; Tempo de 400us, oui, c'est dommage ...
	; ... on pourrait se servir de ce temps pour autre chose ...
	; ... il faut donc refaire la routine de tri !
	movlw	0xFF
	movwf	cmpt_bcl
bcl_toto
	bra		$ + 2
	bra		$ + 2
	bra		$ + 2
	bra		$ + 2
	bra		$ + 2
	bra		$ + 2
	decfsz	cmpt_bcl
	bra		bcl_toto

	fill(bra	$ + 2), 64	; 64 -> Passage en repos à l'état bas

; ###########
; # PHASE 2 #
; ###########
	
	lfsr	FSR0, servo_sorted	; Pointeur de consignes
	lfsr	FSR1, servo_val00	; Pointeur des ports actifs
	lfsr	FSR2, servo_nbr00	; Pointeur de la quantité courante

	clrf	total_qnt
	clrf	cons_prec

phase2
	movf	INDF0, W
	movwf	current_cons		; La consigne courante

	movf	cons_prec, W
	subwf	current_cons, F		; On ne compte que le décalage par rapport au précédent

	bz		phase4				; Consigne courante à 0 ?

	movf	INDF0, W
	movwf	cons_prec

	movlw	.1
	movwf	cmpt_pass


	setf	phase_cmpt			; Phase de comptage


	nop							; Très important pour la précision !
	nop							; Ne pas toucher !!

	nop		; Ajout pour passage en mode repos niveau haut
	nop		; "
	nop		; "

	nop
	nop
	nop
	nop

; ###########
; # PHASE 3 #
; ###########
phase3
	movlw	0xFF
	movwf	TMR0H

	movlw	0xE1

	movwf	TMR0L

	nop

	incf	cmpt_pass			; Incrémente le compteur

	movf	current_cons, W
	cpfsgt	cmpt_pass			; Si cmpt est supérieur à la consigne courante, on skip
	bra		inth_end

	clrf	phase_cmpt			; Sinon, on a fini de compter


; ###########
; # PHASE 4 #
; ###########
	nop
phase4
	call 	Current_Test

	call	clrPort				; Extinction du ou des servos courants

	movf	POSTINC2, W			; Addition de la quantité courante ...
	addwf	total_qnt, F		; ... et de la quantité de servos déjà passés

	incf	FSR0L
	nop

	movlw	NBR_SERVO - 1
	cpfsgt	total_qnt			; Si le total de servo déjà passé est égal au nombre de servo, on skip !
	bra		phase2


; ###########
; # PHASE 5 #
; ###########

;	movlw	0x61				; Supprimé hugo, le 10/02/2005
;	movwf	TMR1H				; Pour être sûr que la période soit de 20ms
;	movlw	0xFF				; ... 0xFF pour 20ms pile poil !
;	movwf	TMR1L

	bcf		T0CON, TMR0ON
;	bsf		T1CON, TMR1ON		; Supprimé hugo, le 10/02/2005

; On sort de l'interruption
inth_end
	bcf		PIR1, TMR1IF
	bcf		INTCON, TMR0IF
	retfie	FAST




	#include "init.inc"
	#include "init2.inc"



	;movlw	0
	;call	setValue

	clrf 	received_counter
	clrf 	pause_mode

; Routine principale

	; Activation des interruptions
	bsf		INTCON, GIEH
	bsf		INTCON, GIEL
	;goto _intl_usart

	

main
	; On boucle temps que 18 octets ne sont pas recu
	movlw .17
	cpfseq received_counter
	goto main

	clrf received_counter

	; Control received bytes
	movlw 0
	addwf pos_0, 0
	addwf pos_1, 0
	addwf pos_2, 0
	addwf pos_3, 0
	addwf pos_4, 0
	addwf pos_5, 0
	addwf pos_6, 0
	addwf pos_7, 0
	addwf pos_8, 0
	addwf pos_9, 0
	addwf pos_10, 0
	addwf pos_11, 0
	addwf pos_12, 0
	addwf pos_13, 0

	cpfseq reception_control
	goto nack

; Tout est bon, on envoie !
ack
	; BleuettePi servo mapping
	; pos_0  RD2 -> SRV0 
	; pos_1  RD3 -> SRV1
	; pos_2  RD4 -> SRV2
	; pos_3  RD5 -> SRV3
	; pos_4  RD6 -> SRV4
	; pos_5  RD7 -> SRV5
	movff pos_0, servo_cons18
	movff pos_1, servo_cons19
	movff pos_2, servo_cons20
	movff pos_3, servo_cons21
	movff pos_4, servo_cons22
	movff pos_5, servo_cons23

	; pos_6  RB0 -> SRV6
	; pos_7  RB1 -> SRV7
	; pos_8  RB2 -> SRV8
	; pos_9  RB3 -> SRV9
	; pos_10 RB4 -> SRV10
	; pos_11 RB5 -> SRV11
	movff pos_6, servo_cons8
	movff pos_7, servo_cons9
	movff pos_8, servo_cons10
	movff pos_9, servo_cons11
	movff pos_10, servo_cons12
	movff pos_11, servo_cons13

	; pos_12 RD0 -> SRVX
	; pos_13 RD1 -> SRVY
	movff pos_12, servo_cons16
	movff pos_13, servo_cons17

	SEND 'O'
	bra	main

nack
	SEND 'N'
	goto main

	#include "functions.inc"

; Met toutes les consignes à la valeur voulues (dans WREG)
setValue
	movwf	servo_cons0
	movwf	servo_cons1
	movwf	servo_cons2
	movwf	servo_cons3
	movwf	servo_cons4
	movwf	servo_cons5
	movwf	servo_cons6
	movwf	servo_cons7
	
	movwf	servo_cons8
	movwf	servo_cons9
	movwf	servo_cons10
	movwf	servo_cons11
	movwf	servo_cons12
	movwf	servo_cons13
	movwf	servo_cons14
	movwf	servo_cons15
	
	movwf	servo_cons16
	movwf	servo_cons17
	movwf	servo_cons18
	movwf	servo_cons19
	movwf	servo_cons20
	movwf	servo_cons21
	movwf	servo_cons22
	movwf	servo_cons23
	return

clearAllOut
	movlw	b'11000000'
	andwf	LATB, 1

	clrf 	LATD
	return


; Tous les ports des servos à 1 ...
; ... sauf ceux qui ont une consigne à 0
setAllPort
; On reprend le principe suivant :
; RA0, RA1, RA2, RA3, RA4, RC0, RC1, RC2 -> Servo 0 à 7
; RB0, RB1, RB2, RB3, RB4, RB5, RE0, RE1 -> Servo 8 à 15
; RD0, RD1, RD2, RD3, RD4, RD5, RD6, RD7 -> Servo 16 à 23
; Inutile de faire une boucle pour gérer le code ci-dessous !

	clrf	buf_PORTB
	clrf	buf_PORTD

	; pos_12 RD0 -> SRVX
	; pos_13 RD1 -> SRVY

	; pos_0  RD2 -> SRV0
	; pos_1  RD3 -> SRV1
	; pos_2  RD4 -> SRV2
	; pos_3  RD5 -> SRV3
	; pos_4  RD6 -> SRV4
	; pos_5  RD7 -> SRV5
	clrf	WREG
	tstfsz	servo_cons18
	movlw	b'00000100'
	iorwf	buf_PORTD

	clrf	WREG
	tstfsz	servo_cons19
	movlw	b'00001000'
	iorwf	buf_PORTD

	clrf	WREG
	tstfsz	servo_cons20
	movlw	b'00010000'
	iorwf	buf_PORTD

	clrf	WREG
	tstfsz	servo_cons21
	movlw	b'00100000'
	iorwf	buf_PORTD

	clrf	WREG
	tstfsz	servo_cons22
	movlw	b'01000000'
	iorwf	buf_PORTD

	clrf	WREG
	tstfsz	servo_cons23
	movlw	b'10000000'
	iorwf	buf_PORTD

	clrf	WREG
	tstfsz	servo_cons16
	movlw	b'00000001'
	iorwf	buf_PORTD

	clrf	WREG
	tstfsz	servo_cons17
	movlw	b'00000010'
	iorwf	buf_PORTD

	; pos_6  RB0 -> SRV6
	; pos_7  RB1 -> SRV7
	; pos_8  RB2 -> SRV8
	; pos_9  RB3 -> SRV9
	; pos_10 RB4 -> SRV10
	; pos_11 RB5 -> SRV11
	clrf	WREG
	tstfsz	servo_cons8
	movlw	b'00000001'
	iorwf	buf_PORTB

	clrf	WREG
	tstfsz	servo_cons9
	movlw	b'00000010'
	iorwf	buf_PORTB

	clrf	WREG
	tstfsz	servo_cons10
	movlw	b'00000100'
	iorwf	buf_PORTB

	clrf	WREG
	tstfsz	servo_cons11
	movlw	b'00001000'
	iorwf	buf_PORTB

	clrf	WREG
	tstfsz	servo_cons12
	movlw	b'00010000'
	iorwf	buf_PORTB

	clrf	WREG
	tstfsz	servo_cons13
	movlw	b'00100000'
	iorwf	buf_PORTB

	movf	buf_PORTB, W
	movwf	LATB
	movf	buf_PORTD, W
	movwf	LATD

	return




; Eteint le port ou les ports spécifiés par le contenu de 3 octets,
; l'adresse du premier des 3 octets étant spécifié dans W
; Les ports utilisés sont A, B, C, D, E, le port C servant
; à recevoir les ordres pour piloter les servos
; x [0;7]	PORTA [0-4], PORTC [0-2]
; x [8;15]	PORTB [0-5], PORTE [1-2]
; x [16;23]	PORTD [0-7]
; RA0, RA1, RA2, RA3, RA4, RC0, RC1, RC2	-> Servo 0 à 7
; RB0, RB1, RB2, RB3, RB4, RB5, RE0, RE1	-> Servo 8 à 15 
; RD0, RD1, RD2, RD3, RD4, RD5, RD6, RD7	-> Servo 16 à 23
clrPort
	; Si la consigne courante est 0
	; Le ou les servo(s) restent comme il sont, à 1
	movf	current_cons, W
	bz		_clrPort

	; Le premier octet concerne
	; les port A et C
	movf	INDF1, W
	nop;
	nop;
	nop;
;	andlw	b'000111111'
;	xorwf	LATA, W		; Pour A
;	movwf	buf_PORTA

	movf	INDF1, W
	nop;
	nop;
	nop;
	nop;
	nop;
	nop;
;	rlncf	WREG
;	rlncf	WREG
;	rlncf	WREG
;	andlw	b'00000111'
;	xorwf	LATC, W		; Pour C
;	movwf	buf_PORTC

	; Le deuxième octet concerne
	; les port B et E
	movf	PREINC1, W
	andlw	b'00111111'
	xorwf	LATB, W		; Pour B
	movwf	buf_PORTB

	movf	POSTINC1, W
	nop;
	nop;
	nop;
	nop;
	nop;
;	rlncf	WREG
;	rlncf	WREG
;	andlw	b'0000011'
;	xorwf	LATE, W		; Pour E
;	movwf	buf_PORTE

	; Le dernier octet concerne le port D
	movf	POSTINC1, W
	xorwf	LATD, W		; Pour D
	movwf	buf_PORTD

	; Maintenant, on place tout le contenu des buffers ...
	; ... dans les ports qui vont bien, ce qui garanti une synchro idéale
;	movf	buf_PORTA, W
;	movwf	LATA
;	movf	buf_PORTC, W
;	movwf	LATC
	nop;
	nop;
	nop;
	nop;

	movf	buf_PORTB, W
	movwf	LATB
	nop;
	nop;
;	movf	buf_PORTE, W
;	movwf	LATE
	movf	buf_PORTD, W
	movwf	LATD

	return

; Rattrapage du temps en cas de consigne nul
; Ne surtout pas modifier
_clrPort

	incf	FSR1L
	incf	FSR1L
	incf	FSR1L
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop

	return



; Tri !
; TODO: refaire tout le code de tri ci dessous !
; il doit y avoir moyen d'économiser beaucoup de temps CPU...
sort

	lfsr	FSR0, servo_sorted	; Pointeur des servos triés
	lfsr	FSR1, servo_val00	; Pointeur des ports actifs
	lfsr	FSR2, servo_nbr00	; Pointeur de la quantité courante

	movff	FSR0L, tmp_FSR0L
	movff	FSR1H, tmp_FSR1H
	movff	FSR1L, tmp_FSR1L
	movff	FSR2L, tmp_FSR2L

	clrf	var_max

	movlw	NBR_SERVO
	movwf	cmpt_bcl

sort_

	; Initialisation des variables, pointeurs...
	lfsr	FSR0, servo_cons0	; Pointeur de consignes
	lfsr	FSR1, buf_tab0		; Buffer de données

	setf	var_min

	clrf	buf_tab0
	clrf	buf_tab1
	clrf	buf_tab2

	movlw	.1
	movwf	cmpt
	movwf	mask_tmp

	clrf	bcl

; C'est parti !
_sort
	incf	bcl
	movlw	NBR_SERVO + 1
	cpfslt	bcl					; Si toutes les consignes y sont passées...
	bra		_sort_quit			; ... on quitte !

	movlw	NBR_SERVO
	subwf	cmpt_bcl, W
	bz		_sort_last

	movf	INDF0, W			; Met la consigne courante dans W ...
	cpfslt	var_max				; ... et test si elle est supérieur à ...
	bra		_sort_paf			; ... var_max, on poursuit, sinon, on retourne au début !

_sort_last
	movf	INDF0, W			; Met la consigne courante dans W ...
	cpfseq	var_min				; Si la consigne courante est égale à var_min ...
	bra		_sort_next

	movf	mask_tmp, W			; ... on va là !
	iorwf	INDF1

	incf	cmpt
	bra		_sort_paf

_sort_next
	cpfslt	var_min				; Si la consigne courante est inférieur à var_min ...
	bra		_sort_inf

	bra		_sort_paf

_sort_inf
	movf	INDF0, W			; ... on file ici !
	movwf	var_min

	clrf	buf_tab0
	clrf	buf_tab1
	clrf	buf_tab2
	movlw	.1
	movwf	cmpt

	movf	mask_tmp, W
	iorwf	INDF1

_sort_paf
	bcf		STATUS, C
	rlcf	mask_tmp
	btfss	STATUS, C
	bra		_sort_pif

	rlcf	mask_tmp
	incf	FSR1L

_sort_pif
	incf	FSR0L
	bra		_sort


_sort_quit

	movf	var_min, W
	movwf	var_max

	movff	tmp_FSR0L, FSR0L	; servo_sorted	->	Pointeur de consignes
	movff	tmp_FSR1H, FSR1H	; servo_val00	->	Pointeur des ports actifs
	movff	tmp_FSR1L, FSR1L
	movff	tmp_FSR2L, FSR2L	; servo_nbr00	->	Pointeur de la quantité courante

	movff	var_min, POSTINC0

	movff	buf_tab0, POSTINC1
	movff	buf_tab1, POSTINC1
	movff	buf_tab2, POSTINC1

	movff	cmpt, POSTINC2

	movff	FSR0L, tmp_FSR0L	; Sauvegarde des pointeurs
	movff	FSR1H, tmp_FSR1H
	movff	FSR1L, tmp_FSR1L
	movff	FSR2L, tmp_FSR2L

	decfsz	cmpt_bcl
	bra		sort_

	return



	END
