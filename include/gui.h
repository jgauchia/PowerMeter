/**
 * @file gui.h
 * @author Jordi Gauchía (jgauchia@jgauchia.com)
 * @brief  Display functions
 * @version 0.1
 * @date 2023-02-03
 */

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
#define LCDWidth u8g2.getDisplayWidth()
#define ALIGN_CENTER(t) ((LCDWidth - (u8g2.getUTF8Width(t))) / 2)
#define ALIGN_RIGHT(t) (LCDWidth - u8g2.getUTF8Width(t))
#define ALIGN_LEFT 0

int current_hour = 0;
int old_hour = 0;

/**
 * @brief Display Main Screen
 *
 */
void show_main_screen()
{
    char time[24];
    sprintf(time, "%02d:%02d:%02d - %02d/%02d/%04d", tm.tm_hour, tm.tm_min, tm.tm_sec, tm.tm_mday, (tm.tm_mon + 1), (tm.tm_year + 1900));

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(0, 10, time);

    u8g2.setFont(u8g2_font_open_iconic_app_1x_t);
    u8g2.drawStr(0, 32, "H");

    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.drawStr(10, 32, JSONdata[current_hour].value_hour);

    u8g2.setFont(u8g2_font_open_iconic_all_1x_t);
    u8g2.setCursor(45, 32);
    u8g2.print(char(158));

    u8g2.setFont(u8g2_font_5x8_tr);
    u8g2.setCursor(55, 32);
    u8g2.print(JSONdata[current_hour].value_price, 4);
    u8g2.drawStr(90, 32, "KW/h");

    u8g2.setFont(u8g2_font_open_iconic_www_2x_t);
    if (JSONdata[current_hour].value_is_cheap == 1)
        u8g2.drawStr(115, 32, "I");
    else if (JSONdata[current_hour].value_is_cheap == 0)
        u8g2.drawStr(115, 32, "R");
    u8g2.setFont(u8g2_font_open_iconic_arrow_1x_t);
    if (JSONdata[current_hour].value_is_under_avg == 1)
        u8g2.drawStr(120, 10, "G");
    else if (JSONdata[current_hour].value_is_under_avg == 0)
        u8g2.drawStr(120, 10, "D");
    u8g2.sendBuffer();
}

/**
 * @brief Update OLED info
 *
 */
void update_oled()
{

    if (is_mainscreen)
    {
        show_main_screen();
    }
    else
    {
        u8g2.clearBuffer();
        u8g2.sendBuffer();
    }
}