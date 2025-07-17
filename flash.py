import serial, zlib

def crc(data): return (zlib.crc32(data)^0xffffffff).to_bytes(4, 'little')
ser = serial.Serial("/dev/ttyUSB0", 9600, timeout=.1)
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
def cm(ack, msg):
    if ack != b'\x3b\0': raise Exception("ack:", ack.hex())
    print(msg)

while not wr(b'\x12'): pass
wc(b'\x52\x08')
ser.baudrate = 2000000

id = rw(b'\x19')
size = int.from_bytes(id[11:13], 'little') - 16
cm(rw(b'\x21' + b'\xff' * 32), "unlock success")
cm(rw(b'\x15'), "erase success")
with open("obj/main.bin", 'rb') as f: data = f.read()
l = len(data := data + b'\xff' * (-len(data) & 7))
for i in range(0,l,size): wc(b'\x24' + i.to_bytes(4,'little') + data[i:i + size])
data += b'\xff' * (1024 - l)
veri = b'\x26' + b'\0' * 4 + len(data).to_bytes(4, 'little')
if crc(data) != (rw(veri), rw(veri))[1][1:]: raise Exception("verify faild")
wc(b'\x40'); wr(b'\x40'); print("success")

