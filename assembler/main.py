import struct
import io

f = open("rom.txt", "r")
w = io.open("rom", "w+", encoding="utf-8")
dictionary={
    "mov":0,
    "draw":1,


}
asmdata = ""
for line in f:
    words = line.split(sep=" ")
    words[0] = dictionary[words[0]]
    words[4] = words[4][:-1]
    for i in range(0, len(words)):
        words[i] = int(words[i])
    print(words)

    asmdata += chr((words[0] << 2) + words[1]) + chr((words[2] << 6)  + (words[3] >> 1)) + chr(((words[3] & 1) << 7) + words[4])
    # w.write(chr((words[0] << 2) + words[1]))
    # w.write(chr((words[2] << 6)  + (words[3] >> 1)))
    # w.write(chr(((words[3] & 1) << 7) + words[4]))
print(ord(asmdata))
w.write(asmdata)
w.close()        
