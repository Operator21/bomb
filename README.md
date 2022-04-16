# Bomba
[Youtube video prezentace](https://youtu.be/aIvJkMaMzTM)
## Cíl práce

Cílem práce je vytvořit propojení arduina s komponenty simulující výbušné zařízení typické pro dramatické filmy. Zařízení poskytuje rozhraní (tlačítka) pro nastavení času ve vteřinách, takto nastavený čas se zobrazuje na 7-segmentovém displeji. Další zásadní součástí je nastavení osmi místného hesla tvořeného kombinací 5 dalších tlačítek, toto heslo je vyžadováno pro spuštění &quot;výbušného procesu&quot;. Během každého stisku tlačítka je při tvorbě hesla rozsvícena ledka s danou pozicí pro jednodušší zapamatování kombinace. Po spuštění procesu se odpočítávají vteřiny až k nule, při dosažení nuly zařízení pípne a displej zobrazí text symbolizující výbuch. Před dosažením nuly je možné odpočet zrušit pomocí kombinace zadané při aktivaci, po zadání špatné kombinace je čas odpočtu snížen aby se zabránilo technice brute-force.


## Hardwarové zapojení

Díky modulu TM1638 je zapojení displeje, tlačítek a ledek výrazně zefektivněno, většina operací tedy pracuje s tímto modulem vyjímaje reproduktoru jehož zapojení je provedeno na nepájeném poli.

Zapojení pinů modulu je následující:

- STROBE - pin 4
- CLOCK - pin 6
- DIO - pin 7
- GND - GND
- Napájení - 5V (může být i 3,3V)

Zapojení reproduktoru:

- Plus - pin 3
- Mínus - rezistor 330 Ω -> GND


## Ovládání bomby

## V režimu vstupu

| S1 | Přidá 1 vteřinu |
| --- | --- |
| S1 + S4 | Přidá 10 vteřin |
| S2 | Odebere 1 vteřinu |
| S2 + S4 | Odebere 10 vteřin |
| S3 | Přepne program do režimu odpočtu (musí být nejprve zadána 8 místná kombinace tlačítek) |
| S4 až S8 | Uloží tlačítko do kombinace (Kombinaci tvoří 8 těchto libovolných tlačítek) |
| S7 + S8 | Resetování stavu - vteřiny se vrátí na 0 a kombinace se vymaže, musí být zadána nová |

## V režimu odpočtu

| S1 | x |
| --- | --- |
| S2 | x |
| S3 | Potvrzení pokusu o deaktivaci |
| S4 až S8 | Uložení tlačítka do deaktivační kombinace |

## Deaktivační kombinace

Jedná se o kód, který umožňuje aktivaci bomby a její následnou deaktivaci. Vždy je 8 místný a tvoří ho tlačítka S4, S5, S6, S7, S8 na TM modulu. Neplatí žádné omezení na použití tlačítek, může být tedy 8x stisknuto stejné tlačítko. Bez kombinace není možné spustit odpočet.

### Příklady kombinací:

S4, S4, S4, S4, S4, S4, S4, S4

S4, S5, S5, S6, S4, S7, S8, S5

S8, S7, S6, S5, S4, S5, S6, S7

## Struktura programu

Program se dělí na dva soubory mainSegment zahrnující hlavní část programu a \_password poskytující třídu pro práci s heslem. V obou souborech je využita knihovna TM1638 Plus, jenž poskytuje metody pro přístup k veškerým prvkům modulu. Soubor \_doom je pouze znělka ze hry Doom převedena na metodu (autorem předělávky do c++ je Robson Couto).

## mainSegment

Ze začátku jsou definovány konstanty určující pozici pinů modulu, dále proměnné uchovávající nastavený čas a stav programu. Nakonec jsou vytvořeny objekty z třídy Password a objekt z třídy TM1638plus pro ovládání modulu.

### setup()

Při spuštění programu je vyvolána metoda displayBegin, která zahájí komunikaci s periferiemi modulu.

### loop()

TM modul poskytuje metodu readButtons pro načtení hodnot tlačítek, pozice jsou dány mocninou dvojky, pro zachycení jednotlivých vstupů je tedy využit switch.

První dvě tlačítka vyvolávají metody, jenž upravují hodnotu proměnné seconds. Třetí tlačítko aktivuje výbušný stav, avšak pouze ve chvíli, kdy je nastavena aktivační kombinace. Zároveň při explozivním stavu vyvolává deaktivační metodu. Posledních pět tlačítek slouží k zadání kombinace v obou režimech.

V závěru cyklu se kontroluje stav programu, při explozi se snižuje počet vteřin, pro umožnění uživateli zadání kombinace během odpočtu je nutné mít časovou prodlevu nižší než 1 vteřina, hodnota 50 se v tomto případě ukázala jako ideální jak pro vstup tak pro přepočet vteřin bez větších odchylek. Úskálím tohoto řešení je typický problém s ukládáním čísla s desetinným místem v paměti, při vyšším počtu vteřin dochází k mírné odchylce.

Po uplynutí času se vyvolává samotná metoda simulující explozi. Nakonec se zobrazuje délka kombinace dle stavu programu.

### increaseTime()

Metoda přijímající dva parametry - časovou prodlevu operace a hodnota o kterou se počet vteřin zvýší. Dle zadaných hodnot je čas upraven a nakonec dochází k obnovení v displeje.

### decreaseTime()

Funkcionálně velice podobná navyšovací funkci, navíc se zde nachází podmínka, která zajišťuje, že zadané číslo bude vždy větší než nula.

### explode()

Metoda jenž vyvolává samotný efekt exploze, nejprve je nastaven čas na 0 aby se zabránilo případným chybám zaviněným desetinným číslem.

### reset()

Vyvoláním reset se program vrací do původního stavu - odpočty se vrací na 0, kombinace se vymažou, explozivní stav je deaktivován a nakonec se resetuje displej společně s ledkami.

### resetLeds()

Jednoduchá metoda která zhasne všechny ledky modulu.

### explosionSound()

Generovaná kombinace tónů v rozmezí 500 - 1000 Hz.

### inputCodeError()

Metoda, jenž zobrazí slovo CODE a pípne, vyvolává se pokud se uživatel snaží zapnout odpočet aniž by zadal celou kombinaci.

## \_password

V tomto souboru se nachází třída, jenž zajišťuje kompletní logiku kombinací, obsahuje 8 místné číselné pole a číselnou proměnnou, jenž ukládá aktuální pozici zadávání.

### Password()

Konstruktor třídy, jenž vyvolává metodu na zaplnění pole defaultní hodnotou.

### displayPasswordSize()

Metoda přijímající jako parametr TM modul, při použití zobrazuje na ledkách počet nastavených míst kombinace.

### setDefaultValues()

Při zavolání prochází všechny prvky pole a nastavuje jim hodnotu 0.

### checkPasswordSet()

Funkce kontrolující aktuální pozici ukazatele, je-li roven 8, vrací se hodnota true, v opačném případě false.

### setCurrentArrayIndexValue()

Nastavuje hodnotu na aktuální pozici ukazatele pole a posouvá ukazatel dále.

### compareWith()

Přijímá jako parametr další instanci třídy Password a v cyklu porovnává hodnoty kombinací obou instancí, při nalezení neekvivalentních hodnot je vráceno false, v případě že cyklus proběhne v pořádku, vrací se true.

## Závěr

Mezi největší problémy tohoto projektu se určitě řadila nemožnost asynchronního programování, čtení vstupu se zobrazováním odpočtu ve stejnou chvíli se tak stalo výraznou obtíží. Řešení se snížením časových prodlev a následného dopočítávání uběhlých vteřin se ukázalo jako optimální i přestože vedlo k mírným odchylkám v delších odpočtech.

Práce s rozšiřujícím modulem a knihovnou TM1638plus výrazně snížila množství potřebných komponent pro stavbu tohoto zapojení. Bzučák se ukázal jako dostačující pro základní pípání v odpočtu, pro efekty výbuchu už bohužel ne. I přestože 7 segmentový displej byl pro tuto simulaci ideální, měl svoje úskalí, především v práci s textem.

Celkově práce dosáhla svých stanovených cílů, uživatel může nastavovat čas a s pomocí kombinace spustit či deaktivovat odpočet, po uplynutí odpočtu je uživateli ohlášen výbuch. Při úspěšné deaktivaci je písničkou ohlášeno vítězství a bomba se vrací do původního stavu.
