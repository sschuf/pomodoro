#include <LiquidCrystal_I2C.h>
#include "pitches.h"
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables globales
const int led = 8;
const byte boton = 2;
unsigned long tiempoEscoria;
unsigned long intervalo = (1*10000);
bool empezar;

// Melodía canción 1
const int buzzer = 10; //Define pin 10, can use other PWM pins  (5,6 or 9)
                       //Note pins 3 and 11 can't be used when using the tone function in Arduino Uno
const int songspeed = 1.5; //Change to 2 for a slower version of the song, the bigger the number the slower the song
//*****************************************
#define NOTE_C4  262   //Defining note frequency
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988
//*****************************************
int notes[] = {       //Note of the song, 0 is a rest/pulse
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,
   
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,
   
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0, 
   NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
   NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,
   
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_D5, NOTE_E5, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
   NOTE_C5, NOTE_A4, NOTE_B4, 0,

   NOTE_A4, NOTE_A4, 
   //Repeat of first part
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,

   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
   NOTE_A4, NOTE_G4, NOTE_A4, 0,
   
   NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0, 
   NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
   NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,
   
   NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0, 
   NOTE_D5, NOTE_E5, NOTE_A4, 0, 
   NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
   NOTE_C5, NOTE_A4, NOTE_B4, 0,
   //End of Repeat

   NOTE_E5, 0, 0, NOTE_F5, 0, 0,
   NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
   NOTE_D5, 0, 0, NOTE_C5, 0, 0,
   NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4,

   NOTE_E5, 0, 0, NOTE_F5, 0, 0,
   NOTE_E5, NOTE_E5, 0, NOTE_G5, 0, NOTE_E5, NOTE_D5, 0, 0,
   NOTE_D5, 0, 0, NOTE_C5, 0, 0,
   NOTE_B4, NOTE_C5, 0, NOTE_B4, 0, NOTE_A4
};
//*****************************************
int duration[] = {         //duration of each note (in ms) Quarter Note is set to 250 ms
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125, 
  250, 125, 250, 125, 
  125, 125, 250, 125, 125,
  125, 125, 375, 375,

  250, 125,
  //Rpeat of First Part
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125, 
  
  125, 125, 250, 125, 125, 
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125, 
  250, 125, 250, 125, 
  125, 125, 250, 125, 125,
  125, 125, 375, 375,
  //End of Repeat
  
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500,

  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500
};

// Melodía canción 2
// The melody array 
int melody[] = {
  NOTE_FS5, NOTE_FS5, NOTE_D5, NOTE_B4, NOTE_B4, NOTE_E5, 
  NOTE_E5, NOTE_E5, NOTE_GS5, NOTE_GS5, NOTE_A5, NOTE_B5, 
  NOTE_A5, NOTE_A5, NOTE_A5, NOTE_E5, NOTE_D5, NOTE_FS5, 
  NOTE_FS5, NOTE_FS5, NOTE_E5, NOTE_E5, NOTE_FS5, NOTE_E5
};
// The note duration, 8 = 8th note, 4 = quarter note, etc.
int durations[] = {
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8, 
  8, 8, 8, 4, 4, 4, 
  4, 5, 8, 8, 8, 8
};
// determine the length of the arrays to use in the loop iteration
int songLength = sizeof(melody)/sizeof(melody[0]);

// Configuración inicial
void setup() {
  // Abre el puerto serie en 9600 bps
  Serial.begin(9600);
  
  // Inicializar puerto de comunicación con módulo Bluetooth
  ble.begin(9600);

  // LDC
  lcd.init();
  lcd.backlight();

  // LED
  pinMode(led, OUTPUT);

  // Interrupción botón
  interrupts();
  pinMode(boton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(boton), botonPresionado, CHANGE);
}

// Programa
void loop() {

  // QUITAR
  digitalWrite(led, LOW);       // QUITAR
  empezar = 0;                  // QUITAR O VER QUÉ PASA

  // LCD Mensaje de bienvenida
  lcd.clear();
  lcd.print("Bienvenido");
  lcd.setCursor(0, 1); 
  lcd.print("a Pomodoro");
  delay(2000);
  lcd.clear();
  lcd.print("Apreta el boton");
  lcd.setCursor(0, 1); 
  lcd.print("para comenzar");

  // Espera hasta que el usuario aprete el botón
  do {
    delay(100);
  } while (empezar!=1);

  // Inicio de POMODORO (El usuario apretó el botón)

  // Limpia la pantalla y muestra log
  lcd.clear();
  lcd.print("SESION INICIADA");
  ble.write(empezar);
  
  // Definición de variables de tiempo
  const int hrs_en_dia = 24; 
  const int min_en_hrs = 60; 
  const int seg_en_min = 60; 
  unsigned long seg,min,hrs;

  // Comienza en loop hasta que tiempo transcurrido = intervalo
  do {
    lcd.setCursor(0, 1);

      // Descomponer millis() en seg y minutos desde que se presionó el botón
     min=(((millis()-tiempoEscoria)/1000));
     seg=(((millis()-tiempoEscoria)/1000)/60);

     seg = seg % seg_en_min; 
     min = min % min_en_hrs;
//   hrs = seg/seg_en_min/min_en_hrs % hrs_en_dia; 

      // Imprime el tiempo en pantalla como log
     (seg < 10) ? lcd.print("0") : NULL;
     lcd.print(seg);
     lcd.print(":");
     (min< 10) ? lcd.print("0") : NULL;
     lcd.print(min);  

  } while (intervalo/1000!=(millis()-tiempoEscoria)/1000);

  // Intervalo de concentración finalizado
  digitalWrite(led, HIGH);
  lcd.clear();
  lcd.print("La sesion");
  lcd.setCursor(0, 1);
  lcd.print("ha finalizado");

//  for (int thisNote = 0; thisNote < songLength; thisNote++){
//    // determine the duration of the notes that the computer understands
//    // divide 1000 by the value, so the first note lasts for 1000/8 milliseconds
//    int duration = 1000/ durations[thisNote];
//    tone(11, melody[thisNote], duration);
//    // pause between notes
//    int pause = duration * 1.3;
//    delay(pause);
//    // stop the tone
//    noTone(11);
//  }

  for (int i=0;i<203;i++){              //203 is the total number of music notes in the song
    int wait = duration[i] * songspeed;
    tone(buzzer,notes[i],wait);          //tone(pin,frequency,duration)
    delay(wait);                        //delay is used so it doesn't go to the next loop before tone is finished playing
  }
  //You can click reset on Arduino to replay the song
  
  delay(5000);

 }

// Cambiar el estado de led                                         // Probar agregar el led como argumento
void prenderLed() {
  if (digitalRead(led) == HIGH) {
    digitalWrite(led, LOW);
  } else {
    digitalWrite(led, HIGH);
  }
}

// INTERRUPCIÓN -> Cuando el usuario apreta el botón
void botonPresionado() {
  
  // Tiempo a restar para que el temporizado comience de cero
  tiempoEscoria = millis();

  // Cambia el estado del LED
  prenderLed();

  // Cambia variable bool para comenzar
  empezar = 1;
}
