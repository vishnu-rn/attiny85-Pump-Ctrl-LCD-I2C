#include <LiquidCrystal_I2C.h>
#include <USI_TWI_Master.h>
#include <tinywirem.h>

#define l_sw 3
#define h_sw 4
#define relay 1

bool l_sw_stat = 1;
bool h_sw_stat = 0;

char *l_sw_txt = "ERR";
char *h_sw_txt = "ERR";

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setCur(uint8_t x = 0, uint8_t y = 0)
{
    delay(100);
    lcd.setCursor(x, y);
}

void clr()
{
    delay(100);
    lcd.clear();
}

void disp(const char *a)
{
    delay(100);
    lcd.print(a);
}

void write(uint8_t a)
{
    delay(100);
    lcd.write(a);
}

void transition(uint8_t code = 255, uint8_t halt = 100)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        setCur(i, 0);
        write(code);
        setCur(15 - i, 1);
        write(code);
        delay(halt);
    }
    for (uint8_t i = 0; i < 16; i++)
    {
        setCur(i, 0);
        disp(" ");
        setCur(15 - i, 1);
        disp(" ");
        delay(halt);
    }
}

void transition_low_only(uint8_t code = 255, uint16_t halt = 100)
{
    for (uint8_t i = 0; i < 16; i++)
    {
        setCur(i, 1);
        write(code);
        delay(halt);
    }
    for (uint8_t i = 0; i < 16; i++)
    {
        setCur(i, 1);
        disp(" ");
        delay(halt);
    }
}

void lcd_init()
{
    lcd.begin(16, 2);
    lcd.backlight();
    clr();
    setCur(2, 0);
    disp("Initializing");
    transition_low_only(255, 2000);
    delay(15000);
    transition();
    setCur();
    disp("Pump Ctrl Active");
    setCur(0, 1);
    disp("----------------");
    delay(50000);
    transition();
}

void disp_stat()
{
    setCur();
    disp("Low level  - ");
    disp(l_sw_txt);
    setCur(0, 1);
    disp("High level - ");
    disp(h_sw_txt);
}

void update_lvl()
{
    delay(100);
    l_sw_stat = digitalRead(l_sw);
    h_sw_stat = digitalRead(h_sw);
    if (l_sw_stat == 1)
        l_sw_txt = "ACT";
    else
        l_sw_txt = "NOR";
    if (h_sw_stat == 1)
        h_sw_txt = "NOR";
    else
        h_sw_txt = "ACT";
}

void pump_start()
{
    clr();
    delay(10000);
    digitalWrite(relay, HIGH);
    setCur();
    disp("--PUMP RUNNING--");
    while (h_sw_stat)
    {
        transition_low_only(255, 1000);
        update_lvl();
    }
    clr();
    delay(5000);
    digitalWrite(relay, LOW);
    setCur();
    disp("Tank Topped Up.");
    setCur(0, 1);
    disp("--Pump Stopped--");
    delay(50000);
    clr();
}

bool float_check()
{
    delay(1000);
    clr();
    transition();
    setCur();
    disp("Float Check...");
    transition_low_only(255, 1000);
    delay(10000);
    update_lvl();
    if (!h_sw_stat)
    {
        while (true)
        {

            setCur();
            disp("Float Error!!!");
            setCur(0, 1);
            disp("Clean Floats..");
            delay(20000);
            transition_low_only();
            setCur(0, 1);
            disp("Restart Module..");
            delay(20000);
            transition_low_only();
        }
    }
    else
    {
        setCur(0, 1);
        disp("Float Check OK!");
        delay(20000);
        transition();
        return true;
    }
}

void setup()
{
    lcd_init();
    pinMode(l_sw, INPUT_PULLUP);
    pinMode(h_sw, INPUT_PULLUP);
    pinMode(relay, OUTPUT);
    clr();
}

void loop()
{
    update_lvl();
    disp_stat();
    delay(20000);
    if (l_sw_stat == 1)
    {
        transition();
        setCur();
        disp("Debounce-Low Lvl");
        transition_low_only(255, 1500);
        delay(10000);
        update_lvl();
    }
    if (l_sw_stat == 1)
    {
        if (float_check())
            pump_start();
    }
}
