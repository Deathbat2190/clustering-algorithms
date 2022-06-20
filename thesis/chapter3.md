\chapter{Rozwiązanie problemu}

# Zbiór danych
W tej pracy do sprawdzenia poprawności i wydajności zaimplementowanych algorytmów zostały wykorzystane różne warianty tego samego zbioru 
danych. Zbiór danych został wygenerowany za pomocą funkcji \verb|make_blobs| \cite{make_blobs} z pakietu scikit-learn. Funkcja ta pozwala 
na wygenerowanie dowolnej liczby skupisk punktów. Dla ułatwienia wizualizacji zbiór danych składał się z dwuwymiarowych punktów. 
Parametry z jakimi wywołana była ta funkcja to:

* \verb|n_samples| - ilość punktów w zbiorze - wartość ta była zmieniana w zależności od testowanego algorytmu, jej zakres wynosił od 1000 do 
  1000000,
* \verb|centers| - ilość skupisk w zbiorze - wartość ta zawsze wynosiła 5,
* \verb|random_state| - stan generatora losowych liczb - wartość ta zawsze wynosiła 0 co pozwoliło na generowanie tego samego zbioru przy 
  każdym uruchomieniu programu.

\noindent Na rysunku @dataset przedstawiono cztery przykładowe warianty zbioru danych.

\clearpage
@dataset Warianty zbioru danych

\clearpage
# Sposoby implementacji algorytmów
W tej sekcji zostały opisane sposoby implementacji wybranych algorytmów grupowania. Dla każdego z algorytmów opisany został interfejs 
algorytmu w języku Python oraz C++, a także zastosowane techniki programistyczne.

## K-Means
Interfejs algorytmu K-Means w języku Python to jedna klasa posiadająca następujące metody:

* \verb|__init__(self, cluster_count, random_state, max_iterations)| - konstruktor posiadający następujące parametry:
    * \verb|cluster_count| - ilość klastrów, na które ma być podzielony zbiór danych,
    * \verb|random_state| - stan generatora liczb losowych odpowiedzialnego za początkową inicjalizację centroidów,
    * \verb|max_iterations| - maksymalna ilość iteracji algorytmu,
* \verb|fit(self, data)| - metoda uruchamiająca algorytm na danych przekazanych przez parameter \verb|data|,
* \verb|predict(self, data)| - metoda pozwalająca na przypisanie nowych danych do klastrów utworzonych w wyniku działania algorytmu,
* \verb|__del__(self)| - destruktor zwalniający zaalokowaną pamięć.

\noindent Interfejs algorytmu K-Means w języku C++ jest bardzo podobny do interfejsu w języku Python. Dodatkowym elementem jest struktura 
o nazwie \verb|KMeans_State|. Struktura ta jest wykorzystywana do przekazywania informacji o wykonywanym algorytmie między funkcjami. 
Posiada ona następujące pola:

* \verb|int clusterCount| - ilość klastrów, na które ma być podzielony zbiór danych,
* \verb|int randomState| - stan generatora liczb losowych,
* \verb|int maxIterations| - maksymalna ilość iteracji algorytmu,
* \verb|float32 *centers| - wskaźnik do listy zawierającej dane centroidów.

\noindent Funkcje zaimplementowane w języku C++ są następujące:

* \verb|void *KMeansInit( int clusterCount, int randomState,|\\
\verb|int maxIterations )| - funkcja tworząca i zwracająca instancję struktury 
  \verb|KMeans_State|. Funkcja ta jest wywoływana przez konstruktor w języku Python oraz posiada identyczne parametry,
* \verb|void KMeansFit( KMeans_State *state, float32 *data,|\\ 
\verb|int sampleCount, int sampleSize )| - funkcja wywoływana przez metodę \verb|fit| w języku Python. Jej parametry to:
    * \verb|KMeans_State *state| - wskaźnik do instancji struktury \verb|KMeans_State| stworzonej przez funkcję \verb|KMeansInit|,
    * \verb|float32 *data| - wskaźnik do listy zawierajacej zbiór danych,
    * \verb|int sampleCount| - ilość punktów w zbiorze danych,
    * \verb|int sampleSize| - wymiarowość punktów w zbiorze danych,
