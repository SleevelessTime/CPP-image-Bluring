---------------------------------------------------------------------------------------------------------

Thread'lerin Kullanıldığı yerler

Kodunuzda, std::async ile paralel olarak çalıştırılacak işlemler tanımlanıyor. Bu işlemler, görüntüleri bulanıklaştırma ve kaydetme işlemleridir.
boxBlurSection Fonksiyonu

cpp

void boxBlurSection(int width, int height, const std::vector<unsigned char>& input, std::vector<unsigned char>& output, int blur_size, int startY, int endY) {
    for (int y = startY; y < endY; ++y) {
        for (int x = 0; x < width; ++x) {
            int sumR = 0, sumG = 0, sumB = 0;
            int count = 0;

            int y_start = std::max(0, y - blur_size);
            int y_end = std::min(height - 1, y + blur_size);
            int x_start = std::max(0, x - blur_size);
            int x_end = std::min(width - 1, x + blur_size);

            for (int ny = y_start; ny <= y_end; ++ny) {
                for (int nx = x_start; nx <= x_end; ++nx) {
                    int index = (ny * width + nx) * 3;
                    sumR += input[index];
                    sumG += input[index + 1];
                    sumB += input[index + 2];
                    ++count;
                }
            }

            int outputIndex = (y * width + x) * 3;
            output[outputIndex] = sumR / count;
            output[outputIndex + 1] = sumG / count;
            output[outputIndex + 2] = sumB / count;
        }
    }
}

Bu fonksiyon, belirtilen satır aralığında (startY ile endY arasında) piksel değerlerini bulanıklaştırır.
boxBlur Fonksiyonu

cpp

void boxBlur(int width, int height, const std::vector<unsigned char>& input, std::vector<unsigned char>& output, int blur_size) {
    int chunk_size = height / NUM_THREADS;
    std::vector<std::future<void>> futures;

    for (int i = 0; i < NUM_THREADS; ++i) {
        int startY = i * chunk_size;
        int endY = (i == NUM_THREADS - 1) ? height : startY + chunk_size;
        futures.push_back(std::async(std::launch::async, boxBlurSection, width, height, std::cref(input), std::ref(output), blur_size, startY, endY));
    }

    for (auto& future : futures) {
        future.get();
    }
}

    Adım 1: Görüntünün yüksekliğini thread sayısına bölerek her bir thread'in işleyeceği yükseklik aralığını hesaplar (chunk_size).
    Adım 2: Her bir thread için boxBlurSection fonksiyonunu çağırır ve bu işlemleri std::async ile başlatır.
    Adım 3: Tüm future nesnelerini bekler (future.get()), bu şekilde tüm thread'lerin işlemleri tamamlanana kadar ana thread bekler.

processImage Fonksiyonu

cpp

void processImage(const std::string& input_file, int blur_size, std::atomic<int>& success_count, std::atomic<int>& failure_count) {
    int width, height;
    std::vector<unsigned char> input_image;
    std::vector<unsigned char> output_image;

    if (loadPPM(input_file, width, height, input_image)) {
        output_image.resize(width * height * 3);
        boxBlur(width, height, input_image, output_image, blur_size);

        if (savePPM(input_file, width, height, output_image)) {
            ++success_count;
        } else {
            ++failure_count;
        }
    } else {
        ++failure_count;
    }
}

Bu fonksiyon, bir görüntü dosyasını yükler, bulanıklaştırır ve kaydeder. Başarı ve başarısızlık sayısını günceller.
Ana Fonksiyon

cpp

int main() {
    std::string directory = "/home/oguzhancem/Desktop/missionIsBlurred/img";
    int blur_size = 10;

    std::atomic<int> success_count(0);
    std::atomic<int> failure_count(0);
    std::vector<std::future<void>> futures;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".ppm") {
            std::string input_file = entry.path().string();
            futures.push_back(std::async(std::launch::async, processImage, input_file, blur_size, std::ref(success_count), std::ref(failure_count)));
        }
    }

    for (auto& future : futures) {
        future.get();
    }

    std::cout << "Total images processed successfully: " << success_count << std::endl;
    std::cout << "Total images failed to process: " << failure_count << std::endl;

    return 0;
}

    Adım 1: Klasördeki .ppm dosyalarını bulur ve her bir dosya için processImage fonksiyonunu asenkron olarak başlatır.
    Adım 2: Tüm future nesnelerini bekler, böylece tüm işlemler tamamlanana kadar ana thread bekler.
    Adım 3: İşlem sonuçlarını ekrana yazdırır.

