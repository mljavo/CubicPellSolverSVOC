# Cubic Pell Equation Solver (LLL)

Tento projekt implementuje efektívne hľadanie fundamentálnych jednotiek v kubicky rozšírených poliach $\mathbb{Q}(\sqrt[3]{c})$ pomocou algoritmu mriežkovej redukcie (LLL).

## Požiadavky (Dependencies)
Pre úspešnú kompiláciu je potrebné mať v systéme nainštalované nasledujúce knižnice:
1. **GMP (GNU Multi-Precision Library)** – potrebná pre prácu s veľkými číslami.
2. **NTL (Number Theory Library)** – kľúčová knižnica pre algoritmus LLL a reálnu aritmetiku s vysokou presnosťou.

*Poznámka: Makefile je nastavený tak, aby hľadal NTL v podpriecinku `./Utilities/libntl/`. Ak máte knižnice nainštalované systémovo, upravte `LDFLAGS` a `CXXFLAGS` v Makefile.*

## Štruktúra projektu
- `CubicSolver/` – Obsahuje triedy `CubicSolver`, ktory brute-force hľadá riešenia a `CubicSolverLLL`, ktorý transformuje problém na SVP.
- `CubicNumber/` – Algebraická reprezentácia prvkov $x + y\sqrt[3]{c} + z\sqrt[3]{c^2}$.
- `BigInt/` – Wrapper pre prácu s veľkými celými číslami.
- `main.cpp` – Vstupný bod programu, kde sa definujú parametre experimentu.

## Kompilácia a spustenie
Projekt skompilujete pomocou priloženého `Makefile`:

```bash
make
```
