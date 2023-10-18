# Encryptor
C++ data encryption utility tool

## Setup
Run `make` to compile `./bin/encryptor`

## Usage
```
Usage: encryptor [OPTIONS]

Options:
  Action:
    --encrypt
    --decrypt
  Input:
    --input_data_type <data_type>
    --input (<data> | <filename>)
  Output:
    --output_data_type <data_type>
    [--output <filename>]
  Algorithm:
    --algorithm <algorithm>
  Key:
    --key_data_type <data_type: (base16 | base64)>
    --key <data>
  IV:
    --iv_data_type <data_type: (base16 | base64)>
    --iv <data>
  Block cipher mode:
    --block_cipher_mode <block_cipher_mode>

<data_type>:
  base16
  base64
  file
  raw
<algorithm>:
  aes128
  aes192
  aes256
<block_cipher_mode>:
  ecb
  cbc

Help:
  --help
```

## Tests
Run `./test.sh` to launch tests.
Run `./build_and_test.sh` to compile `./bin/encryptor` and launch tests.

### Example test output
```
[PASS] data, input base16, output base16, AES128, ECB, key base16
[PASS] data, input base16, output base16, AES128, ECB, key base64
[PASS] data, input base16, output base64, AES128, ECB, key base16
[PASS] data, input base16, output base64, AES128, ECB, key base64
[PASS] data, input base64, output base16, AES128, ECB, key base16
[PASS] data, input base64, output base16, AES128, ECB, key base64
[PASS] data, input base64, output base64, AES128, ECB, key base16
[PASS] data, input base64, output base64, AES128, ECB, key base64
[PASS] data, input base16, output base16, AES192, ECB, key base16
[PASS] data, input base16, output base16, AES192, ECB, key base64
[PASS] data, input base16, output base64, AES192, ECB, key base16
[PASS] data, input base16, output base64, AES192, ECB, key base64
[PASS] data, input base64, output base16, AES192, ECB, key base16
[PASS] data, input base64, output base16, AES192, ECB, key base64
[PASS] data, input base64, output base64, AES192, ECB, key base16
[PASS] data, input base64, output base64, AES192, ECB, key base64
[PASS] data, input base16, output base16, AES256, ECB, key base16
[PASS] data, input base16, output base16, AES256, ECB, key base64
[PASS] data, input base16, output base64, AES256, ECB, key base16
[PASS] data, input base16, output base64, AES256, ECB, key base64
[PASS] data, input base64, output base16, AES256, ECB, key base16
[PASS] data, input base64, output base16, AES256, ECB, key base64
[PASS] data, input base64, output base64, AES256, ECB, key base16
[PASS] data, input base64, output base64, AES256, ECB, key base64
[PASS] data, input base16, output base16, AES128, CBC, key base16, IV base16
[PASS] data, input base16, output base16, AES128, CBC, key base16, IV base64
[PASS] data, input base16, output base16, AES128, CBC, key base64, IV base16
[PASS] data, input base16, output base16, AES128, CBC, key base64, IV base64
[PASS] data, input base16, output base64, AES128, CBC, key base16, IV base16
[PASS] data, input base16, output base64, AES128, CBC, key base16, IV base64
[PASS] data, input base16, output base64, AES128, CBC, key base64, IV base16
[PASS] data, input base16, output base64, AES128, CBC, key base64, IV base64
[PASS] data, input base64, output base16, AES128, CBC, key base16, IV base16
[PASS] data, input base64, output base16, AES128, CBC, key base16, IV base64
[PASS] data, input base64, output base16, AES128, CBC, key base64, IV base16
[PASS] data, input base64, output base16, AES128, CBC, key base64, IV base64
[PASS] data, input base64, output base64, AES128, CBC, key base16, IV base16
[PASS] data, input base64, output base64, AES128, CBC, key base16, IV base64
[PASS] data, input base64, output base64, AES128, CBC, key base64, IV base16
[PASS] data, input base64, output base64, AES128, CBC, key base64, IV base64
[PASS] data, input base16, output base16, AES192, CBC, key base16, IV base16
[PASS] data, input base16, output base16, AES192, CBC, key base16, IV base64
[PASS] data, input base16, output base16, AES192, CBC, key base64, IV base16
[PASS] data, input base16, output base16, AES192, CBC, key base64, IV base64
[PASS] data, input base16, output base64, AES192, CBC, key base16, IV base16
[PASS] data, input base16, output base64, AES192, CBC, key base16, IV base64
[PASS] data, input base16, output base64, AES192, CBC, key base64, IV base16
[PASS] data, input base16, output base64, AES192, CBC, key base64, IV base64
[PASS] data, input base64, output base16, AES192, CBC, key base16, IV base16
[PASS] data, input base64, output base16, AES192, CBC, key base16, IV base64
[PASS] data, input base64, output base16, AES192, CBC, key base64, IV base16
[PASS] data, input base64, output base16, AES192, CBC, key base64, IV base64
[PASS] data, input base64, output base64, AES192, CBC, key base16, IV base16
[PASS] data, input base64, output base64, AES192, CBC, key base16, IV base64
[PASS] data, input base64, output base64, AES192, CBC, key base64, IV base16
[PASS] data, input base64, output base64, AES192, CBC, key base64, IV base64
[PASS] data, input base16, output base16, AES256, CBC, key base16, IV base16
[PASS] data, input base16, output base16, AES256, CBC, key base16, IV base64
[PASS] data, input base16, output base16, AES256, CBC, key base64, IV base16
[PASS] data, input base16, output base16, AES256, CBC, key base64, IV base64
[PASS] data, input base16, output base64, AES256, CBC, key base16, IV base16
[PASS] data, input base16, output base64, AES256, CBC, key base16, IV base64
[PASS] data, input base16, output base64, AES256, CBC, key base64, IV base16
[PASS] data, input base16, output base64, AES256, CBC, key base64, IV base64
[PASS] data, input base64, output base16, AES256, CBC, key base16, IV base16
[PASS] data, input base64, output base16, AES256, CBC, key base16, IV base64
[PASS] data, input base64, output base16, AES256, CBC, key base64, IV base16
[PASS] data, input base64, output base16, AES256, CBC, key base64, IV base64
[PASS] data, input base64, output base64, AES256, CBC, key base16, IV base16
[PASS] data, input base64, output base64, AES256, CBC, key base16, IV base64
[PASS] data, input base64, output base64, AES256, CBC, key base64, IV base16
[PASS] data, input base64, output base64, AES256, CBC, key base64, IV base64
72/72 tests passed
```
