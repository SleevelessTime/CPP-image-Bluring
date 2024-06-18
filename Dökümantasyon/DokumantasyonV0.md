versionV0

""" kütüphane eklemelerini yaptığım kısım
#include <iostream>            # Giriş Çıkış işlemleri gerçekleştirmek için kullanılan kütüphane.
#include <fstream>             # Dosya okuma yazma işlemleri için kullanılan kütüphane.
#include <vector>              #Dinamik diziler oluşturmak için kullanılan kütüphane.
"""
(!) Vektor kullanma sebebim dinamik dizi oluştumadan işlemimi tamamlayamadım çok fazla çözüm denedim ama tek segmentasyon hatamı çözeen buydu.V1 de segmentasyon hatası alınca insan kırılıyor tabi.


<bool loadPPM(const std::string& filename, float& width, float& height, std::vector<float>& data)> loadPPM Fonksiyonu

""" Sabit olacak değerlerin tanımlamalarını yaptığım kısım
const float MAX_WIDTH = 2000.0f;
const float MAX_HEIGHT = 2000.0f;
const float MAX_PIXELS = MAX_WIDTH * MAX_HEIGHT * 3.0f;
"""

""" Dosyanın açılması ve kontolünün sağlandığı kısım
std::ifstream infile(filename, std::ios::binary);                       # ifstream kısmı dosya okuma için kullanılıyor std::ios::binary ise dosyanın binary modda okunması için input stream oluşturuyor .
if (!infile) {                                                          #infile dosya açma işleminde hata varsa true değeri gönderiyor ona göre std::cer ile hata mesajı veriyoruz.
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return false;
}
"""

"""PPM dosyasının okunmsaı ve kontrolünün yapıldığı kısım
std::string header;                                                 # Dosyanın ilk satırını yani başlığını saklamak için bir string değeri oluşturuyoruz.
float max_val;
infile >> header >> width >> height >> max_val;                     # Dosyadan sırasıyla header genişlik uzunluk ve maksimum pixel değeri okunur. Bu işlem infile adlı dosya akışından verileri okuruz ve değişkenlere atamasını yaparız.

if (header != "P6" || max_val != 255.0f || width > MAX_WIDTH || height > MAX_HEIGHT) {  #PPM dosyasının doğru biçimde olup olmadığını ve belirli sınırlar içinde uygunluğunu kontrol ediyoruz.
    std::cerr << "Error: Unsupported or oversized PPM format" << std::endl;             # Hata mesajı ve false ile fonksiyonu sonlandırma.
    return false;
}
"""
(!) PPM dosyanız p3 ise bu değeri burdan değiştirmeniz ve bir kaç şeyi değiştirmeniz gerekir. p3 formatı piksel değerleri ascıı karakterlerle tutulurken p6 formatında binary olarak tutulur. Bu yüzden loadPPM de piksel değerlerini ascıı ile almanız gerek ve ona uygun formata dönüştürmeniz gerek savePPM fonksiyonunda ise verileri yazarken binary değil ascıı ile yazmanız, her piksel arası uygun boşluk bırakmanız ve satır sonlarına /n koyacak şekilde düzenlemeniz gerek.

""" Piksel verilerininn okunduğu kısım
infile.get();                                                       #Dosyadan okuduğu bir karakter okur ama bu karakteri kullanmaz atlar.

data.resize(static_cast<int>(MAX_PIXELS));                         # Data isimli vektörün boyutunu ayarlamak için kullanıyoruz. önceden verdiğimiz max_pixel değerine resize ediyoruz ama burda max_pixel float olduğu için static_cast işlemi kullanıyoruz.

for (float i = 0; i < MAX_PIXELS; ++i) {
    unsigned char pixel;                                            # Her dögü başında  pixel adında bir unsigned char türünde değişken tanımlıyoruz ve bu değişken dosyadan okuduğumuz değişkeni geçici olarak saklamak için kullanıyoruz.
    infile.read(reinterpret_cast<char*>(&pixel), 1);                # Bellek adresi &pixel ile belirtilen yere reinterperet_cast<char*> ile point edip dosyadan pixel değeri okumak için kullanıyoruz.
    data[static_cast<int>(i)] = static_cast<float>(pixel);          # Okuduğumuz pixel değerini pixel de geçici olarak sakladıktan sonra floata cast edip data vektöründe i indexine atıyoruz çünkü neden olmasın neden fazla fazla değişken kullanyalım.
}

return true;                                                        # Fonskiyonu tamamlıyoruz.
"""

<bool savePPM(const std::string& filename, float width, float height, const std::vector<float>& data)> savaPPM fonksiyonu

""" Dosya açma kısmı
std::ofstream outfile(filename, std::ios::binary);       # Verilen dosya adını kullanarak bir ofstream nesnesi oluşturur ve dosyayı binary modda açar.
if (!outfile) {                                          # Dosya yoksa hata mesajı verir ve fonksiyonu bitirir.
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return false;
}
"""

""" Dosya başlığını yazma bölümü
outfile << "P6\n" << static_cast<int>(width) << " " << static_cast<int>(height) << "\n255\n";         # Burda PPM dosyasının başlık bilgileri yazılır; p6 formaatı görüntü boyutları ve maksimum renk değeri dosyaya yazılır. ama görüntü boyutları int e cast eder çünkü ppm dosya formatı tam sayı olmalı.
"""

