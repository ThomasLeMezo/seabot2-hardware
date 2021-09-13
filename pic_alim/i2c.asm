
_i2c_read_data_from_buffer:

;i2c.c,10 :: 		void i2c_read_data_from_buffer(){
;i2c.c,11 :: 		short i = 0;
	CLRF        i2c_read_data_from_buffer_i_L0+0 
;i2c.c,13 :: 		for(i=0; i<(nb_rx_octet-1); i++){
	CLRF        i2c_read_data_from_buffer_i_L0+0 
L_i2c_read_data_from_buffer0:
	DECF        _nb_rx_octet+0, 0 
	MOVWF       R1 
	CLRF        R2 
	MOVLW       0
	SUBWFB      R2, 1 
	MOVLW       128
	BTFSC       i2c_read_data_from_buffer_i_L0+0, 7 
	MOVLW       127
	MOVWF       R0 
	MOVLW       128
	XORWF       R2, 0 
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_read_data_from_buffer54
	MOVF        R1, 0 
	SUBWF       i2c_read_data_from_buffer_i_L0+0, 0 
L__i2c_read_data_from_buffer54:
	BTFSC       STATUS+0, 0 
	GOTO        L_i2c_read_data_from_buffer1
;i2c.c,14 :: 		switch(rxbuffer_tab[0]+i){
	MOVF        i2c_read_data_from_buffer_i_L0+0, 0 
	ADDWF       _rxbuffer_tab+0, 0 
	MOVWF       R4 
	CLRF        R5 
	MOVLW       0
	BTFSC       i2c_read_data_from_buffer_i_L0+0, 7 
	MOVLW       255
	ADDWFC      R5, 1 
	GOTO        L_i2c_read_data_from_buffer3
;i2c.c,15 :: 		case 0x00:  // alimentation
L_i2c_read_data_from_buffer5:
;i2c.c,16 :: 		switch(rxbuffer_tab[i+1]){
	MOVF        i2c_read_data_from_buffer_i_L0+0, 0 
	ADDLW       1
	MOVWF       R0 
	CLRF        R1 
	MOVLW       0
	BTFSC       i2c_read_data_from_buffer_i_L0+0, 7 
	MOVLW       255
	ADDWFC      R1, 1 
	MOVLW       _rxbuffer_tab+0
	ADDWF       R0, 0 
	MOVWF       R2 
	MOVLW       hi_addr(_rxbuffer_tab+0)
	ADDWFC      R1, 0 
	MOVWF       R3 
	GOTO        L_i2c_read_data_from_buffer6
;i2c.c,17 :: 		case 0x01:
L_i2c_read_data_from_buffer8:
;i2c.c,18 :: 		state = POWER_ON;
	MOVLW       1
	MOVWF       _state+0 
;i2c.c,19 :: 		break;
	GOTO        L_i2c_read_data_from_buffer7
;i2c.c,20 :: 		case 0x02:
L_i2c_read_data_from_buffer9:
;i2c.c,21 :: 		time_to_stop = default_time_to_stop;  // Go to Sleep mode
	MOVF        _default_time_to_stop+0, 0 
	MOVWF       _time_to_stop+0 
;i2c.c,22 :: 		start_time_to_stop = 1;
	MOVLW       1
	MOVWF       _start_time_to_stop+0 
;i2c.c,23 :: 		state = WAIT_TO_SLEEP;
	MOVLW       2
	MOVWF       _state+0 
;i2c.c,24 :: 		break;
	GOTO        L_i2c_read_data_from_buffer7
;i2c.c,25 :: 		default:
L_i2c_read_data_from_buffer10:
;i2c.c,26 :: 		break;
	GOTO        L_i2c_read_data_from_buffer7
;i2c.c,27 :: 		}
L_i2c_read_data_from_buffer6:
	MOVFF       R2, FSR0L+0
	MOVFF       R3, FSR0H+0
	MOVF        POSTINC0+0, 0 
	MOVWF       R1 
	MOVF        R1, 0 
	XORLW       1
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_read_data_from_buffer8
	MOVFF       R2, FSR0L+0
	MOVFF       R3, FSR0H+0
	MOVF        POSTINC0+0, 0 
	MOVWF       R1 
	MOVF        R1, 0 
	XORLW       2
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_read_data_from_buffer9
	GOTO        L_i2c_read_data_from_buffer10
L_i2c_read_data_from_buffer7:
;i2c.c,28 :: 		i++;
	INCF        i2c_read_data_from_buffer_i_L0+0, 1 
;i2c.c,29 :: 		break;
	GOTO        L_i2c_read_data_from_buffer4
;i2c.c,30 :: 		case 0x03:
L_i2c_read_data_from_buffer11:
;i2c.c,31 :: 		default_time_to_start[0] = rxbuffer_tab[i+1]; // hours
	MOVF        i2c_read_data_from_buffer_i_L0+0, 0 
	ADDLW       1
	MOVWF       R0 
	CLRF        R1 
	MOVLW       0
	BTFSC       i2c_read_data_from_buffer_i_L0+0, 7 
	MOVLW       255
	ADDWFC      R1, 1 
	MOVLW       _rxbuffer_tab+0
	ADDWF       R0, 0 
	MOVWF       FSR0L+0 
	MOVLW       hi_addr(_rxbuffer_tab+0)
	ADDWFC      R1, 0 
	MOVWF       FSR0L+1 
	MOVF        POSTINC0+0, 0 
	MOVWF       R0 
	MOVF        R0, 0 
	MOVWF       _default_time_to_start+0 
;i2c.c,32 :: 		break;
	GOTO        L_i2c_read_data_from_buffer4
;i2c.c,33 :: 		case 0x04:
L_i2c_read_data_from_buffer12:
;i2c.c,34 :: 		default_time_to_start[1] = rxbuffer_tab[i+1]; // min
	MOVF        i2c_read_data_from_buffer_i_L0+0, 0 
	ADDLW       1
	MOVWF       R0 
	CLRF        R1 
	MOVLW       0
	BTFSC       i2c_read_data_from_buffer_i_L0+0, 7 
	MOVLW       255
	ADDWFC      R1, 1 
	MOVLW       _rxbuffer_tab+0
	ADDWF       R0, 0 
	MOVWF       FSR0L+0 
	MOVLW       hi_addr(_rxbuffer_tab+0)
	ADDWFC      R1, 0 
	MOVWF       FSR0L+1 
	MOVF        POSTINC0+0, 0 
	MOVWF       R0 
	MOVF        R0, 0 
	MOVWF       _default_time_to_start+1 
;i2c.c,35 :: 		break;
	GOTO        L_i2c_read_data_from_buffer4
;i2c.c,36 :: 		case 0x05:
L_i2c_read_data_from_buffer13:
;i2c.c,37 :: 		default_time_to_start[2] = rxbuffer_tab[i+1]; // sec
	MOVF        i2c_read_data_from_buffer_i_L0+0, 0 
	ADDLW       1
	MOVWF       R0 
	CLRF        R1 
	MOVLW       0
	BTFSC       i2c_read_data_from_buffer_i_L0+0, 7 
	MOVLW       255
	ADDWFC      R1, 1 
	MOVLW       _rxbuffer_tab+0
	ADDWF       R0, 0 
	MOVWF       FSR0L+0 
	MOVLW       hi_addr(_rxbuffer_tab+0)
	ADDWFC      R1, 0 
	MOVWF       FSR0L+1 
	MOVF        POSTINC0+0, 0 
	MOVWF       R0 
	MOVF        R0, 0 
	MOVWF       _default_time_to_start+2 
;i2c.c,38 :: 		break;
	GOTO        L_i2c_read_data_from_buffer4
;i2c.c,39 :: 		case 0x06:
L_i2c_read_data_from_buffer14:
;i2c.c,40 :: 		default_time_to_stop = rxbuffer_tab[i+1]; // sec
	MOVF        i2c_read_data_from_buffer_i_L0+0, 0 
	ADDLW       1
	MOVWF       R0 
	CLRF        R1 
	MOVLW       0
	BTFSC       i2c_read_data_from_buffer_i_L0+0, 7 
	MOVLW       255
	ADDWFC      R1, 1 
	MOVLW       _rxbuffer_tab+0
	ADDWF       R0, 0 
	MOVWF       FSR0L+0 
	MOVLW       hi_addr(_rxbuffer_tab+0)
	ADDWFC      R1, 0 
	MOVWF       FSR0L+1 
	MOVF        POSTINC0+0, 0 
	MOVWF       R0 
	MOVF        R0, 0 
	MOVWF       _default_time_to_stop+0 
;i2c.c,41 :: 		break;
	GOTO        L_i2c_read_data_from_buffer4
;i2c.c,42 :: 		case 0x07:
L_i2c_read_data_from_buffer15:
;i2c.c,43 :: 		watchdog_restart_default = rxbuffer_tab[i+1];
	MOVF        i2c_read_data_from_buffer_i_L0+0, 0 
	ADDLW       1
	MOVWF       R0 
	CLRF        R1 
	MOVLW       0
	BTFSC       i2c_read_data_from_buffer_i_L0+0, 7 
	MOVLW       255
	ADDWFC      R1, 1 
	MOVLW       _rxbuffer_tab+0
	ADDWF       R0, 0 
	MOVWF       FSR0L+0 
	MOVLW       hi_addr(_rxbuffer_tab+0)
	ADDWFC      R1, 0 
	MOVWF       FSR0L+1 
	MOVF        POSTINC0+0, 0 
	MOVWF       R0 
	MOVF        R0, 0 
	MOVWF       _watchdog_restart_default+0 
;i2c.c,44 :: 		watchdog_restart = watchdog_restart_default;
	MOVF        _watchdog_restart_default+0, 0 
	MOVWF       _watchdog_restart+0 
;i2c.c,45 :: 		break;
	GOTO        L_i2c_read_data_from_buffer4
;i2c.c,46 :: 		default:
L_i2c_read_data_from_buffer16:
;i2c.c,47 :: 		break;
	GOTO        L_i2c_read_data_from_buffer4
;i2c.c,48 :: 		}
L_i2c_read_data_from_buffer3:
	MOVLW       0
	XORWF       R5, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_read_data_from_buffer55
	MOVLW       0
	XORWF       R4, 0 
L__i2c_read_data_from_buffer55:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_read_data_from_buffer5
	MOVLW       0
	XORWF       R5, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_read_data_from_buffer56
	MOVLW       3
	XORWF       R4, 0 
L__i2c_read_data_from_buffer56:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_read_data_from_buffer11
	MOVLW       0
	XORWF       R5, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_read_data_from_buffer57
	MOVLW       4
	XORWF       R4, 0 
L__i2c_read_data_from_buffer57:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_read_data_from_buffer12
	MOVLW       0
	XORWF       R5, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_read_data_from_buffer58
	MOVLW       5
	XORWF       R4, 0 
L__i2c_read_data_from_buffer58:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_read_data_from_buffer13
	MOVLW       0
	XORWF       R5, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_read_data_from_buffer59
	MOVLW       6
	XORWF       R4, 0 
L__i2c_read_data_from_buffer59:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_read_data_from_buffer14
	MOVLW       0
	XORWF       R5, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_read_data_from_buffer60
	MOVLW       7
	XORWF       R4, 0 
L__i2c_read_data_from_buffer60:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_read_data_from_buffer15
	GOTO        L_i2c_read_data_from_buffer16
L_i2c_read_data_from_buffer4:
;i2c.c,13 :: 		for(i=0; i<(nb_rx_octet-1); i++){
	INCF        i2c_read_data_from_buffer_i_L0+0, 1 
;i2c.c,49 :: 		}
	GOTO        L_i2c_read_data_from_buffer0
L_i2c_read_data_from_buffer1:
;i2c.c,50 :: 		}
L_end_i2c_read_data_from_buffer:
	RETURN      0
; end of _i2c_read_data_from_buffer

_i2c_write_data_to_buffer:

;i2c.c,52 :: 		void i2c_write_data_to_buffer(const unsigned short nb_tx_octet){
;i2c.c,53 :: 		switch(rxbuffer_tab[0]+nb_tx_octet){
	MOVF        FARG_i2c_write_data_to_buffer_nb_tx_octet+0, 0 
	ADDWF       _rxbuffer_tab+0, 0 
	MOVWF       R3 
	CLRF        R4 
	MOVLW       0
	ADDWFC      R4, 1 
	GOTO        L_i2c_write_data_to_buffer17
;i2c.c,54 :: 		case 0x00:
L_i2c_write_data_to_buffer19:
;i2c.c,55 :: 		SSPBUF = battery_voltage[0];
	MOVF        _battery_voltage+0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,56 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,57 :: 		case 0x01:
L_i2c_write_data_to_buffer20:
;i2c.c,58 :: 		SSPBUF = battery_voltage[0] >> 8;
	MOVF        _battery_voltage+1, 0 
	MOVWF       R0 
	CLRF        R1 
	MOVF        R0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,59 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,60 :: 		case 0x02:
L_i2c_write_data_to_buffer21:
;i2c.c,61 :: 		SSPBUF = battery_voltage[1];
	MOVF        _battery_voltage+2, 0 
	MOVWF       SSPBUF+0 
;i2c.c,62 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,63 :: 		case 0x03:
L_i2c_write_data_to_buffer22:
;i2c.c,64 :: 		SSPBUF = battery_voltage[1] >> 8;
	MOVF        _battery_voltage+3, 0 
	MOVWF       R0 
	CLRF        R1 
	MOVF        R0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,65 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,66 :: 		case 0x04:
L_i2c_write_data_to_buffer23:
;i2c.c,67 :: 		SSPBUF = battery_voltage[2];
	MOVF        _battery_voltage+4, 0 
	MOVWF       SSPBUF+0 
;i2c.c,68 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,69 :: 		case 0x05:
L_i2c_write_data_to_buffer24:
;i2c.c,70 :: 		SSPBUF = battery_voltage[2] >> 8;
	MOVF        _battery_voltage+5, 0 
	MOVWF       R0 
	CLRF        R1 
	MOVF        R0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,71 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,72 :: 		case 0x06:
L_i2c_write_data_to_buffer25:
;i2c.c,73 :: 		SSPBUF = battery_voltage[3];
	MOVF        _battery_voltage+6, 0 
	MOVWF       SSPBUF+0 
;i2c.c,74 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,75 :: 		case 0x07:
L_i2c_write_data_to_buffer26:
;i2c.c,76 :: 		SSPBUF = battery_voltage[3] >> 8;
	MOVF        _battery_voltage+7, 0 
	MOVWF       R0 
	CLRF        R1 
	MOVF        R0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,77 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,78 :: 		case 0x08:
L_i2c_write_data_to_buffer27:
;i2c.c,79 :: 		SSPBUF = power_current[0];
	MOVF        _power_current+0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,80 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,81 :: 		case 0x09:
L_i2c_write_data_to_buffer28:
;i2c.c,82 :: 		SSPBUF = power_current[0] >> 8;
	MOVF        _power_current+1, 0 
	MOVWF       R0 
	CLRF        R1 
	MOVF        R0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,83 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,84 :: 		case 0x0A:
L_i2c_write_data_to_buffer29:
;i2c.c,85 :: 		SSPBUF = power_current[1];
	MOVF        _power_current+2, 0 
	MOVWF       SSPBUF+0 
;i2c.c,86 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,87 :: 		case 0x0B:
L_i2c_write_data_to_buffer30:
;i2c.c,88 :: 		SSPBUF = power_current[1] >> 8;
	MOVF        _power_current+3, 0 
	MOVWF       R0 
	CLRF        R1 
	MOVF        R0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,89 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,90 :: 		case 0x0C:
L_i2c_write_data_to_buffer31:
;i2c.c,91 :: 		SSPBUF = power_current[2];
	MOVF        _power_current+4, 0 
	MOVWF       SSPBUF+0 
;i2c.c,92 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,93 :: 		case 0x0D:
L_i2c_write_data_to_buffer32:
;i2c.c,94 :: 		SSPBUF = power_current[2] >> 8;
	MOVF        _power_current+5, 0 
	MOVWF       R0 
	CLRF        R1 
	MOVF        R0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,95 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,97 :: 		case 0xB0:
L_i2c_write_data_to_buffer33:
;i2c.c,98 :: 		SSPBUF = state;
	MOVF        _state+0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,99 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,100 :: 		case 0xB1:
L_i2c_write_data_to_buffer34:
;i2c.c,101 :: 		SSPBUF = watchdog_restart_default;
	MOVF        _watchdog_restart_default+0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,102 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,103 :: 		case 0xB2:
L_i2c_write_data_to_buffer35:
;i2c.c,104 :: 		SSPBUF = ILS;
	MOVLW       0
	BTFSC       PORTA+0, 2 
	MOVLW       1
	MOVWF       SSPBUF+0 
;i2c.c,105 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,107 :: 		case 0xC0:
L_i2c_write_data_to_buffer36:
;i2c.c,108 :: 		SSPBUF = CODE_VERSION;
	MOVLW       1
	MOVWF       SSPBUF+0 
;i2c.c,109 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,110 :: 		case 0xC1:
L_i2c_write_data_to_buffer37:
;i2c.c,111 :: 		SSPBUF = is_init;
	MOVF        _is_init+0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,112 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,113 :: 		default:
L_i2c_write_data_to_buffer38:
;i2c.c,114 :: 		SSPBUF = 0x00;
	CLRF        SSPBUF+0 
;i2c.c,115 :: 		break;
	GOTO        L_i2c_write_data_to_buffer18
;i2c.c,116 :: 		}
L_i2c_write_data_to_buffer17:
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer62
	MOVLW       0
	XORWF       R3, 0 
L__i2c_write_data_to_buffer62:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer19
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer63
	MOVLW       1
	XORWF       R3, 0 
L__i2c_write_data_to_buffer63:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer20
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer64
	MOVLW       2
	XORWF       R3, 0 
L__i2c_write_data_to_buffer64:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer21
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer65
	MOVLW       3
	XORWF       R3, 0 
L__i2c_write_data_to_buffer65:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer22
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer66
	MOVLW       4
	XORWF       R3, 0 
L__i2c_write_data_to_buffer66:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer23
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer67
	MOVLW       5
	XORWF       R3, 0 
L__i2c_write_data_to_buffer67:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer24
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer68
	MOVLW       6
	XORWF       R3, 0 
L__i2c_write_data_to_buffer68:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer25
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer69
	MOVLW       7
	XORWF       R3, 0 
L__i2c_write_data_to_buffer69:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer26
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer70
	MOVLW       8
	XORWF       R3, 0 
L__i2c_write_data_to_buffer70:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer27
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer71
	MOVLW       9
	XORWF       R3, 0 
L__i2c_write_data_to_buffer71:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer28
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer72
	MOVLW       10
	XORWF       R3, 0 
L__i2c_write_data_to_buffer72:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer29
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer73
	MOVLW       11
	XORWF       R3, 0 
L__i2c_write_data_to_buffer73:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer30
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer74
	MOVLW       12
	XORWF       R3, 0 
L__i2c_write_data_to_buffer74:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer31
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer75
	MOVLW       13
	XORWF       R3, 0 
L__i2c_write_data_to_buffer75:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer32
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer76
	MOVLW       176
	XORWF       R3, 0 
L__i2c_write_data_to_buffer76:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer33
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer77
	MOVLW       177
	XORWF       R3, 0 
L__i2c_write_data_to_buffer77:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer34
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer78
	MOVLW       178
	XORWF       R3, 0 
L__i2c_write_data_to_buffer78:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer35
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer79
	MOVLW       192
	XORWF       R3, 0 
L__i2c_write_data_to_buffer79:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer36
	MOVLW       0
	XORWF       R4, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__i2c_write_data_to_buffer80
	MOVLW       193
	XORWF       R3, 0 
L__i2c_write_data_to_buffer80:
	BTFSC       STATUS+0, 2 
	GOTO        L_i2c_write_data_to_buffer37
	GOTO        L_i2c_write_data_to_buffer38
L_i2c_write_data_to_buffer18:
;i2c.c,117 :: 		watchdog_restart = watchdog_restart_default;
	MOVF        _watchdog_restart_default+0, 0 
	MOVWF       _watchdog_restart+0 
;i2c.c,118 :: 		}
L_end_i2c_write_data_to_buffer:
	RETURN      0
; end of _i2c_write_data_to_buffer

_i2c_write_buffer:

;i2c.c,120 :: 		void i2c_write_buffer(unsigned short val){
;i2c.c,121 :: 		do
L_i2c_write_buffer39:
;i2c.c,123 :: 		SSPCON1.WCOL = 0;
	BCF         SSPCON1+0, 7 
;i2c.c,124 :: 		SSPBUF = val;
	MOVF        FARG_i2c_write_buffer_val+0, 0 
	MOVWF       SSPBUF+0 
;i2c.c,126 :: 		while(SSPCON1.WCOL);
	BTFSC       SSPCON1+0, 7 
	GOTO        L_i2c_write_buffer39
;i2c.c,127 :: 		}
L_end_i2c_write_buffer:
	RETURN      0
; end of _i2c_write_buffer

_init_i2c:

;i2c.c,129 :: 		void init_i2c(const unsigned short address_i2c){
;i2c.c,132 :: 		TRISB4_bit = 1; // RB4 en entrée
	BSF         TRISB4_bit+0, BitPos(TRISB4_bit+0) 
;i2c.c,133 :: 		TRISB6_bit = 1; // RB6 en entrée
	BSF         TRISB6_bit+0, BitPos(TRISB6_bit+0) 
;i2c.c,136 :: 		PIE1.SSPIE = 1; // Synchronous Serial Port Interrupt Enable bit
	BSF         PIE1+0, 3 
;i2c.c,137 :: 		PIR1.SSPIF = 0; // Synchronous Serial Port (SSP) Interrupt Flag, I2C Slave
	BCF         PIR1+0, 3 
;i2c.c,139 :: 		PIR2.BCLIE = 1;
	BSF         PIR2+0, 3 
;i2c.c,140 :: 		PIR2.BCLIF = 0;
	BCF         PIR2+0, 3 
;i2c.c,143 :: 		SSPADD = (address_i2c << 1); // Address Register, Get address (7-1 bit). Lsb is read/write flag
	MOVF        FARG_init_i2c_address_i2c+0, 0 
	MOVWF       R0 
	RLCF        R0, 1 
	BCF         R0, 0 
	MOVF        R0, 0 
	MOVWF       SSPADD+0 
;i2c.c,144 :: 		SSPMSK = 0xFF; // A zero (‘0’) bit in the SSPMSK register has the effect of making
	MOVLW       255
	MOVWF       SSPMSK+0 
;i2c.c,148 :: 		SSPSTAT.SMP = 1; // Slew Rate Control bit
	BSF         SSPSTAT+0, 7 
;i2c.c,151 :: 		SSPSTAT.CKE = 1; // // SMBusTM Select bit (1 = Enable SMBus specific inputs)
	BSF         SSPSTAT+0, 6 
;i2c.c,154 :: 		SSPCON2 = 0x00;
	CLRF        SSPCON2+0 
;i2c.c,155 :: 		SSPCON2.GCEN = 0; // General Call Enable bit (0 = disabled)
	BCF         SSPCON2+0, 7 
;i2c.c,156 :: 		SSPCON2.SEN = 0; // Start Condition Enable/Stretch Enable bit (1 = enabled)
	BCF         SSPCON2+0, 0 
;i2c.c,159 :: 		SSPCON1.WCOL = 0; // Write Collision Detect bit
	BCF         SSPCON1+0, 7 
;i2c.c,160 :: 		SSPCON1.SSPOV = 0; // Receive Overflow Indicator bit
	BCF         SSPCON1+0, 6 
;i2c.c,161 :: 		SSPCON1.CKP = 1; // SCK Release Control bit (1=Release clock)
	BSF         SSPCON1+0, 4 
;i2c.c,162 :: 		SSPCON1.SSPM3 = 0b0; // I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled (1-> with S/P, 0 -> without)
	BCF         SSPCON1+0, 3 
;i2c.c,163 :: 		SSPCON1.SSPM2 = 0b1; // I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled
	BSF         SSPCON1+0, 2 
;i2c.c,164 :: 		SSPCON1.SSPM1 = 0b1; // I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled
	BSF         SSPCON1+0, 1 
;i2c.c,165 :: 		SSPCON1.SSPM0 = 0b0; // I2C Slave mode, 7-bit address with Start and Stop bit interrupts enabled
	BCF         SSPCON1+0, 0 
;i2c.c,168 :: 		SSPCON1.SSPEN = 1; // Synchronous Serial Port Enable bit
	BSF         SSPCON1+0, 5 
;i2c.c,169 :: 		}
L_end_init_i2c:
	RETURN      0
; end of _init_i2c

_interrupt_low:
	MOVWF       ___Low_saveWREG+0 
	MOVF        STATUS+0, 0 
	MOVWF       ___Low_saveSTATUS+0 
	MOVF        BSR+0, 0 
	MOVWF       ___Low_saveBSR+0 

;i2c.c,171 :: 		void interrupt_low(){
;i2c.c,172 :: 		unsigned char state = 0;
	CLRF        interrupt_low_state_L0+0 
;i2c.c,174 :: 		if (PIR1.SSPIF){  // I2C Interrupt
	BTFSS       PIR1+0, 3 
	GOTO        L_interrupt_low42
;i2c.c,176 :: 		if(SSPSTAT.S){
	BTFSS       SSPSTAT+0, 3 
	GOTO        L_interrupt_low43
;i2c.c,177 :: 		state  = SSPSTAT & MASTER_MASK;
	MOVLW       37
	ANDWF       SSPSTAT+0, 0 
	MOVWF       interrupt_low_state_L0+0 
;i2c.c,179 :: 		switch (state){
	GOTO        L_interrupt_low44
;i2c.c,180 :: 		case MASTER_READ_ADD:
L_interrupt_low46:
;i2c.c,181 :: 		case MASTER_READ_ADD2:
L_interrupt_low47:
;i2c.c,182 :: 		nb_tx_octet = 0;
	CLRF        _nb_tx_octet+0 
;i2c.c,183 :: 		tmp_rx = SSPBUF;
	MOVF        SSPBUF+0, 0 
	MOVWF       _tmp_rx+0 
;i2c.c,186 :: 		case MASTER_READ_DATA:
L_interrupt_low48:
;i2c.c,187 :: 		i2c_write_data_to_buffer(nb_tx_octet);
	MOVF        _nb_tx_octet+0, 0 
	MOVWF       FARG_i2c_write_data_to_buffer_nb_tx_octet+0 
	CALL        _i2c_write_data_to_buffer+0, 0
;i2c.c,188 :: 		nb_tx_octet++;
	MOVF        _nb_tx_octet+0, 0 
	ADDLW       1
	MOVWF       R0 
	MOVF        R0, 0 
	MOVWF       _nb_tx_octet+0 
;i2c.c,189 :: 		break;
	GOTO        L_interrupt_low45
;i2c.c,191 :: 		case MASTER_WRITE_ADD:
L_interrupt_low49:
;i2c.c,192 :: 		nb_rx_octet = 0;
	CLRF        _nb_rx_octet+0 
;i2c.c,193 :: 		tmp_rx = SSPBUF;
	MOVF        SSPBUF+0, 0 
	MOVWF       _tmp_rx+0 
;i2c.c,194 :: 		break;
	GOTO        L_interrupt_low45
;i2c.c,196 :: 		case MASTER_WRITE_DATA:
L_interrupt_low50:
;i2c.c,197 :: 		if(nb_rx_octet < SIZE_RX_BUFFER){
	MOVLW       8
	SUBWF       _nb_rx_octet+0, 0 
	BTFSC       STATUS+0, 0 
	GOTO        L_interrupt_low51
;i2c.c,198 :: 		rxbuffer_tab[nb_rx_octet] = SSPBUF;
	MOVLW       _rxbuffer_tab+0
	MOVWF       FSR1L+0 
	MOVLW       hi_addr(_rxbuffer_tab+0)
	MOVWF       FSR1L+1 
	MOVF        _nb_rx_octet+0, 0 
	ADDWF       FSR1L+0, 1 
	BTFSC       STATUS+0, 0 
	INCF        FSR1L+1, 1 
	MOVF        SSPBUF+0, 0 
	MOVWF       POSTINC1+0 
;i2c.c,199 :: 		nb_rx_octet++;
	MOVF        _nb_rx_octet+0, 0 
	ADDLW       1
	MOVWF       R0 
	MOVF        R0, 0 
	MOVWF       _nb_rx_octet+0 
;i2c.c,200 :: 		}
	GOTO        L_interrupt_low52
L_interrupt_low51:
;i2c.c,202 :: 		tmp_rx = SSPBUF;
	MOVF        SSPBUF+0, 0 
	MOVWF       _tmp_rx+0 
L_interrupt_low52:
;i2c.c,203 :: 		break;
	GOTO        L_interrupt_low45
;i2c.c,204 :: 		}
L_interrupt_low44:
	MOVF        interrupt_low_state_L0+0, 0 
	XORLW       4
	BTFSC       STATUS+0, 2 
	GOTO        L_interrupt_low46
	MOVF        interrupt_low_state_L0+0, 0 
	XORLW       5
	BTFSC       STATUS+0, 2 
	GOTO        L_interrupt_low47
	MOVF        interrupt_low_state_L0+0, 0 
	XORLW       36
	BTFSC       STATUS+0, 2 
	GOTO        L_interrupt_low48
	MOVF        interrupt_low_state_L0+0, 0 
	XORLW       1
	BTFSC       STATUS+0, 2 
	GOTO        L_interrupt_low49
	MOVF        interrupt_low_state_L0+0, 0 
	XORLW       33
	BTFSC       STATUS+0, 2 
	GOTO        L_interrupt_low50
L_interrupt_low45:
;i2c.c,205 :: 		}
L_interrupt_low43:
;i2c.c,207 :: 		SSPCON1.CKP = 1;
	BSF         SSPCON1+0, 4 
;i2c.c,208 :: 		PIR1.SSPIF = 0; // reset SSP interrupt flag
	BCF         PIR1+0, 3 
;i2c.c,209 :: 		}
L_interrupt_low42:
;i2c.c,210 :: 		}
L_end_interrupt_low:
L__interrupt_low84:
	MOVF        ___Low_saveBSR+0, 0 
	MOVWF       BSR+0 
	MOVF        ___Low_saveSTATUS+0, 0 
	MOVWF       STATUS+0 
	SWAPF       ___Low_saveWREG+0, 1 
	SWAPF       ___Low_saveWREG+0, 0 
	RETFIE      0
; end of _interrupt_low

i2c____?ag:

L_end_i2c___?ag:
	RETURN      0
; end of i2c____?ag
