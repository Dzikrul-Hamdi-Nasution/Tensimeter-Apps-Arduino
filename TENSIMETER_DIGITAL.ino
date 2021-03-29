#include <LiquidCrystal.h> //mengincludekan library LCD
#include <SoftwareSerial.h>

SoftwareSerial BLUETOOTH(6, 7); // RX, TX
//untuk urutannya RS, E, 4, 5, 6, 7
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); //pin yang disambungkan antara arduino dengan LCD
int Sensor_tekanan = A0;
int waktu,waktu_pulsa,bpm,nilai;
int pulsa,diastole,sistole;

int data_puncak = 180;
int data_systole = 120;
int data_diastole = 70;
int data_detak = 35;

 
void setup()
{
//prosedur pemanggilan fungsi LCD
  lcd.begin(16, 2);//16 = Baris, 2 = kolom
  Serial.begin(9600);
  BLUETOOTH.begin(9600);
  pinMode(4, OUTPUT);//pompa
  pinMode(5, OUTPUT);//selenoid
  pinMode(3, INPUT_PULLUP);//tombol
 
  lcd.setCursor(00,00); //set pada baris 1 dan kolom 1
  lcd.print("TENSIMETER"); //menuliskan "nyebarilmu.com"
  lcd.setCursor(4,1);//set pada baris 2 dan kolom 1
  lcd.print("DIGITAL");
}
 
void loop() {
  mati();
  if(digitalRead(3)==LOW){
    Serial.println("LOW");
    pompa_manset();
  }
 loop();
}





int pompa_manset(){
  lcd.clear();
  lcd.setCursor(00,00); //set pada baris 1 dan kolom 1
  lcd.print("Memompa....");
  pompa();
  selonoid();
  Sensor_tekanan=analogRead(A0);
  if(Sensor_tekanan>data_puncak){ 
    mati_pompa();
    pengukuran();}
  pompa_manset();  
}

int pengukuran(){
  lcd.clear();
  lcd.setCursor(00,00); //set pada baris 1 dan kolom 1
  lcd.print("Calculating....");
  Sensor_tekanan=analogRead(A0);
  if(Sensor_tekanan>data_systole){
    sistole=Sensor_tekanan;
  }
  if(Sensor_tekanan>data_detak){
    bpm++;
  }
  if(Sensor_tekanan<data_diastole){
    diastole=Sensor_tekanan;
    //150=15 detik
    waktu=nilai/10;
    waktu_pulsa=60/waktu;
    pulsa=bpm*waktu_pulsa;
    mati();
    nilai=0;
    bpm=0;
    kirim_data();
  }
  delay(100);
  nilai++;
  pengukuran();
}


int kirim_data(){
  lcd.clear();
  lcd.setCursor(00,00); //set pada baris 1 dan kolom 1
  lcd.print("Tekanan :"+ String(sistole)+"/" + String(diastole));
  lcd.setCursor(0,1); 
  lcd.print("BPM     :"+ String(pulsa));
  
  BLUETOOTH.println("*|" + String(sistole) + "|" + String(diastole) + "|" + String(pulsa) + "|#");
  loop();
}



int mati_pompa(){
  digitalWrite(4, LOW); 
}

int pompa(){
  digitalWrite(4, HIGH);  
}

int selonoid(){
  digitalWrite(5, HIGH);  
}

int mati(){
  digitalWrite(4, LOW); 
  digitalWrite(5, LOW); 
}

