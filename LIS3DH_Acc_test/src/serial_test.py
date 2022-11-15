import serial

ser = serial.Serial('COM5')
ser.flushInput()

while True:
    ser_bytes = ser.readline()
    decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
    str_list = decoded_bytes.split(",")
    output = [int(i) for i in str_list]
    print(output)