import mraa
import sys
import serial

port = "/dev/ttyUSB0" #porta a ser usada para transmissão
baud = 9600 #taxa de transmissão
data = 2 #dado a ser enviado

#Usa pinos D0(RX) e D1(TX)
#Define configurações da comunicação
usart = serial.Serial(port,baud)

while True:
	usart.write(data) #Envia dado
	usart.flushInput() #limpa o canal de transmissão