* \verb|int *KMeansPredict( KMeans_State *state, float32 *data,|\\
\verb|int sampleCount, int sampleSize )| - funkcja wywoływana przez metodę 
  \verb|predict| w języku Python. Jej parametry są takie same jak w funkcji \verb|KMeansFit|,
* \verb|void KMeansCleanup( KMeans_State *state )| - funkcja wywoływana przez destruktor w języku Python.

### Zastosowane techniki oraz wyniki pomiarów
W przypadku algorytmu K-Means zastosowano trzy techniki, które pozwoliły uzyskać zadowalające wyniki:

* Usunięcie zbędnych abstrakcji, brak zależności w postaci zewnętrznych bibliotek oraz minimalne użycie standardowej biblioteki 
  języka C++ - użyty był tylko generator liczb losowych z modułu \verb|<random>|.
* Pisanie możliwie najprostszego kodu - cała implementacja algorytmu K-Means ma zaledwie 144 linie kodu (wliczając w to puste linie oraz 
  linie z pojedyńczymi nawiasami klamrowymi).
* Pominięcie operacji pierwiastkowania w trakcie liczenia odległości euklidesowej - w przypadku, w którym ustalenie, która odległość 
  jest większa jest ważniejsze niż sama wartość odległości można pominąć operację pierwiastkowania.

\noindent Aby zapewnić uczciwość pomiaru czasu wykonywania algorytmu mierzony był czas wykonania całego algorytmu oraz zapisywana była 
ilość iteracji, a następnie obliczany był czas wykonania jednej iteracji. Dla każdej wartości punktów w zbiorze danych wykonane 
zostały trzy pomiary, które nastepnie zostały uśrednione aby otrzymać ostateczny wynik. W tabelach \ref{kmeans_time_custom} i 
\ref{kmeans_time_sklearn} przedstawiono wyniki pomiarów. Na rysunku @kmeans_time przedstawiono wyniki pomiarów w postaci wykresu 
punktowego.

\begin{table}[h]
    \begin{center}
    \begin{tblr}{ hlines, vlines, colspec={X[2.5,c] X[1,l] X[1,l] X[1,l] X[1,l] X[1,l] X[1,l]}}
        **Ilość punktów** & 10000 & 50000 & 100000 & 300000 & 600000 & 1000000 \\ 
        
        **Czas\\wykonywania (ms)** & 2.3 & 13.0 & 50.6 & 125.0 & 1984.0 & 4526.0 \\ 
        
        **Ilość iteracji** & 10 & 12 & 24 & 20 & 156 & 229 \\ 
        
        **Czas jednej\\iteracji (ms)** & 0.23 & 1.08 & 2.10 & 6.25 & 12.71 & 19.76 \\
    \end{tblr}

    \caption{Wyniki pomiarów własnej implementacji K-Means}
    \label{kmeans_time_custom}
    \end{center}
\end{table}

\begin{table}[h]
    \begin{center}
    \begin{tblr}{ hlines, vlines, colspec={X[2.5,c] X[1,l] X[1,l] X[1,l] X[1,l] X[1,l] X[1,l]}}
        **Ilość punktów** & 10000 & 50000 & 100000 & 300000 & 600000 & 1000000 \\ 
        
        **Czas\\wykonywania (ms)** & 6.3 & 48.9 & 56.1 & 182.0 & 1248.0 & 7220.0 \\ 
        
        **Ilość iteracji** & 10 & 25 & 15 & 16 & 66 & 300 \\ 
        
        **Czas jednej\\iteracji (ms)** & 0.63 & 1.95 & 3.74 & 11.37 & 18.90 & 24.06 \\
    \end{tblr}

    \caption{Wyniki pomiarów K-Means z pakietu scikit-learn}
    \label{kmeans_time_sklearn}
    \end{center}
\end{table}
\clearpage
@kmeans_time Pomiary czasu wykonywania algorytmu K-Means

## Grupowanie Hierarchiczne
Interfejs algorytmu grupowania hierarchicznego w języku Python to jedna klasa posiadająca następujące metody:

