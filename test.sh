#!/bin/bash

TESTS_DIR="tests"

PASS="[$(tput setaf 2)PASS$(tput sgr0)]"
FAIL="[$(tput setaf 1)FAIL$(tput sgr0)]"
SEGV="[$(tput setaf 1)SEGV$(tput sgr0)]"
RUNNING="[    ]"

TESTS=0
TESTS_PASSED=0

# Printing functions
print_test_init()
{
    echo -n -e "$RUNNING $1"
}

print_test_pass()
{
    echo -n -e "\r"
    echo "$PASS"
}

print_test_fail()
{
    echo -n -e "\r"
    echo "$FAIL"
}

print_test_sigsegv()
{
    echo -n -e "\r"
    echo "$SEGV"
}

# Testing functions
assert_equal()
{
    ((TESTS=TESTS+1))
    if [ "$1" == "$2" ]; then
        ((TESTS_PASSED=TESTS_PASSED+1))
        print_test_pass
    else
        print_test_fail
        echo "       Arguments are not equal."
        echo "       Argument 1: $1"
        echo "       Argument 2: $2"
    fi
}

assert_not_sigsegv()
{
    if [ $? == "139" ]; then
        ((TESTS=TESTS+1))
        print_test_sigsegv
        return 1
    else
        return 0
    fi
}

test_encrypt_decrypt_aes_data()
{
    # $1  - input type
    # $2  - input data
    # $3  - output type
    # $4  - AES size
    # $5  - AES mode
    # $6  - key type
    # $7  - key data
    # $8  - IV type
    # $9  - IV data
    # $10 - padding

    local ENCRYPTED_DATA
    local DECRYPTED_DATA

    if [ "$8" == "" ]; then
        ENCRYPTED_DATA=$(./bin/encryptor "--input_data_type" "$1" "--input" "$2" "--output_data_type" "$3" "--encrypt" "--algorithm" "aes$4" "--block_cipher_mode" "$5" "--key_data_type" "$6" "--key" "$7" "--padding" "${10}")
    else
        ENCRYPTED_DATA=$(./bin/encryptor "--input_data_type" "$1" "--input" "$2" "--output_data_type" "$3" "--encrypt" "--algorithm" "aes$4" "--block_cipher_mode" "$5" "--key_data_type" "$6" "--key" "$7" "--iv_data_type" "$8" "--iv" "$9" "--padding" "${10}")
    fi

    if assert_not_sigsegv; then
        if [ "$8" == "" ]; then
            DECRYPTED_DATA=$(./bin/encryptor "--input_data_type" "$3" "--input" "${ENCRYPTED_DATA}" "--output_data_type" "$1" "--decrypt" "--algorithm" "aes$4" "--block_cipher_mode" "$5" "--key_data_type" "$6" "--key" "$7" "--padding" "${10}")
        else
            DECRYPTED_DATA=$(./bin/encryptor "--input_data_type" "$3" "--input" "${ENCRYPTED_DATA}" "--output_data_type" "$1" "--decrypt" "--algorithm" "aes$4" "--block_cipher_mode" "$5" "--key_data_type" "$6" "--key" "$7" "--iv_data_type" "$8" "--iv" "$9" "--padding" "${10}")
        fi

        if assert_not_sigsegv; then
            assert_equal "$2" "${DECRYPTED_DATA}"
        else
            if [ "$8" == "" ]; then
                echo "./bin/encryptor --input_data_type $3 --input ${ENCRYPTED_DATA} --output_data_type $1 --decrypt --algorithm aes$4 --block_cipher_mode $5 --key_data_type $6 --key $7" "--padding" "${10}"
            else
                echo "./bin/encryptor --input_data_type $3 --input ${ENCRYPTED_DATA} --output_data_type $1 --decrypt --algorithm aes$4 --block_cipher_mode $5 --key_data_type $6 --key $7 --iv_data_type $8 --iv $9" "--padding" "${10}"
            fi
        fi
    else
        if [ "$8" == "" ]; then
            echo "./bin/encryptor --input_data_type $1 --input $2 --output_data_type $3 --encrypt --algorithm aes$4 --block_cipher_mode $5 --key_data_type $6 --key $7" "--padding" "${10}"
        else
            echo "./bin/encryptor --input_data_type $1 --input $2 --output_data_type $3 --encrypt --algorithm aes$4 --block_cipher_mode $5 --key_data_type $6 --key $7 --iv_data_type $8 --iv $9" "--padding" "${10}"
        fi
    fi
}