""" Görüntü verilerini yazma
for (float i = 0; i < width * height * 3.0f; ++i) {
    unsigned char pixel = static_cast<unsigned char>(data[static_cast<int>(i)]);                       # Her pixelin kırmızı yeşil ve mavi bileşenlerini sırayla unsigned char olarak dosyaya yazılır
    outfile.write(reinterpret_cast<char*>(&pixel), 1);                                                 # Data vektöründeki her eleman float olduğu için değerler chara cast edilir.
}

return true;                                                                                           # Fonsiyonu tamamlıyoruz. 
"""



<void boxBlur(float width, float height, const std::vector<float>& input, std::vector<float>& output, float blur_size)> boxBlur fonksiyonu

""" Kopyalama bölümü
    std::vector<float> tempInput = input;                               # Vektörleri değişkenlere atadığımız bölüm.
    std::vector<float> tempOutput = output;
"""

""" Pixel dış dögüsü
    for (float y = 0; y < height; ++y)                                  # Y ve X için yapılmıi dögüler tüm pixelleri dönmesi için yapıldı. 
        for (float x = 0; x < width; ++x) 
"""

""" Pixel toplamlarının hesaplanması
        float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;                    # sumR, sumG ve sumB değişkenleri, kırmızı, yeşil ve mavi renk kanallarının toplamlarını tutar.
        float count = 0.0f;                                             # count, bulanıklaştırma işlemi sırasında toplanan piksel sayısını tutar.
"""

""" Çevresel pixel iç döngüsü
    for (float ky = -blur_size; ky <= blur_size; ++ky)                  # ky ve kx döngüleri, her pikselin etrafındaki bulanıklaştırma alanını (çevresel pikselleri) iterasyonla geçer.
       for (float kx = -blur_size; kx <= blur_size; ++kx)
           float ny = y + ky;                                           # ny ve nx, bulanıklaştırma alanındaki geçerli pikselin koordinatlarıdır.
           float nx = x + kx;
           if (nx >= 0.0f && nx < width && ny >= 0.0f && ny < height)   # Geçerli pikselin görüntü sınırları içinde olup olmadığını kontrol eder.
               float index = (ny * width + nx) * 3.0f;

               float pixelR = tempInput[static_cast<int>(index)];       
               float pixelG = tempInput[static_cast<int>(index) + 1];   # index, geçerli pikselin tempInput vektöründeki konumunu hesaplar. (Aşırı önemli (!))
               float pixelB = tempInput[static_cast<int>(index) + 2];
               sumR += pixelR * 1.0f;
               sumG += pixelG * 1.0f;                                   # sumR, sumG ve sumB, geçerli pikselin renk değerlerini toplar.
               sumB += pixelB * 1.0f;
               count += 1.0f;                                           # geçerli piksel sayısını artırır.
"""

""" Bulanıklaştırılmış pixel değerlerini hesaplama ve atamasını yaptığımız kısım
            float outputIndex = (y * width + x) * 3.0f;                                        # Geçerli pikselin tempOutput vektöründeki konumunu hesaplar.
            tempOutput[static_cast<int>(outputIndex)] = sumR / count;
            tempOutput[static_cast<int>(outputIndex) + 1] = sumG / count;                      # Vektöründe, geçerli pikselin bulanıklaştırılmış kırmızı, yeşil ve mavi renk değerleri atanır.
            tempOutput[static_cast<int>(outputIndex) + 2] = sumB / count;
"""

""" Sonuçları değişkene atadığımız kısım
output = tempOutput;                                                                           # Output a temoOutputu atıyoruz burada(Çok önemli olmazsa olmaz.(!)).
"""


<int main> main fonskiyonu

""" Dosya isimleri ve blur boyut tanımlama kısmı
std::string input_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/input.ppm";            # Dosyayı okuyacağımız yer.
std::string output_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/output.ppm";          # Dosyayı oluşturduüumuz yer.
float blur_size = 10.0f;                                                                       # Blur boyutunu tanımladığımız yer.
"""

""" Görüntü boyutlarını ve pixel versini tanımladığımız kısım
float width, height;
std::vector<float> input_image(static_cast<int>(MAX_PIXELS));                                 # Veri tutma kısmı.
std::vector<float> output_image(static_cast<int>(MAX_PIXELS));  
"""

""" Görüntü dosyasını yükleme
if (!loadPPM(input_file, width, height, input_image)) {                                       # Load ppm çağrılır giriş doosyadan yüklenir yoksa hata mesajı verir.
    return -1;                                                                                # -1 ile sonlandırır.
}
"""

""" Box blurun başlatıldığı kısım
boxBlur(width, height, input_image, output_image, blur_size);                                # BoxBlur fonksiyonu çağrılarak giriş görüntüsüne bulanıklaştırma işlemi uygulanır. Sonuçlar output_image vektörüne yazılır.
"""

"""Bulanıklaştırılmış görüntüyü kayıt etme kısmı
if (!savePPM(output_file, width, height, output_image)) {                                    # SavePPM fonksiyonu çağrılarak bulanıklaştırılmış görüntü dosyaya kaydedilir
    return -1;                                                                               # -1 ile sonlandırır.
}
"""

""" islem tamamanlandığına dair mesaj
std::cout << "Image has been blurred and saved successfully!" << std::endl;                  # İşlem başarılı bir şekilde tamamlandığında kullanıcıya bildiren bir mesaj yazdırılır.
"""
