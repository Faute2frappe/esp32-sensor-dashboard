# ESP32 Sensor Dashboard 🌡️

Un tableau de bord **open source** pour afficher en direct la température, l'humidité et la luminosité mesurées par un ESP32. Le tableau de bord est accessible depuis un téléphone ou ordinateur **sans connexion Internet** : l'ESP32 crée son propre réseau Wi-Fi.

> **Statut : prototype pédagogique non testé sur matériel réel.** Vérifiez le câblage et la compatibilité de votre carte avant utilisation.

## Fonctionnalités

- Interface web responsive (téléphone, PC)
- Actualisation automatique toutes les 3 secondes
- API JSON locale : `GET /api/sensors`
- Capteur DHT11 (température et humidité) et LDR (luminosité brute)
- Aucune clé API ni service cloud requis

## Matériel

- ESP32 **classique avec broche GPIO34** (par exemple ESP32 DevKit V1 ; adapter la broche selon votre carte)
- DHT11 et résistance de tirage si le module n'en intègre pas
- LDR et résistance de 10 kΩ pour constituer un pont diviseur
- Fils de connexion, câble USB

## Branchements (ESP32 classique)

| Élément | Connexion |
| --- | --- |
| DHT11 VCC | 3,3 V (si compatible avec le module) |
| DHT11 GND | GND |
| DHT11 DATA | GPIO4 |
| LDR | Entre 3,3 V et GPIO34 |
| Résistance 10 kΩ | Entre GPIO34 et GND |

⚠️ **Ne dépassez jamais 3,3 V sur les entrées de l'ESP32.** Une LDR ne se branche pas seule : le pont diviseur est nécessaire. Le DHT11 nécessite parfois une résistance de tirage sur DATA.

## Installation

1. Installer [Arduino IDE](https://www.arduino.cc/en/software) et le support de carte **ESP32 by Espressif Systems** via le gestionnaire de cartes.
2. Installer la bibliothèque **DHT sensor library** (Adafruit) et sa dépendance **Adafruit Unified Sensor** via le gestionnaire de bibliothèques.
3. Ouvrir `firmware/esp32_sensor_dashboard.ino`.
4. Sélectionner votre carte ESP32 et son port série, puis téléverser le programme.
5. Sur votre téléphone, rejoindre le Wi-Fi `ESP32-Sensors` (mot de passe de démonstration : `demo12345`).
6. Dans le navigateur, ouvrir **http://192.168.4.1**.

Le téléphone peut signaler « pas d'Internet », ce qui est normal : rester connecté au réseau ESP32.

## API

Exemple de réponse de `GET /api/sensors` :

```json
{"temperature": 22.3, "humidity": 48.0, "light_raw": 2048}
```

Une mesure DHT11 indisponible apparaît comme `null`. `light_raw` est une **valeur analogique brute** de 0 à 4095 : ce n'est pas une mesure en lux.

## Idées d'amélioration

- [ ] Ajouter des graphiques historiques
- [ ] Prévoir un mode client Wi-Fi configurable sans publier de mots de passe
- [ ] Ajouter d'autres capteurs (BH1750, ENS160…)
- [ ] Tester sur plusieurs modèles d'ESP32 et documenter les résultats
- [ ] Ajouter des tests automatisés de la logique de traitement

## Contribuer

Voir [CONTRIBUTING.md](CONTRIBUTING.md). Les retours, corrections et contributions sont bienvenus.

## Licence

MIT — voir [LICENSE](LICENSE).
