# ppm Resim Bulanıklaştırma Uygulaması
Bu proje, bir PPM (P6) resim dosyasını bulanıklaştıran ve sonuçları aynı veya farklı bir dosyaya kaydeden bir C++ programıdır. Maksimum 2000x2000 boyutlarına kadar olan resimleri destekler.V0 dan itibaren optimizasyon iyileştirmeleri ile v6 sürümüne kadar gelmiştir. Uygulama üzerinde temel dosya yolları ve benzer şeyler korunmaya çalışılmıştır yani temel kod yapısı benzerdir.

## Compile etme komutları: 
```
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
```
 

## Resimleri işleme için hazırlama komutu: 
```
./copyScript.sh  
```
Img dizini içinde stop_1.ppm isimli orjinal dosya olması yeterli onu 5000 adete çoğaltacak bash scripti var. 

 

Programı çalıştırma komutu (16 thread): 
```
Time taskset –c 0-15 .exit
```


# CopyScript.sh kopyalama bash script açıklama

## Açıklama:
Verilen Bash betiği, stop_1.ppm adlı bir resim dosyasının 8000 kopyasını oluşturur ve her bir kopyanın dosya adına sıralı bir numara ekler. Bu, benzersiz isimlere sahip çok sayıda resim dosyasının gerekli olduğu görevler için yararlı olabilir.

## Kullanımı:
Betik Konumu: Betiğin, kullanıcının erişebileceği bir dizine yerleştirildiğinden emin olun.
Resim Konumu: Orijinal resim (stop_1.ppm), /home/oguzhancem/Desktop/missionIsBlurred/img/ dizininde bulunmalıdır bu benim kendi dizinim olduğu için sizin bunu değiştirmeniz gerek.

## Betiğin Çalıştırılması:
1. Bir terminal açın.
2. Betiğin bulunduğu dizine gidin.
3. Betiği ```./copy_script.sh``` komutu ile çalıştırın.

## Notlar:
1. Kopyalanacak resim dosyalarını barındırmak için yeterli disk alanına sahip olduğunuzdan emin olun.
2. orjinal resim dosyasının adını ve yolunu cp /home/oguzhancem/Desktop/missionIsBlurred/img/stop_1.ppm kısmında veriyoruz.
3. kopyalama yerini ve yeni dosyaların ismini /home/oguzhancem/Desktop/missionIsBlurred/img/stop_1_$i.ppm kısmında veriyoruz.
4. /stop_1_$i.ppm betiği sürekli aynı dosya isminde kopyalanamayacağı için sayılar artırılıp isme ekleniyor.

