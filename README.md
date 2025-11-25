# ft_ping 🇫🇷

PI : Ce projet a été réalisé à deux pour gagner du temps et se faire plaisir sur un petit projet en C post-tronc commun, pas pour le moins inintéressant.

Parsing : Charles Poulain @CodeWithCharles
Protocole ICMP : @me

## Le protocole ICMP

Ce que je retiens du protocole ICMP dans le cadre de ce projet : 

ICMP pour Internet Control Message Protocol, n'est pas un protocole "au-dessus" d'IP (Internet Protocol), il fait partie de la couche IP elle-même.

ICMP sert à fournir des retours d’information sur les problèmes dans un environnement de communication, par exemple, lorsqu’un datagramme ne peut atteindre sa destination, quand une passerelle manque de capacité de buffer ou encore pour signaler une route plus courte.

**Point important:** IP n’est pas absolument fiable, ICMP ne le rend pas plus fiable, ce protocole se contente de signaler de potentiels problèmes, sans garantie de livraison.

## Ping et les messages echo

### Structure d'un message Echo Request / Reply 
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     TYPE      |     CODE      |           CHECKSUM            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         IDENTIFIER            |        SEQUENCE NUMBER        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           DATA...                             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

#### 1 - Type(8 bits / 1 octet):
 - Echo Request: Type = 8
 - Echo Reply: Type = 0
 - Set dans: build_icmp_echo()

#### 2 - Code(8 bits / 1 octet):
 - Toujours à 0 pour les messages Echo

#### 3 - Checksum(16 bits / 2 octets)
**C’est un morceau important là :**

**- Qu’est-ce que contient le checksum ?** 

  Le checksum est le complément à un sur 16 bits de la somme en complément à un du message ICMP commençant par le type ICMP.
  ---
  MAIS OUIIIIIIIIIIII C CLAIIIIIR

  En gros : Imaginons que le paquet ICMP (ce qu’envoie ping) est une grande liste de petits nombres. Le checksum, c’est comme un anti-cheat: on additionne tous les petits nombres, puis on trafique un peu le résultat (en rajoutant des zéros si impair typiquement) pour obtenir une valeur de 16 bits.

  Ensuite, le paquet arrive au destinataire, il refait la même addition, si sa somme matche la valeur envoyée, tout va bien, sinon, ça va pas, il y a eu problème

  Le but : s’assurer qu’un paquet n’est pas corrompu pendant son voyage. 
  ---

**- Comment calculer le checksum ?**
 
  1. Initialisation: On met le checksum à 0 dans le msg ICMP
  2. Traitement par mots de 16 bits: Traiter le message ICMP comme une séquence de mots de 16 bits (big endian, les plus gros bits au début).
  3. Gestion de longueur impaire: Si longueur totale impaire, les données reçues sont complétées avec un octet de zéro pour le calcul
  4. Addition en complément à un: 
    - On additionne tous les mots de 16 bits ensemble
    - Quand il y a un débordement (au-delà de 16 bits) on ajoute le reste au résultat (c’est ça le complément à un)
    - On continue jusqu’à additionner tous les mots.
  5. Complément à un final: Prendre le complément à un du résultat (inverse tous les bits)

#### Implementation en Code:

```
/// @brief Calcule le checksum 16 bits sur une zone memoire. Implementation classique ICMP (InternetChecksum)
/// @param buffer Pointeur vers la zone memoire a sum (void *) pour de la flexinilite. La memoire est interpretee comme une sequence d'octets
/// @param len Nombre d'octets  a prendre en compte dans le calcul
/// @return checksum 16 bits en complement a un (valeur a passer en en-tete)
uint16_t checksum(void *buffer, int len)
{
    uint32_t sum =0; // accumulateur 32 bits pour stocker les reports
    uint16_t *data = buffer; // a traiter par mots de 16 bits quand c'est possible
    
    while (len > 1)
    {
        sum += *data++; // ajoute le mot suivant et avance le pointeur
        len -= 2; // on a consomme 2 octets
    }
    // s'il reste un octet, on le traite en MSB d'un mot de 16 bits
    if (len == 1)
    {
        uint16_t last = 0;
        *(uint8_t*)&last = *(uint8_t*)data;
        sum += last;
    }

    // replier les reports de 32 bits vers 16 bits
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);
    // retourner le complement a un du resultat (format standard checksum internet)
    return ~sum;
}
```
# Points techniques à soulever pour la réalisation de ce projet:

#### Socket RAW:

On doit utiliser un socket RAW avec ==IPPROTO_ICMP==. => Privilèges root/admin obligatoire. 

#### Ordonnancement de byte:

