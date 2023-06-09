# Pandora_STM32L475_Zephyr

Adapt Zephyr RTOS App for Pandora STM32L475 Development Board

## Hardware

Hardware: [Pandora STM32L475 Development Board](http://www.openedv.com/docs/boards/iot/zdyz_panduola.html)

Zephyr: [Zephyr Project Supported Boards - ST STM32L475 Pandora¶](https://docs.zephyrproject.org/latest/boards/arm/pandora_stm32l475/doc/index.html)

![Pandora STM32L475 Development Board](./assets/images/hardware/Pandora_STM32L475.png)


# Developing With Zephyr

- [Zephyr Project Documentation](https://docs.zephyrproject.org/latest/index.html)

- [Getting Started Guide](https://docs.zephyrproject.org/latest/develop/getting_started/index.html)

- [Zephyr Project Supported Boards](https://docs.zephyrproject.org/latest/boards/index.html)

## Install Zephyr SDK

## Build and run Test

```shell
cd ~/zephyrproject/zephyr
west build -p always -b pandora_stm32l475 samples/basic/blinky
west flash
```

# Build Zephyr RTOS App for Pandora STM32L475 Development Board

```shell
source ~/zephyrproject/.venv/bin/activate
export ZEPHYR_BASE=~/zephyrproject/zephyr
```

## Build and run Test

```shell
cd projects/getting_started/blinky
west build -p always -b pandora_stm32l475
west flash
```

# Samples list

> Adapt Zephyr RTOS App for Pandora STM32L475 Development Board

| Status | Sample | Description | Notes |
| ------ | ------ | ----------- | ----- |
| ✅ Done | [getting_started/blinky](./projects/getting_started/blinky) | Blinky | |
| ✅ Done | [getting_started/button](./projects/getting_started/button) | Hello World | |
| ✅ Done | [drivers/display](./projects/drivers/display) | Display | |
| :construction: Todo | [drivers/gpio](./projects/drivers/gpio) | GPIO | |
| :construction: Todo | [drivers/i2c](./projects/drivers/i2c) | I2C | |
| ✅ Done | [drivers/i2c/shell_scan](./projects/drivers/i2c/shell_scan) | I2C Scan | |
| :construction: Todo | [drivers/pwm](./projects/drivers/pwm) | PWM | |
| :construction: Todo | [drivers/uart](./projects/drivers/uart) | UART | |
| ✅ Done | [drivers/spi_flash](./projects/drivers/spi_flash) | SPI Flash | |
| ✅ Done | [lvgl/lvgl](./projects/lvgl/lvgl) | LVGL | |
| ✅ Done | [subsys/shell/shell_module](./projects/subsys/shell/shell_module) | Shell | |
| ✅ Done | [subsys/shell/devmem_load](./projects/subsys/shell/devmem_load) | Shell | |
| 🐛 Debugging | [subsys/usb/cdc_acm](./projects/subsys/usb/cdc_acm) | USB CDC ACM | |



## Peripheral support

The Zephyr stm32l475ve_pandora board configuration supports the following hardware features:

| No. | Support | Peripheral | Interface | Controller | Driver/Component                |
| --- | ------- | ---------- | --------- | ---------- | ------------------------------- |
| 1.  | ✅ Done | NOR FLASH  | QSPI  | on-chip | Winbond W25Q128JVQIQ 128Mbit NOR FLASH |
| 2.  | ✅ Done | LCD-TFT    | SPI   | on-chip | ST7789V 240x240 LCD-TFT |
| 3.  | :construction: Todo | AHT10      | I2C   | on-chip | AHT10 Temperature and humidity sensor |
| 4.  | :construction: Todo | ICM2068    | I2C   | on-chip | ICM2068 light environment sensor |
| 5.  | :construction: Todo | ES8388     | I2S   | on-chip | ES8388 Audio Decoder |
| 6.  | :construction: Todo | USB-OTG    | USB   | on-chip | OTG USB |
| 7.  | :construction: Todo | AP6181     | SDIO  | on-chip | AP6181 WIFI |
| 8.  | :construction: Todo | ICM20608 | I2C  | on-board | ICM20608 6-axis posture sensor |
| 9.  | :construction: Todo | IR-RX      | GPIO  | on-board | Infrared Receiver(38Khz) |
| 10. | :construction: Todo | IR-TX      | GPIO  | on-board | Infrared Transmitter |
| 11. | ✅ Done | LED        | GPIO  | on-board | LED |
| 12. | ✅ Done | KEY        | GPIO  | on-board | KEY |
| 13. | :construction: Todo | MIC        | GPIO  | on-board | MIC |
| 14. | :construction: Todo | Buzzer     | GPIO  | on-board | Buzzer |
| 15. | :construction: Todo | RTC        | RTC   | on-chip | RTC |
| 16. | :construction: Todo | Motor      | GPIO  | on-board | Motor |
| 17. | :construction: Todo | TF-Card    | SPI  | on-board | TF-Card |

### I2C Devcies

| No. | Support | Peripheral | Bus | Controller | Driver/Component                |
| --- | ------- | ---------- | --------- | ---------- | ------------------------------- |
| 1.  | :construction: Done | AHT10      | GPIO | on-chip | AHT10 Temperature and humidity sensor |
| 2.  | :construction: Done | ICM2068    | I2C3   | on-chip | ICM2068 light environment sensor |
| 3.  | :construction: Done | ES8388     | I2C3   | on-chip | ES8388 Audio Decoder |
| 4.  | :construction: Todo | ICM20608 | I2C3  | on-board | ICM20608 6-axis posture sensor |

### SPI Devcies

| No. | Support | Peripheral | Interface | Controller | Driver/Component                |
| --- | ------- | ---------- | --------- | ---------- | ------------------------------- |
| 1.  | ✅ Done | NOR FLASH  | QSPI  | on-chip | Winbond W25Q128JVQIQ 128Mbit NOR FLASH |
| 2.  | ✅ Done | LCD-TFT    | SPI   | on-chip | ST7789V 240x240 LCD-TFT |
| 3.  | :construction: Todo | TF-Card    | SDIO  | on-board | TF-Card |

### USB Devcies

| No. | Support | Peripheral | Interface | Controller | Driver/Component                |
| --- | ------- | ---------- | --------- | ---------- | ------------------------------- |
| 1.  | :construction: Todo | USB-OTG    | USB   | on-chip | OTG USB |

### SDIO Devcies

| No. | Support | Peripheral | Interface | Controller | Driver/Component                |
| --- | ------- | ---------- | --------- | ---------- | ------------------------------- |
| 1.  | :construction: Todo | AP6181     | SDIO  | on-chip | AP6181 WIFI |

### GPIO Devcies

| No. | Support | Peripheral | Interface | Controller | Driver/Component                |
| --- | ------- | ---------- | --------- | ---------- | ------------------------------- |
| 1.  | :construction: Todo | IR-RX      | GPIO  | on-board | Infrared Receiver(38Khz) |
| 2.  | :construction: Todo | IR-TX      | GPIO  | on-board | Infrared Transmitter |
| 3.  | :construction: Todo | LED        | GPIO  | on-board | LED |
| 4.  | :construction: Todo | KEY        | GPIO  | on-board | KEY |

### RTC Devcies

| No. | Support | Peripheral | Interface | Controller | Driver/Component                |
| --- | ------- | ---------- | --------- | ---------- | ------------------------------- |
| 1.  | :construction: Todo | RTC        | RTC   | on-chip | RTC |

### I2S Devcies

| No. | Support | Peripheral | Interface | Controller | Driver/Component                |
| --- | ------- | ---------- | --------- | ---------- | ------------------------------- |
| 1.  | :construction: Todo | ES8388     | I2S   | on-chip | ES8388 Audio Decoder |

