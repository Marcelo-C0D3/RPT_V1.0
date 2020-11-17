#include <Arduino.h>

#include "CTBot.h"

// const String ssid = "Vírus grátis 2.4";                                // REPLACE mySSID WITH YOUR WIFI SSID
// const String pass = "temcerteza";                                      // REPLACE myPassword YOUR WIFI PASSWORD, IF ANY
// const String token = "1286384184:AAEnco2JfKpeU6asjlFy0iXhvQNMPB6mBaM"; // REPLACE myToken WITH YOUR TELEGRAM BOT TOKEN

#include <ESP8266HTTPClient.h> //Network HTTP librarie
#include <NTPClient.h>         //Network time protocol lib.
#include <ESP8266WiFi.h>       //Wifi Propierts to esp8266
#include <WiFiUdp.h>           //Service UDP, time!
#include <ESP8266WebServer.h>  //Local WebServer used to serve the configuration portal
#include <WiFiClient.h>
#include <main.h> //Progam Main Radio_Pio_v2
#include <acess.h>

Manager go; // reference to class main
Acess cred;
// Define NTP Client to get time
CTBot myBot;
HTTPClient http; //ver situations
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

  Serial.println(cred.checkWiFiConect());

  // check if all things are ok
  if (myBot.testConnection())
  {
    Serial.println("\ntestConnection OK");
  }
  else
  {
    Serial.println(cred.checkWiFiConect());
  }
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

String Acess::checkWiFiConect()
{
  String message;
  if (myBot.testConnection())
  {
    message = "testConnection OK";
  }
  else
  {
    myBot.wifiConnect(ssid, pass);
    myBot.setTelegramToken(token);
    message = "Conect....";
  }

  return message;
}

String Manager::sending(const int64_t &to, String &message)
{

  Serial.println(cred.checkWiFiConect());
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
      _timeout2 = false;
      reset_timer2();
      //yield();
    }
    else
    {
      _timeout1 = false;
      reset_timer1();
    }
    report();
    botCommand();
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
    Serial.println(sending(cred.somMaior, assunto));

    estado = "Fora Do Ar";
    queda = day + ", " + hours + ":" + minutes + "\n";
    relatorio = relatorio + queda;
  }
  else if (_timeout1 && (state == 0 || state == 2))
  {
    state = 1;
    estado = "Ativa e estavel";
    assunto = "A rádio está novamente ativa e estável.";
    Serial.println(sending(cred.somMaior, assunto));
  }
}

void Manager::botCommand()
{
  TBMessage msg;

  if (myBot.getNewMessage(msg))
  {
    //Serial.println(msg.text);
    if (msg.text.equalsIgnoreCase("/state"))
    {
      Serial.println(sending(cred.somMaior, estado));
    }
    else if (msg.text.equalsIgnoreCase("/relat"))
    {
      if (relatorio == relatControl)
      {
        assunto = " A rádio não apresentou nenhuma queda nas ultimas 24H.";
        Serial.println(sending(cred.somMaior, assunto));
      }
      else
      {
        Serial.println(sending(cred.somMaior, relatorio));
      }
    }
    else if (msg.text.equalsIgnoreCase("/help"))
    {
      Serial.println(sending(cred.somMaior, help));
    }
    else
    {
      Serial.println(sending(cred.somMaior, tryThis));
    }
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
  Serial.println(timeClient.getDay());

  if (relatorio != relatControl && timeClient.getHours() == hReport && idRelat == 0)
  {
    idRelat = 1;
    assunto = relatorio;
    Serial.println(sending(cred.somMaior, assunto));
  }
  else if (relatorio == relatControl && timeClient.getHours() == hReport && idRelat == 0)
  {
    idRelat = 1;
    assunto = " A rádio não apresentou nenhuma queda nas ultimas 24H.";
    Serial.println(sending(cred.somMaior, assunto));
  }
  else if (timeClient.getHours() == (hReport + 1) && idRelat == 1)
  {
    idRelat = 0;
    relatorio = relatControl;
  }

    //day off week report teste
  // if (relatorio != relatControl && timeClient.getDay() == dayReport && idRelat == 0)
  // {
  //   idRelat = 1;
  //   assunto = relatorio;
  //   Serial.println(sending(cred.somMaior, assunto));
  // }
  // else if (relatorio == relatControl && timeClient.getDay() == dayReport && idRelat == 0)
  // {
  //   idRelat = 1;
  //   assunto = "A rádio não apresentou nenhuma queda esta Semana";
  //   Serial.println(sending(cred.somMaior, assunto));
  // }
  // else if (timeClient.getDay() == (dayReport + 1) && idRelat == 1)
  // {
  //   idRelat = 0;
  //   relatorio = relatControl;
  // }

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