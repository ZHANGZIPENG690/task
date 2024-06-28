#include <stdio.h>
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "lighting.h"
#include "core/fset_lib.h"
#include "core/application/light/display_manage.h"
#include <unistd.h>
#if !UNIT_TEST
#include "esp_err.h"
#endif

bool ledUpdate(void);
bool flash_delay_flag = false;
led_fifo ledStat_pre = {true, ledUpdate, {0}};
led_fifo ledStat_nex = {true, ledUpdate, {0}};

//*********************************************旧板子****************************************************//
const uint8_t addr_arr1[30] = {ADDR2, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1,
                               ADDR1, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2};

const uint8_t pin_arr1[30] = {8, 1, 2, 3, 4, 5, 6, 7, 8, 9,
                              10, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8};

//*********************************************新板子****************************************************//

const uint8_t addr_arr16[30] = {ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2,
                                ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1};
const uint8_t pin_arr16[30] = {5, 4, 3, 2, 1, 11, 10, 9, 8, 7,
                               6, 5, 4, 3, 2, 1, 12, 10, 9, 8, 7, 6};

//******************************************Pro机************************************************************//
const uint8_t addr_arr_pro[32] = {ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2,
                                  ADDR2, ADDR2, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR3, ADDR3, ADDR3, ADDR3, ADDR3, ADDR3, ADDR3, ADDR3};
const uint8_t pin_arr_pro[32] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 12, 11, 10, 9, 8, 7,
                                 6, 5, 4, 3, 2, 1, 8, 7, 6, 5, 4, 3, 2, 1};

//******************************************C10机************************************************************//
const uint8_t addr_arr_c10[18] = {ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2,
                                  ADDR2, ADDR2, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1, ADDR1};
const uint8_t pin_arr_c10[18] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 12, 11, 10, 9, 8, 7};

//******************************************D11 迷你机************************************************************//
const uint8_t addr_arr_d11[18] = {ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2, ADDR2};
const uint8_t pin_arr_d11[18] = {12, 11, 10, 9, 8, 7, 6, 5};

uint8_t arr4[] = {10, 14, 21, 3};
uint8_t arr6[] = {10, 12, 14, 21, 1, 3};
uint8_t arr8[] = {10, 12, 14, 16, 19, 21, 1, 3};
uint8_t arr12[] = {10, 11, 12, 13, 14, 15, 20, 21, 22, 1, 2, 3};
uint8_t arr16[] = {9, 10, 11, 12, 13, 14, 15, 16, 19, 20, 21, 22, 1, 2, 3, 4};

uint8_t arr32_pro[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32};
uint8_t arr30_pro[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 26, 27, 28, 29, 30, 31, 32, 1, 2, 3, 4, 5, 6, 7, 8};
uint8_t arr24_pro[] = {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 27, 28, 29, 30, 31, 32, 1, 2, 3, 4, 5, 6};
uint8_t arr18_pro[] = {13, 14, 15, 16, 17, 18, 19, 20, 21, 29, 30, 31, 32, 1, 2, 3, 4, 5};
uint8_t arr12_pro[] = {12, 14, 16, 18, 20, 22, 28, 30, 32, 2, 4, 6};
uint8_t arr6_pro[] = {12, 17, 22, 28, 1, 6};

uint8_t arr4_c10[] = {8, 11, 17, 2};
uint8_t arr6_c10[] = {6, 9, 12, 16, 1, 4};
uint8_t arr8_c10[] = {6, 8, 10, 12, 16, 18, 2, 4};
uint8_t arr12_c10[] = {7, 8, 9, 10, 11, 12, 16, 17, 18, 1, 2, 3};
uint8_t arr16_c10[] = {6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 17, 18, 1, 2, 3, 4};

uint8_t arr8_d11[] = {12, 11, 10, 9, 8, 7, 6, 5};

struct display_attribute_m display_attribute;

stat_m m_ext_drive_lighting_IIC_init(void)
{
  DEBUG_TEST(DB_I, " lighting IIC init \r\n ");
#if !UNIT_TEST
  esp_err_t err = m_callable_iic_init();

  if (err == ESP_OK)
  {
    return succ_r;
  }
  else
  {
    DEBUG_TEST(DB_I, "err: %s", esp_err_to_name(err));
    return fail_r;
  }

#endif
  return succ_r;
}

