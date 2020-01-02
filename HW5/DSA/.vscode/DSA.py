import random
import hashlib


global LENGTHOFP
LENGTHOFP = 1024

global LENGTHOFQ
LENGTHOFQ = 160

global MILLERRABINROUNDS
MILLERRABINROUNDS = 5


def extendedGCD(a, b):
    x, old_x = 0, 1
    y, old_y = 1, 0

    while (b != 0):
        quotient = a // b
        a, b = b, a - quotient * b
        old_x, x = x, old_x - quotient * x
        old_y, y = y, old_y - quotient * y

    return old_x


def millerRabinTest(n):
    if n == 2:
        return True
    if n % 2 == 0:
        return False

    r, d = 0, n - 1
    while d % 2 == 0:
        d //= 2
        r += 1
    for _ in range(MILLERRABINROUNDS):
        x = random.randrange(2, n - 1)
        x = pow(x, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True


def gen_num(num):
    p = random.getrandbits(num)
    # make sure length of p is bigger than num
    while (len(bin(p)[2:]) < num):
        p = random.getrandbits(num)
    return p


def gen_prime(num):
    isPrime = False
    while (not isPrime):
        p = random.getrandbits(num)

        # make sure length of prime is bigger than num
        while (len(bin(p)[2:]) < num):
            p = random.getrandbits(num)
        isPrime = millerRabinTest(p)
    return p


def check_key(p, q):
    if(len(bin(p)[2:]) == 1024 and len(bin(q)[2:]) == 160):
        if ((p - 1) % q == 0):
            return True
    return False


# Generate prime p,q
# Follow the document by FIPS PUB 186-3

def gen_P_and_Q():
    isPrime = False

    n = LENGTHOFP // 160
    b = LENGTHOFP % 160
    g = 0
    while (True):
        # gen seed
        # g=seedlen
        while(not isPrime):
            seed = gen_num(LENGTHOFQ)
            g = len(bin(seed)[2:])
            s = hashlib.sha1()
            s.update(str(seed).encode('utf-8'))
            U = (int(s.hexdigest(), 16) % (2**(LENGTHOFQ-1)))
            q = ((2**(LENGTHOFQ-1))+U+1-(U % 2))
            isPrime = millerRabinTest(q)

        offset = 1

        for _ in range(4*LENGTHOFP):
            W = 0
            V = []
            for j in range(n + 1):
                I = (seed + offset + j) % (2 ** g)
                s = hashlib.sha1()
                s.update(str(I).encode('utf-8'))
                temp = int(s.hexdigest(), 16)
                V.append(temp)
            for j in range(n):
                W += (V[j]*(2**(j*LENGTHOFQ)))
            W += ((V[n] % (2 ** b)) * (2 ** (n * LENGTHOFQ)))
            X = W + 2 ** (LENGTHOFP - 1)
            c = X % (2 * q)
            p = X - c + 1
            if (not (p < (2 ** (LENGTHOFP - 1)))):
                isPrime = millerRabinTest(p)
                if (isPrime):
                    return p, q

            offset = offset + n + 1


def gen_G(p, q):
    e = (p - 1) // q
    while (True):
        h = random.randrange(2, p - 1)
        #g = pow(h, e, p)
        g = pow(h, e, p)
        if g > 1:
            return g


def gen_key():
    p, q = gen_P_and_Q()
    print('Length of P:', len(bin(p)[2:]))
    print('\nPrime P: ', p)
    print('\nLength of Q:', len(bin(q)[2:]))
    print('\nPrime Q: ', q)

    while (not check_key(p, q)):
        p, q = gen_P_and_Q()

    # g=generator
    g = gen_G(p, q)
    d = random.randrange(2, q)
    B = pow(g, d, p)
    publicKey = p, q, g, B
    print('\ng: ', g)
    print('\nd: ', d)
    print('\nB: ', B)

    return publicKey, d


def sign(p, q, g, d, x):
    K = random.randrange(1, q)
    r = (pow(g, K, p)) % q

    inverK = extendedGCD(Ｋ, q)
    if (inverK < 0):
        inverK = inverK + q

    h = hashlib.sha1()
    h.update(x.encode('utf-8'))
    print('\nHash of x: ', h.hexdigest())
    
    temp = int(h.hexdigest(), 16)

    s = ((temp + d * r) * inverK) % q
    print('\nr: ', r)
    print('\ns: ', s)
    return x, r, s


def check_sign(publicKey, signature):
    p, q, g, B = publicKey
    x, r, s = signature

    w = extendedGCD(s, q)
    if (w < 0):
        w = w + q

    h = hashlib.sha1()
    h.update(x.encode('utf-8'))
    temp = int(h.hexdigest(), 16)

    u1 = w * temp % q
    u2 = w * r % q
    v = ((pow(g, u1, p) * pow(B, u2, p)) % p) % q
    if (r == v):
        return True
    else:
        return False


if __name__ == "__main__":

    publicKey, privateKey = gen_key()
    x = input('\nInput Plaintest: ')

    signature = sign(publicKey[0], publicKey[1],
                     publicKey[2], privateKey, str(x))

    ans = check_sign(publicKey, signature)
    print('\nSign result: ', ans)