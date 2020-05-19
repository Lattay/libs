
filename = 'big_hash.txt'

pairs = {}
collision = set()
data_n = 0
hash_n = 0

with open(filename) as f:
    for line in f:
        data, hash_, *_ = line.split()
        if hash_ in pairs:
            if data not in pairs[hash_]:
                pairs[hash_].add(data)
                collision.add(hash_)
                data_n += 1
        else:
            pairs[hash_] = {data}
            data_n += 1
            hash_n += 1

collision_n = 0
for hash_ in collision:
    print(f"Collision with {hash_}")
    for data in pairs[hash_]:
        print("    ", data)
    collision_n += len(pairs[hash_])

ln = len(collision)
print(f"Total of {collision_n} colliding data on {data_n} unique messages ({100 * collision_n / data_n:4.3}%), concerning {ln} hashes on {hash_n} unique hashes ({100 * ln / hash_n:4.3}%)")
