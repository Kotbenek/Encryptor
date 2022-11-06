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
    # $1 - input type
    # $2 - input data
    # $3 - output type
    # $4 - AES size
    # $5 - AES mode
    # $6 - key type
    # $7 - key data
    # $8 - IV type
    # $9 - IV data

    local ENCRYPTED_DATA
    local DECRYPTED_DATA

    ENCRYPTED_DATA=$(./bin/encryptor "-i" "$1" "$2" "-o" "$3" "-e" "-a" "aes" "-s" "$4" "-m" "$5" "-k" "$6" "$7" "-v" "$8" "$9")

    if assert_not_sigsegv; then
        DECRYPTED_DATA=$(./bin/encryptor "-i" "$3" "${ENCRYPTED_DATA}" "-o" "$1" "-d" "-a" "aes" "-s" "$4" "-m" "$5" "-k" "$6" "$7" "-v" "$8" "$9")

        if assert_not_sigsegv; then
            assert_equal "$2" "${DECRYPTED_DATA}"
        else
            echo "./bin/encryptor -i $3 ${ENCRYPTED_DATA} -o $1 -d -a aes -s $4 -m $5 -k $6 $7 -v $8 $9"
        fi
    else
        echo "./bin/encryptor -i $1 $2 -o $3 -e -a aes -s $4 -m $5 -k $6 $7 -v $8 $9"
    fi
}

DATA_INPUT_TYPES=("b16" "b64")
AES_SIZES=("128" "192" "256")

for aes_size in "${AES_SIZES[@]}"; do
    for input_type in "${DATA_INPUT_TYPES[@]}"; do
        for output_type in "${DATA_INPUT_TYPES[@]}"; do
            for key_type in "${DATA_INPUT_TYPES[@]}"; do
                for iv_type in "${DATA_INPUT_TYPES[@]}"; do
                    print_test_init "data, input ${input_type}, output ${output_type}, AES${aes_size}, CBC, key ${key_type}, IV ${iv_type}"

                    if [ "${input_type}" == "b16" ]; then
                        INPUT="6BC1BEE22E409F96E93D7E117393172A"
                    else
                        INPUT="a8G+4i5An5bpPX4Rc5MXKg=="
                    fi

                    if [ "${key_type}" == "b16" ]; then
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

                    if [ "${iv_type}" == "b16" ]; then
                        IV="000102030405060708090A0B0C0D0E0F"
                    else
                        IV="AAECAwQFBgcICQoLDA0ODw=="
                    fi

                    test_encrypt_decrypt_aes_data "${input_type}" "${INPUT}" "${output_type}" "${aes_size}" "cbc" "${key_type}" "${KEY}" "${iv_type}" "${IV}"
                done
            done
        done
    done
done

# Display test results
echo "${TESTS_PASSED}/${TESTS} tests passed"