stat_m m_ext_drive_lighting_init(void)
{
  stat_m sta = fail_r;
  DEBUG_TEST(DB_I, " Led  init ... ");
#if !UNIT_TEST
  // m_ext_drive_lighting_IIC_init();
  if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B11 || display_attribute.display_device_version == DEVICE_HEARWARE_B12)
  {
    m_callable_iic_write(ADDR1, 0x4f, 0x00);
    m_callable_iic_write(ADDR1, 0x00, 0x00);
    mDelay_ms(1);
    m_callable_iic_write(ADDR1, 0x00, 0x01);
    m_callable_iic_write(ADDR1, 0x4a, 0x00);
    m_callable_iic_write(ADDR1, 0x4B, 0x00);
    m_callable_iic_write(ADDR2, 0x4f, 0x00);
    m_callable_iic_write(ADDR2, 0x00, 0x00);
    mDelay_ms(1);
    m_callable_iic_write(ADDR2, 0x00, 0x01);
    m_callable_iic_write(ADDR2, 0x4a, 0x00);
    m_callable_iic_write(ADDR2, 0x4B, 0x00);
    mDelay_ms(1);
    m_callable_iic_write(ADDR3, 0x4f, 0x00);
    m_callable_iic_write(ADDR3, 0x00, 0x00);
    mDelay_ms(1);
    m_callable_iic_write(ADDR3, 0x00, 0x01);
    m_callable_iic_write(ADDR3, 0x4a, 0x00);
    m_callable_iic_write(ADDR3, 0x4B, 0x00);
    sta = succ_r;
  }
  else
  {
    m_callable_iic_write(ADDR1, 0x4f, 0x00);
    m_callable_iic_write(ADDR1, 0x00, 0x00);
    mDelay_ms(1);
    m_callable_iic_write(ADDR1, 0x00, 0x01);
    m_callable_iic_write(ADDR1, 0x4a, 0x00);
    m_callable_iic_write(ADDR1, 0x4B, 0x00);
    m_callable_iic_write(ADDR2, 0x4f, 0x00);
    m_callable_iic_write(ADDR2, 0x00, 0x00);
    mDelay_ms(1);
    m_callable_iic_write(ADDR2, 0x00, 0x01);
    m_callable_iic_write(ADDR2, 0x4a, 0x00);
    m_callable_iic_write(ADDR2, 0x4B, 0x00);
    sta = succ_r;
  }
#endif
  return sta;
}

void m_ext_rgb_pin_2(uint8_t pin, uint32_t color)
{
#if !UNIT_TEST
  // DEBUG_TEST("设置 %hhd 号灯 颜色为 %d \r\n", pin, color);
  uint8_t addr = 0x00;
  // color = 0x500000;
  uint8_t R = ((color >> 16) & 0xff);
  uint8_t G = ((color >> 8) & 0xff);
  uint8_t B = ((color >> 0) & 0xff);
  if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B11 || display_attribute.display_device_version == DEVICE_HEARWARE_B12)
  {
    if (pin > 32)
      pin %= 32;
    if (pin == 0)
      pin = 1;
    addr = addr_arr_pro[pin - 1];
    pin = pin_arr_pro[pin - 1];
  }
  else if (display_attribute.display_device_version == DEVICE_HEARWARE_C11)
  {
    if (pin > 18)
      pin %= 18;
    if (pin == 0)
      pin = 1;
    addr = addr_arr_c10[pin - 1];
    pin = pin_arr_c10[pin - 1];
  }
  else if (display_attribute.display_device_version == DEVICE_HEARWARE_D11)
  {
    if (pin > 8)
      pin %= 8;
    if (pin == 0)
      pin = 1;
    addr = addr_arr_d11[pin - 1];
    pin = pin_arr_d11[pin - 1];
  }
  else
  {
    if (pin > 22)
      pin %= 22;
    if (pin == 0)
      pin = 1;
    if (display_attribute.display_device_version == DEVICE_HEARWARE_A001)
    {
      addr = addr_arr1[pin - 1];
      pin = pin_arr1[pin - 1];
    }
    else if (display_attribute.display_device_version == DEVICE_HEARWARE_A002||display_attribute.display_device_version == DEVICE_HEARWARE_A113)
    {
      addr = addr_arr16[pin - 1];
      pin = pin_arr16[pin - 1];
    }
    else
    {
      addr = addr_arr16[pin - 1];
      pin = pin_arr16[pin - 1];
    }
  }
  m_callable_iic_write(addr, 0x26 + (pin * 3) - 1, 0x03);
  m_callable_iic_write(addr, 0x01 + (pin * 3) - 1, R);
  m_callable_iic_write(addr, 0x26 + (pin * 3) - 2, 0x03);
  m_callable_iic_write(addr, 0x01 + (pin * 3) - 2, G);
  m_callable_iic_write(addr, 0x26 + (pin * 3) - 3, 0x03);
  m_callable_iic_write(addr, 0x01 + (pin * 3) - 3, B);

  m_callable_iic_write(addr, 0x25, 0x00);
  m_callable_iic_write(addr, 0x25, 0x00);
