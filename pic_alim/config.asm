
_init_io:

;config.c,56 :: 		void init_io(){
;config.c,59 :: 		ANSEL = 0b00001111;  // Set RC0,RC1,RC2,RC3 to analog (AN4,AN5,AN6,AN7)
	MOVLW       15
	MOVWF       ANSEL+0 
;config.c,60 :: 		ANSELH = 0b11110100; // Set RC6, RC7, RC5 to analog (AN8, AN9, AN11)
	MOVLW       244
	MOVWF       ANSELH+0 
;config.c,62 :: 		CM1CON0 = 0x00; // Not using the comparators
	CLRF        CM1CON0+0 
;config.c,63 :: 		CM2CON0 = 0x00;
	CLRF        CM2CON0+0 
;config.c,67 :: 		TRISA = 0xFF;
	MOVLW       255
	MOVWF       TRISA+0 
;config.c,68 :: 		TRISA2_bit = 1; // INPUT (ILS)
	BSF         TRISA2_bit+0, BitPos(TRISA2_bit+0) 
;config.c,69 :: 		TRISA5_bit = 0; // OUTPUT (POWER)
	BCF         TRISA5_bit+0, BitPos(TRISA5_bit+0) 
;config.c,71 :: 		INTCON2.RABPU = 0; // PORTA and PORTB Pull-up Enable bit
	BCF         INTCON2+0, 7 
;config.c,72 :: 		WPUA.WPUA2 = 1; // Pull-up enabled sur RA2
	BSF         WPUA+0, 2 
;config.c,74 :: 		TRISB = 0xFF;
	MOVLW       255
	MOVWF       TRISB+0 
;config.c,75 :: 		TRISB5_bit = 1; // RC5 input for AN11
	BSF         TRISB5_bit+0, BitPos(TRISB5_bit+0) 
;config.c,76 :: 		TRISB7_bit = 0; // OUTPUT (LED)
	BCF         TRISB7_bit+0, BitPos(TRISB7_bit+0) 
;config.c,78 :: 		TRISC = 0xFF;
	MOVLW       255
	MOVWF       TRISC+0 
;config.c,79 :: 		TRISC0_bit = 1; // RC0 input for AN4
	BSF         TRISC0_bit+0, BitPos(TRISC0_bit+0) 
;config.c,80 :: 		TRISC1_bit = 1; // RC1 input for AN5
	BSF         TRISC1_bit+0, BitPos(TRISC1_bit+0) 
;config.c,81 :: 		TRISC2_bit = 1; // RC2 input for AN6
	BSF         TRISC2_bit+0, BitPos(TRISC2_bit+0) 
;config.c,82 :: 		TRISC3_bit = 1; // RC3 input for AN7
	BSF         TRISC3_bit+0, BitPos(TRISC3_bit+0) 
;config.c,84 :: 		TRISC6_bit = 1; // RC6 input for AN8
	BSF         TRISC6_bit+0, BitPos(TRISC6_bit+0) 
;config.c,85 :: 		TRISC7_bit = 1; // RC7 input for AN9
	BSF         TRISC7_bit+0, BitPos(TRISC7_bit+0) 
;config.c,86 :: 		}
L_end_init_io:
	RETURN      0
; end of _init_io

_measure_power:

;config.c,88 :: 		void measure_power(){
;config.c,89 :: 		battery_voltage[0] = ADC_Get_Sample(4);   // Get 10-bit results of AD conversion AN4 batterie 1
	MOVLW       4
	MOVWF       FARG_ADC_Get_Sample_channel+0 
	CALL        _ADC_Get_Sample+0, 0
	MOVF        R0, 0 
	MOVWF       _battery_voltage+0 
	MOVF        R1, 0 
	MOVWF       _battery_voltage+1 
;config.c,90 :: 		battery_voltage[1] = ADC_Get_Sample(5);   // Get 10-bit results of AD conversion AN5 batterie 2
	MOVLW       5
	MOVWF       FARG_ADC_Get_Sample_channel+0 
	CALL        _ADC_Get_Sample+0, 0
	MOVF        R0, 0 
	MOVWF       _battery_voltage+2 
	MOVF        R1, 0 
	MOVWF       _battery_voltage+3 
;config.c,91 :: 		battery_voltage[2] = ADC_Get_Sample(6);   // Get 10-bit results of AD conversion AN6 batterie 3
	MOVLW       6
	MOVWF       FARG_ADC_Get_Sample_channel+0 
	CALL        _ADC_Get_Sample+0, 0
	MOVF        R0, 0 
	MOVWF       _battery_voltage+4 
	MOVF        R1, 0 
	MOVWF       _battery_voltage+5 
;config.c,92 :: 		battery_voltage[3] = ADC_Get_Sample(7);   // Get 10-bit results of AD conversion AN7 batterie 4
	MOVLW       7
	MOVWF       FARG_ADC_Get_Sample_channel+0 
	CALL        _ADC_Get_Sample+0, 0
	MOVF        R0, 0 
	MOVWF       _battery_voltage+6 
	MOVF        R1, 0 
	MOVWF       _battery_voltage+7 
;config.c,95 :: 		power_current[0] = ADC_Get_Sample(11);
	MOVLW       11
	MOVWF       FARG_ADC_Get_Sample_channel+0 
	CALL        _ADC_Get_Sample+0, 0
	MOVF        R0, 0 
	MOVWF       _power_current+0 
	MOVF        R1, 0 
	MOVWF       _power_current+1 
;config.c,96 :: 		power_current[1] = ADC_Get_Sample(8);
	MOVLW       8
	MOVWF       FARG_ADC_Get_Sample_channel+0 
	CALL        _ADC_Get_Sample+0, 0
	MOVF        R0, 0 
	MOVWF       _power_current+2 
	MOVF        R1, 0 
	MOVWF       _power_current+3 
;config.c,97 :: 		power_current[2] = ADC_Get_Sample(9);
	MOVLW       9
	MOVWF       FARG_ADC_Get_Sample_channel+0 
	CALL        _ADC_Get_Sample+0, 0
	MOVF        R0, 0 
	MOVWF       _power_current+4 
	MOVF        R1, 0 
	MOVWF       _power_current+5 
;config.c,98 :: 		}
L_end_measure_power:
	RETURN      0
