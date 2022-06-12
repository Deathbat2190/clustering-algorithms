\chapter*{Podsumowanie}
\addcontentsline{toc}{chapter}{Podsumowanie}

#* Osiągnięte cele
W tej pracy głównym celem była implementacja algorytmów działających szybciej niż te dostępne w pakiecie scikit-learn. Cel ten został 
osiągnięty w przypadku algorytmów K-Means oraz DBSCAN. Niestety w algorytmie grupowania hierarchicznego nie udało osiągnąć się głównego 
celu, ponieważ implementacja tego algorytmu w pakiecie scikit-learn okazała się być szybsza.

Dodatkowym celem w tej pracy było także pokazanie, że programowanie niskopoziomowe, wbrew powszechnej opinii, nie jest trudne i 
przynosi ono znaczące korzyści w postaci wydajności oraz solidności programu. Odpowiedź na pytanie czy ten cel został osiągnięty należy 
do czytelnika tej pracy.

#* Plany na przyszłość
Zaimplementowane algorytmy można ulepszyć na wiele sposobów np.:

* Zrównoleglenie obliczeń na CPU - wszystkie z implementowanych algorytmów mają potencjał do zrównoleglenia
* Zastosowanie SIMD w algorytmie K-means
* Poprawienie wydajności algorytmu grupowania hierarchicznego
* Przeniesienie obliczeń na GPU

\noindent W przyszłości można będzie też rozszerzyć zbiór algorytmów i stworzyć pakiet oferujący szybkie i łatwe do zrozumienia 
implementacje algorytmów grupowania.

