#include <Wire.h>
//#include <Timer.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4

/* C_X_MAX: 21, C_Y_MAX: 8, total 168 byte */
#define C_X_Dot     6
#define C_Y_Dot     8
#define C_X_MAX     128/C_X_Dot
#define C_Y_MAX     64/C_Y_Dot
#define SCROLL_LINE 1

#define ASCII_NUL   0x00
#define ASCII_BS    0x08
#define ASCII_LF    0x0A
#define ASCII_CR    0x0D
#define ASCII_ESC   0x1B
#define ASCII_SPACE 0x20


//Timer Timer;
Adafruit_SSD1306 display(OLED_RESET);

void setup()
{
  Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  Serial.ardprt(" C_X_MAX: %d, C_Y_MAX: %d\n", C_X_MAX, C_Y_MAX);    //TEST
}

int C_X = 0, C_Y = 0;
char Display_Array[C_Y_MAX][C_X_MAX];
void loop()
{
    // send data only when you receive data:
	if (Serial.available() > 0) {
        // read the incoming byte:
        char incomingByte = Serial.read();
        Serial.ardprt(" R: 0x%x\n", incomingByte);    //TEST

        switch (incomingByte) {
            //back  => clear one char
            case ASCII_BS:
                Serial.ardprt("[A]C_X: %d, C_Y: %d\n", C_X, C_Y);    //TEST
                if(0 == C_X)
                {
                    C_Y--;
                    C_X = (C_X_MAX-1);
                }
                else
                    C_X--;
                Serial.ardprt("[B]C_X: %d, C_Y: %d\n", C_X, C_Y);    //TEST
                display.setCursor(0, 0);
                display.clearDisplay();
                Display_Array[C_Y][C_X] = ASCII_SPACE;
                display.print(&Display_Array[0][0]);
                display.display();
                break;
            //Enter => Next line
            case ASCII_LF:  /* printf("\n"); */
            case ASCII_CR:
                //補空白鍵
                for(int index=C_X;C_X_MAX>index;index++)
                    Display_Array[C_Y][index] = ASCII_SPACE;
                C_X = 0;
                C_Y ++;
                break;
            //Esc   => clear Display
            case ASCII_ESC:
                C_X = 0;
                C_Y = 0;
                display.clearDisplay();
                //clean Display_Array
                for(int index_Y=0 ; C_Y_MAX>index_Y ; index_Y++)
                    for(int index_X=0 ; C_X_MAX>index_X ; index_X++)
                        Display_Array[index_Y][index_X] = ASCII_NUL;
                break;
            default: 
                display.setCursor(C_X_Dot*C_X, C_Y_Dot*C_Y);
                Display_Array[C_Y][C_X] = incomingByte;
                display.print(incomingByte);
                C_X++;
                break;
        }
    }
    display.display();

    if(C_X_MAX <= C_X)
    {
        C_X = 0;
        C_Y ++;
    }

    if(C_Y_MAX <= C_Y)
    {
        C_Y = 0;
        display.setCursor(0, 0);
        display.clearDisplay();
        display.print(&Display_Array[SCROLL_LINE][0]);
        display.display();

        //Update Display_Array
        for(int index_Y=0 ; C_Y_MAX>index_Y ; index_Y++)
        {
            for(int index_X=0 ; C_X_MAX>index_X ; index_X++)
            {
                if((C_Y_MAX-SCROLL_LINE)>index_Y)
                    Display_Array[index_Y][index_X] = Display_Array[index_Y + SCROLL_LINE][index_X];
                else
                    Display_Array[index_Y][index_X] = ASCII_SPACE;
            }
        }

        C_X = 0;
        C_Y = C_Y_MAX - SCROLL_LINE;
    }
}