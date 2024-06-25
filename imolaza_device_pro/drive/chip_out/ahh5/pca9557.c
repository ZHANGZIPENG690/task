#include "pca9557.h"
#if !UNIT_TEST
#include "esp_err.h"
#include "driver/i2c.h"
#endif

uint16_t PCA9557_SLAVE_ADDR = AD554_1;
static int ahh5_vue = 0;
stat_m m_ext_drive_ahh5_init(void)
{
#if !UNIT_TEST
    return m_ext_drive_iic_init();
#endif
}

stat_m m_ext_drive_ahh5_reset(void)
{
#if !UNIT_TEST
    if (m_callable_iic_write(PCA9557_SLAVE_ADDR << 1 | I2C_MASTER_WRITE, PCA9557_CONTROL_REG_1, 0) == ESP_OK)
    {
        return succ_r;
    }
    else
    {
        return fail_r;
    }
#endif
}
stat_m m_ext_drive_ahh5_solenoidvalve_set(uint8_t bit)
{
#if !UNIT_TEST
    stat_m stat = fail_r;

    stat = m_callable_iic_write(PCA9557_SLAVE_ADDR << 1 | I2C_MASTER_WRITE, PCA9557_CONTROL_REG_3, 0x0);
    // vTaskDelay(pdMS_TO_TICKS(5));
    stat = m_callable_iic_write(PCA9557_SLAVE_ADDR << 1 | I2C_MASTER_WRITE, PCA9557_CONTROL_REG_1, bit);
    if (stat == succ_r)
    {
        return stat;
    }
    else
    {
        stat = m_callable_iic_write(AD554_2, PCA9557_CONTROL_REG_1, bit);
    }
    return stat;
#endif
}

stat_m m_ext_drive_ahh5_solenoidvalve_reset(uint8_t bit)
{
    stat_m stat = fail_r;
    ahh5_vue = 0;
#if !UNIT_TEST
    esp_err_t ret;
    uint8_t current_state = 0;
    stat = m_callable_iic_write(PCA9557_SLAVE_ADDR << 1 | I2C_MASTER_WRITE, PCA9557_CONTROL_REG_3, 0x0);
    // vTaskDelay(pdMS_TO_TICKS(5));
    stat = m_callable_iic_write(PCA9557_SLAVE_ADDR << 1 | I2C_MASTER_WRITE, PCA9557_CONTROL_REG_1, 0x00);
    if (stat == succ_r)
    {
        return stat;
    }
    else
    {
        stat = m_callable_iic_write(AD554_2, PCA9557_CONTROL_REG_1, bit);
    }
    return stat;
#endif
}

stat_m m_ext_drive_ahh5_solenoidvalve_two_bit(PinBit bito1, PinBit bit2, uint8_t on_off)
{
    printf("ahh5_SolenoidValve_two_bit(%d--%d)", bito1, bit2);
#if !UNIT_TEST
    uint8_t abb = 0x00;
    if (on_off)
        abb |= 1 << (bito1 - 1);
    abb |= 1 << (bit2 - 1);
    return m_ext_drive_ahh5_solenoidvalve_set(abb);
#endif
}

stat_m m_ext_drive_ahh5_solenoidvalve(PinBit bit, uint8_t on_off)
{
#if !UNIT_TEST
    uint8_t abb = 0x00;
    abb |= 1 << (bit - 1);
    if (on_off)
    {
        ahh5_vue = abb;
        return m_ext_drive_ahh5_solenoidvalve_set(abb);
    }

    else
    {
        ahh5_vue = 0;
        return m_ext_drive_ahh5_solenoidvalve_reset(abb);
    }

#endif
}

/**
 * @brief
 *
 * @param bit
 * @param on_off
 * @return stat_m
 */
stat_m m_ext_drive_ahh5_solenoidvalve_independent(PinBit bit, uint8_t on_off)
{
    stat_m stat = fail_r;
    uint8_t abb = 0x00;
    abb |= 1 << (bit - 1);

    if (on_off)
    {
        ahh5_vue |= abb;
        // printf("***************%d*********\r\n", ahh5_vue);
        stat = m_ext_drive_ahh5_solenoidvalve_set(ahh5_vue);
    }

    else
    {
        ahh5_vue &= (~abb);
        //  printf("***************%d*********\r\n", ahh5_vue);
        stat = m_ext_drive_ahh5_solenoidvalve_reset(ahh5_vue);
    }

    return stat;
}

stat_m m_ext_drive_pca9557_setiostate(PinBit pinBit, uint8_t x)
{
    stat_m stat = fail_r;
    // uint8_t current_state = 0;
    // // m_callable_iic_write(PCA9557_SLAVE_ADDR, PCA9557_CONTROL_REG_3, current_state);
    // // current_state = 0;
    // ret = m_ext_iic_read(PCA9557_SLAVE_ADDR << 1 | I2C_MASTER_WRITE, PCA9557_CONTROL_REG_1, &current_state, 1);
    // if (ret != ESP_OK)
    // {
    //     ret = m_ext_iic_read(AD554_2 << 1 | I2C_MASTER_WRITE, PCA9557_CONTROL_REG_1, &current_state, 1);
    //     if (ret != ESP_OK)
    //         io_chip = chip_hc595;
    // }
    // if (x == 0) // �?0 与违算得�?
    //     current_state &= ~(1 << pinBit);
    // else
    //     current_state |= (1 << pinBit);
    // ret = m_callable_iic_write(PCA9557_SLAVE_ADDR << 1 | I2C_MASTER_WRITE, PCA9557_CONTROL_REG_1, current_state);
    // if (ret != ESP_OK)
    // {
    //     ret = m_callable_iic_write(AD554_2 << 1 | I2C_MASTER_WRITE, PCA9557_CONTROL_REG_1, current_state);
    //     if (ret != ESP_OK)
    //         io_chip = chip_hc595;
    //     return ret;
    // }
    return stat;
}

// void app_main(void)
// {
//     ahh5_i2c_master_init();

//         for (int i = 0; i <= 8; i++)
//         {
//             SolenoidValve(i, 1);
//             vTaskDelay(pdMS_TO_TICKS(200));
//             printf("666666666666\r\n");
//         }
//         vTaskDelay(pdMS_TO_TICKS(1000));
//         m_ahh5_SolenoidValve_reset(0x00);
//         printf("OKKKKKK\r\n");
//         vTaskDelay(pdMS_TO_TICKS(200));

// }

// void app_main(void)
// {
//     uint8_t a = 1;
//     ahh5_i2c_master_init();
//     resetPCA9557();

//     for (int i = 0; i <= 8; i++)
//     {
//         ahh5_SolenoidValve_two_bit(i, i+1, 1);
//          vTaskDelay(pdMS_TO_TICKS(2000));
//     }

//     printf("666666666666\r\n");

//     vTaskDelay(pdMS_TO_TICKS(2000));
//     m_ahh5_SolenoidValve_reset(0x00);
//     printf("OKKKKKK\r\n");
//     vTaskDelay(pdMS_TO_TICKS(200));
// }
