#include "init.h"

int flag = 0;
int drive_wheels_speed; 
int non_driven_wheels_speed;
int light_control_is_alive = 1;
unsigned char gear = 0b00000010; //initialize gear as Neutral  

void resetLED(void){
	/* turn off leds */
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1; 
}




/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void){
    /* board initialization */
    Init();
    resetLED();
	PIT_ConfigureTimer(0, 100); //Channel 0 Timer
	PIT_ConfigureTimer(1, 200); //Channel 1 Timer
	
	/* Start Timer for channel 0 and channel 1*/
	PIT_StartTimer(0);
	PIT_StartTimer(1); 

    for(;;)
    {

    	/*
    		One button should indicate that the user Gear shifted up. 
    		The other button is for Gear shifter down. 
    	*/

		// Button 5: Gear shifted up
		while(BT5 == 0){
			/* Every bit in this byte represents one gear
			   1 byte equals 8 bits. With 8 bit maximum 128 number can be represented
			   So, highest number is 127
			   Gear is shifted up till 127, until reaching higest number
			*/
			if(gear < 128){  
				gear = gear << 1;
				
			}
		}
		
		//Button 6: shifted down
		while(BT6 == 0){
			/*
				Gear can be shifted down until reaching 0b00000001 or Reverse Gear 
				Reverse Gear is represented with 0b00000001 
			*/
			if(gear > 1){
				gear = gear >> 1;
				
			}
		}

    }
}

/********************************************************************
 *                      Interrupt Functions                         *
 ********************************************************************/  

void PITCHANNEL0(void){
	/* send RPM value.  Maximum range is 1024 (decimal)
	   Two byte needs to send 1024 (decimal)
	*/
	CAN_0.BUF[0].DATA.B[0] = ADC_0.CDR[4].B.CDATA;
	CAN_0.BUF[0].DATA.B[1] =  ADC_0.CDR[4].B.CDATA >> 8;		
	CAN_0.BUF[0].CS.B.CODE = 12; // send 
	
    PIT.CH[0].TFLG.B.TIF = 1;
}


void PITCHANNEL1(void){
	// send gear data
    CAN_0.BUF[1].DATA.B[0] = gear;		
	CAN_0.BUF[1].CS.B.CODE = 12;
	
	// send alive signal
	CAN_0.BUF[3].CS.B.CODE = 12;
	
	// send error signal
	if(light_control_is_alive == 0){
		CAN_0.BUF[4].CS.B.CODE = 12;
		LED7 = ~LED7; //blink LED7
	}

	else{
		light_control_is_alive = 0;
		LED7 = 1;
	}
	

	/*
		The Stability Control receives the speed of the drive wheels and the speed of 
		the non-driven wheels. If the speed difference is bigger than 15 km/h a signal 
		should be sent to the bus.
	*/
	if(flag == 1){
		
		int difference;

		if(drive_wheels_speed > non_driven_wheels_speed){
			difference = drive_wheels_speed - non_driven_wheels_speed;
		}

		else{

			difference = non_driven_wheels_speed - drive_wheels_speed;
		}

		if(difference > 15){
			CAN_0.BUF[2].DATA.B[0] = 0x00;
		}

		else{
			CAN_0.BUF[2].DATA.B[0] = 0x01;
		}

		CAN_0.BUF[2].CS.B.CODE = 12;
		flag = 0;
	}

	else{

		flag = 1;
	}
	
    PIT.CH[1].TFLG.B.TIF = 1;
}

void CANMB0003(void){

/* Receive interrupts are being cleared here */
    CAN_0.IFRL.B.BUF00I = 1;
    CAN_0.IFRL.B.BUF01I = 1;
    CAN_0.IFRL.B.BUF02I = 1;
    CAN_0.IFRL.B.BUF03I = 1;
}


//Receive data from CAN bus which is come from the another ECU
void CANMB0407(void){
	if(CAN_0.IFRL.B.BUF05I){
		switch(CAN_0.RXFIFO.ID.B.STD_ID){
			case 0x302:
				non_driven_wheels_speed = (CAN_0.RXFIFO.DATA.B[1] << 8) | CAN_0.RXFIFO.DATA.B[0];
				LED0 = ~LED0;
				break;
			case 0x502:
				drive_wheels_speed = (CAN_0.RXFIFO.DATA.B[1] << 8) | CAN_0.RXFIFO.DATA.B[0];
				LED1 = ~LED1;
				break;
			case 0x303:
				light_control_is_alive = 1;
				LED2 = ~LED2;
				break;
		}
	}  
   
    /* clear flags as last step here! */
    /* don't change anything below! */
    CAN_0.IFRL.B.BUF04I = 1;
    CAN_0.IFRL.B.BUF05I = 1;
    CAN_0.IFRL.B.BUF06I = 1;
    CAN_0.IFRL.B.BUF07I = 1;
}

void CANMB0811(void){
/* No modifications needed here */
/* transmit interrupts are being cleared here */

    CAN_0.IFRL.B.BUF08I = 1;
    CAN_0.IFRL.B.BUF09I = 1;
    CAN_0.IFRL.B.BUF10I = 1;
    CAN_0.IFRL.B.BUF11I = 1;
}

void CANMB1215(void){
/* No modifications needed here */
/* transmit interrupts are being cleared here */
    CAN_0.IFRL.B.BUF12I = 1;
    CAN_0.IFRL.B.BUF13I = 1;
    CAN_0.IFRL.B.BUF14I = 1;
    CAN_0.IFRL.B.BUF15I = 1;
}



/********************************************************************
 *                   Interrupt Vector Table                         *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {

	//Call function based on ISR number
    switch(INTC.IACKR.B.INTVEC){
        case 59:
            PITCHANNEL0();
            break;
        case 60:
            PITCHANNEL1();
        case 68:
            CANMB0003();
            break;
        case 69:
            CANMB0407();
            break;
        case 70:
            CANMB0811();
            break;
        case 71:
            CANMB1215();
            break;        
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}
