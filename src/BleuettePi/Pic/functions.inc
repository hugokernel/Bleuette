
Adc_Read
	; Select Input
	addlw   b'10000000'
	movwf 	ADCON0

	bsf 	ADCON0, ADON

	; Acquisition time from scratch
	movlw	20
	movwf	cmpt_bcl
_Adc_Read_Delay
	nop
	decfsz	cmpt_bcl
	bra		_Adc_Read_Delay

	; Initiate conversion
	bsf		ADCON0, GO_DONE

	; wait for ADC to finish
_Adc_Read_Wait
	btfsc   ADCON0, GO_DONE
	goto    _Adc_Read_Wait

	movf    ADRESH, W
	andlw   0x03
	movwf   Adc_Result_H
	movf    ADRESL, W
	movwf	Adc_Result_L
	return

Serial_Send
	movwf	TXREG
	btfss	PIR1, TXIF
	goto	$ - 2
	return

Serial_Send_Delay
	movwf	TXREG
	btfss	PIR1, TXIF
	goto	$ - 2

	movlw	0xFF
	movwf	cmpt_bcl
_Serial_Send_Delay
	bra		$ + 2
	bra		$ + 2
	bra		$ + 2
	bra		$ + 2
	bra		$ + 2
	bra		$ + 2
	bra		$ + 2
	bra 	$ + 2
	bra 	$ + 2
	decfsz	cmpt_bcl
	bra		_Serial_Send_Delay
	return

;Adc_Serial_Send
;	return

Current_Test
	INT_OFF
	;btfsc 	Int_Status, INT_STATUS_MAX_CURRENT_REACHED
	;goto 	_Current_Test_Exit

	; Read current value just before stop port !
	;movlw 	ADC_AN0;CURRENT_CHANNEL
	movlw 	CURRENT_CHANNEL
	call 	Adc_Read
	movff 	Adc_Result_H, Current_Last_H
	movff 	Adc_Result_L, Current_Last_L

	;movlw 	1
	;movwf 	Current_Max_H
	;clrf 	Current_Max_L

	; Test if upper than max
	; 0. Si H = 0 > Goto 4
	; 1. Si H > HM > Alert !
	; 2. Si H < HM > Exit
	; 3. Si H = HM >
	; 4.	Si L > LM > Alert
	; 5.	Si L < LM > Exit
	; 6. 	Si L = LM > Alert

	; If 0 ? Skip directly to Low nibble test
	;tstfsz 	Current_Last_H
	;bra 	$ + 4
	;goto 	_Current_Test_L

	; High nibble test
_Current_Test_H
	movf 	Current_Max_H, W
	cpfslt 	Current_Last_H
	bra		$ + 4

	goto 	_Current_Test_Exit

	movf 	Current_Max_H, W
	cpfseq 	Current_Last_H
	bra 	$ + 4

	goto 	_Current_Test_Alert
	;goto 	_Current_Test_L

	movf 	Current_Max_H, W
	cpfslt 	Current_Last_H
	goto 	_Current_Test_Alert

	; Low nibble test
_Current_Test_L
	;tstfsz 	Current_Last_L
	;bra 	$ + 4
	;goto 	_Current_Test_Exit

	movf 	Current_Max_L, W
	cpfseq 	Current_Last_L
	bra 	$ + 4
	goto 	_Current_Test_Alert

	movf 	Current_Max_L, W
	cpfslt 	Current_Last_L
	goto 	_Current_Test_Alert

	;cpfslt 	Current_Last_L
	;goto 	_Current_Test_Exit

	goto 	_Current_Test_Exit

_Current_Test_Alert
	bsf 	Int_Status, INT_STATUS_MAX_CURRENT_REACHED
	INT_ON
_Current_Test_Exit
	return