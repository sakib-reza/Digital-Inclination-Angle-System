//*********************************************************************                  
//*                       McMaster University                         *
//*               COMP ENG 2DP4: Microprocessor Systems               *
//*                          Lab Section 05                           * 
//*                   Sakib Reza 400131994 rezas2                     *
//*********************************************************************
//*********************************************************************
//*                          Final Project                            *
//*                 DATA ACQUISITION  AND  DISPLAY:                   *
//*        THE  DIGITAL  INCLINATION  ANGLE  MEASUREMENT  SYSTEM      *
//*********************************************************************

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "SCI.h"
unsigned short val;
float degree;
int button = 0, fDig, sDig, high, low, extra; 

// Prototypes
void setCLK(void); // set bus speed
void setADC(void); // set ADC = AN4
void delay1ms(unsigned int); // to test bus speed, 1ms delay
void OutCRLF(void); // for serial communication


void main(void) {

  setCLK();   // set bus speed to 4 MHz
  setADC();   // set ADC at AN7 with 10-bit resolution
  

  //Set Ports
  DDRJ = 0xFF;      
  DDR1AD = 0b01111111;
  DDR0AD = 0b11111111;
  
             
  TSCR1 = 0x90;    // Timer System Control Register 1, 0x90 = 1001 0000 
                    // TSCR1[7] = TEN:  Timer Enable (0-disable, 1-enable)
                    // TSCR1[6] = TSWAI:  Timer runs during WAI (0-enable, 1-disable)
                    // TSCR1[5] = TSFRZ:  Timer runs during WAI (0-enable, 1-disable)
                    // TSCR1[4] = TFFCA:  Timer Fast Flag Clear All (0-normal 1-read/write clears interrupt flags)
                    // TSCR1[3] = PRT:  Precision Timer (0-legacy, 1-precision)
                    // TSCR1[2:0] not used

  TSCR2 = 0x01;    // Timer System Control Register 2
                    // TSCR2[7] = TOI: Timer Overflow Interrupt Enable (0-inhibited, 1-hardware irq when TOF=1)
                    // TSCR2[6:3] not used
                    // TSCR2[2:0] = Timer Prescaler Select: See Table22-12 of MC9S12G Family Reference Manual r1.25 (set for bus/1)
  
                    
  TIOS = 0xFE;     // Timer Input Capture or Output capture      , 0xFE = 1111 1110 so pin 0 is input-capture, all others are output-compare
                   // set TIC[0] and input (similar to DDR)
  PERT = 0x01;     // Enable Pull-Up resistor on TIC[0]

  TCTL3 = 0x00;    // TCTL3 & TCTL4 configure which edge(s) to capture   (TCTL3 is for pins 7-4, neflect)
  TCTL4 = 0x02;    // Configured for falling edge on TIC[0]              (TCTL4 pins 3-0, pin 2 is set 1 0 = falling edges captured!)
        
   
  TIE = 0x01;      //Timer Interrupt Enable          
  
  // initialize serial communication                      
  SCI_Init(19200);
  //SCI_OutString("Sakib Reza; 400131994");
  //OutCRLF(); 
  
  // enable interrupts
	EnableInterrupts;
	

  for(;;) {
  
    while (button == 0){
      val = ATDDR0;
      
      //refer to excel spreadsheet for these functions
      if (val <= 507.73){
        degree = 0;  
        
      } else if (val <= 535){
        degree = (val-507.73)/1.8981; 
 
      
      }else if (val <= 561){
        degree = (val-507.2)/1.7795; 
 
      
      } else if (val <= 581){
        degree = (val-522.67)/1.3;

      
      } else if (val <= 600) {
        degree = (val-528)/1.2;

        
      } else if (val <= 611) {
        degree = (val-559)/0.7;

        
      } else if (val <= 618) {
        degree = (val - 554.81)/0.696;

        
      } else {
        degree = 90; 
       
      }
     
     
      fDig = degree / 10;
      sDig = degree - (fDig * 10);
      
      //first set of LEDs  
      if(fDig == 0){
        high = 0b00000000;
        extra = 0b00000000;
          
      }
      
      if(fDig == 1){
        high = 0b00010000;
        extra = 0b00000000; 
      }
      
      if(fDig == 2){
        high = 0b00100000;
        extra = 0b00000000;
          
      }
      
      if(fDig == 3){
        high = 0b00110000;
        extra = 0b00000000;
      
      }
      
      if(fDig == 4){
        high = 0b01000000;
        extra = 0b00000000;
          
      }
      
      if(fDig == 5){
        high = 0b01010000;
        extra = 0b00000000;
          
      }
      
      if(fDig == 6){
        high = 0b01100000;
        extra = 0b00000000;
          
      }
      
      if(fDig == 7){
        high = 0b01110000;
        extra = 0b00000000;
          
      }
      
      if(fDig == 8){
        high = 0b00000000;
        extra = 0b00000001;
          
      }
      
      if(fDig == 9){
        high = 0b00010000;
        extra = 0b00000001;
      }
      //second set of LEDS  
      if(sDig == 0){
        low = 0b00000000;
          
      }
      
      if(sDig == 1){
        low = 0b00000001;
      }
      
      if(sDig == 2){
        low = 0b00000010;
        
      }
      
      if(sDig == 3){
        low = 0b00000011;
          
      }
      
      if(sDig == 4){
        low = 0b00000100;
          
      }
      
      if(sDig == 5){
        low = 0b00000101;
          
      }
      
      if(sDig == 6){
        low = 0b00000110;
         
      }
      
      if(sDig == 7){
        low = 0b00000111;
          
      }
      
      if(sDig == 8){
        low = 0b00001000;
          
      }
      
      if(sDig == 9){
        low = 0b00001001;
      }
        
      PT1AD = high|low;
      PT0AD = extra;
     
      //SCI_OutUDec(val);
      //OutCRLF();
     
      SCI_OutUDec(degree);
      OutCRLF();
      delay1ms(100);
    }
    
    
    while (button == 1){
      val = ATDDR0;
      
      // refer to excel spreadsheet for these functions
      if (val <= 506){
        degree = 0;  
        
      } else if (val <= 535){
        degree = (val-507.73)/1.8981; 
 
      
      }else if (val <= 561){
        degree = (val-507.2)/1.7795; 
 
      
      } else if (val <= 581){
        degree = (val-522.67)/1.3;

      
      } else if (val <= 600) {
        degree = (val-528)/1.2;

        
      } else if (val <= 611) {
        degree = (val-559)/0.7;

        
      } else if (val <= 620) {
        degree = (val - 556.81)/0.696;

        
      } else {
        degree = 90; 
       
      }
     
      
      if (degree <= 5){
          PT1AD = 0b00000000;
          PT0AD = 0b00000000;
          
      } else if (degree <= 15){
          PT1AD = 0b00000001;
          PT0AD = 0b00000000;
      
      } else if (degree <= 25){
          PT1AD = 0b00000011;
          PT0AD = 0b00000000;
      
      } else if (degree <= 35){
          PT1AD = 0b00000111;
          PT0AD = 0b00000000;
          
      } else if (degree <= 45){
          PT1AD = 0b00001111;
          PT0AD = 0b00000000;
          
      } else if(degree <= 55){
          PT1AD = 0b00011111;
          PT0AD = 0b00000000;
          
      } else if(degree <= 65){
          PT1AD = 0b00111111;
          PT0AD = 0b00000000;
          
      } else if(degree <= 75){
          PT1AD = 0b01111111;
          PT0AD = 0b00000000;
          
      } else if(degree <= 85){
          PT1AD = 0b01111111;
          PT0AD = 0b00000001;
          
      } else if(degree <= 90){
          PT1AD = 0b01111111;
          PT0AD = 0b00001001  ;
      }
      
      //SCI_OutUDec(val);
      //OutCRLF();
      
      SCI_OutUDec(degree);
      OutCRLF();
      delay1ms(100);
    }
    
  }
}

           
interrupt  VectorNumber_Vtimch0 void ISR_Vtimch0(void) {    // interrupt for PT0 built into CodeWarrior
  unsigned int temp; // used to enable FastFlagClearing
             
  if (button == 0){ // changing between modes & flashing onboard LED to know it worked
    button = 1; 
    PTJ = 0x01;
  } else {
    button = 0;
    PTJ = 0x00;
  }
  
	//SCI_OutString("The new mode is:");
	//SCI_OutUDec (button);
	//OutCRLF();
	
  temp = TC0;       //Allow another TIC interrupt
}  

