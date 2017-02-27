#include <SPI.h>
#include <ILI9341_t3.h>

//SPI
#define SPI_MOSI    11
#define SPI_SCLK    13
#define SPI_MISO    12

//TFT
#define TFT_DC      9
#define TFT_CS      10
#define TFT_RST     8
#define TFT_LED_PWR -1

//TFT
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC, TFT_RST, SPI_MOSI, SPI_SCLK, SPI_MISO);

volatile unsigned long StopTime;
unsigned long StartTime, Measure;
bool StartMeas;
double Freq;
volatile bool Measured;
double Min,Max;
unsigned int Count;
    
 void setup()
 {
    Serial.begin(9600);
     pinMode(TFT_CS, INPUT_PULLUP);
     pinMode(TFT_LED_PWR, OUTPUT);
     pinMode(0, INPUT);
     attachInterrupt(0, RPMCount, RISING);

     //Setup SPI
  SPI.setMOSI(SPI_MOSI);
  SPI.setMISO(SPI_MISO);
  SPI.setSCK(SPI_SCLK);

  //Setup TFT 
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.setRotation(1);

  
     StopTime = 0;
     StartTime = 0;
     Measure = 1;
     Min= 100;
     Max= 0;
     Measured = false;
     Count =0;

     Serial.println("Start");
     tft.println("Start");


 }
 
 void loop()
 {
  if (Measured)
  {
    noInterrupts();
    if ((Count % 3) == 0)
      {
        Measure = StopTime - StartTime;
        StartTime = StopTime;
    
        Freq = 1000000.0/(double)Measure;

      
        Measured = false;

        if (Count / 100 == 1)
        {
          Count=0;
          if (Freq < Min) Min = Freq;
          if (Freq > Max) Max = Freq;
          char Frequency[32];
          sprintf(Frequency,"Freq : %.4f",Freq);
          char Minimum[32];
          sprintf(Minimum,"Min  : %.4f",Min);
          char Maximum[32];
          sprintf(Maximum,"Max  : %.4f",Max);
          tft.setCursor(0,0);
          tft.fillScreen(ILI9341_BLACK);
          tft.println(Minimum);
          tft.println(Frequency);
          tft.println(Maximum);
          Serial.print(Freq);
          Serial.println(", 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25");
        }
      }
      interrupts();
    }
 }

 void RPMCount()
 {
   StopTime = micros();
   Count ++;
   Measured = not(Measured);
 }


