\chapter{Wprowadzenie do problemu}

# Motywacja
Wraz z rosnącym zapotrzebowaniem na oprogramowanie wzrasta ilość nowych technologii (bibliotek, framework'ów itp.) mających na celu
ułatwienie i przyspieszenie pracy programistów. Większość z tych technologii jest tylko kolejną wartstwą abstrakcji nad ich poprzednikami,
co powoduje znaczący wzrost złożoności oraz obniżenie wydajności nawet najprostszych programów.

W związku z gwałtownym rozwojem sprzętu komputerowego (według prawa Moore'a liczba tranzystorów w procesorach wzrasta dwukrotnie co dwa
lata \cite{moore}) spadek wydajności oprogramowania jest praktycznie nieodczuwalny. Zwiększona jest również tolerancja na 
coraz większą ilość błędów w programach. W sytuacji gdy w programie wystąpi błąd użytkownik go ignoruje i ponawia próbę wykonania 
danej czynności lub zwyczajnie uruchamia program ponownie i kontynuuje pracę.

Kolejnym zjawiskiem jest zanikająca wiedza na temat programowania niskopoziomowego. W szkołach i uczelniach przeważnie uczy się 
popularnych języków wykorzystywancyh w programowaniu aplikacji biznesowych (Java, Python, C\#, Javascript itp.), a języki takie 
jak C czy C++ są omawiane bardzo pobieżnie lub wcale. Powoduje to, że programiści nie wiedzą np. jak działa procesor lub czym jest 
pamięć podręczna (ang. cache) procesora. Aktualnie nauczanie programowania skupia się na korzystaniu z konkretych bibliotek i 
framework'ów zamiast na ogólnych zasadach programowania pozwalających tworzyć solidne i wydajne programy.

Jeżeli aktualny trend spadającej jakości oprogramowania oraz sposoby nauczania programowania utrzymają się to wiedza o niskopoziomowym 
programowaniu może zostać zapomniana (lub ograniczona do bardzo specyficznej grupy programistów np. autorów sterowników). Mogłoby się 
wydawać, że jest to niemożliwe jednak w historii zdarzały się już takie sytuacje np.:\\
Mechanizm z Antykithiry - był to mechaniczny kalendarz zbudowany ok. 150 lat p.n.e. przez starożytnych Greków \cite{antikythera} 
(przedstawiony na rysunku @antikythera). 
Była to bardzo złożona konstruckja składająca się z 37 kół zębatych posiadająca możliwości przewidywania takich zjawisk jak:

* pozycje Słońca i Księżyca (uwzględniając jego nierównomierny ruch)
* fazy Księżyca
* zaćmienia Słońca i Księżyca
* wschody i zachody gwiazdozbiorów
* pozycje planet
* daty kolejnych igrzysk olimpijskich

\noindent Pod rządami Rzymian wiedza o budowie tak złożonych mechanizów została zapomniana na ponad 1500 lat (urządzenia o podobnej 
złożoności powstały dopiero w okresie renesansu).

@antikythera Komputerowa rekonstrukcja mechanizmu z Antykithiry

## Cele pracy
Celem tej pracy jest implementacja trzech algorytmów grupowania oraz porównanie ich wydajności z algorytmami dostępnymi w popularnych 
pakietach (np. sklearn). Dodatkowym celem jest również pokazanie, że programowanie niskopoziomowe, wbrew powszechnej opinii, nie jest 
trudne, a także przynosi znaczące korzyści w postaci solidności i wydajności programu.

## Istotność problemu
Problem wydajności oprogramowania zwiększa się z roku na rok. Coraz więcej projektów budowanych jest na zasadzie łączenia ze sobą 
gotowych rozwiązań co powoduje znaczący wzrost złożoności oraz spadek wydajności programów. Programy tworzone bez zależności lub 
z ich minimalną ilością przynoszą wiele korzyści takich jak:

* rozumienie działania programu - programiści dokładnie wiedzą jak działa ich program (brak czarnych skrzynek w postaci bibliotek 
  czy framework'ów).
* oszczędność czasu programistów - programy napisane od zera zazwyczaj kompilują i wykonują się szybciej co pozwala na przyspieszenie 
  cyklu produkcyjnego zwiększając w ten sposób produktywność programistów.
* redukcja kosztów firm - szybsze programy pozwalają na krótsze wynajmowanie chmur obliczeniowych.
* oszczędność energii elektrycznej - w 2018 centra obliczeniowe zużywały szacunkowo 200 TWh energii elektrycznej 
  \cite{datacenter_energy}. Jest to ogromna ilość energii, która pozwoliłaby zasilić 100 milionów domów w Polsce przez rok 
  \cite{poland_energy}. Wydajniejsze programy pozwoliłyby na zwiększenie stosunku wykonanej pracy do zużytej energii. 
  Przewidywane zużycie energii przez centra danych oraz inne technologie zostało przedstawione na rysunku @datacenter_energy.

@datacenter_energy Przewidywane zużycie energii przez technologie informatyczne (centra danych zaznaczone na zielono) \cite{datacenter_energy}

# Uczenie maszynowe

## Czym jest uczenie maszynowe?
Uczenie maszynowe (ang. machine learning) jest to obszar sztucznej inteligencji poświęcony tworzeniu samodoskonalących się algorytmów \cite{ml}. 
Algorytmy te uczą się rozpoznawać wzorce w danych na podstawie tak zwanego zbioru treningowego aby w przyszłości rozpoznawać te same 
wzorce na nieznanych danych. W związku ze wzrastającą popularnością sztucznej inteligencji uczenie maszynowe znajduje coraz więcej 
zastosowań np.:

* rozpoznawanie mowy - np. asystent Google
* rozpoznawanie obrazów - np. rozpoznawanie odręcznego pisma
* automatyczne tłumaczenie
* klasyfikacja danych - np. klasyfikowanie układów gwiezdnych posiadających planety na podstawie zmiennej jasności gwiazdy
* przewidywanie trendów na rynkach finansowych
* sterowanie pojazdami autonomicznymi

## Uczenie maszynowe z nadzorem vs bez nadzoru
Uczenie maszynowe dzieli się na wiele kategorii, jednak najbardziej popularne i powszechnie używane jest uczenie z nadzorem oraz 
uczenie bez nadzoru. Uczenie z nadzorem \cite{sl} polega na przygotowaniu dwóch zbiorów danych: treningowego i testowego. W trakcie uczenia 
algorytm analizuje dane treningowe, wytwarza odpowiedź i porównuje ją z oczekiwanym wynikiem dzięki czemu uczy się rozpoznawać wzorce 
w tych danych. Po zakończeniu uczenia algorytm jest walidowany przy użyciu zbioru testowego. Walidacja polega na porównaniu odpowiedzi 
algorytmu z prawidłowymi odpowiedziami oraz wyliczeniu metryk walidacji określających jakość algorytmu (np. dokładność (ang. accuracy) 
czyli stosunek prawidłowych odpowiedzi do wszystkich odpowiedzi). Do uczenia z nadzorem należą takie algorytmy jak:

* sieci neuronowe
* regresja liniowa
* drzewa decyzyjne
* K najbliższych sąsiadów
* regresja logistyczna

\noindent Uczenie bez nadzoru \cite{ul} od uczenia z nadzorem różni się tym, że w trakcie uczenia algorytm nie porównuje swoich odpowiedzi z 
oczekiwanymi odpowiedziami ze zbioru treningowego. Dzięki temu algorytmy uczenia bez nadzoru znajdują zależności w danych nieoznaczonych. 
Ponieważ algorytmy te operują na danych nieoznaczonych niemożliwa jest ich walidacja (nie wiadomo jaka odpowiedź jest prawidłowa). 
Do uczenia bez nadzoru należą takie algorytmy jak:

* grupowanie
* wykrywanie asocjacji
* wykrywanie anomalii

## Przegląd algorytmów grupowania

### K-Means
Algorytm k-means (nazywany również algorytmem klastrowym) po raz pierwszy został zaproponowany przez Stuart'a Lloyd'a w 1957 roku 
(jednak opbulikowano go dopiero w artykule w 1982 roku) \cite{kmeans_author}. Algorytm ten ma na celu podzielenie $n$ obserwacji na 
$k$ klastrów poprzez minimalizację wewnątrzklastrowej wariancji. 

\noindent Zalety algorytmu k-means:
* prosta implementacja
* dobra skalowalność dla dużych zbiorów danych
* dane grupowane są do klastrów o różnych kształtach

\noindent Wady algorytmu k-means:
* konieczność ręcznego wybrania ilości klastrów
* trudności z grupowaniem danych o zmiennym rozmiarze oraz gęstości
* centroidy klastrów mogą być odciągane od właściwej pozycji przez wartości odstające

### Grupowanie Hierarchiczne
Grupowanie hirerarchiczne ma na celu stworzenie hierarchii klastrów na podstawie ich podobieństwa. W wyniku grupowania powstaje 
dendrogram czyli wielopoziomowa hierarchia, w której klastry z jednego poziomu łącząc się w większe klastry na wyższym poziomie. 
Dzięki takiej reprezentacji możliwe jest wygenerowanie różnej ilości klastrów poprzez odpowiednie rozcięcie dendrogramu.

\noindent Zalety grupowania hierarchicznego:
* brak konieczności ręcznego wybrania ilości klastrów
* możliwość wybrania ilości klastrów po zakończeniu działania algorytmu poprzez rozcięcie dendrogramu
* prosta implementacja

\noindent Wady grupowania hierarchicznego:
* elementy zostają przypisane do klastrów permanentnie - brak możliwości zmiany przydziału
* wysoka czułość na wartości odstające
* wysoka złożoność obliczeniowa ($O(n^3)$) i pamięciowa ($O(n^2)$)

### DBSCAN 
DBSCAN (Density-Based Spatial Clustering of Applications with Noise) to opracowany w 1996 roku (autorzy: Martin Ester, Hans-Peter Kriegel, 
Jörg Sander and Xiaowei Xu \cite{dbscan_author}) algorytm grupujący punkty w przestrzeni znajdujące się bardzo blisko siebie 
jednocześnie odrzucając wartości odstające. Jest to jeden z najpopularniejszych algorytmów grupowania.

\noindent Zalety algorytmu DBSCAN:
* brak konieczności ręcznego wybrania ilości klastrów
* możliwość odnalezienia klastrów o dowolnym kształcie
* odporność na wartości odstające

\noindent Wady algorytmu DBSCAN:
* trudności w grupowaniu danych o różnej gęstości
* dobranie odpowiednich parametrów algorytmu może być trudne w przypadku braku zrozumienia danych
* algorytm nie jest w pełni deterministyczny ponieważ działanie zaczyna od losowego punktu

### Mean Shift
Mean Shift to opracowany w 1975 roku (autorzy: Keinosuke Fukunaga, Larry Hostetler \cite{meanshift_author}) algorytm grupujący 
punkty poprzez ich przesuwanie w stronę środka ciężkości. Algorytm ten głównie używany jest w wizji komputerowej.

\noindent Zalety algorytmu Mean Shift:
* możliwość odnalezienia klastrów o nieregularnym kształcie
* wymagane jest ustawienie tylko jednego parametru - bandwith
* parametr bandwith ma fizyczne znaczenie

\noindent Wady algorytmu Mean Shift:
* dobór odpowiedniej wartości parametru bandwith nie jest trywialny
* słabe działanie w przypadku danych posiadających wiele wymiarów
* często wymagane jest użycie adaptywnej wartości parametry bandwith

# Propozycje rozwiązania problemu

## Brak zależności oraz redukcja abstrakcji
W dzisiejszej kulturze programowania promowane jest jak najczęstsze używanie gotowych rozwiązań w postaci bibliotek i framework'ów. 
Głównym argumentem tego rozumowania jest to, że wiele problemów zostało rozwiązanych przez kogoś innego i nie ma sensu ``wynajdywać 
koła na nowo''. Jednak większość z tych gotowych rozwiązań jest zaprojektowana w sposób generyczny tak aby mogły być zastosowane 
do jak największej ilości wariacji rozwiązywanego problemu. W sytuacji gdy rozwiązywany problem jest tylko podzbiorem jakiegoś 
szerszego zagadnienia zastosowanie gotowego rozwiązania może spowodować znaczący spadek wydajności. Dobrym przykładem takiej sytuacji 
jest prezentacja wygłoszona przez Andreasa Fredrikssona w trakcie konferencji Handmade Seattle 2021 pod tytułem ``Context is everything'' 
\cite{context_is_everything}. W tej prezentacji przedstawiony został przykład parsowania pliku JSON z użyciem gotowej biblioteki oraz 
z użyciem własnego rozwiązania. Ponieważ rozwiązywany problem miał specyficzne założenia pozwoliło to uzyskać trzydziestokrotnie 
szybszy program.

Algorytmy zaimplementowane w tej pracy nie posiadają zewnętrznych zależności oraz posiadają najmniejszą możliwą ilość warstw abstrakcji. 
Takie podejście często wystarcza do stworzenia wydajnego programu ale jeżeli okaże się ono niewystarczające istnieją dodatkowe 
techniki pozwalające na przyspieszenie programu.

## SIMD
SIMD (ang. Single Instruction, Multiple Data) jest rodzajem przetwarzania równoległego, w którym jedna instrukcja wykonywana jest na 
wielu elementach \cite{simd}. SIMD jest tylko ideą - rzeczywiste implementacje instruckji zależą od producentów procesorów. Najczęściej 
instruckcje wspierające SIMD można znaleźć pod nazwami SSE (ang. Streaming SIMD Extensions) lub AVX (ang. Advanced Vector Extensions). 
SIMD pozwala na równoległe wykonywanie podstawowych instrukcji takich jak dodawanie, odejmowanie, mnożnie, dzielenie, porównywanie 
wartości czy pierwiastkowanie zarówno jak i bardziej zaawansowanych operacji np. interpolację wartości.

Główną zaletą SIMD jest możliwość znaczącego przyspieszenia programu poprzez wykonywanie tej samej instruckji na wielu elementach 
korzystając z jednego wątku. Niestety jedną z największych wad SIMD jest to, że nie zawsze możliwe jest jego użycie - nie wszystkie 
algorytmy pozwalają na łatwą wektoryzację.

## Zrównoleglenie obliczeń na CPU
Kolejną możliwością przyspieszenia programu jest zrównoleglenie obliczeń na CPU (ang. Central Processing Unit). Nowoczesne procesory 
posiadają wiele rdzeni pozwalających na równoległe obliczenia. Podobnie jednak jak w przypadku SIMD nie wszystkie algorytmy pozwalają 
na łatwe zrównoleglenie.

## Zrównoleglenie obliczeń na GPU
Alternatywą do obliczeń równoległych na CPU może być przetwarzanie równoległe na GPU (ang. Graphics Processing Unit). W przyapdku 
dużej ilości danych obliczenia na GPU mogą okazać się dużo korzystniejsze niż na CPU ponieważ nowoczesne karty graficzne posiadają 
tysiące rdzeni (rdzenie CUDA \cite{cuda}). Rdzenie te są znacząco wolniejsze od rdzeni CPU jednak ich ilość niweluje różnicę w prędkości. 

# Istniejące rozwiązania
Istnieje wiele gotowych rozwiązań udostępniających algorytmy grupowania. Szczególną popularnością cieszą się pakiety stworzone w języku 
Python, ponieważ język ten dzięki swojej prostej składni pozwala na szybkie stworzenie i ewaluację modelu. Poniżej przedstawione zostały 
wybrane pakiety zawierające implementacje algorytmów grupowania:

* scikit-learn - stworzony w 2007 roku przez Davida Cournapeau pakiet oferujący wiele implementacji algorytmów uczenia maszynowego 
  (modele klasyfikacji, grupowania, regresji itp.) \cite{scikit-learn}. Pakiet ten zaimplementowany jest w języku Python. 
  W tej pracy scikit-learn został użyty jako baza do porównywania wydajności algorytmów grupowania.
* MLlib - biblioteka zbudowana w oparciu o technologię Apache Spark \cite{mllib}. Biblioteka ta oferuje algorytmy klasyfikacji, 
  regresji i grupowania. Jej zaletą jest dostępność API w czterech róznych językach: Scala, Java, Python, R.
* PyClustering - Biblioteka oferująca wiele algorytmów grupowania zaimplementowanych w języku Python lub C++ \cite{pyclustering}. 
  Poza algorytmami grupowania biblioteka ta oferuje także modele sieci neuronowych oraz algorytmy kolorowania grafów.

