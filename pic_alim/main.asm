
_main:

;main.c,23 :: 		void main(){
;main.c,24 :: 		unsigned short k = 0;
	CLRF        main_k_L0+0 
;main.c,34 :: 		OSCCON = 0b11110010;   // 0=4xPLL OFF, 111=IRCF<2:0>=16Mhz  OSTS=0  SCS<1:0>10 1x = Internal oscillator block
	MOVLW       242
	MOVWF       OSCCON+0 
;main.c,36 :: 		asm CLRWDT;// Watchdog
	CLRWDT
;main.c,37 :: 		SWDTEN_bit = 1; //armement du watchdog
	BSF         SWDTEN_bit+0, BitPos(SWDTEN_bit+0) 
;main.c,39 :: 		init_io(); // Initialize I/O
	CALL        _init_io+0, 0
;main.c,40 :: 		init_i2c(0x42); // Initialize I2C
	MOVLW       66
	MOVWF       FARG_init_i2c_address_i2c+0 
	CALL        _init_i2c+0, 0
;main.c,41 :: 		init_timer0(); // Initialize TIMER0 every 1 seconds
	CALL        _init_timer0+0, 0
;main.c,42 :: 		init_timer3(); // Initialize TIMER3 every 100ms
	CALL        _init_timer3+0, 0
;main.c,44 :: 		ADC_Init();
	CALL        _ADC_Init+0, 0
;main.c,46 :: 		ALIM = 1; // sortie MOSFET de puissance, commande de l'alimentation
	BSF         PORTA+0, 5 
;main.c,47 :: 		LED = 0;
	BCF         PORTB+0, 7 
;main.c,49 :: 		INTCON3.INT1IP = 0; //INT1 External Interrupt Priority bit, INT0 always a high
	BCF         INTCON3+0, 6 
;main.c,52 :: 		RCON.IPEN = 1;  //Enable priority levels on interrupts
	BSF         RCON+0, 7 
;main.c,53 :: 		IPR1.SSPIP = 0; //Master Synchronous Serial Port Interrupt Priority bit (low priority = 0)
	BCF         IPR1+0, 3 
;main.c,54 :: 		INTCON.GIEH = 1; //enable all high-priority interrupts
	BSF         INTCON+0, 7 
;main.c,55 :: 		INTCON.GIEL = 1; //enable all low-priority interrupts
	BSF         INTCON+0, 6 
;main.c,57 :: 		INTCON.GIE = 1; // Global Interrupt Enable bit
	BSF         INTCON+0, 7 
;main.c,58 :: 		INTCON.PEIE = 1; // Peripheral Interrupt Enable bit
	BSF         INTCON+0, 6 
;main.c,60 :: 		TMR0IE_bit = 1;  //Enable TIMER0
	BSF         TMR0IE_bit+0, BitPos(TMR0IE_bit+0) 
;main.c,61 :: 		TMR0ON_bit = 1; // Start TIMER1
	BSF         TMR0ON_bit+0, BitPos(TMR0ON_bit+0) 
;main.c,63 :: 		TMR3IE_bit = 1;  //Enable TIMER3
	BSF         TMR3IE_bit+0, BitPos(TMR3IE_bit+0) 
;main.c,64 :: 		TMR3ON_bit = 1; // Start TIMER3
	BSF         TMR3ON_bit+0, BitPos(TMR3ON_bit+0) 
;main.c,66 :: 		is_init = 1;
	MOVLW       1
	MOVWF       _is_init+0 
;main.c,68 :: 		while(1){
L_main0:
;main.c,69 :: 		asm CLRWDT;
	CLRWDT
;main.c,71 :: 		if(step_state_machine==1){
	MOVF        _step_state_machine+0, 0 
	XORLW       1
	BTFSS       STATUS+0, 2 
	GOTO        L_main2
;main.c,72 :: 		step_state_machine=0;
	CLRF        _step_state_machine+0 
;main.c,73 :: 		measure_power();
	CALL        _measure_power+0, 0
;main.c,75 :: 		switch (state){
	GOTO        L_main3
;main.c,76 :: 		case IDLE: // Idle state
L_main5:
;main.c,77 :: 		ALIM = 0;
	BCF         PORTA+0, 5 
;main.c,78 :: 		led_delay = 50;
	MOVLW       50
	MOVWF       _led_delay+0 
;main.c,79 :: 		watchdog_restart = watchdog_restart_default;
	MOVF        _watchdog_restart_default+0, 0 
	MOVWF       _watchdog_restart+0 
;main.c,81 :: 		ils_analysis(POWER_ON);
	MOVLW       1
	MOVWF       FARG_ils_analysis_new_state+0 
	CALL        _ils_analysis+0, 0
;main.c,82 :: 		break;
	GOTO        L_main4
;main.c,84 :: 		case POWER_ON:
L_main6:
;main.c,85 :: 		ALIM = 1;
	BSF         PORTA+0, 5 
;main.c,89 :: 		led_delay = 20; // 2sec
	MOVLW       20
	MOVWF       _led_delay+0 
;main.c,91 :: 		ils_analysis(IDLE);
	CLRF        FARG_ils_analysis_new_state+0 
	CALL        _ils_analysis+0, 0
;main.c,92 :: 		break;
	GOTO        L_main4
;main.c,94 :: 		case WAIT_TO_SLEEP:
L_main7:
;main.c,96 :: 		ALIM = 1;
	BSF         PORTA+0, 5 
;main.c,97 :: 		led_delay = 1;
	MOVLW       1
	MOVWF       _led_delay+0 
;main.c,98 :: 		if(time_to_stop==0){
	MOVF        _time_to_stop+0, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L_main8
;main.c,99 :: 		for(k=0; k<3; k++)
	CLRF        main_k_L0+0 
L_main9:
	MOVLW       3
	SUBWF       main_k_L0+0, 0 
	BTFSC       STATUS+0, 0 
	GOTO        L_main10
;main.c,100 :: 		time_to_start[k] = default_time_to_start[k];
	MOVLW       _time_to_start+0
	MOVWF       FSR1L+0 
	MOVLW       hi_addr(_time_to_start+0)
	MOVWF       FSR1L+1 
	MOVF        main_k_L0+0, 0 
	ADDWF       FSR1L+0, 1 
	BTFSC       STATUS+0, 0 
	INCF        FSR1L+1, 1 
	MOVLW       _default_time_to_start+0
	MOVWF       FSR0L+0 
	MOVLW       hi_addr(_default_time_to_start+0)
	MOVWF       FSR0L+1 
	MOVF        main_k_L0+0, 0 
	ADDWF       FSR0L+0, 1 
	BTFSC       STATUS+0, 0 
	INCF        FSR0L+1, 1 
	MOVF        POSTINC0+0, 0 
	MOVWF       POSTINC1+0 
;main.c,99 :: 		for(k=0; k<3; k++)
	INCF        main_k_L0+0, 1 
;main.c,100 :: 		time_to_start[k] = default_time_to_start[k];
	GOTO        L_main9
L_main10:
;main.c,101 :: 		state = SLEEP;
	MOVLW       3
	MOVWF       _state+0 
;main.c,102 :: 		time_to_stop = default_time_to_stop;
	MOVF        _default_time_to_stop+0, 0 
	MOVWF       _time_to_stop+0 
;main.c,103 :: 		}
L_main8:
;main.c,104 :: 		ils_analysis(POWER_ON);
	MOVLW       1
	MOVWF       FARG_ils_analysis_new_state+0 
	CALL        _ils_analysis+0, 0
;main.c,105 :: 		break;
	GOTO        L_main4
;main.c,107 :: 		case SLEEP:
L_main12:
;main.c,108 :: 		ALIM = 0;
	BCF         PORTA+0, 5 
;main.c,109 :: 		led_delay = 200; // 20 sec
	MOVLW       200
	MOVWF       _led_delay+0 
;main.c,110 :: 		if(time_to_start[0] == 0 && time_to_start[1] == 0 && time_to_start[2] == 0){
	MOVF        _time_to_start+0, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L_main15
	MOVF        _time_to_start+1, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L_main15
	MOVF        _time_to_start+2, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L_main15
L__main45:
;main.c,111 :: 		state = POWER_ON;
	MOVLW       1
	MOVWF       _state+0 
;main.c,112 :: 		}
L_main15:
;main.c,113 :: 		ils_analysis(POWER_ON);
	MOVLW       1
	MOVWF       FARG_ils_analysis_new_state+0 
	CALL        _ils_analysis+0, 0
;main.c,114 :: 		break;
	GOTO        L_main4
;main.c,116 :: 		default:
L_main16:
;main.c,117 :: 		state = POWER_ON;
	MOVLW       1
	MOVWF       _state+0 
;main.c,118 :: 		break;
	GOTO        L_main4
;main.c,119 :: 		}
L_main3:
	MOVF        _state+0, 0 
	XORLW       0
	BTFSC       STATUS+0, 2 
	GOTO        L_main5
	MOVF        _state+0, 0 
	XORLW       1
	BTFSC       STATUS+0, 2 
	GOTO        L_main6
	MOVF        _state+0, 0 
	XORLW       2
	BTFSC       STATUS+0, 2 
	GOTO        L_main7
	MOVF        _state+0, 0 
	XORLW       3
	BTFSC       STATUS+0, 2 
	GOTO        L_main12
	GOTO        L_main16
L_main4:
;main.c,120 :: 		}
L_main2:
;main.c,122 :: 		if(nb_rx_octet>1 && SSPSTAT.P == 1){
	MOVF        _nb_rx_octet+0, 0 
	SUBLW       1
	BTFSC       STATUS+0, 0 
	GOTO        L_main19
	BTFSS       SSPSTAT+0, 4 
	GOTO        L_main19
L__main44:
;main.c,123 :: 		i2c_read_data_from_buffer();
	CALL        _i2c_read_data_from_buffer+0, 0
;main.c,124 :: 		nb_rx_octet = 0;
	CLRF        _nb_rx_octet+0 
;main.c,125 :: 		}
L_main19:
;main.c,126 :: 		}
	GOTO        L_main0
;main.c,127 :: 		}
L_end_main:
	GOTO        $+0
