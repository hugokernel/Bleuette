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

#DEFINE	CONS_ALL_0
;#DEFINE	CONS_ALL_1
;#DEFINE	CONS_ALL_254
;#DEFINE	CONS_DEC
;#DEFINE	CONS_INC

#IFDEF	CONS_ALL_0
	MOVLF	.0, servo_cons0		; 00
	MOVLF	.0, servo_cons1		; 01
	MOVLF	.0, servo_cons2		; 02
	MOVLF	.0, servo_cons3		; 03
	MOVLF	.0, servo_cons4		; 04
	MOVLF	.0, servo_cons5		; 05
	MOVLF	.0, servo_cons6		; 06
	MOVLF	.0, servo_cons7		; 07
	MOVLF	.0, servo_cons8		; 08
	MOVLF	.0, servo_cons9		; 09
	MOVLF	.0, servo_cons10	; 0A
	MOVLF	.0, servo_cons11	; 0B
	MOVLF	.0, servo_cons12	; 0C
	MOVLF	.0, servo_cons13	; 0D
	MOVLF	.0, servo_cons14	; 0E
	MOVLF	.0, servo_cons15	; 0F
	MOVLF	.0, servo_cons16	; 10
	MOVLF	.0, servo_cons17	; 12
	MOVLF	.0, servo_cons18	; 13
	MOVLF	.0, servo_cons19	; 14
	MOVLF	.0, servo_cons20	; 15
	MOVLF	.0, servo_cons21	; 16
	MOVLF	.0, servo_cons22	; 17
	MOVLF	.0, servo_cons23	; 18
#ENDIF

#IFDEF	CONS_ALL_1
	MOVLF	.1, servo_cons0		; 00
	MOVLF	.1, servo_cons1		; 01
	MOVLF	.1, servo_cons2		; 02
	MOVLF	.1, servo_cons3		; 03
	MOVLF	.1, servo_cons4		; 04
	MOVLF	.1, servo_cons5		; 05
	MOVLF	.1, servo_cons6		; 06
	MOVLF	.1, servo_cons7		; 07
	MOVLF	.1, servo_cons8		; 08
	MOVLF	.1, servo_cons9		; 09
	MOVLF	.1, servo_cons10	; 0A
	MOVLF	.1, servo_cons11	; 0B
	MOVLF	.1, servo_cons12	; 0C
	MOVLF	.1, servo_cons13	; 0D
	MOVLF	.1, servo_cons14	; 0E
	MOVLF	.1, servo_cons15	; 0F
	MOVLF	.1, servo_cons16	; 10
	MOVLF	.1, servo_cons17	; 12
	MOVLF	.1, servo_cons18	; 13
	MOVLF	.1, servo_cons19	; 14
	MOVLF	.1, servo_cons20	; 15
	MOVLF	.1, servo_cons21	; 16
	MOVLF	.1, servo_cons22	; 17
	MOVLF	.1, servo_cons23	; 18
#ENDIF

#IFDEF	CONS_ALL_254
	MOVLF	.254, servo_cons0		; 00
	MOVLF	.254, servo_cons1		; 01
	MOVLF	.254, servo_cons2		; 02
	MOVLF	.254, servo_cons3		; 03
	MOVLF	.254, servo_cons4		; 04
	MOVLF	.254, servo_cons5		; 05
	MOVLF	.254, servo_cons6		; 06
	MOVLF	.254, servo_cons7		; 07
	MOVLF	.254, servo_cons8		; 08
	MOVLF	.254, servo_cons9		; 09
	MOVLF	.254, servo_cons10	; 0A
	MOVLF	.254, servo_cons11	; 0B
	MOVLF	.254, servo_cons12	; 0C
	MOVLF	.254, servo_cons13	; 0D
	MOVLF	.254, servo_cons14	; 0E
	MOVLF	.254, servo_cons15	; 0F
	MOVLF	.254, servo_cons16	; 10
	MOVLF	.254, servo_cons17	; 12
	MOVLF	.254, servo_cons18	; 13
	MOVLF	.254, servo_cons19	; 14
	MOVLF	.254, servo_cons20	; 15
	MOVLF	.254, servo_cons21	; 16
	MOVLF	.254, servo_cons22	; 17
	MOVLF	.254, servo_cons23	; 18
#ENDIF

