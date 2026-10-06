#include "arduino_secrets.h"

#include <Stepper.h>
#include <Servo.h>
#include <Ultrasonic.h>

// ========= ULTRASSÃNICO =========
Ultrasonic ultrasonic(4, 3); // trig, echo

// ========= SERVOS =========
Servo servoEsq;
Servo servoDir;

const int pinServoEsq = 11;
const int pinServoDir = 12;

// ========= MOTOR DE PASSO =========
const int passosPorVolta = 2048;
Stepper motor(passosPorVolta, 10, 9, 6, 5);

// posiÃ§Ãµes
const int centro = 90;

const int esqA = 60;
const int dirA = 120;

const int esqB = 120;
const int dirB = 60;

//========================================
void setup()
{
  Serial.begin(9600);

  servoEsq.attach(pinServoEsq);
  servoDir.attach(pinServoDir);

  motor.setSpeed(15);

  // posiÃ§Ã£o inicial
  servoEsq.write(centro);
  servoDir.write(centro);

  delay(1000);

  moverServosSuave(centro, centro, esqA, dirA);
}

//========================================
void loop()
{
  long distancia = ultrasonic.read();

  Serial.println(distancia);

  if (distancia < 15)
  {
    delay(300);

    // SUBIDA + TROCA DE CENTRO DE MASSA
    subirComInclinacao();

    delay(2000);

    // DESCIDA + RETORNO DO CENTRO DE MASSA
    descerComInclinacao();

    delay(1000);
  }

  delay(100);
}

//========================================
// sobe 3 voltas e inclina durante a subida
void subirComInclinacao()
{
  int passosTotais = 3 * 1024;
  int blocos = 60;

  int passosPorBloco = passosTotais / blocos;

  for (int i = 0; i <= blocos; i++)
  {
    // move motor
    motor.step(passosPorBloco);

    // move servos gradualmente
    int posEsq = map(i, 0, blocos, esqA, esqB);
    int posDir = map(i, 0, blocos, dirA, dirB);

    servoEsq.write(posEsq);
    servoDir.write(posDir);

    delay(30);
  }
}

//========================================
// desce 3 voltas e retorna centro de massa
void descerComInclinacao()
{
  int passosTotais = 3 * 1024;
  int blocos = 60;

  int passosPorBloco = passosTotais / blocos;

  for (int i = 0; i <= blocos; i++)
  {
    motor.step(-passosPorBloco);

    int posEsq = map(i, 0, blocos, esqB, esqA);
    int posDir = map(i, 0, blocos, dirB, dirA);

    servoEsq.write(posEsq);
    servoDir.write(posDir);

    delay(30);
  }
}

//========================================
void moverServosSuave(int esqInicial, int dirInicial,
                      int esqFinal, int dirFinal)
{
  for (int i = 0; i <= 60; i++)
  {
    servoEsq.write(map(i, 0, 60, esqInicial, esqFinal));
    servoDir.write(map(i, 0, 60, dirInicial, dirFinal));

    delay(25);
  }
}