import smbus2
import bme280
import RPi_I2C_driver
from time import sleep

mylcd = RPi_I2C_driver.lcd()

porta_i2c = 1
endereco = 0x76
bus = smbus2.SMBus(porta_i2c)

for i in range(10):
    sleep(1)
    calibracao_paramentros = bme280.load_calibration_params(bus, endereco)
    dado = bme280.sample(bus, endereco, calibracao_paramentros)
    mylcd.lcd_display_string("GRSS" + str(i) + " T:" +'{0:.2f}'.format(dado.temperature), 1)
    mylcd.lcd_display_string("U:" +'{0:.2f}'.format(dado.humidity) + " P:" + '{0:.2f}'.format(dado.pressure), 2)

