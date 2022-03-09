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

\noindent Pod rządami Rzymian wiedza o budowie tak złożonych mechanizów została zapomniana na ponad 1500 lat (urządzenie o podobnej 
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
Uczenie maszynowe (ang. machine learning) jest to obszar sztucznej inteligencji poświęcony tworzeniu samodoskonalących się algorytmów. 
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
uczenie bez nadzoru. Uczenie z nadzorem polega na przygotowaniu dwóch zbiorów danych: treningowego i testowego. W trakcie uczenia 
algorytm analizuje dane treningowe, wytwarza odpowiedź i porównuje ją z oczekiwanym wynikiem dzięki czemu uczy się rozpoznawać wzorce 
w tych danych. Po zakończeniu uczenia algorytm jest walidowany przy użyciu zbioru testowego. Walidacja polega na porównaniu odpowiedzi 
algorytmu z prawidłowymi odpowiedziami oraz wyliczeniu metryk walidacji określających jakość algorytmu (np. dokładność (ang. accuracy) 
czyli stosunek prawidłowych odpowiedzi do wszystkich odpowiedzi). Do uczenia z nadzorem należą takie algorytmy jak:

* sieci neuronowe
* regresja liniowa
* drzewa decyzyjne
* K najbliższych sąsiadów
* regresja logistyczna

\noindent Uczenie bez nadzoru od uczenia z nadzorem różni się tym, że w trakcie uczenia algorytm nie porównuje swoich odpowiedzi z 
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

### Mean Shift

# Propozycje rozwiązania problemu

## Redukcja abstrakcji

## SIMD

## GPU


