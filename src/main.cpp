#include <iostream>

#include "crypto/CryptoNight_x86.h"
#include "xmrig.h"
#include "utils/Mem.h"
#include <assert.h>
#include <inttypes.h>
#include <iostream>

using namespace std;

typedef void (*cn_hash_fun)(const uint8_t *input, size_t size, uint8_t *output, cryptonight_ctx **ctx);

cn_hash_fun hash_fun_select(xmrig::Algo algorithm, xmrig::AlgoVariant av, xmrig::Variant variant)
{

    static const cn_hash_fun func_table[xmrig::VARIANT_MAX * 10 * 3] = {
            cryptonight_single_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_0>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_0>,
            cryptonight_single_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_0>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_0>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_0>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   false, xmrig::VARIANT_0>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  false, xmrig::VARIANT_0>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_0>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   true,  xmrig::VARIANT_0>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  true,  xmrig::VARIANT_0>,

            cryptonight_single_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_1>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_1>,
            cryptonight_single_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_1>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_1>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_1>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   false, xmrig::VARIANT_1>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  false, xmrig::VARIANT_1>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_1>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   true,  xmrig::VARIANT_1>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  true,  xmrig::VARIANT_1>,

            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_TUBE

            cryptonight_single_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_XTL>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_XTL>,
            cryptonight_single_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_XTL>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_XTL>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_XTL>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   false, xmrig::VARIANT_XTL>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  false, xmrig::VARIANT_XTL>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_XTL>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   true,  xmrig::VARIANT_XTL>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  true,  xmrig::VARIANT_XTL>,

            cryptonight_single_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_MSR>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_MSR>,
            cryptonight_single_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_MSR>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_MSR>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_MSR>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   false, xmrig::VARIANT_MSR>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  false, xmrig::VARIANT_MSR>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_MSR>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   true,  xmrig::VARIANT_MSR>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  true,  xmrig::VARIANT_MSR>,

            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_XHV

            cryptonight_single_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_XAO>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_XAO>,
            cryptonight_single_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_XAO>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_XAO>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_XAO>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   false, xmrig::VARIANT_XAO>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  false, xmrig::VARIANT_XAO>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_XAO>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   true,  xmrig::VARIANT_XAO>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  true,  xmrig::VARIANT_XAO>,

            cryptonight_single_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_RTO>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_RTO>,
            cryptonight_single_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_RTO>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_RTO>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, false, xmrig::VARIANT_RTO>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   false, xmrig::VARIANT_RTO>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  false, xmrig::VARIANT_RTO>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT, true,  xmrig::VARIANT_RTO>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT,   true,  xmrig::VARIANT_RTO>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT,  true,  xmrig::VARIANT_RTO>,

#       ifndef XMRIG_NO_AEON
            cryptonight_single_hash<xmrig::CRYPTONIGHT_LITE, false, xmrig::VARIANT_0>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT_LITE, false, xmrig::VARIANT_0>,
            cryptonight_single_hash<xmrig::CRYPTONIGHT_LITE, true,  xmrig::VARIANT_0>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT_LITE, true,  xmrig::VARIANT_0>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT_LITE, false, xmrig::VARIANT_0>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT_LITE,   false, xmrig::VARIANT_0>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT_LITE,  false, xmrig::VARIANT_0>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT_LITE, true,  xmrig::VARIANT_0>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT_LITE,   true,  xmrig::VARIANT_0>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT_LITE,  true,  xmrig::VARIANT_0>,

            cryptonight_single_hash<xmrig::CRYPTONIGHT_LITE, false, xmrig::VARIANT_1>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT_LITE, false, xmrig::VARIANT_1>,
            cryptonight_single_hash<xmrig::CRYPTONIGHT_LITE, true,  xmrig::VARIANT_1>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT_LITE, true,  xmrig::VARIANT_1>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT_LITE, false, xmrig::VARIANT_1>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT_LITE,   false, xmrig::VARIANT_1>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT_LITE,  false, xmrig::VARIANT_1>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT_LITE, true,  xmrig::VARIANT_1>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT_LITE,   true,  xmrig::VARIANT_1>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT_LITE,  true,  xmrig::VARIANT_1>,

            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_TUBE
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_XTL
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_MSR
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_XHV
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_XAO
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_RTO
#       else
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
#       endif

#       ifndef XMRIG_NO_SUMO
            cryptonight_single_hash<xmrig::CRYPTONIGHT_HEAVY, false, xmrig::VARIANT_0>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT_HEAVY, false, xmrig::VARIANT_0>,
            cryptonight_single_hash<xmrig::CRYPTONIGHT_HEAVY, true,  xmrig::VARIANT_0>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT_HEAVY, true,  xmrig::VARIANT_0>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT_HEAVY, false, xmrig::VARIANT_0>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT_HEAVY,   false, xmrig::VARIANT_0>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT_HEAVY,  false, xmrig::VARIANT_0>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT_HEAVY, true,  xmrig::VARIANT_0>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT_HEAVY,   true,  xmrig::VARIANT_0>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT_HEAVY,  true,  xmrig::VARIANT_0>,

            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_1

            cryptonight_single_hash<xmrig::CRYPTONIGHT_HEAVY, false, xmrig::VARIANT_TUBE>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT_HEAVY, false, xmrig::VARIANT_TUBE>,
            cryptonight_single_hash<xmrig::CRYPTONIGHT_HEAVY, true,  xmrig::VARIANT_TUBE>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT_HEAVY, true,  xmrig::VARIANT_TUBE>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT_HEAVY, false, xmrig::VARIANT_TUBE>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT_HEAVY,   false, xmrig::VARIANT_TUBE>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT_HEAVY,  false, xmrig::VARIANT_TUBE>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT_HEAVY, true,  xmrig::VARIANT_TUBE>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT_HEAVY,   true,  xmrig::VARIANT_TUBE>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT_HEAVY,  true,  xmrig::VARIANT_TUBE>,

            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_XTL
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_MSR

            cryptonight_single_hash<xmrig::CRYPTONIGHT_HEAVY, false, xmrig::VARIANT_XHV>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT_HEAVY, false, xmrig::VARIANT_XHV>,
            cryptonight_single_hash<xmrig::CRYPTONIGHT_HEAVY, true,  xmrig::VARIANT_XHV>,
            cryptonight_double_hash<xmrig::CRYPTONIGHT_HEAVY, true,  xmrig::VARIANT_XHV>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT_HEAVY, false, xmrig::VARIANT_XHV>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT_HEAVY,   false, xmrig::VARIANT_XHV>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT_HEAVY,  false, xmrig::VARIANT_XHV>,
            cryptonight_triple_hash<xmrig::CRYPTONIGHT_HEAVY, true,  xmrig::VARIANT_XHV>,
            cryptonight_quad_hash<xmrig::CRYPTONIGHT_HEAVY,   true,  xmrig::VARIANT_XHV>,
            cryptonight_penta_hash<xmrig::CRYPTONIGHT_HEAVY,  true,  xmrig::VARIANT_XHV>,

            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_XAO
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, // VARIANT_RTO
#       else
    nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
#       endif
    };

    const size_t index = xmrig::VARIANT_MAX * 10 * algorithm + 10 * variant + av - 1;

#   ifndef NDEBUG
    cn_hash_fun func = func_table[index];

    return func;
#   else
    return func_table[index];
#   endif
}