#IFDEF	CONS_DEC
	MOVLF	.255, servo_cons0		; 00
	MOVLF	.220, servo_cons1		; 01
	MOVLF	.210, servo_cons2		; 02
	MOVLF	.200, servo_cons3		; 03
	MOVLF	.190, servo_cons4		; 04
	MOVLF	.180, servo_cons5		; 05
	MOVLF	.170, servo_cons6		; 06
	MOVLF	.160, servo_cons7		; 07
	MOVLF	.150, servo_cons8		; 08
	MOVLF	.140, servo_cons9		; 09
	MOVLF	.130, servo_cons10	; 0A
	MOVLF	.120, servo_cons11	; 0B
	MOVLF	.110, servo_cons12	; 0C
	MOVLF	.100, servo_cons13	; 0D
	MOVLF	.90, servo_cons14	; 0E
	MOVLF	.80, servo_cons15	; 0F
	MOVLF	.70, servo_cons16	; 10
	MOVLF	.60, servo_cons17	; 12
	MOVLF	.50, servo_cons18	; 13
	MOVLF	.40, servo_cons19	; 14
	MOVLF	.30, servo_cons20	; 15
	MOVLF	.20, servo_cons21	; 16
	MOVLF	.10, servo_cons22	; 17
	MOVLF	.1, servo_cons23	; 18
#ENDIF


#IFDEF	CONS_INC
	MOVLF	.1, servo_cons0		; 00
	MOVLF	.2, servo_cons1		; 01
	MOVLF	.3, servo_cons2		; 02
	MOVLF	.4, servo_cons3		; 03
	MOVLF	.5, servo_cons4		; 04
	MOVLF	.6, servo_cons5		; 05
	MOVLF	.7, servo_cons6		; 06
	MOVLF	.8, servo_cons7		; 07
	MOVLF	.9, servo_cons8		; 08
	MOVLF	.10, servo_cons9	; 09
	MOVLF	.11, servo_cons10	; 0A
	MOVLF	.12, servo_cons11	; 0B
	MOVLF	.13, servo_cons12	; 0C
	MOVLF	.14, servo_cons13	; 0D
	MOVLF	.15, servo_cons14	; 0E
	MOVLF	.16, servo_cons15	; 0F
	MOVLF	.17, servo_cons16	; 10
	MOVLF	.18, servo_cons17	; 12
	MOVLF	.19, servo_cons18	; 13
	MOVLF	.20, servo_cons19	; 14
	MOVLF	.128, servo_cons20	; 15
	MOVLF	.129, servo_cons21	; 16
	MOVLF	.253, servo_cons22	; 17
	MOVLF	.254, servo_cons23	; 18
#ENDIF


	movlb	.1

