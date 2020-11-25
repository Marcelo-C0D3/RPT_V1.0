#include "user_interface.h"

class Manager
{
private:
  const int altOnda = 18;
  const int hReport = 13;
  const int dayReport = 1;

  const String relatControl = "Relatorio de quedas: \n";

  int T = 50000;
  int reqNTP = 40000;
  
public:
  
  const long utcOffsetInSeconds = -10800; //Ajuste fusu-horario BR SC
  const char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sabado"}; //Dayes of week :D
  
  int valor_AD = 0, soma1 = 0, soma2 = 0, soma3 = 0, soma4 = 0, i = 0, i2 = 0, i3 = 0, i4 = 0, x = 1, state = 0, idRelat = 0;
  double med1 = 0, med2 = 0, med3 = 0, med4 = 0, ant = 0;

  //timers states
  bool _timeout1 = false, _timeout2 = false, _timeout3 = false;

  String assunto = " ";
  String estado = " ";
  String help = "Welcome Bot Som Maior \n Use comands: \n `/state` to State radio \n `/relat` to relat control";
  String tryThis = "Try this `/help` to more inf.";
  String relatorio = "Relatorio de quedas: \n";
  String queda = " ";
  String day = " ";
  String hours = " ";
  String minutes = " ";

  os_timer_t mTimer1, mTimer2, mTimer3; //timer 1./timer 2./timer 3.

  void getValues();
  void callMedia();
  void report();
  void timerReport();
  void botCommand();
  String sending(const int64_t &to, String &message);

  void usrInit();
  void reset_timer1();
  void reset_timer2();
  void reset_timer3();
};