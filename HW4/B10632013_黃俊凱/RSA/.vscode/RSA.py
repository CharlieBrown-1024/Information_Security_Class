from math import gcd
import sys

def ext_gcd(a, b):
    if a == 0:
        return (b, 0, 1)
    else:
        g, y, x = ext_gcd(b % a, a)
        return (g, x - (b // a) * y, y)

def mod_inverse(a, m):
    g, x, y = ext_gcd(a, m)
    if g != 1:
        raise Exception('modular inverse does not exist')
    else:
        return x % m

# 選擇兩個質數以及欲加密的明文
p = int(sys.argv[1])
q = int(sys.argv[2])
message = int(sys.argv[3])
#print(p,q,message)
n = p * q

# 檢查明文長度
if message > n:
	raise Exception('message is bigger than n')

# 計算n的歐拉函數 φ(n) = (p-1)(q-1)
phi = (p - 1) * (q - 1)

# 1 < e < φ(n) and gcd( e, φ(n) ) = 1
# 選最小的e
e = 0
for i in range(2, phi):
	if gcd(i, phi) == 1:
		e = i
		break

# 計算 e 對於 φ(n) 的模反元素 d
# ex + φ(n)y = 1
d = mod_inverse(e, phi)

# pow( x, y, z ) = x^y % z
# cipher = plain^e mod n
cipher = pow(message, e, n ) 

# plain = cipher^d mod n
plain = pow(cipher, d, n)

print ("--------- Variables ---------")
print ('p = ',p)
print ('q = ',q)
print ('n = ',n)
print ('phi = ',phi)
print ('e = ',e)
print ('d = ',d)

print ("---------- Messages ---------")
print ('message = ',message)
print ('ciphertext = ',cipher)
print ('plaintext = ',plain)