; end of _measure_power

_init_timer0:

;config.c,100 :: 		void init_timer0(){
;config.c,101 :: 		T0CON = 0x85; // TIMER0 ON (1 s)
	MOVLW       133
	MOVWF       T0CON+0 
;config.c,102 :: 		TMR0H = TIMER0_CPT_H;
	MOVLW       11
	MOVWF       TMR0H+0 
;config.c,103 :: 		TMR0L = TIMER0_CPT_L;
	MOVLW       220
	MOVWF       TMR0L+0 
;config.c,104 :: 		TMR0IE_bit = 0;
	BCF         TMR0IE_bit+0, BitPos(TMR0IE_bit+0) 
;config.c,105 :: 		}
L_end_init_timer0:
	RETURN      0
; end of _init_timer0

_init_timer3:

;config.c,107 :: 		void init_timer3(){
;config.c,108 :: 		T3CON = 0x30;
	MOVLW       48
	MOVWF       T3CON+0 
;config.c,109 :: 		TMR3IF_bit = 0;
	BCF         TMR3IF_bit+0, BitPos(TMR3IF_bit+0) 
;config.c,110 :: 		TMR3H = TIMER3_CPT_H;
	MOVLW       60
	MOVWF       TMR3H+0 
;config.c,111 :: 		TMR3L = TIMER3_CPT_L;
	MOVLW       176
	MOVWF       TMR3L+0 
;config.c,112 :: 		TMR3IE_bit = 0;
	BCF         TMR3IE_bit+0, BitPos(TMR3IE_bit+0) 
;config.c,113 :: 		}
L_end_init_timer3:
	RETURN      0
; end of _init_timer3

_ils_analysis:

;config.c,115 :: 		void ils_analysis(unsigned short new_state){
;config.c,116 :: 		if(ILS==0){ // Magnet detected
	BTFSC       PORTA+0, 2 
	GOTO        L_ils_analysis0
;config.c,117 :: 		ils_cpt--;
	DECF        _ils_cpt+0, 1 
;config.c,118 :: 		set_led_on = 1;
	MOVLW       1
	MOVWF       _set_led_on+0 
;config.c,119 :: 		}
	GOTO        L_ils_analysis1
L_ils_analysis0:
;config.c,121 :: 		ils_cpt = ILS_CPT_TIME;
	MOVLW       4
	MOVWF       _ils_cpt+0 
;config.c,122 :: 		set_led_on = 0;
	CLRF        _set_led_on+0 
;config.c,123 :: 		ils_removed = 1;
	MOVLW       1
	MOVWF       _ils_removed+0 
;config.c,124 :: 		}
L_ils_analysis1:
;config.c,126 :: 		if(ils_removed == 1 && ils_cpt == 0){
	MOVF        _ils_removed+0, 0 
	XORLW       1
	BTFSS       STATUS+0, 2 
	GOTO        L_ils_analysis4
	MOVF        _ils_cpt+0, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L_ils_analysis4
L__ils_analysis5:
;config.c,127 :: 		ils_cpt = ILS_CPT_TIME;
	MOVLW       4
	MOVWF       _ils_cpt+0 
;config.c,128 :: 		state = new_state;
	MOVF        FARG_ils_analysis_new_state+0, 0 
	MOVWF       _state+0 
;config.c,129 :: 		ils_removed = 0;
	CLRF        _ils_removed+0 
;config.c,130 :: 		set_led_on = 0;
	CLRF        _set_led_on+0 
;config.c,131 :: 		}
L_ils_analysis4:
;config.c,132 :: 		}
L_end_ils_analysis:
	RETURN      0
; end of _ils_analysis
