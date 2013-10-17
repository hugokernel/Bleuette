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
; Initialisation du bouzin !

_init

	; Configuration des ports
	MOVLF   b'00011111', TRISA
	MOVLF   b'00000000', TRISB
	MOVLF   b'10000000', TRISC
	MOVLF   b'00000000', TRISD
	MOVLF   b'00000111', TRISE

	; Initialisation des ports
	MOVLF   b'00000000', PORTA
	MOVLF   b'00000000', PORTB
	MOVLF   b'00000000', PORTC
	MOVLF   b'00000000', PORTD
	MOVLF   b'00000000', PORTE

	; RCON REGISTER
	; +------+---+---+----+----+----+-----+-----+
	; | IPEN | — | — | RI | TO | PD | POR | BOR |
	; +------+---+---+----+----+----+-----+-----+
	; bit 7
	; 	IPEN: Interrupt Priority Enable bit
	; 	1 = Enable priority levels on interrupts
	; 	0 = Disable prioyarity levels on interrupts (16CXXX Compatibility mode)
	; bit 6-5 Unimplemented: Read as '0'
	; bit 4 RI: RESET Instruction Flag bit
	; 	1 = The RESET instruction was not executed
	; 	0 = The RESET instruction was executed causing a device RESET
	; 	(must be set in software after a Brown-out Reset occurs)
	; bit 3 TO: Watchdog Time-out Flag bit
	; 	1 = After power-up, CLRWDT instruction, or SLEEP instruction
	; 	0 = A WDT time-out occurred
	; bit 2 PD: Power-down Detection Flag bit
	; 	1 = After power-up or by the CLRWDT instruction
	; 	0 = By execution of the SLEEP instruction
	; bit 1 POR: Power-on Reset Status bit
	; 	1 = A Power-on Reset has not occurred
	; 	0 = A Power-on Reset occurred
	; 	(must be set in software after a Power-on Reset occurs)
	; bit 0 BOR: Brown-out Reset Status bit
	; 	1 = A Brown-out Reset has not occurred
	; 	0 = A Brown-out Reset occurred
	; 	(must be set in software after a Brown-out Reset occurs)
	MOVLF	b'10000000', RCON

	; INTCON Register
	; +----------+-----------+--------+--------+------+--------+--------+------+
	; | GIE/GIEH | PEIE/GIEL | TMR0IE | INT0IE | RBIE | TMR0IF | INT0IF | RBIF |
	; +----------+-----------+--------+--------+------+--------+--------+------+
	; bit 7 GIE/GIEH: Global Interrupt Enable bit
	; 	When IPEN = 0:
	; 		1 = Enables all unmasked interrupts
	; 		0 = Disables all interrupts
	; 	When IPEN = 1:
	; 		1 = Enables all high priority interrupts
	; 		0 = Disables all interrupts
	; bit 6 PEIE/GIEL: Peripheral Interrupt Enable bit
	; 	When IPEN = 0:
	; 		1 = Enables all unmasked peripheral interrupts
	; 		0 = Disables all peripheral interrupts
	; 	When IPEN = 1:
	; 		1 = Enables all low priority peripheral interrupts
	; 		0 = Disables all low priority peripheral interrupts
	; bit 5 TMR0IE: TMR0 Overflow Interrupt Enable bit
	; 	1 = Enables the TMR0 overflow interrupt
	; 	0 = Disables the TMR0 overflow interrupt
	; bit 4 INT0IE: INT0 External Interrupt Enable bit
	; 	1 = Enables the INT0 external interrupt
	; 	0 = Disables the INT0 external interrupt
	; bit 3 RBIE: RB Port Change Interrupt Enable bit
	; 	1 = Enables the RB port change interrupt
	; 	0 = Disables the RB port change interrupt
	; bit 2 TMR0IF: TMR0 Overflow Interrupt Flag bit
	; 	1 = TMR0 register has overflowed (must be cleared in software)
	; 	0 = TMR0 register did not overflow
	; bit 1 INT0IF: INT0 External Interrupt Flag bit
	; 	1 = The INT0 external interrupt occurred (must be cleared in software)
	; 	0 = The INT0 external interrupt did not occur
	; bit 0 RBIF: RB Port Change Interrupt Flag bit
	; 	1 = At least one of the RB7:RB4 pins changed state (must be cleared in software)
	; 	0 = None of the RB7:RB4 pins have changed state
	MOVLF	b'00100100', INTCON
	;bsf		INTCON, TMR0IF

	; INTCON2 Register
	; +------+---------+---------+---------+---+--------+---+------+
	; | RBPU | INTEDG0 | INTEDG1 | INTEDG2 | — | TMR0IP | — | RBIP |
	; +------+---------+---------+---------+---+--------+---+------+
	; bit 7 RBPU: PORTB Pull-up Enable bit
	; 	1 = All PORTB pull-ups are disabled
	; 	0 = PORTB pull-ups are enabled by individual port latch values
	; bit 6 INTEDG0:External Interrupt0 Edge Select bit
	; 	1 = Interrupt on rising edge
	; 	0 = Interrupt on falling edge
	; bit 5 INTEDG1: External Interrupt1 Edge Select bit
	; 	1 = Interrupt on rising edge
	; 	0 = Interrupt on falling edge
	; bit 4 INTEDG2: External Interrupt2 Edge Select bit
	; 	1 = Interrupt on rising edge
	; 	0 = Interrupt on falling edge
	; bit 3 Unimplemented: Read as '1'
	; bit 2 TMR0IP: TMR0 Overflow Interrupt Priority bit
	; 	1 = TMR0 Overflow Interrupt is a high priority event
	; 	0 = TMR0 Overflow Interrupt is a low priority event
	; bit 1 Unimplemented: Read as '1'
	; bit 0 RBIP: RB Port Change Interrupt Priority bit
	; 	1 = RB Port Change Interrupt is a high priority event
	; 	0 = RB Port Change Interrupt is a low priority event
	MOVLF	b'10001110', INTCON2


	; T0CON: TIMER0 CONTROL REGISTER
	; +--------+--------+------+------+-----+-------+-------+-------+
	; | TMR0ON | T08BIT | T0CS | T0SE | PSA | T0PS2 | T0PS1 | T0PS0 |
	; +--------+--------+------+------+-----+-------+-------+-------+
	; bit 7 TMR0ON: Timer0 On/Off Control bit
	; 	1 = Enables Timer0
	; 	0 = Stops Timer0
	; bit 6 T08BIT: Timer0 8-bit/16-bit Control bit
	; 	1 = Timer0 is configured as an 8-bit timer/counter
	; 	0 = Timer0 is configured as a 16-bit timer/counter
	; bit 5 T0CS: Timer0 Clock Source Select bit
	; 	1 = Transition on T0CKI pin
	; 	0 = Internal instruction cycle clock (CLKO)
	; bit 4 T0SE: Timer0 Source Edge Select bit
	; 	1 = Increment on high-to-low transition on T0CKI pin
	; 	0 = Increment on low-to-high transition on T0CKI pin
	; bit 3 PSA: Timer0 Prescaler Assignment bit
	; 	1 = TImer0 prescaler is NOT assigned. Timer0 clock input bypasses prescaler.
	; 	0 = Timer0 prescaler is assigned. Timer0 clock input comes from prescaler output.
	; bit 2-0 T0PS2:T0PS0: Timer0 Prescaler Select bits
	; 	111 = 1:256 prescale value
	; 	110 = 1:128 prescale value
	; 	101 = 1:64 prescale value 
	; 	100 = 1:32 prescale value
	; 	011 = 1:16 prescale value
	; 	010 = 1:8 prescale value
	; 	001 = 1:4 prescale value
	; 	000 = 1:2 prescale value