Neden std::async ve std::future Kullanıyoruz?

    Kolay Kullanım: std::async ile bir iş parçacığı başlatmak ve yönetmek daha basittir.
    Asenkron İşleme: İşlemleri asenkron olarak başlatıp yönetmek kolaydır.
    Senkranizasyon: std::future ile iş parçacıklarının tamamlanma durumunu kontrol etmek ve sonuçlarını almak mümkündür.

---------------------------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------------------
Kutu Bulanıklaştırma (Box Blur) Nedir ve Nasıl Çalışıyor:

Bir görüntünün her pikseli için komşu piksellerin ortalama değerini hesaplayarak gerçekleştirilir. Bu sayede, keskin kenarlar yumuşatılır ve görüntü bulanık hale getirilir. Bu tür bulanıklaştırmaya "kutu bulanıklaştırma" (box blur) denir.

Giriş Görüntüsü:

    Bulanıklaştırmak istediğimiz orijinal görüntü. Bu görüntü, her piksel için kırmızı (R), yeşil (G) ve mavi (B) değerlerini içeren bir dizi olarak temsil edilir.

Bulanıklaştırma Boyutu:

    Bulanıklaştırma boyutu (örneğin, blur_size = 10), her piksel için ortalama alınacak komşu piksellerin mesafesini belirler. Bu boyut ne kadar büyük olursa, görüntü o kadar bulanık olur.

Ortalama Hesaplama:

    Her piksel için, belirli bir mesafedeki komşu piksellerin renk değerlerinin ortalaması alınır. Bu, R, G ve B bileşenleri için ayrı ayrı yapılır.
    Bu işlem, belirli bir pikselle sınırlı kalmaz, belirli bir alan (kutu) içindeki tüm pikselleri içerir.

Kod kısmı:
    void boxBlurSection(int width, int height, const std::vector<unsigned char>& input, std::vector<unsigned char>& output, int blur_size, int startY, int endY) {
    for (int y = startY; y < endY; ++y) {
        for (int x = 0; x < width; ++x) {
            int sumR = 0, sumG = 0, sumB = 0;
            int count = 0;

            int y_start = std::max(0, y - blur_size);
            int y_end = std::min(height - 1, y + blur_size);
            int x_start = std::max(0, x - blur_size);
            int x_end = std::min(width - 1, x + blur_size);

            for (int ny = y_start; ny <= y_end; ++ny) {
                for (int nx = x_start; nx <= x_end; ++nx) {
                    int index = (ny * width + nx) * 3;
                    sumR += input[index];
                    sumG += input[index + 1];
                    sumB += input[index + 2];
                    ++count;
                }
            }

            int outputIndex = (y * width + x) * 3;
            output[outputIndex] = sumR / count;
            output[outputIndex + 1] = sumG / count;
            output[outputIndex + 2] = sumB / count;
        }
    }
}

Kod Açıklaması

    Döngüler:
        İlk iki for döngüsü, görüntünün her pikselini (x ve y koordinatlarını) dolaşır.

    Toplama ve Sayma:
        sumR, sumG ve sumB değişkenleri, komşu piksellerin kırmızı, yeşil ve mavi bileşenlerinin toplamlarını tutar.
        count değişkeni, ortalama hesaplamak için komşu piksellerin sayısını tutar.

    Komşu Piksel Alanı:
        y_start, y_end, x_start ve x_end değişkenleri, mevcut pikselin komşu piksel alanının sınırlarını belirler.
        Bu sınırlar, bulanıklaştırma boyutuna göre ayarlanır ve görüntü sınırlarının dışına çıkmamalarını sağlamak için std::max ve std::min ile kontrol edilir.

    Komşu Piksel Değerlerini Toplama:
        İç içe for döngüleri, komşu piksel alanındaki tüm pikselleri dolaşır.
        Her pikselin R, G ve B değerleri toplanır ve toplam sayısı (count) arttırılır.

    Ortalama Hesaplama ve Yazma:
        Komşu piksel değerlerinin ortalaması alınır (R, G ve B bileşenleri için ayrı ayrı).
        Ortalaması alınan değerler, çıkış görüntüsünün karşılık gelen pikseline yazılır.

---------------------------------------------------------------------------------------------------------