* \verb|__init__(self, cluster_count)| - konstruktor posiadający jeden parametr \verb|cluster_count| odpowiedzialny za ustalenie ilości 
  klastrów, na które ma być podzielony zbiór danych,
* \verb|fit(self, data)| - metoda uruchamiająca algorytm na danych przekazanych przez parameter \verb|data|,
* \verb|__del__(self)| - destruktor zwalniający zaalokowaną pamięć.

\noindent Klasa ta posiada także pole \verb|labels| pozwalające na dostęp do informacji o przypisaniu punktów ze zbioru danych 
do klastrów.

\noindent Interfejs algorytmu grupowania hierarchicznego w języku C++ jest bardzo podobny do interfejsu w języku Python. 
Dodatkowym elementem jest struktura o nazwie \verb|Hierarchical_State|. Struktura ta jest wykorzystywana do przekazywania informacji o 
wykonywanym algorytmie między funkcjami. Posiada ona następujące pola:

* \verb|int sampleCount| - ilość punktów w zbiorze danych,
* \verb|int sampleSize| - wymiarowość punktów w zbiorze danych,
* \verb|int clusterCount| - ilość klastrów, na które ma być podzielony zbiór,
* \verb|int *labels| - wskaźnik do listy z indeksami klastrów przypisanych do poszczególnych punktów ze zbioru danych. 

\noindent Funkcje zaimplementowane w języku C++ są następujące:

* \verb|void *HierarchicalInit( int clusterCount )| - funkcja tworząca i zwracająca instancję struktury \verb|Hierarchical_State|. 
  Funkcja ta jest wywoływana przez konstruktor w języku Python oraz posiada identyczne parametry,
* \verb|int *HierarchicalFit( Hierarchical_State *state, float32 *data,|\\
\verb|int sampleCount, int sampleSize )| - funkcja wywoływana przez 
  metodę \verb|fit| w języku Python. Jej parametry to:
    * \verb|Hierarchical_State *state| - wskaźnik do instancji struktury\\ \verb|Hierarchical_State| stworzonej przez funkcję \verb|HierarchicalInit|,
    * \verb|float32 *data| - wskaźnik do listy zawierajacej zbiór danych,
    * \verb|int sampleCount| - ilość punktów w zbiorze danych,
    * \verb|int sampleSize| - wymiarowość punktów w zbiorze danych,
* \verb|void HierarchicalCleanup( Hierarchical_State *state )| - funkcja wywoływana przez destruktor w języku Python.

### Zastosowane techniki oraz wyniki pomiarów
W przypadku algorytmu grupowania hierarchicznego pomimo zastosowanych technik nie udało się osiągnąć zadowalających rezultatów. 
Zastosowane techniki to:

* Usunięcie zbędnych abstrakcji oraz brak zależności w postaci zewnętrznych bibliotek.
* Pisanie możliwie najprostszego kodu - cała implementacja algorytmu grupowania hierarchicznego ma zaledwie 199 linii kodu (wliczając w to 
  puste linie oraz linie z pojedyńczymi nawiasami klamrowymi).
* Użycie dodatkowej tablicy zawierającej obliczone dystansy między klastrami - dzięki tej tablicy w każdej iteracji algorytmu 
  aktualizowane były tylko dystansy między nowo powstałym klastrem a pozostałymi klastrami. Bez tej tablicy w każdej iteracji algorytmu 
  dystansy między wszystkimi klastrami musiałyby być obliczane na nowo.

\noindent W przeciwieństwie do algorytmu K-Means w algorytmie grupowania hierarchicznego wystarczy mierzyć całkowity czas wykonania 
algorytmu, ponieważ algorytm ten nie posiada elementów losowosci i ilość iteracji zależna jest tylko od ilości punktów w zbiorze danych. 
Dla każdej wartości punktów w zbiorze danych wykonane zostały trzy pomiary, które nastepnie zostały uśrednione aby otrzymać ostateczny 
wynik. W tabelach \ref{hierarchical_time_custom} i \ref{hierarchical_time_sklearn} przedstawiono wyniki pomiarów. Na 
rysunku @hierarchical_time przedstawiono wyniki pomiarów w postaci wykresu punktowego.

