
#include "i2c_lcd.h"


void lcd_command(uint8_t command)
{
  uint8_t high_nibble, low_nibble; //하위비트 상위비트 나눠줌
  uint8_t i2c_buffer[4];

  high_nibble = command & 0xf0;
  low_nibble = (command << 4) & 0xf0;

  i2c_buffer[0] = high_nibble | 0x04 | 0x08;
  //enable pin=1, rs pin=0, rw pin = 0, backlight=1
  i2c_buffer[1] = high_nibble | 0x00 | 0x08;
  //enable pin=0, rs pin=0, rw pin = 0, backlight=1
  i2c_buffer[2] = low_nibble  | 0x04 | 0x08;
  //enable pin=1, rs pin=0, rw pin = 0, backlight=1
  i2c_buffer[3] = low_nibble  | 0x00 | 0x08;
  //enable pin=0, rs pin=0, rw pin = 0, backlight=1

  while(HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS, i2c_buffer, 4, 100) != HAL_OK)
    {
      //HAL_Delay(1);
    }
}

void lcd_data(uint8_t data)
{
  uint8_t high_nibble, low_nibble; //하위비트 상위비트 나눠줌
  uint8_t i2c_buffer[4];

  high_nibble = data & 0xf0;
  low_nibble = (data << 4) & 0xf0;

  i2c_buffer[0] = high_nibble | 0x05 | 0x08;
  //enable pin=1, rs pin=1, rw pin = 0, backlight=1
  i2c_buffer[1] = high_nibble | 0x01 | 0x08;
  //enable pin=0, rs pin=1, rw pin = 0, backlight=1
  i2c_buffer[2] = low_nibble  | 0x05 | 0x08;
  //enable pin=1, rs pin=1, rw pin = 0, backlight=1
  i2c_buffer[3] = low_nibble  | 0x01 | 0x08;
  //enable pin=0, rs pin=1, rw pin = 0, backlight=1

  while(HAL_I2C_Master_Transmit(&hi2c1, I2C_LCD_ADDRESS, i2c_buffer, 4, 100) != HAL_OK)
    {
      //HAL_Delay(1);
    }

}

void i2c_lcd_init(void)
{
  HAL_Delay(50);
  lcd_command(0x33);
  HAL_Delay(5);
  lcd_command(0x32);
  HAL_Delay(5);
  lcd_command(0x28);
  HAL_Delay(5);
  lcd_command(DISPLAY_ON);
  HAL_Delay(5);
  lcd_command(0x06);
  HAL_Delay(5);
  lcd_command(CLEAR_DISPLAY);
  HAL_Delay(2);
}


void lcd_string(char *str)
{
  while(*str)
    { //문자열은 배열은 주소
      lcd_data(*str++);
    }
}

void move_cursor(uint8_t row, uint8_t col)
{
  lcd_command(0x80 | row<<6 | col);
}



/*
void LCD_Update(uint8_t cur, uint8_t tar, DoorState doorState)
{
    char line1[17], line2[17];
    char direction[8] = " ";

    if (tar > cur)
        strcpy(direction, "^");
    else if (tar < cur)
        strcpy(direction, "v");
    else
        strcpy(direction, " ");

    sprintf(line1, "Floor : %d -> %d %s", cur, tar, direction);
    // 두 번째 줄: 도어 상태

    switch (doorState)
    {
        case DOOR_OPENING:
        case DOOR_OPEN_WAIT:
            sprintf(line2, "DOOR OPEN  ");
            break;
        case DOOR_CLOSING:
            sprintf(line2, "DOOR CLOSE ");
            break;
        default:
            sprintf(line2, "STOP       ");
            break;
    }
    move_cursor(0, 0);
    lcd_string(line1);
    move_cursor(1, 0);
    lcd_string(line2);
}


void LCD_UPDOWN(uint8_t cur, uint8_t tar, const char* line2)
{
    //char line2[17];

    move_cursor(1, 0);
    lcd_string(line2);
}*/

