DATA_INPUT_TYPES=("base16" "base64")
AES_SIZES=("128" "192" "256")
BLOCK_CIPHER_MODES=("ecb" "cbc" "pcbc")
PADDING_ALGORITHMS=("pkcs7" "x923" "iso7816")

for block_cipher_mode in "${BLOCK_CIPHER_MODES[@]}"; do
    for aes_size in "${AES_SIZES[@]}"; do
        for input_type in "${DATA_INPUT_TYPES[@]}"; do
            for output_type in "${DATA_INPUT_TYPES[@]}"; do
                for key_type in "${DATA_INPUT_TYPES[@]}"; do
                    for padding in "${PADDING_ALGORITHMS[@]}"; do
                        if [ "${block_cipher_mode}" == "ecb" ]; then
                            print_test_init "data, input ${input_type}, output ${output_type}, AES${aes_size}, ${block_cipher_mode^^}, key ${key_type}, padding ${padding}"

                                if [ "${input_type}" == "base16" ]; then
                                    INPUT="6BC1BEE22E409F96E93D7E117393172A"
                                else
                                    INPUT="a8G+4i5An5bpPX4Rc5MXKg=="
                                fi

                                if [ "${key_type}" == "base16" ]; then
                                    if [ "${aes_size}" == "128" ]; then
                                        KEY="2B7E151628AED2A6ABF7158809CF4F3C"
                                    elif [ "${aes_size}" == "192" ]; then
                                        KEY="8E73B0F7DA0E6452C810F32B809079E562F8EAD2522C6B7B"
                                    else
                                        KEY="603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4"
                                    fi
                                else
                                    if [ "${aes_size}" == "128" ]; then
                                        KEY="K34VFiiu0qar9xWICc9PPA=="
                                    elif [ "${aes_size}" == "192" ]; then
                                        KEY="jnOw99oOZFLIEPMrgJB55WL46tJSLGt7"
                                    else
                                        KEY="YD3rEBXKcb4rc67whX13gR81LAc7YQjXLZgQowkU3/Q="
                                    fi
                                fi

                                test_encrypt_decrypt_aes_data "${input_type}" "${INPUT}" "${output_type}" "${aes_size}" ${block_cipher_mode} "${key_type}" "${KEY}" "" "" "${padding}"
                        else
                            for iv_type in "${DATA_INPUT_TYPES[@]}"; do
                                print_test_init "data, input ${input_type}, output ${output_type}, AES${aes_size}, ${block_cipher_mode^^}, key ${key_type}, IV ${iv_type}, padding ${padding}"

                                if [ "${input_type}" == "base16" ]; then
                                    INPUT="6BC1BEE22E409F96E93D7E117393172A"
                                else
                                    INPUT="a8G+4i5An5bpPX4Rc5MXKg=="
                                fi

                                if [ "${key_type}" == "base16" ]; then
                                    if [ "${aes_size}" == "128" ]; then
                                        KEY="2B7E151628AED2A6ABF7158809CF4F3C"
                                    elif [ "${aes_size}" == "192" ]; then
                                        KEY="8E73B0F7DA0E6452C810F32B809079E562F8EAD2522C6B7B"
                                    else
                                        KEY="603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4"
                                    fi
                                else
                                    if [ "${aes_size}" == "128" ]; then
                                        KEY="K34VFiiu0qar9xWICc9PPA=="
                                    elif [ "${aes_size}" == "192" ]; then
                                        KEY="jnOw99oOZFLIEPMrgJB55WL46tJSLGt7"
                                    else
                                        KEY="YD3rEBXKcb4rc67whX13gR81LAc7YQjXLZgQowkU3/Q="
                                    fi
                                fi

                                if [ "${iv_type}" == "base16" ]; then
                                    IV="000102030405060708090A0B0C0D0E0F"
                                else
                                    IV="AAECAwQFBgcICQoLDA0ODw=="
                                fi

                                test_encrypt_decrypt_aes_data "${input_type}" "${INPUT}" "${output_type}" "${aes_size}" ${block_cipher_mode} "${key_type}" "${KEY}" "${iv_type}" "${IV}" "${padding}"
                            done
                        fi
                    done
                done
            done
        done
    done
done

# Display test results
echo "${TESTS_PASSED}/${TESTS} tests passed"