#IFDEF	CONS_INC

	MOVLFB	.1, servo_nbr00
	MOVLFB	.1, servo_nbr01
	MOVLFB	.1, servo_nbr02
	MOVLFB	.1, servo_nbr03
	MOVLFB	.1, servo_nbr04
	MOVLFB	.1, servo_nbr05
	MOVLFB	.1, servo_nbr06
	MOVLFB	.1, servo_nbr07
	MOVLFB	.1, servo_nbr08
	MOVLFB	.1, servo_nbr09
	MOVLFB	.1, servo_nbr10
	MOVLFB	.1, servo_nbr11
	MOVLFB	.1, servo_nbr12
	MOVLFB	.1, servo_nbr13
	MOVLFB	.1, servo_nbr14
	MOVLFB	.1, servo_nbr15
	MOVLFB	.1, servo_nbr16
	MOVLFB	.1, servo_nbr17
	MOVLFB	.1, servo_nbr18
	MOVLFB	.1, servo_nbr19
	MOVLFB	.1, servo_nbr20
	MOVLFB	.1, servo_nbr21
	MOVLFB	.1, servo_nbr22
	MOVLFB	.1, servo_nbr23

	MOVLFB	b'00000001', servo_val00
	MOVLFB	b'00000000', servo_val01		; 0
	MOVLFB	b'00000000', servo_val02

	MOVLFB	b'00000010', servo_val03
	MOVLFB	b'00000000', servo_val04		; 1
	MOVLFB	b'00000000', servo_val05

	MOVLFB	b'00000100', servo_val06
	MOVLFB	b'00000000', servo_val07		; 2
	MOVLFB	b'00000000', servo_val08

	MOVLFB	b'00001000', servo_val09
	MOVLFB	b'00000000', servo_val10		; 3
	MOVLFB	b'00000000', servo_val11

	MOVLFB	b'00010000', servo_val12
	MOVLFB	b'00000000', servo_val13		; 4
	MOVLFB	b'00000000', servo_val14

	MOVLFB	b'00100000', servo_val15
	MOVLFB	b'00000000', servo_val16		; 5
	MOVLFB	b'00000000', servo_val17

	MOVLFB	b'01000000', servo_val18
	MOVLFB	b'00000000', servo_val19		; 6
	MOVLFB	b'00000000', servo_val20

	MOVLFB	b'10000000', servo_val21
	MOVLFB	b'00000000', servo_val22		; 7
	MOVLFB	b'00000000', servo_val23

	MOVLFB	b'00000000', servo_val24
	MOVLFB	b'00000001', servo_val25		; 8
	MOVLFB	b'00000000', servo_val26

	MOVLFB	b'00000000', servo_val27
	MOVLFB	b'00000010', servo_val28		; 9
	MOVLFB	b'00000000', servo_val29

	MOVLFB	b'00000000', servo_val30
	MOVLFB	b'00000100', servo_val31		; 10
	MOVLFB	b'00000000', servo_val32

	MOVLFB	b'00000000', servo_val33
	MOVLFB	b'00001000', servo_val34		; 11
	MOVLFB	b'00000000', servo_val35

	MOVLFB	b'00000000', servo_val36
	MOVLFB	b'00010000', servo_val37		; 12
	MOVLFB	b'00000000', servo_val38

	MOVLFB	b'00000000', servo_val39
	MOVLFB	b'00100000', servo_val40		; 13
	MOVLFB	b'00000000', servo_val41

	MOVLFB	b'00000000', servo_val42
	MOVLFB	b'01000000', servo_val43		; 14
	MOVLFB	b'00000000', servo_val44

	MOVLFB	b'00000000', servo_val45
	MOVLFB	b'10000000', servo_val46		; 15
	MOVLFB	b'00000000', servo_val47

	MOVLFB	b'00000000', servo_val48
	MOVLFB	b'00000000', servo_val49		; 16
	MOVLFB	b'00000001', servo_val50

	MOVLFB	b'00000000', servo_val51
	MOVLFB	b'00000000', servo_val52		; 17
	MOVLFB	b'00000010', servo_val53

	MOVLFB	b'00000000', servo_val54
	MOVLFB	b'00000000', servo_val55		; 18
	MOVLFB	b'00000100', servo_val56

	MOVLFB	b'00000000', servo_val57
	MOVLFB	b'00000000', servo_val58		; 19
	MOVLFB	b'00001000', servo_val59

	MOVLFB	b'00000000', servo_val60
	MOVLFB	b'00000000', servo_val61		; 20
	MOVLFB	b'00010000', servo_val62

	MOVLFB	b'00000000', servo_val63
	MOVLFB	b'00000000', servo_val64		; 21
	MOVLFB	b'00100000', servo_val65

	MOVLFB	b'00000000', servo_val66
	MOVLFB	b'00000000', servo_val67		; 22
	MOVLFB	b'01000000', servo_val68

	MOVLFB	b'00000000', servo_val69
	MOVLFB	b'00000000', servo_val70		; 23
	MOVLFB	b'10000000', servo_val71

#ENDIF

