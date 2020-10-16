#include <Arduino.h>

#include "CTBot.h"
CTBot myBot;
TBMessage msg;


#include <ESP8266HTTPClient.h> //Network HTTP librarie
#include <NTPClient.h>         //Network time protocol lib.
#include <ESP8266WiFi.h>       //Wifi Propierts to esp8266
#include <WiFiUdp.h>           //Service UDP, time!
#include <ESP8266WebServer.h>  //Local WebServer used to serve the configuration portal
#include <WiFiClient.h>
#include <main.h> //Progam Main Radio_Pio_v2

bool shouldSaveConfig = false;                                                                      //Flag Save  config WiFi
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"}; //Dayes of week :D

Manager go; // reference to class main
// Define NTP Client to get time
HTTPClient http;
WiFiClient client;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", go.utcOffsetInSeconds);

void tCallback1(void *tCall);
void tCallback2(void *tCall);
void tCallback3(void *tCall);

void setup()
{
  Serial.begin(115200);

  Serial.println("Starting TelegramBot...");

  // connect the ESP8266 to the desired access point
  myBot.wifiConnect(ssid, pass);

  // set the telegram bot token
  myBot.setTelegramToken(token);

  // check if all things are ok
  if (myBot.testConnection())
    Serial.println("\ntestConnection OK");
  else
    Serial.println("\ntestConnection NOK");

  go.usrInit();
}

void loop()
{

  if (go.i < 40)
  {
    go.getValues();
  }
  else
  {
    go.callMedia();
  }
}

void Manager::checkMessage(String &message)
{

  // if there is an incoming message...
  if (myBot.getNewMessage(msg))
  {
    Serial.println(msg.text);
    Serial.println(msg.sender.id);
    Serial.println(sending(somMaior, assunto));
  }
  // wait 500 milliseconds
  //delay(500);
}

String Manager::sending(const int64_t &to, String &message)
{

  if (myBot.testConnection())
  {
    Serial.println("\ntestConnection OK");
  }
  else
  {
    myBot.wifiConnect(ssid, pass);
    Serial.println("\nReconect....");
  }
  // ...forward it to the sender
  //myBot.sendMessage(msg.sender.id, msg.text);
  myBot.sendMessage(to, message);

  return "menssagem enviada com sucesso";
}

void Manager::getValues()
{
  valor_AD = analogRead(A0);
  soma1 = soma1 + valor_AD;
  i++;
}
void Manager::callMedia()
{
  switch (x)
  {
  case 1:
    med1 = soma1 / i;
    i = 0, soma1 = 0;
    soma2 += med1;
    i2++;
    if (i2 == 15)
    {
      x = 2;
    }
    break;
  case 2:
    med2 = soma2 / i2;
    i2 = 0, soma2 = 0;
    soma3 += med2;
    x = 1;
    i3++;
    if (i3 == 10)
    {
      x = 3;
    }
    break;
  case 3:
    med3 = soma3 / i3;
    i3 = 0, soma3 = 0;
    soma4 += med3;
    x = 1;
    i4++;
    if (i4 == 5)
    {
      x = 4;
    }
    break;
  case 4:
    med4 = soma4 / i4;
    Serial.println(med4);
    i4 = 0, soma4 = 0;
    x = 200;
    break;
  case 200:
    if (med4 > altOnda)
    {
      Serial.println("reset 2");
      _timeout2 = false;
      reset_timer2();
      //yield();
    }
    else
    {
      Serial.println("reset 1");
      _timeout1 = false;
      reset_timer1();
    }
    go.report();
    x = 1;
    break;
  default:

    break;
  }
}

//function to resports.
void Manager::report()
{

  if (_timeout3)
  {
    //timerReport();
    digitalWrite(D5, !digitalRead(D5));
    _timeout3 = false;
    reset_timer3();
    timerReport();
    yield();
  }
  else if (_timeout2 && (state == 0 || state == 1))
  {
    state = 2;
    assunto = "A rádio está fora do ar, ou Sem áudio perceptível.";
    Serial.println(sending(somMaior, assunto));

    queda = day + ", " + hours + ":" + minutes + "\n";
    relatorio = relatorio + queda;
  }
  else if (_timeout1 && (state == 0 || state == 2))
  {
    state = 1;
    assunto = "A rádio está ativa e estável.";
    Serial.println(sending(somMaior, assunto));
  }
}

// function to reprot relat in time exact;
void Manager::timerReport()
{

  timeClient.update();
  day = daysOfTheWeek[timeClient.getDay()];
  hours = timeClient.getHours();
  minutes = timeClient.getMinutes();

  Serial.print(hours);
  Serial.print(":");
  Serial.print(minutes);
  Serial.println();

  if (relatorio != relatControl && timeClient.getMinutes() == hReport && idRelat == 0)
  {
    idRelat = 1;
    assunto = relatorio;
    Serial.println(sending(somMaior, assunto));
  }
  else if (relatorio == relatControl && timeClient.getHours() == hReport && idRelat == 0)
  {
    idRelat = 1;
    assunto = "A rádio não apresentou nenhuma queda nas ultimas 24H.";
    assunto = " /n A rádio  \n não apresentou \n nenhuma  /n queda nas \n ultimas 24H.";
    Serial.println(sending(somMaior, assunto));
  }
  else if (timeClient.getHours() == (hReport + 1) && idRelat == 1)
  {
    idRelat = 0;
    relatorio = relatControl;
  }
}

// inicia  Timers
void Manager::usrInit()
{
  os_timer_setfn(&mTimer1, tCallback1, NULL);
  os_timer_arm(&mTimer1, T, true);
  os_timer_setfn(&mTimer2, tCallback2, NULL);
  os_timer_arm(&mTimer2, T, true);
  os_timer_setfn(&mTimer3, tCallback3, NULL);
  os_timer_arm(&mTimer3, chargingT, true);

  reset_timer3();
  reset_timer2();
  reset_timer1();
}

void Manager::reset_timer1()
{
  os_timer_arm(&mTimer1, T, true);
}
void Manager::reset_timer2()
{
  os_timer_arm(&mTimer2, T, true);
}
void Manager::reset_timer3()
{
  os_timer_arm(&mTimer3, chargingT, true);
}

void tCallback1(void *tCall)
{
  go._timeout1 = true;
}

void tCallback2(void *tCall)
{
  go._timeout2 = true;
}

void tCallback3(void *tCall)
{
  go._timeout3 = true;
}