#endif
}

bool ledUpdate(void)
{
  // DEBUG_TEST("LED 节点更新\r\n");
#if !UNIT_TEST
  flash_delay_flag = true;
  if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B11 || display_attribute.display_device_version == DEVICE_HEARWARE_B12)
  {
    for (uint16_t i = 0; i < 33; i++)
    {
      if (ledStat_pre._LS_PRO[i] != ledStat_nex._LS_PRO[i])
      {
        m_ext_rgb_pin_2(i, ledStat_nex._LS_PRO[i]);
        ledStat_pre._LS_PRO[i] = ledStat_nex._LS_PRO[i];
        if (flash_delay_flag && ledStat_nex._LS_PRO[i] == 0)
        {
          m_ext_rgb_pin_2(i, ledStat_nex._LS_PRO[i]);
          flash_delay_flag = false;
        }
      }
    }
  }
  else
  {
    for (uint16_t i = 0; i < 23; i++)
    {
      if (ledStat_pre._LS[i] != ledStat_nex._LS[i])
      {
        m_ext_rgb_pin_2(i, ledStat_nex._LS[i]);
        ledStat_pre._LS[i] = ledStat_nex._LS[i];
        if (flash_delay_flag && ledStat_nex._LS[i] == 0)
        {
          m_ext_rgb_pin_2(i, ledStat_nex._LS[i]);
          flash_delay_flag = false;
        }
      }
    }
  }

  flash_delay_flag = false;
#endif
  return false;
}

