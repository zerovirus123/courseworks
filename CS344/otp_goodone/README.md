# One-Time Pad Encryption
* Client/server simulation of a one-time pad encryption system
* Encrypts and decrypts a plaintext message using a generated key
* Accepts A-Z and space characters
* otp_enc_d and otp_dec_d run in the background as daemons

### Usage
* Generate a key using keygen
  * Length must be at least as long as the plaintext string to encrypt/decrypt
  * Syntax: `keygen keyLength`
  * Example: `keygen 256 > myKey`
* Let otp_enc_d and otp_dec_d run in the background
  * Syntax: `otp_enc_d listening_port`/`otp_dec_d listening_port`
  * Example: `otp_enc_d 57171 &`
  * Example: `otp_dec_d 57172 &`
* Encrypt a plaintext string
  * Syntax: `otp_enc plaintext key port`
  * Example: `otp_enc myPlaintext myKey 57571 > myCiphertext`
* Decrypt a ciphertext string
  * Syntax: `otp_dec ciphertext key port`
  * Example: `otp_dec myCiphertext myKey 57172 > myPlaintextA`

###### To-do
* Refactor common code
