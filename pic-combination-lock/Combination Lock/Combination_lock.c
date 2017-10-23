/*******************************************
Keypad and LCD lock on Pic16f628A with 3*4 keypad
Project name: Combination Lock (Demonstration of the Keypad Library routines)
LCD Display in 4-bit mode on PORTA
Keypad on PORTB

Door1 = 05
Door2 = 28
Door3 = 82
Door1 & 2 = 33

Three Attempts before Alarm
Designed: By Ky Dat Nguyen (Student)
From: Australia (Victoria University)
year:2007
Course: EBES
*****************************************/

unsigned char var1, var2, kp;
char attempts=0;
char state = 1;//start from state 1
char txt[4];

char          text[] = "Combination Lock" ;//message text
char          text1[]= " *Door1 On*";
char          text2[]= " *Door2 On*";
char          text3[]= "*Door1&2 On*";

char table[17] = {0,1,2,3,0,4,5,6,0,7,8,9,0,10,0,11,0};

void main(void) {

  CMCON = 0x07                     ;//PORTA to digital mode
  LCD_Config(&PORTA,7,6,5,3,2,1,0) ;//Pin assignment
                                   ;//RS,E,RW,D7,D6,D5,D4
  LCD_Cmd(LCD_CLEAR)               ;//Clear display
  LCD_Cmd(LCD_CURSOR_OFF)          ;//Turn cursor off
  LCD_Cmd(LCD_SECOND_ROW)          ;//Move to ROW2 for rest of program
  //Seup Keypad on PORTB
  Keypad_Init(&PORTB)              ;// Initialize Keypad on PORTB
   LCD_OUT(1,1,text)                ;//Print welcome message
  while(1) {   //while ever loop
               switch(state){ //check the current state
case 1:
               LCD_Cmd(LCD_CLEAR)    ;
               LCD_out(1,1,text);
               LCD_out(2,1,"EnterPass:");
               while(!keypad_Read()){}   //wait for key1 press
               Delay_ms(10)             ;//debounce time
               kp = Keypad_Released()   ;//wait for release
               var1=kp; //store var1 into kp
               var1=table[kp];
               ByteToStr(var1,txt);
               Lcd_Out_Cp(txt);
               state = 2;
               break;
case 2:
               if(kp==15) state=1; //go back state 1 when kp = # (enter key)
               else if(kp==13) state=1;//go back state 1 when kp = * (clear key)
               else
               state = 3;//go to state 3 get key2
               break;
case 3:
               while(!keypad_Read()){}   //wait for key2 press
               Delay_ms(10)             ;//debounce time
               kp = Keypad_Released()   ;//wait for release
               var2=kp;//store var2 into kp
               var2=table[kp];
               ByteToStr(var2,txt);
               Lcd_Out_Cp(txt);
               state=4;
               break;
case 4:
               if(kp==15) state=1;//enter key goes back state 1
               else if(kp==13) state=1;//clear key goes back state 1
               else
               state = 5;//continue goes to state 4
               break;
 case 5:
               while(!keypad_Read()){}   //wait for enter key press
               Delay_ms(10)             ;//debounce time
               kp = Keypad_Released()   ;//wait for release
               state = 6;
               break;
case 6:
               if(kp==13)
               state =1; //clear goes back to s1
               else if(kp==15)
               state = 7;//enter key goes to state 7
               else state = 5;
               break;
case 7:
               if (var1==0 && var2==5)//comparing for door1
                state = 12;  //goes state 11 open door1
               else state =8;
               break;
case 8:
               if (var1==2 && var2 == 8)//comparing for door2
               state = 13;//goes to state 9 for door 2
               else state=9;
               break;
case 9:
               if(var1==8 && var2 == 2) //comparing foor door3
               state = 14; //goes to state 15 for door 3
               else state =10;//goes next state
               break;
case 10:
               if (var1==3 && var2 == 3)
               state = 15;//goes to state 14 for door1 & 2
               else
               state =11;
               break;
case 11:
               LCD_OUT(2,1,"**Invalid Code**") ;
               attempts++; //increment attempts
               kp+=attempts;//kp = kp + attempts
               if(attempts < 3)  //attempts less than 3 times
               state=1;     //go state 1
               else
               state=16;
               while(!keypad_Read()){}   //wait for enter key press
               Delay_ms(10)             ;//debounce time
               kp = Keypad_Released()   ;//wait for release
               LCD_Cmd(LCD_CLEAR);
               break;
case 12://open door1 for 5secs
               LCD_Cmd(LCD_CLEAR);
               Lcd_out(2,1,text1) ;
               Lcd_out(1,1,text);
               //turn door1 led on
               PORTA.F4=1;
               PORTB.f4=1;
               PORTB.F7 = 0;
               PORTB.F6 = 0;
               PORTB.F5 = 0 ;
               Delay_ms(5000); //delay 5secs
               //turn door1 led off
               PORTA.F4 = 0;
               LCD_Cmd(LCD_CLEAR)         ;
               attempts=0;
               state=1;
               break;
case 13: //open door2 for 5secs
               LCD_Cmd(LCD_CLEAR);//clear display
               LCD_OUT(2,1,text2);
               Lcd_out(1,1,text) ;
               //turn door2 led on
               PORTA.F4=1;
               PORTB.F5=1;
               PORTB.f4=0;
               PORTB.f6=0;
               PORTB.f7=0;
               Delay_ms(5000);
               PORTA.F4 = 0 ;//turn door2 led off
               LCD_Cmd(LCD_CLEAR);
               attempts=0;
               state =1;
               break;
case 14:      //open door3
              LCD_Cmd(LCD_CLEAR);
              LCD_OUT(2,1," *Door3 On*");
              Lcd_out(1,1,text) ;
              PORTA.F4=1;
              PORTB.F4=0;
              PORTB.F5=0;
              PORTB.f6=1;
              PORTB.f7=0;
              Delay_ms(5000);
              PORTA.F4 =0;
              LCD_Cmd(LCD_CLEAR);
              attempts=0;
              state =1;
              break;
case 15: //open door1 and door2
              LCD_Cmd(LCD_CLEAR);
              LCD_OUT(2,1,text3);
              Lcd_out(1,1,text) ;
              PORTA.F4=1;
              PORTB.F4=1;
              PORTB.F5=1;
              PORTb.f6=0;
              PORTb.f7=0;
              Delay_ms(5000);
              PORTA.F4 = 0 ;
              LCD_Cmd(LCD_CLEAR);
              attempts=0;
              state =1;
              break;
default:// must be a bad attempts
              LCD_OUT(2,1,"  **Alarm On**");
              LCD_OUT(1,1,text);
              PORTA.F4=1;
              PORTB.F4=0;
              PORTB.F5=0;
              PORTb.f6=0;
              PORTb.f7=1;
              Delay_ms(5000);
              PORTA.F4=0;
              LCD_Cmd(LCD_CLEAR);
              attempts=0;
              state = 1; //goes to state 1
              break;
                       }
                         }
                         }