; end of _main

_interrupt:

;main.c,132 :: 		void interrupt(){
;main.c,137 :: 		if (TMR0IF_bit){
	BTFSS       TMR0IF_bit+0, BitPos(TMR0IF_bit+0) 
	GOTO        L_interrupt20
;main.c,140 :: 		if(state == SLEEP){
	MOVF        _state+0, 0 
	XORLW       3
	BTFSS       STATUS+0, 2 
	GOTO        L_interrupt21
;main.c,141 :: 		if(time_to_start[2]>0){
	MOVF        _time_to_start+2, 0 
	SUBLW       0
	BTFSC       STATUS+0, 0 
	GOTO        L_interrupt22
;main.c,142 :: 		time_to_start[2]--;
	DECF        _time_to_start+2, 1 
;main.c,143 :: 		}
	GOTO        L_interrupt23
L_interrupt22:
;main.c,145 :: 		if(time_to_start[1]>0){
	MOVF        _time_to_start+1, 0 
	SUBLW       0
	BTFSC       STATUS+0, 0 
	GOTO        L_interrupt24
;main.c,146 :: 		time_to_start[1]--;
	DECF        _time_to_start+1, 1 
;main.c,147 :: 		time_to_start[2]=59;
	MOVLW       59
	MOVWF       _time_to_start+2 
;main.c,148 :: 		}
	GOTO        L_interrupt25
L_interrupt24:
;main.c,150 :: 		if(time_to_start[0]>0){
	MOVF        _time_to_start+0, 0 
	SUBLW       0
	BTFSC       STATUS+0, 0 
	GOTO        L_interrupt26
;main.c,151 :: 		time_to_start[0]--;
	DECF        _time_to_start+0, 1 
;main.c,152 :: 		time_to_start[1]=59;
	MOVLW       59
	MOVWF       _time_to_start+1 
;main.c,153 :: 		}
L_interrupt26:
;main.c,154 :: 		}
L_interrupt25:
;main.c,155 :: 		}
L_interrupt23:
;main.c,156 :: 		}
L_interrupt21:
;main.c,158 :: 		if(state == WAIT_TO_SLEEP){
	MOVF        _state+0, 0 
	XORLW       2
	BTFSS       STATUS+0, 2 
	GOTO        L_interrupt27
;main.c,159 :: 		if(time_to_stop>0)
	MOVF        _time_to_stop+0, 0 
	SUBLW       0
	BTFSC       STATUS+0, 0 
	GOTO        L_interrupt28
;main.c,160 :: 		time_to_stop--;
	DECF        _time_to_stop+0, 1 
L_interrupt28:
;main.c,161 :: 		}
L_interrupt27:
;main.c,164 :: 		if(state == POWER_ON && watchdog_restart_default!=0){
	MOVF        _state+0, 0 
	XORLW       1
	BTFSS       STATUS+0, 2 
	GOTO        L_interrupt31
	MOVF        _watchdog_restart_default+0, 0 
	XORLW       0
	BTFSC       STATUS+0, 2 
	GOTO        L_interrupt31
L__interrupt46:
;main.c,165 :: 		if(watchdog_cpt_sec>0)
	MOVF        _watchdog_cpt_sec+0, 0 
	SUBLW       0
	BTFSC       STATUS+0, 0 
	GOTO        L_interrupt32
;main.c,166 :: 		watchdog_cpt_sec--;
	DECF        _watchdog_cpt_sec+0, 1 
	GOTO        L_interrupt33
L_interrupt32:
;main.c,168 :: 		watchdog_cpt_sec = watchdog_cpt_default;
	MOVF        _watchdog_cpt_default+0, 0 
	MOVWF       _watchdog_cpt_sec+0 
;main.c,170 :: 		if(watchdog_restart>0)
	MOVF        _watchdog_restart+0, 0 
	SUBLW       0
	BTFSC       STATUS+0, 0 
	GOTO        L_interrupt34
;main.c,171 :: 		watchdog_restart--;
	DECF        _watchdog_restart+0, 1 
	GOTO        L_interrupt35
L_interrupt34:
;main.c,174 :: 		default_time_to_start[0] = 0;
	CLRF        _default_time_to_start+0 
;main.c,175 :: 		default_time_to_start[1] = 0;
	CLRF        _default_time_to_start+1 
;main.c,176 :: 		default_time_to_start[2] = 2; // 2s
	MOVLW       2
	MOVWF       _default_time_to_start+2 
;main.c,177 :: 		time_to_stop = default_time_to_stop;
	MOVF        _default_time_to_stop+0, 0 
	MOVWF       _time_to_stop+0 
;main.c,179 :: 		state = WAIT_TO_SLEEP;
	MOVLW       2
	MOVWF       _state+0 
;main.c,180 :: 		watchdog_restart = watchdog_restart_default;
	MOVF        _watchdog_restart_default+0, 0 
	MOVWF       _watchdog_restart+0 
;main.c,181 :: 		}
L_interrupt35:
;main.c,182 :: 		}
L_interrupt33:
;main.c,183 :: 		}
L_interrupt31:
;main.c,185 :: 		TMR0H = TIMER0_CPT_H;
	MOVLW       11
	MOVWF       TMR0H+0 
