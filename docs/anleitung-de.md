% Anleitung zum "Quiz-Board"
% Joerg Desch (https://github.com/joede/quiz-board)
% Revision R.2

# Über das Dokument

In diesem Dokument soll die Funktionsweise des "Quiz-Board" aus Sicht
eines Benutzer beschrieben werden. Es soll als Grundlage für die Entwicklung
und in Grenzen als Dokumentation für den Betrieb genutzt werden.

Dieses Dokument wird keine technischen Details beschreiben, die über den
reinen Betrieb / die Benutzung hinausgehen.


# Ziel des Projekt

Das Projekt "Quiz-Board" soll ein von einem Microcontroller gesteuertes
Quiz realisieren. In diesem Quiz werden dem Befragten verschiedene Fragen
mit möglichen Antworten präsentiert. Diese Fragen gilt es zu beantworten.
Das Resultat jeder Antwort wird umgehend angezeigt. Sind alle Fragen
beantwortet, wird das Gesamtergebnis visualisiert.

## Einschränkungen

Zur Zeit sind folgende Einschränkungen vorhanden:

* jede Frage muss immer vier Antworten haben
* ein Quiz besteht immer aus genau acht Fragen
* die optionale Maximaldauer liegt fix bei drei Minuten.


# Details zur Umsetzung

Die Fragen werden zusammen mit den Antworten in Textform als Papierkarten an
einem Board befestigt. Neben der Frage befinden sich vier fest installierte
Taster. Jeder der Taster repräsentiert somit eine mögliche Antwort. Daneben
sind zwei Leuchtdioden (im folgenden als *LED* bezeichnet), die das Ergebnis
der Antwort anzeigen.

Die Anzahl der möglichen Fragen ist zur Zeit auf acht festgelegt. Die oben
beschriebenen Blöcke aus Fragekarte, Taster und LEDs werden vertikal
untereinander angeordnet. Diese "Zeilen" bilden dann das komplette Quiz.

![Skizze der Anordnung der Elemente des Board. Die Farbgebung ist eine
Möglichkeit die Fragen und die LED zu gruppieren. Auf die Darstellung der Taster
in dem grau hinterlegten Feld wurde in der Skizze verzichtet.](images/Puzzleboard-Sample.eps)

Eine Reihenfolge beim Beantworten der Fragen muss *nicht* eingehalten werden.

Abseits dieser Tasten und LEDs befindet sich der Start-Knopf. Dieser startet
ein neues Quiz. Ein laufender Durchgang wird hiermit ohne weitere Rückfrage
abgebrochen. Nach dem Abbruch ist ein erneutes Starten möglich.


## Abfrage einer Antwort

Für jede Antwort gibt es nur eine richtige Antwort. Die Antwort wird durch das
Drücken des jeweiligen Tasters neben der Frage eingegeben. Nach der Eingabe
der Antwort ist keine Änderung mehr möglich. Ein erneutes Drücken eines
Tasters einer bereits beantworteten Frage wird ignoriert.

Das Ergebnis wird umgehend angezeigt. Die richtige Antwort wird durch eine
grüne LED angezeigt, die Falsche durch eine rote LED. Die jeweilige LED leuchtet
dauerhaft.


## Betriebsmodus und dessen Anzeige

Der Betrieb der Anlage wird durch eine gelbe LED angezeigt. Diese "Status" LED zeigt
durch ihr Blinken den jeweiligen Betriebszustand an.

* ist die dauerhaft LED an, befindet sich das System im Bereitschaftmodus (Idle).
* blinkt die LED, dann ist ein Quiz am laufen.
* ist die LED aus, so ist auch das System ausgeschaltet.

**Hinweis:** das System verfügt über einen eigenen Netzschalter, mit dem die
Spannungsversorgung komplett getrennt werden kann. Siehe Abschnitt
*"Spannungsversorgung"*.


### Zustand "Idle"

Nach dem Einschalten wartet das System auf den Start eines Quiz (Idle). Um
dies anzuzeigen, ist die gelbe LED dauerhaft an.


<!-- ZRU ZEIT NICHT UMGESETZT
### Zustand "Stromsparen"

Wird im Zustand "Idle" nicht innerhalb einer Minute das Quiz gestartet, schaltet
sich das System wieder aus. Bei einem laufenden Quiz erfolgt diese Abschaltung
spätestens fünf Minuten nach dem letzten Tastendruck.

Abschalten bedeutet hier, dass das System in einen speziellen "Stromspar-Modus"
wechselt. Hierzu wird der Microcontroller in einer Schlafmodus geschaltet, in
dem er nahezu keine Energie benötigt. Aus diesem "Stromspar-Modus" kann direkt
ein Quiz gestartet werden.

Dieses Abschalten wird nicht visualisiert. Da Energie gespart werden soll, wird
keine LED zur Anzeige verwendet.
-->


### Zustand "Abfrage"

Der Zustand "Abfrage" ist der normale "laufende Betrieb". Das bedeutet, dass
das Quiz aktiv ist. Dieser Zustand wird durch eine gleichmäßig (1:1)
blinkende gelbe LED anzeigt.

Wenn die *Maximaldauer* für das Quiz aktiviert ist, beginnt die gelbe LED ca.
30% vor Ablauf der Maximaldauer schneller zu blinken. Dies ist aber nur dezent
umgesetzt, sodass es den Anwender nicht stören sollte.

> **Hinweis:** Zur Zeit ist diese Funktionalität nicht aktiviert! Die Implementation
> sieht eine maximale Laufzeit von 3 Minuten zur Beanwortung der 8 Fragen vor.
> Dies bedeutet, dass im Schnitt 22 Sekunden pro Frage möglich sind. Nach
> 2 Minuten beginnt die LED schneller zu blinken.


### Zustand "Ergebnis"

Ist die letzte Frage beantwortet, wird in den Zustand "Ergebnis" gewechselt.
Die Funktion dieses Zustands ist einzig die Anzeige des Resultats.

> **Hinweis:** Zur Zeit ist die Animation des Ergebnisses nicht aktiviert!
> Für den Fall einer aktiven Animation, ergibt sich folgendes Bild. Sind alle
> Fragen richtig beantwortet, fangen alle grünen LED an im Gleichtakt (1:1)
> zu blinken. Sind nicht alle Antworten korrekt, blinken alle roten LED im
> Gleichtakt (1:1).

Die Anzeige des Resultats erfolgt maximal für 30 Sekunden. Danach wechselt das
System automatisch in den Zustand "Idle".


### Neustarten

Im Prinzip lässt sich das System in jedem Zustand mit der START-Taste erneut
starten. Ist zum Zeitpunkt des Neustarts eine Abfrage aktiv, wird diese mit dem
ersten Tastendruck auf START zuerst abgebrochen. Das System wechselt in den
Zustand "Ergebnis", sodass dem Benutzer noch Zeit bleibt, seine bisherigen
Antworten zu begutachten.

# Die Spannungsversorgung

Das System wird wahlweise mit einem Steckernetzteil oder einem Batterieblock mit
Akkus betrieben. Die Spannung wird über einen eigenen Kippschalter geschaltet,
sodass sich das System komplett von der Spannungsversorgung trennen lässt.


# Historie des Dokuments

**draft-1**
:    erster überarbeiter Entwurf des Dokuments. (10.5.2013/jd)

**R.1**
:    Freigabe der hier beschriebenen Funktionalität. Die Umsetzung der
     erweiterten Funktionen sind optional und hängen von der zur Verfügung
     stehenden Zeit ab. (26.5.2013/jd)

**R.2**
:    Version 1.0 der Firmware fertiggestellt.