\begin{table}[h]
    \begin{center}
    \begin{tblr}{ hlines, vlines, colspec={X[2.5,c] X[1,l] X[1,l] X[1,l] X[1,l] X[1,l]}}
        **Ilość punktów** & 1000 & 2000 & 3000 & 4000 & 5000 \\ 
        
        **Czas\\wykonywania (ms)** & 159.0 & 1251.0 & 4325.0 & 14067.0 & 32378.0 \\ 
        
    \end{tblr}

    \caption{Wyniki pomiarów własnej implementacji grupowania hierarchicznego}
    \label{hierarchical_time_custom}
    \end{center}
\end{table}

\begin{table}[h]
    \begin{center}
    \begin{tblr}{ hlines, vlines, colspec={X[2.5,c] X[1,l] X[1,l] X[1,l] X[1,l] X[1,l]}}
        **Ilość punktów** & 1000 & 2000 & 3000 & 4000 & 5000 \\ 
        
        **Czas\\wykonywania (ms)** & 11.3 & 43.2 & 105.6 & 215.0 & 345.5 \\ 
        
    \end{tblr}

    \caption{Wyniki pomiarów grupowania hierarchicznego z pakietu scikit-learn}
    \label{hierarchical_time_sklearn}
    \end{center}
\end{table}

@hierarchical_time Pomiary czasu wykonywania grupowania hierarchicznego
\clearpage

\noindent W algorytmie grupowania hierarchicznego zastosowana została tablica zawierająca obliczone dystansy między klastrami. Pomimo, 
że tablica ta zaalokowana była jako jeden jednowymiarowy blok pamięci interpretowana była ona jako tablica dwuwymiarowa o 
rozmiarze $n * n$ gdzie $n$ to ilość punktów w zbiorze danych. Każdy klaster posiadał swój indeks, który odpowiadał indeksowi w tabeli dystansów. 
Ponieważ tablica ta była symetryczna dystans między dwoma klastrami można odczytać na dwa sposoby. Zakładając, że klaster $A$ ma 
indeks $I_A$ oraz klaster $B$ ma indeks $I_B$, dystans między klastrami można odczytać z pola w tablicy o indeksie $I_A * n + I_B$ lub 
$I_B * n + I_A$. W sytuacji gdy wyszukiwany jest dystans między klastrem $A$ a wszystkimi pozostałymi klastrami indeksowanie w postaci 
$I_A * n + I_B$ jest znacząco szybsze, ponieważ przy odczytywaniu wartości z tablicy do pamięci podręcznej procesora ładowana jest nie 
tylko ta wartość ale też 15 wartości sąsiadujących z nią (zazwyczaj ładowane są 64 bajty na raz czyli 16 wartości liczb 
zmiennoprzecinkowych), co oznacza, że odczytanie dystansu między klastrem $A$ a klastrem o indeksie $I_B + 1$ będzie praktycznie 
natychmiastowe. W przypadku indeksowania w postaci $I_B * n + I_A$ do pamięci podręcznej załadowana zostanie potrzebna wartość jednak 
15 pozostałych wartości zostanie zignorowanych i procesor chcąc odczytać następną wartość na indeksie $(I_B + 1) * n + I_A$ będzie musiał 
załadować nowe wartości z pamięci podręcznej wyższego poziomu lub z pamięci RAM. W tabeli \ref{hierarchical_cache_table} przedstawiono wyniki 
pomiaru czasu wykonywania algorytmu z indeksowaniem w postaci $I_B * n + I_A$, a na rysunku @hierarchical_cache przedstawiono wykres 
porównujący oba sposoby indeksowania.

\clearpage
\begin{table}[h]
    \begin{center}
    \begin{tblr}{ hlines, vlines, colspec={X[2.5,c] X[1,l] X[1,l] X[1,l] X[1,l] X[1,l]}}
        **Ilość punktów** & 1000 & 2000 & 3000 & 4000 & 5000 \\ 
        
        **Czas\\wykonywania (ms)** & 227.3 & 1845.0 & 9801.0 & 33197.0 & 77886.0 \\ 
        
    \end{tblr}

    \caption{Wyniki pomiarów własnej implementacji grupowania hierarchicznego z wolnym indeksowaniem}
    \label{hierarchical_cache_table}
    \end{center}