;main.c,186 :: 		TMR0L = TIMER0_CPT_L;
	MOVLW       220
	MOVWF       TMR0L+0 
;main.c,187 :: 		TMR0IF_bit = 0;
	BCF         TMR0IF_bit+0, BitPos(TMR0IF_bit+0) 
;main.c,188 :: 		}
	GOTO        L_interrupt36
L_interrupt20:
;main.c,191 :: 		else if (TMR3IF_bit){
	BTFSS       TMR3IF_bit+0, BitPos(TMR3IF_bit+0) 
	GOTO        L_interrupt37
;main.c,193 :: 		if(set_led_on == 1) // For ILS
	MOVF        _set_led_on+0, 0 
	XORLW       1
	BTFSS       STATUS+0, 2 
	GOTO        L_interrupt38
;main.c,194 :: 		LED = 1;
	BSF         PORTB+0, 7 
	GOTO        L_interrupt39
L_interrupt38:
;main.c,196 :: 		if (cpt_led > 0){
	MOVF        _cpt_led+0, 0 
	SUBLW       0
	BTFSC       STATUS+0, 0 
	GOTO        L_interrupt40
;main.c,197 :: 		LED = 0;
	BCF         PORTB+0, 7 
;main.c,198 :: 		cpt_led--;
	DECF        _cpt_led+0, 1 
;main.c,199 :: 		}
	GOTO        L_interrupt41
L_interrupt40:
;main.c,201 :: 		LED = 1;
	BSF         PORTB+0, 7 
;main.c,202 :: 		cpt_led=led_delay;
	MOVF        _led_delay+0, 0 
	MOVWF       _cpt_led+0 
;main.c,203 :: 		}
L_interrupt41:
;main.c,204 :: 		}
L_interrupt39:
;main.c,207 :: 		if(cpt_state_machine==0){
	MOVF        _cpt_state_machine+0, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L_interrupt42
;main.c,208 :: 		cpt_state_machine = CPT_STATE_MACHINE_DEFAULT;
	MOVLW       5
	MOVWF       _cpt_state_machine+0 
;main.c,209 :: 		step_state_machine = 1;
	MOVLW       1
	MOVWF       _step_state_machine+0 
;main.c,210 :: 		}
	GOTO        L_interrupt43
L_interrupt42:
;main.c,212 :: 		cpt_state_machine--;
	DECF        _cpt_state_machine+0, 1 
L_interrupt43:
;main.c,214 :: 		TMR3H = TIMER3_CPT_H;
	MOVLW       60
	MOVWF       TMR3H+0 
;main.c,215 :: 		TMR3L = TIMER3_CPT_L;
	MOVLW       176
	MOVWF       TMR3L+0 
;main.c,216 :: 		TMR3IF_bit = 0;
	BCF         TMR3IF_bit+0, BitPos(TMR3IF_bit+0) 
;main.c,217 :: 		}
L_interrupt37:
L_interrupt36:
;main.c,218 :: 		}
L_end_interrupt:
L__interrupt49:
	RETFIE      1
; end of _interrupt
