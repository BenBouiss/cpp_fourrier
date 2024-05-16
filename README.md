# Installation

Installation nécessaire de Open Frameworks: https://openframeworks.cc/
  -Installation sur linux: https://openframeworks.cc/setup/linux-install/


Une fois l'archive téléchargé et décompressé, ce git repo doit être cloné dans la structure suivante of_untar_folder/examples/sound/. Git clone git@github.com:BenBouiss/cpp_fourrier.git à lancé ici.

Lancé make dans le git cloné puis make run pour lancé l'application.
# Synthétiseur


Pour les notes voir les références : 
https://fr.wikipedia.org/wiki/Fr%C3%A9quences_des_touches_du_piano
Douze notes. Sept pour les touches blanches : Do, Ré, Mi, Fa, Sol, La, Si, et cinq pour les touches noires : Do#, Ré#, Fa#, Sol#, La#. 
On considère 12 notes par octave numérotées de 0 pour Do à 11 pour Si
L'octave 0 est la plus grave. 
<br> Le La4 ou "A4" correspond à une fréquence de 440 Hz et sera ici dénottée pitch numéro 57.

|Note |Notation gb |Octave 0| 1  | 2 | 3 | 4 | 5 |
|---|---|  ---| ---| --- | --- |--- |--- |
|Do |C | 0 | 12 | 24| 36 | 48 | 60 |
|Do# | | 1 | | |  |  |  |
|Ré |D | 2 | | |  |  |  |
|Ré# | | 3 | | |  |  |  |
|Mi | E| 4 | | |  |  |  |
|Fa | F| 5 | | |  |  |  |
|Fa# | | 6 | | |  |  |  |
|Sol | G| 7 | | |  |  |  |
|Sol# | | 8 | | |  |  |  |
|La | A| 9 | | |  | 57 |  |
|La# | | 10 | | |  |  |  |
|Si | B | 11 | | |  |  |  |

Touches sur le clavier et notes correspondantes: 
| Touches clavier | Note |
|--- |--- |
|q|Do |
|z|Do# | 
|s|Ré |
|e |Ré# | 
|d |Mi | 
|f|Fa | 
|t|Fa# | 
|g|Sol | 
|y|Sol# | 
|h|La | 
|u|La# | 
|j|fzuSi |

Autres commandes sur les touches : 
| Touches clavier | Action |
|--- |--- |
|+| Volume up |
|-| Volume down |
| w| Octave up |
|x| Octave down |
|*| start|
|/| stop|

(Nadia)
| Touches clavier | fonction |
|m| brillance +|
|n| brillance -|
|0| switch entre sin/saw/square wave|

Filtre bande passante, synthèse soustractive (Benjamin)

| Touches clavier | fonction |
|--- |--- |
|o(lettre)|Reset pass filter (variables)|
|1/ctrl+1| omega0 +1/-1|
|3| switch filters lpf / lpf + hpf / hpf / rien|
|4/ctrl+4| quality +1/-1|
|5| hold pour écouter le son filtré|

