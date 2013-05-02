% Anleitung zum "Puzzle-Board"
% Joerg Desch (https://github.com/joede/puzzle-board)
% draft-1

# Über das Dokument

In diesem Dokument soll die Funktionsweise des "Puzzle-Board" aus Sicht
eines Benutzer beschrieben werden. Es soll als Grundlage für die Entwicklung
und in Grenzen als Dokumentation für den Betrieb genutzt werden.

Dieses Dokument wird keine technischen Details beschreiben, die über den
reinen Betrieb / die Benutzung hinausgehen.


# Ziel des Projekt

Das Projekt "Puzzle-Board" soll ein von einem Microcontroller gesteuertes
Quiz realisieren. In diesem Quiz werden dem Befragten verschiedene Fragen
mit möglichen Antworten präsentiert. Diese Fragen gilt es zu beantworten.
Das Resultat jeder Antwort wird umgehend angezeigt. Sind alle Fragen
beantwortet, wird das Gesamtergebnis visualisiert.

## Einschränkungen

Zur Zeit sind folgende Einschränkungen vorhanden:

* jede Frage muss immer vier Antworten haben
* ein Quiz besteht immer aus genau acht Fragen
* die optionale Maximaldauer liegt fix bei zwei Minuten.


# Details zur Umsetzung

Die Fragen werden zusammen mit den Antworten in Textform als Papierkarten an
einem Board befestigt. Neben der Frage befinden sich vier festinstallierte
Taster. Jeder der Taster repräsentiert somit eine mögliche Antwort. Daneben
sind zwei Leuchtdioden (im folgenden als *LED* bezeichnet), die das Ergebnis
der Antwort anzeigen.

Die Anzahl der möglichen Fragen ist zur Zeit auf acht festgelegt. Die oben
beschriebenen Blöcke aus Fragenkarte, Taster und LEDs werden vertikal
untereinander angeordnet. Diese "Zeilen" bilden dann das komplette Quiz.

Eine Reihenfolge beim Beantorten der Fragen muss *nicht* eingehalten werden.

Abseits dieser Tasten und LEDs befindet sich der Start-Knopf. Dieser startet
ein neues Quiz. Ein laufender Durchgang wird hiermit ohne weitere Rückfrage
abgebrochen.

## Abfrage einer Antwort

Für jede Antwort gibt es nur eine richtige Antwort. Die Antwort wird durch das
Drücken des jeweiligen Tasters neben der Frage eingegeben. Nach der Eingabe
der Antwort ist keine Änderung mehr möglich. Ein erneutes Drücken eines
Tasters einer bereits beantworteten Frage wird ignoriert.

Das Ergebnis wird umgehend angezeigt. Die richtige Antwort wird durch eine
grüne LED angezeigt, die Falsche durch eine rote LED. Die jeweilige LED leuchtet
dauerhaft.

## Betriebsmodus und dessen Anzeige

Der Betrieb der Anlage wird durch eine gelbe LED angezeigt. Diese LED zeigt
durch ihr Blinken den jeweiligen Betriebszustand an. Ist die LED aus, so ist
auch das System ausgeschaltet.

**Hinweis:** das System verfügt über einen eigenen Netzschalter, mit dem die
Spannungsversorgung komplett getrennt werden kann. Siehe Abschnitt
*"Spannungsversorgung"*.

### Zustand "Idle"

Nach dem Einschalten wartet das System auf den Start eines Quiz (Idle). Um
dies anzuzeigen, blinkt die gelbe LED. Damit dies nicht zuviel Batterieladung
verbraucht, blinkt die LED zu kurz auf. Es wird ein 1:5 Verhältnis verwendet.
Die LED ist für 1/5 Sekunde an und für 4/5 Sekunden aus.

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

### Zustand "Abfrage"

Der Zustand "Abfrage" ist der normale "laufende Betrieb". Das bedeutet, dass
das Quiz aktiv ist. Dieser Zustand wird durch eine gleichmäßig (1:1)
blinkende gelbe LED anzeigt.

Wenn die maximale Zeit für das Quiz aktiviert ist, beginnt die gelbe LED ca.
15% vor Ablauf der Maximaldauer schneller zu blinken. Die Puls-Pausen-Zeit
wechselt von einer Sekunde auf eine halbe Sekunde.

> **Hinweis:** Zur Zeit ist noch nicht definiert, wie eine Maximaldauer verändert
> werden kann. Aus dem Grund beträgt die Vorgabe zwei Minuten. Somit beginnt die
> LED 18 Sekunden (15%) vor dem Ablaufen an schneller zu blinken.

### Zustand "Ergebnis"

Ist die letzte Frage beantwortet, wird in den Zustand "Ergebnis" gewechselt.
Die Funktion dieses Zustands ist einzig die Anzeige des Resultats. Sind alle
Fragen richtig beantwortet, fangen alle grünen LED an im Gleichtakt (1:1)
zu blinken.

Sind nicht alle Antworten korrekt, blinken alle roten LED im Gleichtakt (1:1).

Die Anzeige des Resultats erfolgt maximal für 30 Sekunden. Danach wird das
System automatisch abgeschaltet (Zustand "Stromsparen").

### Neustarten

In jedem Zustand läßt sich das System mit der START-Taste erneut starten.
Eventuell laufende Abfragen werden abgebrochen. Bei aktiver Zeitüberwachung
beginnt die Zeitzählung sofort nach diesem Start.


# Die Spannungsversorgung

Das System wahlweise mit einer Steckernetzteil oder einem Batterieblock mit
Akkus betrieben. Die Spannung wird über einen eigenen Kippschalter geschaltet,
sodass sich das System komplett von der Spannungsversorgung trennen läßt.

Bei eingeschalteten System verfällt der Microcontroller bei Inaktivität in einen
Stromsparmodus, in dem die Batterie fast nicht entladen wird.


# Historie des Dokuments

**draft-1**
:    (2.5.2013) -- erster Entwurf des Dokuments
