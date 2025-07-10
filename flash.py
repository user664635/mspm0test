import serial
import zlib

def crc(data): return (zlib.crc32(data)^0xffffffff).to_bytes(4, 'little')
def wr(data):
    ser.write(b'\x80' + len(data).to_bytes(2, 'little') + data + crc(data))
    return ser.read(1) 
def rd():
    header = ser.read(1)[0]
    if header != 8: raise Exception("header:", header)
    data = ser.read(int.from_bytes(ser.read(2),'little'))
    if crc(data) != ser.read(4): raise Exception("crc")
    return data
def ck(ack):
    if not ack: raise Exception("no ack")
    ack = ack[0]
    if ack: raise Exception("ack:", hex(ack))
def wc(data): ck(wr(data))
def rw(data): wc(data); return rd()

ser = serial.Serial("/dev/ttyUSB0", 9600, timeout=.1)
while not wr(b'\x12'): pass
wc(b'\x52\x08')
ser.baudrate = 2000000
ser.timeout = 1

id = rw(b'\x19')
#print("Command Interpreter version:", id[2:0:-1].hex())
#print("Build ID:", id[4:2:-1].hex())
#print("Application version:", id[8:4:-1].hex())
#print("Active Plug-in interface version:", id[10:8:-1].hex())
#print("BSL Max buffer size:", id[12:10:-1].hex())
#print("BSL Buffer Start address:", id[16:12:-1].hex())
#print("BCR Config ID:", id[20:16:-1].hex())
#print("BSL Config ID:", id[24:20:-1].hex())

print(rw(b'\x21' + b'\xff' * 32).hex())
print(rw(b'\x15').hex())
with open("obj/main.bin", 'rb') as f:
    data = f.read()
l = len(data)
data += b'\xff' * (-l & 7)
addr = b'\0' * 4
wc(b'\x24' + addr + data)

l = len(data)
data += b'\xff' * (1024 - l)
veri = b'\x26' + addr + len(data).to_bytes(4, 'little')
print(crc(data).hex())
print(rw(veri).hex())
print(rw(veri).hex())
wc(b'\x40')