#IFDEF	CONS_ALL_0

	MOVLFB	.23, servo_nbr00
	MOVLFB	.0, servo_nbr01
	MOVLFB	.0, servo_nbr02
	MOVLFB	.0, servo_nbr03
	MOVLFB	.0, servo_nbr04
	MOVLFB	.0, servo_nbr05
	MOVLFB	.0, servo_nbr06
	MOVLFB	.0, servo_nbr07
	MOVLFB	.0, servo_nbr08
	MOVLFB	.0, servo_nbr09
	MOVLFB	.0, servo_nbr10
	MOVLFB	.0, servo_nbr11
	MOVLFB	.0, servo_nbr12
	MOVLFB	.0, servo_nbr13
	MOVLFB	.0, servo_nbr14
	MOVLFB	.0, servo_nbr15
	MOVLFB	.0, servo_nbr16
	MOVLFB	.0, servo_nbr17
	MOVLFB	.0, servo_nbr18
	MOVLFB	.0, servo_nbr19
	MOVLFB	.0, servo_nbr20
	MOVLFB	.0, servo_nbr21
	MOVLFB	.0, servo_nbr22
	MOVLFB	.0, servo_nbr23

	MOVLFB	b'11111111', servo_val00
	MOVLFB	b'11111111', servo_val01		; 0
	MOVLFB	b'11111111', servo_val02

	MOVLFB	b'00000000', servo_val03
	MOVLFB	b'00000000', servo_val04		; 1
	MOVLFB	b'00000000', servo_val05

	MOVLFB	b'00000000', servo_val06
	MOVLFB	b'00000000', servo_val07		; 2
	MOVLFB	b'00000000', servo_val08

	MOVLFB	b'00000000', servo_val09
	MOVLFB	b'00000000', servo_val10		; 3
	MOVLFB	b'00000000', servo_val11

	MOVLFB	b'00000000', servo_val12
	MOVLFB	b'00000000', servo_val13		; 4
	MOVLFB	b'00000000', servo_val14

	MOVLFB	b'00000000', servo_val15
	MOVLFB	b'00000000', servo_val16		; 5
	MOVLFB	b'00000000', servo_val17

	MOVLFB	b'00000000', servo_val18
	MOVLFB	b'00000000', servo_val19		; 6
	MOVLFB	b'00000000', servo_val20

	MOVLFB	b'00000000', servo_val21
	MOVLFB	b'00000000', servo_val22		; 7
	MOVLFB	b'00000000', servo_val23

	MOVLFB	b'00000000', servo_val24
	MOVLFB	b'00000000', servo_val25		; 8
	MOVLFB	b'00000000', servo_val26

	MOVLFB	b'00000000', servo_val27
	MOVLFB	b'00000000', servo_val28		; 9
	MOVLFB	b'00000000', servo_val29

	MOVLFB	b'00000000', servo_val30
	MOVLFB	b'00000000', servo_val31		; 10
	MOVLFB	b'00000000', servo_val32

	MOVLFB	b'00000000', servo_val33
	MOVLFB	b'00000000', servo_val34		; 11
	MOVLFB	b'00000000', servo_val35

	MOVLFB	b'00000000', servo_val36
	MOVLFB	b'00000000', servo_val37		; 12
	MOVLFB	b'00000000', servo_val38

	MOVLFB	b'00000000', servo_val39
	MOVLFB	b'00000000', servo_val40		; 13
	MOVLFB	b'00000000', servo_val41

	MOVLFB	b'00000000', servo_val42
	MOVLFB	b'00000000', servo_val43		; 14
	MOVLFB	b'00000000', servo_val44

	MOVLFB	b'00000000', servo_val45
	MOVLFB	b'00000000', servo_val46		; 15
	MOVLFB	b'00000000', servo_val47

	MOVLFB	b'00000000', servo_val48
	MOVLFB	b'00000000', servo_val49		; 16
	MOVLFB	b'00000000', servo_val50

	MOVLFB	b'00000000', servo_val51
	MOVLFB	b'00000000', servo_val52		; 17
	MOVLFB	b'00000000', servo_val53

	MOVLFB	b'00000000', servo_val54
	MOVLFB	b'00000000', servo_val55		; 18
	MOVLFB	b'00000000', servo_val56

	MOVLFB	b'00000000', servo_val57
	MOVLFB	b'00000000', servo_val58		; 19
	MOVLFB	b'00000000', servo_val59

	MOVLFB	b'00000000', servo_val60
	MOVLFB	b'00000000', servo_val61		; 20
	MOVLFB	b'00000000', servo_val62

	MOVLFB	b'00000000', servo_val63
	MOVLFB	b'00000000', servo_val64		; 21
	MOVLFB	b'00000000', servo_val65

	MOVLFB	b'00000000', servo_val66
	MOVLFB	b'00000000', servo_val67		; 22
	MOVLFB	b'00000000', servo_val68

	MOVLFB	b'00000000', servo_val69
	MOVLFB	b'00000000', servo_val70		; 23
	MOVLFB	b'00000000', servo_val71

#ENDIF

	movlb	.0