;	MOVLF	b'10010010', T0CON
;#DEFINE	T0CON_VALUE	b'10010000'
;	MOVLF	T0CON_VALUE, T0CON
	MOVLF	b'10010000', T0CON


	; T2CON Register
	; +---+---------+---------+---------+---------+--------+---------+---------+
	; |	— | TOUTPS3 | TOUTPS2 | TOUTPS1 | TOUTPS0 | TMR2ON | T2CKPS1 | T2CKPS0 |
	; +---+---------+---------+---------+---------+--------+---------+---------+
	; bit 7 Unimplemented: Read as '0'
	; bit 6-3 TOUTPS3:TOUTPS0: Timer2 Output Postscale Select bits
	; 0000 = 1:1 Postscale
	; 0001 = 1:2 Postscale
	; •
	; •
	; •
	; 1111 = 1:16 Postscale
	; bit 2 TMR2ON: Timer2 On bit
	; 	1 = Timer2 is on
	; 	0 = Timer2 is off
	; bit 1-0 T2CKPS1:T2CKPS0: Timer2 Clock Prescale Select bits
	; 	00 = Prescaler is 1
	; 	01 = Prescaler is 4
	; 	1x = Prescaler is 16
	MOVLF	b'00000000', T2CON

;	bsf		PIE1, TMR2IE
;	MOVLF	b'11111111', PR2


	; T3CON: TIMER3 CONTROL REGISTER
	; +------+--------+---------+---------+--------+-------+---------+--------+
	; | RD16 | T3CCP2 | T3CKPS1 | T3CKPS0 | T3CCP1 | T3SYN |C TMR3CS | TMR3ON |
	; +------+--------+---------+---------+--------+-------+---------+--------+
	; bit 7 RD16: 16-bit Read/Write Mode Enable bit
	; 	1 = Enables register Read/Write of Timer3 in one 16-bit operation
	; 	0 = Enables register Read/Write of Timer3 in two 8-bit operations
	; bit 6-3 T3CCP2:T3CCP1: Timer3 and Timer1 to CCPx Enable bits
	; 	1x = Timer3 is the clock source for compare/capture CCP modules
	; 	01 = Timer3 is the clock source for compare/capture of CCP2, Timer1 is the clock source for compare/capture of CCP1
	; 	00 = Timer1 is the clock source for compare/capture CCP modules
	; bit 5-4 T3CKPS1:T3CKPS0: Timer3 Input Clock Prescale Select bits
	; 	11 = 1:8 Prescale value
	; 	10 = 1:4 Prescale value
	; 	01 = 1:2 Prescale value
	; 	00 = 1:1 Prescale value
	; bit 2 T3SYNC: Timer3 External Clock Input Synchronization Control bit
	; 	(Not usable if the system clock comes from Timer1/Timer3)
	; 	When TMR3CS = 1:
	; 		1 = Do not synchronize external clock input
	; 		0 = Synchronize external clock input
	; 	When TMR3CS = 0:
	; 		This bit is ignored. Timer3 uses the internal clock when TMR3CS = 0.
	; bit 1 TMR3CS: Timer3 Clock Source Select bit
	; 	1 = External clock input from Timer1 oscillator or T1CKI
	; 		(on the rising edge after the first falling edge)
	; 	0 = Internal clock (FOSC/4)
	; bit 0 TMR3ON: Timer3 On bit
	; 	1 = Enables Timer3
	; 	0 = Stops Timer3
