#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
DigitalInOut ping(D11);
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
Timer t;
BBCar car(pin5, pin6, servo_ticker);

int main() {
   float val;
   char direction;
   char recv[1];
   uart.set_baud(9600);
   while(1){
      ping.output();
      ping = 0; wait_us(200);
      ping = 1; wait_us(5);
      ping = 0; wait_us(5);

      ping.input();
      while(ping.read() == 0);
      t.start();
      while(ping.read() == 1);
      val = t.read();
      if (val*17700.4f < 15) {
         car.stop();
         printf("less than 15cm, stop\n");
         ThisThread::sleep_for(1s);
         t.stop();
         t.reset();
         continue;
      } else {
         printf("Ping = %lf\r\n", val*17700.4f);
         t.stop();
         t.reset();
      }
      if(uart.readable()){
         uart.read(recv, sizeof(recv));
         pc.write(recv, sizeof(recv));
         direction = *recv;
         if (direction == 'r') {
            car.turn(80, -0.4);
            ThisThread::sleep_for(230ms);
            car.stop();
            ThisThread::sleep_for(270ms);
         } else if (direction == 'l'){
            car.turn(80, 0.4);
            ThisThread::sleep_for(230ms);
            car.stop();
            ThisThread::sleep_for(270ms);
         } else {
            car.goStraight(80);
            ThisThread::sleep_for(500ms);
         }
         
      }
   }
}

