import serial, zlib

crc = lambda data: (zlib.crc32(data) ^ 0xffffffff).to_bytes(4, 'little')
ser = serial.Serial("/dev/ttyUSB0", 9600, timeout=.1)
def wr(data):
    ser.write(b'\x80' + len(data).to_bytes(2, 'little') + data + crc(data))
    return ser.read(1) 
def ex(msg, val=None): raise Exception(msg, val)
def rd():
    if (header := ser.read(1)[0]) != 8: ex("header:", header)
    data = ser.read(int.from_bytes(ser.read(2),'little'))
    if crc(data) != ser.read(4): ex("crc")
    return data
def wc(data): ack[0] and ex("ack:", ack.hex()) if (ack := wr(data)) else ex("no ack")
def rw(data): wc(data); return rd()
def rc(data, msg): print(msg) if (ack := rw(data)) == b'\x3b\0' else ex("ack:", ack.hex())

while not wr(b'\x12'): pass
wc(b'\x52\x08'); ser.baudrate = 2000000
size = int.from_bytes(rw(b'\x19')[11:13], 'little') - 8
rc(b'\x21' + b'\xff' * 32, "unlock success")
rc(b'\x15', "erase success")
with open("obj/main.bin", 'rb') as f: data = f.read()
l = len(data := data + b'\xff' * (-len(data) & 7))
for i in range(0, l, size): wc(b'\x24' + i.to_bytes(4,'little') + data[i:i + size])
data += b'\xff' * (1024 - l)
veri = b'\x26' + b'\0' * 4 + len(data).to_bytes(4, 'little')
if crc(data) != (rw(veri), rw(veri))[1][1:]: ex("verify faild")
wc(b'\x40'); print("reset success")

