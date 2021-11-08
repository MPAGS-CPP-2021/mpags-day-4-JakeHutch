#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include "CipherMode.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <map>

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
  public:

    /**
     * \brief Create a new PlayfairCipher, converting the given string into the key
     *
     * \param key the string to convert into the key to be used in the cipher
     */
    PlayfairCipher(const std::string& key);

    /**
     * \brief Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher(const std::string& inputText,
                            const CipherMode cipherMode) const;

    /**
     * \brief Sets up the Key grid from the provided key phrase
     * 
     * \param key the string to be converted into the key grid
     */
    void setKey(const std::string& key);

  private:
    /// The cipher key, essentially a constant shift to be applied
    std::string key_{"A"};

    using coord = std::pair<int, int>;
    /// A map to convert from coordinates in the grid to a letter
    std::map <coord, char> coordToLetter_ ;
    /// A map to convert from a letter in the grid to coordinates
    std::map <char, coord> letterToCoord_ ;
};

#endif