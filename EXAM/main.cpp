#include "mbed.h"
#include "uLCD_4DGL.h"

uLCD_4DGL uLCD(D1, D0, D2); // serial tx, serial rx, reset pin;
AnalogOut aout(PA_4);
//AnalogOut Rvin(D6);
DigitalIn But1(A2);
DigitalIn But2(A1);
DigitalIn But3(A0);
InterruptIn button(USER_BUTTON);
AnalogIn VIN(A3);
EventQueue queue_generate(32 * EVENTS_EVENT_SIZE);
EventQueue queue_sample(32 * EVENTS_EVENT_SIZE);
Thread t;
Thread s;

int frequency = 1; 
float sample2[480];
void wave_generate(){
      int i,j;
      while(1){
            for(j = 0; j < 2; j++){
                  for (i = 0; i < (80/frequency); i++){
                        aout.write_u16((uint16_t)(59578 * i * frequency/ 80));
                        ThisThread::sleep_for(1ms);
                        sample2[i + 240 * j] = float(VIN);
                  }
                  for (i = 80/frequency; i < 240 - (80/frequency); i++){
                        aout.write_u16((uint16_t)(59578));
                        ThisThread::sleep_for(1ms);
                        sample2[i + 240 * j] = float(VIN);
                  }
                  for (i = 0; i < (80/frequency); i++){
                        aout.write_u16((uint16_t)(59578 * ((80/frequency) - i) * frequency/ 80));
                        ThisThread::sleep_for(1ms);
                        sample2[240 * j + 240 + i - 80/frequency] = float(VIN);
                  }
            }
    }
}

void wave_sample(){
      int i;
      for (i = 0; i < 480; i++){
            printf("%f\r\n", sample2[i]);
      }

}

int main(){
    int i;
    uint16_t sample = 0;
    

    t.start(callback(&queue_generate, &EventQueue::dispatch_forever));
    s.start(callback(&queue_sample, &EventQueue::dispatch_forever));
    button.rise(queue_sample.event(wave_sample));
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
    queue_generate.call(wave_generate);
    
}