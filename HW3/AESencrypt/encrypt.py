from PIL import Image
from Crypto.Cipher import AES
import os
import io
import sys

# store image
ppmPicture = 'linux.ppm'
im = Image.open('linux.svg.png')
im.save(ppmPicture)

# read file
with open(ppmPicture,'rb') as file:
    header = file.readline() + file.readline() + file.readline() 
    data = file.read()

BLOCK_SIZE = 16

# padding
padding = BLOCK_SIZE - (len(data) % BLOCK_SIZE)
if padding == 16:
    padding = 0
temp = bytearray()
for i in range(padding):
    temp.append(padding)


# how many blocks
if len(data) % BLOCK_SIZE == 0:
    block = int(len(data) / BLOCK_SIZE)
else:
    block = int(len(data) / BLOCK_SIZE) + 1

##############################################################
# padding
plaintext = data + temp

# key
key = os.urandom(16)

# ECB mode
cipher = AES.new(key,AES.MODE_ECB)
output = bytearray()
for i in range(block):
    text = plaintext[16*i:16+16*i]
    ciphertext = cipher.encrypt(text)
    for j in range(BLOCK_SIZE):
        output.append(ciphertext[j])
    #print(ciphertext)
#print(output)

# unpadding and adding header
output = output[0:len(output) - padding + 1]
output = header + output
#print(output)

# store image
newPicture = 'output1.jpg'
im = Image.open(io.BytesIO(output))
im.save(newPicture)
print("output1 complete")
####################################################

# xor IV and data
IV = os.urandom(len(data))
result = bytearray()
for i in range(len(data)):
    result.append(data[i] ^ IV[i])

# padding
plaintext = result + temp

# CBC mode
key = os.urandom(16)
cipher = AES.new(key,AES.MODE_ECB)
output = bytearray()
text = plaintext[0:16]
for i in range(block):
    #print(text)
    ciphertext = cipher.encrypt(text)
    for j in range(BLOCK_SIZE):
        output.append(ciphertext[j])
    if i == block - 1:
        break
    next_text = plaintext[(16*(i+1)):(16+16*(i+1))]
    t = bytearray()
    for k in range(BLOCK_SIZE):
        t.append(next_text[k] ^ ciphertext[k])
    text = t
    #print(text)
#print(output)

# unpadding and adding header
output = output[0:len(output) - padding + 1]
output = header + output

# store image
newPicture = 'output2.jpg'
im = Image.open(io.BytesIO(output))
im.save(newPicture)
print("output2 complete")
##############################################

plaintext = data + temp
key = os.urandom(BLOCK_SIZE)
IV = os.urandom(len(plaintext))

# CTR mode
cipher = AES.new(key,AES.MODE_ECB)
new_IV = bytearray()
for i in range(block):

    text = IV[16*i:16+16*i]
    # print(text)
    ciphertext = cipher.encrypt(text)
    for j in range(BLOCK_SIZE):
        new_IV.append(ciphertext[j])

output = bytearray()
for k in range(len(plaintext)):
    output.append(plaintext[k]^new_IV[k])
output = output[0:len(output) - padding + 1]
output = header + output

newPicture = 'output3.jpg'
im = Image.open(io.BytesIO(output))
im.save(newPicture)
print("output3 complete")





    
    

