V0 --> V1

Değişen tek şey resmi giriş dosyası üstüne yazması onu da çıkış konumu olarak aynı şeyi vermiş olmam bunun sebebi kodda bir hata çıkınça V0 da düzenleyip çalıştığı zaman V1'i güncellemek bu sayded sürekli resim oluşturmama gerek kalmıyor.

V0 : std::string output_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/output.ppm";
V1 :std::string output_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/input.ppm";

V1 --> V1.2

Birden fazla resim dosyası kullanmak için <filesystem> kütüphanesini dahil ettim.

I/O kısmına dosya yerine dizin verdim.

V1 :std::string input_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/input.ppm";
std::string output_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/input.ppm"; // Çıkış dosyası, giriş dosyasının üzerine yazacak
V1.2 :std::string directory = "/home/oguzhancem/Desktop/missionIsBlurred/img";

Dizin işlemleri için yeni döngü ekledim.

V1.2 : for (const auto& entry : std::filesystem::directory_iterator(directory)).
        if (entry.path().extension() == ".ppm") 
            std::string input_file = entry.path().string();
            float width, height;

            std::vector<float> input_image(static_cast<int>(MAX_PIXELS));
            std::vector<float> output_image(static_cast<int>(MAX_PIXELS));

            if (!loadPPM(input_file, width, height, input_image)) 
                std::cerr << "Failed to load image: " << input_file << std::endl;
                ++failure_count;
                continue;

islemler sonrası çıktı yerine sadece başarı ve başarısızlık sayacı ekledim.

V1.2  : int success_count = 0;
        int failure_count = 0;

        std::cout << "Total images processed successfully: " << success_count << std::endl;
        std::cout << "Total images failed to process: " << failure_count << std::endl;

V1.2 --> V2 
Genel optimizasyon için float olan veri tipleri int değerine çevrildi hem sürekli cast etmek zorunda kalmıyoruz.

Box blur işlemindeki gibi geçici vektör kullanımlarının kaldırılması ve gereksiz yapılan kopyalama işlemleri kaldırıldı.

Main fonksiyonunda I/O vektörlerinin yeniden boyutlandırılması işlemi vektörlerin gerekli boyutlandırılmasıyla yapılmış. Böylece gereksiz bellek kullanımı önlenmiş ve kod daha okunabilir hale getirildi.

V2 --> V3
Optimizasyon için veri kopyalamaları kaldırıldı.

data.resize() ve data.data() kullanılarak veri okuma ve yazma işlemleri optimize edildi.

Gerekli olan değişkenler yerel değişkenler olarak tanımlandı.

Bellek erişiminde gereksiz işlemler kaldırıldı.

Döngülerde gereksiz kontroller kaldırıldı ve optimize edildi.

V3 --> V3.1

Mantık değişkenlerini en aza indirerek, continue ifadelerinin kullanılmasını en aza indirdik.

Döngü sınırlarını belirlerken, mantık işlemlerini minimize etmek için std::max ve std::min fonksiyonlarını kullandık.

std::fill kullanarak output vektörünü başlatırken gereksiz döngüleri ortadan kaldırdık.

V3.1 --> V4
 
 #include <thread> eklenerek çoklu iş parçacığı desteği eklenmiş. NUM_THREADS sabiti tanımlanarak toplam thread sayısı belirlenmiş. Bu, işlemlerin paralel olarak yürütülmesini sağlar.

 boxBlur fonksiyonu, resmi işlemek için birden fazla iş parçacığı oluşturacak şekilde yeniden düzenlenmiş. Resmi parçalara bölerek her bir iş parçacığına farklı bir bölge atamış ve bu iş parçacıklarını ayrı thread'lerde çalıştırmış.

 boxBlurSection Fonksiyonu Eklendi: Bu fonksiyon, boxBlur fonksiyonundaki her bir iş parçacığı için belirli bir bölgeyi işlemektedir.

 Kodun başında tanımlanan NUM_THREADS değişkeni, iş parçacığı sayısını belirlemekte kullanılmıştır.


 V4 --> V4.1

 loadPPM ve savePPM fonksiyonlarındaki hata kontrolü işlemleri basitleştirilmiş. Artık sadece dosya açma işlemi başarısız olduğunda false döndürülüyor ve diğer hata durumlarında varsayılan olarak good() fonksiyonu kullanmaya başladık.

Hata durumlarında özel hata mesajları vermedim yani std::cerr kullanmadım.

V4.1 --> V4.2

#include <future> ifadesi eklenerek ve std::future ve std::async kullanılarak boxBlur fonksiyonunun paralelleştirilmesi sağlanmış. Artık iş parçacıklarının sonuçları std::future ile yönetilerek, paralel çalışma sağlanmış.

