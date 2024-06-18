## Compile etme komutları: 

g++ -g -o exit imgBlurV6.cpp -pthread 

g++ -g -o exit imgBlurV5.cpp -pthread 

g++ -g -o exit imgBlurV4.2.cpp -pthread 

g++ -g -o exit imgBlurV4.1.cpp -pthread 

g++ -o exit imgBlurV4.cpp -pthread 

g++ -o exit imgBlurV3.1.cpp 

g++ -o exit imgBlurV3.cpp 

g++ -o exit imgBlurV2.cpp 

g++ -o exit imgBlurV1.2.cpp 

g++ -o exit imgBlurV1.cpp 

g++ -o exit imgBlurV0.cpp 

 

## Resimleri işleme için hazırlama komutu: 

./copyScript.sh  

Img dizini içinde stop_1.ppm isimli orjinal dosya olması yeterli onu 5000 adete çoğaltacak bash scripti var. 

 

Programı çalıştırma komutu (16 thread): 

Time taskset –c 0-15 .exit  
