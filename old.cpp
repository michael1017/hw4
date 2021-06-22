#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalInOut pin10(D10);
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

BBCar car(pin5, pin6, servo_ticker);

int main() {
   
   char direction;
   char recv[1];
   uart.set_baud(9600);
   parallax_ping  ping1(pin10);
   while(1){
      if(uart.readable()){
         uart.read(recv, sizeof(recv));
         pc.write(recv, sizeof(recv));
         direction = *recv;
         if (direction == 'r') {
            car.turn(80, -0.4);
            ThisThread::sleep_for(100ms);
            car.stop();
            ThisThread::sleep_for(400ms);
         } else if (direction == 'l'){
            car.turn(80, 0.4);
            ThisThread::sleep_for(100ms);
            car.stop();
            ThisThread::sleep_for(400ms);
         } else {
            car.goStraight(80);
            ThisThread::sleep_for(500ms);
         }
         printf("%f\n", (float)ping1);
      }
   }
}

