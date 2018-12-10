# DATZ3073
## Pastāvīgie darbi
### Pēteris Pakalns pp15008

**Ir paredzēts, ka visas komandas izsauks caur komandrindu!!!**

#### 1A - RGB bildes sadalīšana pa krāsu kanāliem

__Lietošana:__
```sh
1a.exe <ceļš uz bildi>
python3 1a.py <ceļš uz bildi>
```

Izpildot natīvo lietotni nepieciešams uzinstalēt [gnuplot](http://www.gnuplot.info/).

Izpildot python3 lietotni nepieciešams uzstādīt šādas pakotnes: `numpy`, `matplotlib`, `cv2`.

Abas programmas izzīmē katram krāsas kanālam 3d grafiku, kurš sastāv no punktiem, kur katrs punkts attēlo attiecīgā pikseļa krāsas intensitāti. Natīvā lietotne izzīmē punktu diagrammu, python lietone izzīmē stabiņu diagrammu.

Pieejami testa attēli:
* `test_images/small/unequalized.jpg`
* `test_images/small/3991273.jpg`
* `test_images/small/rainbow.jpg`
* `test_images/small/pallete.jpg`
* `test_images/small/gradient.jpg`
* `test_images/small/son3rot1.jpg`
* `test_images/small/cln1.jpg`

#### 2_1B - Daļiņu modelis

__Lietošana:__
```sh
2_1b.exe
```

Daļiņu modelis (primitīva strūklaka). 66666 daļiņas.

#### 2_2A - DFT, IDFT

__Lietošana:__
```sh
2_2a.exe <ceļš uz bildi>
```

Diskrētā Furjē transformācija.
1) Izvada doto bildi.
2) Izvada bildi, kas iegūta pielietojot DFT uz dotās bildes.
3) Izvada bildi, kas tiek iegūta pielietojot inverso DFT uz 2. punkta bildes. (Jāsakrīt ar orģinālo)

Testēšanai var pielietot *1A* punktā izmantotos testa attēlus.

#### 3D - Daudzstūra izzīmēšana

__Lietošana:__
```sh
3d.exe <ceļš uz daudzstūra apraksta failu>
```

Daudzstūra apraksta faila formāts:
```txt
W H
N
x_1 y_1
x_2 y_2
...
x_N y_N
```
Kur W >= 0, H >= 0 ir attēla augstums, platums. Kur N >= 3 ir daudzstūra virsotņu skaits. x_i, y_i - i - tās virsotnes koordinātes.
Visi skaitļi ir veseli.

Testa faili:
* `test_files/3d.in1` - izzīmē taisnstūri,
* `test_files/3d.in2` - izzīmē daļēji redzamu trīstūri,
* `test_files/3d.in3` - izzīmē daļēji redzamu daudzstūri, kas pats sevi krusto,
* `test_files/3d.in4` - izzīmē taisnstūri, kas pilnībā iekļauj izzīmēto reģionu,
* `test_files/3d.in5` - izzīmē daļēji redzamu daudzstūri, kas pats sevi krusto.

#### 4A - Dodekaedra karkasa modeļa galvenās ģeometriskās transformācijas matricu formā

__Lietošana:__
```sh
4a.exe
```

Dodekaedram tiek pielietotas dažādas ģeometriskās transformācijas ar nejauša lieluma parametriem.
Tas tiks izzīmēts uz ekrāna vispirms ortogrāfiskajā projekcijā, pēc tam perspektīvā projekcijā.

Programmu var izpildīt vairākas reizes, lai novērotu dažādās dodekaedram pielietotās transformācijas.

#### 8B - Histogrammas vienmērīgošana

__Lietošana:__
```sh
8b.exe <ceļš uz bildi>
```

Labs piemērs:
```
8b.exe test_images/small/unequalized.jpg
```

Programma izvada šāda veida attēlus:
1) Oriģinālais RGB attēls,
2) Attēls, kas iegūts pielietojot Opencv histogrammas vienmērīgošanu YCbCr krāsu telpā luminiscences kanālam.
3) Attēls, kas iegūts pielietojot Paša realizētu histogrammas vienmērīgošanu YCbCr krāsu telpā luminiscences kanālam.

Testēšanai var pielietot *1A* punktā izmantotos testa attēlus.


#### 9a - Izpludināšana, assināšana ar dažāda izmēra filtriem

```sh
./9a path_to_image avg_filter_radius gaussian_filter_radius gaussian_filter_sigma sharpen_filter_radius sharpen_alpha
```

Piemēri:
```sh
./9a.exe ./test_images/small/3991273.jpg 3 3 0.7 3 3
./9a.exe ./test_images/small/3991273.jpg 30 30 6 30 30
```

Programma attēlo:
1. Ievadā doto attēlu,
2. Attēlu izplidinātu ar vidējās vērtības filtru (filtra rādiuss: avg_filter_radius)
3. Attēlu izpludinātu ar gausa filtru (rādiuss: gaussian_filter_radius, sigma: gaussian_filter_sigma)
4. Assinātu attēlu, kur izpludinātais attēls iegūts ar vidējās vērtības filtru (rādius: sharpen_filter_radius), 
   asināšanas koeficients: sharpen_alpha.
5. Attēlu izpludinātu, kam pielietots gausa filtrs ar 3. soļa konstantēm.
6. Assinātu 5. soļa attēlu ar 4. solī aprakstītajām konstantēm.


#### 11_1A - Divu krāsainu attēlu normalizēta korelācija

```sh
./11_1a.exe Ceļš_uz_attēlu Ceļš_uz_meklējamo_attēlu
```

Piemēri:
```sh
./11_1a.exe ./test_images/correlation/text.jpg ./test_images/correlation/a.jpg
./11_1a.exe ./test_images/correlation/comet.jpg ./test_images/correlation/comet_search.jpg
./11_1a.exe ./test_images/correlation/asian_friendly.jpg ./test_images/correlation/asian_friendly_part.jpg
```

Programma attēlo:
1. Meklējamo attēla fragmentu,
2. Doto attēlu,
3. Luminiscences normalizēta korelācija,
4. Cb kanāla normalizēta korelācija,
5. Cr kanāla normalizēta korelācija, 
	* Korelāciju uz attēlu pārkonvertētu YCbCr fromātā, jo uz tā novēroju vislabākos rezultātus.
5. Krāsaino attēlu normalizēta korelācija - korelāciju rezultātu veidotā trīsdimensiju vektoru normalizēti garumi.
6. 5. soļa attēls ar 80% slieksni.
