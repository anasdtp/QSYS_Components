# DE1 Basic Computer - Synthese Avalon MM (court)

## Synthese du projet
Ce projet suit le tutoriel "Making Qsys Components" (Quartus II 13.0) sur DE1. Il cree un composant Qsys personnalise: un registre 16 bits expose sur le bus **Avalon Memory-Mapped (MM)**. Les donnees du registre sont aussi exportees vers des afficheurs 7-segments via une interface **Conduit** (sortie directe hors Qsys).

## Architecture
NIOS II (maitre Avalon MM) -> interconnect Qsys -> reg16_avalon_interface (esclave) -> reg16 -> Q_export -> decodeurs 7 segments -> HEX0..HEX3.
![alt text](<doc/Qsys Component Avalon-2026-05-18-045403.svg>)

## Interfaces et signaux Avalon MM utilises

### Avalon MM - Slave (registre 16 bits)
Un **esclave** (peripherique) repond aux acces du **maitre** (CPU). Le CPU lit/ ecrit le registre comme une case memoire.

| Signal | Dir. | Largeur | Explication simple |
|---|---|---|---|
| `clock` | In | 1 | Horloge: cadence les changements d'etat (50 MHz) |
| `resetn` | In | 1 | Reset actif-bas: remet le registre a 0 |
| `chipselect` | In | 1 | Selection du peripherique |
| `read` | In | 1 | Demande de lecture |
| `write` | In | 1 | Demande d'ecriture |
| `readdata` | Out | 16 | Donnees lues du registre |
| `writedata` | In | 16 | Donnees a ecrire |
| `byteenable` | In | 2 | Active l'ecriture par octet |

**Ecriture (resume):** adresse + `writedata`, puis `chipselect=1` et `write=1`. Le registre capture les donnees au front montant de `clock`.

**Lecture (resume):** adresse, puis `chipselect=1` et `read=1`. Le registre place sa valeur sur `readdata`.

**`byteenable` (2 bits):** controle quels octets sont ecrits.
- `byteenable[0]=1`: ecriture bits 7..0 (octet bas)
- `byteenable[1]=1`: ecriture bits 15..8 (octet haut)

### Avalon Clock (clock_sink)
- `clk`: horloge d'entree pour synchroniser tout le composant.

### Avalon Reset (reset_sink)
- `reset_n`: reset actif-bas, force la valeur 0x0000.

### Avalon Conduit (export)
- `Q_export[15:0]`: sortie directe du registre vers l'exterieur de Qsys.
- Conduit = liaison simple, sans protocole de bus (pas de handshake).

## Composants internes (role rapide)
- `reg16`: registre 16 bits avec ecriture par octet.
- `reg16_avalon_interface`: adapte les signaux Avalon MM au registre interne.
- `hex7seg`: decodeur combinatoire pour afficher chaque nibble sur 7 segments.

## Exemple de flux (ecriture)
Le CPU ecrit 0xABCD a l'adresse du registre. Le bus Avalon MM active `write` + `chipselect`, puis le registre stocke 0xABCD au prochain front d'horloge. La valeur sort sur `Q_export` et s'affiche sur HEX.
   - `HEX2` (nibble 2) = décode 0xB → 7 segments
   - `HEX3` (nibble 3) = décode 0xA → 7 segments
   - **Affichage:** "ABCD" sur les 4 afficheurs

---

## Phases de Création du Composant

### Phase 1: Conception HDL
- ✅ Créer `reg16.vhd` (registre pur)
- ✅ Créer `reg16_avalon_interface.vhd` (wrapper Avalon MM)

### Phase 2: Configuration Qsys Component Editor
- ✅ Définir le nom: `reg16_avalon_interface`
- ✅ Ajouter les fichiers HDL
- ✅ Analyser les fichiers
- ✅ Configurer les signaux:
  - **Clock:** `clock` → interface "Clock Input" → signal "clk"
  - **Reset:** `resetn` → interface "Reset Input" → signal "reset_n"
  - **Avalon MM Slave:** auto-détection des signaux `read`, `write`, `readdata`, `writedata`, `chipselect`, `byteenable`
  - **Conduit:** `Q_export` → interface "Conduit" → signal "export"
- ✅ Associer clock et reset à l'interface Avalon MM

### Phase 3: Instantiation dans Qsys
- ✅ Ajouter le composant au système
- ✅ Connecter au CPU (data_master)
- ✅ Exporter l'interface Conduit: `to_hex`
- ✅ Assigner l'adresse: `0x00800000`

### Phase 4: Top-level VHDL (component_tutorial.vhd)
- ✅ Instantier `embedded_system` (généré par Qsys)
- ✅ Connecter horloge et reset physiques
- ✅ Connecter `to_hex_export` → décodeurs 7-segments

---

## Avantages de l'Interface Avalon MM

| Avantage | Description |
|----------|-------------|
| **Standardisé** | Tous les composants Qsys respectent le protocole |
| **Scalable** | Peut gérer plusieurs maîtres/esclaves via un arbitre |
| **Flexible** | Supporte lecture, écriture, accès par octet (byteenable) |
| **Synchrone** | Tous les signaux synchronisés sur l'horloge |
| **Facile d'intégration** | Qsys génère automatiquement l'interconnexion |

---

## Points Clés à Retenir

1. **Avalon MM Slave** = interface pour CPU/masters d'accéder à des ressources
2. **Signaux Avalon MM** = clock, reset, chipselect, read, write, readdata, writedata, byteenable
3. **byteenable** = contrôle granulaire (par octet) de l'écriture
4. **Q_export (Conduit)** = interface supplémentaire pour exporter les données hors Qsys
5. **Adressage** = aligné sur 32 bits, esclaves voient les bits d'adresse bas
6. **Synchronisation** = tous les changements sur fronts montants d'horloge

---

## Fichiers du Projet

```
DE1_Basic_Computer/
├── vhdl/
│   ├── reg16.vhd                       # Registre 16-bit pur
│   ├── reg16_avalon_interface.vhd      # Wrapper Avalon MM
│   ├── hex7seg.vhd                     # Décodeur 7-segments
│   ├── component_tutorial.vhd          # Top-level
│   ├── nios_system.qsys                # Système Qsys
│   └── nios_system/                    # Fichiers générés par Qsys
├── doc/
│   └── Qsys Component Avalon-*.mmd     # Schéma architecture
└── README.md                           # Ce fichier
```

---

## Ressources

- **Tutoriel:** Making Qsys Components for Quartus II 13.0 (Altera University Program, May 2013)
- **Spécifications:** Avalon Interface Specifications (Altera/Intel)
- **Plateforme:** Altera DE1-SoC ou DE2-115
- **Outil:** Quartus II 13.0, Qsys

---

**Version:** V0 - TP SoC Qsys Components  
**Date:** Mai 2026  
**Statut:** Tutoriel complété ✅