// functions

void setCLK(void) {  // Bus Speed = 4 MHz
  CPMUPROT = 0x26; // protection of clock config is enabled
  CPMUCLKS = 0x80; // choose PLL = 1 MHz
  CPMUOSC = 0x00;
  CPMUSYNR = 0x0F; // VCOFRQ = 00, SYNDIN = 15 => VCLOCK = 2*1MHz*(1+SYNDIV) = 32MHz;
  CPMUFLG = 0x00; // LOCK = 0: PLLCLK = 32MHz/4 = 8MHz & BUSCLK = PLLCLK/2 = 4MHz
    
}

void setADC(void) {     // ADC at AN7 with 10-BIT resolution		
	ATDCTL1 = 0x2F; // setting to 10-bit resolution
	ATDCTL3 = 0x88;	// right justified, one sample per sequence   
	ATDCTL4 = 0x01;	// prescaler = 1; ATD clock = 4MHz / (2 * (2 + 1)) == 1MHz      
	ATDCTL5 = 0x27;	// conversion occurs at AN7 
}

void delay1ms(unsigned int k) {   // function modified from Lab Code to set a k ms delay at 4 Mhz bus speed
  int ix; 
  TSCR1 = 0x90; // enable timer and fast timer flag clear
  TSCR2 = 0x00; // disabling timer interrupt
  TIOS |= 0x02; 
  TC1 = TCNT + 4000; // (1/4MHz)*4000 = 1ms
  
  for(ix=0;ix<k;ix++) {
    while(!(TFLG1_C1F));
     TC1 += 4000; 
  }
  TIOS &= ~0x01;  
}

void OutCRLF(void){   // newline function for serial communication
  SCI_OutChar(CR);
  SCI_OutChar(LF);
  PTJ ^= 0x20;
}