// blob : 0707b7b2cbdb05e8af0310798932ad273fe81c61883b0a877ce3c0c262ce5f6a5e21b40063b7c600000000c10ad46d7c16835c7188c60fdcbed0a25ce8b7bf3b201fc55c0d8c75600d87d203
// target : 7b5e0400


const static uint8_t test_input[380] = {
        0x03, 0x05, 0xA0, 0xDB, 0xD6, 0xBF, 0x05, 0xCF, 0x16, 0xE5, 0x03, 0xF3, 0xA6, 0x6F, 0x78, 0x00,
        0x7C, 0xBF, 0x34, 0x14, 0x43, 0x32, 0xEC, 0xBF, 0xC2, 0x2E, 0xD9, 0x5C, 0x87, 0x00, 0x38, 0x3B,
        0x30, 0x9A, 0xCE, 0x19, 0x23, 0xA0, 0x96, 0x4B, 0x00, 0x00, 0x00, 0x08, 0xBA, 0x93, 0x9A, 0x62,
        0x72, 0x4C, 0x0D, 0x75, 0x81, 0xFC, 0xE5, 0x76, 0x1E, 0x9D, 0x8A, 0x0E, 0x6A, 0x1C, 0x3F, 0x92,
        0x4F, 0xDD, 0x84, 0x93, 0xD1, 0x11, 0x56, 0x49, 0xC0, 0x5E, 0xB6, 0x01,
        0x01, 0x00, 0xFB, 0x8E, 0x8A, 0xC8, 0x05, 0x89, 0x93, 0x23, 0x37, 0x1B, 0xB7, 0x90, 0xDB, 0x19,
        0x21, 0x8A, 0xFD, 0x8D, 0xB8, 0xE3, 0x75, 0x5D, 0x8B, 0x90, 0xF3, 0x9B, 0x3D, 0x55, 0x06, 0xA9,
        0xAB, 0xCE, 0x4F, 0xA9, 0x12, 0x24, 0x45, 0x00, 0x00, 0x00, 0x00, 0xEE, 0x81, 0x46, 0xD4, 0x9F,
        0xA9, 0x3E, 0xE7, 0x24, 0xDE, 0xB5, 0x7D, 0x12, 0xCB, 0xC6, 0xC6, 0xF3, 0xB9, 0x24, 0xD9, 0x46,
        0x12, 0x7C, 0x7A, 0x97, 0x41, 0x8F, 0x93, 0x48, 0x82, 0x8F, 0x0F, 0x02,
        0x07, 0x07, 0xB4, 0x87, 0xD0, 0xD6, 0x05, 0x26, 0xE0, 0xC6, 0xDD, 0x9B, 0xC7, 0x18, 0xC3, 0xCF,
        0x52, 0x04, 0xBD, 0x4F, 0x9B, 0x27, 0xF6, 0x73, 0xB9, 0x3F, 0xEF, 0x7B, 0xB2, 0xF7, 0x2B, 0xBB,
        0x3F, 0x3E, 0x9C, 0x3E, 0x9D, 0x33, 0x1E, 0xDE, 0xAD, 0xBE, 0xEF, 0x4E, 0x00, 0x91, 0x81, 0x29,
        0x74, 0xB2, 0x70, 0xE7, 0x6D, 0xD2, 0x2A, 0x5F, 0x52, 0x04, 0x93, 0xE6, 0x18, 0x89, 0x40, 0xD8,
        0xC6, 0xE3, 0x90, 0x6E, 0xAA, 0x6A, 0xB7, 0xE2, 0x08, 0x7E, 0x78, 0x0E,
        0x01, 0x00, 0xEE, 0xB2, 0xD1, 0xD6, 0x05, 0xFF, 0x27, 0x7F, 0x26, 0xDB, 0xAA, 0xB2, 0xC9, 0x26,
        0x30, 0xC6, 0xCF, 0x11, 0x64, 0xEA, 0x6C, 0x8A, 0xE0, 0x98, 0x01, 0xF8, 0x75, 0x4B, 0x49, 0xAF,
        0x79, 0x70, 0xAE, 0xEE, 0xA7, 0x62, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x47, 0x8C, 0x63, 0xE7, 0xD8,
        0x40, 0x02, 0x3C, 0xDA, 0xEA, 0x92, 0x52, 0x53, 0xAC, 0xFD, 0xC7, 0x8A, 0x4C, 0x31, 0xB2, 0xF2,
        0xEC, 0x72, 0x7B, 0xFF, 0xCE, 0xC0, 0xE7, 0x12, 0xD4, 0xE9, 0x2A, 0x01,
        0x07, 0x07, 0xA9, 0xB7, 0xD1, 0xD6, 0x05, 0x3F, 0x0D, 0x5E, 0xFD, 0xC7, 0x03, 0xFC, 0xFC, 0xD2,
        0xCE, 0xBC, 0x44, 0xD8, 0xAB, 0x44, 0xA6, 0xA0, 0x3A, 0xE4, 0x4D, 0x8F, 0x15, 0xAF, 0x62, 0x17,
        0xD1, 0xE0, 0x92, 0x85, 0xE4, 0x73, 0xF9, 0x00, 0x00, 0x00, 0xA0, 0xFC, 0x09, 0xDE, 0xAB, 0xF5,
        0x8B, 0x6F, 0x1D, 0xCA, 0xA8, 0xBA, 0xAC, 0x74, 0xDD, 0x74, 0x19, 0xD5, 0xD6, 0x10, 0xEC, 0x38,
        0xCF, 0x50, 0x29, 0x6A, 0x07, 0x0B, 0x93, 0x8F, 0x8F, 0xA8, 0x10, 0x04
};


struct cryptonight_ctx;


void print_uint8_t(uint8_t x) {
    printf("%" PRIu8, x);
}

int main() {
    size_t N = 5;
    uint8_t m_hash[32];
    cryptonight_ctx *m_ctx[5];
    MemInfo m_memory = Mem::create(m_ctx, xmrig::CRYPTONIGHT, N);

    cn_hash_fun f = hash_fun_select(xmrig::CRYPTONIGHT,xmrig::AV_AUTO,xmrig::VARIANT_1);
    f(test_input, 76, m_hash, m_ctx);

    for (int i = 0; i < 32; ++i) {
        cout << int(i) << endl;
    }

    return 0;
}