\end{table}

@hierarchical_cache Porównanie sposobów indeksowania tablicy dystansów
\clearpage
## DBSCAN
Interfejs algorytmu DBSCAN w języku Python to jedna klasa posiadająca następujące metody:

* \verb|__init__(self, eps, min_points)| - konstruktor posiadający następujące parametry:
    * \verb|eps| - wartość promienia obszaru definiującego sąsiedztwo punktu,
    * \verb|min_points| - wymagana ilość punktów w sąsiedztwie danego punktu aby można było uznać go za rdzeń,
* \verb|fit(self, data)| - metoda uruchamiająca algorytm na danych przekazanych przez parameter \verb|data|,
* \verb|__del__(self)| - destruktor zwalniający zaalokowaną pamięć.

\noindent Klasa ta posiada także pole \verb|labels| pozwalające na dostęp do informacji o przypisaniu punktów ze zbioru danych 
do klastrów lub ustaleniu czy punkt jest wartością odstającą.

\noindent Interfejs algorytmu DBSCAN w języku C++ jest bardzo podobny do interfejsu w języku Python. 
Dodatkowym elementem jest struktura o nazwie \verb|DBSCAN_State|. Struktura ta jest wykorzystywana do przekazywania informacji o 
wykonywanym algorytmie między funkcjami. Posiada ona następujące pola:

* \verb|float32 eps| - wartość promienia obszaru definiującego sąsiedztwo punktu,
* \verb|int minPoints| - wymagana ilość punktów w sąsiedztwie danego punktu aby można było uznać go za rdzeń,
* \verb|int sampleCount| - ilość punktów w zbiorze danych,
* \verb|int sampleSize| - wymiarowość punktów w zbiorze danych,
* \verb|int *labels| - wskaźnik do listy z indeksami klastrów przypisanych do poszczególnych punktów ze zbioru danych, 
* \verb|float32 distanceBuffer[8]| - bufor używany w trakcie wyliczania dystansu do punktu,
* \verb|__m256 epsSIMD| - rejestr SIMD zawierający na wszystkich 8 miejscach wartość parametru \verb|eps|.

\noindent Funkcje zaimplementowane w języku C++ są następujące:

* \verb|void *DBSCANInit( float32 eps, int minPoints )| - funkcja tworząca i zwracająca instancję struktury \verb|DBSCAN_State|. 
  Funkcja ta jest wywoływana przez konstruktor w języku Python oraz posiada identyczne parametry,
* \verb|int *DBSCANFit( DBSCAN_State *state, float32 *data,|\\
\verb|int sampleCount, int sampleSize )| - funkcja wywoływana przez 
  metodę \verb|fit| w języku Python. Jej parametry to:
    * \verb|DBSCAN_State *state| - wskaźnik do instancji struktury\\ \verb|DBSCAN_State| stworzonej przez funkcję \verb|DBSCANInit|,
    * \verb|float32 *data| - wskaźnik do listy zawierajacej zbiór danych,
    * \verb|int sampleCount| - ilość punktów w zbiorze danych,
    * \verb|int sampleSize| - wymiarowość punktów w zbiorze danych,
* \verb|void DBSCANCleanup( DBSCAN_State *state )| - funkcja wywoływana przez destruktor w języku Python.

### Zastosowane techniki oraz wyniki pomiarów
W przypadku algorytmu DBSCAN udało się osiągnąć zadowalające wyniki stosując następujące techniki:

* Usunięcie zbędnych abstrakcji oraz brak zależności w postaci zewnętrznych bibliotek.
* Pisanie możliwie najprostszego kodu - cała implementacja algorytmu DBSCAN ma zaledwie 180 linii kodu (wliczając w to 
  puste linie oraz linie z pojedyńczymi nawiasami klamrowymi).
