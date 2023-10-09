#include "AES.h"

#include "Array.h"
#include "block_cipher_mode.h"

#include <algorithm>
#include <cstdint>

AES::AES(key_length len)
{
    if (len == key_length::AES128)
    {
        Nk = 4;
        Nr = 10;
        Round_Key = new Array(176);
    }
    else if (len == key_length::AES192)
    {
        Nk = 6;
        Nr = 12;
        Round_Key = new Array(208);
    }
    else if (len == key_length::AES256)
    {
        Nk = 8;
        Nr = 14;
        Round_Key = new Array(240);
    }

    key_set = false;
    iv_set = false;
}

AES::~AES()
{
    delete Round_Key;
    if (iv_set)
        delete IV;
}

uint8_t AES::set_key(Array* K)
{
    //AES128
    if (Nk == 4 && K->size() != 16)
        return 1;
    //AES192
    else if (Nk == 6 && K->size() != 24)
        return 1;
    //AES256
    else if (Nk == 8 && K->size() != 32)
        return 1;

    KeyExpansion(K);

    key_set = true;

    return 0;
}

uint8_t AES::set_IV(Array* iv)
{
    if (iv->size() != STATE_SIZE)
        return 1;

    IV = new Array(*iv);

    iv_set = true;

    return 0;
}

uint8_t AES::set_block_cipher_mode(int block_cipher_mode)
{
    switch (block_cipher_mode)
    {
        case block_cipher_mode::CBC:
            __encrypt_block_cipher_mode = &AES::__encrypt_CBC;
            __decrypt_block_cipher_mode = &AES::__decrypt_CBC;
            return 0;
        default:
            return 1;
    }
}

uint8_t AES::encrypt(Array* data)
{
    return (this->*__encrypt_block_cipher_mode)(data);
}

uint8_t AES::decrypt(Array* data)
{
    return (this->*__decrypt_block_cipher_mode)(data);
}

uint8_t AES::get_required_input_alignment()
{
    //For now, the only implemented block cipher mode is CBC, which requires padding equal to
    //state size
    return STATE_SIZE;
}