;	MOVLF	b'10011000', T3CON

;	bsf		PIE2, TMR3IE
;	bsf		IPR2, TMR3IP



	; Set ADCON0
  	movlw   b'10000001'
  	movwf   ADCON0
	
	; Set ADCON1
 	movlw   b'10000000'
  	movwf   ADCON1



	MOVLF	b'10100000', T1CON
	bsf		PIE1, TMR1IE
	bsf		IPR1, TMR1IP


	; ##################
	movlw	.1
	movwf	phase

	clrf	phase_cmpt

	clrf	cmpt_pass
	movlw	.128
	movwf	current_cons

;	movlw	.1
;	movwf	cmpt_word
	clrf 	received_counter

;	movwf	pos_recept
	

	; Liaison série !
;	#define	BAUD  .9600
;	#define	XTAL  .4000000
;	#define	X     ((XTAL * .1000000) / ( .64 * BAUD)) - 1

	#define 	BAUD	.9600
	#define		XTAL	.40000000
	#define		X		((XTAL / BAUD) / 64) - 1
;	MOVLF	X, SPBRG

;messg X

	; 64 : 9600baud OK
	;MOVLF	.64, SPBRG

	; 32 : 19200baud OK
	MOVLF	.32, SPBRG

	; 7 : 76.8 (78.13)
	;MOVLF 	.7, SPBRG
	;bcf TXSTA, BRGH

	; 32 : 19200baud, BRGH = 1
	;MOVLF	.129, SPBRG
	;bsf		TXSTA, BRGH

	; 25 : 96k, BRGH = 1
	;MOVLF 	.25, SPBRG
	;bsf		TXSTA, BRGH

	bcf		TXSTA, SYNC

	bsf		RCSTA, SPEN
	bcf		RCSTA, RX9
	bsf		RCSTA, CREN

	bsf		PIE1, RCIE
	bcf		IPR1, RCIP

	; Ajout hugo, le 11/01/2006
	MOVLF	b'00100001', TXSTA

	bsf		PIE1, RCIE
	bcf		IPR1, TXIP
	; Fin ajout hugo

;	clrf 	Current_Cmd
;	clrf 	Arg_Count
;	clrf 	Arg0
;	clrf 	Arg1

	movlw 	0b00000011
	movwf 	Current_Max_H

	movlw 	0b00000000
	movwf 	Current_Max_L

	clrf	Int_Status

	clrf	PORTC