* Użycie SIMD do obliczania dystansów między punktami - dokładny opis użycia SIMD znajduje się poniżej.
* Minimalizacja ilości alokacji pamięci - zastosowanie pomocniczej struktury o nazwie \verb|Neighbors| służącej do przechowywania 
  sąsiadów danego punktu pozwoliło na minimalizację ilości alokacji pamięci. Zamiast alokować pamięć na sąsiadów przy każdym 
  wyszukiwaniu, resetowany był tylko licznik sąsiadów co pozwalało na ponowne wykorzystanie zaalokowanej pamięci do następnego 
  wyszukiwania. Struktura \verb|Neighbors| ma nastepujące pola:
    * \verb|int maxNeighbors| - maksymalna ilość sąsiadów możliwych do przechowania w strukturze, po przekroczeniu tej wartości lista 
      sąsiadów jest rozszerzana o kolejne 8 pozycji,
    * \verb|int *neighbors| - wskaźnik do listy zawierającej sąsiadów danego punktu,
    * \verb|int neighborsCount| - aktualna liczba sąsiadów przechowywana w liście.
  

\noindent Podobnie jak w przypadku grupowania hierarchicznego w algorytmie DBSCAN mierzony był całkowity czas wykonywania algorytmu.
Dla każdej wartości punktów w zbiorze danych wykonane zostały trzy pomiary, które nastepnie zostały uśrednione aby otrzymać ostateczny 
wynik. W tabelach \ref{dbscan_time_custom} i \ref{dbscan_time_sklearn} przedstawiono wyniki pomiarów. Na 
rysunku @dbscan_time przedstawiono wyniki pomiarów w postaci wykresu punktowego.

\begin{table}[h]
    \begin{center}
    \begin{tblr}{ hlines, vlines, colspec={X[2.5,c] X[1,l] X[1,l] X[1,l] X[1,l] X[1,l]}}
        **Ilość punktów** & 5000 & 10000 & 15000 & 20000 & 25000 \\ 
        
        **Czas\\wykonywania (ms)** & 25.7 & 384.8 & 1788.0 & 5416.0 & 12950.0 \\ 
        
    \end{tblr}

    \caption{Wyniki pomiarów własnej implementacji algorytmu DBSCAN}
    \label{dbscan_time_custom}
    \end{center}
\end{table}

\begin{table}[h]
    \begin{center}
    \begin{tblr}{ hlines, vlines, colspec={X[2.5,c] X[1,l] X[1,l] X[1,l] X[1,l] X[1,l]}}
        **Ilość punktów** & 5000 & 10000 & 15000 & 20000 & 25000 \\ 
        
        **Czas\\wykonywania (ms)** & 231.1 & 939.0 & 2152.0 & 3661.0 & 5527.0 \\ 
        
    \end{tblr}

    \caption{Wyniki pomiarów algorytmu DBSCAN z pakietu scikit-learn}
    \label{dbscan_time_sklearn}
    \end{center}
\end{table}
@dbscan_time Pomiary czasu wykonywania algorytmu DBSCAN
\clearpage

\noindent W implementacji algorytmu DBSCAN do obliczania dystansu między punktami zastosowano SIMD. Pozwoliło to na jednoczesne obliczanie 
ośmiu dystansów i sprawdzanie czy punkt jest w sąsiedztwie innego punktu. Do użycia SIMD dodatkowo wymagana była transformacja danych 
tak aby lista punktów miała nastepującą postać: $x_1, x_2, ... x_n, y_1, y_2, ... y_n$ gdzie $x_1$ to współrzędna $x$ pierwszego punktu 
oraz $y_1$ to współrzędna $y$ pierwszego punktu itd. Na poniższym listingu przedstawiono funkcję odpowiedzialną za obliczanie dystansu 
między punktami oraz ustalanie czy punkty te są w sąsiedztwie danego punktu.\\

