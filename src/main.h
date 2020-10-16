#include "user_interface.h"

class Manager
{
private:
  
  
  int T = 30000;
  int chargingT = 30000;

public:
  int valor_AD = 0, soma1 = 0, soma2 = 0, soma3 = 0, soma4 = 0, i = 0, i2 = 0, i3 = 0, i4 = 0, x = 1, state = 0, idRelat = 0;
  double med1 = 0, med2 = 0, med3 = 0, med4 = 0, ant = 0;
  const long utcOffsetInSeconds = -10800; //Ajuste fusu-horario BR SC
  
  bool _timeout1 = false, _timeout2 = false, _timeout3 = false;

  String assunto = " ";
  String relatorio = "Relatorio de quedas: \n";
  String queda = " ";
  String day = " ";
  String hours = " ";
  String minutes = " ";

  os_timer_t mTimer1, mTimer2, mTimer3; //timer 1./timer 2./timer 3.

  String checkWiFiConect();
  void getValues();
  void callMedia();
  void report();
  void timerReport();
  String sending(const int64_t& to, String& message);
  void checkMessage(String& message);

  void usrInit();
  void reset_timer1();
  void reset_timer2();
  void reset_timer3();
};