Les champs multi-octets sont en **network byte order** (big endian). Utiliser ==htons()==, ==htonl()== et [compagnie](https://linux.die.net/man/3/htons)

#### Pour l’identifier et la Sequence: 

Implémenter un timeout sans utiliser ==select()== ou ==poll()== pour détecter les paquets perdus.

## Comportement du programme: 

1. Parse des arguments
2. Résolution de l’hôte -> ==resolve_target()==
3. Ouverture du socket RAW ICMP -> ==open_icmp_socket()==
4. Init des stats -> ==stats_init()==
5. Boucle principale (envoyer, recevoir, mettre à jour, afficher)
  -> Construction du paquet ICMP -> ==build_icmp_echo()==
    Calcul le checksum ==checksum()==
    Horodatage dans le payload
  -> Envoyer ==send_packet()==
  -> Attendre réception ==receive_packet()==
    Parse l’en-tête IP, calcul de l’ip_hlen, récupération du ICMP
    Set du retour (1,2,3)
    Récupération du timediff ==timeval_diff_ms()==
6. Print des stats.

# ft_ping 🇬🇧

PI: This project was carried out by two people to save time and have fun on a small C project after the common core, and it turned out to be quite interesting.

Parsing: Charles Poulain @CodeWithCharles
ICMP Protocol: @me

## The ICMP protocol

What I’m retaining from the ICMP protocol in the context of this project:

ICMP, Internet Control Message Protocol, is not a protocol “on top” of IP (Internet Protocol), it is part of the IP layer itself.

ICMP is used to provide feedback on problems in a communication environment, for example, when a datagram cannot reach its destination, when a gateway lacks buffer capacity, or to signal a shorter route.

**Important point:** IP is not absolutely reliable, ICMP does not make it more reliable, this protocol only reports potential problems, with no delivery guarantee.

## Ping and echo messages

### Structure of an Echo Request / Reply message
 0                   1                   2                   3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     TYPE      |     CODE      |           CHECKSUM            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         IDENTIFIER            |        SEQUENCE NUMBER        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                           DATA...                             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

#### 1 - Type (8 bits / 1 byte):
 - Echo Request: Type = 8
 - Echo Reply: Type = 0
 - Set in: build_icmp_echo()

#### 2 - Code (8 bits / 1 byte):
 - Always 0 for Echo messages

#### 3 - Checksum (16 bits / 2 bytes)
**This is an important part:**

**- What does the checksum contain?**

  The checksum is the one’s complement over 16 bits of the one’s complement sum of the ICMP message starting from the ICMP type.
  ---
  YES THANK YOU IT’S SO CLEAR

  Basically: imagine the ICMP packet (what ping sends) is a big list of small numbers. The checksum is like an anti-cheat: we add up all the little numbers, then tweak the result a bit (typically padding if odd length) to obtain a 16-bit value.

  Then the packet reaches the destination, it performs the same sum, if its result matches the checksum value sent, everything is fine, otherwise something went wrong.

  The goal: ensure that the packet was not corrupted during its journey.
  ---

**- How to compute the checksum?**

  1. Initialization: set the checksum to 0 in the ICMP message
  2. Process by 16-bit words: process the ICMP message as a sequence of 16-bit words (big endian, highest bits first)
  3. Odd length handling: if the total length is odd, complete with one zero byte for calculation
  4. One’s complement addition:
    - Add all 16-bit words together
    - When an overflow occurs (beyond 16 bits) add the carry to the result (that’s the one’s complement behavior)
    - Continue until all words are added
  5. Final one’s complement: take the one’s complement of the result (invert all the bits)

#### Implementation in Code:

```
/// @brief Computes the 16-bit checksum over a memory area. Classic ICMP implementation (InternetChecksum)
/// @param buffer Pointer to the memory area to sum (void *) for flexibility. Memory is interpreted as a byte sequence
/// @param len Number of bytes to include in the calculation
/// @return 16-bit checksum in one’s complement (value to insert into the header)
uint16_t checksum(void *buffer, int len)
{
uint32_t sum =0; // 32-bit accumulator to store carries
uint16_t *data = buffer; // processed as 16-bit words when possible

while (len > 1)
{
    sum += *data++; // add next word and advance pointer
    len -= 2; // consumed 2 bytes
}
// if one byte remains, treat as MSB of a 16-bit word
if (len == 1)
{
    uint16_t last = 0;
    *(uint8_t*)&last = *(uint8_t*)data;
    sum += last;
}

// fold carries from 32 bits to 16 bits
while (sum >> 16)
    sum = (sum & 0xFFFF) + (sum >> 16);
// return the one’s complement of the result (standard Internet checksum format)
return ~sum;

```

# Technical points to highlight for this project:

#### RAW Socket:

A RAW socket with ==IPPROTO_ICMP== must be used. => root/admin privileges required.

#### Byte order:

Multi-byte fields are in **network byte order** (big endian). Use ==htons()==, ==htonl()== and friends.

#### For the identifier and the sequence:

Implement a timeout without using ==select()== or ==poll()== to detect lost packets.

## Program behavior:

1. Parse arguments
2. Resolve host -> ==resolve_target()==
3. Open RAW ICMP socket -> ==open_icmp_socket()==
4. Init stats -> ==stats_init()==
5. Main loop (send, receive, update, display)
  -> Build ICMP packet -> ==build_icmp_echo()==
    Compute checksum ==checksum()==
    Timestamp inside payload
  -> Send ==send_packet()==
  -> Wait for reception ==receive_packet()==
    Parse IP header, compute ip_hlen, extract ICMP
    Set return code (1,2,3)
    Compute timediff ==timeval_diff_ms()==
6. Print stats.