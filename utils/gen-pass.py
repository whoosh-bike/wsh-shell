#!/usr/bin/env python3
import argparse
import hashlib

# source .venv/bin/activate
# python3 gen-pass.py --salt 1q2w3e4r5t6y7u8i --password 1234


def make_sha256(data: bytes) -> str:
    digest = hashlib.sha256(data).hexdigest()
    return digest


def make_jenkins(data: bytes) -> str:
    hash_val = 0
    for b in data:
        hash_val = (hash_val + b) & 0xFFFFFFFF
        hash_val = (hash_val + ((hash_val << 10) & 0xFFFFFFFF)) & 0xFFFFFFFF
        hash_val ^= hash_val >> 6

    hash_val = (hash_val + ((hash_val << 3) & 0xFFFFFFFF)) & 0xFFFFFFFF
    hash_val ^= hash_val >> 11
    hash_val = (hash_val + ((hash_val << 15) & 0xFFFFFFFF)) & 0xFFFFFFFF

    return f"{hash_val & 0xFFFFFFFF:08x}"


def main():
    parser = argparse.ArgumentParser(description="Generate hash(salt+password) ")
    parser.add_argument("--salt", required=True, help="Salt string")
    parser.add_argument("--password", required=True, help="Password string")
    args = parser.parse_args()

    data = (args.salt + args.password).encode("utf-8")
    h1 = make_sha256(data)
    h2 = make_jenkins(data)

    print(f"data: {data}")
    print(f'sha256 hash(salt|pass): "{h1}"')
    print(f'jenkins hash(salt|pass): "{h2}"')


if __name__ == "__main__":
    main()