void m_ext_rgb_pin_1(uint8_t pin, uint8_t red, uint8_t green, uint8_t blue)
{
#if !UNIT_TEST
  // DEBUG_TEST("设置 %hhd 号灯,%hhd红,  %hhd绿   %hhd蓝  \r\n" , pin, red, green, blue);
  uint8_t addr = 0x00;
  if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B11 || display_attribute.display_device_version == DEVICE_HEARWARE_B12)
  {
    if (pin > 32)
      pin %= 32;
  }
  else if (display_attribute.display_device_version == DEVICE_HEARWARE_C11)
  {
    if (pin > 18)
      pin %= 18;
  }
  else if (display_attribute.display_device_version == DEVICE_HEARWARE_D11)
  {
    if (pin > 8)
      pin %= 8;
  }
  else
  {
    if (pin > 22)
      pin %= 22;
  }
  if (pin == 0)
    pin = 1;

  if (display_attribute.display_device_version == DEVICE_HEARWARE_A001)
  {
    addr = addr_arr1[pin - 1];
    pin = pin_arr1[pin - 1];
  }
  else if (display_attribute.display_device_version == DEVICE_HEARWARE_A002||display_attribute.display_device_version == DEVICE_HEARWARE_A113)
  {
    addr = addr_arr16[pin - 1];
    pin = pin_arr16[pin - 1];
  }
  else if (display_attribute.display_device_version == DEVICE_HEARWARE_C11)
  {
    addr = addr_arr_c10[pin - 1];
    pin = pin_arr_c10[pin - 1];
  }
  else if (display_attribute.display_device_version == DEVICE_HEARWARE_D11)
  {
    addr = addr_arr_d11[pin - 1];
    pin = pin_arr_d11[pin - 1];
  }
  else if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B11 || display_attribute.display_device_version == DEVICE_HEARWARE_B12)
  {
    addr = addr_arr_pro[pin - 1];
    pin = pin_arr_pro[pin - 1];
  }
  else
  {
    addr = addr_arr16[pin - 1];
    pin = pin_arr16[pin - 1];
  }

  m_callable_iic_write(addr, 0x26 + (pin * 3) - 1, 0x03);
  m_callable_iic_write(addr, 0x01 + (pin * 3) - 1, red);
  m_callable_iic_write(addr, 0x26 + (pin * 3) - 2, 0x03);
  m_callable_iic_write(addr, 0x01 + (pin * 3) - 2, green);
  m_callable_iic_write(addr, 0x26 + (pin * 3) - 3, 0x03);
  m_callable_iic_write(addr, 0x01 + (pin * 3) - 3, blue);
  m_callable_iic_write(addr, 0x25, 0x00);
  m_callable_iic_write(addr, 0x25, 0x00);
#endif
}

void m_ext_drive_lighting_set_point_color(uint8_t point, uint32_t color)
{
#if !UNIT_TEST

  uint32_t adjustedColor = MY_COLOR_CONCOC(color);
  //  提取调整后的颜色通道值
  uint8_t redChannel = (adjustedColor * display_attribute.display_lighting_strength >> 16) & 0xFF;
  uint8_t greenChannel = (adjustedColor * display_attribute.display_lighting_strength >> 8) & 0xFF;
  uint8_t blueChannel = adjustedColor * display_attribute.display_lighting_strength & 0xFF;
  if (display_attribute.display_device_version == DEVICE_HEARWARE_A003 || display_attribute.display_device_version == DEVICE_HEARWARE_B11 || display_attribute.display_device_version == DEVICE_HEARWARE_B12 || display_attribute.display_device_version == DEVICE_HEARWARE_C11||display_attribute.display_device_version == DEVICE_HEARWARE_D11)
  {
    // 设置灯的颜色
    m_ext_rgb_pin_1(point, blueChannel, greenChannel, redChannel);
  }
  else
  {
    // 设置灯的颜色
    m_ext_rgb_pin_1(point, redChannel, greenChannel, blueChannel);
  }
#endif
}

void m_ext_drive_lighting_set_range_color(uint8_t s_point, uint8_t e_point, uint32_t color)
{
#if !UNIT_TEST
  for (size_t pin = s_point; pin <= e_point; pin++)
    m_ext_drive_lighting_set_point_color(pin, color);
#endif
}

void m_ext_drive_lighting_blink_point(uint8_t blink_param, uint8_t point, uint32_t color)
{
#if !UNIT_TEST
  m_ext_drive_lighting_set_point_color(point, color);
  while (1)
  {
    mDelay_ms(blink_param);
    m_ext_drive_lighting_set_point_color(point, black);
    mDelay_ms(blink_param);
    m_ext_drive_lighting_set_point_color(point, color);
  }
#endif
}

void m_ext_lighting_blink_range_color(uint8_t blink_param, uint8_t s_point, uint8_t e_point, uint32_t color)
{
#if !UNIT_TEST
  for (uint8_t point = s_point; point <= e_point; point++)
  {
    m_ext_drive_lighting_set_point_color(point, color);
  }
  while (1)
  {
    mDelay_ms(blink_param);
    for (uint8_t point = s_point; point <= e_point; point++)
    {
      m_ext_drive_lighting_set_point_color(point, black);
    }
    mDelay_ms(blink_param);
    for (uint8_t point = s_point; point <= e_point; point++)
    {
      m_ext_drive_lighting_set_point_color(point, color);
    }
  }
#endif
}
