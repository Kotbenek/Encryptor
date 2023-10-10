#pragma once

#include "Array.h"
#include "EncryptionAlgorithm.h"

#include <cstdint>

class AES : public EncryptionAlgorithm
{
public:
    enum class key_length
    {
        AES128,
        AES192,
        AES256
    };

    AES(key_length len);
    ~AES();

    uint8_t set_key(Array* K);
    uint8_t set_IV(Array* iv);
    uint8_t set_block_cipher_mode(int block_cipher_mode);
    uint8_t encrypt(Array* data);
    uint8_t decrypt(Array* data);
    uint8_t get_required_input_alignment();

private:
    //Round key
    Array* Round_Key;
    //Number of 32-bit words comprising the Cipher key
    uint8_t Nk;
    //Number of rounds
    uint8_t Nr;
    //Initialization vector
    Array* IV;

    //Temporary state for MixColumns and InvMixColumns
    Array* temp_state;

    //Number of columns (32-bit words) comprising the State
    static const uint8_t Nb = 4;
    //Number of rows comprising the State
    static const uint8_t NUMBER_OF_ROWS = 4;
    //State size
    static const uint8_t STATE_SIZE = NUMBER_OF_ROWS * Nb;
    //Round constant
    static const uint8_t Rcon[];
    //Substitution table
    static const uint8_t Sbox[];
    //Inverse substitution table
    static const uint8_t Inv_Sbox[];

    bool key_set;
    bool iv_set;

    void AddRoundKey(Array* state, uint8_t round_number);
    void Cipher(Array* data);
    void InvCipher(Array* data);
    void InvMixColumns(Array* state);
    void InvShiftRows(Array* state);
    void InvSubBytes(Array* state);
    void KeyExpansion(Array* K);
    void MixColumns(Array* state);
    void RotWord(Array* word);
    void ShiftRows(Array* state);
    void SubBytes(Array* state);
    void SubWord(Array* word);
    uint8_t multiply_gf(uint8_t a, uint8_t b);
    uint8_t xtime(uint8_t b);

    void __encrypt(Array* data, Array* state, uint64_t data_index);
    void __decrypt(Array* data, Array* state, uint64_t data_index);

    uint8_t (AES::*__encrypt_block_cipher_mode)(Array* data) = NULL;
    uint8_t (AES::*__decrypt_block_cipher_mode)(Array* data) = NULL;

    uint8_t __encrypt_CBC(Array* data);
    uint8_t __decrypt_CBC(Array* data);
};
