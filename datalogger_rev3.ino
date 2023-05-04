#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <RTClib.h>  //BIBLIOTECA PARA O MÓDULO I2C RTC (RELÓGIO DE TEMPO REAL)
RTC_DS1307 rtc;
#include <SD.h>  //BIBLIOTECA PARA O MÓDULO SPI DE CARTÃO SD
// SD_FAT_TYPE = 0 for SdFat/File as defined in SdFatConfig.h,
// 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#include <SPI.h>              //BIBLIOTECA PARA COMUNICAÇÃO SPI - CS, MISO, MOSI, SCK...
#include <Adafruit_BME280.h>  //BIBLIOTECA PARA O MÓDULO I2C BME280,
Adafruit_BME280 bme;
#define pino_umi A0  //DEFINE O PINO ANALÓGICO DO SENSOR DE UMIDADE DO SOLO
#define pin_ilumi A1
#define pinBotao 2
// byte min_ini;    //MOMENTO DE GRAVAÇÃO
// byte min_atual;  //MOMENTO DE GRAVAÇÃO
byte hor_gravacao;
char data_arq_ini[7]; //DATA INICIAL
char data_arq_nova[7];

File datalog;
byte grau[8] = {
  B11100,
  B10100,
  B11100,
  B00011,
  B00100,
  B00100,
  B00100,
  B00011,
};

byte ur[8] = {
  B10100,
  B10100,
  B11100,
  B00111,
  B00101,
  B00111,
  B00110,
  B00101,
};
byte mb[8] = {
  B01010,
  B10101,
  B10101,
  B00000,
  B00100,
  B00110,
  B00101,
  B00110,
};

