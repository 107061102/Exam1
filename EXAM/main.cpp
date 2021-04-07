#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
AnalogOut aout(PA_4);
//AnalogOut Rvin(D6);
DigitalIn But1(A2);
DigitalIn But2(A1);
DigitalIn But3(A0);
DigitalIn mypin(USER_BUTTON);
InterruptIn button(USER_BUTTON);
AnalogIn VIN(A3);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

int frequency = 1;


void wave_sample(){
      int i;
      for (i = 0; i < 240; i++){
            printf("%f\r\n", VIN);
      }

}

int main(){
    int i;
    uint16_t sample = 0;
    float sample2[240];

    t.start(callback(&queue, &EventQueue::dispatch_forever));
    button.rise(queue.event(wave_sample));
    while(1){
        if (But1 == 1 && frequency != 1){
            frequency /= 2;
            uLCD.color(BLUE);  
            uLCD.background_color(WHITE);
            uLCD.textbackground_color(WHITE);
            uLCD.cls();
            // basic printf demo = 16 by 18 characters on screen
            uLCD.locate(1, 1);
            uLCD.text_width(4); //4X size text
            uLCD.text_height(4);
            uLCD.color(GREEN);
            if (frequency == 1){
                  uLCD.printf("1\n");
            }
            else{
                  uLCD.printf("1/%d\n", frequency);
            }
        }
        if (But2 ==1){
              break;
        }
        if (But3 == 1 && frequency != 8){

            frequency *= 2;
            uLCD.color(BLUE);  
            uLCD.background_color(WHITE);
            uLCD.textbackground_color(WHITE);
            uLCD.cls();
            // basic printf demo = 16 by 18 characters on screen
            uLCD.locate(1, 1);
            uLCD.text_width(4); //4X size text
            uLCD.text_height(4);
            uLCD.color(GREEN);
            uLCD.printf("1/%d\n", frequency);

        }
    }
    while(1){
            for (i = 0; i < (80/frequency); i++){
                  aout.write_u16((uint16_t)(59578 * i * frequency/ 80));
                  wait_us(1000);
            }
            for (i = 80/frequency; i < 240 - (80/frequency); i++){
                  aout.write_u16((uint16_t)(59578));
                  wait_us(1000);
            }
            for (i = 0; i < (80/frequency); i++){
                  aout.write_u16((uint16_t)(59578 * ((80/frequency) - i) * frequency/ 80));
                  wait_us(1000);
            }
    }
}