## V0 --> V1

### 1. Değişen tek şey resmi giriş dosyası üstüne yazması onu da çıkış konumu olarak aynı şeyi vermiş olmam bunun sebebi kodda bir hata çıkınça V0 da düzenleyip çalıştığı zaman V1'i güncellemek bu sayded sürekli resim oluşturmama gerek kalmıyor.

```
V0 : std::string output_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/output.ppm";
V1 :std::string output_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/input.ppm";
```

## V1 --> V1.2

### 1. Birden fazla resim dosyası kullanmak için <filesystem> kütüphanesini dahil ettim.

### 2. I/O kısmına dosya yerine dizin verdim.

```
V1 :std::string input_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/input.ppm";
std::string output_file = "/home/oguzhancem/Desktop/missionIsBlurred/img/input.ppm"; // Çıkış dosyası, giriş dosyasının üzerine yazacak
V1.2 :std::string directory = "/home/oguzhancem/Desktop/missionIsBlurred/img";
```

### 3. Dizin işlemleri için yeni döngü ekledim.

```
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

```

### 4. islemler sonrası çıktı yerine sadece başarı ve başarısızlık sayacı ekledim.

```
V1.2  : int success_count = 0;
        int failure_count = 0;

        std::cout << "Total images processed successfully: " << success_count << std::endl;
        std::cout << "Total images failed to process: " << failure_count << std::endl;
```

## V1.2 --> V2 

### 1. Genel optimizasyon için float olan veri tipleri int değerine çevrildi hem sürekli cast etmek zorunda kalmıyoruz.

### 2. Box blur işlemindeki gibi geçici vektör kullanımlarının kaldırılması ve gereksiz yapılan kopyalama işlemleri kaldırıldı.

### 3. Main fonksiyonunda I/O vektörlerinin yeniden boyutlandırılması işlemi vektörlerin gerekli boyutlandırılmasıyla yapılmış. Böylece gereksiz bellek kullanımı önlenmiş ve kod daha okunabilir hale getirildi.

## V2 --> V3

### 1.Optimizasyon için veri kopyalamaları kaldırıldı.

### 2. data.resize() ve data.data() kullanılarak veri okuma ve yazma işlemleri optimize edildi.

### 3. Gerekli olan değişkenler yerel değişkenler olarak tanımlandı.

### 4. Bellek erişiminde gereksiz işlemler kaldırıldı.

### 5. Döngülerde gereksiz kontroller kaldırıldı ve optimize edildi.

## V3 --> V3.1

### 1. Mantık değişkenlerini en aza indirerek, continue ifadelerinin kullanılmasını en aza indirdik.

### 2. Döngü sınırlarını belirlerken, mantık işlemlerini minimize etmek için std::max ve std::min fonksiyonlarını kullandık.

### 3. std::fill kullanarak output vektörünü başlatırken gereksiz döngüleri ortadan kaldırdık.

## V3.1 --> V4
 
### 1. #include <thread> eklenerek çoklu iş parçacığı desteği eklenmiş. NUM_THREADS sabiti tanımlanarak toplam thread sayısı belirlenmiş. Bu, işlemlerin paralel olarak yürütülmesini sağlar.

### 2. boxBlur fonksiyonu, resmi işlemek için birden fazla iş parçacığı oluşturacak şekilde yeniden düzenlenmiş. Resmi parçalara bölerek her bir iş parçacığına farklı bir bölge atamış ve bu iş parçacıklarını ayrı thread'lerde çalıştırmış.

### 3. boxBlurSection Fonksiyonu Eklendi: Bu fonksiyon, boxBlur fonksiyonundaki her bir iş parçacığı için belirli bir bölgeyi işlemektedir.

 ### 4. Kodun başında tanımlanan NUM_THREADS değişkeni, iş parçacığı sayısını belirlemekte kullanılmıştır.


## V4 --> V4.1

### 1. loadPPM ve savePPM fonksiyonlarındaki hata kontrolü işlemleri basitleştirilmiş. Artık sadece dosya açma işlemi başarısız olduğunda false döndürülüyor ve diğer hata durumlarında varsayılan olarak good() fonksiyonu kullanmaya başladık.