char dia[3], mes[3], ano[3], hora[3], minuto[3], segundo[3];  //Tempo formatado
void setup() {
  Serial.begin(9600);
  pinMode(pinBotao, INPUT_PULLUP);
  pinMode(pin_ilumi, INPUT);
  bme.begin(0x76);
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.clear();
  lcd.createChar(0, grau);  //0 = CARACTERE grau (ºC)
  lcd.createChar(1, ur);    //1 = CARACTERE UR (UMIDADE RELATIVA)
  lcd.createChar(2, mb);    //2 = caractere MB (milibars)
  rtc.begin();
  DateTime now = rtc.now();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //Ajuste do tempo
  // min_ini = min_atual;
  hor_gravacao = 100;  
}
void loop() {
  if (digitalRead(pinBotao) == LOW) {
    escreveLCD();
    delay(250);
  } else {
    escreveLCD2();
    delay(5000);
    escreveLCD3();
    delay(5000);
  }
  delay(1000);
  DateTime now = rtc.now();
  //SE DIA E MÊS TIVEREM APENAS UM DÍGITO, UM ZERO É ACRESCENTADO NA FRENTE DELE
  if (now.day() < 10)
    sprintf(dia, "0%1d", now.day());
  else
    sprintf(dia, "%2d", now.day());

  if (now.month() < 10)
    sprintf(mes, "0%1d", now.month());
  else
    sprintf(mes, "%2d", now.month());

  sprintf(ano, "%2d", now.year() - 2000);

  if (now.hour() < 10)
    sprintf(hora, "0%1d", now.hour());
  else
    sprintf(hora, "%2d", now.hour());

  if (now.minute() < 10)
    sprintf(minuto, "0%1d", now.minute());
  else
    sprintf(minuto, "%2d", now.minute());
  // min_atual = now.minute();



  // if ((min_atual > min_ini) || (min_atual == 0 && min_ini == 59)) {
    if(now.hour() != hor_gravacao){    
    hor_gravacao = now.hour();
    DateTime now = rtc.now();  //LEITURA DE DATA E HORA ATUAIS DO RTC
    byte seg = now.second();   //TEMPO (SEGUNDOS) ANTES DE TENTAR GRAVAR NO SD
    // min_ini = min_atual;
    sd_card();  //SALVA OS RESULTADOS NO SD
    //SE TEMPO DE GRAVAÇÃO NO SD FOR SUPERIOR A 5 SEGUNDOS SINALIZA ERRO (SD SEM ESPAÇO)
    if (now.second() > seg + 5) {
      Serial.println("ERRO: cartão SD sem espaço");
    } else
      return;
  }

}
int PressaoAtm() {
  return bme.readPressure() / 100;  //LEITURA E CONVERSÃO DA PRESSÃO ATMOSFÉRICA
}
float iluminancia() {
  float volts, amps, microamps, lux;
  volts = analogRead(pin_ilumi) * 5.0 / 1024.0;
  amps = volts / 10000.0;
  microamps = amps * 1000000;
  lux = microamps * 2.0;
  return lux;
}
float luz_ambiente() {
  float luz;
  int valor_luz;
  valor_luz = analogRead(pin_ilumi);
  luz = valor_luz * 0.0976;
  return luz;
}
int UmidadeAr() {
  return bme.readHumidity();
}
float Temperatura() {
  float temp = bme.readTemperature();
  return temp;
}
char* UmidadeSolo() {
  int umidadeSolo = analogRead(pino_umi);
  if (umidadeSolo < 500) {
    return "Alta";
  } else if (umidadeSolo < 800) {
    return "Media";
  } else return "Baixa";
}
int analogUmi() {
  return analogRead(pino_umi);
}
void escreveLCD() {
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print(dia);
  lcd.print('/');
  lcd.print(mes);
  lcd.print('/');
  lcd.print(ano);
  lcd.print(' ');
  lcd.print(hora);
  lcd.print(F((":")));
  lcd.print(minuto);

  lcd.setCursor(0, 1);
  lcd.print(UmidadeAr());
  lcd.print('%');
  lcd.write(byte(1));
  lcd.print(' ');
  lcd.print(Temperatura());
  lcd.write(byte(0));
  lcd.print(' ');
  lcd.print(PressaoAtm());
  lcd.write(byte(2));
}
void escreveLCD2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UmiSolo: ");
  lcd.print(UmidadeSolo());
  lcd.setCursor(0, 1);
  lcd.print("Analog: ");
  lcd.print(analogUmi());
}
void escreveLCD3() {
  float luz;
  int valor_luz;
  valor_luz = analogRead(pin_ilumi);
  luz = valor_luz * 0.0976;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Luz_ambi:");
  lcd.print(luz);
  lcd.print('%');
  lcd.setCursor(0, 1);
  float volts, amps, microamps, lux;
  volts = valor_luz * 5.0 / 1024.0;
  amps = volts / 10000.0;
  microamps = amps * 1000000;
  lux = microamps * 2.0;
  lcd.print("ilu: ");
  lcd.print(lux);
  lcd.print(" lux");
}
void sd_card() {
  DateTime now = rtc.now();
  while (!Serial) {
    ;  // Espera a porta serial conectar
  }
  Serial.print(F("Inicializando cartão SD..."));
  if (!SD.begin(53)) {
    Serial.println("Falha na inicialização!");
    while (1)
      ;
  }char nome_arq[13];  //NOME DO ARQUIVO A SER SALVO NO SD:
                        //MÁXIMO 8 CARACTERES + '.' + EXTENSÃO "csv"
    
    sprintf(data_arq_nova, "%s%s%s", ano, mes, dia); //FORMATA DATA CORRENTE COMO AAMMDD 
                                                     //PARA FACILITAR A INDEXAÇÃO DOS ARQUIVOS

   //FORAM USADAS CONVERSÕES DE STRING PARA LONG (ATOL)
   //PORQUE AS STRINGS AAMMDD TEM 6 DÍGITOS E SÃO MAIORES QUE UM INTEIRO (INT)
   if(atol(data_arq_nova) > atol(data_arq_ini))  //se a data no momento da gravação for superior à lida na 
      {                                          //inicialização, atualiza data de inicialização 
        sprintf(nome_arq, "%s%s%s", "VA", data_arq_nova, ".csv"); //NOME ARQUIVO = DATA ATUALIZADA
                                                                  //VAAAMMDD.csv
        sprintf(data_arq_ini, "%s", data_arq_nova);               //(PASSOU DE 1/2 NOITE)  
      }
   else
        sprintf(nome_arq, "%s%s%s", "VA", data_arq_ini, ".csv"); //NOME ARQUIVO = DATA DA INICIALIZAÇÃO
                                                                //(NÃO PASSOU DE 1/2 NOITE) 
 
  Serial.println("Inicialização feita.");
  datalog = SD.open(nome_arq, FILE_WRITE);
  if (datalog) {
    //GRAVA DADOS NO SD
    datalog.print(dia);
    datalog.print("/");
    datalog.print(mes);
    datalog.print("/");
    datalog.print(ano);
    datalog.print(",");
    datalog.print(hora);
    datalog.print(":");
    datalog.print(minuto);
    datalog.print(",");
    datalog.print(UmidadeAr());
    datalog.print("%");
    datalog.print(",");
    datalog.print(Temperatura());
    datalog.print(",");
    datalog.print(UmidadeSolo());
    datalog.print(",");
    datalog.print(analogUmi());
    datalog.print(",");
    datalog.print(PressaoAtm());
    datalog.print(",");
    datalog.print(luz_ambiente());
    datalog.print(",");
    datalog.println(iluminancia());

    datalog.close();

  } else {
    // Se este arquivo não abriu, imprima um erro:
    Serial.println("Erro ao abrir datalog.csv");
  }

  // Abre novamente o arquivo para leitura:
  // datalog = SD.open(nome_arq);
  // if (datalog) {
  //   Serial.println(nome_arq);
  //   // Lê do arquivo até não ter mais nada nele:
  //   while (datalog.available()) {
  //     Serial.write(datalog.read());
  //   }
  //   // fecha o arquivo:
  //   datalog.close();
  // } else {
  //   // Se este arquivo não abriu, imprima um erro:
  //   Serial.println("Erro ao abrir datalog.csv");
  //   while (datalog.available()) {
  //     Serial.write(datalog.read());
  //   }
  //   datalog.close();
  // }
}