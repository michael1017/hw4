#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"

Ticker servo_ticker;
Ticker encoder_ticker;
PwmOut pin5(D5), pin6(D6);

DigitalIn encoderl(A5);
DigitalIn encoderr(A4);
volatile int steps;
volatile int last;

DigitalInOut ping(D11);
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx
BufferedSerial xbee(A1,A0); //tx,rx
Timer t;
BBCar car(pin5, pin6, servo_ticker);
void encoder_controlr() {
   int value = encoderr;
   if (!last && value) steps++;
   last = value;
}
void encoder_controll() {
   int value = encoderl;
   if (!last && value) steps++;
   last = value;
}
void stage1(void) {
   float val;
   char direction;
   char recv[1];
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
      if (val*17700.4f < 50) {
         car.stop();
         printf("less than 50cm, stop\n");
         t.stop();
         t.reset();
         break;
      } else if (val*17700.4f > 50) {
         car.goStraight(80);
         ThisThread::sleep_for(500ms);
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
            ThisThread::sleep_for(200ms);
            car.stop();
            ThisThread::sleep_for(300ms);
         } else if (direction == 'l'){
            car.turn(80, 0.4);
            ThisThread::sleep_for(200ms);
            car.stop();
            ThisThread::sleep_for(300ms);
         } else {
            car.goStraight(80);
            ThisThread::sleep_for(500ms);
         }
         
      }
   }
   uart.write("c", 1);
}
void stage2(void) {
   car.turn(80, 0.4);
   ThisThread::sleep_for(5200ms);
   car.stop();
}
void stage3(void) {
   float val;
   char direction;
   char recv[1];
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
      if (val*17700.4f < 40) {
         car.stop();
         printf("less than 20cm, stop\n");
         t.stop();
         t.reset();
         break;
      } else if (val*17700.4f > 60) {
         car.goStraight(80);
         ThisThread::sleep_for(500ms);
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
            ThisThread::sleep_for(200ms);
            car.stop();
            ThisThread::sleep_for(300ms);
         } else if (direction == 'l'){
            car.turn(80, 0.4);
            ThisThread::sleep_for(200ms);
            car.stop();
            ThisThread::sleep_for(300ms);
         } else {
            car.goStraight(80);
            ThisThread::sleep_for(500ms);
         }
         
      }
   }
}
void stage4(void) {
   car.turn(80, -0.4);
   ThisThread::sleep_for(2400ms);
   car.stop();
}
void stage5(void) {
   float val;
   char direction;
   char recv[1];
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
      if (val*17700.4f < 20) {
         car.stop();
         printf("less than 20cm, stop\n");
         t.stop();
         t.reset();
         break;
      }
      else {
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
            ThisThread::sleep_for(200ms);
            car.stop();
            ThisThread::sleep_for(300ms);
         } else if (direction == 'l'){
            car.turn(80, 0.4);
            ThisThread::sleep_for(200ms);
            car.stop();
            ThisThread::sleep_for(300ms);
         } else {
            car.goStraight(80);
            ThisThread::sleep_for(500ms);
         }
         
      }
   }
}
int main() {
   uart.set_baud(9600);
   xbee.set_baud(9600);
   
   xbee.write("stage1 start\n", 12);
   stage1();
   xbee.write("stage1 end..\n", 12);
   xbee.write("stage2 start\n", 12);
   stage2();
   xbee.write("stage2 end..\n", 12);
   xbee.write("stage3 start\n", 12);
   stage3();
   xbee.write("stage3 end..\n", 12);
   xbee.write("stage4 start\n", 12);
   stage4();
   xbee.write("stage4 end..\n", 12);
   xbee.write("stage5 start\n", 12);
   stage5();
   xbee.write("stage5 end..\n", 12);
}