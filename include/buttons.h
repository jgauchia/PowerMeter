/**
 * @file buttons.h
 * @author Jordi Gauchía (jgauchia@jgauchia.com)
 * @brief  Buttons definition
 * @version 0.1
 * @date 2023-01-30
 */

OneButton left_but = OneButton(D5, true, true);
OneButton mid_but = OneButton(D3, true, true);
OneButton right_but = OneButton(D4, true, true);

bool is_mainscreen = true;

/**
 * @brief Left Click Handle
 *
 */
static void leftClick()
{
}

/**
 * @brief Middle Click Handle
 *
 */
static void middleClick()
{
}

/**
 * @brief Middle Long Click Handle
 *
 */
static void middleLongClick()
{
    if (is_mainscreen)
        is_mainscreen = false;
    else
        is_mainscreen = true;
}

/**
 * @brief Right CLick Handle
 *
 */
static void rightClick()
{
}

/**
 * @brief Init Buttons
 *
 */
void init_buttons()
{
    left_but.attachClick(leftClick);
    mid_but.setPressTicks(500);
    mid_but.attachClick(middleClick);
    mid_but.attachLongPressStop(middleLongClick);
    right_but.attachClick(rightClick);
}

/**
 * @brief Get buttons ticks
 *
 */
void get_buttons()
{
    left_but.tick();
    mid_but.tick();
    right_but.tick();
}