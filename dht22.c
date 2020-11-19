//  gcc dht22.c -o dht22 -L/usr/local/lib -l wiringPi

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#define MAX_TIME 85
#define dht22PIN 7
#define ATTEMPTS 20
int dht22_val[5]={0,0,0,0,0};

int dht22_read_val()
{
  float humid=0.0;
  float tempC=0.0;
  float tempF=0.0;
  uint8_t lststate=HIGH;
  uint8_t counter=0;
  uint8_t j=0,i,ix;
  FILE *fp;
  char *out_string;
  char *ctime_string;
  time_t current_time;
  current_time=time(NULL);

  for(i=0;i<5;i++)
     dht22_val[i]=0;
  pinMode(dht22PIN,OUTPUT);
  digitalWrite(dht22PIN,LOW);
  delay(18);
  digitalWrite(dht22PIN,HIGH);
  delayMicroseconds(40);
  pinMode(dht22PIN,INPUT);
  for(i=0;i<MAX_TIME;i++)
  {
    counter=0;
    while(digitalRead(dht22PIN)==lststate){
      counter++;
      delayMicroseconds(1);
      if(counter==255)
        break;
    }
    lststate=digitalRead(dht22PIN);
    if(counter==255)
       break;
    // top 3 transistions are ignored
    if((i>=4)&&(i%2==0)){
      dht22_val[j/8]<<=1;
      if(counter>16)
        dht22_val[j/8]|=1;
      j++;
    }
  }
  // verify checksum and print the verified data
  if((j>=40)&&(dht22_val[4]==((dht22_val[0]+dht22_val[1]+dht22_val[2]+dht22_val[3])& 0xFF)))
  {
  fp = fopen("/home/pi/air_quality.log", "a");
//  printf("%d.%d,%d.%d\n",dht22_val[0],dht22_val[1],dht22_val[2],dht22_val[3]);
//  for a DHT22, change the 10.* in the next two lines to 256.*
    humid=(256.*dht22_val[0]+dht22_val[1])/10.;
    tempC=(256.*dht22_val[2]+dht22_val[3])/10.;
    tempF=(9.*tempC/5.)+32.;
    printf("%s,%5.1f,%5.1f\n",ctime(&current_time),tempF,humid);
    fprintf(fp,"%5.1f, %5.1f\n",tempF,humid);
    fclose(fp);
    return 1;
  }
  else
    printf("%s\terror reading dht22 sensor\n",ctime(&current_time));
    delay(2000);
    return 0;
}

int main(void)
{
  int attempts=ATTEMPTS;
  if(wiringPiSetup()==-1)
    exit(1);
  while(attempts)
  {
    int success = dht22_read_val();
    if (success) {
      break;
    }
    attempts--;
    delay(500);
  }
  return 0;
}
