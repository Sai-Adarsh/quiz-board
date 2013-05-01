% Anleitung zum "Puzzle-Board"
% Joerg Desch
% draft-1

# Über das Dokument

In diesem Dokument soll die Funktionsweise des "Puzzle-Board" aus Sicht eines
Benutzer beschrieben werden. Es soll als Grundlage für die Entwicklung und als
Dokumentation für den Betrieb genutzt werden.

Dieses Dokument wird keine technischen Details beschreiben, die über den
reinen Betrieb / die Benutzung hinausgehen.


# Ziel des Projekt

Das Projekt "Puzzle-Board" soll ein von einem Microcontroller gesteuertes
Quiz realisieren. In diesem Quiz werden dem Befragten Fragen mit möglichen
Antworten präsentiert. Diese Fragen gilt es zu beantworten. Das Resultat der
Anwort wird umgehend angezeigt. Sind alle Fragen beantwortet, wird das
Gesamtergebnis visualisiert.

## Einschränkungen

Zur Zeit sind folgende Einschränkungen vorhanden:

* jede Frage muss immer vier Antworten haben
* ein Quiz besteht immer aus genau acht Fragen


# Details zur Umsetzung

Die Fragen werden zusammen mit den Antworten in Textform auf Papierkarten an
einem Board befestigt. Neben der Frage befinden sich vier festinstallierte
Taster. Jeder der Taster repräsentiert somit eine mögliche Anwort. Daneben
sind zwei Leuchtdioden (im folgenden als *LED* bezeichnet), die das Ergebnis
der Anwort anzeigen.

Die Anzahl der möglichen Fragen ist zur Zeit auf acht festgelegt. Die oben
beschriebenen Blöcke aus Fragenkarte, Taster und LEDs werden vertikal
untereinander angeordnet. Diese "Zeilen" bilden dann das komplette Quiz.

## Abfrage einer Anwort

Für jede Anwort gibt es nur eine richtige Anwort. Die Anwort wird das
Drücken des jeweiligen Tasters neben der Frage eingegeben. Nach der Eingabe
der Antwort ist keine Änderung mehr möglich. Ein erneutes Drücken eines
Tasters einer bereits beantworteten Frage wird ignoriert.

Das Ergebnis wird umgehend angezeigt. Die richtige Antwort wird durch eine
grüne LED angezeigt, die Falsche durch eine rote LED.

## Betriebsmodus und dessen Anzeige

Der Betrieb der Anlage wird durch eine gelbe LED angezeigt. Diese LED zeigt
durch ihr Blinken den jeweiligen Betriebszustand an. Ist die LED aus, so ist
auch das System ausgeschaltet.

### Zustand "Idle"

Nach dem Einschalten wartet das System auf den Start eines Quiz (Idle). Um
dies anzuzeigen blinkt die gelbe LED. Damit dies nicht zuviel Batterieladung
verbraucht, blinkt die LED zu kurz auf. Es wird ein 1:5 Verhältnis verwendet.
Die LED ist für 1/5 Sekunde an und für 4/5 Sekunden aus.

Wir innerhalb einer Minute kein Quiz gestartet, schaltet sich das System wieder
aus. Hierzu welchselt es in einen speziellen "Stromspar-Modus".