const uint8_t AES::Rcon[] = {0x8D, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

const uint8_t AES::Sbox[] = {
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16};

const uint8_t AES::Inv_Sbox[] = {
    0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
    0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
    0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
    0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
    0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
    0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
    0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
    0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
    0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
    0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
    0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
    0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
    0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
    0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
    0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D};

void AES::AddRoundKey(Array* state, uint8_t round_number)
{
    for (uint8_t i = 0; i < NUMBER_OF_ROWS; i++)
        for (uint8_t j = 0; j < Nb; j++)
            state->data[j + i * Nb] ^=
                Round_Key->data[i + j * NUMBER_OF_ROWS + round_number * STATE_SIZE];
}

void AES::Cipher(Array* data)
{
    AddRoundKey(data, 0);

    for (uint8_t i = 1; i < Nr; i++)
    {
        SubBytes(data);
        ShiftRows(data);
        MixColumns(data);
        AddRoundKey(data, i);
    }

    SubBytes(data);
    ShiftRows(data);
    AddRoundKey(data, Nr);
}

void AES::InvCipher(Array* data)
{
    AddRoundKey(data, Nr);

    for (uint8_t i = Nr - 1; i > 0; i--)
    {
        InvShiftRows(data);
        InvSubBytes(data);
        AddRoundKey(data, i);
        InvMixColumns(data);
    }

    InvShiftRows(data);
    InvSubBytes(data);
    AddRoundKey(data, 0);
}

void AES::InvMixColumns(Array* state)
{
    //Create the temporary state
    Array* temp_state = new Array(STATE_SIZE);

    //Mix each column
    for (uint8_t i = 0; i < Nb; i++)
    {
        temp_state->data[i + 0 * Nb] = multiply_gf(0x0E, state->data[i + 0 * Nb]) ^
                                       multiply_gf(0x0B, state->data[i + 1 * Nb]) ^
                                       multiply_gf(0x0D, state->data[i + 2 * Nb]) ^
                                       multiply_gf(0x09, state->data[i + 3 * Nb]);
        temp_state->data[i + 1 * Nb] = multiply_gf(0x09, state->data[i + 0 * Nb]) ^
                                       multiply_gf(0x0E, state->data[i + 1 * Nb]) ^
                                       multiply_gf(0x0B, state->data[i + 2 * Nb]) ^
                                       multiply_gf(0x0D, state->data[i + 3 * Nb]);
        temp_state->data[i + 2 * Nb] = multiply_gf(0x0D, state->data[i + 0 * Nb]) ^
                                       multiply_gf(0x09, state->data[i + 1 * Nb]) ^
                                       multiply_gf(0x0E, state->data[i + 2 * Nb]) ^
                                       multiply_gf(0x0B, state->data[i + 3 * Nb]);
        temp_state->data[i + 3 * Nb] = multiply_gf(0x0B, state->data[i + 0 * Nb]) ^
                                       multiply_gf(0x0D, state->data[i + 1 * Nb]) ^
                                       multiply_gf(0x09, state->data[i + 2 * Nb]) ^
                                       multiply_gf(0x0E, state->data[i + 3 * Nb]);
    }

    //Copy temporary state to state
    std::copy(&temp_state->data[0], &temp_state->data[STATE_SIZE], state->data);

    delete temp_state;
}

void AES::InvShiftRows(Array* state)
{
    uint8_t temp;

    //Shift row 0 - 0 columns to the right
    //It is already in this state, no need to shift

    //Shift row 1 - 1 column to the right
    temp = state->data[3 + 1 * Nb];
    state->data[3 + 1 * Nb] = state->data[2 + 1 * Nb];
    state->data[2 + 1 * Nb] = state->data[1 + 1 * Nb];
    state->data[1 + 1 * Nb] = state->data[0 + 1 * Nb];
    state->data[0 + 1 * Nb] = temp;

    //Shift row 2 - 2 columns to the right
    temp = state->data[0 + 2 * Nb];
    state->data[0 + 2 * Nb] = state->data[2 + 2 * Nb];
    state->data[2 + 2 * Nb] = temp;

    temp = state->data[1 + 2 * Nb];
    state->data[1 + 2 * Nb] = state->data[3 + 2 * Nb];
    state->data[3 + 2 * Nb] = temp;

    //Shift row 3 - 3 columns to the right
    temp = state->data[0 + 3 * Nb];
    state->data[0 + 3 * Nb] = state->data[1 + 3 * Nb];
    state->data[1 + 3 * Nb] = state->data[2 + 3 * Nb];
    state->data[2 + 3 * Nb] = state->data[3 + 3 * Nb];
    state->data[3 + 3 * Nb] = temp;
}

void AES::InvSubBytes(Array* state)
{
    for (uint8_t i = 0; i < NUMBER_OF_ROWS; i++)
        for (uint8_t j = 0; j < Nb; j++)
            state->data[j + i * Nb] = Inv_Sbox[(uint8_t)state->data[j + i * Nb]];
}

void AES::KeyExpansion(Array* K)
{
    Array* temp = new Array(NUMBER_OF_ROWS);

    for (uint8_t i = 0; i < Nk; i++)
        for (uint8_t j = 0; j < NUMBER_OF_ROWS; j++)
            Round_Key->data[NUMBER_OF_ROWS * i + j] = K->data[NUMBER_OF_ROWS * i + j];

    for (uint8_t i = Nk; i < Nb * (Nr + 1); i++)
    {
        for (uint8_t j = 0; j < NUMBER_OF_ROWS; j++)
            temp->data[j] = Round_Key->data[NUMBER_OF_ROWS * (i - 1) + j];

        if (i % Nk == 0)
        {
            RotWord(temp);
            SubWord(temp);
            temp->data[0] ^= Rcon[i / Nk];
        }
        else if (Nk > 6 && i % Nk == 4)
            SubWord(temp);

        for (uint8_t j = 0; j < NUMBER_OF_ROWS; j++)
            Round_Key->data[NUMBER_OF_ROWS * i + j] =
                Round_Key->data[NUMBER_OF_ROWS * (i - Nk) + j] ^ temp->data[j];
    }

    delete temp;
}

void AES::MixColumns(Array* state)
{
    //Create the temporary state
    Array* t = new Array(STATE_SIZE);

    //Mix each column
    for (uint8_t i = 0; i < Nb; i++)
    {
        t->data[i + 0 * Nb] = multiply_gf(0x02, state->data[i + 0 * Nb]) ^
                              multiply_gf(0x03, state->data[i + 1 * Nb]) ^ state->data[i + 2 * Nb] ^
                              state->data[i + 3 * Nb];
        t->data[i + 1 * Nb] = state->data[i + 0 * Nb] ^ multiply_gf(0x02, state->data[i + 1 * Nb]) ^
                              multiply_gf(0x03, state->data[i + 2 * Nb]) ^ state->data[i + 3 * Nb];
        t->data[i + 2 * Nb] = state->data[i + 0 * Nb] ^ state->data[i + 1 * Nb] ^
                              multiply_gf(0x02, state->data[i + 2 * Nb]) ^
                              multiply_gf(0x03, state->data[i + 3 * Nb]);
        t->data[i + 3 * Nb] = multiply_gf(0x03, state->data[i + 0 * Nb]) ^ state->data[i + 1 * Nb] ^
                              state->data[i + 2 * Nb] ^ multiply_gf(0x02, state->data[i + 3 * Nb]);
    }

    //Copy temporary state to state
    std::copy(&t->data[0], &t->data[STATE_SIZE], state->data);

    delete t;
}

void AES::RotWord(Array* word)
{
    uint8_t temp;

    temp = word->data[0];
    word->data[0] = word->data[1];
    word->data[1] = word->data[2];
    word->data[2] = word->data[3];
    word->data[3] = temp;
}

void AES::ShiftRows(Array* state)
{
    uint8_t temp;

    //Shift row 0 - 0 columns to the left
    //It is already in this state, no need to shift

    //Shift row 1 - 1 column to the left
    temp = state->data[0 + 1 * Nb];
    state->data[0 + 1 * Nb] = state->data[1 + 1 * Nb];
    state->data[1 + 1 * Nb] = state->data[2 + 1 * Nb];
    state->data[2 + 1 * Nb] = state->data[3 + 1 * Nb];
    state->data[3 + 1 * Nb] = temp;

    //Shift row 2 - 2 columns to the left
    temp = state->data[0 + 2 * Nb];
    state->data[0 + 2 * Nb] = state->data[2 + 2 * Nb];
    state->data[2 + 2 * Nb] = temp;

    temp = state->data[1 + 2 * Nb];
    state->data[1 + 2 * Nb] = state->data[3 + 2 * Nb];
    state->data[3 + 2 * Nb] = temp;

    //Shift row 3 - 3 columns to the left
    temp = state->data[3 + 3 * Nb];
    state->data[3 + 3 * Nb] = state->data[2 + 3 * Nb];
    state->data[2 + 3 * Nb] = state->data[1 + 3 * Nb];
    state->data[1 + 3 * Nb] = state->data[0 + 3 * Nb];
    state->data[0 + 3 * Nb] = temp;
}

void AES::SubBytes(Array* state)
{
    for (uint8_t i = 0; i < NUMBER_OF_ROWS; i++)
        for (uint8_t j = 0; j < Nb; j++)
            state->data[j + i * Nb] = Sbox[(uint8_t)state->data[j + i * Nb]];
}

void AES::SubWord(Array* word)
{
    for (uint8_t i = 0; i < NUMBER_OF_ROWS; i++)
        word->data[i] = Sbox[(uint8_t)word->data[i]];
}

uint8_t AES::multiply_gf(uint8_t a, uint8_t b)
{
    uint8_t r = 0;
    uint8_t ax = a;
    uint8_t y = b;

    for (uint8_t i = 0; i < 8; i++)
    {
        //Check the least significant bit
        //If (y & 0x01) == 0, r ^= 0 (r remains unchanged)
        //If (y & 0x01) == 1, r ^= ax
        r ^= ax * (y & 0x01);

        //Multiply ax by the polynomial x modulo (x^8 + x^4 + x^3 + x + 1)
        ax = xtime(ax);

        //Right shift y (prepare data in the least significant bit)
        y >>= 1;
    }

    return r;
}

uint8_t AES::xtime(uint8_t b)
{
    //If (b >> 7) == 0, return (b << 1)
    //If (b >> 7) == 1, return (b << 1) ^ 0x1B
    return (b << 1) ^ (((b >> 7) & 0x01) * 0x1B);
}

void AES::__encrypt(Array* data, Array* state, uint64_t data_index)
{
    //Prepare data
    for (uint8_t j = 0; j < NUMBER_OF_ROWS; j++)
        for (uint8_t k = 0; k < Nb; k++)
            state->data[k + j * Nb] = data->data[j + k * NUMBER_OF_ROWS + data_index];

    //Encrypt data
    Cipher(state);

    //Unload data
    for (uint8_t j = 0; j < NUMBER_OF_ROWS; j++)
        for (uint8_t k = 0; k < Nb; k++)
            data->data[j + k * NUMBER_OF_ROWS + data_index] = state->data[k + j * Nb];
}

void AES::__decrypt(Array* data, Array* state, uint64_t data_index)
{
    //Prepare data
    for (uint8_t j = 0; j < NUMBER_OF_ROWS; j++)
        for (uint8_t k = 0; k < Nb; k++)
            state->data[k + j * Nb] = data->data[j + k * NUMBER_OF_ROWS + data_index];

    //Decrypt data
    InvCipher(state);

    //Unload data
    for (uint8_t j = 0; j < NUMBER_OF_ROWS; j++)
        for (uint8_t k = 0; k < Nb; k++)
            data->data[j + k * NUMBER_OF_ROWS + data_index] = state->data[k + j * Nb];
}

uint8_t AES::__encrypt_CBC(Array* data)
{
    if (!key_set)
        return 1;
    if (!iv_set)
        return 1;

    Array* state = new Array(STATE_SIZE);

    for (uint64_t i = 0; i < data->size(); i += STATE_SIZE)
    {
        //Add IV
        for (uint8_t j = 0; j < NUMBER_OF_ROWS; j++)
            for (uint8_t k = 0; k < Nb; k++)
                data->data[k + j * Nb + i] ^= IV->data[k + j * Nb];

        //Encrypt data
        __encrypt(data, state, i);

        //Save IV for next block
        std::copy(&data->data[i], &data->data[STATE_SIZE + i], IV->data);
    }

    delete state;

    return 0;
}

uint8_t AES::__decrypt_CBC(Array* data)
{
    if (!key_set)
        return 1;
    if (!iv_set)
        return 1;

    Array* state = new Array(STATE_SIZE);
    Array* next_IV = new Array(STATE_SIZE);

    for (uint64_t i = 0; i < data->size(); i += STATE_SIZE)
    {
        //Store IV for next block
        std::copy(&data->data[i], &data->data[STATE_SIZE + i], next_IV->data);

        //Decrypt data
        __decrypt(data, state, i);

        //Add IV
        for (uint8_t j = 0; j < NUMBER_OF_ROWS; j++)
            for (uint8_t k = 0; k < Nb; k++)
                data->data[k + j * Nb + i] ^= IV->data[k + j * Nb];

        //Save IV for next block
        std::copy(&next_IV->data[0], &next_IV->data[STATE_SIZE], IV->data);
    }

    delete state;
    delete next_IV;

    return 0;
}
