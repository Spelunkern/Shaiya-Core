# Shaiya-Core
Core is an open-source project designed to provide a universal Shaiya base for running private servers. This repository contains the code used by the server (PSGame), login (PSLogin), database agent (PSDBAgent), and the client (Game.exe).

As the spiritual successor to the Essentials project, Core aims to deliver a better development foundation — with fewer modifications and a focus on preserving the game’s original content.

### Getting Started
Please note that this repository is simply a merge of two existing projects (EP6 + Essentials) to offer a cleaner and easier-to-manage solution. If you prefer, you may compile each part separately from their original repositories.

### Server Code
The server-side code does not receive direct modifications in this project; it remains a strict copy of the EP6 project without a single line changed, offering a stable and immutable version. If you want to keep the latest code updates, please keep using EP6 repository code instead.
You can refer to the original server/login/agent code here:  https://github.com/kurtekat/shaiya-episode-6

### Client Code
The client-side code does receive small, controlled updates. The original Essentials repository remains archived as an immutable and stable reference.
You can refer to the original client code here:  https://github.com/Spelunkern/shaiya-essentials

---

⚠ 29 March Update ⚠

Shaiya Core Project currently is during a big overhaul with a lot of changes coming in the near future.
This repository can be considered an effective fork of the original with a new direction and goals.

An small overview:
- Episode 6 code is designed to be as close to the original as possible, Core have a production/final user goal rather than sticking with original game contents
- Core repository remains stable overtime, adding features and quality of life but keeping a safe and easy base to start with
- No bloat and complete transparency are some of the biggest goals of Core project, using clean executables and defining more things via code instead
This also provides more flexibility and final user practicity with some topics.
- Unlike previous usage of Essentials/Core/Episode6 , Shaiya Core new goal isn't about providing true support to EP6 features, but to use the EP6 client on standard 4.5/5.4 servers
This doesn't mean you can't use this project for a EP6 server, but the default parameters are now 5.4 or lower, cutting some EP6 content mostly when it comes to client side.
- Starting on Shaiya Core build v1.0 , named Eonar, the game.exe handles UTF-8 by default using an embedded manifiest.
This is sadly not enough to support proper vietnamese typing, but it covers everyone else letting the client to display any language and making the project more Universal.
