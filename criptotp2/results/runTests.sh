#!/bin/bash
echo "Running tests..."
####
echo "Hiding michelin.jpg with carrier fun.wav: LSB1, BigEndian" 
./bin/criptoSteg -embed -in "results/ejemplo/michelin.jpg" -p "results/ejemplo/fun.wav" -out "prueba1.wav" -steg LSB1 > /dev/null
echo "Compare result <"
cmp -l prueba1.wav results/ejemplo/funT1.wav
echo ">"
rm prueba1.wav
####
echo "Hiding michelin.jpg with carrier fun.wav: LSB4, BigEndian" 
./bin/criptoSteg -embed -in "results/ejemplo/michelin.jpg" -p "results/ejemplo/fun.wav" -out "prueba4.wav" -steg LSB4 > /dev/null
echo "Compare result <"
cmp -l prueba4.wav results/ejemplo/funT4.wav
echo ">"
rm prueba4.wav
####
echo "Hiding michelin.jpg with carrier fun.wav: LSB1, BigEndian, encrypted aes192 pass \"oculto\""
./bin/criptoSteg -embed -in "results/ejemplo/michelin.jpg" -p "results/ejemplo/fun.wav" -out "pruebaE1.wav" -steg LSB1 -a aes192 -m cbc -pass "oculto" > /dev/null
echo "Compare result <"
cmp -l pruebaE1.wav results/ejemplo/funT1E.wav
echo ">"
rm pruebaE1.wav

