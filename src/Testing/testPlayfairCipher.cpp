//! Unit Tests for MPAGSCipher PlayfairCipher Class
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "PlayfairCipher.hpp"

TEST_CASE("Playfair Cipher encryption", "[Playfair]")
{
    PlayfairCipher pc{"playfairtest"};
    REQUIRE(pc.applyCipher("THEQUICKBROWNFOXJUMPSOVERTHELAZYDOG", CipherMode::Encrypt) == "SDSOPBBMCINXQAXYBPKLEQXRTEGSAYXFGNHX");
}

TEST_CASE("Playfair Cipher decryption", "[Playfair]")
{
    PlayfairCipher pc{"playfairtest"};
    REQUIRE(pc.applyCipher("SDSOPBBMCINXQAXYBPKLEQXRTEGSAYXFGNHX", CipherMode::Decrypt) == "THEQUICKBROWNFOXIUMPSOVERTHELAZYDOGZ");
}