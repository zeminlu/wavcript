#!/bin/bash
echo "Running tests for Big Endian wavs..."
####
echo "Hiding michelin.jpg with carrier fun.wav: LSB1, BigEndian" 
./bin/criptoSteg -embed -in "results/ejemploBE/michelin.jpg" -p "results/ejemploBE/fun.wav" -out "prueba1.wav" -steg LSB1 -e "big" > /dev/null
echo "Compare result <"
cmp -l prueba1.wav results/ejemploBE/funT1.wav
echo ">"
rm prueba1.wav
####
echo "Hiding michelin.jpg with carrier fun.wav: LSB4, BigEndian" 
./bin/criptoSteg -embed -in "results/ejemploBE/michelin.jpg" -p "results/ejemploBE/fun.wav" -out "prueba4.wav" -steg LSB4 -e "big"  > /dev/null
echo "Compare result <"
cmp -l prueba4.wav results/ejemploBE/funT4.wav
echo ">"
rm prueba4.wav
####
echo "Hiding michelin.jpg with carrier fun.wav: LSB1, BigEndian, encrypted aes192 pass \"oculto\""
./bin/criptoSteg -embed -in "results/ejemploBE/michelin.jpg" -p "results/ejemploBE/fun.wav" -out "pruebaE1.wav" -steg LSB1 -a aes192 -m cbc -pass "oculto" -e "big" > /dev/null
echo "Compare result <"
cmp -l pruebaE1.wav results/ejemploBE/funT1E.wav
echo ">"
rm pruebaE1.wav
####
echo "Unhiding the message in funT1.wav: LSB1, BigEndian" 
./bin/criptoSteg -extract -p "results/ejemploBE/funT1.wav" -out "pruebaExt1" -steg LSB1 -e "big" > /dev/null
echo "Compare result <"
cmp -l pruebaExt1.jpg results/ejemploBE/michelin.jpg
echo ">"
rm pruebaExt1.jpg
####
echo "Unhiding the message in funT4.wav: LSB4, BigEndian" 
./bin/criptoSteg -extract -p "results/ejemploBE/funT4.wav" -out "pruebaExt4" -steg LSB4 -e "big" > /dev/null
echo "Compare result <"
cmp -l pruebaExt4.jpg results/ejemploBE/michelin.jpg
echo ">"
rm pruebaExt4.jpg
####
echo "Unhiding the message in funT1E.wav: LSB1, BigEndian" 
./bin/criptoSteg -extract -p "results/ejemploBE/funT1E.wav" -out "pruebaExt1E" -steg LSB1 -e "big" -a aes192 -m cbc -pass "oculto" > /dev/null
echo "Compare result <"
cmp -l pruebaExt1E.jpg results/ejemploBE/michelin.jpg
echo ">"
rm pruebaExt1E.jpg
#######################################
#######################################
echo "Running tests for Little Endian wavs..."
####
echo "Hiding michelin.jpg with carrier fun.wav: LSB1, LittleEndian" 
./bin/criptoSteg -embed -in "results/ejemploLE/michelin.jpg" -p "results/ejemploLE/fun.wav" -out "prueba1.wav" -steg LSB1  > /dev/null
echo "Compare result <"
cmp -l prueba1.wav results/ejemploLE/funT1LE.wav
echo ">"
rm prueba1.wav
####
echo "Hiding michelin.jpg with carrier fun.wav: LSB4, LittleEndian" 
./bin/criptoSteg -embed -in "results/ejemploLE/michelin.jpg" -p "results/ejemploLE/fun.wav" -out "prueba4.wav" -steg LSB4 > /dev/null
echo "Compare result <"
cmp -l prueba4.wav results/ejemploLE/funT4LE.wav
echo ">"
rm prueba4.wav
####
echo "Hiding michelin.jpg with carrier fun.wav: LSB1, LittleEndian, encrypted aes192 pass \"oculto\""
./bin/criptoSteg -embed -in "results/ejemploLE/michelin.jpg" -p "results/ejemploLE/fun.wav" -out "pruebaE1.wav" -steg LSB1 -a aes192 -m cbc -pass "oculto" > /dev/null
echo "Compare result <"
cmp -l pruebaE1.wav results/ejemploBE/funT1ELE.wav
echo ">"
rm pruebaE1.wav
####
echo "Unhiding the message in funT1.wav: LSB1, LittleEndian" 
./bin/criptoSteg -extract -p "results/ejemploLE/funT1LE.wav" -out "pruebaExt1" -steg LSB1 > /dev/null
echo "Compare result <"
cmp -l pruebaExt1.jpg results/ejemploLE/michelin.jpg
echo ">"
rm pruebaExt1.jpg
####
echo "Unhiding the message in funT4.wav: LSB4, LittleEndian" 
./bin/criptoSteg -extract -p "results/ejemploLE/funT4LE.wav" -out "pruebaExt4" -steg LSB4 > /dev/null
echo "Compare result <"
cmp -l pruebaExt4.jpg results/ejemploLE/michelin.jpg
echo ">"
rm pruebaExt4.jpg
####
echo "Unhiding the message in funT1E.wav: LSB1, LittleEndian" 
./bin/criptoSteg -extract -p "results/ejemploLE/funT1ELE.wav" -out "pruebaExt1E" -steg LSB1 -a aes192 -m cbc -pass "oculto" > /dev/null
echo "Compare result <"
cmp -l pruebaExt1E.jpg results/ejemploLE/michelin.jpg
echo ">"
rm pruebaExt1E.jpg