\begin{lstlisting}[language=C++]
!inline void DistanceSIMD( DBSCAN_State *state, float32 *data, 
!                          int aIndex, int pointsOffset, 
!                          int sampleCount, int sampleSize )
!{
!    // Obliczenie dystansu
!    __m256 result = _mm256_set1_ps( 0.0f );
!    for ( int i = 0; i < sampleSize; ++i )
!    {
!        float32 aValue = ( data + aIndex )[ i * sampleCount ];
!        __m256 pointA = _mm256_set1_ps( aValue );
!        float32 *p = data + pointsOffset + i * sampleCount;
!        __m256 points = _mm256_load_ps( p );
!        __m256 subtract = _mm256_sub_ps( pointA, points );
!        __m256 square = _mm256_mul_ps( subtract, subtract );
!        result = _mm256_add_ps( result, square );
!    }
!
!    // Sprawdzenie czy punkty sa w sasiedztwie
!    result = _mm256_cmp_ps( result, state->epsSIMD, 
!                            _CMP_LE_OS );
!    _mm256_store_ps( state->distanceBuffer, result );
!}
\end{lstlisting}

\noindent W tabeli \ref{dbscan_simd_table} przedstawiono pomiary czasu wykonywania algorytmu DBSCAN bez użycia SIMD, natomiast na 
rysunku @dbscan_simd przedstawiono porównanie obu sposobów obliczania dystansu.

\begin{table}[h]
    \begin{center}
    \begin{tblr}{ hlines, vlines, colspec={X[2.5,c] X[1,l] X[1,l] X[1,l] X[1,l] X[1,l]}}
        **Ilość punktów** & 5000 & 10000 & 15000 & 20000 & 25000 \\ 
        
        **Czas\\wykonywania (ms)** & 45.2 & 449.7 & 1893.0 & 5588.0 & 13145.0 \\ 
        
    \end{tblr}

    \caption{Wyniki pomiarów algorytmu DBSCAN bez użycia SIMD}
    \label{dbscan_simd_table}
    \end{center}
\end{table}

@dbscan_simd Porównanie sposobów obliczania dystansu
\clearpage

# Wnioski
Oprócz algorytmu grupowania hierarchicznego własne implementacje algorytmów opisanych w tej pracy okazały się sukcesem. 

\noindent Najlepiej wypadł algorytm K-Means, który okazał się znacząco szybszy od implementacji w pakiecie scikit-learn. Mimo to nie 
jest to jeszcze najszybsza możliwa implementacja tego algorytmu. Do potencjalnych usprawnień można zaliczyć:

* Zrównoleglenie obliczeń - równolegle można wyliczać centroidy lub przypisywać punkty do nowych klastrów.
* Użycie SIMD - podobnie jak w algorytmie DBSCAN można zastosować SIMD do obliczania dystansów między punktami a centroidami.

\noindent Na drugim miejscu uplasował się algorytm DBSCAN. Jego implementacja okazała się być szybsza ale tylko dla małych ilości danych. Dla 
zbioru danych posiadającego więcej niż 15000 punktów implementacja w pakiecie scikit-learn była szybsza. Spowodowane było to tym, że 
scikit-learn oblicza tablicę dystansów między punktami, dzięki czemu szybciej znajduje sąsiadów punktu. Do potencjalnych usprawnień 
algorytmu DBSCAN można zaliczyć:

* Użycie tablicy z wyliczonymi dystansami między punktami.
* Zmiana układu danych w pamięci - aktualny układ danych w pamięci w postaci $x_1, x_2, ... x_n, y_1, y_2, ... y_n$ nie był w stu 
  procentach optymalny. Aby SIMD działało jak najlepiej układ danych powinien mieć następującą postać: 
  $x_1, x_2, ... x_8, y_1, y_2, ... y_8, x_9, x_{10} ...x_{16}, y_9, y_{10} ... y_{16}$.

\noindent Najgorzej wypadł algorytm grupowania hierarchicznego, który okazał się znacząco wolniejszy od implementacji w pakiecie 
scikit-learn. Pomimo tego jego implementacja była wartościowa, ponieważ pozwoliła zaobserwować działanie pamięci podręcznej procesora 
i pokazała jak ważny jest układ danych w pamięci oraz sposób odczytywania danych. Do potencjalnych usprawnień algorytmu grupowania 
hierarchicznego można zaliczyć:

* Zrównoleglenie obliczania dystansu oraz wyszukiwania najmniejszego dystansu między klastrami.
* Zastosowanie struktury optymalizującej wyszukiwanie najmniejszego dystansu, aby pozbyć się konieczności przeszukiwania wszystkich 
  dystansów w każdej iteracji algorytmu.


