# stm32_iicPeriph
stm32 iic  peripheral devices

- `RTC` 基于[RX8025T](https://www.lcsc.com/datasheet/lcsc_datasheet_2208031430_Seiko-Epson-RX8025T-UB_C17353.pdf) 实现时钟  

- `tca9545a`基于[tca9545a](https://www.ti.com/lit/ds/symlink/tca9545a.pdf?ts=1713938623923) 实现多路 `I²C` 通信

- `abpmanv`系列气压传感器基于 [ABPMANV_015/150](https://prod-edam.honeywell.com/content/dam/honeywell-edam/sps/siot/fr-ca/products/sensors/pressure-sensors/board-mount-pressure-sensors/common/documents/sps-siot-i2c-comms-digital-output-pressure-sensors-tn-008201-3-en-ciid-45841.pdf)手册实现

- `Amphenol DLH` 低压数字压力传感器基于 [dlhl05d](https://www.mouser.cn/datasheet/2/18/1/DS-0355_Rev_B-1499264.pdf)手册实现