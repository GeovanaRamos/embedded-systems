import smbus2
import bme280

porta_i2c = 1
endereco = 0x76
bus = smbus2.SMBus(porta_i2c)

calibracao_paramentros = bme280.load_calibration_params(bus, endereco)
dado = bme280.sample(bus, endereco, calibracao_paramentros)
print("T " + str(dado.temperature))
print("H " + str(dado.humidity))
print("P " + str(dado.pressure))