### 2. Hata durumlarında özel hata mesajları vermedim yani std::cerr kullanmadım.

## V4.1 --> V4.2

### include <future> ifadesi eklenerek ve std::future ve std::async kullanılarak boxBlur fonksiyonunun paralelleştirilmesi sağlanmış. Artık iş parçacıklarının sonuçları std::future ile yönetilerek, paralel çalışma sağlanmış.

## V4.2 --> V5

### 1. processImage Fonksiyonu:
v4.2: Blurlama işlemi main fonksiyonunda doğrudan gerçekleştirilmiştir.
v5: Blurlama işlemi processImage isimli yeni bir fonksiyona taşınmıştır. Bu fonksiyon, belirli bir PPM dosyasını yükler, blurlama işlemini gerçekleştirir ve sonucu kaydeder. Ayrıca, başarı ve başarısızlık sayısını atomik değişkenler kullanarak günceller.

### 2. Çoklu İmage İşleme:
v4.2: Dosya işleme işlemleri sırayla gerçekleştirilmiştir.
v5: Her bir PPM dosyası için std::async kullanılarak ayrı bir iş parçacığı oluşturulmuş ve işlemler paralel olarak gerçekleştirilmiştir. Bu sayede, programın performansı artırılmıştır.

### 3. Atomik Sayaçlar:
v4.2: Başarı ve başarısızlık sayıları normal değişkenler olarak tanımlanmış ve güncellenmiştir.
v5: Bu sayılar std::atomic<int> türüne dönüştürülmüş ve güncellemeler atomik olarak gerçekleştirilmiştir. Bu sayede, paralel işlemler sırasında sayaçların doğru şekilde güncellenmesi sağlanmıştır.

### 4. Kodun Düzenlenmesi:
v5: Kodun okunabilirliğini ve bakımını kolaylaştırmak amacıyla bazı fonksiyonlar (örneğin, processImage) oluşturulmuş ve kod parçalara ayrılmıştır.

## V5 --> V6

### 1. Çoklu İşleme için İş Kuyruğu ve Koşul Değişkeni:
v5: Her bir görüntü std::async kullanılarak işleniyordu.
v6: İşlemler iş kuyruğu ve koşul değişkeni kullanılarak işçi iş parçacıklarına dağıtılıyor. Bu yöntem iş parçacıklarının etkin bir şekilde yönetilmesini sağlıyor.

### 2.İşçi İş Parçacıkları:
v5: Her bir PPM dosyası için std::async ile paralel olarak işleme yapılır.
v6: workerThread fonksiyonu oluşturuldu ve işçi iş parçacıkları bu fonksiyon içinde çalışıyor. İş parçacıkları, iş kuyruğundan iş alıp işleyerek daha verimli çalışıyor.

### 3. Koşul Değişkeni ve İş Kuyruğu Kullanımı:
v5: std::async kullanımı ile paralel işleme yapılır.
v6: std::condition_variable, std::queue, std::mutex kullanılarak işlerin iş kuyruğunda toplanması ve iş parçacıklarına dağıtılması sağlanıyor. Bu, bekleyen işlerin etkin bir şekilde işlenmesini ve iş parçacıklarının daha verimli kullanılmasını sağlıyor.

### 4. Başarı ve Başarısızlık Sayısı Yönetimi:
v5: Başarı ve başarısızlık sayıları std::atomic kullanılarak güncelleniyor.
v6: Başarı ve başarısızlık sayıları std::mutex ile korunan normal değişkenler kullanılarak güncelleniyor. std::lock_guard ile bu değişkenler güvenli bir şekilde güncelleniyor.

### 5. Kodun Genel Yapısı ve Akışı:
v5: Görüntü işleme işlemi doğrudan main fonksiyonunda yapılır.
v6: Görüntü işleme işlemi işçi iş parçacıkları tarafından gerçekleştirilir ve main fonksiyonu sadece iş kuyruğunu doldurur ve iş parçacıklarını başlatır.
